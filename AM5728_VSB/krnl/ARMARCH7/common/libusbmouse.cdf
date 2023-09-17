Symbol usb2TakeDevice {
}
Symbol usb2ReleaseDevice {
}
Symbol evdevPtrSendMsg {
}
Symbol usbHstCommonCallback {
}
Symbol usbHstURBSubmit {
}
Symbol usbHstURBCancel {
}
Symbol ossFree {
}
Symbol usb2ClassDeviceDelete {
}
Symbol semTake {
}
Symbol ossCalloc {
}
Symbol usbdTranslateDeviceHandle {
}
Symbol usbHstSetConfiguration {
}
Symbol usbHstSetInterface {
}
Symbol usb2FindInterface {
}
Symbol usb2FindEndpoint {
}
Symbol memset {
}
Symbol cacheArchAlignSize {
}
Symbol usbHstPipePrepare {
}
Symbol pUsb2MseDriver {
}
Symbol usb2TakeDriver {
}
Symbol usb2ClassDeviceCreate {
}
Symbol usb2ReleaseDriver {
}
Symbol usb2HidProtoSet {
}
Symbol usb2HidIdleTimeSet {
}
Symbol bzero {
}
Symbol evdevPtrReg {
}
Symbol usbMsgPost {
}
Symbol evdevPtrUnreg {
}
Symbol usb2DeviceDeleteMsgPost {
}
Symbol usb2MseInit {
}
Symbol usb2ClassDriverCreate {
}
Symbol usbHstDriverRegister {
}
Symbol usb2ClassDriverDelete {
}
Symbol usb2MseDeinit {
}
Symbol usbHstDriverDeregister {
}
Symbol usb2DriverIsEmpty {
}
Symbol sysClkRateGet {
}
Symbol taskDelay {
}
ObjectFile libusbmouse.a::usb2Mse.o {
	NAME Object file usb2Mse.o from archive libusbmouse.a
	EXPORTS pUsb2MseDriver usb2MseInit usb2MseDeinit
	IMPORTS usb2TakeDevice usb2ReleaseDevice evdevPtrSendMsg usbHstCommonCallback usbHstURBSubmit usbHstURBCancel ossFree usb2ClassDeviceDelete semTake ossCalloc usbdTranslateDeviceHandle usbHstSetConfiguration usbHstSetInterface usb2FindInterface usb2FindEndpoint memset cacheArchAlignSize usbHstPipePrepare usb2TakeDriver usb2ClassDeviceCreate usb2ReleaseDriver usb2HidProtoSet usb2HidIdleTimeSet bzero evdevPtrReg usbMsgPost evdevPtrUnreg usb2DeviceDeleteMsgPost usb2ClassDriverCreate usbHstDriverRegister usb2ClassDriverDelete usbHstDriverDeregister usb2DriverIsEmpty sysClkRateGet taskDelay
	DECLARES 
	USES 
}
Module usb2Mse.o {
	OBJECT += libusbmouse.a::usb2Mse.o
}
Library libusbmouse.a {
	MODULES libusbmouse.a::usb2Mse.o
}
