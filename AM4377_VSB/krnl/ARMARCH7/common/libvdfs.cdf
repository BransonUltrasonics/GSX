Symbol xbdIoctl {
}
Symbol errnoSet {
}
Symbol semTake {
}
Symbol lstDelete {
}
Symbol lstAdd {
}
Symbol semGive {
}
Symbol semDelete {
}
Symbol bio_free {
}
Symbol free {
}
Symbol bzero {
}
Symbol selWakeupListLen {
}
Symbol selWakeupListTerm {
}
Symbol vdFsDevInit {
}
Symbol vdFsDrvNum {
}
Symbol malloc {
}
Symbol xbdBlockSize {
}
Symbol xbdNBlocks {
}
Symbol semMCreate {
}
Symbol iosLock {
}
Symbol iosDevReplaceExt {
}
Symbol iosDevDelCallback {
}
Symbol xbdEventCategory {
}
Symbol xbdEventRemove {
}
Symbol erfHandlerRegister {
}
Symbol xbdEventMediaChanged {
}
Symbol erfHandlerUnregister {
}
Symbol iosUnlock {
}
Symbol fsPathAddedEventRaise {
}
Symbol lstFirst {
}
Symbol lstNext {
}
Symbol iosDevDelete {
}
Symbol iosDevSuspend {
}
Symbol lstGet {
}
Symbol printErr {
}
Symbol semCCreate {
}
Symbol semBCreate {
}
Symbol bio_alloc {
}
Symbol calloc {
}
Symbol vdFsAioXferQSizeSet {
}
Symbol ioctl {
}
Symbol vdFsAioXferQSelect {
}
Symbol select {
}
Symbol vdFsAioXferQEntryTake {
}
Symbol bcopy {
}
Symbol vdFsAioXferQSizeGet {
}
Symbol vdFsAioXferQNonEmpty {
}
Symbol vdFsMonitorDevCreate {
}
Symbol vdFsPartMount {
}
Symbol strncpy {
}
Symbol open {
}
Symbol fsPathAddedEventSetup {
}
Symbol close {
}
Symbol fsWaitForPath {
}
Symbol vdFsDiskProbe {
}
Symbol devName {
}
Symbol strcmp {
}
Symbol xbdStrategy {
}
Symbol vdFsInit {
}
Symbol iosDrvInstall {
}
Symbol fsmProbeInstall {
}
Symbol _func_rawFsDevInitRtn {
}
Symbol iosDrvRemove {
}
Symbol vdFsVolUnmount {
}
Symbol __errno {
}
Symbol __div64 {
}
Symbol selNodeAdd {
}
Symbol selWakeup {
}
Symbol selNodeDelete {
}
Symbol xbdSize {
}
Symbol selWakeupListInit {
}
Symbol __rem64 {
}
Symbol selWakeupAll {
}
Symbol taskPriorityGet {
}
ObjectFile libvdfs.a::vdFsLib.o {
	NAME Object file vdFsLib.o from archive libvdfs.a
	EXPORTS vdFsDevInit vdFsDrvNum vdFsAioXferQSizeSet vdFsAioXferQSelect vdFsAioXferQEntryTake vdFsAioXferQSizeGet vdFsAioXferQNonEmpty vdFsMonitorDevCreate vdFsPartMount vdFsDiskProbe vdFsInit vdFsVolUnmount
	IMPORTS xbdIoctl errnoSet semTake lstDelete lstAdd semGive semDelete bio_free free bzero selWakeupListLen selWakeupListTerm malloc xbdBlockSize xbdNBlocks semMCreate iosLock iosDevReplaceExt iosDevDelCallback xbdEventCategory xbdEventRemove erfHandlerRegister xbdEventMediaChanged erfHandlerUnregister iosUnlock fsPathAddedEventRaise lstFirst lstNext iosDevDelete iosDevSuspend lstGet printErr semCCreate semBCreate bio_alloc calloc ioctl select bcopy strncpy open fsPathAddedEventSetup close fsWaitForPath devName strcmp xbdStrategy iosDrvInstall fsmProbeInstall _func_rawFsDevInitRtn iosDrvRemove __errno __div64 selNodeAdd selWakeup selNodeDelete xbdSize selWakeupListInit __rem64 selWakeupAll taskPriorityGet
	DECLARES 
	USES 
}
Module vdFsLib.o {
	OBJECT += libvdfs.a::vdFsLib.o
}
Library libvdfs.a {
	MODULES libvdfs.a::vdFsLib.o
}
