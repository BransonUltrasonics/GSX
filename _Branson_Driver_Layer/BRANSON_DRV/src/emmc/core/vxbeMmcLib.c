/* vxbeMmcLib.c - EMMC bus Library */

/*
 * Copyright TBD
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
13aug18,r_r  create, add eMMC support. Driver adapted from Windriver SD Card driver
*/

/*
DESCRIPTION

This is the generic EMMC bus library to implement neccessary commands for
a eMMC device, specification 4.0 and above.

*/

/* includes */

#include <vxWorks.h>
#include <intLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cacheLib.h>
#include <iv.h>

#include <hwif/vxBus.h>
#include <hwif/vxbus/vxbBusType.h>
#include <subsys/int/vxbIntLib.h>
#include <subsys/int/vxbDyncIntLib.h>
#include <subsys/timer/vxbTimerLib.h>


#include <vxbeMmcLib.h>

/* defines */

#ifndef  EMMC_LIB_DBG_ON

#ifdef  LOCAL
#undef  LOCAL
#define LOCAL
#endif /* LOCAL */

#define EMMC_LIB_DBG_BUSINIT       0x00000001
#define EMMC_LIB_DBG_CMD           0x00000002
#define EMMC_LIB_DBG_ERR           0x00000004
#define EMMC_LIB_DBG_INT           0x00000008
#define EMMC_LIB_DBG_ALL           0xffffffff
#define EMMC_LIB_DBG_OFF           0x00000000

UINT32 emmcLibDbgMask = EMMC_LIB_DBG_OFF;

IMPORT FUNCPTR _func_logMsg;

#define EMMC_LIB_DBG(mask, string, a, b, c, d, e, f)			\
    if ((emmcLibDbgMask & mask) || (mask == EMMC_LIB_DBG_ALL))	\
        if (_func_logMsg != NULL) 								\
            (* _func_logMsg)(string, a, b, c, d, e, f)

#else
#   define EMMC_LIB_DBG(mask, string, a, b, c, d, e, f)
#endif

BUSTYPE_DEF(vxbeMmcBus, "eMmc bus type");

/*******************************************************************************
*
* emmcDevCreate - notify the bus subsystem of a device on SD bus
*
* This routine send SDIO cmd5 to target and check target's type. And store the
* cmdIssue pointer to SD_HARDWARE struct. The SD_HARDWARE sturct is stored to
* the pVxbIvars field of VxBus device structure.
* finally, emmcDevCreate is called to announce the device to the system.
*
* RETURNS: OK, or ERROR
*
* ERRNO
*/

STATUS emmcDevCreate
    (
    VXB_DEV_HANDLE pInst,
    void * pArg
    )
    {
    VXB_DEV_ID      pDev = NULL;
    SD_HARDWARE   * pSdHardWare;
    SD_HOST_SPEC  * pSdHostSpec;
    int i = 0;
    STATUS rc;
    UINT32  capbility;
    UINT32 timeOut = 0;

    /* initialize generic bus info */
    rc = vxbDevCreate (&pDev);

    if (rc != OK)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_ERR,
                    "sdDeviceAnnounce() - vxbDevStructAlloc not success\n",
                    0, 0, 0, 0, 0, 0);
        return(ERROR);
        }

    /* initialize bus-specific info */

    pSdHardWare = (SD_HARDWARE *) vxbMemAlloc(sizeof (SD_HARDWARE));
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_ERR,
                    "sdDeviceAnnounce - hwMemAlloc not success for pSdHardWare\n",
                    0, 0, 0, 0, 0, 0);
        vxbMemFree(pDev);
        return(ERROR);
        }

    rc = VXB_SDSPECINFO_GET(pInst, &(pSdHardWare->pHostSpec), &(pSdHardWare->vxbSdCmdIssue));
    if (rc != OK)
        {
        EMMC_LIB_DBG(EMMC_LIB_DBG_ERR,
                   "sdDeviceAnnounce - VXB_SDSPECINFO_GET not success\n",
                   0, 0, 0, 0, 0 ,0);
        vxbMemFree(pDev);
        vxbMemFree(pSdHardWare);
        return(ERROR);
        }

    vxbDevClassSet(pDev, VXB_BUSID_SDMMC);

    vxbDevIvarsSet(pDev,pSdHardWare);

    pSdHostSpec = (SD_HOST_SPEC *)(pSdHardWare->pHostSpec);
    if (pSdHostSpec == NULL)
        {
        EMMC_LIB_DBG(EMMC_LIB_DBG_ERR, "sdDeviceAnnounce - pSdHostSpec null\n",
                0, 0, 0, 0, 0, 0);
        vxbMemFree(pDev);
        vxbMemFree(pSdHardWare);
        return(ERROR);
        }

    pSdHostSpec->childDev = pDev;
    capbility = pSdHostSpec->capbility;

    /* get host controller unit number */
    pSdHostSpec->unitNumber = vxbDevUnitGet(pInst);

    /* save interrupt info in vxbDev structure */
    pSdHardWare->pHostDev = pInst;

    /* CMD Line start: CMD0 */
    rc = emmcCmdGoIdleState (pDev);
    if (rc == ERROR)
        {

        EMMC_LIB_DBG(EMMC_LIB_DBG_ERR,
                   "sdDeviceAnnounce - emmcCmdGoIdleState not success\n\n",
                   0, 0, 0, 0, 0, 0);
        vxbMemFree(pDev);
        vxbMemFree(pSdHardWare);
        return(ERROR);
        }
    vxbMsDelay(1); /* Wait 1 ms */
    
    /* CMD Line start: CMD5 */
    rc = emmcCmdIoSendOpCond(pDev, capbility & OCR_VDD_VOL_MASK);

    if ((pSdHardWare->cmdErr & SDMMC_CMD_ERR_TIMEOUT) != 0x0)
        {

        /* To conform to JEDEC specification */
    	/* CMD Line start: CMD1 */
        rc = emmcCmdSendOpCond(pDev, 0xC0FF8080);
        if ((rc == ERROR) ||
            ((pSdHardWare->cmdErr & SDMMC_CMD_ERR_TIMEOUT) != 0x0))
            pSdHardWare->isMmc = FALSE;
        else
            pSdHardWare->isMmc = TRUE;

        if (pSdHardWare->isMmc)
            {
            emmcCmdGoIdleState(pDev);
            while (timeOut++ < SDMMC_COMMAND_TIMEOUT)
                {
                rc = emmcCmdSendOpCond(pDev,
                                        (pSdHardWare->ocrValue));
                /* See if 31st bit is set */
                if (/*(rc == OK) &&*/
                    ((pSdHardWare->ocrValue & (UINT32)OCR_CARD_PWRUP_STS) != 0x0))
                    {
                    pSdHardWare->voltage = (pSdHardWare->ocrValue) & OCR_VDD_VOL_MASK;
                    break;
                    }
                vxbMsDelay(1);
                }
            /* Set device storage name - "emmcStorage" */
            vxbDevNameSet (pDev, EMMC_CARD_NAME, TRUE);
            }
        else
            {
            (void)emmcCmdSendIfCond (pDev, capbility);
            if (pSdHardWare->version == SD_VERSION_UNKNOWN)
                {
                EMMC_LIB_DBG(EMMC_LIB_DBG_ERR,
                           "sdDeviceAnnounce - emmcCmdGoIdleState not success\n\n",
                           0, 0, 0, 0, 0, 0);
                vxbMemFree(pDev);
                vxbMemFree(pSdHardWare);
                return(ERROR);
                }
            while (timeOut++ < SDMMC_COMMAND_TIMEOUT)
                {
                rc = emmcACmdSendOpCond (pDev, capbility, pSdHardWare->version);

                if ((rc == OK) && ((pSdHardWare->ocrValue & (UINT32)OCR_CARD_PWRUP_STS) != 0x0))
                    {
                    pSdHardWare->voltage = (pSdHardWare->ocrValue) & OCR_VDD_VOL_MASK;
                    break;
                    }
                vxbMsDelay(1);
                }
            if (timeOut >= SDMMC_COMMAND_TIMEOUT)
                {
                EMMC_LIB_DBG(EMMC_LIB_DBG_ERR,
                           "sdDeviceAnnounce - emmcACmdSendOpCond not success\n",
                           0, 0, 0, 0, 0, 0);
                vxbMemFree(pDev);
                vxbMemFree(pSdHardWare);
                return(ERROR);
                }
            vxbDevNameSet (pDev, SD_CARD_NAME, TRUE);
            }

            pSdHardWare->isComboCard = FALSE;
            pSdHardWare->isSdio = FALSE;

            rc = vxbDevAdd (pInst, pDev);
            if (rc == ERROR)
                {
                EMMC_LIB_DBG(EMMC_LIB_DBG_ERR,
                           "sdDeviceAnnounce - vxbDevAdd not success\n",
                           0, 0, 0, 0, 0, 0);
                vxbMemFree(pDev);
                vxbMemFree(pSdHardWare);
                return(ERROR);
                }

            EMMC_LIB_DBG (EMMC_LIB_DBG_ERR, "sdioDeviceAnnounce - ok\n",
                        0, 0, 0, 0, 0, 0);

        /* coverity[leaked_storage] */

        /*
         * pDev and pSdHardWare are used with vxBus system,
         * so we can't free it. Set coverity keyword
         */

        return OK;
        }

    if (rc == OK)
        {
        UINT8 funcNum = 0;

        funcNum = (pSdHardWare->ocrValue >> 28) & 0x07;

        if (funcNum > 0)
            {
            rc = emmcCmdIoSendOpCond(pDev, capbility & 0x00FFFFFF);
            if (rc == ERROR)
                {
                EMMC_LIB_DBG (EMMC_LIB_DBG_ERR,
                            "sdDeviceAnnounce - emmcCmdIoSendOpCond not success\n",
                            0, 0, 0, 0, 0, 0);
                vxbMemFree(pDev);
                vxbMemFree(pSdHardWare);
                return(ERROR);
                }
            do{
               rc = emmcCmdSendRelativeAddr (pDev);
               if (rc == ERROR)
                   {
                   EMMC_LIB_DBG (EMMC_LIB_DBG_ERR,
                               "sdDeviceAnnounce - emmcCmdSendRelativeAddr not success\n",
                               0, 0, 0, 0, 0, 0);
                   vxbMemFree(pDev);
                   vxbMemFree(pSdHardWare);
                   return(ERROR);
                   }
               } while (pSdHardWare->rcaValue == 0);

            rc = emmcCmdSelectCard (pDev);
            if (rc == ERROR)
                {
                EMMC_LIB_DBG (EMMC_LIB_DBG_ERR,
                            "sdDeviceAnnounce - emmcCmdSelectCard not success\n",
                            0, 0, 0, 0, 0, 0);
                vxbMemFree(pDev);
                vxbMemFree(pSdHardWare);
                return(ERROR);
                }

            for (i = 0; i < funcNum; i++)
                {
                if (pDev == NULL)
                    {
                    rc = vxbDevCreate (&pDev);
                    if (rc == ERROR)
                        {
                        EMMC_LIB_DBG (EMMC_LIB_DBG_ERR,
                                    "sdDeviceAnnounce - vxbDevCreate not success\n",
                                    0, 0, 0, 0, 0, 0);
                        vxbMemFree(pSdHardWare);
                        return(ERROR);
                        }
                    }
                if (pSdHardWare == NULL)
                    {
                    pSdHardWare = (SD_HARDWARE *) vxbMemAlloc(sizeof (SD_HARDWARE));

                    if (pSdHardWare == NULL)
                        {
                        EMMC_LIB_DBG (EMMC_LIB_DBG_ERR,
                                    "sdDeviceAnnounce - hwMemAlloc not success for pSdHardWare\n",
                                    0, 0, 0, 0, 0, 0);
                        vxbMemFree(pDev);
                        return(ERROR);
                        }

                    rc = VXB_SDSPECINFO_GET(pInst, &(pSdHardWare->pHostSpec), &(pSdHardWare->vxbSdCmdIssue));
                    if (rc != OK)
                        {
                        EMMC_LIB_DBG (EMMC_LIB_DBG_ERR,
                                    "sdDeviceAnnounce - VXB_SDSPECINFO_GET not success\n",
                                    0, 0, 0, 0, 0, 0);
                        vxbMemFree(pDev);
                        vxbMemFree(pSdHardWare);
                        return(ERROR);
                        }

                    vxbDevClassSet(pDev, VXB_BUSID_SDMMC);

                    vxbDevIvarsSet(pDev,pSdHardWare);

                    pSdHostSpec = (SD_HOST_SPEC *)(pSdHardWare->pHostSpec);
                    if (pSdHostSpec == NULL)
                        {
                        EMMC_LIB_DBG (EMMC_LIB_DBG_ERR,
                                    "sdDeviceAnnounce - pSdHostSpec null\n",
                                    0, 0, 0, 0, 0, 0);
                        vxbMemFree(pDev);
                        vxbMemFree(pSdHardWare);
                        return(ERROR);
                        }

                    capbility = pSdHostSpec->capbility;
                    }

                pSdHardWare->isSdio = TRUE;
                pSdHardWare->isComboCard = FALSE;
                pSdHardWare->funcNum = i + 1;

                /* Need not check return status at here */

                rc = vxbDevAdd (pInst, pDev);
                if (rc == ERROR)
                    {
                    EMMC_LIB_DBG (EMMC_LIB_DBG_ERR,
                                "sdDeviceAnnounce - vxbDevAdd not success\n",
                                0, 0, 0, 0, 0, 0);
                    vxbMemFree(pDev);
                    vxbMemFree(pSdHardWare);
                    return(ERROR);
                    }

                pDev = NULL;
                pSdHardWare = NULL;
                }
            }
        }

    return OK;
    }

/*******************************************************************************
*
* emmcCmdGoIdleState - issue CMD0 to reset all cards to idle state
*
* This routine issues CMD0 to reset all cards to idle state.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdGoIdleState
    (
    VXB_DEV_HANDLE pDev
    )
    {
    SD_CMD cmd;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdGoIdleState() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pParent = pSdHardWare->pHostDev;

    SD_CMD_FORM (cmd, SDMMC_CMD_GO_IDLE_STATE, 0, SDMMC_CMD_RSP_NONE, FALSE);

    EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdGoIdleState command[0]\n", 0, 0, 0, 0, 0, 0);

    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);

    /* Need at last 1ms delay for waiting for CMD0 respond */
    vxbMsDelay(1);
    
    return (rc);
    }

/*******************************************************************************
*
* emmcCmdSendOpCond - issue CMD1 to get mmc card OCR value
*
* This routine issues CMD1 to get mmc card OCR value.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSendOpCond
    (
    VXB_DEV_HANDLE pDev,
    UINT32 ocrValue
    )
    {
    SD_CMD cmd;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdSendOpCond() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    SD_CMD_FORM (cmd, MMC_CMD_SEND_OP_COND, ocrValue, SDMMC_CMD_RSP_R3, FALSE);

    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    pSdHardWare->cmdErr = cmd.cmdErr;
    pSdHardWare->dataErr = cmd.dataErr;

    if (rc == ERROR)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSendOpCond() - cmdIssue return error\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pSdHardWare->ocrValue = cmd.cmdRsp[0];
    EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                "emmcCmdSendOpCond() - ocrValue = 0X%x\n",
                pSdHardWare->ocrValue, 0, 0, 0, 0, 0);
    
    return (rc);
    }

/*******************************************************************************
*
* emmcCmdAllSendCid - issue CMD2 to ask all cards to send their CIDs
*
* This routine issues CMD2 to ask all cards to send their CIDs.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdAllSendCid
    (
    VXB_DEV_HANDLE pDev,
    void * pCid
    )
    {
    SD_CMD cmd;
    STATUS rc;
    VXB_DEV_HANDLE pParent;
    SD_HARDWARE * pSdHardWare;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdAllSendCid() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    if (pCid == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdAllSendCid() - pCid is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    SD_CMD_FORM (cmd, SDMMC_CMD_ALL_SEND_CID, 0, SDMMC_CMD_RSP_R2, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    if (rc == ERROR)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdAllSendCid() - cmdIssue return error\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    bcopy ((char *)cmd.cmdRsp, (char *)pCid, CID_SIZE);

    return OK;
    }

/*******************************************************************************
*
* emmcCmdSendRelativeAddr - issue CMD3 to ask card to publish a new RCA
*
* This routine issues CMD3 to ask card to publish a new RCA.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSendRelativeAddr
    (
    VXB_DEV_HANDLE pDev
    )
    {
    SD_CMD cmd;
    UINT16 rcaTemp = 0;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSendRelativeAddr() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    SD_CMD_FORM (cmd, SD_CMD_SEND_RELATIVE_ADDR, 0, SDMMC_CMD_RSP_R6, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    if (rc == ERROR)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSendRelativeAddr() - cmdIssue return error\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    rcaTemp = (UINT16)((cmd.cmdRsp[0] >> 16) & 0xffff);
    pSdHardWare->rcaValue = rcaTemp;

    EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSendRelativeAddr() - rcaValue = 0X%x\n",
                0, 0, 0, 0, 0, 0);
    return (OK);
    }

/*******************************************************************************
*
* emmcCmdSetRelativeAddr - issue CMD3 to ask card to publish a new RCA
*
* This routine issues CMD3 to ask card to publish a new RCA.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSetRelativeAddr
    (
    VXB_DEV_HANDLE pDev
    )
    {
    SD_CMD cmd;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdSendRelativeAddr() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pParent = pSdHardWare->pHostDev;

    if (pSdHardWare->rcaValue == 0x0)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdSendRelativeAddr() - pSdHardWare rcaValue is 0x0\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    
    EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                        "pSdHardWare->rcaValue:%d\n",
						pSdHardWare->rcaValue, 0, 0, 0, 0, 0);

    SD_CMD_FORM (cmd, MMC_CMD_SET_RELATIVE_ADDR, (UINT32)(pSdHardWare->rcaValue << 16),
                 SDMMC_CMD_RSP_R1, FALSE);

    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    if (rc == ERROR)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdSendRelativeAddr() - cmdIssue return error\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    return (OK);
    }

/*******************************************************************************
*
* emmcCmdSetDsr - issue CMD4 to program the DSR of all cards
*
* This routine issues CMD4 to program the DSR of all cards.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSetDsr
    (
    VXB_DEV_HANDLE pDev,
    UINT32        dsr
    )
    {
    SD_CMD cmd;

    SD_CMD_FORM (cmd, SDMMC_CMD_SET_DSR, dsr << 16, SDMMC_CMD_RSP_NONE, FALSE);

    return (emmcCmdIssue(pDev, &cmd));
    }

/*******************************************************************************
*
* emmcCmdIoSendOpCond - issue CMD5 to ask SDIO card to publish OCR value
*
* This routine issues CMD5 to ask SDIO card to publish OCR value.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdIoSendOpCond
    (
    VXB_DEV_HANDLE pDev,
    UINT32 ocrValue
    )
    {
    SD_CMD cmd;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdIoSendOpCond() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    SD_CMD_FORM (cmd, SDIO_CMD_SEND_OPCOND, ocrValue, SDMMC_CMD_RSP_R4, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    pSdHardWare->cmdErr = cmd.cmdErr;
    pSdHardWare->dataErr = cmd.dataErr;

    if (rc == ERROR)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdIoSendOpCond() - cmdIssue return error\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pSdHardWare->ocrValue = cmd.cmdRsp[0];
    EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdIoSendOpCond() - ocrValue = 0X%x\n",
                0, 0, 0, 0, 0, 0);

    return (OK);
    }

/*******************************************************************************
*
* emmcSwitch - issue CMD6 to write MMC card's ext_csd register.
*
* This routine issue CMD6 to read/write MMC card's ext_csd register. Index is
* byte offset address in ext_csd register. Data is wrote to ext_csd register's
* value. Set is operation attribute.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcSwitch
    (
    VXB_DEV_HANDLE pDev,
    UINT8  index,
    UINT8  data,
    UINT8  set
    )
    {
    SD_CMD cmd;
    STATUS rc;
    UINT32 arg = 0;
    UINT32 rspValue = 0;
    UINT32 timeOut = 0;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcSwitch() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pParent = pSdHardWare->pHostDev;

    arg = (MMC_SWITCH_MODE_WRITE_BYTE << 24) |
           (index << 16) |
           (data << 8) |
           set;

    SD_CMD_FORM (cmd, MMC_CMD_SWITCH, arg, SDMMC_CMD_RSP_R1B, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    if (rc == ERROR)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcSwitch() - cmdIssue return error\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    do
        {

        /* Need not check return status */

        (void)emmcCmdSendStatus (pDev, &rspValue);
        timeOut++;
        if (timeOut >= SDMMC_COMMAND_TIMEOUT)
            return (ERROR);
        }while(CARD_STS_CUR_GET(rspValue) == CARD_STS_PRG);

    return (OK);
    }

/*******************************************************************************
*
* emmcACmdSwitchFunc - send application specific command 6
*
* This routine sends application specific command 6. The input buffer and
* buffer length are provided by the caller.
*
* RETURNS: OK, or ERROR if command issue failed
*
* ERRNO: N/A
*
* \NOMANUAL
*/

STATUS emmcACmdSwitchFunc
    (
    VXB_DEV_HANDLE pDev,
    void * buffer,
    UINT32 len,
    int    mode,
    int    group,
    int    val
    )
    {
    SD_CMD cmd;
    STATUS rc;
    UINT32 arg = 0;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcACmdSwitchFunc() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    arg = mode;
    arg |= 0xffffff;
    arg &= ~(0xf << (group * 4));
    arg |= (val << (group * 4));

    SD_CMD_FORM (cmd, SD_CMD_SWITCH_FUNC, arg, SDMMC_CMD_RSP_R1, TRUE);
    SD_DATA_FORM (cmd, buffer, 1, len, TRUE);

    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    if (rc == ERROR)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcACmdSwitchFunc() - cmdIssue return error\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pSdHardWare->cmdErr = cmd.cmdErr;
    pSdHardWare->dataErr = cmd.dataErr;

    return OK;
    }

/*******************************************************************************
*
* emmcCmdSelectCard - issue CMD7 to select a card
*
* This routine issues CMD7 to select a card.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSelectCard
    (
    VXB_DEV_HANDLE pDev
    )
    {
    SD_CMD cmd;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;
    STATUS rc;
    UINT32 i;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdIoSendOpCond() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pParent = pSdHardWare->pHostDev;

    SD_CMD_FORM (cmd, SDMMC_CMD_SELECT_CARD,
                 SDMMC_CMD_ARG_RCA(pSdHardWare->rcaValue),
                 SDMMC_CMD_RSP_R1B, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    for (i = 0; i < SDMMC_CMD_RETRY; i++)
        {
        rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
        if (rc == ERROR)
			{
				continue;
			}
			else
			{
				break;
			}
        }

    return rc;
    }

/*******************************************************************************
*
* emmcCmdDeselectCard - issue CMD7 to deselect a card
*
* This routine issues CMD7 to deselect a card.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdDeselectCard
    (
    VXB_DEV_HANDLE pDev
    )
    {
    SD_CMD cmd;

    SD_CMD_FORM (cmd, SDMMC_CMD_SELECT_CARD, 0, SDMMC_CMD_RSP_R1B, FALSE);

    return (emmcCmdIssue(pDev, &cmd));
    }

/*******************************************************************************
*
* sdMmcACmdSendScr - issue ACMD51 to ask card to send its SCR
*
* This routine issues ACMD51 to ask card to send its SCR.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcSendTuning
    (
    VXB_DEV_HANDLE pDev
    )
    {
    STATUS rc;
    UINT8 *sendData;
    SD_CMD cmd;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdIoSendOpCond() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    sendData = (UINT8 *)cacheDmaMalloc (sizeof(UINT8) * SDHC_TUNING_BLOCK_PATTERN_LEN);
    if (sendData == NULL)
        return ERROR;

    SD_CMD_FORM (cmd, SDMMC_CMD_SEND_TUNING, 0x000000000000AA55, SDMMC_CMD_RSP_R1, TRUE);
    SD_DATA_FORM (cmd, sendData, 1, sizeof(UINT8) * SDHC_TUNING_BLOCK_PATTERN_LEN, TRUE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    (void)cacheDmaFree ((void *)sendData);

    return (rc);
    }

/*******************************************************************************
*
* emmcCmdSendIfCond - issue CMD8 to ask card to send interface condition
*
* This routine issues CMD8 to ask card to send interface condition.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSendIfCond
    (
    VXB_DEV_HANDLE pDev,
    UINT32 capbility
    )
    {
    SD_CMD cmd;
    UINT32 arg = 0;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSendIfCond() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pParent = pSdHardWare->pHostDev;

    arg = (SD_CMD8_ARG_VHS_27_36 << SD_CMD8_ARG_VHS_SHIFT) |
           SD_CMD8_ARG_CHK_PATTERN;

    SD_CMD_FORM (cmd, SD_CMD_SEND_IF_COND, arg, SDMMC_CMD_RSP_R7, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);

    if (cmd.cmdErr & SDMMC_CMD_ERR_TIMEOUT)
        {
        pSdHardWare->version = SD_VERSION_100;
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdSendIfCond() - sd target version is SD_VERSION_100\n",
                    0, 0, 0, 0, 0, 0);
        return (OK);
        }

    if (rc == ERROR)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSendIfCond() - cmdIssue return error\n",
                    0, 0, 0, 0, 0, 0);
        return (ERROR);
        }

    if ((cmd.cmdRsp[0] & 0xff) != SD_CMD8_ARG_CHK_PATTERN)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdSendIfCond() - sd target version is SD_VERSION_UNKNOWN\n",
                    0, 0, 0, 0, 0, 0);
        pSdHardWare->version = SD_VERSION_UNKNOWN;
        }
    else
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdSendIfCond() - sd target version is SD_VERSION_200\n",
                    0, 0, 0, 0, 0, 0);
        pSdHardWare->version = SD_VERSION_200;
        }
    return (OK);
    }

/*******************************************************************************
*
* emmcCmdReadExtCsd - issue CMD8 to read MMC card's ext_csd register
*
* This routine issue CMD8 to read MMC card's ext_csd register.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdReadExtCsd
    (
    VXB_DEV_HANDLE  pDev,
    UINT8 * pExtCsd
    )
    {
    SD_CMD cmd;
    UINT8 * pExtData;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdReadExtCsd() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pParent = pSdHardWare->pHostDev;

    pExtData = (UINT8 *)cacheDmaMalloc (sizeof(UINT8) * 512);
    if (pExtData == NULL)
        return ERROR;
    bzero((char *)pExtData, sizeof(UINT8) * 512);
    
    /* The host reads the EXT_CSD register by issuing the SEND_EXT_CSD command.
     *  The Device sends the EXT_CSD register as a block of data, 512 bytes long.
     */
  
    SD_CMD_FORM (cmd, MMC_CMD_SEND_EXT_CSD,
    			0,
				SDMMC_CMD_RSP_R1,
				TRUE);    
    
    SD_DATA_FORM (cmd, pExtData, 1, (512), TRUE);
    
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    if (rc == ERROR)
        {
        (void)cacheDmaFree((void *)pExtCsd);
        return ERROR;
        }
    bcopy ((char *)pExtData, (char *)(pExtCsd), (sizeof(UINT8) * 512));
    (void)cacheDmaFree((void *)pExtData);
    return (OK);
    }

/*******************************************************************************
*
* emmcCmdSendCsd - issue CMD9 to ask card to send its CSD
*
* This routine issues CMD9 to ask card to send its CSD.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSendCsd
    (
    VXB_DEV_HANDLE pDev,
    SDMMC_CSD * pCsd
    )
    {
    SD_CMD cmd;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSendCsd() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pParent = pSdHardWare->pHostDev;

    if (pCsd == NULL)
        return ERROR;

    SD_CMD_FORM (cmd, SDMMC_CMD_SEND_CSD, SDMMC_CMD_ARG_RCA (pSdHardWare->rcaValue),
                 SDMMC_CMD_RSP_R2, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    if (rc == ERROR)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSendCsd() - cmdIssue return error\n",
                    0, 0, 0, 0, 0, 0);
        return (ERROR);
        }

    bcopy ((char *)cmd.cmdRsp, (char *)pCsd, 16);
    return OK;
    }

/*******************************************************************************
*
* emmcCmdSendCid - issue CMD10 to ask card to send its CID
*
* This routine issues CMD10 to ask card to send its CID.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSendCid
    (
    VXB_DEV_HANDLE pDev,
    void * pCid
    )
    {
    SD_CMD cmd;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    if (pCid == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSendCid() - pCid is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSendCid() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    SD_CMD_FORM (cmd, SDMMC_CMD_SEND_CID, SDMMC_CMD_ARG_RCA (pSdHardWare->rcaValue),
                 SDMMC_CMD_RSP_R2, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    if (rc == ERROR)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSendCid() - cmdIssue return error\n",
                    0, 0, 0, 0, 0, 0);
        return (ERROR);
        }

    bcopy ((char *)cmd.cmdRsp, (char *)pCid, CID_SIZE);

    return OK;
    }

/*******************************************************************************
*
* emmcCmdSwitchVoltage - issue CMD11 to set card's voltage to 1.8V
*
* This routine issue CMD11 to set card's voltage to 1.8V.
* This routine is used for UHS-I supported card and mandatory for UHS-I card
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSwitchVoltage
    (
    VXB_DEV_HANDLE pDev
    )
    {
    SD_CMD cmd;

    SD_CMD_FORM (cmd, SD_CMD_SWITCH_VOLTAGE, 0, SDMMC_CMD_RSP_R1, FALSE);

    return (emmcCmdIssue(pDev, &cmd));
    }

/*******************************************************************************
*
* emmcCmdStopTransmission - issue CMD12 to force card to stop transmission
*
* This routine issues CMD12 to force card to stop transmission.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdStopTransmission
    (
    VXB_DEV_HANDLE pDev
    )
    {
    SD_CMD cmd;
    
    SD_CMD_FORM (cmd, SDMMC_CMD_STOP_TRANSMISSION, 0, SDMMC_CMD_RSP_R1B, FALSE);

    return (emmcCmdIssue(pDev, &cmd));
    }

/*******************************************************************************
*
* emmcCmdSendStatus - issue CMD13 to ask card to send its status
*
* This routine issues CMD13 to ask card to send its status.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSendStatus
    (
    VXB_DEV_HANDLE pDev,
    UINT32 * stsData
    )
    {
    SD_CMD cmd;
    SD_HARDWARE * pSdHardWare;
    STATUS rc;
    VXB_DEV_HANDLE pParent;
    
    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO (pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSendStatus() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pParent = pSdHardWare->pHostDev;

    SD_CMD_FORM (cmd, SDMMC_CMD_SEND_STATUS,
                 SDMMC_CMD_ARG_RCA (pSdHardWare->rcaValue),
                 SDMMC_CMD_RSP_R1, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    if (rc == ERROR)
        return (ERROR);

    *stsData = cmd.cmdRsp[0];
    return (OK);
    }

/*******************************************************************************
*
* emmcCmdGoInactiveState - issue CMD15 to put card into the inactive state
*
* This routine issues CMD15 to put card into the inactive state.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdGoInactiveState
    (
    VXB_DEV_HANDLE pDev
    )
    {
    SD_CMD cmd;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdGoInactiveState() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    SD_CMD_FORM (cmd, SDMMC_CMD_GO_INACTIVE_STATE,
                 SDMMC_CMD_ARG_RCA (pSdHardWare->rcaValue),
                 SDMMC_CMD_RSP_NONE, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    return (pSdHardWare->vxbSdCmdIssue(pParent, &cmd));
    }

/*******************************************************************************
*
* emmcCmdSetBlockLen - issue CMD16 to set block length for block commands
*
* This routine issues CMD16 to set block length for block commands.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSetBlockLen
    (
    VXB_DEV_HANDLE pDev,
    UINT32 blkLen
    )
    {
    SD_CMD cmd;

    /* block length is always 512 bytes */

    SD_CMD_FORM (cmd, SDMMC_CMD_SET_BLOCKLEN, blkLen,
                 SDMMC_CMD_RSP_R1, FALSE);

    return (emmcCmdIssue(pDev, &cmd));
    }

/*******************************************************************************
*
* emmcCmdSetBlockNum - issue CMD16 to set block length for block commands
*
* This routine issues CMD16 to set block length for block commands.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSetBlockNum
    (
    VXB_DEV_HANDLE pDev,
    UINT32 blkNum
    )
    {
    SD_CMD cmd;
    UINT32 arg;

    if (blkNum >> 31)
        arg = blkNum;
    else
        arg = (blkNum & 0xFFFF);

    SD_CMD_FORM (cmd, MMC_CMD_SET_BLOCK_COUNT, arg,
                 SDMMC_CMD_RSP_R1, FALSE);

    return (emmcCmdIssue(pDev, &cmd));
    }


/*******************************************************************************
*
* emmcCmdSeEraseGrpStart - issue CMD35 to set erase group start address
*
* This routine issues CMD35 to set erase group start address.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSeEraseGrpStart
    (
    VXB_DEV_HANDLE pDev,
    UINT32 startAddr
    )
    {
    SD_CMD cmd;
    UINT32 cmdIndex;
    SD_HARDWARE * pSdHardWare;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdSeEraseGrpStart() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    if (pSdHardWare->isMmc)
        cmdIndex = MMC_CMD_ERASE_GROUP_START;
    else
        cmdIndex = SD_CMD_ERASE_WR_BLK_START;

    SD_CMD_FORM (cmd, cmdIndex, startAddr,
                 SDMMC_CMD_RSP_R1, FALSE);

    return (emmcCmdIssue(pDev, &cmd));
    }

/*******************************************************************************
*
* emmcCmdSeEraseGrpStart - issue CMD35 to set erase group start address
*
* This routine issues CMD35 to set erase group start address.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSeEraseGrpEnd
    (
    VXB_DEV_HANDLE pDev,
    UINT32 endAddr
    )
    {
    SD_CMD cmd;
    UINT32 cmdIndex;
    SD_HARDWARE * pSdHardWare;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSeEraseGrpEnd() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    if (pSdHardWare->isMmc)
        cmdIndex = MMC_CMD_ERASE_GROUP_END;
    else
        cmdIndex = SD_CMD_ERASE_WR_BLK_END;

    SD_CMD_FORM (cmd, cmdIndex, endAddr,
                 SDMMC_CMD_RSP_R1, FALSE);

    return (emmcCmdIssue(pDev, &cmd));
    }

/*******************************************************************************
*
* emmcCmdSeEraseGrpStart - issue CMD35 to set erase group start address
*
* This routine issues CMD35 to set erase group start address.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdSetEraseGrp
    (
    VXB_DEV_HANDLE pDev,
    UINT eraseArg
    )
    {
    SD_CMD cmd;
    UINT32 rspValue = 0;
    UINT32 timeOut = 0;
    STATUS rc = OK;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSetEraseGrp() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pParent = pSdHardWare->pHostDev;

    SD_CMD_FORM (cmd, SDMMC_CMD_ERASE, eraseArg, SDMMC_CMD_RSP_R1B, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    if (rc == ERROR)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdSeEraseGrp() - cmdIssue return error\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    do
        {

        /* Need not check return status */

        (void)emmcCmdSendStatus (pDev, &rspValue);
        timeOut++;
        if (timeOut >= SDMMC_COMMAND_TIMEOUT)
            return (ERROR);
        }while((!(rspValue & CARD_STS_READY_FOR_DATA)) ||
               (CARD_STS_CUR_GET(rspValue) == CARD_STS_PRG));

    return (OK);
    }

/*******************************************************************************
*
* emmcSendHpi - send HPI command to device
*
* This routine sends HPI command to device.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcSendHpi
    (
    VXB_DEV_HANDLE pDev,
    UINT32 cmdIndex,
    UINT32 * pStatus
    )
    {
    STATUS rc = OK;
    SD_CMD emmcCmd;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;
    UINT32 argValue;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcSendHpi() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;
    argValue = ((pSdHardWare->rcaValue << 16) | 0x01);

    if (cmdIndex == SDMMC_CMD_STOP_TRANSMISSION)
        {
        SD_CMD_FORM (emmcCmd, cmdIndex, argValue, SDMMC_CMD_RSP_R1B, FALSE);
        }
    else if (cmdIndex == SDMMC_CMD_SEND_STATUS)
        {
        SD_CMD_FORM (emmcCmd, cmdIndex, argValue, SDMMC_CMD_RSP_R1, FALSE);
        }
    rc = pSdHardWare->vxbSdCmdIssue(pParent, &emmcCmd);
    if (rc == ERROR)
        return (ERROR);

    if (pStatus != NULL)
        *pStatus = emmcCmd.cmdRsp[0];
    return (OK);
    }

/*******************************************************************************
*
* emmcCmdReadSingleBlock - issue CMD17 to read a single block
*
* This routine issues CMD17 to read a single block.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdReadSingleBlock
    (
    VXB_DEV_HANDLE   pDev,
    void *          buf,
    UINT32          blkNo,
    BOOL            highCapa
    )
    {
    SD_CMD cmd;
    UINT32 arg;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdReadSingleBlock() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pParent = pSdHardWare->pHostDev;

    if (highCapa)
        arg = blkNo;
    else
        arg = blkNo * pSdHardWare->blockSize;

    EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdReadSingleBlock() - arg = %d\n",
                0, 0, 0, 0, 0, 0);

    SD_CMD_FORM (cmd, SDMMC_CMD_READ_SINGLE_BLOCK, arg, SDMMC_CMD_RSP_R1, TRUE);
    SD_DATA_FORM (cmd, buf, 1, pSdHardWare->blockSize, TRUE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;
    if (!pSdHardWare->isMmc)
        cmd.rspType |= SDMMC_CMD_AUTOCMD12;

    return (pSdHardWare->vxbSdCmdIssue(pParent, &cmd));
    }

/*******************************************************************************
*
* emmcCmdReadMultipleBlock - issue CMD18 to read multiple blocks
*
* This routine issues CMD18 to read multiple blocks.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdReadMultipleBlock
    (
    VXB_DEV_HANDLE   pDev,
    void *          buf,
    UINT32          blkNo,
    UINT32          numBlks,
    BOOL            highCapa
    )
    {
    SD_CMD cmd;
    UINT32 arg;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdReadMultipleBlock() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pParent = pSdHardWare->pHostDev;

    if (highCapa)
        arg = blkNo;
    else
        arg = blkNo * pSdHardWare->blockSize;

    EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                "emmcCmdReadMultipleBlock() - arg = %d\n",
                arg, 0, 0, 0, 0, 0);

    SD_CMD_FORM (cmd, SDMMC_CMD_READ_MULTI_BLOCK, arg, SDMMC_CMD_RSP_R1, TRUE);
    SD_DATA_FORM (cmd, buf, numBlks, pSdHardWare->blockSize, TRUE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;
    if (!pSdHardWare->isMmc)
        cmd.rspType |= SDMMC_CMD_AUTOCMD12;

    return (pSdHardWare->vxbSdCmdIssue(pParent, &cmd));
    }

/*******************************************************************************
*
* emmcCmdReadBlock - issue CMD17 or CMD18 to read blocks
*
* This routine issues CMD17 or CMD18 to read blocks.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdReadBlock
    (
    VXB_DEV_HANDLE   pDev,
    void *          buf,
    UINT64          blkNo,
    UINT32          numBlks,
    BOOL            highCapa
    )
    {
    SD_CMD cmd;
    UINT32 idx;
    UINT64 arg;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdReadBlock() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    if (numBlks > 1)
        idx = SDMMC_CMD_READ_MULTI_BLOCK;
    else
        idx = SDMMC_CMD_READ_SINGLE_BLOCK;

    if (highCapa)
        arg = blkNo;
    else
        arg = blkNo * pSdHardWare->blockSize;

    SD_CMD_FORM (cmd, idx, arg, SDMMC_CMD_RSP_R1, TRUE);
    SD_DATA_FORM (cmd, buf, numBlks, pSdHardWare->blockSize, TRUE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;
    if (!pSdHardWare->isMmc)
        cmd.rspType |= SDMMC_CMD_AUTOCMD12;

    return (pSdHardWare->vxbSdCmdIssue(pParent, &cmd));
    }

/*******************************************************************************
*
* emmcCmdWriteSingleBlock - issue CMD24 to write a single block
*
* This routine issues CMD24 to write a single block.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdWriteSingleBlock
    (
    VXB_DEV_HANDLE   pDev,
    void *          buf,
    UINT32          blkNo,
    BOOL            isHighCapa
    )
    {
    SD_CMD cmd;
    UINT32 arg;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdWriteSingleBlock() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pParent = pSdHardWare->pHostDev;

    if (isHighCapa)
        arg = blkNo;
    else
        arg = blkNo * pSdHardWare->blockSize;

    SD_CMD_FORM (cmd, SDMMC_CMD_WRITE_SINGLE_BLOCK, arg, SDMMC_CMD_RSP_R1, TRUE);
    SD_DATA_FORM (cmd, buf, 1, pSdHardWare->blockSize, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;
    if (!pSdHardWare->isMmc)
        cmd.rspType |= SDMMC_CMD_AUTOCMD12;

    return (pSdHardWare->vxbSdCmdIssue(pParent, &cmd));
    }

/*******************************************************************************
*
* emmcCmdWriteMultipleBlock - issue CMD25 to write multiple blocks
*
* This routine issues CMD25 to writ multiple blocks.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdWriteMultipleBlock
    (
    VXB_DEV_HANDLE  pDev,
    void *          buf,
    UINT32          blkNo,
    UINT32          numBlks,
    BOOL            isHighCapa
    )
    {
    SD_CMD cmd;
    UINT32 arg;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdWriteMultipleBlock() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pParent = pSdHardWare->pHostDev;

    if (isHighCapa)
        arg = blkNo;
    else
        arg = blkNo * pSdHardWare->blockSize;

    EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                "emmcCmdWriteMultipleBlock() - arg = %d\n",
                arg, 0, 0, 0, 0, 0);

    SD_CMD_FORM (cmd, SDMMC_CMD_WRITE_MULTI_BLOCK, arg, SDMMC_CMD_RSP_R1, TRUE);
    SD_DATA_FORM (cmd, buf, numBlks, pSdHardWare->blockSize, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;
    if (!pSdHardWare->isMmc)
        cmd.rspType |= SDMMC_CMD_AUTOCMD12;

    return (pSdHardWare->vxbSdCmdIssue(pParent, &cmd));
    }

/*******************************************************************************
*
* emmcCmdWriteBlock - issue CMD24 or CMD25 to write blocks
*
* This routine issues CMD24 or CMD25 to write blocks.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdWriteBlock
    (
    VXB_DEV_HANDLE  pDev,
    void *          buf,
    UINT64          blkNo,
    UINT32          numBlks,
    BOOL            isHighCapa
    )
    {
    SD_CMD cmd;
    UINT32 idx;
    UINT64 arg;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdWriteBlock() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    if (numBlks > 1)
    {
        idx = SDMMC_CMD_WRITE_MULTI_BLOCK;
    }
    else
    {
        idx = SDMMC_CMD_WRITE_SINGLE_BLOCK;
    }

    if (isHighCapa)
        arg = blkNo;
    else
        arg = blkNo * pSdHardWare->blockSize;
    

    SD_CMD_FORM (cmd, idx, arg, SDMMC_CMD_RSP_R1B, TRUE);
    SD_DATA_FORM (cmd, buf, numBlks, pSdHardWare->blockSize, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;
    if (!pSdHardWare->isMmc)
        cmd.rspType |= SDMMC_CMD_AUTOCMD12;

    return (pSdHardWare->vxbSdCmdIssue(pParent, &cmd));
    }

/*******************************************************************************
*
* emmcCmdAppCmd - issue CMD55 to indicate an application command followed
*
* This routine issues CMD55 to indicate an application command followed.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdAppCmd
    (
    VXB_DEV_HANDLE  pDev
    )
    {
    SD_CMD cmd;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdAppCmd() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    SD_CMD_FORM (cmd, SDMMC_CMD_APP_CMD, SDMMC_CMD_ARG_RCA (pSdHardWare->rcaValue),
                 0, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    return (pSdHardWare->vxbSdCmdIssue(pParent, &cmd));
    }

/*******************************************************************************
*
* emmcACmdSetBusWidth - issue ACMD6 to set the bus width for data transfer
*
* This routine issues ACMD6 to set the bus width for data transfer.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcACmdSetBusWidth
    (
    VXB_DEV_HANDLE   pDev,
    UINT32          width
    )
    {
    SD_CMD cmd;
    UINT32 arg;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcACmdSetBusWidth() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    
    rc = emmcCmdAppCmd (pDev);
    if (rc == ERROR)
        return ERROR;

    if (width == SDMMC_BUS_WIDTH_1BIT)
        arg = SD_ACMD6_ARG_1BIT_BUS;
    else
        arg = SD_ACMD6_ARG_4BIT_BUS;

    SD_CMD_FORM (cmd, SD_ACMD_SET_BUS_WIDTH, arg, SDMMC_CMD_RSP_R1, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    return (pSdHardWare->vxbSdCmdIssue(pParent, &cmd));
    }

/*******************************************************************************
*
* emmcACmdSendOpCond - issue ACMD41 to ask card to send its OCR
*
* This routine issues ACMD41 to ask card to send its OCR.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcACmdSendOpCond
    (
    VXB_DEV_HANDLE  pDev,
    UINT32 capbility,
    UINT32 version
    )
    {
    SD_CMD cmd;
    UINT32 arg;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcACmdSendOpCond() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    rc = emmcCmdAppCmd (pDev);
    if (rc == ERROR)
        return (ERROR);

    arg = capbility & OCR_VDD_VOL_MASK;
    if (version == SD_VERSION_200)
        {
        if (capbility & OCR_CARD_CAP_STS)
            arg |= OCR_CARD_CAP_STS;
        }

   /*
    * Set ACMD41 argument to 3.3V if both 3.0V & 3.3V are not reported
    * by host controller's capbilities register
    */

    if (!(arg & (OCR_VDD_VOL_29_30 | OCR_VDD_VOL_30_31 |
                 OCR_VDD_VOL_32_33 | OCR_VDD_VOL_33_34)))
        arg |= (OCR_VDD_VOL_32_33 | OCR_VDD_VOL_33_34);
    
    SD_CMD_FORM (cmd, SD_ACMD_SEND_OP_COND, arg, SDMMC_CMD_RSP_R3, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);

    if (rc == ERROR)
        return (ERROR);

    pSdHardWare->ocrValue = cmd.cmdRsp[0];
    return (OK);
    }

/*******************************************************************************
*
* emmcACmdClrCardDetect - issue ACMD42 to clrear card detect
*
* This routine issues ACMD42 to clrear card detect.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcACmdClrCardDetect
    (
    VXB_DEV_HANDLE   pDev,
    UINT32          connect
    )
    {
    SD_CMD cmd;
    UINT32 arg;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_ERR,
                    "emmcACmdClrCardDetect() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0 ,0);
        return ERROR;
        }
        
    rc = emmcCmdAppCmd (pDev);
    if (rc == ERROR)
        return ERROR;

    arg = connect;
    cmd.cmdIdx = 0;
    SD_CMD_FORM (cmd, SD_ACMD_SET_CLR_CARD_DETECT, arg, SDMMC_CMD_RSP_R1, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    return (pSdHardWare->vxbSdCmdIssue(pSdHardWare->pHostDev, &cmd));
    }
    
/*******************************************************************************
*
* emmcACmdSendWriteBlkNum - issue ACMD22 to ask card to send the num of write block
*
* This routine issues ACMD22 to ask card to send the number of the written 
* (without errors) write blocks.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcACmdSendWriteBlkNum
    (
    VXB_DEV_HANDLE  pDev,
    UINT32 * wrBlkNum
    )
    {
    SD_CMD cmd;
    UINT32 * numWr;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcACmdSendWriteBlkNum() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    numWr = (UINT32 *)cacheDmaMalloc (sizeof(UINT32));
    if (numWr == NULL)
        return ERROR;

    rc = emmcCmdAppCmd (pDev);
    if (rc == ERROR)
        {
        (void)cacheDmaFree((void *)numWr);
        return ERROR;
        }

    SD_CMD_FORM (cmd, SD_ACMD_SEND_NUM_WR_BLOCKS, 0, SDMMC_CMD_RSP_R1, TRUE);
    SD_DATA_FORM (cmd, numWr, 1, sizeof(UINT32), TRUE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    if (rc == ERROR)
        {
        (void)cacheDmaFree((void *)numWr);
        return ERROR;
        }

    bcopy ((char *)numWr, (char *)(wrBlkNum), sizeof(SDMMC_SCR));
    (void)cacheDmaFree((void *)numWr);
    return (OK);
    }

/*******************************************************************************
*
* emmcACmdSendScr - issue ACMD51 to ask card to send its SCR
*
* This routine issues ACMD51 to ask card to send its SCR.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcACmdSendScr
    (
    VXB_DEV_HANDLE  pDev,
    SDMMC_SCR * scrData
    )
    {
    SD_CMD cmd;
    SDMMC_SCR * scr;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;
    UINT32 i;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcACmdSendScr() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    scr = (SDMMC_SCR *)cacheDmaMalloc (sizeof(SDMMC_SCR));
    if (scr == NULL)
        return ERROR;

    for (i = 0; i < SDMMC_CMD_RETRY; i++)
        {
        rc = emmcCmdAppCmd (pDev);
        if (rc == ERROR)
            {
            vxbMsDelay(1);
            continue; 
            } 
        
        SD_CMD_FORM (cmd, SD_ACMD_SEND_SCR, 0, SDMMC_CMD_RSP_R1, TRUE);
        SD_DATA_FORM (cmd, scr, 1, sizeof(SDMMC_SCR), TRUE);
        pSdHardWare->dataErr = 0x0;
        pSdHardWare->cmdErr = 0x0;
        
        rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
        if (rc == ERROR)
            {
            vxbMsDelay(1);
            continue;
            }
        else
            break;
        }
        
    if (rc != ERROR)
        {    
        bcopy ((char *)scr, (char *)(scrData), sizeof(SDMMC_SCR));
        (void)cacheDmaFree((void *)scr);
        return (OK);
        }
    else
        {
        (void)cacheDmaFree((void *)scr);
        return (rc);
        }
    }

/*******************************************************************************
*
* emmcACmdSendSsr - issue ACMD13 to ask card to send its SSR
*
* This routine issues ACMD13 to ask card to send its SSR.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcACmdSendSsr
    (
    VXB_DEV_HANDLE  pDev,
    UINT8 * ssrData
    )
    {
    SD_CMD cmd;
    UINT8 * ssr;
    STATUS rc;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcACmdSendSsr() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    ssr = (UINT8 *)cacheDmaMalloc (sizeof(UINT8) * SD_SSR_SIZE);
    if (ssr == NULL)
        return ERROR;

    rc = emmcCmdAppCmd (pDev);
    if (rc == ERROR)
        {
        (void)cacheDmaFree((void *)ssr);
        return ERROR;
        }

    SD_CMD_FORM (cmd, SD_ACMD_SD_STATUS, 0, SDMMC_CMD_RSP_R1, TRUE);
    SD_DATA_FORM (cmd, ssr, 1, sizeof(UINT8) * SD_SSR_SIZE, TRUE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    if (rc == ERROR)
        {
        (void)cacheDmaFree((void *)ssr);
        return ERROR;
        }

    bcopy ((char *)ssr, (char *)(ssrData), sizeof(UINT8) * SD_SSR_SIZE);
    (void)cacheDmaFree((void *)ssr);
    return (OK);
    }

/*******************************************************************************
*
* emmcCmdIoRwDirect - issue CMD52 to read/write sdio card's register.
*
* This routine issues CMD52 to read/write sdio card's register. CMD52 must be
* input register offset, function number, rwOps. rwOps parameter include two
* part, R/W flag and RAW flag. R/W flag is defined at bit31 and RAW flag is
* defined at bit27. These flags's define is same with CMD52 description.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdIoRwDirect
    (
    VXB_DEV_HANDLE pDev,
    UINT32 regOffset,
    UINT32 funcNum,
    UINT32 rwOps,
    UINT8 * data
    )
    {
    SD_CMD cmd;
    STATUS rc;
    UINT32 arg = 0;
    UINT8  cmdSts = 0;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdIoRwDirect() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    pParent = pSdHardWare->pHostDev;

    arg |= rwOps;
    arg |= SDIO_DIRECT_IO_REGADRS(regOffset);
    arg |= SDIO_DIRECT_IO_FUNC(funcNum);
    arg |= *data;

    SD_CMD_FORM (cmd, SDIO_CMD_DIRECT_IO, arg, SDMMC_CMD_RSP_R5, FALSE);
    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);

    pSdHardWare->cmdErr = cmd.cmdErr;
    pSdHardWare->dataErr = cmd.dataErr;

    if (rc == ERROR)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdIoRwDirect() - cmdIssue return error\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    cmdSts = (UINT8)((cmd.cmdRsp[0] >> 8) & 0xFF);
    if ((cmdSts &
        (SDIO_DIRECTIO_OUTRANGE | SDIO_DIRECTIO_ERR | SDIO_DIRECTIO_FUNCERR))
        != 0x0)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdIoRwDirect() - cmdSts = 0X%x return error\n",
                    cmdSts, 0, 0, 0, 0, 0);
        return ERROR;
        }
    if ((rwOps & (UINT32)SDIO_DIRECT_IO_RW) == 0x0)
        {
        *data = (UINT8)(cmd.cmdRsp[0] & 0xFF);
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdIoRwDirect() - REG DATA = 0X%x\n",
                    cmd.cmdRsp[0] & 0xFF, 0, 0, 0, 0, 0);
        }
    return (OK);
    }

/*******************************************************************************
*
* emmcCmdIoRwExtend - issue CMD53 to read/write sdio card's data.
*
* This routine issues CMD53 to read/write sdio card's data. CMD53 must be
* input data offset, function number, rwOps. rwOps parameter include two
* part, R/W flag and RAW flag. R/W flag is defined at bit31 and RAW flag is
* defined at bit27. These flags's define is same with CMD53 description.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdIoRwExtend
    (
    VXB_DEV_HANDLE pDev,
    UINT32 regOffset,
    UINT32 funcNum,
    UINT32 ops,
    SDDATA * pSdData
    )
    {
    SD_CMD cmd;
    STATUS rc;
    UINT32 arg = 0;
    UINT8  cmdSts = 0;
    BOOL   isRead;
    SD_HARDWARE * pSdHardWare;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdIoRwExtend() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    pParent = pSdHardWare->pHostDev;

    arg |= ops;
    arg |= SDIO_DIRECT_IO_REGADRS(regOffset);
    arg |= SDIO_DIRECT_IO_FUNC(funcNum);

    if ((ops & SDIO_DIRECT_IOEXT_BLK) == 0x0)
        arg |= (pSdData->blkSize == 512) ? 0 : pSdData->blkSize;    /* byte mode */
    else
        arg |= SDIO_DIRECT_IOEXT_BLK | pSdData->blkNum;     /* block mode */

    if ((ops & (UINT32)SDIO_DIRECT_IO_RW) == 0x0)
        isRead = TRUE;
    else
        isRead = FALSE;

    SD_CMD_FORM (cmd, SDIO_CMD_DIRECT_EXT, arg, SDMMC_CMD_RSP_R5, TRUE);
    SD_DATA_FORM (cmd, pSdData->buffer, pSdData->blkNum, pSdData->blkSize, isRead);

    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, &cmd);
    pSdHardWare->cmdErr = cmd.cmdErr;
    pSdHardWare->dataErr = cmd.dataErr;

    if (rc == ERROR)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD,
                    "emmcCmdIoRwExtend() - cmdIssue return error\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    cmdSts = (UINT8)((cmd.cmdRsp[0] >> 8) & 0xFF);
    if ((cmdSts &
        (SDIO_DIRECTIO_OUTRANGE | SDIO_DIRECTIO_ERR | SDIO_DIRECTIO_FUNCERR))
        != 0x0)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdIoRwExtend() - cmdSts = 0X%x return error\n",
                    cmdSts, 0, 0, 0, 0, 0);
        return ERROR;
        }

    return (OK);
    }

/*******************************************************************************
*
* emmcCmdIssue - the feature to issue one new SD/SDIO command
*
* This routine issues one new command.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcCmdIssue
    (
    VXB_DEV_HANDLE pDev,
    SD_CMD * pSdCmd
    )
    {
    SD_HARDWARE * pSdHardWare;
    STATUS rc;
    VXB_DEV_HANDLE pParent;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdIssue() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }
    if (pSdCmd == NULL)
        return ERROR;

    pParent = pSdHardWare->pHostDev;

    pSdHardWare->dataErr = 0x0;
    pSdHardWare->cmdErr = 0x0;

    rc = pSdHardWare->vxbSdCmdIssue(pParent, pSdCmd);
    return (rc);
    }

/*******************************************************************************
*
* vxbeMmcReadCis - get all of the CIS value from one SDIO target device
*
* This routine get all of the CIS value from one SDIO target device
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS vxbeMmcReadCis
    (
    VXB_DEV_HANDLE pDev
    )
    {
    STATUS ret;
    SDIO_FUNC_TUPLE * pCurr;
    SDIO_FUNC_TUPLE * pNext;
    SD_HARDWARE * pSdHardWare;
    UINT8 cisData = 0;
    UINT32 i;
    UINT32 ptr = 0;
    UINT8 tplCode = 0;
    UINT8  tplLink = 0;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "vxbeMmcReadCis() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    for (i = 0; i < 3; i++)
        {
        ret = emmcCmdIoRwDirect(pDev,
                                (UINT32)(SDIO_FBR_BASE(pSdHardWare->funcNum) +
                                SDIO_FBR_CIS + i),
                                0, 0, &cisData);
        if (ret == ERROR)
            return (ERROR);

        ptr |= cisData << (i * 8);
        }

    do {
        ret = emmcCmdIoRwDirect(pDev, ptr++, 0, 0, &tplCode);
        if (ret == ERROR)
            break;

        /* size of 0xFF means we're done */

        if (tplCode == 0xFF)
            break;

        /* 0x00 means no link field or data */

        if (tplCode == 0x00)
            continue;

        ret = emmcCmdIoRwDirect(pDev, ptr++, 0, 0, &tplLink);
        if (ret == ERROR)
            break;

        /* size of 0xFF means we're done */

        if (tplLink == 0xFF)
            break;

        pNext = (SDIO_FUNC_TUPLE *)malloc(sizeof(SDIO_FUNC_TUPLE));
        if (pNext == NULL)
            return ERROR;

        pCurr = pNext;

        if (pSdHardWare->pSdioFuncTuple == NULL)
            {
            pSdHardWare->pSdioFuncTuple = pNext;
            pCurr = pNext;
            }
        else if (pCurr != NULL)
            pCurr->next = pNext;

        pSdHardWare->pSdioFuncTuple = pCurr;

        if (tplLink != 0x0)
            {
            pSdHardWare->pSdioFuncTuple->pData = (UINT8 *)malloc(sizeof(UINT8) * tplLink);
            if (pSdHardWare->pSdioFuncTuple->pData == NULL)
                {
                free (pSdHardWare->pSdioFuncTuple);
                return ERROR;
                }
            }

        for (i = 0; i < tplLink; i++)
            {
            ret = emmcCmdIoRwDirect(pDev, ptr + i, 0, 0, (pCurr->pData + i));
            if (ret == ERROR)
                break;
            }
        if (ret == ERROR)
            {
            free(pCurr->pData);
            free(pCurr);
            break;
            }
        if (tplCode == SDIO_CISTPL_MANFID)
            {
            UINT16 * pData;
            pData = (UINT16 *)pCurr->pData;
            pSdHardWare->vxbeMmcId.sdioDevId = *(pData);
            pData++;
            pSdHardWare->vxbeMmcId.sdioVenderId = *(pData);
            }
        ptr += tplLink;
        } while (ret != ERROR);

    return ret;
    }

/*******************************************************************************
*
* vxbeMmcSetFunc - set one SDIO function enable/disable
*
* This routine set one SDIO function enable/disable
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*/

STATUS vxbeMmcSetFunc
    (
    VXB_DEV_HANDLE pDev,
    UINT8 func,
    BOOL  isEn
    )
    {
    STATUS ret;
    UINT8 ioRegData = 0;
    UINT32 timeOut = 0;
    SD_HARDWARE * pSdHardWare;

    pSdHardWare = (SD_HARDWARE *)GET_HARDWAREINFO(pDev);
    if (pSdHardWare == NULL)
        {
        EMMC_LIB_DBG (EMMC_LIB_DBG_CMD, "emmcCmdIoSendOpCond() - pSdHardWare is NULL\n",
                    0, 0, 0, 0, 0, 0);
        return ERROR;
        }

    ret = emmcCmdIoRwDirect(pDev, SDIO_IOEN_CCCR, 0, 0, &ioRegData);
    if (ret == ERROR)
        return (ERROR);
    ioRegData = (UINT8)(ioRegData | (1 << func));

    ret = emmcCmdIoRwDirect(pDev, SDIO_IOEN_CCCR, 0, (UINT32)SDIO_DIRECT_IO_RW, &ioRegData);
    if (ret == ERROR)
        return (ERROR);

    do
        {
        ret = emmcCmdIoRwDirect(pDev, SDIO_IOEN_CCCR, 0, 0, &ioRegData);
        if (ret == ERROR)
            return (ERROR);
        timeOut++;
        }while (((ioRegData & (1 << func)) == 0x0) && (timeOut > SDIO_OPREG_TIMEOUT));
    if (timeOut >= SDIO_OPREG_TIMEOUT)
        return (ERROR);
    return (OK);
    }

LOCAL struct vxbDExtECSDHandle_t dExtCSDHandle;

void registerDecodeEcsd
    (VXB_DEV_ID 		pDev,
     VXB_DECODE_EXT_CSD *pDecodeEcsd
	)
    {	
	dExtCSDHandle.pInst			= pDev;
	dExtCSDHandle.pDecodeEcsd	= pDecodeEcsd;
    }

STATUS vxbDecodeExtCSD ( )
    {
	if (dExtCSDHandle.pInst== NULL)
		return ERROR;
	else
		return dExtCSDHandle.pDecodeEcsd->decodeExtCSD(dExtCSDHandle.pInst);
    }
