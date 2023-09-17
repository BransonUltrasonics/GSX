/*
 * ecatSlaveBspSource.h
 *
 *  Created on: Apr 21, 2017
 */

#ifndef _ECATSLAVEBSPSOURCE_H_
#define _ECATSLAVEBSPSOURCE_H_

/*-----------------------------------------------------------------------------------------
------
------  Includes
------
-----------------------------------------------------------------------------------------*/
#include <stdio.h>

#ifndef ECAT_LIMITED_DEMO
#include <ecat_def.h>
#endif

#ifndef __ECATSLAVE_SLAVE_APP__
#include <ecatSlaveUserApp.h>
#endif



//#include <vxbFdtTiPrussCoreEnd.h>

/*Single datagram accessing contiguous multiple FMMU mapped areas in  a single slave for process data
is supported now by TI ESC firmware.
Process path latency in TI ESC is high when this support is active
For specific use cases (4SM with 3 FMMUs or multiple FMMUs (in a given ESC) are not accessed in a single datagram)
process path latency improvement can be achieved by disabling below define */
#define ENABLE_MULTIPLE_SM_ACCESS_IN_SINGLE_DATAGRAM     0


#define SPI_EEPROM_DATA_OFFSET 0xA0000

/*#define   OS_TICKS_IN_MILLI_SEC   10 // 100us tick */
#define OS_TICKS_IN_MILLI_SEC   1 /* 1000us tick */
#define PROTO extern

#define SWAPWORD
#define SWAPDWORD


#define ICSS_MDIO_USRPHYSEL_LINKINT_ENABLE 0x40
#define ICSS_MDIO_USRPHYSEL_LINKSTAT_MLINK 0x80

#define CM_PER_SPINLOCK_CLKCTRL_OFFSET  0x44E0010C //Could not find starteware equivalent
#define SPINLOCK_LOCKREG0_OFFSET 0x480CA800


#define PRUESC_PERM_RW       0x0
#define PRUESC_PERM_WRITE_ONLY 0x1
#define PRUESC_PERM_READ_ONLY 0x2

#define PRUESC_PERM_WRITE PRUESC_PERM_WRITE_ONLY
#define PRUESC_PERM_READ PRUESC_PERM_READ_ONLY


#define PRUESC_MDIO_CLKDIV   79 //For 2.5MHz MDIO clock: 200/(PRUESC_MDIO_CLKDIV+1)

#define PRUESC_MDIO_RX_LINK_DISABLE  0 //Slow MDIO state m/c based link detection
#define PRUESC_MDIO_RX_LINK_ENABLE   1 //Fast link detect using RXLINK forward from PHY to MDIO MLINK
#define PRUESC_LINK_POL_ACTIVE_LOW       1
#define PRUESC_LINK_POL_ACTIVE_HIGH      0

#define PDI_WD_TRIGGER_RX_SOF       (0 << 4)
#define PDI_WD_TRIGGER_LATCH_IN     (1 << 4)
#define PDI_WD_TRIGGER_SYNC0_OUT    (2 << 4)
#define PDI_WD_TRIGGER_SYNC1_OUT    (3 << 4)

#if ENABLE_MULTIPLE_SM_ACCESS_IN_SINGLE_DATAGRAM
#define PRUESC_PORT0_TX_DELAY    0x98
#else
#define PRUESC_PORT0_TX_DELAY    0x48
#endif
#define PRUESC_PORT1_TX_DELAY    PRUESC_PORT0_TX_DELAY

#define PDI_ISR_EDIO_NUM    7 //GPMC_CSN(2) -> pr1_edio_data_out7 for ICEv2.J4.Pin21

#define USE_ECAT_TIMER

#define ASSERT_DMB() __asm("    dmb")
#define ASSERT_DSB() __asm("    dsb")

#ifdef USE_ECAT_TIMER
#define ECAT_TIMER_INC_P_MS 1000000
#else
#define ECAT_TIMER_INC_P_MS ecat_timer_inc_p_ms /* ARM frequency: Will be detected during bsp_init*/
extern volatile Uint32 ecat_timer_inc_p_ms;
#endif

typedef void (*bsp_function_handler) (void);

#define LOCK_PD_BUF_AVAILABLE_FOR_HOST  0
#define LOCK_PD_BUF_HOST_ACCESS_START   1
#define LOCK_PD_BUF_HOST_ACCESS_FINISH  2

#define PRUSS_DRAM0_ACCESS 			0x1
#define PRUSS_DRAM1_ACCESS 			0x2
#define PRUSS_IEP_ACCESS 			0x3 

#define HOST2PRU_SYSTEM_TIME_LOW   	0x90
#define HOST2PRU_SYSTEM_TIME_HIGH 	0x94
#define HOST2PRU_SM_CONFIG_ONGOING 	0x98
#define HOST2PRU_CMD_LOW 			0xA0
#define HOST2PRU_CMDLOW_ACK 		0xA2
#define HOST2PRU_PARAM1LOW 			0xA4
#define HOST2PRU_PARAM2LOW 			0xA6
#define HOST2PRU_RESP1LOW 			0xA8
#define HOST2PRU_RESP2LOW 			0xAA
#define HOST2PRU_SM_PROCESS_DATA_0 	0x180
#define HOST2PRU_SM_PROCESS_DATA_1 	0x184
#define HOST2PRU_SM_PROCESS_DATA_2 	0x188
#define HOST2PRU_SM_PROCESS_DATA_3 	0x18C
#define HOST2PRU_SM_PROCESS_DATA_4 	0x190
#define HOST2PRU_SM_PROCESS_DATA_5 	0x194

#define HW_ICSS_IEP_GLOBAL_CFG                                 (0x0U)
#define HW_ICSS_IEP_GLOBAL_STATUS                              (0x4U)
#define HW_ICSS_IEP_COMPEN                                     (0x8U)
#define HW_ICSS_IEP_COUNT                                      (0xcU)
#define HW_ICSS_IEP_CAP_CFG                                    (0x10U)
#define HW_ICSS_IEP_CAP_STATUS                                 (0x14U)
#define HW_ICSS_IEP_CAPR0                                      (0x18U)
#define HW_ICSS_IEP_CAPR1                                      (0x1cU)
#define HW_ICSS_IEP_CAPR2                                      (0x20U)
#define HW_ICSS_IEP_CAPR3                                      (0x24U)
#define HW_ICSS_IEP_CAPR4                                      (0x28U)
#define HW_ICSS_IEP_CAPR5                                      (0x2cU)
#define HW_ICSS_IEP_CAPR6                                      (0x30U)
#define HW_ICSS_IEP_CAPF6                                      (0x34U)
#define HW_ICSS_IEP_CAPR7                                      (0x38U)
#define HW_ICSS_IEP_CAPF7                                      (0x3cU)
#define HW_ICSS_IEP_CMP_CFG                                    (0x40U)
#define HW_ICSS_IEP_CMP_STATUS                                 (0x44U)
#define HW_ICSS_IEP_CMP0                                       (0x48U)
#define HW_ICSS_IEP_CMP1                                       (0x4cU)
#define HW_ICSS_IEP_CMP2                                       (0x50U)
#define HW_ICSS_IEP_CMP3                                       (0x54U)
#define HW_ICSS_IEP_CMP4                                       (0x58U)
#define HW_ICSS_IEP_CMP5                                       (0x5cU)
#define HW_ICSS_IEP_CMP6                                       (0x60U)
#define HW_ICSS_IEP_CMP7                                       (0x64U)
#define HW_ICSS_IEP_RXIPG0                                     (0x80U)
#define HW_ICSS_IEP_RXIPG1                                     (0x84U)
#define HW_ICSS_IEP_SYNC_CTRL                                  (0x100U)
#define HW_ICSS_IEP_SYNC_FIRST_STAT                            (0x104U)
#define HW_ICSS_IEP_SYNC0_STAT                                 (0x108U)
#define HW_ICSS_IEP_SYNC1_STAT                                 (0x10cU)
#define HW_ICSS_IEP_SYNC_PWIDTH                                (0x110U)
#define HW_ICSS_IEP_SYNC0_PERIOD                               (0x114U)
#define HW_ICSS_IEP_SYNC1_DELAY                                (0x118U)
#define HW_ICSS_IEP_SYNC_START                                 (0x11cU)
#define HW_ICSS_IEP_WD_PREDIV                                  (0x200U)
#define HW_ICSS_IEP_PDI_WD_TIM                                 (0x204U)
#define HW_ICSS_IEP_PD_WD_TIM                                  (0x208U)
#define HW_ICSS_IEP_WD_STATUS                                  (0x20cU)
#define HW_ICSS_IEP_WD_EXP_CNT                                 (0x210U)
#define HW_ICSS_IEP_WD_CTRL                                    (0x214U)
#define HW_ICSS_IEP_DIGIO_CTRL                                 (0x300U)
#define HW_ICSS_IEP_DIGIO_STATUS                               (0x304U)
#define HW_ICSS_IEP_DIGIO_DATA_IN                              (0x308U)
#define HW_ICSS_IEP_DIGIO_DATA_IN_RAW                          (0x30cU)
#define HW_ICSS_IEP_DIGIO_DATA_OUT                             (0x310U)
#define HW_ICSS_IEP_DIGIO_DATA_OUT_EN                          (0x314U)
#define HW_ICSS_IEP_DIGIO_EXP                                  (0x318U)


#define CHANNEL0                0
#define CHANNEL1                1
#define CHANNEL2                2
#define CHANNEL3                3
#define CHANNEL4                4
#define CHANNEL5                5
#define CHANNEL6                6
#define CHANNEL7                7
#define CHANNEL8                8
#define CHANNEL9                9

#define PRU0                    0
#define PRU1                    1
#define PRU_EVTOUT0             2
#define PRU_EVTOUT1             3
#define PRU_EVTOUT2             4
#define PRU_EVTOUT3             5
#define PRU_EVTOUT4             6
#define PRU_EVTOUT5             7
#define PRU_EVTOUT6             8
#define PRU_EVTOUT7             9

#define	HOST_AL_EVENT		(20-CHANNEL2)+CHANNEL5
#define	HOST_SYNC0_EVENT	(20-CHANNEL2)+CHANNEL3
#define	HOST_SYNC1_EVENT	(20-CHANNEL2)+CHANNEL4
#define	HOST_CMD_LOW_ACK_EVENT	(20-CHANNEL2)+CHANNEL6

#define PD_WD_EXPIRY_EVENT   9
#define PDI_WD_EXPIRY_EVENT  10
#define LATCH1_IN_EVENT      11
#define LATCH0_IN_EVENT      12
#define SYNC1_OUT_EVENT      13
#define SYNC0_OUT_EVENT      14
#define PRU_ARM_EVENT0       18 //SYS_EVT_AL_EVENT_REQUEST
#define ARM_PRU_EVENT0       16 //SYS_EVT_HOST_CMD
#define ARM_PRU_EVENT1       19 //SYS_EVT_HOST_CMD_LOW
#define PRU_ARM_EVENT2       20 //SYS_EVT_HOST_CMD_LOW_ACK
#define PRU0_RX_ERR32_EVENT  33
#define PRU0_RX_OVF_EVENT    38
#define PORT0_TX_UNF_EVENT   39
#define PORT0_TX_OVF_EVENT   40
#define MII_LINK0_EVENT      41
#define PRU1_RX_ERR32_EVENT  45
#define PRU1_RX_OVF_EVENT    50
#define PORT1_TX_UNF_EVENT   51
#define PORT1_TX_OVF_EVENT   52
#define MII_LINK1_EVENT      53
#define ICSS1_PRUSS0_HOST_INTR5		56

#define MAX_SM_PROCESS_DATA 6
#define MAX_SYNC_MAN 8


typedef struct
{
    Uint16 physical_start_addr;
    Uint16 length;
} t_sm_properties;

/* Struct for host to PRUSS command interface */
typedef struct {
    Uint8 sm_buf_index;
    Uint8 lock_state;
    Uint16 addr;
} t_sm_processdata_1;


/* Struct for register permission array used in pruicss Attach procedure */
typedef struct {
    Uint8 reserved[1024];
    Uint8 reg_properties[4096];
} t_register_properties;

PROTO Int32 bsp_main(void);
PROTO void bsp_init_sharedRam();
PROTO void bsp_init_dramIep() ;
PROTO inline Uint8  bsp_get_pdi_access_perm(Uint16 address, Uint8 access);
PROTO inline Uint8  bsp_pdi_access_perm_word(Uint16 address, Uint8 access);
PROTO inline Uint8  bsp_pdi_access_perm_dword(Uint16 address, Uint8 access);
PROTO inline Uint8  bsp_pdi_access_perm_array(Uint16 address, Uint8 access, Uint16 size);
PROTO void bsp_exit();
PROTO Uint32 bsp_get_timer_register();
PROTO void bsp_clear_timer_register();
PROTO void bsp_get_local_sys_time(Uint32 *systime_low, Uint32 *systime_high);
PROTO void bsp_eeprom_emulation_init(void);
PROTO Int32 bsp_eeprom_load_esc_registers ( Int32 reload_flag);
PROTO Int32 bsp_eeprom_emulation_reload();
PROTO void bsp_eeprom_emulation_command_ack();
PROTO void bsp_eeprom_emulation_flush(void);
PROTO void bsp_eeprom_emulation_exit(void);
PROTO Uint8 *bsp_get_eeprom_cache_base(void);
void bsp_set_eeprom_updated_time(void);
Uint32 bsp_get_eeprom_updated_time(void);
PROTO void bsp_send_command_to_firmware( Uint32 command, Uint16 param1, Uint16 param2);
PROTO void bsp_pdi_post_read_indication( Uint16 address);
PROTO void bsp_pdi_write_indication( Uint16 address,Uint16 value);
PROTO void bsp_pdi_mbx_read_start();
PROTO void bsp_pdi_mbx_read_complete();
PROTO void bsp_pdi_mbx_write_start();
PROTO void bsp_pdi_mbx_write_complete();

PROTO Int16 bsp_get_sm_index (Uint16 address, Uint16 len);
PROTO Uint16 bsp_get_process_data_address( Uint16 address, Uint16 len, Int16* p_sm_index);
PROTO void bsp_process_data_access_complete( Uint16 address, Uint16 len, Int16 sm_index);

PROTO void bsp_set_sm_properties (Uint8 sm, Uint16 address, Uint16 len);
PROTO t_sm_properties *bsp_get_sm_properties (Uint8 sm);
PROTO Uint32 bsp_read_dword( Uint16 address);
PROTO Uint32 bsp_read_dword_isr( Uint16 address);
PROTO Uint16 bsp_read_word( Uint16 address);
PROTO Uint16 bsp_read_word_isr( Uint16 address);
PROTO Uint8 bsp_read_byte( Uint16 address);
PROTO Uint8 bsp_read_byte_isr( Uint16 address);
PROTO void bsp_read( Uint8 *pdata, Uint16 address, Uint16 len);
PROTO void bsp_write_dword( Uint32 val, Uint16 address);
PROTO void bsp_write_word( Uint16 val, Uint16 address);
PROTO void bsp_write_byte( Uint8 val, Uint16 address);
PROTO void bsp_write( Uint8 *pdata, Uint16 address, Uint16 len);
PROTO Uint32 bsp_pruss_iepreg_read ( Uint32 regoffset);
PROTO void bsp_pruss_iepreg_write ( Uint32 val, Uint32 regoffset);
PROTO Uint8 bsp_pdi_sm_config_ongoing ();

PROTO void bsp_hwspinlock_init (void);
PROTO Uint32 bsp_hwspinlock_lock (int num);
PROTO void bsp_hwspinlock_unlock (int num);
PROTO void bsp_global_mutex_lock (void);
PROTO void bsp_global_mutex_unlock (void);

PROTO void bsp_set_eeprom_update_status(Uint8 status);
PROTO Uint8 bsp_get_eeprom_update_status(void);

#undef PROTO

#define USE_ECAT_TIMER

#endif /* SOURCES_PKGS_NET_ECATSTACK_SRC_ECATSLAVEBSPSOURCE_H_ */
