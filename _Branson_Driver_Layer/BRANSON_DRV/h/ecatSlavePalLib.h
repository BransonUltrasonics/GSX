#ifndef __INCecatSlavePalLibh
#define __INCecatSlavePalLibh

#ifdef __cplusplus
extern "C" {
#endif

#include <vxWorks.h>
#include <vsbConfig.h>
#include <semLib.h>
#include <stdlib.h>
#include <string.h>
#include <intLib.h>
#include <stdio.h>
#include <string.h>
#include <lstLib.h>
#include <semLib.h>
#include <sysLib.h>
#include <wdLib.h>
#include <vmLib.h>
#include <etherMultiLib.h>
#include <end.h>
#include <endLib.h>
#include <endMedia.h>
#include <cacheLib.h>
#include <iosLib.h> 
#include <hwif/vxBus.h>
#include <hwif/buslib/vxbFdtLib.h>
#include <hwif/buslib/vxbI2cLib.h>
#include <subsys/clk/vxbClkLib.h>
#include <subsys/pinmux/vxbPinMuxLib.h>
#include <subsys/int/vxbIntLib.h>
#include <subsys/timer/vxbTimerLib.h>
#include <time.h>
#include <private/timeP.h>    
//#include <PRUESC.h>
//#include <vxbFdtTiPrussEnd.h> /* Driver Header FIle */
#include <vxbFdtTiPrussCoreEnd.h>
#include <vxbPru.h>

#define TEST_FAILED -1
#define TEST_SUCCESS 0

#define OK 0

/** @def BOARD_DEFAULT
 * 		 DEFAULT value used to set other options
 */
#define BOARD_DEFAULT							(0u)

#define BOARD_TRICOLOR0_RED						(1u << (BOARD_DEFAULT + 4u))
/** @def BOARD_TRICOLOR0_GREEN
 * 		 Macro for configuring Tri color0 green
 */
#define BOARD_TRICOLOR0_GREEN					(1u << (BOARD_DEFAULT + 5u))
/** @def BOARD_TRICOLOR0_YELLOW
 * 		 Macro for configuring Tri color0 yellow
 */
#define BOARD_TRICOLOR0_YELLOW					(1u << (BOARD_DEFAULT + 6u))
/** @def BOARD_TRICOLOR1_RED
 * 		 Macro for configuring Tri color1 red
 */
#define BOARD_TRICOLOR1_RED						(1u << (BOARD_DEFAULT + 7u))
/** @def BOARD_TRICOLOR1_GREEN
 * 		 Macro for configuring Tri color1 green
 */
#define BOARD_TRICOLOR1_GREEN					(1u << (BOARD_DEFAULT + 8u))
/** @def BOARD_TRICOLOR1_YELLOW
 * 		 Macro for configuring Tri color1 yellow
 */
#define BOARD_TRICOLOR1_YELLOW					(1u << (BOARD_DEFAULT + 9u))

#define GPIO_BANK2_PIN23 0x57
#define GPIO_BANK2_PIN24 0x58  //Red
#define GPIO_BANK2_PIN25 0x59
#define GPIO_BANK2_PIN22 0x56
#define GPIO_BANK4_PIN13 0x8D
#define GPIO_BANK4_PIN16 0x90

uint32_t bsp_read_Revision(void);
uint32_t bsp_read_ControllerId(void);
uint32_t bsp_read_buildVersion(void);
uint32_t bsp_read_al_control(void);


uint8_t  ReadByte_dram_iep(uint32_t offset, uint8_t op);

uint16_t  ReadWord_dram_iep(uint32_t offset, uint8_t op);

uint32_t  ReadDword_dram_iep(uint32_t offset, uint8_t op);

void  WriteByte_dram_iep(uint32_t offset, uint8_t data, uint8_t op);

void  WriteWord_dram_iep(uint32_t offset, uint16_t data, uint8_t op);

void  WriteDword_dram_iep(uint32_t offset, uint32_t data, uint8_t op);

void ReadBlock_dram_iep(uint32_t offset, uint8_t *pData, uint32_t len, uint8_t op);

void WriteBlock_dram_iep(uint32_t offset, uint8_t *pData, uint32_t len, uint8_t op);


uint8_t  ReadByte_shdram(uint32_t offset);

uint16_t  ReadWord_shdram(uint32_t offset);

uint32_t  ReadDword_shdram(uint32_t offset);

void  WriteByte_shdram(uint32_t offset, uint8_t data);

void  WriteWord_shdram(uint32_t offset, uint16_t data);

void  WriteDword_shdram(uint32_t offset, uint32_t data);

void ReadBlock_shdram(uint32_t offset, uint8_t *pData, uint32_t len);

void WriteBlock_shdram(uint32_t offset, uint8_t *pData, uint32_t len);

void  ReadRange_shdram(uint32_t offset, uint32_t len);

void vxbGpioSetValueLED(uint32_t ledType, uint8_t value);

#ifdef __cplusplus
}
#endif

IMPORT void ecatSlave_Register
    (
    VXB_DEV_ID pDev
    );

IMPORT void ecatSlave_Register_core
    (
    VXB_DEV_ID pDev
    );

#endif /* __INCecatSlavePalLibh */
