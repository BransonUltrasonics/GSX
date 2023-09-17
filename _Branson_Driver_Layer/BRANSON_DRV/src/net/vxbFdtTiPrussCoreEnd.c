/*
 * PRU-subsystem Core 0/1 driver for AM437x IDK
 *
 *
 * Copyright information - To be Done.
 *
 */


/* includes */
#include <vxWorks.h>
#include <vsbConfig.h>
#include <semLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <lstLib.h>
#include <iosLib.h>
#include <hwif/vxBus.h>
#include <hwif/buslib/vxbFdtLib.h>
#include <hwif/buslib/vxbI2cLib.h>
#include <subsys/clk/vxbClkLib.h>
#include <subsys/pinmux/vxbPinMuxLib.h>
#include <subsys/timer/vxbTimerLib.h>
#include <time.h>
#include <private/timeP.h>

#include "vxbFdtTiPrussCoreEnd.h" /* Driver Header FIle */
#include <PRUESC.h>
#include <ecat_frame_handler_bin.h>
#include <ecat_host_interface_bin.h>

#include <ecatSlavePalLib.h>


/* driver utility functions */
LOCAL VXB_DRV_METHOD pruCoreMethods[] = {
    { VXB_DEVMETHOD_CALL(vxbDevProbe),    (FUNCPTR)pruCoreProbe    },
    { VXB_DEVMETHOD_CALL(vxbDevAttach),   (FUNCPTR)pruCoreAttach   },
    { 0, 0 }
};


/* VxBus driver definition */
VXB_DRV  pruCoreDrv =
    {
    { NULL } ,
    "pruCore",          /* drvName */
    "PRU Cores 0/1",    /* Description */
    VXB_BUSID_FDT,      /* Class */
    0,                  /* Flags */
    0,                  /* Reference count */
    pruCoreMethods      /* Method table */
    };
VXB_DRV_DEF(pruCoreDrv)


/* struct used for matching a device */
LOCAL VXB_FDT_DEV_MATCH_ENTRY prussCoreMatch[] =
    {
        {
        "ti,am437x-pru-core",   /* compatible */
        NULL,                   /* no configuration */
        },
        {}                      /* Empty terminated list */
    };

/******************************************************************************
*
* pruCoreProbe - probe for device presence at specific address
*
* This routine probes for device presence at specific address.
*
* RETURNS: OK if probe passes.  ERROR otherwise.
*
* ERRNO: N/A
*/
LOCAL STATUS pruCoreProbe
    (
    VXB_DEV_ID pDev
    )
    {
    return vxbFdtDevMatch (pDev, prussCoreMatch, NULL);
    }

/******************************************************************************
*
* pruCoreAttach - attach PRU Core device
*
* This is the PRU EtherCAT Core initialization routine.
*
* RETURNS: OK or ERROR when initialize failed
*
* ERRNO: N/A
*/

LOCAL STATUS pruCoreAttach
    (
    VXB_DEV_ID          pDev
    )
    {
	VXB_RESOURCE_ADR *      pResAdr         = NULL;
    VXB_RESOURCE *          pRes            = NULL;
    STATUS                  ret             = ERROR;
    VXB_FDT_DEV *           pFdtDev;
    int                     offset,regIndex        =0;
    VXB_DEV_ID              pCur;
    char    			  * pruCore0  = "pruCore1_0";
    char    			  * pruCore1  = "pruCore1_1";
    int       				pruNum    = 0;
    int       				strLen    = 0;
    
    int result, len;
    
    if (pDev == NULL)
        {
        return ERROR;
        }
    
    pFdtDev = vxbFdtDevGet (pDev);
    
    if (pFdtDev == NULL)
        {
        return ERROR;
        }
    
    strLen    = strlen(pFdtDev->name);

    /* create controller driver context structure for core */
    result = vxFdtPropGet(pFdtDev->offset, "reg", &len);

    pDrvCtrl = (PRUICSS_CORE_CTRL *) vxbMemAlloc (sizeof (PRUICSS_CORE_CTRL));
    if (pDrvCtrl == NULL)
        {
        return ERROR;
        }
       
    /* save pDrvCtrl in VXB_DEVICE structure */

    vxbDevSoftcSet (pDev, pDrvCtrl);
    /* save instance ID */
    pDrvCtrl->pDev = pDev;

    pRes = vxbResourceAlloc(pDrvCtrl->pDev, VXB_RES_MEMORY, 0);
    if(pRes == NULL)
        {
        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "pRes is NULL\n");
        goto attach_error;
        }

    pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
    if (pResAdr == NULL)
        {
        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "pResAdr is NULL\n");
        goto attach_error;
        }

    pDrvCtrl->pIramregHandle = pResAdr->pHandle;
    pDrvCtrl->pIramregBase= (void *)pResAdr->virtual;

    pRes = vxbResourceAlloc(pDrvCtrl->pDev, VXB_RES_MEMORY, 1);
    if(pRes == NULL)
        {
        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "pRes is NULL\n");
        goto attach_error;
        }

    pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
    if (pResAdr == NULL)
        {
        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "pResAdr is NULL\n");
        goto attach_error;
        }

    pDrvCtrl->pCtrlregHandle = pResAdr->pHandle;
    pDrvCtrl->pCtrlregBase= (void *)pResAdr->virtual;

    /* PRU Reset Start*/
    pFdtDev = vxbFdtDevGet(pDev);


    /* PRU Frame firmware header length */
    pru_frame_proc_len  = sizeof(FrameProc);

    /* PRU Proc firmware header length */
    pru_host_proc_len   = sizeof(HostProc);

    /* Update PRU Core 0 with Frame firmware header */    
    if(strncmp(pFdtDev->name, pruCore0, strLen) == 0)
        {
        pruNum = 0;
        object.pruBinBuff[pruNum] = FrameProc;
        object.buffLen[pruNum] = pru_frame_proc_len;
        }

     /* Update PRU Core 1 with Proc firmware header */ 
    if(strncmp(pFdtDev->name, pruCore1, strLen) == 0)
        {
        pruNum = 1;
        object.pruBinBuff[pruNum] = HostProc;
        object.buffLen[pruNum] = pru_host_proc_len;
        }

    /* Reset only PRU Cores 0/1 of PRUICSS 1 */    
    if ((strncmp(pFdtDev->name, pruCore0, strLen) == 0) ||
        (strncmp(pFdtDev->name, pruCore1, strLen) == 0))
            {
                if(OK != pruCoreReset(pDev, pruNum))
                    {
                    //PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "Unable to RESET PRU CORE[%d]\n", pruNum);
                    }
            }

        pRes = vxbResourceAlloc(pDrvCtrl->pDev, VXB_RES_MEMORY, 2);
        if(pRes == NULL)
            {
            PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "pRes is NULL\n");
            goto attach_error;
            }
    
        pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
        if (pResAdr == NULL)
            {
            PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "pResAdr is NULL\n");
            goto attach_error;
            }

        pDrvCtrl->pDbgregHandle = pResAdr->pHandle;
        pDrvCtrl->pDbgregBase= (void *)pResAdr->virtual;


        {
        regIndex = pruCoreStringLookup(pFdtDev->offset, "reg-names", "iram");
        if (regIndex >= 0)
        {
        pRes = vxbResourceAlloc(pDrvCtrl->pDev, VXB_RES_MEMORY, regIndex);
        if (pRes == NULL || VXB_RES_TYPE(pRes->id) != VXB_RES_MEMORY)
            {
            PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "PRU ESC->(%s) iram       pRes error(dram0)\n",pFdtDev->name);
            goto attach_error;
            }
        pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
        if (pResAdr == NULL)
            {
            PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "PRU ESC->(%s) iram   pResAdr is NULL(dram0)\n",pFdtDev->name);
            goto attach_error;
            }
    
        /* save the resource info */
        
        pDrvCtrl->pIramregHandle  = pResAdr->pHandle;
        pDrvCtrl->pIramregBase = (void*)pResAdr->virtual;
        

        }
        else
        {
        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "Invalid Reg lookup of dram0 (%s), regIndex=%d\n",pFdtDev->name, regIndex);
        }
        }

        /* Load firmware only on PRU Cores 0/1 of PRUICSS 1 */

        if ((strncmp(pFdtDev->name, pruCore0, strLen) == 0) ||
            (strncmp(pFdtDev->name, pruCore1, strLen) == 0))
            {
            pruCoreBootFirmware(pDev, pruNum);
        
            {               
                if (strncmp(pFdtDev->name, pruCore0, strLen) == 0)
                {
                regIndex = pruCoreStringLookup(pFdtDev->offset, "reg-names", "sdram0");
                }
                else
                {
                regIndex = pruCoreStringLookup(pFdtDev->offset, "reg-names", "sdram1");
                }
            
                if (regIndex >= 0)
                    {
                    pRes = vxbResourceAlloc(pDrvCtrl->pDev, VXB_RES_MEMORY, regIndex);
                    if (pRes == NULL || VXB_RES_TYPE(pRes->id) != VXB_RES_MEMORY)
                        {
                        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "PRU CORE->(%s) shdram1       pRes error(cfg)\n",pFdtDev->name);
                        return ERROR;
                        }
                    pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
                    if (pResAdr == NULL)
                        {
                        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "PRU CORE->(%s) shdram1   pResAdr is NULL(cfg)\n",pFdtDev->name);
                        return ERROR;
                        }                   
            
                    
                    ecatSlave_Register_core(pDev);
                    
                    pDrvCtrl-> pPsdrmRegHandle = pResAdr->pHandle;
                    pDrvCtrl->pPsdrmRegBase = (void*)pResAdr->virtual;
                    //PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "PRU ESC(%s) SDRAM   Register P=0x%08x,V=0x%08x, regIndex=%d\n",
                    //  pFdtDev->name,(UINT32)pResAdr->start,(UINT32)pResAdr->virtAddr, regIndex);
                    uint8_t     tSlvCtrlr, revID, fwBldVer1;
                    uint16_t    fwBldVer ;

                    if (strncmp(pFdtDev->name, pruCore1, strLen) == 0)
                        {
                        uint32_t   pdiCtrlVal   = 0; /* PDI Control Status Value */

                        /* PDI Control will be at offset 0x140 of Shared RAM */
                        uint32_t * address      = (uint32_t *)(pResAdr->virtAddr + PRUSS_CONTROL_STATUS);

                        /* Shared RAM Base Address */
                        uint32_t * Revaddress   = (uint32_t *)pResAdr->virtAddr;

                        /* we have to wait here, until the ESC is started */
                        do {
                            pdiCtrlVal = vxbRead32(pResAdr->pHandle,address);
                            pdiCtrlVal = ((uint8_t)pdiCtrlVal) & 0xFF;

                        }while (pdiCtrlVal != 0x80); //Looking for PDI COntrol Status to be initialized
                        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\t\t\tPDI Control Status       : 0x%x\n", pdiCtrlVal);
                        pdiCtrlVal = vxbRead32(pResAdr->pHandle,Revaddress);
                        
                        tSlvCtrlr   = (pdiCtrlVal) & 0xff;
                        revID       = (pdiCtrlVal >> (8)) & 0xff;
                        fwBldVer    = (pdiCtrlVal >> (16)) & 0xffff;

                        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\n\n\t\t\t*******************Device & Build information*******************\n");
                        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\t\t\tType of Slave Controller : 0x%x  TI ESC: 0x90\n", tSlvCtrlr);
                        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\t\t\tRevision ID              : 0x%x  AM335X: 0x1, AM437X: 0x2\n", revID);
                        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\t\t\tFirmware Build Version   : 0x%x  AM335x/AM437x : 0x3B3\n", fwBldVer);
                        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\t\t\t****************************************************************\n");
                        
                        pdiCtrlVal = vxbRead32(pResAdr->pHandle,Revaddress+1);
                        
                        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\t\t\tvalue of 0x4 register : 0x%x\n",(pdiCtrlVal & 0xff));
                        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\t\t\tvalue of 0x5 register : 0x%x\n",((pdiCtrlVal >> 8) & 0xff));
                        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\t\t\tvalue of 0x6 register : 0x%x\n",((pdiCtrlVal >> 16) & 0xff));
                        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\t\t\tvalue of 0x7 register : 0x%x\n",((pdiCtrlVal >> 24) & 0xff));
                
                                           
                                             	                        
                        }

                    PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\n\t\t\tPRU IS INITIALIZED!\n");

                    PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\n\n\n\t\t\tPRU READ/WRITE TEST!\n");
                    volatile int32_t temp, temp1 = fwBldVer;

                    /* Write/Read from PDI Control area of Shared RAM */ 

                    uint32_t * testAddress = ((uint32_t *)pResAdr->virtAddr) + PRUSS_CONTROL_STATUS;

                    temp = vxbRead32(pResAdr->pHandle,testAddress);

                    PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\t\t\tWriting the value to PDI SHRD RAM   : 0x%08x\n",temp1);
                    vxbWrite32(pResAdr->pHandle,testAddress, temp1);

                    temp = vxbRead32(pResAdr->pHandle,testAddress);
                    PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\t\t\tReading the value from PDI SHRD RAM : 0x%08x\n\n\n\n",temp);

                    }
                else
                    {
                    PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "Invalid Reg lookup of cfg (%s), regIndex=%d\n",pFdtDev->name, regIndex);
                    }
                }

            }
        
    return OK;
        
    attach_error:
	
            if (pDrvCtrl->semSync != SEM_ID_NULL)
                {
                (void)semDelete (pDrvCtrl->semSync);
                }

            if (pDrvCtrl->intRes != NULL)
                {
                (void)vxbResourceFree (pDev, pDrvCtrl->intRes);
                }

            if (pRes != NULL)
                {
                (void)vxbResourceFree (pDev, pRes);
                }

            (void)vxbClkDisableAll (pDev);
            (void)vxbPinMuxDisable (pDev);
            vxbMemFree (pDrvCtrl);
            vxbDevSoftcSet (pDev, NULL);
            return ERROR;
    }


/******************************************************************************
*
* pruCoreBootFirmware - loads and boots PRUICSS firmware on to PRU Cores 0/1
*
* RETURNS: OK.  ERROR otherwise.
*
* ERRNO: N/A
*/

LOCAL   STATUS pruCoreBootFirmware  
    (
    VXB_DEV_ID      pDev,
    int             pruNum
    )
    {

        VXB_RESOURCE_ADR *      pResAdr = NULL;
        VXB_RESOURCE *          pRes    = NULL;
        int regIndex = 0;
        VXB_FDT_DEV *           pFdtDev;

        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\t\t\tLoad & Boot the PRU CORE[%d]\n", pruNum);
        pFdtDev = vxbFdtDevGet (pDev);
        
        if (pFdtDev == NULL)
            {
            PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "FDT IS NULL\n");
            return ERROR;
            }

    regIndex = pruCoreStringLookup(pFdtDev->offset, "reg-names", "iram");
    if (regIndex >= 0)
        {
        pRes = vxbResourceAlloc(pDrvCtrl->pDev, VXB_RES_MEMORY, regIndex);
        if (pRes == NULL || VXB_RES_TYPE(pRes->id) != VXB_RES_MEMORY)
            {
            PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "PRU ESC->(%s) iram       pRes error(dram0)\n",pFdtDev->name);
            return ERROR;
            }
        pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
        if (pResAdr == NULL)
            {
            PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "PRU ESC->(%s) iram   pResAdr is NULL(dram0)\n",pFdtDev->name);
            return ERROR;
            }
    
        /* save the resource info */
        
        pDrvCtrl->pIramregHandle  = pResAdr->pHandle;
        pDrvCtrl->pIramregBase = (void*)pResAdr->virtual;
        
        //PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "(%s) iram   Register P=0x%08x,V=0x%08x, regIndex=%d\n",
        //    pFdtDev->name,(UINT32)pResAdr->start,(UINT32)pResAdr->virtAddr, regIndex);
        }
    else
        {
        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "Invalid Reg lookup of iram (%s), regIndex=%d\n",pFdtDev->name, regIndex);
        }

    pruCoreDisable(pDev,pruNum);
    
        if(pruNum == 0)
            {   
            pruCoreWriteMemory(pDev
                              ,pResAdr->virtual
                              ,0
                              ,(uint32_t *) object.pruBinBuff[pruNum]
                              ,object.buffLen[pruNum]
                              , pruNum
                              );
            }
        else
            {   
            pruCoreWriteMemory(pDev
                              ,pResAdr->virtual
                              ,0
                              ,(uint32_t *) object.pruBinBuff[pruNum]
                              ,object.buffLen[pruNum]
                              , pruNum
                              );
            }
            
        pruCoreEnable(pDev,pruNum);

        return OK;
    }

/******************************************************************************
*
* pruCoreDisable - Disables PRU Cores 0/1
*
* RETURNS: OK.  ERROR otherwise.
*
* ERRNO: N/A
*/
    
LOCAL STATUS pruCoreDisable
    (VXB_DEV_ID pDev
    ,int pruNum
    )
    {
    uint32_t mask   = PRUSS_SYSCFG_STANDBY_MODE_MASK;
    uint32_t val    = 0;
    uint32_t set    = PRUSS_SYSCFG_STANDBY_MODE_SMART;

    VXB_FDT_DEV *           pFdtDev;
            
    pFdtDev = vxbFdtDevGet (pDev);
    
    if (pFdtDev == NULL)
        {
        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "FDT IS NULL\n");
        return ERROR;
        }

    val = vxbRead32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase);

    val &= ~mask;
    val |= (set & mask);

    if(pruNum == 0)
        {
 		 PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO,  "\n\t\t\tSetting up PRUICSS for Firmware loading. PRU[%d] is disabled!\n", pruNum);
		 val = 0;
		 vxbWrite32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase, val);
		 
		 val = vxbRead32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase);
        }
    else
        {	
		 PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO,  "\n\t\t\tSetting up PRUICSS for Firmware loading. PRU[%d] is disabled!\n", pruNum);
		 val = 0;
		 vxbWrite32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase, val);
		 
		 val = vxbRead32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase);
        }

    return OK;    
    }


/******************************************************************************
*
* pruCoreEnable - Enables PRU Cores 0/1
*
* RETURNS: OK.  ERROR otherwise.
*
* ERRNO: N/A
*/
    
LOCAL STATUS pruCoreEnable
    (VXB_DEV_ID pDev
    ,int pruNum
    )
    {
    
	uint32_t mask = PRUSS_SYSCFG_STANDBY_MODE_MASK;
	uint32_t val = vxbRead32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase);
	uint32_t set = PRUSS_SYSCFG_STANDBY_MODE_SMART;

	//PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO,  "%s\n", __FUNCTION__);

	val &= ~mask;
	val |= (set & mask);

	if(pruNum == 0)
		{
		 PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO,  "\n\t\t\t**********Boot Complete!**********\n\n");
		 PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO,  "\n\t\t\tPRUICSS Firmware is loaded. PRU[%d] is enabled!\n", pruNum);
		 val = vxbRead32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase);
		 val |= 0x2;
		 vxbWrite32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase, val);
		 
		 val = vxbRead32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase);

		 /* Read the Program Counter Value */

		 val = vxbRead32(pDrvCtrl->pCtrlregHandle,((uint32_t*)pDrvCtrl->pCtrlregBase)+0x1);
		 PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\t\t\tPROGRAM COUNTER of PRU Core[%d] : 0x%x\n", pruNum, val);
	
		}
	else
		{
		 PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO,  "\n\t\t\t**********Boot Complete!**********\n\n");
		 PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO,  "\n\t\t\tPRUICSS Firmware is loaded. PRU[%d] is enabled!\n", pruNum);

		 val = vxbRead32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase);
		 val |= 0x2;
		 vxbWrite32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase, val);
		 
		 val = vxbRead32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase);

		 /* Read the Program Counter Value */
		 
		 val = vxbRead32(pDrvCtrl->pCtrlregHandle,((uint32_t*)pDrvCtrl->pCtrlregBase)+0x1);
		 PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\t\t\tPROGRAM COUNTER of PRU Core[%d] : 0x%x\n", pruNum, val);
		}

    return OK;
    }


/******************************************************************************
*
* pruCoreWriteMemory - Write firmware onto PRU Cores 0/1
*
* RETURNS: OK.  ERROR otherwise.
*
* ERRNO: N/A
*/
    
LOCAL STATUS pruCoreWriteMemory
    (VXB_DEV_ID pDev
    ,uint32_t   pruMem
    ,uint32_t   wordoffset
    ,uint32_t * source_mem
    ,uint32_t   bytelength
    ,int        pruNum
    )
   {
    uint32_t                baseaddr;
    uint32_t                wordlength, val;
    int                     regIndex;
    VXB_FDT_DEV *           pFdtDev;
    VXB_RESOURCE_ADR *      pResAdr = NULL;
    VXB_RESOURCE *          pRes    = NULL;

    PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\t\t\t%s\n", __FUNCTION__);

    wordlength = (bytelength + 3) >> 2;

            pFdtDev = vxbFdtDevGet (pDev);
    
    if (pFdtDev == NULL)
        {
        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "FDT IS NULL\n");
        return ERROR;
        }

    /* Retrieve IRAM Register base address for writing firmware onto PRU Cores */

    regIndex = pruCoreStringLookup(pFdtDev->offset, "reg-names", "iram");
    if (regIndex >= 0)
        {
        pRes = vxbResourceAlloc(pDrvCtrl->pDev, VXB_RES_MEMORY, regIndex);
        if (pRes == NULL || VXB_RES_TYPE(pRes->id) != VXB_RES_MEMORY)
            {
            PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "PRU ESC->(%s) iram       pRes error(dram0)\n",pFdtDev->name);
            return ERROR;
            }
        pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
        if (pResAdr == NULL)
            {
            PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "PRU ESC->(%s) iram   pResAdr is NULL(dram0)\n",pFdtDev->name);
            return ERROR;
            }
    
        /* save the resource info */
        
        pDrvCtrl->pIramregHandle  = pResAdr->pHandle;
        pDrvCtrl->pIramregBase = (void*)pResAdr->virtual;        

        }
    else
        {
        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "Invalid Reg lookup of dram0 (%s), regIndex=%d\n",pFdtDev->name, regIndex);
        }

    PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\t\t\tLoading the firmware on PRU[%d]..\e[s\n", pruNum);
    
	for (regIndex = 0; regIndex <= wordlength; regIndex++)
	{
			{               
			vxbWrite32(pResAdr->pHandle,(uint32_t*)pResAdr->virtual+(regIndex), source_mem[regIndex]);
			val = vxbRead32(pResAdr->pHandle,(((uint32_t*)(pResAdr->virtual))+(regIndex << 1)));
			int pct = ((float) regIndex / wordlength) * 100;

			//PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\t\t\t\t\t\t\t**********flashing.. %d %%\r", pct);
			}
			 
	}
	PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\n");
        
        return wordlength;
    }

/******************************************************************************
 *
 * pruCoreStringLookup - find string in a list and return index.
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
LOCAL STATUS pruCoreStringLookup
    (
    int     offset,
    char  * propName,
    char  * string
    )
    {
    char  * pPropStr;
    int     propLen, strLen;
    int     ix = 0;

    if (propName == NULL || string == NULL)
        {
        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "parameter error.\n");
        return (-1);
        }

    pPropStr = (char*)vxFdtPropGet (offset, propName, &propLen);
    if (pPropStr == NULL)
        {
        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "search %s error.\n",propName);
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
    PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "not found %s error.\n",propName);
    return (-1);    
    }

/******************************************************************************
*
* pruCoreReset - Reset PRU Cores 0/1
*
* RETURNS: OK.  ERROR otherwise.
*
* ERRNO: N/A
*/

LOCAL STATUS pruCoreReset
    (VXB_DEV_ID     pDev
    ,int            pruNum)
    {
        int val, shift, mask = 0;
        PRUICSS_CORE_DBG_MSG ( PRUICSS_CORE_DBG_INFO, "\n\n\t\t\tResetting PRU Core: %d\n", pruNum);
        
        val = vxbRead32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase);

        mask    =   PRU_CORE_CTRL_SOFT_RST_N_MASK;
        shift   =   PRU_CORE_CTRL_SOFT_RST_N_SHIFT;
        val     &=  (~mask);
        val     |=  (val << shift) & mask;
        vxbWrite32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase, val);
        val = vxbRead32(pDrvCtrl->pCtrlregHandle,(uint32_t*)pDrvCtrl->pCtrlregBase);

        if(val == 0)
            return OK;
        else
            return ERROR;
    }


/*******************************************************************************
*
* ReadByte_SHdram - Read byte from the SHDRAM at the offset 
* 					provided.
*               
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS  ReadByte_SHdram
    (
     VXB_DEV_ID pDev,
	 uint32_t offset
    )
{
	uint8_t val;
	PRUICSS_CORE_CTRL *pDrvCtrl = NULL;
	
	if (pDev == NULL)
		        {
		        return ERROR;
		        }
		    
		    
		    pDrvCtrl = (PRUICSS_CORE_CTRL *)vxbDevSoftcGet (pDev);
		        if (pDrvCtrl == NULL)
		            {
		            return ERROR;
		            }
		        
		        /* Shared RAM Base Address */
		        uint8_t * Revaddress   = (uint8_t *)pDrvCtrl->pPsdrmRegBase;
		        val = vxbRead8(pDrvCtrl->pPsdrmRegHandle,((Revaddress + offset)));
		        return val;
		        }

/*******************************************************************************
*
* ReadWord_SHdram - Read two bytes(a word) from the SHDRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS  ReadWord_SHdram
    (
     VXB_DEV_ID pDev,
	 uint32_t offset
    )
{
	uint16_t val;
	PRUICSS_CORE_CTRL *pDrvCtrl = NULL;
	
	if (pDev == NULL)
		        {
		        return ERROR;
		        }
		    
		    
		    pDrvCtrl = (PRUICSS_CORE_CTRL *)vxbDevSoftcGet (pDev);
		        if (pDrvCtrl == NULL)
		            {
		            return ERROR;
		            }
		        
		        /* Shared RAM Base Address */
		        uint8_t * Revaddress   = (uint8_t *)pDrvCtrl->pPsdrmRegBase;
		        val = vxbRead16(pDrvCtrl->pPsdrmRegHandle,((UINT16 *)(Revaddress + offset)));
		        return val;
}

/*******************************************************************************
*
* ReadDword_SHdram - Read four byte (dword)  from the SHDRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS  ReadDword_SHdram
    (
     VXB_DEV_ID pDev,
	 uint32_t offset
    )
{
	uint32_t val;
	PRUICSS_CORE_CTRL *pDrvCtrl = NULL;
	
	if (pDev == NULL)
		        {
		        return ERROR;
		        }
		    
		    
		    pDrvCtrl = (PRUICSS_CORE_CTRL *)vxbDevSoftcGet (pDev);
		        if (pDrvCtrl == NULL)
		            {
		            return ERROR;
		            }
		        
		        /* Shared RAM Base Address */
		        uint8_t * Revaddress   = (uint8_t *)pDrvCtrl->pPsdrmRegBase;
		        val = vxbRead32(pDrvCtrl->pPsdrmRegHandle,((UINT32 *)(Revaddress + offset)));
		        return val;
}

/*******************************************************************************
*
* WriteByte_SHdram - Write byte to the SHDRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS  WriteByte_SHdram
    (
     VXB_DEV_ID pDev,
	 uint32_t offset, uint8_t data
    )
{
	PRUICSS_CORE_CTRL *pDrvCtrl = NULL;
	
	if (pDev == NULL)
		        {
		        return ERROR;
		        }
		    
		    
		    pDrvCtrl = (PRUICSS_CORE_CTRL *)vxbDevSoftcGet (pDev);
		        if (pDrvCtrl == NULL)
		            {
		            return ERROR;
		            }
		        
		        /* Shared RAM Base Address */
		        uint8_t * Revaddress   = (uint8_t *)pDrvCtrl->pPsdrmRegBase;
		        vxbWrite8(pDrvCtrl->pPsdrmRegHandle,((Revaddress + offset)),data);
		        
		        return OK;
}

/*******************************************************************************
*
* WriteWord_SHdram - Write two byte(word) to the SHDRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS  WriteWord_SHdram
    (
     VXB_DEV_ID pDev,
	 uint32_t offset, uint16_t data
    )
{
	PRUICSS_CORE_CTRL *pDrvCtrl = NULL;
	
	if (pDev == NULL)
		        {
		        return ERROR;
		        }
		    
		    
		    pDrvCtrl = (PRUICSS_CORE_CTRL *)vxbDevSoftcGet (pDev);
		        if (pDrvCtrl == NULL)
		            {
		            return ERROR;
		            }
		        
		        /* Shared RAM Base Address */
		        uint8_t * Revaddress   = (uint8_t *)pDrvCtrl->pPsdrmRegBase;
		        vxbWrite16(pDrvCtrl->pPsdrmRegHandle,((UINT16 *)(Revaddress + offset)),data);
		        return OK;
}

/*******************************************************************************
*
* WriteDword_SHdram - Write four byte(dword) to the SHDRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS  WriteDword_SHdram
    (
     VXB_DEV_ID pDev,
	 uint32_t offset, uint32_t data
    )
{
	PRUICSS_CORE_CTRL *pDrvCtrl = NULL;
	
	if (pDev == NULL)
		        {
		        return ERROR;
		        }
		    
		    
		    pDrvCtrl = (PRUICSS_CORE_CTRL *)vxbDevSoftcGet (pDev);
		        if (pDrvCtrl == NULL)
		            {
		            return ERROR;
		            }
		        
		        /* Shared RAM Base Address */
		        uint8_t * Revaddress   = (uint8_t *)pDrvCtrl->pPsdrmRegBase;
		        vxbWrite32(pDrvCtrl->pPsdrmRegHandle,((UINT32 *)(Revaddress + offset)),data);
		        return OK;
}

/*******************************************************************************
*
* ReadBlock_SHdram - Read four byte (dword)  from the SHDRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS ReadBlock_SHdram
(
     VXB_DEV_ID pDev,
	 uint32_t offset, uint8_t *pData, uint32_t len
    )
{
	uint32_t i;
	PRUICSS_CORE_CTRL *pDrvCtrl = NULL;
	
	if (pDev == NULL)
			        {
			        return ERROR;
			        }
			    
			    
			    pDrvCtrl = (PRUICSS_CORE_CTRL *)vxbDevSoftcGet (pDev);
			        if (pDrvCtrl == NULL)
			            {
			            return ERROR;
			            }
			        
	uint8_t * Revaddress   = (uint8_t *)pDrvCtrl->pPsdrmRegBase;
	for(i = 0; i < len; i++)
	{
		*(pData + i) = vxbRead8(pDrvCtrl->pPsdrmRegHandle,((Revaddress + offset + i)));
	}
	return OK;
	
}

/*******************************************************************************
*
* WriteBlock_SHdram - Write four byte(dword) to the SDRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS WriteBlock_SHdram
(
     VXB_DEV_ID pDev,
	 uint32_t offset, uint8_t *pData, uint32_t len
    )
{
	uint32_t i;
	uint8_t temp;
	PRUICSS_CORE_CTRL *pDrvCtrl = NULL;
	
	if (pDev == NULL)
			        {
			        return ERROR;
			        }
			    
			    
			    pDrvCtrl = (PRUICSS_CORE_CTRL *)vxbDevSoftcGet (pDev);
			        if (pDrvCtrl == NULL)
			            {
			            return ERROR;
			            }
			        
   	uint8_t * Revaddress   = (uint8_t *)pDrvCtrl->pPsdrmRegBase;
   	for(i = 0; i < len; i++)
	{
				vxbWrite8(pDrvCtrl->pPsdrmRegHandle,((Revaddress + offset + i)),(*(pData+i)));
	}
   	return OK;
}

/*******************************************************************************
*
* ReadRange_SHdram - Read range of bytes  from the SharedRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
STATUS ReadRange_SHdram
(
     VXB_DEV_ID pDev,
	 uint32_t offset, uint32_t len
    )
{
	uint32_t i;
	PRUICSS_CORE_CTRL *pDrvCtrl = NULL;
	
	if (pDev == NULL)
			        {
			        return ERROR;
			        }
			    
			    
			    pDrvCtrl = (PRUICSS_CORE_CTRL *)vxbDevSoftcGet (pDev);
			        if (pDrvCtrl == NULL)
			            {
			            return ERROR;
			            }
			        
	uint8_t * Revaddress   = (uint8_t *)pDrvCtrl->pPsdrmRegBase;
	for(i = 0; i < len; i++)
	{
		kprintf("Reg @0x%d : 0x%x\n",i,vxbRead8(pDrvCtrl->pPsdrmRegHandle,((Revaddress + offset + i))));
	}
		
	return OK;
}
