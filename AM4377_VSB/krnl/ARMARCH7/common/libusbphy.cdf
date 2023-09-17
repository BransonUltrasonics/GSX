Symbol vxbUsbPhyDelete {
}
Symbol usbPhyDevListLock {
}
Symbol usbPhyDevList {
}
Symbol vxbUsbPhyAdd {
}
Symbol usbPhyDevLibInited {
}
Symbol vxAtomicGet {
}
Symbol vxAtomicSet {
}
Symbol vxBusLockInit {
}
Symbol vxbUsbPhyTypeGet {
}
Symbol vxbUsbPhyDisable {
}
Symbol vxbUsbPhyEnable {
}
Symbol vxbUsbPhyRead {
}
Symbol vxbUsbPhyWrite {
}
Symbol vxbDevIvarsGet {
}
Symbol vxbFdtUsbPhyDevGet {
}
Symbol vxbFdtDevGet {
}
Symbol vxFdtPropGet {
}
Symbol vxFdt32ToCpu {
}
Symbol vxFdtNodeOffsetByPhandle {
}
Symbol vxbFdtUsbPhyTypeGet {
}
Symbol strncmp {
}
Symbol vxbFdtDevMatch {
}
Symbol calloc {
}
Symbol memset {
}
Symbol vxbResourceAlloc {
}
Symbol vxbGpioBind {
}
Symbol vxbDevSoftcSet {
}
Symbol vxbDevSoftcGet {
}
Symbol vxbResourceFree {
}
Symbol ossFree {
}
Symbol vxbDevProbe_desc {
}
Symbol vxbDevAttach_desc {
}
Symbol vxbDevDetach_desc {
}
Symbol vxbDevShutdown_desc {
}
Symbol vxbFdtUsbPhyUlpiViewPortDrv {
}
Symbol __dataset__driver_vxbFdtUsbPhyUlpiViewPortDrv {
}
Symbol usbPhyHwInitImx {
}
Symbol usbFslPhyAddr {
}
Symbol vxbWrite32 {
}
Symbol vxbUsDelay {
}
Symbol vxbMsDelay {
}
Symbol usbPhyHwInitVybrid {
}
Symbol vxbRead32 {
}
Symbol vxbPinMuxEnable {
}
Symbol vxbClkEnableAll {
}
Symbol g_usbPlatformFlag {
}
Symbol vxbWrite8 {
}
Symbol vxbSwap32 {
}
Symbol vxbFdtUsbPhyFslOnChipDrv {
}
Symbol __dataset__driver_vxbFdtUsbPhyFslOnChipDrv {
}
Symbol free {
}
Symbol sysUsDelay {
}
Symbol sysMsDelay {
}
Symbol vxbFdtUsbPhyAltSocGen5Drv {
}
Symbol __dataset__driver_vxbFdtUsbPhyAltSocGen5Drv {
}
Symbol vxbDevAcquireByName {
}
Symbol vxbFdtUsbPhyAm335xDrv {
}
Symbol __dataset__driver_vxbFdtUsbPhyAm335xDrv {
}
ObjectFile libusbphy.a::vxbUsbPhyLib.o {
	NAME Object file vxbUsbPhyLib.o from archive libusbphy.a
	EXPORTS vxbUsbPhyDelete vxbUsbPhyAdd usbPhyDevLibInited vxbUsbPhyTypeGet vxbUsbPhyDisable vxbUsbPhyEnable vxbUsbPhyRead vxbUsbPhyWrite
	IMPORTS vxAtomicGet vxAtomicSet vxBusLockInit
	DECLARES usbPhyDevListLock usbPhyDevList
	USES 
}
Module vxbUsbPhyLib.o {
	OBJECT += libusbphy.a::vxbUsbPhyLib.o
}
ObjectFile libusbphy.a::vxbFdtUsbPhyLib.o {
	NAME Object file vxbFdtUsbPhyLib.o from archive libusbphy.a
	EXPORTS vxbFdtUsbPhyDevGet vxbFdtUsbPhyTypeGet
	IMPORTS usbPhyDevListLock usbPhyDevList vxbDevIvarsGet vxbFdtDevGet vxFdtPropGet vxFdt32ToCpu vxFdtNodeOffsetByPhandle strncmp
	DECLARES 
	USES 
}
Module vxbFdtUsbPhyLib.o {
	OBJECT += libusbphy.a::vxbFdtUsbPhyLib.o
}
ObjectFile libusbphy.a::vxbFdtUsbPhyUlpiViewPort.o {
	NAME Object file vxbFdtUsbPhyUlpiViewPort.o from archive libusbphy.a
	EXPORTS vxbFdtUsbPhyUlpiViewPortDrv __dataset__driver_vxbFdtUsbPhyUlpiViewPortDrv
	IMPORTS vxbFdtDevMatch calloc memset vxbResourceAlloc vxbFdtUsbPhyTypeGet vxbGpioBind vxbDevSoftcSet vxbUsbPhyAdd vxbDevSoftcGet vxbUsbPhyDelete vxbResourceFree ossFree vxbDevProbe_desc vxbDevAttach_desc vxbDevDetach_desc vxbDevShutdown_desc
	DECLARES 
	USES 
}
Module vxbFdtUsbPhyUlpiViewPort.o {
	OBJECT += libusbphy.a::vxbFdtUsbPhyUlpiViewPort.o
}
ObjectFile libusbphy.a::vxbFdtUsbPhyFslOnChip.o {
	NAME Object file vxbFdtUsbPhyFslOnChip.o from archive libusbphy.a
	EXPORTS usbPhyHwInitImx usbFslPhyAddr usbPhyHwInitVybrid g_usbPlatformFlag vxbFdtUsbPhyFslOnChipDrv __dataset__driver_vxbFdtUsbPhyFslOnChipDrv
	IMPORTS vxbDevIvarsGet strncmp vxbWrite32 vxbUsDelay vxbMsDelay vxbRead32 vxbFdtDevGet vxbFdtDevMatch vxbPinMuxEnable vxbClkEnableAll vxFdtPropGet vxbGpioBind calloc memset vxbResourceAlloc ossFree vxbWrite8 vxbSwap32 vxbDevSoftcSet vxbFdtUsbPhyTypeGet vxbUsbPhyAdd vxbDevSoftcGet vxbUsbPhyDelete vxbResourceFree vxbDevProbe_desc vxbDevAttach_desc vxbDevDetach_desc vxbDevShutdown_desc
	DECLARES 
	USES 
}
Module vxbFdtUsbPhyFslOnChip.o {
	OBJECT += libusbphy.a::vxbFdtUsbPhyFslOnChip.o
}
ObjectFile libusbphy.a::vxbFdtUsbPhyAltSocGen5.o {
	NAME Object file vxbFdtUsbPhyAltSocGen5.o from archive libusbphy.a
	EXPORTS vxbFdtUsbPhyAltSocGen5Drv __dataset__driver_vxbFdtUsbPhyAltSocGen5Drv
	IMPORTS vxbFdtDevMatch calloc vxbResourceAlloc vxbDevSoftcSet vxbFdtUsbPhyTypeGet vxbUsbPhyAdd vxbResourceFree free sysUsDelay vxbRead32 vxbWrite32 sysMsDelay vxbDevProbe_desc vxbDevAttach_desc
	DECLARES 
	USES 
}
Module vxbFdtUsbPhyAltSocGen5.o {
	OBJECT += libusbphy.a::vxbFdtUsbPhyAltSocGen5.o
}
ObjectFile libusbphy.a::vxbFdtUsbPhyAm335x.o {
	NAME Object file vxbFdtUsbPhyAm335x.o from archive libusbphy.a
	EXPORTS vxbFdtUsbPhyAm335xDrv __dataset__driver_vxbFdtUsbPhyAm335xDrv
	IMPORTS vxbFdtDevMatch calloc vxbDevSoftcSet vxbFdtUsbPhyTypeGet vxbUsbPhyAdd vxbDevSoftcGet vxbUsbPhyDelete ossFree vxbDevAcquireByName vxbWrite32 vxbMsDelay vxbDevProbe_desc vxbDevAttach_desc vxbDevDetach_desc vxbDevShutdown_desc
	DECLARES 
	USES 
}
Module vxbFdtUsbPhyAm335x.o {
	OBJECT += libusbphy.a::vxbFdtUsbPhyAm335x.o
}
Library libusbphy.a {
	MODULES libusbphy.a::vxbUsbPhyLib.o libusbphy.a::vxbFdtUsbPhyLib.o libusbphy.a::vxbFdtUsbPhyUlpiViewPort.o libusbphy.a::vxbFdtUsbPhyFslOnChip.o libusbphy.a::vxbFdtUsbPhyAltSocGen5.o libusbphy.a::vxbFdtUsbPhyAm335x.o
}
