Symbol vxbFdtDevMatch {
}
Symbol sdhcCtrlProbe {
}
Symbol vxbDevDrvDataSet {
}
Symbol vxbDevSoftcGet {
}
Symbol sdhcCtrlInstConnect {
}
Symbol vxbFdtDevGet {
}
Symbol vxFdtPropGet {
}
Symbol vxFdt32ToCpu {
}
Symbol taskSpawn {
}
Symbol vxbWrite32 {
}
Symbol vxbIntConnect {
}
Symbol vxbIntEnable {
}
Symbol taskDelete {
}
Symbol vxbGpioGetValue {
}
Symbol vxbRead32 {
}
Symbol semGive {
}
Symbol sdhcCtrlAttach {
}
Symbol vxbDevDrvDataGet {
}
Symbol vxbClkEnableAll {
}
Symbol vxbPinMuxEnable {
}
Symbol vxbGpioAlloc {
}
Symbol vxbGpioSetDir {
}
Symbol vxbGpioIntConfig {
}
Symbol vxbGpioIntConnect {
}
Symbol vxbGpioIntEnable {
}
Symbol vxbGpioFree {
}
Symbol __udiv32 {
}
Symbol vxbUsDelay {
}
Symbol sdhcCtrlInit {
}
Symbol vxbDmaBufMapSync {
}
Symbol sdDeviceCreate {
}
Symbol vxbMsDelay {
}
Symbol vxbDevRemove {
}
Symbol erfLibInitialized {
}
Symbol sysClkRateGet {
}
Symbol taskDelay {
}
Symbol sdhcCtrlCmdIssuePoll {
}
Symbol sdhcCtrlCmdIssue {
}
Symbol semTake {
}
Symbol __div32 {
}
Symbol sdhcCtrlCardInsertSts {
}
Symbol vxbDevProbe_desc {
}
Symbol vxbDevAttach_desc {
}
Symbol vxbSdspecinfoGet_desc {
}
Symbol vxbFslSdhcCtrlDrv {
}
Symbol __dataset__driver_vxbFslSdhcCtrlDrv {
}
Symbol calloc {
}
Symbol vxbDevSoftcSet {
}
Symbol vxbClkGet {
}
Symbol vxbClkEnable {
}
Symbol vxbClkRateGet {
}
Symbol vxbResourceAlloc {
}
Symbol vxbDmaBufTagParentGet {
}
Symbol cacheArchAlignSize {
}
Symbol vxbDmaBufTagCreate {
}
Symbol sdhcCtrlBusWidthSetup {
}
Symbol sdhcCtrlCardWpCheck {
}
Symbol sdhcCtrlClkFreqSetup {
}
Symbol sdhcCtrlCmdPrepare {
}
Symbol sdhcCtrlIsr {
}
Symbol free {
}
Symbol semBCreate {
}
Symbol semMCreate {
}
Symbol semDelete {
}
Symbol vxbDmaBufMapCreate {
}
Symbol sdhcCtrlPioRead {
}
Symbol sdhcCtrlPioWrite {
}
Symbol vxbDevIvarsGet {
}
Symbol vxbDmaBufMapLoad {
}
Symbol vxbDmaBufMapUnload {
}
Symbol vxbPinMuxEnableById {
}
Symbol sdSendTuning {
}
Symbol vxbImxSdhcCtrlDrv {
}
Symbol __dataset__driver_vxbImxSdhcCtrlDrv {
}
Symbol vxbPciDevMatch {
}
Symbol vxbDevClassGet {
}
Symbol vxbDevParent {
}
Symbol VXB_PCI_CFG_READ {
}
Symbol vxbPciSdhcCtrlDrv {
}
Symbol __dataset__driver_vxbPciSdhcCtrlDrv {
}
Symbol vxbZynqSdhcCtrlDrv {
}
Symbol __dataset__driver_vxbZynqSdhcCtrlDrv {
}
Symbol vxbDmaChanFree {
}
Symbol vxbDmaChanAlloc {
}
Symbol cacheUserFuncs {
}
Symbol bzero {
}
Symbol vxbDmaChanControl {
}
Symbol vxbDmaChanPreMemCpy {
}
Symbol vxbDmaChanStart {
}
Symbol vxbGpioSetDebounce {
}
Symbol vxbTiMmchsCtrlDrv {
}
Symbol __dataset__driver_vxbTiMmchsCtrlDrv {
}
ObjectFile libsdhclib.a::vxbFslSdhcCtrl.o {
	NAME Object file vxbFslSdhcCtrl.o from archive libsdhclib.a
	EXPORTS vxbFslSdhcCtrlDrv __dataset__driver_vxbFslSdhcCtrlDrv
	IMPORTS vxbFdtDevMatch sdhcCtrlProbe vxbDevDrvDataSet vxbDevSoftcGet sdhcCtrlInstConnect vxbFdtDevGet vxFdtPropGet vxFdt32ToCpu taskSpawn vxbWrite32 vxbIntConnect vxbIntEnable taskDelete vxbGpioGetValue vxbRead32 semGive sdhcCtrlAttach vxbDevDrvDataGet vxbClkEnableAll vxbPinMuxEnable vxbGpioAlloc vxbGpioSetDir vxbGpioIntConfig vxbGpioIntConnect vxbGpioIntEnable vxbGpioFree __udiv32 vxbUsDelay sdhcCtrlInit vxbDmaBufMapSync sdDeviceCreate vxbMsDelay vxbDevRemove erfLibInitialized sysClkRateGet taskDelay sdhcCtrlCmdIssuePoll sdhcCtrlCmdIssue semTake __div32 sdhcCtrlCardInsertSts vxbDevProbe_desc vxbDevAttach_desc vxbSdspecinfoGet_desc
	DECLARES 
	USES 
}
Module vxbFslSdhcCtrl.o {
	OBJECT += libsdhclib.a::vxbFslSdhcCtrl.o
}
ObjectFile libsdhclib.a::vxbSdhcCtrl.o {
	NAME Object file vxbSdhcCtrl.o from archive libsdhclib.a
	EXPORTS sdhcCtrlProbe sdhcCtrlAttach sdhcCtrlBusWidthSetup sdhcCtrlCardWpCheck sdhcCtrlCardInsertSts sdhcCtrlClkFreqSetup sdhcCtrlCmdPrepare sdhcCtrlInit sdhcCtrlIsr sdhcCtrlInstConnect sdhcCtrlPioRead sdhcCtrlPioWrite sdhcCtrlCmdIssue sdhcCtrlCmdIssuePoll
	IMPORTS calloc vxbDevSoftcSet vxbClkGet vxbFdtDevGet vxFdtPropGet vxFdt32ToCpu vxbClkEnable vxbClkRateGet vxbResourceAlloc vxbDmaBufTagParentGet cacheArchAlignSize vxbDmaBufTagCreate free semBCreate semMCreate semDelete vxbDevSoftcGet vxbWrite32 vxbRead32 vxbMsDelay vxbDmaBufMapCreate semGive vxbUsDelay vxbDmaBufMapSync vxbDevIvarsGet semTake vxbDmaBufMapLoad vxbDmaBufMapUnload sysClkRateGet __div32 taskDelay __udiv32
	DECLARES 
	USES 
}
Module vxbSdhcCtrl.o {
	OBJECT += libsdhclib.a::vxbSdhcCtrl.o
}
ObjectFile libsdhclib.a::vxbFslImxShdcCtrl.o {
	NAME Object file vxbFslImxShdcCtrl.o from archive libsdhclib.a
	EXPORTS vxbImxSdhcCtrlDrv __dataset__driver_vxbImxSdhcCtrlDrv
	IMPORTS vxbFdtDevMatch sdhcCtrlProbe vxbDevDrvDataSet vxbDevSoftcGet sdhcCtrlInstConnect taskSpawn vxbWrite32 vxbIntConnect vxbIntEnable taskDelete vxbRead32 vxbGpioGetValue semGive sdhcCtrlAttach vxbFdtDevGet vxbDevDrvDataGet vxFdtPropGet vxFdt32ToCpu vxbClkEnableAll vxbPinMuxEnable vxbGpioAlloc vxbGpioSetDir vxbGpioIntConfig vxbGpioIntConnect vxbGpioIntEnable vxbGpioFree __udiv32 vxbUsDelay sdhcCtrlInit vxbDmaBufMapSync sdDeviceCreate vxbMsDelay vxbDevRemove erfLibInitialized sysClkRateGet taskDelay sdhcCtrlCmdIssuePoll sdhcCtrlCmdIssue vxbPinMuxEnableById semTake __div32 sdhcCtrlCardInsertSts sdhcCtrlCardWpCheck sdSendTuning vxbDevProbe_desc vxbDevAttach_desc vxbSdspecinfoGet_desc
	DECLARES 
	USES 
}
Module vxbFslImxShdcCtrl.o {
	OBJECT += libsdhclib.a::vxbFslImxShdcCtrl.o
}
ObjectFile libsdhclib.a::vxbPciSdhcCtrl.o {
	NAME Object file vxbPciSdhcCtrl.o from archive libsdhclib.a
	EXPORTS vxbPciSdhcCtrlDrv __dataset__driver_vxbPciSdhcCtrlDrv
	IMPORTS vxbPciDevMatch vxbDevSoftcGet sdhcCtrlInstConnect taskSpawn vxbWrite32 vxbIntConnect vxbIntEnable taskDelete vxbRead32 semGive sdhcCtrlAttach vxbDevClassGet vxbDevIvarsGet vxbDevParent VXB_PCI_CFG_READ vxbMsDelay sdhcCtrlInit sdhcCtrlCmdIssuePoll sdhcCtrlCmdIssue sdDeviceCreate vxbDevRemove erfLibInitialized sysClkRateGet taskDelay vxbDevProbe_desc vxbDevAttach_desc vxbSdspecinfoGet_desc
	DECLARES 
	USES 
}
Module vxbPciSdhcCtrl.o {
	OBJECT += libsdhclib.a::vxbPciSdhcCtrl.o
}
ObjectFile libsdhclib.a::vxbZynqSdhcCtrl.o {
	NAME Object file vxbZynqSdhcCtrl.o from archive libsdhclib.a
	EXPORTS vxbZynqSdhcCtrlDrv __dataset__driver_vxbZynqSdhcCtrlDrv
	IMPORTS vxbFdtDevMatch sdhcCtrlProbe vxbDevDrvDataSet vxbDevSoftcGet sdhcCtrlInstConnect vxbFdtDevGet vxFdtPropGet vxFdt32ToCpu taskSpawn vxbWrite32 vxbIntConnect vxbIntEnable taskDelete vxbRead32 semGive sdhcCtrlAttach vxbDevDrvDataGet vxbMsDelay sdhcCtrlInit sdDeviceCreate vxbDevRemove erfLibInitialized sysClkRateGet taskDelay sdhcCtrlCmdIssuePoll sdhcCtrlCmdIssue vxbDevProbe_desc vxbDevAttach_desc vxbSdspecinfoGet_desc
	DECLARES 
	USES 
}
Module vxbZynqSdhcCtrl.o {
	OBJECT += libsdhclib.a::vxbZynqSdhcCtrl.o
}
ObjectFile libsdhclib.a::vxbTiMmchsCtrl.o {
	NAME Object file vxbTiMmchsCtrl.o from archive libsdhclib.a
	EXPORTS vxbTiMmchsCtrlDrv __dataset__driver_vxbTiMmchsCtrlDrv
	IMPORTS vxbFdtDevMatch vxbDevDrvDataSet vxbDevSoftcGet vxbGpioGetValue vxbRead32 vxbWrite32 vxbUsDelay semGive vxbDmaChanFree vxbDmaChanAlloc cacheUserFuncs bzero vxbDmaChanControl vxbDmaChanPreMemCpy vxbDmaChanStart taskSpawn vxbIntConnect vxbIntEnable semBCreate calloc vxbDevSoftcSet vxbFdtDevGet vxFdtPropGet vxFdt32ToCpu vxbClkEnableAll vxbPinMuxEnable vxbResourceAlloc vxbGpioAlloc vxbGpioSetDir vxbGpioSetDebounce vxbGpioIntConfig vxbGpioIntConnect vxbGpioIntEnable vxbDevDrvDataGet vxbGpioFree free __udiv32 erfLibInitialized sysClkRateGet taskDelay sdDeviceCreate vxbDevRemove semTake __div32 vxbDevProbe_desc vxbDevAttach_desc vxbSdspecinfoGet_desc
	DECLARES 
	USES 
}
Module vxbTiMmchsCtrl.o {
	OBJECT += libsdhclib.a::vxbTiMmchsCtrl.o
}
Library libsdhclib.a {
	MODULES libsdhclib.a::vxbFslSdhcCtrl.o libsdhclib.a::vxbSdhcCtrl.o libsdhclib.a::vxbFslImxShdcCtrl.o libsdhclib.a::vxbPciSdhcCtrl.o libsdhclib.a::vxbZynqSdhcCtrl.o libsdhclib.a::vxbTiMmchsCtrl.o
}
