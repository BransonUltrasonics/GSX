Symbol flTakeMutex {
}
Symbol errnoSet {
}
Symbol flSocketSetBusy {
}
Symbol flNeedVcc {
}
Symbol flDontNeedVcc {
}
Symbol flFreeMutex {
}
Symbol flMediaCheck {
}
Symbol __div32 {
}
Symbol flMount {
}
Symbol fromUNALLONG {
}
Symbol flFormat {
}
Symbol malloc {
}
Symbol free {
}
Symbol flFileSysSectorStart {
}
Symbol memset {
}
Symbol strncmp {
}
Symbol flIdentifyFlash {
}
Symbol __udiv32 {
}
Symbol flInit {
}
Symbol flSocketOf {
}
Symbol flCreateMutex {
}
Symbol flSysfunInit {
}
Symbol flRegisterComponents {
}
Symbol flInitSockets {
}
Symbol flInitIdentifyTable {
}
Symbol flInstallTimer {
}
Symbol flCall {
}
Symbol noOfDrives {
}
Symbol flMsecCounter {
}
Symbol flIntervalRoutine {
}
Symbol flGetVolume {
}
Symbol memcpy {
}
Symbol memcmp {
}
Symbol flCrc16 {
}
Symbol bzero {
}
Symbol flRandByte {
}
Symbol sectorChecksumWrite {
}
Symbol logMsg {
}
Symbol flSocketNoOf {
}
Symbol flBufferOf {
}
Symbol printf {
}
Symbol mountFTL {
}
Symbol spinLockTaskInit {
}
Symbol semBCreate {
}
Symbol __udiv64 {
}
Symbol sectorChecksum {
}
Symbol memalign {
}
Symbol bcopy {
}
Symbol spinLockTaskTake {
}
Symbol spinLockTaskGive {
}
Symbol sleep {
}
Symbol __assert {
}
Symbol semGive {
}
Symbol tffsWrtLock {
}
Symbol semTake {
}
Symbol tffsWrtUnlock {
}
Symbol formatFTL {
}
Symbol mountValidateSet {
}
Symbol tffsVolStatInit {
}
Symbol vxAtomic32Set {
}
Symbol tffsVolStatShow {
}
Symbol vxAtomic32Get {
}
Symbol noOfTLs {
}
Symbol tlTable {
}
Symbol __rem32 {
}
Symbol __urem32 {
}
Symbol mountSSFDC {
}
Symbol formatSSFDC {
}
Symbol flIdentifyRegister {
}
Symbol mtdIdentifyTable {
}
Symbol flMap {
}
Symbol flSetPowerOnCallback {
}
Symbol flIntelIdentify {
}
Symbol flNeedVpp {
}
Symbol flDelayLoop {
}
Symbol flDontNeedVpp {
}
Symbol flIntelSize {
}
Symbol flResetCardChanged {
}
Symbol noOfMTDs {
}
Symbol mtdTable {
}
Symbol needCheckRAM {
}
Symbol flWriteProtected {
}
Symbol flSetWindowBusWidth {
}
Symbol flSetWindowSpeed {
}
Symbol flSetWindowSize {
}
Symbol flInitSocket {
}
Symbol flGetMappingContext {
}
Symbol flPollSemId {
}
Symbol tffsDrv {
}
Symbol fromUNAL {
}
Symbol tffsDevCreate {
}
Symbol semMCreate {
}
Symbol semDelete {
}
Symbol snprintf {
}
Symbol taskSpawn {
}
Symbol tffsDevOptionsSet {
}
Symbol tffsDrvOptionsSet {
}
Symbol tffsRawio {
}
Symbol tffsDiskChangeAnnounce {
}
Symbol tffsDevFormat {
}
Symbol tffsDevStatShow {
}
Symbol sysClkRateGet {
}
Symbol __mul64 {
}
Symbol __div64 {
}
Symbol tffsDevRawSpeedShow {
}
Symbol tickGet {
}
Symbol _d_lltod {
}
Symbol flPollTask {
}
Symbol taskDelay {
}
Symbol flRemoveTimer {
}
Symbol taskDelete {
}
Symbol randBytes {
}
Symbol rand {
}
Symbol flCurrentTime {
}
Symbol time {
}
Symbol localtime {
}
Symbol flCurrentDate {
}
Symbol flAddLongToFarPointer {
}
Symbol toUNAL {
}
Symbol toUNALLONG {
}
Symbol tffscmpWords {
}
ObjectFile libbdmTffs.a::fatlite.o {
	NAME Object file fatlite.o from archive libbdmTffs.a
	EXPORTS flFileSysSectorStart flInit flCall flMsecCounter flGetVolume
	IMPORTS flTakeMutex errnoSet flSocketSetBusy flNeedVcc flDontNeedVcc flFreeMutex flMediaCheck __div32 flMount fromUNALLONG flFormat malloc free memset strncmp flIdentifyFlash __udiv32 flSocketOf flCreateMutex flSysfunInit flRegisterComponents flInitSockets flInitIdentifyTable flInstallTimer noOfDrives flIntervalRoutine
	DECLARES 
	USES 
}
Module fatlite.o {
	OBJECT += libbdmTffs.a::fatlite.o
}
ObjectFile libbdmTffs.a::ftllite.o {
	NAME Object file ftllite.o from archive libbdmTffs.a
	EXPORTS sectorChecksumWrite mountFTL sectorChecksum tffsWrtLock tffsWrtUnlock formatFTL mountValidateSet tffsVolStatInit tffsVolStatShow
	IMPORTS __div32 memcpy memcmp flCrc16 errnoSet bzero flRandByte logMsg free malloc flSocketNoOf flBufferOf printf spinLockTaskInit semBCreate __udiv64 memset memalign bcopy spinLockTaskTake spinLockTaskGive sleep __assert semGive semTake __udiv32 vxAtomic32Set flGetVolume vxAtomic32Get
	DECLARES 
	USES 
}
Module ftllite.o {
	OBJECT += libbdmTffs.a::ftllite.o
}
ObjectFile libbdmTffs.a::fltl.o {
	NAME Object file fltl.o from archive libbdmTffs.a
	EXPORTS flMount flFormat
	IMPORTS flSocketOf flIdentifyFlash noOfTLs tlTable
	DECLARES 
	USES 
}
Module fltl.o {
	OBJECT += libbdmTffs.a::fltl.o
}
ObjectFile libbdmTffs.a::ssfdc.o {
	NAME Object file ssfdc.o from archive libbdmTffs.a
	EXPORTS mountSSFDC formatSSFDC
	IMPORTS __div32 __rem32 __udiv32 memcmp __urem32 flSocketNoOf flBufferOf malloc free memset
	DECLARES 
	USES 
}
Module ssfdc.o {
	OBJECT += libbdmTffs.a::ssfdc.o
}
ObjectFile libbdmTffs.a::flflash.o {
	NAME Object file flflash.o from archive libbdmTffs.a
	EXPORTS flIdentifyRegister flInitIdentifyTable flIntelIdentify flIntelSize flIdentifyFlash needCheckRAM
	IMPORTS flMap memcpy flSetPowerOnCallback flNeedVpp flDelayLoop flDontNeedVpp flResetCardChanged flMediaCheck noOfMTDs mtdTable
	DECLARES mtdIdentifyTable
	USES 
}
Module flflash.o {
	OBJECT += libbdmTffs.a::flflash.o
}
ObjectFile libbdmTffs.a::flsocket.o {
	NAME Object file flsocket.o from archive libbdmTffs.a
	EXPORTS flSocketNoOf flSocketOf flBufferOf flWriteProtected flResetCardChanged flMediaCheck flSetWindowBusWidth flSetWindowSpeed flSetWindowSize flInitSocket flInitSockets noOfDrives flGetMappingContext flMap flSocketSetBusy flNeedVcc flDontNeedVcc flNeedVpp flDontNeedVpp flSetPowerOnCallback flIntervalRoutine
	IMPORTS errnoSet malloc
	DECLARES 
	USES 
}
Module flsocket.o {
	OBJECT += libbdmTffs.a::flsocket.o
}
ObjectFile libbdmTffs.a::backgrnd.o {
	NAME Object file backgrnd.o from archive libbdmTffs.a
	EXPORTS 
	IMPORTS 
	DECLARES 
	USES 
}
Module backgrnd.o {
	OBJECT += libbdmTffs.a::backgrnd.o
}
ObjectFile libbdmTffs.a::tffsDrv.o {
	NAME Object file tffsDrv.o from archive libbdmTffs.a
	EXPORTS tffsDrv tffsDevCreate tffsDevOptionsSet tffsDrvOptionsSet tffsRawio tffsDiskChangeAnnounce tffsDevFormat tffsDevStatShow tffsDevRawSpeedShow
	IMPORTS semBCreate flInit semGive fromUNAL flFileSysSectorStart vxAtomic32Set malloc bzero flCall semMCreate free errnoSet semDelete snprintf taskSpawn semTake sysClkRateGet printf vxAtomic32Get __udiv32 __mul64 __div64 memset tickGet _d_lltod
	DECLARES flPollSemId
	USES 
}
Module tffsDrv.o {
	OBJECT += libbdmTffs.a::tffsDrv.o
}
ObjectFile libbdmTffs.a::tffsLib.o {
	NAME Object file tffsLib.o from archive libbdmTffs.a
	EXPORTS flCreateMutex flTakeMutex flFreeMutex flSysfunInit flPollTask flInstallTimer flRemoveTimer flRandByte flCurrentTime flCurrentDate flAddLongToFarPointer
	IMPORTS semMCreate semTake semGive sysClkRateGet flPollSemId __div32 semDelete taskDelay taskSpawn taskDelete randBytes rand time localtime
	DECLARES 
	USES 
}
Module tffsLib.o {
	OBJECT += libbdmTffs.a::tffsLib.o
}
ObjectFile libbdmTffs.a::flbase.o {
	NAME Object file flbase.o from archive libbdmTffs.a
	EXPORTS toUNAL fromUNAL toUNALLONG fromUNALLONG tffscmpWords flCrc16
	IMPORTS 
	DECLARES 
	USES 
}
Module flbase.o {
	OBJECT += libbdmTffs.a::flbase.o
}
Library libbdmTffs.a {
	MODULES libbdmTffs.a::fatlite.o libbdmTffs.a::ftllite.o libbdmTffs.a::fltl.o libbdmTffs.a::ssfdc.o libbdmTffs.a::flflash.o libbdmTffs.a::flsocket.o libbdmTffs.a::backgrnd.o libbdmTffs.a::tffsDrv.o libbdmTffs.a::tffsLib.o libbdmTffs.a::flbase.o
}
