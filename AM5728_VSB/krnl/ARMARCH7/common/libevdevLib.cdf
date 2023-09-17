Symbol evdevPtrReg {
}
Symbol evdevRegDev {
}
Symbol evdevPtrUnreg {
}
Symbol evdevUnregDev {
}
Symbol evdevPtrSendMsg {
}
Symbol tickGet {
}
Symbol sysClkRateGet {
}
Symbol __udiv32 {
}
Symbol evdevSendMsg {
}
Symbol memcpy {
}
Symbol vxAtomicGet {
}
Symbol vxAtomicOr {
}
Symbol evdevTsReg {
}
Symbol calloc {
}
Symbol bcopy {
}
Symbol msgQCreate {
}
Symbol wdCreate {
}
Symbol bzero {
}
Symbol snprintf {
}
Symbol taskSpawn {
}
Symbol wdDelete {
}
Symbol taskDelete {
}
Symbol msgQDelete {
}
Symbol vxAtomicAnd {
}
Symbol free {
}
Symbol evdevTsUnreg {
}
Symbol wdCancel {
}
Symbol evdevTsSendMsg {
}
Symbol msgQSend {
}
Symbol evdevTsSendMtMsg {
}
Symbol evdevTsEvalPoint {
}
Symbol __div32 {
}
Symbol evdevTsDevIsr {
}
Symbol wdStart {
}
Symbol excJobAdd {
}
Symbol evdevTsGetCalibQuick {
}
Symbol strcmp {
}
Symbol printf {
}
Symbol msgQReceive {
}
Symbol evdevKbdMapInit {
}
Symbol lstInit {
}
Symbol evdevKbdMapAdd {
}
Symbol lstAdd {
}
Symbol lstFirst {
}
Symbol lstNext {
}
Symbol evdevKbdReport2Unicode {
}
Symbol evdevKbdModifier2Unicode {
}
Symbol evdevKbdReport2Keycode {
}
Symbol evdevKbdModifier2Keycode {
}
Symbol evdevKbdInit {
}
Symbol evdevKbdGetConsole {
}
Symbol evdevKbdIsRedirectTty {
}
Symbol evdevKbdReg {
}
Symbol tyIRd {
}
Symbol evdevKbdUnreg {
}
Symbol evdevKbdSendMsg {
}
Symbol evdevKbdMapUSInit {
}
Symbol evdevInit {
}
Symbol dllCreate {
}
Symbol semBCreate {
}
Symbol iosDrvInstall {
}
Symbol iosDevAdd {
}
Symbol selWakeupListInit {
}
Symbol iosDevDelete {
}
Symbol semDelete {
}
Symbol iosDrvRemove {
}
Symbol dllDelete {
}
Symbol selWakeupAll {
}
Symbol semTake {
}
Symbol dllAdd {
}
Symbol semGive {
}
Symbol msgQNumMsgs {
}
Symbol selWakeupListTerm {
}
Symbol dllRemove {
}
Symbol strlen {
}
Symbol __errno {
}
Symbol selNodeAdd {
}
Symbol selWakeupType {
}
Symbol selWakeup {
}
Symbol selNodeDelete {
}
Symbol taskDelay {
}
ObjectFile libevdevLib.a::evdevLibPtr.o {
	NAME Object file evdevLibPtr.o from archive libevdevLib.a
	EXPORTS evdevPtrReg evdevPtrUnreg evdevPtrSendMsg
	IMPORTS evdevRegDev evdevUnregDev tickGet sysClkRateGet __udiv32 evdevSendMsg memcpy
	DECLARES 
	USES 
}
Module evdevLibPtr.o {
	OBJECT += libevdevLib.a::evdevLibPtr.o
}
ObjectFile libevdevLib.a::evdevLibTs.o {
	NAME Object file evdevLibTs.o from archive libevdevLib.a
	EXPORTS evdevTsReg evdevTsUnreg evdevTsSendMsg evdevTsSendMtMsg evdevTsEvalPoint evdevTsDevIsr evdevTsGetCalibQuick
	IMPORTS vxAtomicGet vxAtomicOr calloc bcopy msgQCreate wdCreate bzero snprintf taskSpawn evdevPtrReg wdDelete taskDelete msgQDelete evdevPtrUnreg vxAtomicAnd free wdCancel msgQSend __div32 wdStart excJobAdd strcmp memcpy printf msgQReceive evdevPtrSendMsg tickGet sysClkRateGet __udiv32 evdevSendMsg
	DECLARES 
	USES 
}
Module evdevLibTs.o {
	OBJECT += libevdevLib.a::evdevLibTs.o
}
ObjectFile libevdevLib.a::evdevLibKbdMap.o {
	NAME Object file evdevLibKbdMap.o from archive libevdevLib.a
	EXPORTS evdevKbdMapInit evdevKbdMapAdd evdevKbdReport2Unicode evdevKbdModifier2Unicode evdevKbdReport2Keycode evdevKbdModifier2Keycode
	IMPORTS calloc lstInit lstAdd lstFirst lstNext
	DECLARES 
	USES 
}
Module evdevLibKbdMap.o {
	OBJECT += libevdevLib.a::evdevLibKbdMap.o
}
ObjectFile libevdevLib.a::evdevLibKbd.o {
	NAME Object file evdevLibKbd.o from archive libevdevLib.a
	EXPORTS evdevKbdInit evdevKbdReg evdevKbdUnreg evdevKbdSendMsg
	IMPORTS calloc sysClkRateGet __udiv32 evdevKbdMapInit free evdevKbdGetConsole evdevKbdIsRedirectTty wdCreate evdevRegDev wdDelete tyIRd evdevUnregDev wdCancel tickGet evdevSendMsg bzero evdevKbdReport2Unicode evdevKbdModifier2Unicode evdevKbdReport2Keycode evdevKbdModifier2Keycode wdStart memcpy excJobAdd
	DECLARES 
	USES 
}
Module evdevLibKbd.o {
	OBJECT += libevdevLib.a::evdevLibKbd.o
}
ObjectFile libevdevLib.a::evdevLibKbdMapUS.o {
	NAME Object file evdevLibKbdMapUS.o from archive libevdevLib.a
	EXPORTS evdevKbdMapUSInit
	IMPORTS evdevKbdMapAdd
	DECLARES 
	USES 
}
Module evdevLibKbdMapUS.o {
	OBJECT += libevdevLib.a::evdevLibKbdMapUS.o
}
ObjectFile libevdevLib.a::evdevLibCore.o {
	NAME Object file evdevLibCore.o from archive libevdevLib.a
	EXPORTS evdevInit evdevRegDev evdevUnregDev evdevSendMsg
	IMPORTS calloc dllCreate semBCreate iosDrvInstall iosDevAdd selWakeupListInit iosDevDelete semDelete iosDrvRemove dllDelete free __udiv32 msgQSend selWakeupAll sysClkRateGet semTake bzero snprintf dllAdd tickGet taskSpawn semGive msgQNumMsgs msgQReceive msgQDelete selWakeupListTerm dllRemove strlen msgQCreate __errno selNodeAdd selWakeupType selWakeup selNodeDelete memcpy __div32 taskDelay
	DECLARES 
	USES 
}
Module evdevLibCore.o {
	OBJECT += libevdevLib.a::evdevLibCore.o
}
Library libevdevLib.a {
	MODULES libevdevLib.a::evdevLibPtr.o libevdevLib.a::evdevLibTs.o libevdevLib.a::evdevLibKbdMap.o libevdevLib.a::evdevLibKbd.o libevdevLib.a::evdevLibKbdMapUS.o libevdevLib.a::evdevLibCore.o
}
