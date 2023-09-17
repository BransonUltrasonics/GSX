Symbol kernelLockInit {
}
Symbol kernelStateLock {
}
Symbol kernelLockTake {
}
Symbol vxKernelVars {
}
Symbol vxCpuIndexGet {
}
Symbol vxAtomicAdd {
}
Symbol vxAtomicGet {
}
Symbol kernelLockGive {
}
Symbol vxAtomicSet {
}
Symbol kernelLockTry {
}
Symbol vxAtomicCas {
}
Symbol kernelLockOwnedByMe {
}
Symbol kernelLockOwnerGet {
}
Symbol spinLockIsrNdInit {
}
Symbol spinLockIsrNdTake {
}
Symbol vxAtomic32Set {
}
Symbol spinLockIsrNdGive {
}
Symbol vxAtomic32Clear {
}
Symbol spinlockIsrNdInit {
}
Symbol spinlockIsrNdTake {
}
Symbol spinlockIsrNdGive {
}
Symbol spinLockIsrInit {
}
Symbol vxCpuConfigured {
}
Symbol spinLockIsrTake {
}
Symbol vxAtomic32Cas {
}
Symbol rtSpinlockMaxSpin {
}
Symbol vxAtomic32Get {
}
Symbol vxAtomic32Add {
}
Symbol vxAtomic32Sub {
}
Symbol spinLockIsrHeld {
}
Symbol spinLockRestrict {
}
Symbol spinLockIsrGive {
}
Symbol taskCpuUnlockInternal {
}
Symbol spinLockTaskInit {
}
Symbol spinLockTaskTake {
}
Symbol evtAction {
}
Symbol vxAtomicInc {
}
Symbol eventPointSwHandle {
}
Symbol spinLockTaskGive {
}
Symbol spinLockDbgDisable {
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
Symbol vxAbsTicksGet {
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
ObjectFile libspl.a::kernelLockLib.o {
	NAME Object file kernelLockLib.o from archive libspl.a
	EXPORTS kernelLockInit kernelLockTake kernelLockGive kernelLockTry kernelLockOwnedByMe kernelLockOwnerGet
	IMPORTS vxKernelVars vxCpuIndexGet vxAtomicAdd vxAtomicGet vxAtomicSet vxAtomicCas
	DECLARES kernelStateLock
	USES 
}
Module kernelLockLib.o {
	OBJECT += libspl.a::kernelLockLib.o
}
ObjectFile libspl.a::spinLockIsrNdLib.o {
	NAME Object file spinLockIsrNdLib.o from archive libspl.a
	EXPORTS spinLockIsrNdInit spinLockIsrNdTake spinLockIsrNdGive spinlockIsrNdInit spinlockIsrNdTake spinlockIsrNdGive
	IMPORTS vxAtomic32Set vxAtomic32Clear
	DECLARES 
	USES 
}
Module spinLockIsrNdLib.o {
	OBJECT += libspl.a::spinLockIsrNdLib.o
}
ObjectFile libspl.a::spinLockSmpLib.o {
	NAME Object file spinLockSmpLib.o from archive libspl.a
	EXPORTS spinLockIsrInit spinLockIsrTake rtSpinlockMaxSpin spinLockIsrHeld spinLockRestrict spinLockIsrGive spinLockTaskInit spinLockTaskTake spinLockTaskGive spinLockDbgDisable
	IMPORTS vxCpuConfigured vxCpuIndexGet vxKernelVars vxAtomic32Cas vxAtomic32Set vxAtomic32Get vxAtomic32Add vxAtomic32Sub kernelLockOwnerGet taskCpuUnlockInternal evtAction vxAtomicInc vxAtomicGet eventPointSwHandle vxAtomicSet
	DECLARES 
	USES 
}
Module spinLockSmpLib.o {
	OBJECT += libspl.a::spinLockSmpLib.o
}
ObjectFile libspl.a::spinLockNdTimedLib.o {
	NAME Object file spinLockNdTimedLib.o from archive libspl.a
	EXPORTS spinLockNdTimedInit spinLockNdTimedTake spinLockNdTimedGive spinLockNdTimedPrevOwnerGet
	IMPORTS sysClkRateGet __udiv32 __urem64 __udiv64 vxAbsTicksGet atomic32Get atomic32Cas errnoSet
	DECLARES 
	USES 
}
Module spinLockNdTimedLib.o {
	OBJECT += libspl.a::spinLockNdTimedLib.o
}
Library libspl.a {
	MODULES libspl.a::kernelLockLib.o libspl.a::spinLockIsrNdLib.o libspl.a::spinLockSmpLib.o libspl.a::spinLockNdTimedLib.o
}
