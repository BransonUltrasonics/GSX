#if 0
typedef struct pruDevInfo {
    struct vxbFdtDev   vxbFdtDev;
    VXB_RESOURCE_LIST  vxbResList;
} VXB_PRU_DEV_INFO;
#endif
#include "pruicss_v1.h"


#define TIESC_PERM_RW       0x0
#define TIESC_PERM_WRITE_ONLY 0x1
#define TIESC_PERM_READ_ONLY 0x2

#define TIESC_PERM_WRITE TIESC_PERM_WRITE_ONLY
#define TIESC_PERM_READ TIESC_PERM_READ_ONLY

#if 0
/* Struct for register permission array */
typedef struct {
    uint8_t reserved[1024];
    uint8_t reg_properties[4096];
} t_register_properties;

uint8_t pdi_reg_perm_array[4096];
#endif
LOCAL volatile uint32_t *pru_frame_proc = NULL;
LOCAL volatile uint32_t *pru_host_proc = NULL;

LOCAL volatile uint32_t pru_frame_proc_len = 0;
LOCAL volatile uint32_t pru_host_proc_len = 0;
#if 0
typedef struct
{
    uint16_t physical_start_addr;
    uint16_t length;
} t_sm_properties;

#define    MAX_SYNC_MAN                         8
#define    SIZEOF_SM_REGISTER                   8


static t_sm_properties sm_properties[MAX_SYNC_MAN];









#if 0
uint32_t configReg 	   = 0x54426000;
uint32_t control_0_Reg = 0x54422000;
uint32_t control_1_Reg = 0x54424000;
uint32_t iram_0_Reg = 0x54434000;
uint32_t iram_1_Reg = 0x54438000;
uint32_t pr1dram_0_reg = 0x54400000;
uint32_t pr1dram_1_reg = 0x54402000;
uint32_t sdram0_reg = 0x54410000;
#endif



/* Struct for host to PRUSS command interface */
typedef struct {
    uint8_t sm_buf_index;
    uint8_t lock_state;
    uint16_t addr;
} t_sm_processdata;



typedef struct {
    uint8_t reserved1[0x90];
    uint32_t system_time_low;
    uint32_t system_time_high;
    uint8_t sm_config_ongoing;
    uint8_t reserved2[7];
    uint16_t cmdlow;
    uint16_t cmdlow_ack;
    uint16_t param1low;
    uint16_t param2low;
    uint16_t resp1low;
    uint16_t resp2low;
#ifndef SYSTEM_TIME_PDI_CONTROLLED
    uint8_t reserved3[212];
#else
    uint8_t reserved3[24];
    uint32_t systime_offset_low;
    uint32_t systime_offset_high;
    uint8_t reserved4[180];
#endif
    t_sm_processdata sm_processdata[6];
} t_host_interface;

#include "vxbFdtTiPrussIntcEnd.h"
#endif
PRUICSS_V1_Object object;

