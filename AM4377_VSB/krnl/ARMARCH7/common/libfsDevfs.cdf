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
Symbol rawFsInit {
}
Symbol rawFsDrvNum {
}
Symbol iosDrvInstall {
}
Symbol rawFsFdListMutexOptions {
}
Symbol semMCreate {
}
Symbol malloc {
}
Symbol bzero {
}
Symbol rawFsFdMutexOptions {
}
Symbol free {
}
Symbol rawFsDevInit {
}
Symbol _func_rawFsDevInitRtn {
}
Symbol xbdBlockSize {
}
Symbol xbdNBlocks {
}
Symbol rawFsVolMutexOptions {
}
Symbol semBCreate {
}
Symbol rawFsBioBufferSize {
}
Symbol bio_alloc {
}
Symbol iosLock {
}
Symbol iosDevReplaceExt {
}
Symbol iosUnlock {
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
Symbol iosDevDelete {
}
Symbol iosDevDelCallback {
}
Symbol fsPathAddedEventRaise {
}
Symbol bio_free {
}
Symbol semDelete {
}
Symbol lstFirst {
}
Symbol lstNext {
}
Symbol iosDevSuspend {
}
Symbol lstGet {
}
Symbol rawFsVolUnmount {
}
Symbol __errno {
}
Symbol __div64 {
}
Symbol xbdSize {
}
Symbol __rem64 {
}
Symbol xbdStrategy {
}
Symbol bcopy {
}
ObjectFile libfsDevfs.a::rawFsLib.o {
	NAME Object file rawFsLib.o from archive libfsDevfs.a
	EXPORTS rawFsInit rawFsDrvNum rawFsFdListMutexOptions rawFsFdMutexOptions rawFsDevInit rawFsVolMutexOptions rawFsBioBufferSize rawFsVolUnmount
	IMPORTS xbdIoctl errnoSet semTake lstDelete lstAdd semGive iosDrvInstall semMCreate malloc bzero free _func_rawFsDevInitRtn xbdBlockSize xbdNBlocks semBCreate bio_alloc iosLock iosDevReplaceExt iosUnlock xbdEventCategory xbdEventRemove erfHandlerRegister xbdEventMediaChanged erfHandlerUnregister iosDevDelete iosDevDelCallback fsPathAddedEventRaise bio_free semDelete lstFirst lstNext iosDevSuspend lstGet __errno __div64 xbdSize __rem64 xbdStrategy bcopy
	DECLARES 
	USES 
}
Module rawFsLib.o {
	OBJECT += libfsDevfs.a::rawFsLib.o
}
Library libfsDevfs.a {
	MODULES libfsDevfs.a::rawFsLib.o
}
