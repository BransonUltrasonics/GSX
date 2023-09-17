#ifndef __ECATSLAVE_HW__
#define __ECATSLAVE_HW__


#ifndef __ECATSLAVE_SLAVE_APP__
#include <ecatSlaveUserApp.h>
#endif

#ifndef __INCecatSlavePalLibh
#include <ecatSlavePalLib.h>
#endif

#ifndef _ECATSLAVEBSPSOURCE_H_
#include <ecatSlaveBspSource.h>
#endif

#define DISABLE_ESC_INT()               ;//bsp_global_mutex_lock();
#define ENABLE_ESC_INT()                ;//bsp_global_mutex_unlock();

#define    MAX_SYNC_MAN                         8
#define    SIZEOF_SM_REGISTER                   8

#define ECAT_TIMER_INC_P_MS 1000000

#ifdef ECAT_LIMITED_DEMO
    //Lib Mode
    #define PRUESC_REVISION_NUMBER               0x00000001
#else
    //Patch mode
    #define PRUESC_REVISION_NUMBER               0x00000011
#endif

#define ECAT_SLAVE_EEPROM_SIZE         0x800

#define    MAILBOX_WRITE                        0
#define    MAILBOX_READ                         1
#define    PROCESS_DATA_OUT                     2
#define    PROCESS_DATA_IN                      3

#define     MBX_WRITE_EVENT                 ((Uint16) 0x0100)
#define     MBX_READ_EVENT                  ((Uint16) 0x0200)

//Below constants are not defined in esc.h

#define ESC_ADDR_CONFIG_STATION_ALIAS	0x012
#define	ESC_ADDR_DLSTATUS		0x110
#define ESC_ADDR_ALCONTROL		0x120
#define	ESC_ADDR_ALSTATUS		0x130
#define	ESC_ADDR_PDI_CONTROL    0x140
#define	ESC_PDI_CONTROL_ELD_ALL_PORTS_MASK (1 << 1)
#define	ESC_ADDR_PDI_CONFIG		0x150
#define ESC_ADDR_AL_EVENT_MASK  0x204
#define ESC_ADDR_AL_EVENT_REQ   0x220
#define	ESC_ADDR_SM_WD_STATUS	0x440
#define ESC_ADDR_EEPROM_CTRL    0x502
#define	ESC_ADDR_MI_ECAT_ACCESS	0x516
#define	ESC_ADDR_MI_PDI_ACCESS	0x517

#define ESC_EEPROM_CMD_MASK                     0x0700 //Description (0x502.8:10): Command bit mask
#define ESC_EEPROM_CMD_READ_MASK                0x0100 //Description (0x502.8): Currently executed read command
#define ESC_EEPROM_CMD_WRITE_MASK               0x0200 //Description (0x502.9): Initialize Write Command
#define ESC_EEPROM_CMD_RELOAD_MASK              0x0400 //Description (0x502.10): Trigger EEPROM reload
#define ESC_EEPROM_ERROR_MASK                   0x7800 //Description : Mask all EEPROM error bits; Checksum error (0x0502.11); EEPROM not loaded (0x0502.12); Missing EEPROM Acknowledge (0x0502.13); Write Error (0x0502.14)
#define ESC_EEPROM_ERROR_CRC                    0x0800 //Description (0x502.11): EEPROM CRC Error
#define ESC_EEPROM_ERROR_CMD_ACK                0x2000 //Description (0x502.13): EEPROM Busy
#define ESC_EEPROM_BUSY_MASK                    0x8000  //Description (0x502.15): EEPROM Busy

#define ESC_ADDR_SYNCMAN 0x800

#define ESC_ADDR_SM1_STATUS 0x80D
#define SM_STATUS_MBX_FULL  0x08

#define	ESC_ADDR_SM0_STATUS     0x805
#define	ESC_ADDR_SM0_ACTIVATE	0x806
#define	ESC_ADDR_SM1_ACTIVATE	0x806+8
#define	ESC_ADDR_SM2_ACTIVATE	0x806+8*2
#define	ESC_ADDR_SM3_ACTIVATE	0x806+8*3
#define	ESC_ADDR_SM4_ACTIVATE	0x806+8*4
#define	ESC_ADDR_SM5_ACTIVATE	0x806+8*5
#define	ESC_ADDR_SM6_ACTIVATE	0x806+8*6
#define	ESC_ADDR_SM7_ACTIVATE	0x806+8*7
#define	ESC_ADDR_SM0_PDI_CONTROL	0x807
#define	ESC_ADDR_SM1_PDI_CONTROL	0x807+8
#define	ESC_ADDR_SM2_PDI_CONTROL	0x807+8*2
#define	ESC_ADDR_SM3_PDI_CONTROL	0x807+8*3
#define	ESC_ADDR_SM4_PDI_CONTROL	0x807+8*4
#define	ESC_ADDR_SM5_PDI_CONTROL	0x807+8*5
#define	ESC_ADDR_SM6_PDI_CONTROL	0x807+8*6
#define	ESC_ADDR_SM7_PDI_CONTROL	0x807+8*7

#define SM_PDI_CONTROL_SM_DISABLE      1

#define	ESC_ADDR_SYSTIME  		    0x910
#define	ESC_ADDR_SYSTIME_HIGH		0x914
#define ESC_ADDR_SYSTIME_OFFSET		0x920
#define ESC_ADDR_SYSTIME_DELAY		0x928
#define ESC_ADDR_SPEEDCOUNTER_START	0x930
#define ESC_ADDR_TIMEDIFF_FILTDEPTH	0x934
#define ESC_ADDR_SPEEDDIFF_FILTDEPTH 0x935
#define	ESC_ADDR_SYNC_PULSE_LENGTH	0x982
#define	ESC_ADDR_SYNC_STATUS		0x98E
#define	ESC_ADDR_LATCH0_POS_EDGE	0x9B0
#define	ESC_ADDR_LATCH0_NEG_EDGE	0x9B8
#define	ESC_ADDR_LATCH1_POS_EDGE	0x9C0
#define	ESC_ADDR_LATCH1_NEG_EDGE	0x9C8
#define ESC_ADDR_TI_PORT0_ACTIVITY	0xE00
#define ESC_ADDR_TI_PORT1_ACTIVITY	0xE04
#define ESC_ADDR_TI_PORT0_PHYADDR	0xE08
#define ESC_ADDR_TI_PORT1_PHYADDR	0xE09
#define	ESC_ADDR_TI_PDI_ISR_PINSEL	0xE0A
#define	ESC_ADDR_TI_PHY_LINK_POLARITY	0XE0C
#define	ESC_ADDR_TI_PORT0_TX_START_DELAY	0xE10
#define	ESC_ADDR_TI_PORT1_TX_START_DELAY	0xE12
#define	ESC_ADDR_TI_ESC_RESET		0xE14
#define	TI_ESC_RST_CMD_U	0x545352
#define	TI_ESC_RST_CMD_L	0x747372

#define	ESC_ADDR_MEMORY			0x1000

#define	CMD_DL_USER_CLEAR_AL_EVENT_HIGH		0x0
#define	CMD_DL_USER_GET_BUFFER_READ_ADDR	0x1
#define	CMD_DL_USER_GET_BUFFER_WRITE_ADDR	0x2
#define	CMD_DL_USER_SET_BUFFER_WRITE_DONE	0x3
#define	CMD_DL_USER_ACK_MBX_READ			0x4
#define	CMD_DL_USER_ACK_MBX_WRITE			0x5

#define	CMD_DL_USER_EEPROM_CMD_ACK			0x6
#define	CMD_DL_USER_READ_SYNC_STATUS		0x7
#define	SYNC0	0
#define	SYNC1	1
#define	CMD_DL_USER_READ_AL_CONTROL			0x8
#define	CMD_DL_USER_WRITE_AL_STATUS			0x9
#define	CMD_DL_USER_READ_PD_WD_STATUS		0xA
#define	CMD_DL_USER_READ_SM_ACTIVATE		0xB
#define	CMD_DL_USER_WRITE_SM_PDI_CTRL		0xC
#define	CMD_DL_USER_READ_LATCH_TIME			0xD
#define	LATCH0_POS_EDGE	0
#define	LATCH0_NEG_EDGE	1
#define	LATCH1_POS_EDGE	2
#define	LATCH1_NEG_EDGE	3
#define	CMD_DL_USER_READ_SYS_TIME			0xE
#define	CMD_DL_USER_CLEAR_AL_EVENT_LOW		0xF
#ifdef SYSTEM_TIME_PDI_CONTROLLED
#define CMD_DL_USER_SYSTIME_PDI_CONTROL    0x10
#define WRITE_SYSTIME              0
#define WRITE_SYSTIME_OFFSET       1
#define WRITE_FILTER_CONFIG        2
#endif

#ifndef TIMER_INT_HEADER
#define	TIMER_INT_HEADER
#endif


#ifndef EE_BUSY_TIMEOUT_VALUE
#define EE_BUSY_TIMEOUT_VALUE 0x2000
#endif

#define ESC_RD					0x02    /* read acces to ESC */
#define	ESC_WR					0x04    /* write acces to ESC */

#ifdef	_PRUESCHW_
    #undef PROTO
    #define PROTO
#else
    #define PROTO extern
#endif

#define SUPPORT_CMDACK_POLL_MODE  /* Currently supporting Polling mode for DRAM0 access
									Used in send command to Firmware */
/*-----------------------------------------------------------------------------------------
------
------	variables
------
-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------	function prototypes
------
-----------------------------------------------------------------------------------------*/
#if UC_SET_ECAT_LED
PROTO void HW_SetLed(Uint8 RunLed, Uint8 ErrLed);
#endif
PROTO Uint8 HW_Init(void);
PROTO void HW_Release(void);

#define HW_GetALEventRegister() (bsp_read_word(ESC_ADDR_AL_EVENT_REQ))
#define HW_GetALEventRegister_Isr() (bsp_read_word_isr(ESC_ADDR_AL_EVENT_REQ))

PROTO void HW_ResetALEventMask(Uint16 intMask);
PROTO void HW_SetALEventMask(Uint16 intMask);
PROTO Uint16 HW_GetALEventMask(void);
PROTO void HW_EscRead(Uint8 * pData, Uint16 Address, Uint16 Len);
PROTO void HW_EscReadIsr(Uint8 * pData, Uint16 Address, Uint16 Len);
PROTO void HW_EscReadByteIsr(Uint8 ByteValue, Uint16 Address);

PROTO Uint8 __HW_EscReadByteIsr( Uint16 Address);
PROTO Uint16 __HW_EscReadWordIsr(Uint16 Address);
PROTO Uint32 __HW_EscReadDWordIsr(Uint16 Address);
#define HW_EscReadByteIsr(ByteValue, Address) ((ByteValue) = __HW_EscReadByteIsr(Address))
#define HW_EscReadWordIsr(WordValue, Address) ((WordValue) = __HW_EscReadWordIsr( Address))
#define HW_EscReadDWordIsr(DWordValue, Address)((DWordValue) = __HW_EscReadDWordIsr(Address))



#define HW_EscReadByte(ByteValue, Address) ((ByteValue) = bsp_read_byte( Address))
#define HW_EscReadWord(WordValue, Address) ((WordValue) = bsp_read_word( Address))
#define HW_EscReadDWord(DWordValue, Address) ((DWordValue) = bsp_read_dword( Address))

PROTO void HW_EscReadMbxMem(Uint8 * pData, Uint16 Address, Uint16 Len);

PROTO void HW_EscWrite(Uint8 * pData, Uint16 Address, Uint16 Len);
PROTO void HW_EscWriteIsr(Uint8 * pData, Uint16 Address, Uint16 Len);
PROTO void HW_EscWriteByte(Uint8 ByteValue, Uint16 Address);
PROTO void HW_EscWriteByteIsr(Uint8 ByteValue, Uint16 Address);
PROTO void HW_EscWriteWord(Uint16 WordValue, Uint16 Address);
PROTO void HW_EscWriteWordIsr(Uint16 WordValue, Uint16 Address);
PROTO void HW_EscWriteDWord(Uint32 DWordValue, Uint16 Address);
PROTO void HW_EscWriteDWordIsr(Uint32 DWordValue, Uint16 Address);

PROTO void HW_EscWriteMbxMem(Uint8 * pData, Uint16 Address, Uint16 Len);

PROTO void HW_DisableSyncManChannel(Uint16 channel);
PROTO void HW_EnableSyncManChannel(Uint16 channel);
PROTO void *HW_GetSyncMan(Uint8 channel);

PROTO void HW_EcatIsr(void);
PROTO unsigned int HW_GetTimer();
PROTO void HW_ClearTimer();
PROTO Uint16 HW_EepromReload ();

PROTO void HW_RestartTarget(void);

PROTO void PDI_Isr(void);
PROTO void PDI_IsrFlag(void);
PROTO void Sync0_Isr(void);
PROTO void Sync1_Isr(void);

#undef PROTO


#endif //__ECATSLAVE_HW__
