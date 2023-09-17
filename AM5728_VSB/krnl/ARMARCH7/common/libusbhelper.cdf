Symbol usb2FindInterface {
}
Symbol usb2FindEndpoint {
}
Symbol usb2FindEndpointHelperDescr {
}
Symbol lstFirst {
}
Symbol lstNext {
}
Symbol usb2DriverIsEmpty {
}
Symbol usb2ClassDeviceDelete {
}
Symbol lstDelete {
}
Symbol ossFree {
}
Symbol semTake {
}
Symbol semDelete {
}
Symbol usb2ClassDeviceCreate {
}
Symbol ossCalloc {
}
Symbol semMCreate {
}
Symbol snprintf {
}
Symbol semBCreate {
}
Symbol lstAdd {
}
Symbol usb2ClassDeviceIsValid {
}
Symbol lstFind {
}
Symbol usb2TakeDevice {
}
Symbol semGive {
}
Symbol usb2ReleaseDevice {
}
Symbol usb2TakeDriver {
}
Symbol usb2ReleaseDriver {
}
Symbol usb2VendorClassSpecific {
}
Symbol usbHstCommonCallback {
}
Symbol usbHstURBSubmit {
}
Symbol errnoSet {
}
Symbol sysClkRateGet {
}
Symbol __div32 {
}
Symbol errnoGet {
}
Symbol usbHstURBCancel {
}
Symbol usb2ClassDeviceAddCallbackSet {
}
Symbol usb2ClassDeviceDelCallbackSet {
}
Symbol usb2ClassDriverDelete {
}
Symbol usb2ClassDriverCreate {
}
Symbol lstInit {
}
Symbol strlen {
}
Symbol bcopy {
}
Symbol msgQReceive {
}
Symbol msgQSend {
}
Symbol taskDelay {
}
Symbol msgQDelete {
}
Symbol usb2DeviceDeleteMsgPost {
}
Symbol usb2DeviceDeleteHelperInit {
}
Symbol msgQCreate {
}
Symbol taskSpawn {
}
Symbol usb2DeviceDeleteHelperDeInit {
}
ObjectFile libusbhelper.a::usb2Helper.o {
	NAME Object file usb2Helper.o from archive libusbhelper.a
	EXPORTS usb2FindInterface usb2FindEndpoint usb2FindEndpointHelperDescr usb2DriverIsEmpty usb2ClassDeviceDelete usb2ClassDeviceCreate usb2ClassDeviceIsValid usb2TakeDevice usb2ReleaseDevice usb2TakeDriver usb2ReleaseDriver usb2VendorClassSpecific usb2ClassDeviceAddCallbackSet usb2ClassDeviceDelCallbackSet usb2ClassDriverDelete usb2ClassDriverCreate usb2DeviceDeleteMsgPost usb2DeviceDeleteHelperInit usb2DeviceDeleteHelperDeInit
	IMPORTS lstFirst lstNext lstDelete ossFree semTake semDelete ossCalloc semMCreate snprintf semBCreate lstAdd lstFind semGive usbHstCommonCallback usbHstURBSubmit errnoSet sysClkRateGet __div32 errnoGet usbHstURBCancel lstInit strlen bcopy msgQReceive msgQSend taskDelay msgQDelete msgQCreate taskSpawn
	DECLARES 
	USES 
}
Module usb2Helper.o {
	OBJECT += libusbhelper.a::usb2Helper.o
}
Library libusbhelper.a {
	MODULES libusbhelper.a::usb2Helper.o
}
