/* 40usb.cdf - Universal Serial Bus component description file */

/*
 * Copyright (c) 2000-2002, 2004-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
14may14,m_y  add USB controller folder
17apr14,s_z  add USB target stack support on VxBus new mode and
             legacy mode (US23307 and US34223), move ossLib init
             to usrUsbInit.c, add init group
24feb14,j_x  add FOLDER_CONNECTIVITY to the ROOT folder (VXW7-2089)
11feb14,j_x  decoupling USB CORE with USBD
10jan14,s_z  make USB component enable on both new VxBus model and vxBus
             legacy model, move INCLUDE_USB from usb host cdf to this file
06jan14,j_x  split to different layers
26dec13,y_f  move kbd, mse and ts to evdev
12nov13,j_x  abstract kbd attach to shell to evdev kbd lib
16oct13,j_x  Remove GEN1 components (WIND00429656)
10oct13,y_f  update evdev dependence
01sep13,j_x  Add touch screen
09aug13,wyy  Drop USB_GEN2_SERIAL_COMMON_TASK_PRIORITY from 100 to 150 to
             prevent that task from occupying too many time slot (WIND00429944)
23jul13,j_x  Support evdev/legacy model
28apr13,ghs  Add support of USB message (WIND00411952)
16apr13,ghs  Add USB PL2303 serial device support (WIND00412983)
16jan13,x_s  Added the hidden component INCLUDE_USB_PCI_STUB_CUSTOM.
06jan13,s_z  Redefine debug level using bitmap (WIND00371346)
25sep12,j_x  Add XHCD debug support
16may12,s_z  Add USB PLX TCD support
19mar11,w_x  Add USB 3.0 xHCI HCD related components
24may12,ljg  Require INCLUDE_SHELL when add KEYBOARD_SHELL_ATTACH
             (WIND00347236)
16aug11,j_x  Unify name of MHCD to MHDRCHCD (WIND00288821)
05aug11,ljg  Modify Capabilities String (WIND00291584)
06jul11,ghs  Adapting GEN2 MSC Driver parameters (WIND00183500)
12jul11,s_z  Add IAD support by default
22apr11,ljg  Add USBTGT printer and USBTGT keyboard emulators support
10apr11,s_z  Correct the definition of USBTGT_NETWORK_DATA_POOL_COUNT
07apr11,ghs  Correct Target Serial Function driver parameter name
             (WIND00266172)
28mar11,s_z  Remove unused components
24mar11,s_z  Modify the usb target RNDIS function driver configuration
24mar11,m_y  Modify usb target MSC function driver configuration items
23mar11,w_x  Remove unnecessary OTG parameters
22mar11,ghs  Remove unused parameter for serial emulator(WIND00261904)
17mar11,m_y  Modify the usb target MSC function driver configuration
08mar11,s_z  Fix RNDIS dependence issue (WIND00258048)
08mar11,w_x  Add more OTG configuration parameters
07mar11,ghs  fix code review issues
23feb11,s_z  Add parameters to charge the target configuration
22feb11,w_x  Fix some target component dependencies
18feb11,w_x  Update target MSC and MHDRC TCD configuration
12feb11,s_z  Add USB target network function driver support
30jan11,ghs  Add usb serial emulator support
14jan11,w_x  Initial USB OTG support changes (WIND00211541)
14jan11,ghs  Correct dependence of audio demo (WIND00250645)
07dec10,ghs  Change initialization order of USB host and target stack
             (WIND00245236)
25nov10,ghs  Correct target stack initialization order (WIND00242986)
10nov10,ghs  Change default name size for Bulk/CBI driver (WIND00240908)
01nov10,ghs  Disable usbPciStub.c for 64-bit compilation (WIND00237286)
22oct10,ghs  Add usrUsbAudioDemo to usbHostDevInit (WIND00237406)
18oct10,ghs  Combine class driver initialization into one group
             (WIND00235953)
16sep10,j_l  Include DMA32 only when needed (Fix for WIND00230745)
08sep10,ghs  Add USB MHCI and SYNOPSYSHCI support
07sep10,ghs  Use "usb0" as usb pegasus default name
01sep10,ghs  Add parameter PEGASUS_END_NAME (WIND00231161)
23aug10,m_y  Update prototype for EHCI/OHCI/UHCI init routine
             (WIND00229662)
07jul10,m_y  Mask synopsys USB host controller support
10jun10,ghs  Add USB serial components
24may10,m_y  Add parameter USB_WAIT_TIME_OUT_VALUE define.(WIND00183499)
23mar10,j_x  Changed for USB debug (WIND00184542)
04mar10,ghs  Required dependency add INCLUDE_PC_CONSOLE for usb boot shell
             attach(WIND00202105)
04mar10,m_y  add USB_CHECK_RESUBMIT_URB_FLAG define.(WIND00200272)
17nov09,m_y  Add synopsys USB host controller support(WIND00123426)
20oct09,w_x  Fix GEN2 MSC dependency and a small typo (WIND00187216)
09sep09,adh  Add USB_MEM_PART_SIZE define (WIND00142070)
09sep09,w_x  Remove FSL TCD silicon config options, they now should be set
             by BSP config.h (WIND00180575)
08sep09,y_l  Add usb Keyboard defination for boot shell and kernel shell
21aug09,ghs  Add USB_HUB_POLLING_INTERVAL define(WIND00160843)
17aug09,w_x  Reorganize GEN2 class driver component description
02jul09,s_z  Add the GEN2 Dm960x serial USB-Ethernet support and adjust
             the GEN2 USB-Ethernet dependency.
18jun09,s_z  Adjust the GEN2 USB-Ethernet dependency.
29apr09,l_z  GEN2 device driver dependency.
12mar09,w_x  Added more GEN2 mass storage driver configuration options
05mar09,s_z  Added USB GEN2 Usb-Ethernet class driver support
09mar09,w_x  Added FSL TCD driver and USB target CDC serial emulator
05jan09,w_x  Added USB GEN2 MSC class driver support
13nov08,j_x  USB GEN2 class driver support
09jul08,s_z  Reorder initialization of USB Keyboard (Defect WIND00119420)
19sep07,ami  CQ:WIND00102614 Fix (Changes in USB Initialization Sequence)
31aug07,jrp  WIND0010335 - remove trailing blanks in continuations
02aug07,adh  WIND00034090 fix, usb Keyboard in target shell
10aug07,p_g  defect fix WIND00038962
26jul07,jrp  WIND00099137 - changed initialization to match command line
28mar07,sup  Change the return type of API for speaker initialization
05feb07,jrp  USB Headset Integration and Defect 69432
25jan07,ami  Changes made as per vxBus modifications
18jul06,???  fix for defect WIND00045229
01mar06,ami  Extra Tab removed from END_PEGASUS Component (SPR#114377)
10jan06,ami  Parameter to configure the Keyboard Buffer Size added (SPR
             #116401 Fix)
26dec05,ami  Required Dependency added for the USB Tool (SPR #113168)
12oct05,hch  Fix malta4kc peripheral project initialization bug(SPR#110507)
             Removed the extra "\" in INCLUDE_USB_MS_CBI_INIT component
07oct05,hch  Add USB_BULK_NON_REMOVABLE_DISK configuration parameter
28sep05,hch  Remove INCLUDE_DOSFS from INCLUDE_USB_MS_CBI_INIT(SPR#112903)
21sep05,ami  Dependencies of usbTool Components added (SPR#111867)
24aug05,ami  Returned type of function prototype changed (SPR #111445)
18apr05,hch  Add PEGASUS_MAX_DEVS configuration parameter
03mar05,hch  Remove PEGASUS_DESTINATION_ADDRESS parameter
             Change the default PEGASUS_MAX_DEVS to 1
02mar05,hch  Change the PEGASUS_TARGET_NAME to be usbTarg
25feb05,hch  Add macros for multiple Pegasus device support (SPR #105739)
20jan05,ami  Modifications to configure UFI_MAX_DEVS & UFI_MAX_DRV_NAME_SZ
             from Project Facility
06jan05,ami  Bulk Maximum Device Name Size added
05jan05,ami  Changes for setting the maximum number of drives supported by
             Bulk Class Driver through Project Facility
20sep04,ami  NET2280 tested for High Speed
17sep04,ami  Changes for NET2280
16aug04,pdg  Fix for long booting time of OHCI and EHCI host controllers
             in pentium
21jul04,hch  Merged from development view
             Corrected the file versions in modification history
16jul04,???  Removed dependency of usbTool on the USB host
09jul04,???  Support for isp1582 initialization using project facility
30jun04,???  mass storage emulator inclusion
11may04,hch  merge after D12 driver testing.
22apr04,???  Support for USB 2.0 peripheral stack
08may02,wef  SPR #77048 - removed INCLUDE_OHCI_PCI_INIT component.
27mar02,wef  SPR 74822: fixed typeo in INCLUDE_AUDIO_DEMO component.
26mar02,pmr  SPR 73970: removed INCLUDE_PCI requirement from
             INCLUDE_OHCI_PCI_INIT.
22mar02,rhe  Remove unwanted tab in USB Audio Demo SPR 73326
08dec01,dat  Adding BSP_STUBS, INIT_BEFORE,
08dec01,wef  removed references to ACM, KLSI, NC1080 and UGL components /
             parameters, added new PEGASUS parameters.
25apr01,wef  moved end initialization to usrIosExtraInit
25apr01,wef  added communication, mass storage class, ugl drivers, moved
             usb init order to usrIosCoreInit
01may00,wef  removed usbTargTool
25apr00,wef  added usbTargTool for testing the USB Target stack
29mar00,wef  broke up usrUsbPciLib.c into three separtate
             stub<bsp>PciLib.c files included in bsp conponent
10feb00,wef  cleaned-up component description and removed init groups.
09feb00,wef  added component INCLUDE_OHCI_INIT.  initializes OHCI on PCI
             bus, fixed bug - two instances of MODULE instead of MODULES
25jan00,wef  written
*/

/*
DESCRIPTION
This file contains descriptions for the USB components.
*/

/* connectivity folder */

Folder FOLDER_CONNECTIVITY {
    NAME       connectivity components
    SYNOPSIS   connectivity components
    _CHILDREN  FOLDER_ROOT
}

/* usb core components */

Folder    FOLDER_USB {
    NAME            USB
    SYNOPSIS        Universal Serial Bus (USB)
    _CHILDREN       FOLDER_CONNECTIVITY
}

Folder    FOLDER_USB_COMMON {
    NAME            USB Common
    SYNOPSIS        USB Common Components
    _CHILDREN       FOLDER_USB
}

Folder    FOLDER_USB_CONTROLLER {
    NAME            USB Controller
    SYNOPSIS        USB Controller Components
    _CHILDREN       FOLDER_USB
}

Component    INCLUDE_USB {
    NAME            USB Common Stack
    _CHILDREN       FOLDER_USB_COMMON
    SYNOPSIS        USB Common Stack
#ifdef _WRS_CONFIG_VXBUS_LEGACY
#ifndef _WRS_CONFIG_LP64
    BSP_STUBS       usbPciStub.c
#endif
#endif
    CONFIGLETTES    usrUsbInit.c    \
                    usrUsbDebug.c
    CFG_PARAMS      USB_HUB_POLLING_INTERVAL           \
                    USB_MEM_PART_SIZE                  \
                    USB_CHECK_RESUBMIT_URB_FLAG        \
                    USB_WAIT_TIME_OUT_VALUE
}

Component    INCLUDE_USB_INIT {
    NAME            USB Common Stack Init
    _CHILDREN       FOLDER_USB_COMMON
    SYNOPSIS        USB Common Stack Initialization
    REQUIRES        INCLUDE_USB
    CONFIGLETTES    usrUsbInit.c
    PROTOTYPE       STATUS usbInit (void);
    INIT_RTN        usbInit ();
}


InitGroup usrUsbPerStageInit {
    PROTOTYPE       void usrUsbPerStageInit (void)
    INIT_RTN        usrUsbPerStageInit();
    SYNOPSIS        The usb earlier initialization before vxBus init
    INIT_ORDER      INCLUDE_USB_INIT  \
                    INCLUDE_USB_TGT
#ifndef _WRS_CONFIG_VXBUS_LEGACY
    _INIT_ORDER     usrRoot
    INIT_BEFORE     INCLUDE_VXBUS
#else
    _INIT_ORDER     vxbDevInit
    INIT_BEFORE     INCLUDE_VXBUS_DEV_INIT
#endif
}


InitGroup usrUsbGroupInit {
    PROTOTYPE       void usrUsbGroupInit (void)
    INIT_RTN        usrUsbGroupInit();
    SYNOPSIS        The USB group initialization
    INIT_ORDER      vxbUsbTcdRegister        \
                    usrUsbTgtFuncInit
    _INIT_ORDER     usrRoot
    INIT_AFTER      usrIosExtraInit
    INIT_BEFORE     usrToolsInit
}

Parameter    USB_HUB_POLLING_INTERVAL {
    NAME            USB Hub Polling Interval (in ms)
    SYNOPSIS        USB Hub Polling Interval (in ms)
    REQUIRES        INCLUDE_USB
    TYPE            int
    DEFAULT         2
}

Parameter    USB_MEM_PART_SIZE {
    NAME            USB Initial Memory Partition Size (in bytes)
    SYNOPSIS        USB Initial Memory Partition Size (in bytes)
    REQUIRES        INCLUDE_USB
    TYPE            int
    DEFAULT         0x200000
}

Parameter    USB_CHECK_RESUBMIT_URB_FLAG {
    NAME            Check URB resubmited multiple times enable flag (TRUE or FALSE)
    SYNOPSIS        Check if a URB is resubmitted multiple times with the same transfer buffer (TRUE or FALSE)
    REQUIRES        INCLUDE_USB
    TYPE            bool
    DEFAULT         FALSE
}

Parameter    USB_WAIT_TIME_OUT_VALUE {
    NAME            USB Waiting Time Out Value (in ms)
    SYNOPSIS        USB Waiting Time Out Value (in ms)
    REQUIRES        INCLUDE_USB
    TYPE            int
    DEFAULT         5000
}

/* debug information */

Folder    FOLDER_USB_SHOW {
    NAME            USB Show
    SYNOPSIS        USB Stack Show
    _CHILDREN       FOLDER_USB_DEBUG
    CHILDREN        INCLUDE_USB_SHOW
}

Component    INCLUDE_USB_SHOW {
    NAME            usbShow
    SYNOPSIS        USB Show - Show USB device information
    REQUIRES        INCLUDE_USB
    CONFIGLETTES    usrUsbShow.c
    CFG_PARAMS      USB_SHOW_LEVEL
}

Parameter    USB_SHOW_LEVEL {
    NAME            USB Show Level
    SYNOPSIS        USB Show Level (0:SIMPLE, 1:NORMAL, 2:VERBOSE)
    DEFAULT         2
}



/* debug information */

Folder    FOLDER_USB_DEBUG {
    NAME            USB Debug
    SYNOPSIS        USB Stack Debug
    _CHILDREN       FOLDER_USB
}

Folder    FOLDER_USB_TRACK {
    NAME            USB Track
    SYNOPSIS        USB Stack Track
    _CHILDREN       FOLDER_USB_DEBUG
}

Folder    FOLDER_USB_TRACK_TU {
    NAME            TU
    SYNOPSIS        USB Translation Unit
    _CHILDREN       FOLDER_USB_TRACK
    CHILDREN        INCLUDE_USB_TRACK_TU
}

Component    INCLUDE_USB_TRACK_TU {
    NAME            TU
    SYNOPSIS        USB Translation Unit Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB
    CFG_PARAMS      USB_TRACK_TU
}

Parameter    USB_TRACK_TU {
    NAME            USB TU Debug Level BitMap
    SYNOPSIS        USB TU Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}
