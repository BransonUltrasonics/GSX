Symbol usb2HidDescriptorGet {
}
Symbol usb2FindInterface {
}
Symbol usb2HidReportDescriptorGet {
}
Symbol usb2VendorClassSpecific {
}
Symbol usb2HidIdleTimeSet {
}
Symbol usb2HidIdleTimeGet {
}
Symbol usb2HidProtoSet {
}
Symbol usb2HidProtoGet {
}
Symbol usb2HidReportSet {
}
Symbol usb2HidReportGet {
}
Symbol usb2HidCalculateIdleTimeFromMs {
}
Symbol usb2HIDReportDescDump {
}
Symbol hid_init {
}
Symbol hid_use_report_desc {
}
Symbol hid_dump_descriptor {
}
Symbol hid_dispose_report_desc {
}
Symbol dump_hid_table {
}
Symbol printf {
}
Symbol hid_usage_page {
}
Symbol snprintf {
}
Symbol hid_usage_in_page {
}
Symbol hid_parse_usage_page {
}
Symbol strcmp {
}
Symbol hid_parse_usage_in_page {
}
Symbol strchr {
}
Symbol strncmp {
}
Symbol hid_dump_item {
}
Symbol __stdout {
}
Symbol fprintf {
}
Symbol hid_start_parse {
}
Symbol hid_get_item {
}
Symbol hid_end_parse {
}
Symbol malloc {
}
Symbol memset {
}
Symbol free {
}
Symbol hid_report_size {
}
Symbol hid_locate {
}
Symbol hid_get_data {
}
Symbol hid_get_data_unsigned {
}
Symbol hid_put_data_unsigned {
}
Symbol __lsl64 {
}
Symbol __lsr64 {
}
Symbol hid_is_collection {
}
Symbol hid_is_mouse {
}
Symbol hid_is_keyboard {
}
Symbol __errno {
}
Symbol memcpy {
}
ObjectFile libusbhid.a::usb2Hid.o {
	NAME Object file usb2Hid.o from archive libusbhid.a
	EXPORTS usb2HidDescriptorGet usb2HidReportDescriptorGet usb2HidIdleTimeSet usb2HidIdleTimeGet usb2HidProtoSet usb2HidProtoGet usb2HidReportSet usb2HidReportGet usb2HidCalculateIdleTimeFromMs usb2HIDReportDescDump
	IMPORTS usb2FindInterface usb2VendorClassSpecific hid_init hid_use_report_desc hid_dump_descriptor hid_dispose_report_desc
	DECLARES 
	USES 
}
Module usb2Hid.o {
	OBJECT += libusbhid.a::usb2Hid.o
}
ObjectFile libusbhid.a::usage.o {
	NAME Object file usage.o from archive libusbhid.a
	EXPORTS dump_hid_table hid_init hid_usage_page hid_usage_in_page hid_parse_usage_page hid_parse_usage_in_page hid_dump_item hid_dump_descriptor
	IMPORTS printf snprintf strcmp strchr strncmp __stdout fprintf hid_start_parse hid_get_item hid_end_parse
	DECLARES 
	USES 
}
Module usage.o {
	OBJECT += libusbhid.a::usage.o
}
ObjectFile libusbhid.a::usb_hid.o {
	NAME Object file usb_hid.o from archive libusbhid.a
	EXPORTS hid_start_parse hid_end_parse hid_get_item hid_report_size hid_locate hid_get_data hid_get_data_unsigned hid_put_data_unsigned hid_is_collection hid_is_mouse hid_is_keyboard hid_use_report_desc hid_dispose_report_desc
	IMPORTS malloc memset free printf __lsl64 __lsr64 __errno memcpy
	DECLARES 
	USES 
}
Module usb_hid.o {
	OBJECT += libusbhid.a::usb_hid.o
}
Library libusbhid.a {
	MODULES libusbhid.a::usb2Hid.o libusbhid.a::usage.o libusbhid.a::usb_hid.o
}
