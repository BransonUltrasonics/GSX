
#ifndef ti_drivers_PRUICSS__include
#define ti_drivers_PRUICSS__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>



#define PRUICSS_NUM_PRU_HOSTS        10
#define PRUICSS_NUM_PRU_CHANNELS     10
#define PRUICSS_NUM_PRU_SYS_EVTS     64
#define PRUICSS_MAX_WAIT_EVENTS	10

#define PRUICSS_PRU0_DATARAM         0
#define PRUICSS_PRU1_DATARAM     1
#define PRUICSS_PRU0_IRAM        2
#define PRUICSS_PRU1_IRAM        3
#define PRUICSS_SHARED_DATARAM   4
#define	PRUICSS_CFG              5
#define	PRUICSS_UART             6
#define	PRUICSS_IEP              7
#define	PRUICSS_ECAP             8
#define	PRUICSS_MII_RT           9
#define	PRUICSS_MDIO            10

/** @brief Interrupt handler   */
typedef void (*PRUICSSDRV_IRQ_HANDLER)(void*);
typedef struct  PRUICSS_Config          *PRUICSS_Handle;




/***********************************************************************
*  Macros to support dynamic chip detecting feature
***********************************************************************/



typedef void *(*prussdrv_function_handler) (void *);


/*!
 *  @brief      PRUICSS system to channel map
 *
 */
typedef struct __PRUICSS_SysevtToChannelMap
{
    uint8_t sysevt;   /*System event number */
    uint8_t channel;  /*Channel number */

    uint8_t polarity;  /*Interrupt polarity [ 0 - active low , 1 - active high ] */

    uint8_t type;  /*Interrupt type [ 0 - level or pulse interrupt , 1 - edge interrupt ] */
}PRUICSS_SysevtToChannelMap;

/*!
 *  @brief      PRUICSS channel to host map
 *
 */
typedef struct __PRUICSS_ChannelToHostMap
 {
     short channel;
     short host;
 }PRUICSS_ChannelToHostMap;

/*!
 *  @brief      PRUICSS Interrupt controller initialisation data structure
 *
 */
 typedef struct __PRUICSS_IntcInitData
 {
     /*Enabled SYSEVTs - Range:0..63
     {-1} indicates end of list*/
     char sysevts_enabled[PRUICSS_NUM_PRU_SYS_EVTS];

      /*SysEvt to Channel map. SYSEVTs - Range:0..63 Channels -Range: 0..9
     {-1, -1} indicates end of list*/
     PRUICSS_SysevtToChannelMap sysevt_to_channel_map[PRUICSS_NUM_PRU_SYS_EVTS];

     /*Channel to Host map.Channels -Range: 0..9  HOSTs - Range:0..9
     {-1, -1} indicates end of list*/
     PRUICSS_ChannelToHostMap channel_to_host_map[PRUICSS_NUM_PRU_CHANNELS];

     /*10-bit mask - Enable Host0-Host9 {Host0/1:PRU0/1, Host2..9 : PRUEVT_OUT0..7)*/
     uint32_t host_enable_bitmask;
 }PRUICSS_IntcInitData;


/*!
 *  @brief     brief PRUICSS Global configuration
 *
 */
typedef struct PRUICSS_Config {

    /*! Pointer to a driver specific data object */
    void                   *object;

    /*! Pointer to a driver specific hardware attributes structure */
    void          		   *hwAttrs;
} PRUICSS_Config;


/*!
 *  @brief     brief PRUICSS IRQ handlers mapping
 *
 */
typedef struct __PRUICSS_IrqFunMap
{

	PRUICSSDRV_IRQ_HANDLER irqHandler;
	/**< Interrupt handler*/
	void* semHandle;
    /**< Binary semaphore to wait on */
}PRUICSS_IrqFunMap;

/*!
 *  @brief     brief PRUICSS IRQ handlers mapping
 *
 */
typedef struct _PRUICSS_IrqArgs
{

	PRUICSS_IrqFunMap* pruFunMap;
	/**< PRUICSS IRQ handlers mapping*/
	PRUICSS_Handle handle;
    /**< PRUICSS_Configuration */
	int host;
}PRUICSS_IrqArgs;

/**
 *  @brief PRU Firmware binary header
 *
 */
typedef struct _PRUICSS_FirmwareBinHeader
{

	uint32_t image_size;
    /**< Image size */
	uint32_t load_addr;
    /**< Address to which this binary need to be loaded */
	uint32_t run_addr;
    /**< Address at which the execution should start */
}PRUICSS_FirmwareBinHeader;


typedef enum PRUICSS_Return
{
  PRUICSS_RETURN_FAILURE = -1,
  PRUICSS_RETURN_SUCCESS = 0

}PRUICSS_Return;

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_PRUICSS__include */
