Symbol edrLogBaseGet {
}
Symbol edrPmRegion {
}
Symbol edrPmArena {
}
Symbol pmRegionAddr {
}
Symbol errnoSet {
}
Symbol edrInitStatus {
}
Symbol edrLogSizeGet {
}
Symbol pmRegionSize {
}
Symbol edrStatusMsg {
}
Symbol edrLibInit {
}
Symbol edrErrLogCreate {
}
Symbol edrErrLogAttach {
}
Symbol edrErrLogHeaderUnprotect {
}
Symbol edrMissedErrors {
}
Symbol vxAtomic32Get {
}
Symbol edrErrLogHeaderProtect {
}
Symbol kernelIdGet {
}
Symbol printToEDRbuf {
}
Symbol oprintf {
}
Symbol taskIdCurrent {
}
Symbol vxIntStackBase {
}
Symbol vxIntStackEnd {
}
Symbol trcLibFuncs {
}
Symbol trcStackCtxTrace {
}
Symbol _dbgDsmInstRtn {
}
Symbol __div32 {
}
Symbol vxMemProbe {
}
Symbol _func_kernelModuleListIdGet {
}
Symbol wrs_kernel_text_end {
}
Symbol wrs_kernel_text_start {
}
Symbol intContext {
}
Symbol kernelState {
}
Symbol semTake {
}
Symbol dllEach {
}
Symbol semGive {
}
Symbol _func_excInfoShow {
}
Symbol __urem32 {
}
Symbol taskCpuUnlock {
}
Symbol evtAction {
}
Symbol eventPointSwHandle {
}
Symbol taskRegsShowOutput {
}
Symbol edrErrorInject {
}
Symbol vxAtomic32Inc {
}
Symbol edrErrorInjectPrePostHookTable {
}
Symbol edrErrLogNodeAlloc {
}
Symbol edrErrLogNodeCommit {
}
Symbol edrErrLogNodeUnprotect {
}
Symbol memset {
}
Symbol vxWorksVersionMajor {
}
Symbol vxWorksVersionMinor {
}
Symbol vxWorksVersionMaint {
}
Symbol sysProcNumGet {
}
Symbol vxAbsTicks {
}
Symbol time {
}
Symbol sysSymTbl {
}
Symbol ctime {
}
Symbol snprintf {
}
Symbol strlen {
}
Symbol strlcpy {
}
Symbol taskName {
}
Symbol edrErrorInjectTextHookTable {
}
Symbol edrErrorInjectDsmHook {
}
Symbol vxAtomic32Dec {
}
Symbol edrErrLogNodeProtect {
}
Symbol edrErrorInjectHookTable {
}
Symbol edrErrorLogClear {
}
Symbol edrErrLogClear {
}
Symbol edrErrorRecordCount {
}
Symbol edrErrLogNodeCount {
}
Symbol edrBootCountGet {
}
Symbol edrErrorInjectHookAdd {
}
Symbol hookAddToTail {
}
Symbol errnoGet {
}
Symbol edrErrorInjectHookDelete {
}
Symbol hookDelete {
}
Symbol edrErrorInjectPrePostHookAdd {
}
Symbol edrErrorInjectPrePostHookDelete {
}
Symbol edrErrorInjectTextHookAdd {
}
Symbol edrErrorInjectTextHookDelete {
}
Symbol _func_symFindSymbol {
}
Symbol edrUsesDemangler {
}
Symbol cplusDemangle {
}
Symbol _func_moduleSegInfoGet {
}
Symbol _func_dsmInst {
}
Symbol memcpy {
}
Symbol edrFlagsGet {
}
Symbol sysDebugModeGet {
}
Symbol edrIsDebugMode {
}
Symbol vmLibInfo {
}
Symbol checksum {
}
Symbol strncmp {
}
Symbol __udiv32 {
}
Symbol pmLibInit {
}
Symbol pmInit {
}
Symbol pmDefaultArena {
}
Symbol sysStartType {
}
Symbol pmFreeSpace {
}
Symbol pmRegionOpen {
}
Symbol pmRegionProtect {
}
Symbol pmRegionCreate {
}
Symbol pmRegionClose {
}
Symbol pmValidate {
}
Symbol pmInvalidate {
}
Symbol pmShow {
}
Symbol printf {
}
Symbol sysBootLine {
}
Symbol bootStringToStruct {
}
Symbol sysDebugModeInit {
}
Symbol sysDebugModeSet {
}
Symbol sysDebugModeSetHookAdd {
}
Symbol sysDebugModeSetHookDelete {
}
Symbol edrSystemDebugModeInit {
}
Symbol edrSystemDebugModeGet {
}
Symbol edrSystemDebugModeSet {
}
Symbol edrErrLogValidate {
}
Symbol intLock {
}
Symbol intUnlock {
}
Symbol edrNoMmu {
}
Symbol malloc {
}
Symbol edrErrLogIterCreate {
}
Symbol edrErrLogIterNext {
}
Symbol __rem32 {
}
Symbol edrErrLogMaxNodeCount {
}
Symbol edrErrLogNodePosition {
}
Symbol edrShowInit {
}
Symbol edrErrorRecordDecode {
}
Symbol edrShow {
}
Symbol __errno {
}
Symbol puts {
}
Symbol edrDebug {
}
Symbol free {
}
Symbol edrFatalShow {
}
Symbol edrInfoShow {
}
Symbol edrIntShow {
}
Symbol edrInitShow {
}
Symbol edrRebootShow {
}
Symbol edrBootShow {
}
Symbol edrKernelShow {
}
Symbol edrUserShow {
}
Symbol edrRtpShow {
}
Symbol edrClear {
}
Symbol edrInjectHookShow {
}
Symbol hookShow {
}
Symbol edrInjectTextHookShow {
}
Symbol edrInjectPrePostHookShow {
}
Symbol edrHookShow {
}
Symbol edrHelp {
}
Symbol edrScInit {
}
Symbol _edrErrorInjectSc {
}
Symbol scMemValidateStr {
}
Symbol scMemValidate {
}
Symbol edrErrorInjectStub {
}
Symbol edrRtpFatalPolicyHandler {
}
Symbol edrFlagsGetSc {
}
ObjectFile libedr.a::edrLib.o {
	NAME Object file edrLib.o from archive libedr.a
	EXPORTS edrLogBaseGet edrInitStatus edrLogSizeGet edrStatusMsg edrLibInit edrMissedErrors edrErrorInject edrErrorInjectPrePostHookTable edrErrorInjectTextHookTable edrErrorInjectDsmHook edrErrorInjectHookTable edrErrorLogClear edrErrorRecordCount edrBootCountGet edrErrorInjectHookAdd edrErrorInjectHookDelete edrErrorInjectPrePostHookAdd edrErrorInjectPrePostHookDelete edrErrorInjectTextHookAdd edrErrorInjectTextHookDelete edrUsesDemangler
	IMPORTS edrPmRegion edrPmArena pmRegionAddr errnoSet pmRegionSize edrErrLogCreate edrErrLogAttach edrErrLogHeaderUnprotect vxAtomic32Get edrErrLogHeaderProtect kernelIdGet printToEDRbuf oprintf taskIdCurrent vxIntStackBase vxIntStackEnd trcLibFuncs trcStackCtxTrace _dbgDsmInstRtn __div32 vxMemProbe _func_kernelModuleListIdGet wrs_kernel_text_end wrs_kernel_text_start intContext kernelState semTake dllEach semGive _func_excInfoShow __urem32 taskCpuUnlock evtAction eventPointSwHandle taskRegsShowOutput vxAtomic32Inc edrErrLogNodeAlloc edrErrLogNodeCommit edrErrLogNodeUnprotect memset vxWorksVersionMajor vxWorksVersionMinor vxWorksVersionMaint sysProcNumGet vxAbsTicks time sysSymTbl ctime snprintf strlen strlcpy taskName vxAtomic32Dec edrErrLogNodeProtect edrErrLogClear edrErrLogNodeCount hookAddToTail errnoGet hookDelete _func_symFindSymbol cplusDemangle _func_moduleSegInfoGet
	DECLARES 
	USES 
}
Module edrLib.o {
	OBJECT += libedr.a::edrLib.o
}
ObjectFile libedr.a::edrFuncBind.o {
	NAME Object file edrFuncBind.o from archive libedr.a
	EXPORTS 
	IMPORTS 
	DECLARES _dbgDsmInstRtn _func_dsmInst
	USES 
}
Module edrFuncBind.o {
	OBJECT += libedr.a::edrFuncBind.o
}
ObjectFile libedr.a::edrPrintfLib.o {
	NAME Object file edrPrintfLib.o from archive libedr.a
	EXPORTS printToEDRbuf
	IMPORTS memcpy
	DECLARES 
	USES 
}
Module edrPrintfLib.o {
	OBJECT += libedr.a::edrPrintfLib.o
}
ObjectFile libedr.a::edrSysDbgLib.o {
	NAME Object file edrSysDbgLib.o from archive libedr.a
	EXPORTS edrFlagsGet edrIsDebugMode
	IMPORTS sysDebugModeGet
	DECLARES 
	USES 
}
Module edrSysDbgLib.o {
	OBJECT += libedr.a::edrSysDbgLib.o
}
ObjectFile libedr.a::pmLib.o {
	NAME Object file pmLib.o from archive libedr.a
	EXPORTS pmLibInit pmInit pmFreeSpace pmRegionOpen pmRegionAddr pmRegionSize pmRegionProtect pmRegionCreate pmRegionClose pmValidate pmInvalidate pmShow
	IMPORTS vmLibInfo checksum strncmp __udiv32 pmDefaultArena sysStartType memset strlcpy printf
	DECLARES 
	USES 
}
Module pmLib.o {
	OBJECT += libedr.a::pmLib.o
}
ObjectFile libedr.a::sysDbgLib.o {
	NAME Object file sysDbgLib.o from archive libedr.a
	EXPORTS sysDebugModeInit sysDebugModeGet sysDebugModeSet sysDebugModeSetHookAdd sysDebugModeSetHookDelete edrSystemDebugModeInit edrSystemDebugModeGet edrSystemDebugModeSet
	IMPORTS sysBootLine bootStringToStruct
	DECLARES 
	USES 
}
Module sysDbgLib.o {
	OBJECT += libedr.a::sysDbgLib.o
}
ObjectFile libedr.a::edrErrLogLib.o {
	NAME Object file edrErrLogLib.o from archive libedr.a
	EXPORTS edrErrLogValidate edrErrLogHeaderProtect edrErrLogHeaderUnprotect edrErrLogNodeProtect edrErrLogNodeUnprotect edrNoMmu edrErrLogCreate edrErrLogIterCreate edrErrLogIterNext edrErrLogAttach edrErrLogClear edrErrLogNodeCommit edrErrLogNodeAlloc edrErrLogNodeCount edrErrLogMaxNodeCount edrErrLogNodePosition
	IMPORTS checksum vmLibInfo intLock intUnlock __udiv32 memset malloc vxWorksVersionMinor vxWorksVersionMajor vxWorksVersionMaint __div32 __rem32
	DECLARES 
	USES 
}
Module edrErrLogLib.o {
	OBJECT += libedr.a::edrErrLogLib.o
}
ObjectFile libedr.a::edrShow.o {
	NAME Object file edrShow.o from archive libedr.a
	EXPORTS edrShowInit edrErrorRecordDecode edrShow edrDebug edrFatalShow edrInfoShow edrIntShow edrInitShow edrRebootShow edrBootShow edrKernelShow edrUserShow edrRtpShow edrClear edrInjectHookShow edrInjectTextHookShow edrInjectPrePostHookShow edrHookShow edrHelp
	IMPORTS printToEDRbuf oprintf ctime snprintf strlen __errno edrInitStatus edrStatusMsg printf edrLogBaseGet edrErrLogNodeCount puts edrMissedErrors vmLibInfo edrErrLogIterCreate malloc edrErrLogIterNext edrErrLogNodePosition memcpy free edrErrorLogClear edrErrorInjectHookTable hookShow edrErrorInjectTextHookTable edrErrorInjectPrePostHookTable
	DECLARES 
	USES 
}
Module edrShow.o {
	OBJECT += libedr.a::edrShow.o
}
ObjectFile libedr.a::edrScLib.o {
	NAME Object file edrScLib.o from archive libedr.a
	EXPORTS edrScInit _edrErrorInjectSc edrFlagsGetSc
	IMPORTS scMemValidateStr scMemValidate edrErrorInjectStub memset edrRtpFatalPolicyHandler __errno edrFlagsGet
	DECLARES 
	USES 
}
Module edrScLib.o {
	OBJECT += libedr.a::edrScLib.o
}
Library libedr.a {
	MODULES libedr.a::edrLib.o libedr.a::edrFuncBind.o libedr.a::edrPrintfLib.o libedr.a::edrSysDbgLib.o libedr.a::pmLib.o libedr.a::sysDbgLib.o libedr.a::edrErrLogLib.o libedr.a::edrShow.o libedr.a::edrScLib.o
}
