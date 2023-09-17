Symbol memSysPartId {
}
Symbol memPartFree {
}
Symbol strncpy {
}
Symbol bzero {
}
Symbol strlen {
}
Symbol memchr {
}
Symbol bcopy {
}
Symbol memmove {
}
Symbol dosFsMsgLevel {
}
Symbol printf {
}
Symbol __div64 {
}
Symbol malloc {
}
Symbol free {
}
Symbol memPartAlloc {
}
Symbol memFindMax {
}
Symbol dosfsHostToDisk32 {
}
Symbol dosChkDsk {
}
Symbol time {
}
Symbol dosChkMinDate {
}
Symbol ctime {
}
Symbol clock_settime {
}
Symbol strncat {
}
Symbol tickGet {
}
Symbol dosfsDiskToHost32 {
}
Symbol _func_dosChkStatPrint {
}
Symbol dosChkLibInit {
}
Symbol dosFsChkRtn {
}
Symbol dosChkDebug {
}
Symbol __udiv32 {
}
Symbol __rem64 {
}
Symbol dosFsCacheLibInit {
}
Symbol dosFsCacheMountRtn {
}
Symbol dosFsCacheUnmountRtn {
}
Symbol dosFsCacheCreate {
}
Symbol dosFsCacheCreateRtn {
}
Symbol dosFsCacheDeleteRtn {
}
Symbol dosFsCacheDataDirDefaultSize {
}
Symbol dosFsCacheFatDefaultSize {
}
Symbol dosFsDefaultDataDirCacheSizeGet {
}
Symbol dosFsDefaultFatCacheSizeGet {
}
Symbol dosFsDefaultCacheSizeSet {
}
Symbol BLOCKS_PER_DATA_DIR_CACHE_GROUP {
}
Symbol BLOCKS_PER_FAT_CACHE_GROUP {
}
Symbol __urem32 {
}
Symbol cacheDmaMalloc {
}
Symbol semBCreate {
}
Symbol spinLockTaskInit {
}
Symbol dllAdd {
}
Symbol semMCreate {
}
Symbol semDelete {
}
Symbol cacheDmaFree {
}
Symbol semGive {
}
Symbol semTake {
}
Symbol dosFsXbdIoctl {
}
Symbol dosFsXbdBlkWrite {
}
Symbol dosFsXbdBlkRead {
}
Symbol dosFsXbdBytesRW {
}
Symbol dosFsXbdBlkCopy {
}
Symbol dosFsVolDescGet {
}
Symbol snprintf {
}
Symbol taskSpawn {
}
Symbol taskDelete {
}
Symbol dosFsCacheDelete {
}
Symbol xbdStrategy {
}
Symbol vxAtomicDec {
}
Symbol spinLockTaskTake {
}
Symbol spinLockTaskGive {
}
Symbol taskIdSelf {
}
Symbol dllRemove {
}
Symbol dllInsert {
}
Symbol taskDelay {
}
Symbol __errno {
}
Symbol vxAtomicInc {
}
Symbol dosFsDiscard {
}
Symbol xbdIoctl {
}
Symbol errnoSet {
}
Symbol vxAtomicGet {
}
Symbol dosFsCacheTune {
}
Symbol dosFsCacheInfo {
}
Symbol dosfsDiskToHost16 {
}
Symbol localtime_r {
}
Symbol mktime {
}
Symbol dosfsHostToDisk16 {
}
Symbol bfill {
}
Symbol bcmp {
}
Symbol dosPathParse {
}
Symbol errnoGet {
}
Symbol memPartRealloc {
}
Symbol dosDirOldDebug {
}
Symbol logMsg {
}
Symbol _func_dosDirOldShow {
}
Symbol dosDirOldLibInit {
}
Symbol dosDirHdlrsList {
}
Symbol dosFsHdlrInstall {
}
Symbol fat16Debug {
}
Symbol __div32 {
}
Symbol __mul64 {
}
Symbol __lsl64 {
}
Symbol fat16ClustValueSet {
}
Symbol fat16ClustValueGet {
}
Symbol fat16VolMount {
}
Symbol _func_dosFsFatShow {
}
Symbol fatClugFac {
}
Symbol dosFsFatInit {
}
Symbol dosFatHdlrsList {
}
Symbol ioctl {
}
Symbol read {
}
Symbol write {
}
Symbol strcmp {
}
Symbol dosFsVolIsFat12 {
}
Symbol dosFsFmtLargeWriteSize {
}
Symbol devGetByName {
}
Symbol getHiddenSectorsFromDPT {
}
Symbol ffsMsb {
}
Symbol printErr {
}
Symbol fsPathAddedEventSetup {
}
Symbol fsWaitForPath {
}
Symbol dosFsVolFormatWithLabel {
}
Symbol open {
}
Symbol close {
}
Symbol dosFsVolFormat {
}
Symbol dosFsVolFormatFd {
}
Symbol dosFsFmtLibInit {
}
Symbol _func_dosFsFmtRtn {
}
Symbol iosDevFind {
}
Symbol dosFsVolumeOptionsSet {
}
Symbol dosFsVolumeOptionsGet {
}
Symbol dosSetVolCaseSens {
}
Symbol dosFsVolUnmount {
}
Symbol dosFsChkDsk {
}
Symbol dosFsDebug {
}
Symbol xbdNBlocks {
}
Symbol xbdBlockSize {
}
Symbol dosFsFdFree {
}
Symbol dosFsFdGet {
}
Symbol __asr64 {
}
Symbol dosFsClose {
}
Symbol dosFsOpen {
}
Symbol memcmp {
}
Symbol strncasecmp {
}
Symbol dosVDirLibUptDotDot {
}
Symbol dosFsIoctl {
}
Symbol ioFullFileNameGet {
}
Symbol dosVDirReaddirPlus {
}
Symbol dosFsLastAccessDateEnable {
}
Symbol dosFsLibInit {
}
Symbol dosFsDrvNum {
}
Symbol iosDrvInstall {
}
Symbol dosFsDevCreate {
}
Symbol dosFsMutexOptions {
}
Symbol semCCreate {
}
Symbol dosFsVolumeBioBufferSize {
}
Symbol bio_alloc {
}
Symbol iosLock {
}
Symbol iosDevReplaceExt {
}
Symbol iosUnlock {
}
Symbol iosDevDelCallback {
}
Symbol fsPathAddedEventRaise {
}
Symbol bio_free {
}
Symbol dosFsDevDelete {
}
Symbol intContext {
}
Symbol iosDevDelete {
}
Symbol xbdEventCategory {
}
Symbol xbdEventRemove {
}
Symbol xbdEventMediaChanged {
}
Symbol erfHandlerUnregister {
}
Symbol iosDevSuspend {
}
Symbol dosFsMonitorDevCreate {
}
Symbol erfHandlerRegister {
}
Symbol dosFsDiskProbe {
}
Symbol _func_dosVDirShow {
}
Symbol __ctype {
}
Symbol dosFsCompatNT {
}
Symbol __ctype_tolower {
}
Symbol _func_utf8ToUtf16String {
}
Symbol _func_utf16ToUtf8String {
}
Symbol __ctype_toupper {
}
Symbol dosVDirDebug {
}
Symbol dosVDirMutexOpt {
}
Symbol dosVDirLibInit {
}
Symbol dosFsShowInit {
}
Symbol dosFsShow {
}
Symbol _func_dosFsShowRtn {
}
Symbol print64Fine {
}
Symbol print64Mult {
}
Symbol dosFsCacheShow {
}
Symbol dosFsTCacheShow {
}
Symbol print64 {
}
Symbol print64Row {
}
ObjectFile libfsDosfs.a::dosChkLib.o {
	NAME Object file dosChkLib.o from archive libfsDosfs.a
	EXPORTS dosChkDsk dosChkMinDate dosChkLibInit dosChkDebug
	IMPORTS memSysPartId memPartFree strncpy bzero strlen memchr bcopy memmove dosFsMsgLevel printf __div64 malloc free memPartAlloc memFindMax dosfsHostToDisk32 time ctime clock_settime strncat tickGet dosfsDiskToHost32 _func_dosChkStatPrint dosFsChkRtn
	DECLARES 
	USES 
}
Module dosChkLib.o {
	OBJECT += libfsDosfs.a::dosChkLib.o
}
ObjectFile libfsDosfs.a::dosFsCacheLib.o {
	NAME Object file dosFsCacheLib.o from archive libfsDosfs.a
	EXPORTS dosFsCacheLibInit dosFsCacheCreate dosFsDefaultDataDirCacheSizeGet dosFsDefaultFatCacheSizeGet dosFsDefaultCacheSizeSet dosFsCacheDelete dosFsCacheTune dosFsCacheInfo
	IMPORTS __udiv32 __rem64 __div64 dosFsCacheMountRtn dosFsCacheUnmountRtn dosFsCacheCreateRtn dosFsCacheDeleteRtn dosFsCacheDataDirDefaultSize dosFsCacheFatDefaultSize bzero BLOCKS_PER_DATA_DIR_CACHE_GROUP BLOCKS_PER_FAT_CACHE_GROUP printf __urem32 cacheDmaMalloc semBCreate malloc spinLockTaskInit dllAdd semMCreate semDelete free cacheDmaFree semGive semTake dosFsXbdIoctl dosFsXbdBlkWrite dosFsXbdBlkRead dosFsXbdBytesRW dosFsXbdBlkCopy dosFsVolDescGet snprintf taskSpawn taskDelete xbdStrategy vxAtomicDec spinLockTaskTake spinLockTaskGive taskIdSelf dllRemove dllInsert taskDelay __errno vxAtomicInc bcopy dosFsDiscard xbdIoctl errnoSet vxAtomicGet
	DECLARES 
	USES 
}
Module dosFsCacheLib.o {
	OBJECT += libfsDosfs.a::dosFsCacheLib.o
}
ObjectFile libfsDosfs.a::dosDirOldLib.o {
	NAME Object file dosDirOldLib.o from archive libfsDosfs.a
	EXPORTS dosDirOldDebug dosDirOldLibInit
	IMPORTS dosfsDiskToHost16 dosfsDiskToHost32 bzero time localtime_r mktime dosfsHostToDisk16 bfill errnoSet bcmp dosfsHostToDisk32 bcopy __rem64 dosPathParse errnoGet strlen dosFsXbdBytesRW memSysPartId memPartRealloc logMsg __udiv32 _func_dosDirOldShow dosFsXbdIoctl dosDirHdlrsList dosFsHdlrInstall
	DECLARES 
	USES 
}
Module dosDirOldLib.o {
	OBJECT += libfsDosfs.a::dosDirOldLib.o
}
ObjectFile libfsDosfs.a::dosFsFat.o {
	NAME Object file dosFsFat.o from archive libfsDosfs.a
	EXPORTS fat16Debug fat16ClustValueSet fat16ClustValueGet fat16VolMount fatClugFac dosFsFatInit
	IMPORTS dosfsHostToDisk32 errnoSet semTake logMsg semGive __div64 __rem64 dosFsDiscard bzero __div32 malloc free __mul64 __lsl64 __udiv32 bfill memSysPartId memPartRealloc dosFsXbdBytesRW dosfsDiskToHost32 _func_dosFsFatShow dosFsXbdBlkCopy dosFsXbdIoctl semMCreate dosFatHdlrsList dosFsHdlrInstall
	DECLARES 
	USES 
}
Module dosFsFat.o {
	OBJECT += libfsDosfs.a::dosFsFat.o
}
ObjectFile libfsDosfs.a::dosFsFmtLib.o {
	NAME Object file dosFsFmtLib.o from archive libfsDosfs.a
	EXPORTS dosFsFmtLargeWriteSize dosFsVolFormatWithLabel dosFsVolFormat dosFsVolFormatFd dosFsFmtLibInit
	IMPORTS dosFsMsgLevel printf __udiv32 __div32 errnoSet __errno bzero ioctl read write malloc free dosfsDiskToHost16 dosfsDiskToHost32 bcopy strcmp dosFsVolIsFat12 tickGet __div64 dosfsHostToDisk16 dosfsHostToDisk32 devGetByName getHiddenSectorsFromDPT ffsMsb strncpy printErr fsPathAddedEventSetup fsWaitForPath strlen open close _func_dosFsFmtRtn
	DECLARES 
	USES 
}
Module dosFsFmtLib.o {
	OBJECT += libfsDosfs.a::dosFsFmtLib.o
}
ObjectFile libfsDosfs.a::dosFsLib.o {
	NAME Object file dosFsLib.o from archive libfsDosfs.a
	EXPORTS dosfsHostToDisk32 dosfsHostToDisk16 dosfsDiskToHost32 dosfsDiskToHost16 dosFsVolDescGet dosFsVolumeOptionsSet dosFsVolumeOptionsGet dosSetVolCaseSens dosFsVolUnmount dosFsCacheUnmountRtn dosFsChkDsk dosFsChkRtn dosFsDebug dosFsVolIsFat12 dosFsXbdBytesRW dosDirHdlrsList dosFatHdlrsList dosFsFdFree dosFsFdGet dosFsClose dosFsOpen dosPathParse dosFsIoctl dosFsCacheMountRtn dosFsLastAccessDateEnable dosFsLibInit dosFsDrvNum dosFsDevCreate dosFsMutexOptions dosFsXbdBlkWrite dosFsXbdBlkRead dosFsXbdIoctl dosFsDevDelete dosFsCacheDeleteRtn dosFsMonitorDevCreate dosFsCacheCreateRtn dosFsCacheDataDirDefaultSize dosFsCacheFatDefaultSize dosFsDiskProbe dosFsHdlrInstall dosFsXbdBlkCopy dosFsDiscard _func_dosDirOldShow _func_dosVDirShow _func_dosFsFatShow _func_dosChkStatPrint
	IMPORTS iosDevFind errnoSet dosFsMsgLevel printf semTake semGive __div32 logMsg strlen bcmp __udiv32 xbdStrategy bcopy __errno xbdIoctl xbdNBlocks xbdBlockSize ffsMsb strcmp errnoGet bzero __asr64 time __div64 memcmp strncasecmp dosVDirLibUptDotDot ioFullFileNameGet __lsl64 dosVDirReaddirPlus iosDrvInstall memSysPartId memPartAlloc memPartFree semMCreate semCCreate semBCreate dosFsVolumeBioBufferSize bio_alloc iosLock iosDevReplaceExt iosUnlock iosDevDelCallback fsPathAddedEventRaise semDelete bio_free intContext iosDevDelete xbdEventCategory xbdEventRemove xbdEventMediaChanged erfHandlerUnregister iosDevSuspend open close erfHandlerRegister
	DECLARES 
	USES 
}
Module dosFsLib.o {
	OBJECT += libfsDosfs.a::dosFsLib.o
}
ObjectFile libfsDosfs.a::dosVDirLib.o {
	NAME Object file dosVDirLib.o from archive libfsDosfs.a
	EXPORTS dosVDirDebug dosVDirReaddirPlus dosVDirMutexOpt dosVDirLibInit dosVDirLibUptDotDot
	IMPORTS bzero dosfsDiskToHost16 dosfsDiskToHost32 time localtime_r mktime dosfsHostToDisk16 bcmp __ctype bcopy dosFsCompatNT __ctype_tolower _func_utf8ToUtf16String __div32 _func_utf16ToUtf8String __urem32 snprintf __ctype_toupper bfill dosFsMsgLevel printf errnoSet dosfsHostToDisk32 __rem64 semTake semGive logMsg strncpy __div64 memSysPartId memPartAlloc dosPathParse errnoGet memPartFree strlen dosFsXbdBytesRW memPartRealloc __udiv32 _func_dosVDirShow dosFsXbdIoctl semMCreate dosDirHdlrsList dosFsHdlrInstall malloc free
	DECLARES 
	USES 
}
Module dosVDirLib.o {
	OBJECT += libfsDosfs.a::dosVDirLib.o
}
ObjectFile libfsDosfs.a::dosFsShow.o {
	NAME Object file dosFsShow.o from archive libfsDosfs.a
	EXPORTS dosFsShowInit dosFsShow dosFsCacheShow dosFsTCacheShow
	IMPORTS _func_dosDirOldShow _func_dosVDirShow _func_dosFsFatShow _func_dosChkStatPrint _func_dosFsShowRtn dosFsVolDescGet dosFsMsgLevel printf dosFsOpen dosFsClose dosFsFdGet dosFsIoctl dosFsFdFree print64Fine print64Mult __udiv32 __mul64 __lsl64
	DECLARES 
	USES 
}
Module dosFsShow.o {
	OBJECT += libfsDosfs.a::dosFsShow.o
}
ObjectFile libfsDosfs.a::print64Lib.o {
	NAME Object file print64Lib.o from archive libfsDosfs.a
	EXPORTS print64 print64Fine print64Row print64Mult
	IMPORTS printf __urem32 __udiv32 __div64
	DECLARES 
	USES 
}
Module print64Lib.o {
	OBJECT += libfsDosfs.a::print64Lib.o
}
Library libfsDosfs.a {
	MODULES libfsDosfs.a::dosChkLib.o libfsDosfs.a::dosFsCacheLib.o libfsDosfs.a::dosDirOldLib.o libfsDosfs.a::dosFsFat.o libfsDosfs.a::dosFsFmtLib.o libfsDosfs.a::dosFsLib.o libfsDosfs.a::dosVDirLib.o libfsDosfs.a::dosFsShow.o libfsDosfs.a::print64Lib.o
}
