//------------------------------------------------------------------------------
// <copyright file="testcmd.h" company="Atheros">
//    Copyright (c) 2004-2010 Atheros Corporation.  All rights reserved.
// 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6003
// </summary>
//
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#ifndef  TESTCMD_H_
#define  TESTCMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef AR6002_REV2
#define TCMD_MAX_RATES 12
#else
#define TCMD_MAX_RATES 28
#endif

#define WMI_CMD_ID_SIZE   4
#define WMI_CMDS_SIZE_MAX 2048
#define TC_CMDS_GAP       16
// should add up to the same size as buf[WMI_CMDS_SIZE_MAX]
//#define TC_CMDS_SIZE_MAX  (WMI_CMDS_SIZE_MAX - sizeof(TC_CMDS_HDR) - WMI_CMD_ID_SIZE - TC_CMDS_GAP)  
#define TC_CMDS_SIZE_MAX  256

typedef enum {
    ZEROES_PATTERN = 0,
    ONES_PATTERN,
    REPEATING_10,
    PN7_PATTERN,
    PN9_PATTERN,
    PN15_PATTERN
}TX_DATA_PATTERN;

/* Continous tx
   mode : TCMD_CONT_TX_OFF - Disabling continous tx
          TCMD_CONT_TX_SINE - Enable continuous unmodulated tx
          TCMD_CONT_TX_FRAME- Enable continuous modulated tx
   freq : Channel freq in Mhz. (e.g 2412 for channel 1 in 11 g)
dataRate: 0 - 1 Mbps
          1 - 2 Mbps
          2 - 5.5 Mbps
          3 - 11 Mbps
          4 - 6 Mbps
          5 - 9 Mbps
          6 - 12 Mbps
          7 - 18 Mbps
          8 - 24 Mbps
          9 - 36 Mbps
         10 - 28 Mbps
         11 - 54 Mbps
  txPwr: twice the Tx power in dBm, actual dBm values of [5 -11] for unmod Tx,
      [5-14] for mod Tx
antenna:  1 - one antenna
          2 - two antenna
Note : Enable/disable continuous tx test cmd works only when target is awake.
*/

typedef enum {
    TCMD_CONT_TX_OFF = 0,
    TCMD_CONT_TX_SINE,
    TCMD_CONT_TX_FRAME,
    TCMD_CONT_TX_TX99,
    TCMD_CONT_TX_TX100
} TCMD_CONT_TX_MODE;

typedef enum {
    TCMD_WLAN_MODE_NOHT = 0,
    TCMD_WLAN_MODE_HT20 = 1,
    TCMD_WLAN_MODE_HT40PLUS = 2,
    TCMD_WLAN_MODE_HT40MINUS = 3,
    TCMD_WLAN_MODE_CCK = 4,

    TCMD_WLAN_MODE_MAX,
    TCMD_WLAN_MODE_INVALID = TCMD_WLAN_MODE_MAX,

} TCMD_WLAN_MODE;

typedef enum {
    TPC_TX_PWR = 0,
    TPC_FORCED_GAIN,
    TPC_TGT_PWR
} TPC_TYPE;

typedef PREPACK struct {
    A_UINT32                testCmdId;
    A_UINT32                mode;
    A_UINT32                freq;
    A_UINT32                dataRate;
    A_INT32                 txPwr;
    A_UINT32                antenna;
    A_UINT32                enANI;
    A_UINT32                scramblerOff;
    A_UINT32                aifsn;
    A_UINT16                pktSz;
    A_UINT16                txPattern;
    A_UINT32                shortGuard;
    A_UINT32                numPackets;
    A_UINT32                wlanMode;
    A_UINT32                tpcm;
} POSTPACK TCMD_CONT_TX;

#define TCMD_TXPATTERN_ZERONE                 0x1
#define TCMD_TXPATTERN_ZERONE_DIS_SCRAMBLE    0x2

/* Continuous Rx
 act: TCMD_CONT_RX_PROMIS - promiscuous mode (accept all incoming frames)
      TCMD_CONT_RX_FILTER - filter mode (accept only frames with dest
                                             address equal specified
                                             mac address (set via act =3)
      TCMD_CONT_RX_REPORT  off mode  (disable cont rx mode and get the
                                          report from the last cont
                                          Rx test)

     TCMD_CONT_RX_SETMAC - set MacAddr mode (sets the MAC address for the
                                                 target. This Overrides
                                                 the default MAC address.)

*/
typedef enum {
    TCMD_CONT_RX_PROMIS =0,
    TCMD_CONT_RX_FILTER,
    TCMD_CONT_RX_REPORT,
    TCMD_CONT_RX_SETMAC,
    TCMD_CONT_RX_SET_ANT_SWITCH_TABLE,
 
    TC_CMD_RESP,
} TCMD_CONT_RX_ACT;

typedef PREPACK struct {
    A_UINT32        testCmdId;
    A_UINT32        act;
    A_UINT32        enANI;
    PREPACK union {
        struct PREPACK TCMD_CONT_RX_PARA {
            A_UINT32    freq;
            A_UINT32    antenna;
            A_UINT32    wlanMode;
        } POSTPACK para;
        struct PREPACK TCMD_CONT_RX_REPORT {
            A_UINT32    totalPkt;
            A_INT32     rssiInDBm;
            A_UINT32    crcErrPkt;
            A_UINT32    secErrPkt;
            A_UINT16    rateCnt[TCMD_MAX_RATES];
            A_UINT16    rateCntShortGuard[TCMD_MAX_RATES];
        } POSTPACK report;
        struct PREPACK TCMD_CONT_RX_MAC {
            A_UCHAR     addr[ATH_MAC_LEN];			
            A_UCHAR     btaddr[ATH_MAC_LEN];     
            A_UINT16    regDmn[2];			
            A_UINT32    otpWriteFlag;    			
        } POSTPACK mac;
        struct PREPACK TCMD_CONT_RX_ANT_SWITCH_TABLE {
            A_UINT32                antswitch1;
            A_UINT32                antswitch2;
        }POSTPACK antswitchtable;
    } POSTPACK u;
} POSTPACK TCMD_CONT_RX;

/* Force sleep/wake  test cmd
 mode: TCMD_PM_WAKEUP - Wakeup the target
       TCMD_PM_SLEEP - Force the target to sleep.
 */
typedef enum {
    TCMD_PM_WAKEUP = 1, /* be consistent with target */
    TCMD_PM_SLEEP,
    TCMD_PM_DEEPSLEEP
} TCMD_PM_MODE;

typedef PREPACK struct {
    A_UINT32  testCmdId;
    A_UINT32  mode;
} POSTPACK TCMD_PM;

typedef enum {
    TC_CMDS_VERSION_RESERVED=0,
    TC_CMDS_VERSION_MDK,
    TC_CMDS_VERSION_TS,
    TC_CMDS_VERSION_LAST,
} TC_CMDS_VERSION;

typedef enum {
    TC_CMDS_TS =0,
    TC_CMDS_CAL,
    TC_CMDS_TPCCAL = TC_CMDS_CAL,
    TC_CMDS_TPCCAL_WITH_OTPWRITE,
    TC_CMDS_OTPDUMP,
    TC_CMDS_OTPSTREAMWRITE,    
    TC_CMDS_EFUSEDUMP,    
    TC_CMDS_EFUSEWRITE,    
    TC_CMDS_READTHERMAL,
} TC_CMDS_ACT;

typedef PREPACK struct {
    A_UINT32   testCmdId;
    A_UINT32   act;
    PREPACK union {
        A_UINT32  enANI;    // to be identical to CONT_RX struct
        struct PREPACK {
            A_UINT16   length;
            A_UINT8    version;
            A_UINT8    bufLen;
        } POSTPACK parm;
    } POSTPACK u;
} POSTPACK TC_CMDS_HDR;

typedef PREPACK struct {
    TC_CMDS_HDR  hdr;
    A_UINT8      buf[TC_CMDS_SIZE_MAX];
} POSTPACK TC_CMDS;

typedef PREPACK struct {
    A_UINT32    testCmdId;
    A_UINT32    regAddr;
    A_UINT32    val;
    A_UINT16    flag;
} POSTPACK TCMD_SET_REG;

typedef enum {
    TCMD_CONT_TX_ID,
    TCMD_CONT_RX_ID,
    TCMD_PM_ID,
    TC_CMDS_ID,
    TCMD_SET_REG_ID,
	
	/*For synergy purpose we added the following tcmd id but these
	tcmd's will not go to the firmware instead we will write values 
	to the NV area */
	
	TCMD_NIC_MAC = 100, 
	TCMD_CAL_FILE_INDEX = 101,
} TCMD_ID;

typedef PREPACK struct 
{
    A_UINT32  testCmdId;
    A_UINT8   mac_address[ATH_MAC_LEN];
} POSTPACK TCMD_NIC_MAC_S;

typedef PREPACK struct 
{
       A_UINT32  testCmdId;
       A_UINT32   cal_file_index;
} POSTPACK TCMD_CAL_FILE_INDEX_S;

typedef PREPACK union {
          TCMD_CONT_TX         contTx;
          TCMD_CONT_RX         contRx;
          TCMD_PM              pm;
          // New test cmds from ART/MDK ...
          TC_CMDS              tcCmds;          
          TCMD_SET_REG setReg;
} POSTPACK TEST_CMD;

#ifdef __cplusplus
}
#endif

#endif /* TESTCMD_H_ */


