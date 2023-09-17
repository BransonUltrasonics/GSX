Symbol vxbDevMatch {
}
Symbol vxbDevSoftcGet {
}
Symbol vxbDevIvarsGet {
}
Symbol __mul64 {
}
Symbol calloc {
}
Symbol bzero {
}
Symbol bcopy {
}
Symbol sdCmdAllSendCid {
}
Symbol sdMmcCmdSetRelativeAddr {
}
Symbol sdCmdSendCsd {
}
Symbol sdCmdSelectCard {
}
Symbol mmcCmdReadExtCsd {
}
Symbol sdCmdSetBlockLen {
}
Symbol mmcHrfsInclude {
}
Symbol mmcSwitch {
}
Symbol __udiv64 {
}
Symbol snprintf {
}
Symbol blkXbdDevCreate {
}
Symbol free {
}
Symbol vxbDevSoftcSet {
}
Symbol sdCmdSendStatus {
}
Symbol mmcCmdSeEraseGrpStart {
}
Symbol mmcCmdSeEraseGrpEnd {
}
Symbol mmcCmdSetEraseGrp {
}
Symbol mmcCmdSetBlockNum {
}
Symbol sdCmdReadBlock {
}
Symbol sdCmdWriteBlock {
}
Symbol blkXbdDevDelete {
}
Symbol vxbDevProbe_desc {
}
Symbol vxbDevAttach_desc {
}
Symbol vxbDevShutdown_desc {
}
Symbol vxbDevDetach_desc {
}
Symbol vxbMmcStorageDrv {
}
Symbol __dataset__driver_vxbMmcStorageDrv {
}
Symbol cacheDmaMalloc {
}
Symbol memset {
}
Symbol sdACmdSwitchFunc {
}
Symbol cacheDmaFree {
}
Symbol sdCmdSwitchVoltage {
}
Symbol sdCmdSendRelativeAddr {
}
Symbol sdACmdSendScr {
}
Symbol sdACmdSetBusWidth {
}
Symbol sdACmdClrCardDetect {
}
Symbol vxbSdStorageDrv {
}
Symbol __dataset__driver_vxbSdStorageDrv {
}
Symbol __udiv32 {
}
Symbol errnoGet {
}
Symbol bio_done {
}
Symbol semTake {
}
Symbol semGive {
}
Symbol printf {
}
Symbol taskPrioritySet {
}
Symbol xbdEventCategory {
}
Symbol xbdEventInstantiated {
}
Symbol erfHandlerUnregister {
}
Symbol strncpy {
}
Symbol strncat {
}
Symbol semMCreate {
}
Symbol semBCreate {
}
Symbol taskSpawn {
}
Symbol erfHandlerRegister {
}
Symbol xbdAttach {
}
Symbol xbdEventPrimaryInsert {
}
Symbol erfEventRaise {
}
Symbol semDelete {
}
Symbol taskDelete {
}
Symbol vxAbsTicks {
}
Symbol taskCpuLock {
}
Symbol taskIsPended {
}
Symbol taskCpuUnlock {
}
Symbol taskDelay {
}
Symbol sysClkRateGet {
}
Symbol xbdEventRemove {
}
Symbol xbdDetach {
}
Symbol devUnmap {
}
Symbol __div64 {
}
Symbol xbdEventSoftInsert {
}
Symbol devName {
}
ObjectFile libsdmmcstoargecard.a::vxbMmcStorage.o {
	NAME Object file vxbMmcStorage.o from archive libsdmmcstoargecard.a
	EXPORTS vxbMmcStorageDrv __dataset__driver_vxbMmcStorageDrv
	IMPORTS vxbDevMatch vxbDevSoftcGet vxbDevIvarsGet __mul64 calloc bzero bcopy sdCmdAllSendCid sdMmcCmdSetRelativeAddr sdCmdSendCsd sdCmdSelectCard mmcCmdReadExtCsd sdCmdSetBlockLen mmcHrfsInclude mmcSwitch __udiv64 snprintf blkXbdDevCreate free vxbDevSoftcSet sdCmdSendStatus mmcCmdSeEraseGrpStart mmcCmdSeEraseGrpEnd mmcCmdSetEraseGrp mmcCmdSetBlockNum sdCmdReadBlock sdCmdWriteBlock blkXbdDevDelete vxbDevProbe_desc vxbDevAttach_desc vxbDevShutdown_desc vxbDevDetach_desc
	DECLARES 
	USES 
}
Module vxbMmcStorage.o {
	OBJECT += libsdmmcstoargecard.a::vxbMmcStorage.o
}
ObjectFile libsdmmcstoargecard.a::vxbSdStorage.o {
	NAME Object file vxbSdStorage.o from archive libsdmmcstoargecard.a
	EXPORTS vxbSdStorageDrv __dataset__driver_vxbSdStorageDrv
	IMPORTS vxbDevMatch vxbDevIvarsGet cacheDmaMalloc memset sdACmdSwitchFunc cacheDmaFree vxbDevSoftcGet calloc bcopy sdCmdSwitchVoltage sdCmdAllSendCid sdCmdSendRelativeAddr sdCmdSendCsd __mul64 sdCmdSelectCard sdACmdSendScr sdACmdSetBusWidth sdACmdClrCardDetect __udiv64 snprintf blkXbdDevCreate vxbDevSoftcSet sdCmdSendStatus sdCmdReadBlock sdCmdWriteBlock blkXbdDevDelete free vxbDevProbe_desc vxbDevAttach_desc vxbDevShutdown_desc vxbDevDetach_desc
	DECLARES 
	USES 
}
Module vxbSdStorage.o {
	OBJECT += libsdmmcstoargecard.a::vxbSdStorage.o
}
ObjectFile libsdmmcstoargecard.a::blkXbd.o {
	NAME Object file blkXbd.o from archive libsdmmcstoargecard.a
	EXPORTS blkXbdDevCreate blkXbdDevDelete
	IMPORTS __udiv32 errnoGet bio_done semTake semGive printf taskPrioritySet xbdEventCategory xbdEventInstantiated erfHandlerUnregister strncpy strncat semMCreate semBCreate taskSpawn erfHandlerRegister xbdAttach xbdEventPrimaryInsert erfEventRaise semDelete taskDelete vxAbsTicks taskCpuLock taskIsPended taskCpuUnlock taskDelay sysClkRateGet xbdEventRemove xbdDetach devUnmap __mul64 __div64 xbdEventSoftInsert devName
	DECLARES 
	USES 
}
Module blkXbd.o {
	OBJECT += libsdmmcstoargecard.a::blkXbd.o
}
Library libsdmmcstoargecard.a {
	MODULES libsdmmcstoargecard.a::vxbMmcStorage.o libsdmmcstoargecard.a::vxbSdStorage.o libsdmmcstoargecard.a::blkXbd.o
}
