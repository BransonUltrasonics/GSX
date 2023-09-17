Symbol spinLockIsrInit {
}
Symbol spinLockIsrTake {
}
Symbol spinLockIsrGive {
}
Symbol spinLockTaskInit {
}
Symbol spinLockTaskTake {
}
Symbol taskIdCurrent {
}
Symbol evtAction {
}
Symbol eventPointSwHandle {
}
Symbol spinLockTaskGive {
}
Symbol taskCpuUnlock {
}
Symbol spinLockRestrict {
}
Symbol spinLockIsrNdInit {
}
Symbol spinLockIsrNdTake {
}
Symbol spinLockIsrNdGive {
}
Symbol spinlockIsrNdInit {
}
Symbol spinlockIsrNdTake {
}
Symbol spinlockIsrNdGive {
}
Symbol spinLockNdTimedInit {
}
Symbol spinLockNdTimedTake {
}
Symbol sysClkRateGet {
}
Symbol __udiv32 {
}
Symbol __urem64 {
}
Symbol __udiv64 {
}
Symbol vxAbsTicks {
}
Symbol atomic32Get {
}
Symbol atomic32Cas {
}
Symbol errnoSet {
}
Symbol spinLockNdTimedGive {
}
Symbol spinLockNdTimedPrevOwnerGet {
}
ObjectFile libspl.a::spinLockUpLib.o {
	NAME Object file spinLockUpLib.o from archive libspl.a
	EXPORTS spinLockIsrInit spinLockIsrTake spinLockIsrGive spinLockTaskInit spinLockTaskTake spinLockTaskGive spinLockRestrict
	IMPORTS taskIdCurrent evtAction eventPointSwHandle taskCpuUnlock
	DECLARES 
	USES 
}
Module spinLockUpLib.o {
	OBJECT += libspl.a::spinLockUpLib.o
}
ObjectFile libspl.a::spinLockIsrNdLib.o {
	NAME Object file spinLockIsrNdLib.o from archive libspl.a
	EXPORTS spinLockIsrNdInit spinLockIsrNdTake spinLockIsrNdGive spinlockIsrNdInit spinlockIsrNdTake spinlockIsrNdGive
	IMPORTS 
	DECLARES 
	USES 
}
Module spinLockIsrNdLib.o {
	OBJECT += libspl.a::spinLockIsrNdLib.o
}
ObjectFile libspl.a::spinLockNdTimedLib.o {
	NAME Object file spinLockNdTimedLib.o from archive libspl.a
	EXPORTS spinLockNdTimedInit spinLockNdTimedTake spinLockNdTimedGive spinLockNdTimedPrevOwnerGet
	IMPORTS sysClkRateGet __udiv32 __urem64 __udiv64 vxAbsTicks atomic32Get atomic32Cas errnoSet
	DECLARES 
	USES 
}
Module spinLockNdTimedLib.o {
	OBJECT += libspl.a::spinLockNdTimedLib.o
}
Library libspl.a {
	MODULES libspl.a::spinLockUpLib.o libspl.a::spinLockIsrNdLib.o libspl.a::spinLockNdTimedLib.o
}
