Symbol vxdbgLibInit {
}
Symbol calloc {
}
Symbol taskIdCurrent {
}
Symbol semMCreate {
}
Symbol vxdbgClntRegister {
}
Symbol spinLockTaskInit {
}
Symbol strdup {
}
Symbol free {
}
Symbol spinLockTaskTake {
}
Symbol spinLockTaskGive {
}
Symbol vxdbgClntUnregister {
}
Symbol errnoSet {
}
Symbol vxdbgHookAdd {
}
Symbol __errno {
}
Symbol vxdbgHookInvoke {
}
Symbol intContext {
}
Symbol vxdbgIsAttached {
}
Symbol semTake {
}
Symbol semGive {
}
Symbol vxdbgInternalEventInject {
}
Symbol vxdbgEventInject {
}
Symbol vxdbgAttach {
}
Symbol realloc {
}
Symbol taskIsStopped {
}
Symbol vxdbgRunCtrlHookInvoke {
}
Symbol vxdbgStop {
}
Symbol vxdbgDetach {
}
Symbol vxdbgCont {
}
Symbol vxdbgHooksInit {
}
Symbol taskCreateHookAdd {
}
Symbol taskDeleteHookAdd {
}
Symbol taskStackAllot {
}
Symbol memset {
}
Symbol taskIdSelf {
}
Symbol vxdbgEventCancel {
}
Symbol vxdbgMemCtxSwitch {
}
Symbol _func_taskMemCtxSwitch {
}
Symbol _func_vxdbgRtpIdVerify {
}
Symbol kernelId {
}
Symbol kernelState {
}
Symbol vmLibInfo {
}
Symbol vxdbgMemAccess {
}
Symbol __udiv32 {
}
Symbol _func_vxdbgMemRead {
}
Symbol _func_vxdbgMemWrite {
}
Symbol vxdbgMemRead {
}
Symbol vxdbgMemWrite {
}
Symbol vxdbgMemMove {
}
Symbol _func_vxdbgMemMove {
}
Symbol memmove {
}
Symbol vxdbgMemScan {
}
Symbol _func_vxdbgMemScan {
}
Symbol memcmp {
}
Symbol vxdbgMemProbe {
}
Symbol vxMemProbe {
}
Symbol vxdbgTaskLibInit {
}
Symbol taskStopHookAdd {
}
Symbol taskContHookAdd {
}
Symbol vxdbgTaskHookInit {
}
Symbol vxdbgTaskCont {
}
Symbol taskClassId {
}
Symbol objVerify {
}
Symbol taskLock {
}
Symbol vxdbgBpFind {
}
Symbol taskUnlock {
}
Symbol taskCpuLock {
}
Symbol taskCont {
}
Symbol taskCpuUnlock {
}
Symbol vxdbgTaskStop {
}
Symbol taskStopForce {
}
Symbol taskStop {
}
Symbol vxdbgTaskStep {
}
Symbol wdbDbgSysCallCheck {
}
Symbol vxdbgBpAdd {
}
Symbol vxdbgTaskBpToInstall {
}
Symbol pDbgClnt {
}
Symbol _func_vxdbgBpDelete {
}
Symbol wdbDbgAddrCheck {
}
Symbol vxdbgBpAddrCheck {
}
Symbol vxdbgInstrCmp {
}
Symbol wdbDbgTrap {
}
Symbol _func_vxdbgTrap {
}
Symbol _func_wdbTrap {
}
Symbol vxdbgBpMsgQCreate {
}
Symbol malloc {
}
Symbol vxAtomicClear {
}
Symbol vxdbgBpMsgGet {
}
Symbol vxAtomicGet {
}
Symbol vxAtomicInc {
}
Symbol __urem32 {
}
Symbol vxdbgBpMsgQHdlrIsRunning {
}
Symbol vxdbgBpMsgPost {
}
Symbol excJobAdd {
}
Symbol vxdbgBpMsgDrop {
}
Symbol vxAtomicSet {
}
Symbol printf {
}
Symbol iosFdTableLock {
}
Symbol iosFdTableUnlock {
}
Symbol ioTaskStdGet {
}
Symbol fdprintf {
}
Symbol taskName {
}
Symbol vxAtomicDec {
}
Symbol vxdbgEventLibInit {
}
Symbol semBInit {
}
Symbol _func_logMsg {
}
Symbol taskSpawn {
}
Symbol vxdbgEventIdAlloc {
}
Symbol vxdbgEventIdFree {
}
Symbol bcopy {
}
Symbol vxAtomicCas {
}
Symbol vxdbgRunCtrlStateChangeNotify {
}
Symbol vxdbgEventRunCtrlStateChangeInject {
}
Symbol semGiveDefer {
}
Symbol _func_vxdbgCtxStop {
}
Symbol _func_vxdbgCtxCont {
}
Symbol vxdbgRunCtrlLibInit {
}
Symbol usrBreakpointInit {
}
Symbol semBCreate {
}
Symbol vxdbgBpList {
}
Symbol wdbDbgArchInit {
}
Symbol vxdbgBpDelete {
}
Symbol vxdbgUnbreakableOld {
}
Symbol usrBreakpointSet {
}
Symbol wdbDbgHwBpSet {
}
Symbol wdbDbgRegsClear {
}
Symbol wdbDbgRegsSet {
}
Symbol vxdbgBpAllDisable {
}
Symbol vxdbgEnable {
}
Symbol vxdbgDisableReason {
}
Symbol strncpy {
}
Symbol vxdbgIsEnabled {
}
Symbol taskSwitchHookAdd {
}
Symbol taskBpHookSet {
}
Symbol vxdbgStep {
}
Symbol _func_vxdbgRtpStop {
}
Symbol _func_vxdbgRtpCont {
}
Symbol vxdbgBpUserKeySet {
}
Symbol vxdbgBpUserKeyGet {
}
Symbol wdbDbgHwAddrCheck {
}
Symbol memcpy {
}
Symbol vxdbgHwBpAdd {
}
Symbol vxdbgHwBpTypeGet {
}
Symbol vxdbgHwBpTypeValidate {
}
Symbol taskIdVerify {
}
Symbol taskDelay {
}
Symbol taskCpuUnlockNoResched {
}
Symbol wdbDbgHwBpFind {
}
Symbol wdbDbgGetNpc {
}
Symbol wdbDbgTraceModeSet {
}
Symbol wdbDbgTraceModeClear {
}
Symbol vxdbgBpTraceAbort {
}
Symbol intCnt {
}
Symbol vxdbgLockUnbreakable {
}
Symbol vxdbgSafeUnbreakable {
}
Symbol taskRegsSet {
}
Symbol vxdbgIsBpAddr {
}
Symbol vxdbgExcLibInit {
}
Symbol _func_sigExcKill {
}
Symbol _func_rtpSigExcKill {
}
Symbol vxdbgRtpLibInit {
}
Symbol rtpTaskAllStop {
}
Symbol rtpDeleteHookAdd {
}
Symbol rtpVerifyAndLock {
}
Symbol rtpTaskAllCont {
}
Symbol taskOptionsSet {
}
Symbol rtpClassId {
}
Symbol vxdbgModuleLibInit {
}
Symbol moduleUnloadHookAdd {
}
Symbol moduleSegEach {
}
Symbol printErr {
}
ObjectFile libhwdb.a::vxdbgLib.o {
	NAME Object file vxdbgLib.o from archive libhwdb.a
	EXPORTS vxdbgLibInit vxdbgClntRegister vxdbgClntUnregister vxdbgHookAdd vxdbgHookInvoke vxdbgIsAttached vxdbgInternalEventInject vxdbgAttach vxdbgDetach vxdbgHooksInit vxdbgMemCtxSwitch _func_vxdbgRtpIdVerify
	IMPORTS calloc taskIdCurrent semMCreate spinLockTaskInit strdup free spinLockTaskTake spinLockTaskGive errnoSet __errno intContext semTake semGive vxdbgEventInject realloc taskIsStopped vxdbgRunCtrlHookInvoke vxdbgStop vxdbgCont taskCreateHookAdd taskDeleteHookAdd taskStackAllot memset taskIdSelf vxdbgEventCancel _func_taskMemCtxSwitch kernelId kernelState vmLibInfo
	DECLARES 
	USES 
}
Module vxdbgLib.o {
	OBJECT += libhwdb.a::vxdbgLib.o
}
ObjectFile libhwdb.a::vxdbgMemLib.o {
	NAME Object file vxdbgMemLib.o from archive libhwdb.a
	EXPORTS vxdbgMemAccess vxdbgMemRead vxdbgMemWrite vxdbgMemMove vxdbgMemScan vxdbgMemProbe
	IMPORTS __udiv32 errnoSet _func_vxdbgMemRead _func_vxdbgMemWrite _func_vxdbgMemMove memmove _func_vxdbgMemScan memcmp vmLibInfo vxMemProbe
	DECLARES 
	USES 
}
Module vxdbgMemLib.o {
	OBJECT += libhwdb.a::vxdbgMemLib.o
}
ObjectFile libhwdb.a::vxdbgTaskLib.o {
	NAME Object file vxdbgTaskLib.o from archive libhwdb.a
	EXPORTS vxdbgTaskLibInit vxdbgTaskHookInit vxdbgTaskCont vxdbgTaskStop vxdbgTaskStep vxdbgTaskBpToInstall
	IMPORTS taskStopHookAdd taskContHookAdd taskDeleteHookAdd taskClassId objVerify taskIsStopped __errno taskLock vxdbgBpFind taskUnlock taskCpuLock taskCont taskCpuUnlock taskStopForce taskStop _func_taskMemCtxSwitch wdbDbgSysCallCheck vxdbgBpAdd pDbgClnt _func_vxdbgBpDelete vxdbgEventCancel vxdbgRunCtrlHookInvoke vxdbgInternalEventInject
	DECLARES 
	USES 
}
Module vxdbgTaskLib.o {
	OBJECT += libhwdb.a::vxdbgTaskLib.o
}
ObjectFile libhwdb.a::vxdbgUtilLib.o {
	NAME Object file vxdbgUtilLib.o from archive libhwdb.a
	EXPORTS vxdbgBpAddrCheck vxdbgInstrCmp wdbDbgTrap _func_vxdbgTrap _func_wdbTrap
	IMPORTS wdbDbgAddrCheck intContext vxMemProbe _func_taskMemCtxSwitch __errno vmLibInfo vxdbgMemRead
	DECLARES 
	USES 
}
Module vxdbgUtilLib.o {
	OBJECT += libhwdb.a::vxdbgUtilLib.o
}
ObjectFile libhwdb.a::vxdbgBpMsgQLib.o {
	NAME Object file vxdbgBpMsgQLib.o from archive libhwdb.a
	EXPORTS vxdbgBpMsgQCreate vxdbgBpMsgGet vxdbgBpMsgQHdlrIsRunning vxdbgBpMsgPost vxdbgBpMsgDrop
	IMPORTS malloc strdup vxAtomicClear free vxAtomicGet vxAtomicInc __urem32 taskIdSelf excJobAdd vxAtomicSet printf taskIdCurrent kernelId iosFdTableLock iosFdTableUnlock ioTaskStdGet fdprintf taskName vxAtomicDec
	DECLARES 
	USES 
}
Module vxdbgBpMsgQLib.o {
	OBJECT += libhwdb.a::vxdbgBpMsgQLib.o
}
ObjectFile libhwdb.a::vxdbgEventLib.o {
	NAME Object file vxdbgEventLib.o from archive libhwdb.a
	EXPORTS vxdbgEventLibInit vxdbgEventIdAlloc vxdbgEventIdFree vxdbgEventInject vxdbgEventRunCtrlStateChangeInject vxdbgEventCancel
	IMPORTS semBInit _func_logMsg taskSpawn calloc free bcopy semTake vxAtomicCas vxdbgRunCtrlStateChangeNotify errnoSet semGive vxAtomicSet semGiveDefer
	DECLARES 
	USES 
}
Module vxdbgEventLib.o {
	OBJECT += libhwdb.a::vxdbgEventLib.o
}
ObjectFile libhwdb.a::vxdbgFuncBind.o {
	NAME Object file vxdbgFuncBind.o from archive libhwdb.a
	EXPORTS _func_vxdbgCtxStop _func_vxdbgCtxCont _func_vxdbgBpDelete _func_vxdbgMemRead _func_vxdbgMemWrite _func_vxdbgMemMove _func_vxdbgMemScan
	IMPORTS 
	DECLARES 
	USES 
}
Module vxdbgFuncBind.o {
	OBJECT += libhwdb.a::vxdbgFuncBind.o
}
ObjectFile libhwdb.a::vxdbgRunCtrlLib.o {
	NAME Object file vxdbgRunCtrlLib.o from archive libhwdb.a
	EXPORTS vxdbgRunCtrlLibInit vxdbgStop vxdbgCont vxdbgBpDelete vxdbgRunCtrlStateChangeNotify vxdbgBpAllDisable vxdbgEnable vxdbgIsEnabled vxdbgStep _func_vxdbgRtpStop _func_vxdbgRtpCont vxdbgBpUserKeySet vxdbgBpUserKeyGet vxdbgBpAdd vxdbgRunCtrlHookInvoke vxdbgHwBpAdd vxdbgHwBpTypeValidate vxdbgBpFind vxdbgBpTraceAbort vxdbgIsBpAddr
	IMPORTS usrBreakpointInit semBCreate vxdbgTaskLibInit wdbDbgArchInit _func_vxdbgTrap _func_vxdbgCtxStop _func_vxdbgCtxCont _func_vxdbgBpDelete calloc vxdbgHookInvoke taskIdCurrent kernelState _func_vxdbgRtpIdVerify memset vxdbgTaskBpToInstall vxdbgMemCtxSwitch usrBreakpointSet wdbDbgHwBpSet wdbDbgRegsClear wdbDbgRegsSet vxdbgEventRunCtrlStateChangeInject strncpy taskLock taskUnlock semTake taskSwitchHookAdd taskBpHookSet vxdbgTaskHookInit _func_vxdbgMemRead _func_vxdbgMemWrite _func_vxdbgMemMove _func_vxdbgMemScan semGive vxdbgTaskStep __errno vxdbgTaskStop vxdbgTaskCont intContext taskClassId objVerify _func_taskMemCtxSwitch vxMemProbe wdbDbgHwAddrCheck vxdbgBpAddrCheck vxAtomicInc memcpy vxdbgInternalEventInject vxdbgHwBpTypeGet vxAtomicDec memmove malloc free memcmp vxAtomicGet realloc taskIdVerify taskIsStopped taskDelay vxdbgEventCancel kernelId taskCpuUnlockNoResched taskStop taskCpuUnlock wdbDbgHwBpFind wdbDbgGetNpc wdbDbgTraceModeSet wdbDbgTraceModeClear taskStopForce intCnt taskRegsSet
	DECLARES vxdbgBpList vxdbgUnbreakableOld vxdbgDisableReason vxdbgLockUnbreakable vxdbgSafeUnbreakable
	USES 
}
Module vxdbgRunCtrlLib.o {
	OBJECT += libhwdb.a::vxdbgRunCtrlLib.o
}
ObjectFile libhwdb.a::vxdbgExcLib.o {
	NAME Object file vxdbgExcLib.o from archive libhwdb.a
	EXPORTS vxdbgExcLibInit
	IMPORTS _func_sigExcKill _func_rtpSigExcKill taskIdSelf vxdbgBpTraceAbort vxdbgRunCtrlHookInvoke vxdbgInternalEventInject taskStop
	DECLARES 
	USES 
}
Module vxdbgExcLib.o {
	OBJECT += libhwdb.a::vxdbgExcLib.o
}
ObjectFile libhwdb.a::vxdbgRtpLib.o {
	NAME Object file vxdbgRtpLib.o from archive libhwdb.a
	EXPORTS vxdbgRtpLibInit
	IMPORTS _func_vxdbgRtpCont rtpTaskAllStop _func_vxdbgRtpStop _func_vxdbgRtpIdVerify rtpDeleteHookAdd rtpVerifyAndLock __errno taskLock taskClassId objVerify taskIsStopped vxdbgBpFind taskUnlock semGive rtpTaskAllCont taskIdCurrent taskOptionsSet vxdbgBpAllDisable _func_vxdbgBpDelete rtpClassId
	DECLARES 
	USES 
}
Module vxdbgRtpLib.o {
	OBJECT += libhwdb.a::vxdbgRtpLib.o
}
ObjectFile libhwdb.a::vxdbgModuleLib.o {
	NAME Object file vxdbgModuleLib.o from archive libhwdb.a
	EXPORTS vxdbgModuleLibInit
	IMPORTS moduleUnloadHookAdd vxdbgBpDelete moduleSegEach printErr
	DECLARES 
	USES 
}
Module vxdbgModuleLib.o {
	OBJECT += libhwdb.a::vxdbgModuleLib.o
}
Library libhwdb.a {
	MODULES libhwdb.a::vxdbgLib.o libhwdb.a::vxdbgMemLib.o libhwdb.a::vxdbgTaskLib.o libhwdb.a::vxdbgUtilLib.o libhwdb.a::vxdbgBpMsgQLib.o libhwdb.a::vxdbgEventLib.o libhwdb.a::vxdbgFuncBind.o libhwdb.a::vxdbgRunCtrlLib.o libhwdb.a::vxdbgExcLib.o libhwdb.a::vxdbgRtpLib.o libhwdb.a::vxdbgModuleLib.o
}
