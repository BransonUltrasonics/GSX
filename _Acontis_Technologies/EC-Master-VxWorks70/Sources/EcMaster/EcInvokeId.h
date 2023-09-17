/*-----------------------------------------------------------------------------
 * EcInvokeId.h		        file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 awg
 * Description              description of file
 * Date                     2006/11/22::13:00
 *---------------------------------------------------------------------------*/

#ifndef __ECINVOKEID_H__
#define __ECINVOKEID_H__    1

/*-INCLUDES------------------------------------------------------------------*/

/*-DEFINES-------------------------------------------------------------------*/

/* master related invoke ids */
/* 0x0000xxxx low values are reserved for transition code invoke ids, see ECAT_INITCMD_I_P, ECAT_INITCMD_BEFORE, ... */
/* 0x0001xxxx are reserved for slave related invoke ids */
/* 0x0002xxxx are reserved for master related invoke ids */
/* 0x000200xx are reserved for scanbus related invoke ids */
/* 0x000201xx are reserved for dcs related invoke ids */
/* 0x000202xx are reserved for dcl related invoke ids */

typedef enum _EC_T_MBXINVOKEID
{
    eMbxInvokeId_UNKNOWN,                           /* skip the low values corresponding to the transition codes used as invoke ID too */
    eMbxInvokeId_FIRST                          = 0x00010000,
    eMbxInvokeId_RECV_FROM_SLAVE                = 0x00010000,
    eMbxInvokeId_POLL_RECV_FROM_SLAVE           = 0x00010001,
    eMbxInvokeId_SEND_TO_SLAVE                  = 0x00010002,
    eMbxInvokeId_INITCMD_RECV_FROM_SLAVE        = 0x00010003,
    eMbxInvokeId_INITCMD_POLL_RECV_FROM_SLAVE   = 0x00010004,
    eMbxInvokeId_INITCMD_SEND_TO_SLAVE          = 0x00010005,
    eMbxInvokeId_INVALIDATED_RECV_FROM_SLAVE    = 0x00010006,
    eMbxInvokeId_SEND_FILL_TO_SLAVE             = 0x00010007,
    eMbxInvokeId_TOGGLE_REPEAT_REQ              = 0x00010008,
    eMbxInvokeId_TOGGLE_REPEAT_RES              = 0x00010009,
    eMbxInvokeId_INITCMD_TOGGLE_REPEAT_REQ      = 0x0001000a,
    eMbxInvokeId_INITCMD_TOGGLE_REPEAT_RES      = 0x0001000b,
    eMbxInvokeId_INVALIDATED_SEND_TO_SLAVE      = 0x0001000c,
    eMbxInvokeId_LAST                           = 0x0001000c,

    /* Borland C++ datatype alignment correction */
    eMbxInvokeId_BCppDummy                      = 0xFFFFFFFF
} EC_T_MBXINVOKEID;

/* Queue Raw Cmd (EcSlave) */
#define INVOKE_ID_TFER_RAW_CMD          (eMbxInvokeId_LAST+1)   /* transfer raw command, blocking */

/* internal */
#define INVOKE_ID_WRITEEVENTMASK        (eMbxInvokeId_LAST+3)   /* write ECAT event mask */

#if (defined INCLUDE_SLAVE_STATISTICS)
#define INVOKE_ID_GET_SLAVE_STATISTICS  (eMbxInvokeId_LAST+7)   /* get slave statistics counters */
#endif

/* Scanbus (EcBusTopology) */
#define INVOKE_ID_SB_COUNT              0x00020000      /* count slaves on bus */
#define INVOKE_ID_SB_GETBUSSLAVES       0x00020001      /* get information about slaves on bus */
#define INVOKE_ID_SB_PORTSTATUS         0x00020002      /* Get Port State information per slave */
#define INVOKE_ID_SB_ACTIVATEALIAS      0x00020003      /* Send activation command to second station address for addressing */
#define INVOKE_ID_SB_PDICONTROL         0x00020004      /* Read out PDI Control to know about Power safe mode etc. of slave node */
#define INVOKE_ID_SB_WRTTMPSLVADDR      0x00020005      /* write temporary slave fixed address to ensure consistency while performing BT data collection */
#define INVOKE_ID_SB_WRTSLVFIXADDR      0x00020006      /* write slave fixed address to slaves (before performing border close) */
#define INVOKE_ID_SB_CHECKTMPADDRESS    0x00020007      /* check if write of temporary slave fixed address was successful */
#define INVOKE_ID_SB_RDPORTCONTROL      0x00020008      /* read DL Control register to check how port's states are, to have information about topology */
#define INVOKE_ID_SB_WRTPORTCONTROL     0x00020009      /* write DL Control register to correct Port configuration to a valid mode */
#define INVOKE_ID_SB_EEPADDR            0x0002000A      /* write EEPROM SII Address to all slaves */
#define INVOKE_ID_SB_EEPBUSY            0x0002000B      /* check and wait, whether slaves are done with processing the SII request */
#define INVOKE_ID_SB_EEPREAD            0x0002000C      /* Read SII Data from each slave individually */
#define INVOKE_ID_SB_EEPPREBUSY         0x0002000D      /* check and wait if EEPROM is busy before sending SII command */
#define INVOKE_ID_SB_EEPACCACQUIRE      0x0002000E      /* Assign EEPROM of all slaves to EtherCAT Master (not PDI) */
#if (defined INCLUDE_DC_SUPPORT)
#define INVOKE_ID_SB_PROBEDCUNIT        0x0002000F      /* probe DC unit */
#endif
#define INVOKE_ID_SB_IDENTIFYSLAVE      0x00020010      /* send identify command to detect slave explicitly */
#define INVOKE_ID_SB_CHECKFIXEDADDRESS  0x00020011      /* check slave fixed address again autoinc address */
#define INVOKE_ID_SB_READEVENTREGISTER  0x00020012      /* Read Event Register after DL Status IRQ */
#define INVOKE_ID_SB_READEVENTMASK      0x00020013      /* Read Event Mask Register */
#define INVOKE_ID_SB_WRITEEVENTMASK     0x00020014      /* Read Event Mask Register */
#define INVOKE_ID_SB_READALSTATUS       0x00020015      /* read out AL status of bus slave */
#define INVOKE_ID_SB_OPENCLOSEDPORTA    0x00020016      /* NOP frame to open closed ports 0 */
#define INVOKE_ID_SB_EEPCHECKSUMERROR   0x00020017      /* read EEPROM status registers to look for slave(s) with EEPROM checksum error */
#define INVOKE_ID_SB_CHECKDUPLICATE     0x00020018      /* check for unique fixed addresses (detect duplicates) */
#if (defined INCLUDE_RED_DEVICE)
#define INVOKE_ID_SB_REDHANDLING        0x00020019
#endif
#define INVOKE_ID_SB_LATCHRECVTIMES     0x00020020      /* latch receive times (broadcast on Receive Time Port0) to detect crossed lines */
#define INVOKE_ID_SB_READRECVTIMES      0x00020021		/* read receive times */
#define INVOKE_ID_SB_CHKCLEAN           0x00020022      /* clean slaves on bus after topology check modified them */
#define INVOKE_ID_SB_ACKSLVERROR        0x00020023      /* acknowledge slave error */
#define INVOKE_ID_SB_ACYCALSTATUSBRD    0x00020024      /* acyclic AL status broadcast */
#define INVOKE_ID_SB_MAX                0x000200ff      /* dummy invoke id to mark highest invoke id reserved for BT Scan */

/* Distributed Clocks (EcDistributedClocks) */
#define INVOKE_ID_DC_CLEARSYSTIMEVAL    0x00020100

#define INVOKE_ID_DC_DISABLESYNCSIGNALS 0x00020102
#define INVOKE_ID_DC_LATCHRECVTIMES     0x00020103
#define INVOKE_ID_DC_READRECVTIMES      0x00020104
#define INVOKE_ID_DC_WRITESTOFFSET      0x00020105
#define INVOKE_ID_DC_WRITEPROPDELAY     0x00020106
#define INVOKE_ID_DC_DBGSYSTIMEVAL      0x00020108
#define INVOKE_ID_DC_UPDATESLAVESTO     0x00020109
#define INVOKE_ID_DC_READSYNCPULSEWDT   0x0002010A
#define INVOKE_ID_DC_ARMWBURST          0x0002010B
#define INVOKE_ID_DC_SETDCSTARTTIME     0x0002010C
#define INVOKE_ID_DC_SETDCACTIVATION    0x0002010D
#define INVOKE_ID_DC_SETDCCYCLETIME     0x0002010E
#define INVOKE_ID_DC_SETDCLATCHCONFIG   0x0002010F
#define INVOKE_ID_DC_CFGDCCONTROLLERS   0x00020110
#define INVOKE_ID_DC_REDOPENCLOSEPORT   0x00020111
#define INVOKE_ID_DC_REDHANDLING        0x00020112
#define INVOKE_ID_DC_WRITESYSTIMEVAL    0x00020113
#define INVOKE_ID_DC_WRITEPROPAGDELAYS  0x00020114
#define INVOKE_ID_DC_READREFTIME        0x00020115
#define INVOKE_ID_DC_WAITFORINSYNC      0x00020116
#define INVOKE_ID_DC_ACYCDISTRIBUTION   0x00020117

#define INVOKE_ID_DC_MAX                0x000201ff

/* EEPROM Access */
#define INVOKE_ID_EEPA_START            0x00020300
#define INVOKE_ID_EEPA_CMD              0x00020301
#define INVOKE_ID_EEPA_PREADCMPL        0x00020302
#define INVOKE_ID_EEPA_READVALUE        0x00020303            
#define INVOKE_ID_EEPA_WWRITECMDDAT     0x00020304
#define INVOKE_ID_EEPA_PWRITECMPL       0x00020305
#define INVOKE_ID_EEPA_ASRD             0x00020306  /*< Check if the EEPROM was already correct assigned */
#define INVOKE_ID_EEPA_ASWR             0x00020307  /*< Assign the EEPROM  */

#define INVOKE_ID_EEPA_ACRD             0x00020308
#define INVOKE_ID_EEPA_WRELOADCMD       0x00020309 /*< EEPROM write reload command */
#define INVOKE_ID_EEPA_PRELOADCMPL      0x0002030A /*< EEPROM poll reload complete */
#define INVOKE_ID_EEPA_RESET_R          0x0002030B
#define INVOKE_ID_EEPA_RESET_E          0x0002030C
#define INVOKE_ID_EEPA_RESET_S          0x0002030D
#define INVOKE_ID_EEPA_ASCHECK          0x0002030E  /*< Check if the EEPROM assignment was successful */
#define INVOKE_ID_EEPA_READ_EEP_STATUS_REG  0x0002030F
#define INVOKE_ID_EEPA_READ_EEP_CONFIG_REG  0x00020310
#define INVOKE_ID_EEPA_CLEAR_ERR        0x00020311

#define INVOKE_ID_EEPA_MAX              0x000203ff

/* Port operation */
#define INVOKE_ID_PORTOP_CHANGEPORT     0x00030000
#define INVOKE_ID_PORTOP_RDDLCTRLCL     0x00030001
#define INVOKE_ID_PORTOP_CHECKDLCTRLCL  0x00030002

#define INVOKE_ID_PORTOP_MAX            0x000300ff

/* invoke IDs 0xFF00.0000 to 0xFFFE.0000 are reserved for client specific queued raw commands */
/* Invoke ID format: 0xFFCC.xxxx  CC = client id 0 .. 0xFE, a maximum of 255 clients are possible */
#define INVOKE_ID_CLNT_TFER_Q_RAW_CMD  (0xff000000) 

/* broadcast queued raw command (all registered clients will get the response */                                                        
#define INVOKE_ID_TFER_Q_RAW_CMD       (0xffff0000)

/*-TYPEDEFS------------------------------------------------------------------*/

/*-CLASS---------------------------------------------------------------------*/

/*-FUNCTION DECLARATION------------------------------------------------------*/

/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-FUNCTIONS-----------------------------------------------------------------*/


#endif  /*__ECINVOKEID_H__*/ 

/*-END OF SOURCE FILE--------------------------------------------------------*/
