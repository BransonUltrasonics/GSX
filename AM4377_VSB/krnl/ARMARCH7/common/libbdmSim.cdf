Symbol lstInit {
}
Symbol semMCreate {
}
Symbol fsimInit {
}
Symbol mtdDevCreate {
}
Symbol mtdDevDelete {
}
Symbol semTake {
}
Symbol lstFirst {
}
Symbol strcmp {
}
Symbol lstNext {
}
Symbol semGive {
}
Symbol fsimStart {
}
Symbol errnoSet {
}
Symbol open {
}
Symbol taskIdCurrent {
}
Symbol printf {
}
Symbol lseek {
}
Symbol memset {
}
Symbol write {
}
Symbol close {
}
Symbol lstAdd {
}
Symbol __rem32 {
}
Symbol ioctl {
}
Symbol fsimDevCreate {
}
Symbol ffs32Lsb {
}
Symbol __udiv64 {
}
Symbol fsimChipCreate {
}
Symbol malloc {
}
Symbol free {
}
Symbol fsimChipDelete {
}
Symbol fsimCreate {
}
Symbol strlen {
}
Symbol strcpy {
}
Symbol fsimErase {
}
Symbol fsimReadPage {
}
Symbol fsimWritePage {
}
Symbol fsimRead {
}
Symbol fsimWrite {
}
Symbol fsimReadPages {
}
Symbol fsimWritePages {
}
Symbol fsimIsBlankPage {
}
Symbol lstDelete {
}
Symbol fsimDevDelete {
}
Symbol fsimDelete {
}
Symbol read {
}
Symbol fsimMount {
}
Symbol fsimUnmount {
}
Symbol errnoGet {
}
Symbol fsimRun {
}
Symbol fsimStop {
}
Symbol fsimDevPageRW {
}
Symbol __udiv32 {
}
Symbol __lsr64 {
}
Symbol __urem64 {
}
Symbol fsimDevPagesRW {
}
Symbol getChipBlkNo {
}
Symbol getChipPageNo {
}
Symbol fsimIoctl {
}
Symbol fsimShow {
}
Symbol fsimShowAll {
}
Symbol __div32 {
}
Symbol fsimPrintBuf {
}
Symbol memcpy {
}
Symbol fsimDump {
}
Symbol fsimDumpData {
}
Symbol fsimDumpOob {
}
Symbol fsimImageSave {
}
Symbol fsimImageCopy {
}
Symbol fsimDevStatChip {
}
Symbol fsimStatChip {
}
Symbol fsimStatBlk {
}
Symbol fsimStatPage {
}
Symbol reboot {
}
Symbol fsimMarkBad {
}
Symbol fsimUnmarkBad {
}
Symbol fsimIsBadBlock {
}
Symbol fsimInjectError {
}
Symbol fsimErrorGenerator {
}
Symbol fsimRemoveError {
}
Symbol fsimReboot {
}
Symbol taskIdVerify {
}
Symbol fsimInvalidateMap {
}
Symbol fsimIsPageMapped {
}
Symbol fsimUpdateMapStatus {
}
Symbol fsimFind {
}
Symbol fsimDbgTurn {
}
Symbol tickGet {
}
Symbol srand {
}
Symbol rand {
}
Symbol __urem32 {
}
Symbol semDelete {
}
Symbol taskIdSelf {
}
Symbol sysClkRateGet {
}
Symbol taskDelay {
}
Symbol fsimCrc32 {
}
Symbol mtdRegisterNotify {
}
Symbol mtdUnRegisterNotify {
}
Symbol mtdFindAndNotify {
}
Symbol mtdNotify {
}
Symbol fsimTffsRegister {
}
Symbol noOfDrives {
}
Symbol flSocketOf {
}
Symbol needCheckRAM {
}
Symbol printErr {
}
Symbol strncpy {
}
Symbol fsimTffsList {
}
Symbol fsimNorIdentify {
}
Symbol flIdentifyRegister {
}
Symbol flInitSocket {
}
Symbol fsimTffsUnregister {
}
Symbol fsimTffsShow {
}
Symbol fsimTffsFind {
}
Symbol flSetWindowSize {
}
Symbol fsimTffsFormat {
}
Symbol tffsDevFormat {
}
Symbol fsimNorMap {
}
ObjectFile libbdmSim.a::fsimLib.o {
	NAME Object file fsimLib.o from archive libbdmSim.a
	EXPORTS fsimInit fsimStart fsimDevCreate fsimCreate fsimErase fsimReadPage fsimWritePage fsimRead fsimWrite fsimReadPages fsimWritePages fsimIsBlankPage fsimDevDelete fsimDelete fsimMount fsimUnmount fsimRun fsimStop fsimDevPageRW fsimDevPagesRW getChipBlkNo getChipPageNo fsimIoctl fsimShow fsimShowAll fsimPrintBuf fsimDump fsimDumpData fsimDumpOob fsimImageSave fsimImageCopy fsimDevStatChip fsimStatChip fsimStatBlk fsimStatPage fsimMarkBad fsimUnmarkBad fsimIsBadBlock fsimInjectError fsimErrorGenerator fsimRemoveError fsimReboot fsimInvalidateMap fsimIsPageMapped fsimUpdateMapStatus fsimFind fsimDbgTurn
	IMPORTS lstInit semMCreate mtdDevCreate mtdDevDelete semTake lstFirst strcmp lstNext semGive errnoSet open taskIdCurrent printf lseek memset write close lstAdd __rem32 ioctl ffs32Lsb __udiv64 fsimChipCreate malloc free fsimChipDelete strlen strcpy lstDelete read errnoGet __udiv32 __lsr64 __urem64 __div32 memcpy reboot taskIdVerify
	DECLARES 
	USES 
}
Module fsimLib.o {
	OBJECT += libbdmSim.a::fsimLib.o
}
ObjectFile libbdmSim.a::fsimChip.o {
	NAME Object file fsimChip.o from archive libbdmSim.a
	EXPORTS fsimChipCreate fsimChipDelete
	IMPORTS __udiv32 lseek write taskIdCurrent printf errnoSet ioctl semTake memset semGive errnoGet read tickGet srand rand __urem32 open close malloc semMCreate memcpy semDelete free taskIdSelf sysClkRateGet __rem32 __div32 taskDelay
	DECLARES 
	USES 
}
Module fsimChip.o {
	OBJECT += libbdmSim.a::fsimChip.o
}
ObjectFile libbdmSim.a::fsimMtd.o {
	NAME Object file fsimMtd.o from archive libbdmSim.a
	EXPORTS fsimCrc32 mtdRegisterNotify mtdUnRegisterNotify mtdFindAndNotify mtdNotify mtdDevCreate mtdDevDelete
	IMPORTS malloc printf memset free memcpy fsimInjectError __udiv32 taskIdCurrent fsimIsBadBlock strcmp
	DECLARES 
	USES 
}
Module fsimMtd.o {
	OBJECT += libbdmSim.a::fsimMtd.o
}
ObjectFile libbdmSim.a::fsimTffs.o {
	NAME Object file fsimTffs.o from archive libbdmSim.a
	EXPORTS fsimTffsRegister fsimNorIdentify fsimTffsUnregister fsimTffsShow fsimTffsFind fsimTffsFormat fsimNorMap
	IMPORTS noOfDrives flSocketOf fsimFind printf memset needCheckRAM malloc printErr strncpy lstAdd flIdentifyRegister flInitSocket lstDelete free lstFirst lstNext flSetWindowSize tffsDevFormat __udiv32 fsimInvalidateMap fsimIsPageMapped fsimUpdateMapStatus
	DECLARES fsimTffsList
	USES 
}
Module fsimTffs.o {
	OBJECT += libbdmSim.a::fsimTffs.o
}
Library libbdmSim.a {
	MODULES libbdmSim.a::fsimLib.o libbdmSim.a::fsimChip.o libbdmSim.a::fsimMtd.o libbdmSim.a::fsimTffs.o
}
