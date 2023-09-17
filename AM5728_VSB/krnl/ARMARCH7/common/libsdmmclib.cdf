Symbol VXB_SDSPECINFO_GET {
}
Symbol vxbSdspecinfoGet_desc {
}
Symbol vxbDevMethodFind {
}
Symbol sdCmdGoIdleState {
}
Symbol vxbDevIvarsGet {
}
Symbol bzero {
}
Symbol vxbMsDelay {
}
Symbol sdMmcCmdSendOpCond {
}
Symbol sdCmdSendRelativeAddr {
}
Symbol sdioCmdIoSendOpCond {
}
Symbol sdCmdSelectCard {
}
Symbol sdCmdSendIfCond {
}
Symbol sdCmdAppCmd {
}
Symbol sdACmdSendOpCond {
}
Symbol sdDeviceCreate {
}
Symbol vxbDevCreate {
}
Symbol calloc {
}
Symbol vxbDevClassSet {
}
Symbol vxbDevIvarsSet {
}
Symbol vxbDevUnitGet {
}
Symbol vxbDevNameSet {
}
Symbol vxbDevAdd {
}
Symbol free {
}
Symbol sdCmdAllSendCid {
}
Symbol bcopy {
}
Symbol sdMmcCmdSetRelativeAddr {
}
Symbol sdCmdIssue {
}
Symbol sdCmdSetDsr {
}
Symbol sdCmdSendStatus {
}
Symbol mmcSwitch {
}
Symbol sdACmdSwitchFunc {
}
Symbol sdCmdDeselectCard {
}
Symbol sdMmcSendTuning {
}
Symbol cacheDmaMalloc {
}
Symbol cacheDmaFree {
}
Symbol mmcCmdReadExtCsd {
}
Symbol sdCmdSendCsd {
}
Symbol sdCmdSendCid {
}
Symbol sdCmdSwitchVoltage {
}
Symbol sdCmdStopTransmission {
}
Symbol sdCmdGoInactiveState {
}
Symbol sdCmdSetBlockLen {
}
Symbol mmcCmdSetBlockNum {
}
Symbol sdSendTuning {
}
Symbol malloc {
}
Symbol mmcCmdSeEraseGrpStart {
}
Symbol mmcCmdSeEraseGrpEnd {
}
Symbol mmcCmdSetEraseGrp {
}
Symbol mmcSendHpi {
}
Symbol sdCmdReadSingleBlock {
}
Symbol sdCmdReadMultipleBlock {
}
Symbol sdCmdReadBlock {
}
Symbol __mul64 {
}
Symbol sdCmdWriteSingleBlock {
}
Symbol sdCmdWriteMultipleBlock {
}
Symbol sdCmdWriteBlock {
}
Symbol sdACmdSetBusWidth {
}
Symbol sdACmdClrCardDetect {
}
Symbol sdACmdSendWriteBlkNum {
}
Symbol sdACmdSendScr {
}
Symbol sdACmdSendSsr {
}
Symbol sdioCmdIoRwDirect {
}
Symbol sdioCmdIoRwExtend {
}
Symbol vxbSdioReadCis {
}
Symbol vxbSdioSetFunc {
}
Symbol vxbSdMmcBus_desc {
}
Symbol __dataset__bustype_vxbSdMmcBus_desc {
}
ObjectFile libsdmmclib.a::vxbSdMmcMethod.o {
	NAME Object file vxbSdMmcMethod.o from archive libsdmmclib.a
	EXPORTS VXB_SDSPECINFO_GET vxbSdspecinfoGet_desc
	IMPORTS vxbDevMethodFind
	DECLARES 
	USES 
}
Module vxbSdMmcMethod.o {
	OBJECT += libsdmmclib.a::vxbSdMmcMethod.o
}
ObjectFile libsdmmclib.a::vxbSdMmcLib.o {
	NAME Object file vxbSdMmcLib.o from archive libsdmmclib.a
	EXPORTS sdCmdGoIdleState sdMmcCmdSendOpCond sdCmdSendRelativeAddr sdioCmdIoSendOpCond sdCmdSelectCard sdCmdSendIfCond sdCmdAppCmd sdACmdSendOpCond sdDeviceCreate sdCmdAllSendCid sdMmcCmdSetRelativeAddr sdCmdIssue sdCmdSetDsr sdCmdSendStatus mmcSwitch sdACmdSwitchFunc sdCmdDeselectCard sdMmcSendTuning mmcCmdReadExtCsd sdCmdSendCsd sdCmdSendCid sdCmdSwitchVoltage sdCmdStopTransmission sdCmdGoInactiveState sdCmdSetBlockLen mmcCmdSetBlockNum sdSendTuning mmcCmdSeEraseGrpStart mmcCmdSeEraseGrpEnd mmcCmdSetEraseGrp mmcSendHpi sdCmdReadSingleBlock sdCmdReadMultipleBlock sdCmdReadBlock sdCmdWriteSingleBlock sdCmdWriteMultipleBlock sdCmdWriteBlock sdACmdSetBusWidth sdACmdClrCardDetect sdACmdSendWriteBlkNum sdACmdSendScr sdACmdSendSsr sdioCmdIoRwDirect sdioCmdIoRwExtend vxbSdioReadCis vxbSdioSetFunc vxbSdMmcBus_desc __dataset__bustype_vxbSdMmcBus_desc
	IMPORTS vxbDevIvarsGet bzero vxbMsDelay vxbDevCreate calloc VXB_SDSPECINFO_GET vxbDevClassSet vxbDevIvarsSet vxbDevUnitGet vxbDevNameSet vxbDevAdd free bcopy cacheDmaMalloc cacheDmaFree malloc __mul64
	DECLARES 
	USES 
}
Module vxbSdMmcLib.o {
	OBJECT += libsdmmclib.a::vxbSdMmcLib.o
}
Library libsdmmclib.a {
	MODULES libsdmmclib.a::vxbSdMmcMethod.o libsdmmclib.a::vxbSdMmcLib.o
}
