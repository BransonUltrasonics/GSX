Symbol usb2HidReportSet {
}
Symbol bcopy {
}
Symbol usb2ReleaseDevice {
}
Symbol usb2TakeDevice {
}
Symbol evdevKbdSendMsg {
}
Symbol usbHstCommonCallback {
}
Symbol usbHstURBSubmit {
}
Symbol errnoSet {
}
Symbol usbHstURBCancel {
}
Symbol excJobAdd {
}
Symbol semDelete {
}
Symbol ossFree {
}
Symbol usb2ClassDeviceDelete {
}
Symbol semTake {
}
Symbol ossCalloc {
}
Symbol semBCreate {
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
Symbol pUsb2KbdDriver {
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
Symbol evdevKbdReg {
}
Symbol usbMsgPost {
}
Symbol evdevKbdUnreg {
}
Symbol usb2DeviceDeleteMsgPost {
}
Symbol usb2KbdInit {
}
Symbol usb2ClassDriverCreate {
}
Symbol usbHstDriverRegister {
}
Symbol usb2ClassDriverDelete {
}
Symbol usb2KbdDeinit {
}
Symbol usbHstDriverDeregister {
}
Symbol usb2DriverIsEmpty {
}
Symbol sysClkRateGet {
}
Symbol taskDelay {
}
ObjectFile libusbkeyboard.a::usb2Kbd.o {
	NAME Object file usb2Kbd.o from archive libusbkeyboard.a
	EXPORTS pUsb2KbdDriver usb2KbdInit usb2KbdDeinit
	IMPORTS usb2HidReportSet bcopy usb2ReleaseDevice usb2TakeDevice evdevKbdSendMsg usbHstCommonCallback usbHstURBSubmit errnoSet usbHstURBCancel excJobAdd semDelete ossFree usb2ClassDeviceDelete semTake ossCalloc semBCreate usbdTranslateDeviceHandle usbHstSetConfiguration usbHstSetInterface usb2FindInterface usb2FindEndpoint memset cacheArchAlignSize usbHstPipePrepare usb2TakeDriver usb2ClassDeviceCreate usb2ReleaseDriver usb2HidProtoSet usb2HidIdleTimeSet evdevKbdReg usbMsgPost evdevKbdUnreg usb2DeviceDeleteMsgPost usb2ClassDriverCreate usbHstDriverRegister usb2ClassDriverDelete usbHstDriverDeregister usb2DriverIsEmpty sysClkRateGet taskDelay
	DECLARES 
	USES 
}
Module usb2Kbd.o {
	OBJECT += libusbkeyboard.a::usb2Kbd.o
}
Library libusbkeyboard.a {
	MODULES libusbkeyboard.a::usb2Kbd.o
}
