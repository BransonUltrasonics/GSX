/* gfxTiDssIosDrv.c - TI DSS ios driver */

/*
 * Copyright (c) 2013-2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
27jul17,s_m  Modified for TI AM5728 FB implementation
24jan14,mgc  Modified for VxWorks 7 release
22mar13,af  Written
*/

/* includes */

#include <gfxTiDssDrv.h>
#include <gfxFbIosDrv.inl>

/* locals */

/* List of video modes known to the driver */
LOCAL FB_VIDEO_MODE gfxTiAm57xxHdmiFbModesDb[] =
    {
        {"480x800",
        60, 480, 800, 32, 4*480,
        0, 0, 0, 0, 0, 0, 0,
        0,
        FB_VMODE_NONINTERLACED,
        0,
        1,
        2},
        {"1920x1080",
        60, 1920, 1080, 32, 4*1920,
        0, 0, 0, 0, 0, 0, 0,
        0,
        FB_VMODE_NONINTERLACED,
        0,
        1,
        2}
    };
LOCAL FB_VIDEO_MODE gfxTiAm35xxHdmiFbModesDb[] =
    {
        {"1280x800",
        60, 1280, 800, 32, 4*1280,
        0, 0, 0, 0, 0, 0, 0,
        0,
        FB_VMODE_NONINTERLACED,
        0,
        1,
        2},
        {"1280x720",
        60, 1280, 720, 32, 4*1280,
        0, 0, 0, 0, 0, 0, 0,
        0,
        FB_VMODE_NONINTERLACED,
        0,
        1,
        2},
        {"1024x768",
        60, 1024, 768, 32, 4*1024,
        0, 0, 0, 0, 0, 0, 0,
        0,
        FB_VMODE_NONINTERLACED,
        0,
        1,
        2}
    };

LOCAL FB_VIDEO_MODE gfxTiAm35xxLcdFbModesDb[] =
    {
        {"480x272",
        60, 480, 272, 32, 4*480,
        0, 0, 0, 0, 0, 0, 0,
        0,
        FB_VMODE_NONINTERLACED,
        0,
        1,
        2}
    };

LOCAL FB_VIDEO_MODE gfxTiAm37xxDviFbModesDb[] =
    {
        {"1280x800",
        60, 1280, 800, 32, 4*1280,
        0, 0, 0, 0, 0, 0, 0,
        0,
        FB_VMODE_NONINTERLACED,
        0,
        1,
        2},
        {"1280x720",
        60, 1280, 720, 32, 4*1280,
        0, 0, 0, 0, 0, 0, 0,
        0,
        FB_VMODE_NONINTERLACED,
        0,
        1,
        2},
        {"1024x768",
        60, 1024, 768, 32, 4*1024,
        0, 0, 0, 0, 0, 0, 0,
        0,
        FB_VMODE_NONINTERLACED,
        0,
        1,
        2}
    };

LOCAL FB_VIDEO_MODE gfxTiAm37xxLcdFbModesDb[] =
    {
        {"480x640",
        60, 480, 640, 32, 4*480,
        0, 0, 0, 0, 0, 0, 0,
        0,
        FB_VMODE_NONINTERLACED,
        0,
        1,
        2}
    };

LOCAL FB_VIDEO_MODE gfxTiOmap35xxDviFbModesDb[] =
    {
        {"1280x800",
        60, 1280, 800, 32, 4*1280,
        0, 0, 0, 0, 0, 0, 0,
        0,
        FB_VMODE_NONINTERLACED,
        0,
        1,
        2},
        {"1280x720",
        60, 1280, 720, 32, 4*1280,
        0, 0, 0, 0, 0, 0, 0,
        0,
        FB_VMODE_NONINTERLACED,
        0,
        1,
        2},
        {"1024x768",
        60, 1024, 768, 32, 4*1024,
        0, 0, 0, 0, 0, 0, 0,
        0,
        FB_VMODE_NONINTERLACED,
        0,
        1,
        2}
    };

LOCAL FB_VIDEO_MODE gfxTiOmap35xxLcdFbModesDb[] =
    {
        {"480x640",
        60, 480, 640, 32, 4*480,
        0, 0, 0, 0, 0, 0, 0,
        0,
        FB_VMODE_NONINTERLACED,
        0,
        1,
        2}
    };

/* forward declarations */

IMPORT STATUS gfxTiDssHwInit (GFX_FBDEV*);
IMPORT STATUS gfxTiDssSetVideoModeEx (GFX_FBDEV*, FB_VIDEO_MODE*);
IMPORT STATUS gfxTiDssSetFbAddr (GFX_FBDEV*);
IMPORT void gfxTiDssIntHandlerVsync (GFX_FBDEV*);

/*******************************************************************************
 *
 * gfxTiDssInit - frame buffer driver initialization
 *
 * RETURNS: N/A
 *
 */
void gfxTiDssInit
    (
    const char*     pSysModel,
    FUNCPTR         getCfgFuncPtr
    )
    {
    GFX_FBDEV*      pDev;

    pDev = drvInit(getCfgFuncPtr, 0);
    if (pDev == NULL)
        return;


    if (strstr (pSysModel, "TI AM35xx") != NULL)
        {
        if (OK == gfxStrcmp (pDev->displayName, GFX_DISP_HDMI))
            {
            pDev->disp = GFX_DISP_TIAM35XX_HDMI;
            pDev->fbModesDb = gfxTiAm35xxHdmiFbModesDb;
            pDev->fbModesCount = NELEMENTS(gfxTiAm35xxHdmiFbModesDb);
            }
        else if (OK == gfxStrcmp (pDev->displayName, GFX_DISP_FLAT_PANEL))
            {
            pDev->disp = GFX_DISP_TIAM35XX_FLAT_PANEL;
            pDev->fbModesDb = gfxTiAm35xxLcdFbModesDb;
            pDev->fbModesCount = NELEMENTS(gfxTiAm35xxLcdFbModesDb);
            }
        else
            {
            (void)fprintf (stderr, "Invalid display %s\n", pDev->displayName);
            drvCleanup (pDev);
            return;
            }
        pDev->setVideoModeExFuncPtr = gfxTiDssSetVideoModeEx;
        pDev->setFbAddrFuncPtr = gfxTiDssSetFbAddr;
#if defined(GFX_VSYNC_IRQ)
        pDev->intHandlerVsyncFuncPtr = gfxTiDssIntHandlerVsync;
#endif
        if (ERROR == gfxTiDssHwInit (pDev))
            {
            drvCleanup (pDev);
            return;
            }
        }
    else if (strstr (pSysModel, "Mistral DM37x") != NULL)
        {
        if (OK == gfxStrcmp (pDev->displayName, GFX_DISP_DVI))
            {
            pDev->disp = GFX_DISP_TIAM37XX_DVI;
            pDev->fbModesDb = gfxTiAm37xxDviFbModesDb;
            pDev->fbModesCount = NELEMENTS(gfxTiAm37xxDviFbModesDb);
            }
        else if (OK == gfxStrcmp (pDev->displayName, GFX_DISP_FLAT_PANEL))
            {
            pDev->disp = GFX_DISP_TIAM37XX_FLAT_PANEL;
            pDev->fbModesDb = gfxTiAm37xxLcdFbModesDb;
            pDev->fbModesCount = NELEMENTS(gfxTiAm37xxLcdFbModesDb);
            }
        else
            {
            (void)fprintf (stderr, "Invalid display %s\n", pDev->displayName);
            drvCleanup (pDev);
            return;
            }
        pDev->setVideoModeExFuncPtr = gfxTiDssSetVideoModeEx;
        pDev->setFbAddrFuncPtr = gfxTiDssSetFbAddr;
#if defined(GFX_VSYNC_IRQ)
        pDev->intHandlerVsyncFuncPtr = gfxTiDssIntHandlerVsync;
#endif
        if (ERROR == gfxTiDssHwInit (pDev))
            {
            drvCleanup (pDev);
            return;
            }
        }
    else if (strstr (pSysModel, "Mistral OMAP35xx") != NULL)
        {
        if (OK == gfxStrcmp (pDev->displayName, GFX_DISP_DVI))
            {
            pDev->disp = GFX_DISP_TIOMAP35XX_DVI;
            pDev->fbModesDb = gfxTiOmap35xxDviFbModesDb;
            pDev->fbModesCount = NELEMENTS(gfxTiOmap35xxDviFbModesDb);
            }
        else if (OK == gfxStrcmp (pDev->displayName, GFX_DISP_FLAT_PANEL))
            {
            pDev->disp = GFX_DISP_TIOMAP35XX_FLAT_PANEL;
            pDev->fbModesDb = gfxTiOmap35xxLcdFbModesDb;
            pDev->fbModesCount = NELEMENTS(gfxTiOmap35xxLcdFbModesDb);
            }
        else
            {
            (void)fprintf (stderr, "Invalid display %s\n", pDev->displayName);
            drvCleanup (pDev);
            return;
            }
        pDev->setVideoModeExFuncPtr = gfxTiDssSetVideoModeEx;
        pDev->setFbAddrFuncPtr = gfxTiDssSetFbAddr;
#if defined(GFX_VSYNC_IRQ)
        pDev->intHandlerVsyncFuncPtr = gfxTiDssIntHandlerVsync;
#endif
        if (ERROR == gfxTiDssHwInit (pDev))
            {
            drvCleanup (pDev);
            return;
            }
        }
    else
        {
        pDev->disp = GFX_DISP_TIAM57XX_HDMI;
        pDev->fbModesDb = gfxTiAm57xxHdmiFbModesDb;
        pDev->fbModesCount = NELEMENTS(gfxTiAm57xxHdmiFbModesDb);

        pDev->setVideoModeExFuncPtr = gfxTiDssSetVideoModeEx;
        pDev->setFbAddrFuncPtr = gfxTiDssSetFbAddr;
#if defined(GFX_VSYNC_IRQ)
        pDev->intHandlerVsyncFuncPtr = gfxTiDssIntHandlerVsync;
#endif
        if (ERROR == gfxTiDssHwInit (pDev))
            {
            drvCleanup (pDev);
            return;
            }

        }

    if (ERROR == drvInstall ())
        {
        (void)fprintf (stderr, "Unable to install driver\n");
        drvCleanup (pDev);
        return;
        }

    if (ERROR == devAdd (pDev))
        {
        (void)fprintf (stderr, "Unable to add driver\n");
        drvCleanup (pDev);
        return;
        }

    pDev->enabled = TRUE;
    }
