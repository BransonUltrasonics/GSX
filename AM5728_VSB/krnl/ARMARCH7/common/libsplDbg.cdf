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
Symbol intContext {
}
Symbol setjmp {
}
Symbol edrErrorInjectStub {
}
Symbol edrInterruptFatalPolicyHandler {
}
Symbol vxCpuConfigured {
}
Symbol spinLockIsrTake {
}
Symbol vxAtomic32Get {
}
Symbol vxAtomic32Cas {
}
Symbol rtSpinlockMaxSpin {
}
Symbol vxAtomic32Add {
}
Symbol vxAtomic32Sub {
}
Symbol intCpuEnable {
}
Symbol edrSystemFatalPolicyHandler {
}
Symbol reboot {
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
Symbol spinLockIsrHeld {
}
Symbol spinLockRestrict {
}
Symbol spinLockDbgDisable {
}
ObjectFile libsplDbg.a::kernelLockLib.o {
	NAME Object file kernelLockLib.o from archive libsplDbg.a
	EXPORTS kernelLockInit kernelLockTake kernelLockGive kernelLockTry kernelLockOwnedByMe kernelLockOwnerGet
	IMPORTS vxKernelVars vxCpuIndexGet vxAtomicAdd vxAtomicGet vxAtomicSet vxAtomicCas
	DECLARES kernelStateLock
	USES 
}
Module kernelLockLib.o {
	OBJECT += libsplDbg.a::kernelLockLib.o
}
ObjectFile libsplDbg.a::spinLockIsrNdLib.o {
	NAME Object file spinLockIsrNdLib.o from archive libsplDbg.a
	EXPORTS spinLockIsrNdInit spinLockIsrNdTake spinLockIsrNdGive spinlockIsrNdInit spinlockIsrNdTake spinlockIsrNdGive
	IMPORTS vxAtomic32Set vxAtomic32Clear
	DECLARES 
	USES 
}
Module spinLockIsrNdLib.o {
	OBJECT += libsplDbg.a::spinLockIsrNdLib.o
}
ObjectFile libsplDbg.a::spinLockSmpDbgLib.o {
	NAME Object file spinLockSmpDbgLib.o from archive libsplDbg.a
	EXPORTS spinLockIsrInit spinLockIsrTake rtSpinlockMaxSpin spinLockIsrGive spinLockTaskInit spinLockTaskTake spinLockTaskGive spinLockIsrHeld spinLockRestrict spinLockDbgDisable
	IMPORTS intContext setjmp edrErrorInjectStub edrInterruptFatalPolicyHandler vxCpuConfigured vxCpuIndexGet vxAtomic32Get vxKernelVars vxAtomic32Cas vxAtomic32Set vxAtomic32Add vxAtomic32Sub intCpuEnable edrSystemFatalPolicyHandler reboot kernelLockOwnerGet taskCpuUnlockInternal evtAction vxAtomicInc vxAtomicGet eventPointSwHandle vxAtomicSet
	DECLARES 
	USES 
}
Module spinLockSmpDbgLib.o {
	OBJECT += libsplDbg.a::spinLockSmpDbgLib.o
}
Library libsplDbg.a {
	MODULES libsplDbg.a::kernelLockLib.o libsplDbg.a::spinLockIsrNdLib.o libsplDbg.a::spinLockSmpDbgLib.o
}
