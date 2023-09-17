/* vxbFdtTiPrussMdioEnd.c - driver for TI PRU ICSS FEC MDIO port 
 * 

DESCRIPTION

This module provides support for accessing the MDIO interface on
the TI PRU ICSS network controllers.

INCLUDE FILES: vxBus.h miiBus.h endLib.h vxbFdtLib.h

SEE ALSO: vxBus, miiBus
*/

/*
 * Copyright information - To be Done.
 *
 */

#include <vxWorks.h>
#include <semLib.h>
#include <stdlib.h>
#include <string.h>

#include <netLib.h>
#include <netBufLib.h>
#include <end.h>
#define END_MACROS
#include <endLib.h>

#include <hwif/vxBus.h>
#include <hwif/buslib/vxbFdtLib.h>
#include <hwif/methods/vxbMiiMethod.h>
#include <subsys/clk/vxbClkLib.h>
#include <subsys/pinmux/vxbPinMuxLib.h>
#include <miiBus.h>
#include <hEnd.h>
#include <subsys/timer/vxbTimerLib.h>
#include "vxbFdtTiPrussMdioEnd.h"
//#include "vxbCpswEnd.h"
#include <subsys/gpio/vxbGpioLib.h>
//#include <vxbFdtTiPrussEnd.h>
#include <vxbMdioLib.h>
/* defines */

MDIO_DRV_CTRL *      pDrvCtrl = NULL;

LOCAL int status_glob = 0;


#define	ESC_ADDR_TI_PORT0_TX_START_DELAY	0xE10
#define	ESC_ADDR_TI_PORT1_TX_START_DELAY	0xE12

#define TIESC_PORT0_TX_DELAY    0x48
#define TIESC_PORT1_TX_DELAY    TIESC_PORT0_TX_DELAY

#define MDIO_BAR(p)      ((MDIO_DRV_CTRL *)vxbDevSoftcGet(p))->regBase
#define MDIO_HANDLE(p)   ((MDIO_DRV_CTRL *)vxbDevSoftcGet(p))->handle

#define CSR_READ_4(pDev, addr)          \
    vxbRead32(MDIO_HANDLE(pDev),        \
            (UINT32 *)((char *)MDIO_BAR(pDev) + (addr)))

#define CSR_WRITE_4(pDev, addr, data)   \
    vxbWrite32(MDIO_HANDLE(pDev),       \
            (UINT32 *)((char *)MDIO_BAR(pDev) + (addr)), (data))

/* VxBus methods */

LOCAL STATUS pruProbe  (VXB_DEV_ID);
LOCAL STATUS pruAttach (VXB_DEV_ID);


/* miiBus methods */

LOCAL STATUS pruMDIOPhyRead  (VXB_DEV_ID , UINT8, UINT8, UINT16 *);
LOCAL STATUS pruMDIOPhyWrite (VXB_DEV_ID , UINT8, UINT8, UINT16);

LOCAL STATUS pruLogEnable(VXB_DEV_ID pDev);
LOCAL STATUS pruLogDisable(VXB_DEV_ID pDev);
LOCAL STATUS pruPrintRegister(VXB_DEV_ID pDev);

LOCAL VXB_DRV_METHOD fmMethods[] =
   {
   /* DEVICE API */
   { VXB_DEVMETHOD_CALL(vxbDevProbe),  pruProbe},
   { VXB_DEVMETHOD_CALL(vxbDevAttach), pruAttach},
 
   /* MII API */
   { VXB_DEVMETHOD_CALL(miiRead),  (FUNCPTR)pruMDIOPhyRead},
   { VXB_DEVMETHOD_CALL(miiWrite), (FUNCPTR)pruMDIOPhyWrite},
    VXB_DEVMETHOD_END
   };   
 
VXB_DRV prussMdioDrv =
    {
    { NULL } ,
    "TiMdio",		               /* drvName */
    "TI PRU ICSS mdio driver",     /* Description */
    VXB_BUSID_FDT,	               /* Class */
    0,		                       /* Flags */
    0,		                       /* Reference count */
    fmMethods                      /* Method table */
    };

VXB_DRV_DEF(prussMdioDrv);

LOCAL VXB_FDT_DEV_MATCH_ENTRY TiMdioMatch[] =
    {
        {
        "ti,am437x-prussMdio",  /* compatible */
        NULL
        },
        {} /* Empty terminated list */
    };

/*********************************************************************
*
* pruProbe - check whether device and driver go together
*
* This routine probe a device with a device driver.
*
* RETURNS: OK if the device and driver match up, ERROR otherwise
*
* ERRNO
*/
 
LOCAL int pruProbe
    (
    VXB_DEV_ID pDev
    )
    {
    VXB_ASSERT (pDev != NULL, ERROR)

    return vxbFdtDevMatch (pDev, TiMdioMatch, NULL);
    }

/*******************************************************************************
*
* pruAttach - attach pru mdio device
*
* This routine attach pru mdio device with Drv,it creates device 
* and assign resource for it.
*
* \NOMANUAL
*
* RETURNS: OK if attached successfully, otherwise ERROR
*
* ERRNO: N/A
*/
 
LOCAL STATUS pruAttach
    (
    VXB_DEV_ID pDev
    )
    {
	VXB_RESOURCE_ADR* pResAdr = NULL;
    VXB_RESOURCE 	* pRes;
    VXB_CLK 		* pClk;
    VXB_FDT_DEV     * pFdtDev      = NULL;
    VXB_MDIO_FUNC 	* mdio_func  = NULL;
    UINT16 			  value = 0;
    uint32_t 		  mdioClockFreq, mdioBusFreq;
    uint32_t 		  clkDiv = 0, val = 0;
    int               regIndex     = 0;
    
    
    VXB_ASSERT (pDev != NULL, ERROR)
    
    pClk = vxbClkGet (pDev, NULL);
    
    if (pClk == NULL)
	  return ERROR;
           
    (void) vxbClkEnable (pClk);
    
    (void) vxbPinMuxEnable (pDev);
  
    pDrvCtrl = (MDIO_DRV_CTRL *)vxbMemAlloc (sizeof(MDIO_DRV_CTRL));
    if (pDrvCtrl == NULL)
        return (ERROR);

    pDrvCtrl->clkFreq = (UINT32)vxbClkRateGet (pClk);

    if (pDrvCtrl->clkFreq == 0)
        {
        vxbMemFree(pDrvCtrl);
        return ERROR;
        }
    
    pDrvCtrl->pDev = pDev;
   
    pFdtDev = vxbFdtDevGet(pDev);
	
	if(pFdtDev == NULL)
	return ERROR;
        
        /* Get and allocate memory mapped address 
         * of register base address of "mdio" if it exists */
        {
        regIndex = pruicssStringLookup(pFdtDev->offset, "reg-names", "mdio");
        if (regIndex >= 0)
            {
            pRes = vxbResourceAlloc(pDrvCtrl->pDev, VXB_RES_MEMORY, regIndex);
            
            if (pRes == NULL || VXB_RES_TYPE(pRes->id) != VXB_RES_MEMORY)
                {
                PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,
                		"PRU ESC->(%s) mdio pRes error(mdio)\n",
						pFdtDev->name);
                return ERROR;
                }
            
            pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
            
            if (pResAdr == NULL)
                {
                PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,
                		"PRU ESC->(%s) mdio  pResAdr is "
                		"NULL(mdio)\n",pFdtDev->name);
                return ERROR;
                }
        
            /* save the resource info */
            pDrvCtrl->pRes = pRes;
            pDrvCtrl->regBase = (void*)pResAdr->virtual;
            pDrvCtrl->handle  = pResAdr->pHandle;
            
            PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,
            		"PRU ESC(%s) dram0   Register "
            		"P=0x%08x,V=0x%08x, regIndex=%d\n",
                pFdtDev->name,(UINT32)pResAdr->start,
				(UINT32)pResAdr->virtAddr, regIndex);
            }
        else
            {
            PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,
            		"Invalid Reg lookup of mdio (%s), "
            		"regIndex=%d\n",pFdtDev->name, regIndex);
            }
        }
    
        
        /* Get and allocate memory mapped address of
         *  register base address of "shdram" if it exists */
		{
		regIndex = pruicssStringLookup(pFdtDev->offset, "reg-names",
									"shdram");
		if (regIndex >= 0)
			{
			pRes = vxbResourceAlloc(pDrvCtrl->pDev, VXB_RES_MEMORY, regIndex);
			
			if (pRes == NULL || VXB_RES_TYPE(pRes->id) != VXB_RES_MEMORY)
				{
				PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,
						"PRU ESC->(%s) shdram  pRes error(shdram)\n",
						pFdtDev->name);
				return ERROR;
				}
			
			pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
			
			if (pResAdr == NULL)
				{
				PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,
						"PRU ESC->(%s) shdram  pResAdr is "
						"NULL(shdram)\n", pFdtDev->name);
				return ERROR;
				}
		
			/* save the resource info */
			pDrvCtrl->shdpRes = pRes;
			pDrvCtrl->shdregBase = (void*)pResAdr->virtual;
			pDrvCtrl->shdhandle  = pResAdr->pHandle;
			
			PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,
					"PRU ESC(%s) dram0   Register "
					"P=0x%08x,V=0x%08x, regIndex=%d\n",
			    pFdtDev->name, (UINT32)pResAdr->start,
				(UINT32)pResAdr->virtAddr, regIndex);
			}
		else
			{
			PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,
					"Invalid Reg lookup of shdram (%s),"
					" regIndex=%d\n",pFdtDev->name, regIndex);
			}
		}
            
    vxbDevSoftcSet (pDev, pDrvCtrl);
 
   /* Configure MDIO clock = 200/(clkdiv+1)
    * Disable preamble , enable fault detection
    */
   mdioClockFreq = 200000000;
   mdioBusFreq = 2200000;
   clkDiv = (mdioClockFreq/mdioBusFreq) - 1U;
   
   PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,
		   	   	   	   "Revision ID  %x\n",CSR_READ_4 (pDev, 0x0));
	   
   PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,
		   	   	   	   "MDIO Control %x\n",CSR_READ_4 (pDev, 
		   	   	   			   PRU_MDIO_CONTROL));
   
   val |= PRU_MDIO_EN | MDIO_CTRL_PREAMBLE_MASK | 
		   MDIO_CTRL_FAULTENB_MASK | clkDiv;
   
   PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,
		   	   	   	   "MDIO Control Val %x\n",val);
	
   // mdio enable
   /*CSR_WRITE_4 (pDev, 
		   PRU_MDIO_CONTROL,PRU_MDIO_EN | 
		   MDIO_CTRL_PREAMBLE_MASK | MDIO_CTRL_FAULTENB_MASK | 
		   clkDiv);*/
   CSR_WRITE_4 (pDev, 
   		   PRU_MDIO_CONTROL,val);
   
   PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,"MDIO Control %x\n",
		   	   	   	   CSR_READ_4 (pDev, PRU_MDIO_CONTROL));


   /* PHY RESET */
   vxbGpioAlloc(GPIO_BANK4_PIN20);
   vxbGpioSetDir(GPIO_BANK4_PIN20,1);
   vxbGpioSetValue(GPIO_BANK4_PIN20,0);
   vxbUsDelay(300);
   vxbGpioSetValue(GPIO_BANK4_PIN20,1);
 
   
   //Enable AutoMDIX for particular port
   pruMDIOPhyRead(pDev,TLK105_PHY0,TLKPHY_PHYCR_REG,&value);
   value = value | TLKPHY_AUTOMDIX_ENABLE ;
   pruMDIOPhyWrite(pDev,TLK105_PHY0,TLKPHY_PHYCR_REG,value);

   pruMDIOPhyRead(pDev,TLK105_PHY1,TLKPHY_PHYCR_REG,&value);
   value = value | TLKPHY_AUTOMDIX_ENABLE ;
   pruMDIOPhyWrite(pDev,TLK105_PHY1,TLKPHY_PHYCR_REG,value);
	   
   //MLED Configuration of TLK 105 PHY0
   pruMDIOPhyWrite(pDev,TLK105_PHY0,TLKPHY_REGCR_REG,
		   EXT_REG_ADDRESS_ACCESS);
   pruMDIOPhyWrite(pDev,TLK105_PHY0,TLKPHY_ADDR_REG,
		   TLK105_EXT_MLEDCR_REG);
   pruMDIOPhyWrite(pDev,TLK105_PHY0,TLKPHY_REGCR_REG,
		   EXT_REG_DATA_NORMAL_ACCESS);

   pruMDIOPhyRead(pDev,TLK105_PHY0,TLKPHY_ADDR_REG,&value);
					  
   value |= ((1 << 10) | (5 << 3) | 0);
				   
   pruMDIOPhyWrite(pDev,TLK105_PHY0,TLKPHY_ADDR_REG,value);
		   
   //MLED Configuration of TLK 105 PHY 1
   pruMDIOPhyWrite(pDev,TLK105_PHY1,TLKPHY_REGCR_REG,
		   EXT_REG_ADDRESS_ACCESS);
   pruMDIOPhyWrite(pDev,TLK105_PHY1,TLKPHY_ADDR_REG,
		   TLK105_EXT_MLEDCR_REG);
   pruMDIOPhyWrite(pDev,TLK105_PHY1,TLKPHY_REGCR_REG,
		   EXT_REG_DATA_NORMAL_ACCESS);

   pruMDIOPhyRead(pDev,TLK105_PHY1,TLKPHY_ADDR_REG,&value);
									
   value |= ((1 << 10) | (5 << 3) | 0);
								 
   pruMDIOPhyWrite(pDev,TLK105_PHY1,TLKPHY_ADDR_REG,value);
   
   do
	 {
	 pruMDIOPhyRead(pDev,TLK105_PHY0,PHY_PHYIDR1_REG,&value);
	 PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,
						 "Reading value of PHY_PHYIDR1_REG for PHY-0, "
						 "PHY_PHYIDR1_REG = %x\n PHY-0 is ready "
						 "for use\n",
						 value);
	 } while(value != 0x2000);
	  
			  
   do
	 {  
	 pruMDIOPhyRead(pDev,TLK105_PHY1,PHY_PHYIDR1_REG,&value);
	 PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,
			 "Reading value of PHY_PHYIDR1_REG for PHY-1, "
			 "PHY_PHYIDR1_REG = %x\n PHY-1 is ready for use\n",value);  	  
	 } while(value != 0x2000);
			
   
   //SWSCR2
   //Bit2: Enable RXERR during IDLE detection
   //Bit1: Disable detection of transmit error in odd-nibble boundary for odd nibble insertion)
   //Bit5: Force Full-Duplex while working with link partner in forced 100B-TX. When the
   //PHY is set to Auto-Negotiation or Force 100B-TX and the link partner is operated
   //in Force 100B-TX, the link is always Full Duplex
   //For EtherCAT : Disable enhanced LED link function
   
	/*Function to enable Extended Full Duplex ability*/
   value = 0;
   pruMDIOPhyRead(pDev,TLK105_PHY0,TLKPHY_CR2_REG,&value);
   value = value | (1 << 5) ;
   pruMDIOPhyWrite(pDev,TLK105_PHY0,TLKPHY_CR2_REG,value);
  
   value = 0;
   pruMDIOPhyRead(pDev,TLK105_PHY1,TLKPHY_CR2_REG,&value);
   value = value | (1 << 5) ;
   pruMDIOPhyWrite(pDev,TLK105_PHY1,TLKPHY_CR2_REG,value);

				
  /*Function to Disbale ODD Nibble detection*/
   value = 0;
   pruMDIOPhyRead(pDev,TLK105_PHY0,TLKPHY_CR2_REG,&value);
   value = value | (1 << 1) ;
   pruMDIOPhyWrite(pDev,TLK105_PHY0,TLKPHY_CR2_REG,value);
   
   value = 0;
   pruMDIOPhyRead(pDev,TLK105_PHY1,TLKPHY_CR2_REG,&value);
   value = value | (1 << 1) ;
   pruMDIOPhyWrite(pDev,TLK105_PHY1,TLKPHY_CR2_REG,value);
 
				
  /*Enables Detection of Receive Symbol Error During IDLE State*/
   value = 0;
   pruMDIOPhyRead(pDev,TLK105_PHY0,TLKPHY_CR2_REG,&value);
   value = value | (1 << 2) ;
   pruMDIOPhyWrite(pDev,TLK105_PHY0,TLKPHY_CR2_REG,value);
  
   value = 0;
   pruMDIOPhyRead(pDev,TLK105_PHY1,TLKPHY_CR2_REG,&value);
   value = value | (1 << 2) ;
   pruMDIOPhyWrite(pDev,TLK105_PHY1,TLKPHY_CR2_REG,value);
 
   
  /*Function does the  LED Configuration of TLK 105
   * Mode 2: ON for Good Link, BLINK for Activity
   */
   value = 0;
   pruMDIOPhyRead(pDev,TLK105_PHY0,TLKPHY_PHYCR_REG,&value);
   value &= ~(1<<5);
   pruMDIOPhyWrite(pDev,TLK105_PHY0,TLKPHY_PHYCR_REG,value);
  
   value = 0;
   pruMDIOPhyRead(pDev,TLK105_PHY1,TLKPHY_PHYCR_REG,&value);
   value &= ~(1<<5);
   pruMDIOPhyWrite(pDev,TLK105_PHY1,TLKPHY_PHYCR_REG,value);
 
  
  /*Function to Configure EthernetLED Blink rate
   * 10 = 5Hz (200mSec)
   */
 
   value = 0;
   pruMDIOPhyRead(pDev,TLK105_PHY0,TLKPHY_LEDCR_REG,&value);
   value &= 0xF9FF;
   value |= 0x0400;
   pruMDIOPhyWrite(pDev,TLK105_PHY0,TLKPHY_LEDCR_REG,value);
			
   value = 0;
   pruMDIOPhyRead(pDev,TLK105_PHY1,TLKPHY_LEDCR_REG,&value);
   value &= 0xF9FF;
   value |= 0x0400;
   pruMDIOPhyWrite(pDev,TLK105_PHY1,TLKPHY_LEDCR_REG,value);
 
   
  //Bit3: Drop the link based on RX Error count of the MII interface, when a predefined number
  // of 32 RX Error occurrences in a 10us interval is reached, the link will be dropped
  // Bit0: Drop the link based on Signal/Energy loss indication, when the Energy detector
  //indicates Energy Loss, the link will be dropped. Typical reaction time is 10us.
  /* Function to enable Fast Link Down Detection
   * FAST_LINKDOWN_SIGENERGY|FAST_LINKDOWN_RXERR 
   */
 
   value = 0;
   pruMDIOPhyRead(pDev,TLK105_PHY0,TLKPHY_CR3_REG,&value);
   value  |= (1 | (1 << 3));
   pruMDIOPhyWrite(pDev,TLK105_PHY0,TLKPHY_CR3_REG,value);
	
   value = 0;
   pruMDIOPhyRead(pDev,TLK105_PHY1,TLKPHY_CR3_REG,&value);
   value  |= (1 | (1 << 3));
   pruMDIOPhyWrite(pDev,TLK105_PHY1,TLKPHY_CR3_REG,value);
 
  
  /* Function to enable Fast RXDV Detection
   * FASTRXDV_DET_ENABLE
   */
 
   value = 0;
   pruMDIOPhyRead(pDev,TLK105_PHY0,TLKPHY_CR1_REG,&value);
   value = value | (1 << 1) ;
   pruMDIOPhyWrite(pDev,TLK105_PHY0,TLKPHY_CR1_REG,value);
				
   value = 0;
   pruMDIOPhyRead(pDev,TLK105_PHY1,TLKPHY_CR1_REG,&value);
   value = value | (1 << 1) ;
   pruMDIOPhyWrite(pDev,TLK105_PHY1,TLKPHY_CR1_REG,value);
 
	   
  /* Select PHY address enable link change interrupt 
   * at MDIOLinkIntMasked and MDIOLinkIntRaw */
	 
  //MDIO enableLinkInterrupt for PHY0
   value = 0;
   value |= 0x40;
   value |= 0x80;
   CSR_WRITE_4 (pDev,MDIO_USERPHYSEL(0),value);
	 
   //MDIO enableLinkInterrupt for PHY1                   
   value = 1;
   value |= 0x40;
   value |= 0x80;
   CSR_WRITE_4 (pDev,MDIO_USERPHYSEL(1),value);
   
   mdio_func = vxbMemAlloc (sizeof (VXB_MDIO_FUNC));
   if (mdio_func == NULL)
	  {
	  return ERROR;
	  }

   mdio_func->mdioDisable = pruLogDisable;
   mdio_func->mdioEnable = pruLogEnable;
   mdio_func->mdioPrintReg = pruPrintRegister;
   mdio_Register (pDev, mdio_func); 
					  
//  (void) miiBusCreate (pDev, VXB_MDIO_FLAG_CLAUSE22);
  return (OK);
}

/*******************************************************************************
*
* pruLogEnable - enable background link polling logs
*
* This routine enable background link polling logs.
*
* ERRNO: N/A
*/
LOCAL STATUS pruLogEnable(VXB_DEV_ID pDev)
{

	status_glob = 1;
    return 0;
}


/*******************************************************************************
*
* pruLogDisable - disable background link polling logs
*
* This routine disable background link polling logs.
*
* ERRNO: N/A
*/
LOCAL STATUS pruLogDisable(VXB_DEV_ID pDev)
{
	
	status_glob = 0;
    return 0;
}

/*******************************************************************************
*
* pruPrintRegister - prints all the MDIO register
*
* This routine prints all the MDIO register.
*
* ERRNO: N/A
*/
LOCAL STATUS pruPrintRegister(VXB_DEV_ID pDev)
{

    int i;
	for(i=0; i < 0x30; i+=4){
		kprintf("Reg 0x%x = %x \n",i,CSR_READ_4 (pDev,i));
	}
	for(i=0x80; i < 0x90; i+=4){
		kprintf("Reg 0x%x = %x \n",i,CSR_READ_4 (pDev,i));
	}

    return 0;
}

/*******************************************************************************
*
* pruMDIOPhyWrite - writes to TLK105L register
*
* This routine takes PHY address, register to read in TLK105L and data as input and
* writes to corresponding TLK105L register
*
*
* ERRNO: N/A
*/
LOCAL STATUS pruMDIOPhyWrite
    (
    VXB_DEV_ID   pPhyParent,
    UINT8       phyAddr,
    UINT8       regAddr,
    UINT16       dataVal
    )
    {
    UINT32          ret;
    UINT32          index = 0;
    
   // if(phyAddr == 1)
    //	index = 1;
    
   
    if (phyAddr >= 32)
        {
        return ERROR;
        }

    do
        {
    	  ret = CSR_READ_4 (pPhyParent,PRU_USERACCESSn(index));
        } while (ret & PRU_MDIO_GO);

    ret = (phyAddr << PRU_PHY_ADDR_SHIFT) |
          (regAddr << PRU_REG_ADDR_SHIFT) |
          PRU_MDIO_GO | PRU_MDIO_WRITE | dataVal;
    
    CSR_WRITE_4 (pPhyParent, PRU_USERACCESSn(index), ret);
    do
        {
        ret = CSR_READ_4 (pPhyParent, PRU_USERACCESSn(index));
        }  while (((ret & 0x80000000) & (PRU_MDIO_GO)));
    
       if(status_glob)
       PRUICSS_MDIO_DBG_MSG (  PRUICSS_MDIO_DBG_INFO,
    		   "The value write to PHY-1 register - %d = %x\n",
			   regAddr,dataVal);

    return OK;
    }

/*******************************************************************************
*
* pruMDIOPhyRead - reads to TLK105L register
*
* This routine takes PHY address and register to write to in TLK105L as input and
* reads corresponding TLK105L register
*
* ERRNO: N/A
*/
LOCAL STATUS pruMDIOPhyRead
    (
    VXB_DEV_ID   pPhyParent,
    UINT8      phyAddr,
    UINT8      regAddr,
    UINT16   * pDataVal
    )
    {
    UINT32          ret;
    UINT32          index = 0;
    
  //  if(phyAddr == 1)
    //   index = 1;
    
    if (phyAddr >= 32)
       {
        return ERROR;
       }
    
    do
        {
        ret = CSR_READ_4 (pPhyParent, PRU_USERACCESSn(index));
        } while (((ret & 0x80000000) & (PRU_MDIO_GO)));
        

    ret = phyAddr << PRU_PHY_ADDR_SHIFT |
          regAddr << PRU_REG_ADDR_SHIFT |
          PRU_MDIO_GO;
 
    CSR_WRITE_4 (pPhyParent, PRU_USERACCESSn(index), ret);
    
  
    do
        {
    	ret = CSR_READ_4 (pPhyParent, PRU_USERACCESSn(index));
        } while (((ret & 0x80000000) & (PRU_MDIO_GO)));
    
    
    
        *pDataVal = ret & 0xffff;
    
    if(status_glob)
    PRUICSS_MDIO_DBG_MSG ( PRUICSS_MDIO_DBG_INFO,
    		"The value Read from PHY-1 register - %d = %x\n",
			regAddr,*pDataVal);

    return OK;
    }

/******************************************************************************
 *
 * pruicssStringLookup - find string in a list and return index.
 *
 * This function find string in a list and return index of matched string, for
 * example, here is an dts node:
 *
 *
 * RETURNS: 
 *   0: for first string in list
 *   1: for second string in list
 *   2: for third string in list
 *   n: for n'TH string in list
 *(-1): when failed.
 *
 * ERRNO: N/A
 */
LOCAL STATUS pruicssStringLookup
    (
    int     offset,
    char        * propName,
    char        * string
    )
    {
    char  * pPropStr;
    int     propLen, strLen;
    int     ix = 0;

    if (propName == NULL || string == NULL)
        {
        PRUICSS_MDIO_DBG_MSG (  PRUICSS_MDIO_DBG_INFO,
        		"parameter error.\n");
        return (-1);
        }
    
    pPropStr = (char*)vxFdtPropGet (offset, propName, &propLen);
    if (pPropStr == NULL)
        {
        PRUICSS_MDIO_DBG_MSG (  PRUICSS_MDIO_DBG_INFO,
        		"search %s error.\n",propName);
        return (-1);
        }

    /* 
     * the returned property is collection of NULL end strings like this:
     * 'b' 'l' 'u' 'e' '0' 'r' 'e' 'd' '0'
     */

    while (propLen > 0)
        {
        strLen = strlen(pPropStr);
        if (strncmp(pPropStr, string, strLen) == 0)
            {
            return ix;
            }
        propLen  = propLen - strLen - 1;
        pPropStr = pPropStr + strLen + 1; /*move the string point to next string*/
        ix++;
        }

    /* not found, return -1 */
    PRUICSS_MDIO_DBG_MSG (  PRUICSS_MDIO_DBG_INFO,
    		"not found %s error.\n",propName);
    return (-1);    
    }
