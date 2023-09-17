Symbol __errno {
}
Symbol taskPriorityGet {
}
Symbol ossStatus {
}
Symbol errnoSet {
}
Symbol ossShutdown {
}
Symbol ossThreadCreate {
}
Symbol taskSpawn {
}
Symbol ossThreadDestroy {
}
Symbol taskDelete {
}
Symbol ossThreadSleep {
}
Symbol sysClkRateGet {
}
Symbol __div32 {
}
Symbol __rem32 {
}
Symbol taskDelay {
}
Symbol ossSemCreate {
}
Symbol semCCreate {
}
Symbol ossSemDestroy {
}
Symbol semDelete {
}
Symbol ossSemGive {
}
Symbol semGive {
}
Symbol ossSemTake {
}
Symbol semTake {
}
Symbol ossMutexCreate {
}
Symbol semMCreate {
}
Symbol ossMutexDestroy {
}
Symbol ossMutexTake {
}
Symbol ossMutexRelease {
}
Symbol ossPartSizeGet {
}
Symbol ossPartSizeSet {
}
Symbol ossPartIdGet {
}
Symbol ossMemUsedGet {
}
Symbol ossMalloc {
}
Symbol ossMallocFuncPtr {
}
Symbol ossPartMalloc {
}
Symbol cacheArchAlignSize {
}
Symbol memalign {
}
Symbol memPartCreate {
}
Symbol free {
}
Symbol memPartAlignedAlloc {
}
Symbol memPartAddToPool {
}
Symbol ossOldMalloc {
}
Symbol ossCalloc {
}
Symbol memset {
}
Symbol ossFree {
}
Symbol ossFreeFuncPtr {
}
Symbol ossPartFree {
}
Symbol memPartFree {
}
Symbol ossOldFree {
}
Symbol ossOldInstall {
}
Symbol ossTime {
}
Symbol tickGet {
}
Symbol ossInitialize {
}
Symbol usbDescrCopy32 {
}
Symbol memcpy {
}
Symbol usbDescrCopy {
}
Symbol usbDescrStrCopy32 {
}
Symbol strlen {
}
Symbol usbDescrStrCopy {
}
Symbol usbHandleInitialize {
}
Symbol usbHandleShutdown {
}
Symbol usbHandleCreate {
}
Symbol __urem32 {
}
Symbol usbHandleDestroy {
}
Symbol usbHandleValidate {
}
Symbol usbTransferTime {
}
Symbol __udiv32 {
}
Symbol usbRecurringTime {
}
Symbol usbDescrParseSkip {
}
Symbol usbDescrParse {
}
Symbol usbListLink {
}
Symbol usbListLinkProt {
}
Symbol usbListUnlink {
}
Symbol usbListUnlinkProt {
}
Symbol usbListFirst {
}
Symbol usbListNext {
}
Symbol usbQueueDestroy {
}
Symbol usbQueueCreate {
}
Symbol usbQueuePut {
}
Symbol usbQueueGet {
}
Symbol vxbUsbBus_desc {
}
Symbol __dataset__bustype_vxbUsbBus_desc {
}
Symbol vxbUsbRootHub_desc {
}
Symbol __dataset__bustype_vxbUsbRootHub_desc {
}
ObjectFile libusbcore.a::ossLib.o {
	NAME Object file ossLib.o from archive libusbcore.a
	EXPORTS ossStatus ossShutdown ossThreadCreate ossThreadDestroy ossThreadSleep ossSemCreate ossSemDestroy ossSemGive ossSemTake ossMutexCreate ossMutexDestroy ossMutexTake ossMutexRelease ossPartSizeGet ossPartSizeSet ossPartIdGet ossMemUsedGet ossMalloc ossPartMalloc ossOldMalloc ossCalloc ossFree ossPartFree ossOldFree ossOldInstall ossTime ossInitialize
	IMPORTS __errno taskPriorityGet errnoSet taskSpawn taskDelete sysClkRateGet __div32 __rem32 taskDelay semCCreate semDelete semGive semTake semMCreate cacheArchAlignSize memalign memPartCreate free memPartAlignedAlloc memPartAddToPool memset memPartFree tickGet
	DECLARES ossMallocFuncPtr ossFreeFuncPtr
	USES 
}
Module ossLib.o {
	OBJECT += libusbcore.a::ossLib.o
}
ObjectFile libusbcore.a::usbDescrCopyLib.o {
	NAME Object file usbDescrCopyLib.o from archive libusbcore.a
	EXPORTS usbDescrCopy32 usbDescrCopy usbDescrStrCopy32 usbDescrStrCopy
	IMPORTS memcpy strlen
	DECLARES 
	USES 
}
Module usbDescrCopyLib.o {
	OBJECT += libusbcore.a::usbDescrCopyLib.o
}
ObjectFile libusbcore.a::usbHandleLib.o {
	NAME Object file usbHandleLib.o from archive libusbcore.a
	EXPORTS usbHandleInitialize usbHandleShutdown usbHandleCreate usbHandleDestroy usbHandleValidate
	IMPORTS ossCalloc ossStatus ossMutexCreate ossFree ossMutexTake ossMutexDestroy __urem32 ossMutexRelease
	DECLARES 
	USES 
}
Module usbHandleLib.o {
	OBJECT += libusbcore.a::usbHandleLib.o
}
ObjectFile libusbcore.a::usbLib.o {
	NAME Object file usbLib.o from archive libusbcore.a
	EXPORTS usbTransferTime usbRecurringTime usbDescrParseSkip usbDescrParse
	IMPORTS __udiv32
	DECLARES 
	USES 
}
Module usbLib.o {
	OBJECT += libusbcore.a::usbLib.o
}
ObjectFile libusbcore.a::usbListLib.o {
	NAME Object file usbListLib.o from archive libusbcore.a
	EXPORTS usbListLink usbListLinkProt usbListUnlink usbListUnlinkProt usbListFirst usbListNext
	IMPORTS ossMutexTake ossMutexRelease
	DECLARES 
	USES 
}
Module usbListLib.o {
	OBJECT += libusbcore.a::usbListLib.o
}
ObjectFile libusbcore.a::usbQueueLib.o {
	NAME Object file usbQueueLib.o from archive libusbcore.a
	EXPORTS usbQueueDestroy usbQueueCreate usbQueuePut usbQueueGet
	IMPORTS ossStatus ossSemDestroy ossMutexTake ossMutexDestroy ossFree ossCalloc ossSemCreate ossMutexCreate ossSemTake ossSemGive ossMutexRelease memcpy
	DECLARES 
	USES 
}
Module usbQueueLib.o {
	OBJECT += libusbcore.a::usbQueueLib.o
}
ObjectFile libusbcore.a::vxbUsbBus.o {
	NAME Object file vxbUsbBus.o from archive libusbcore.a
	EXPORTS vxbUsbBus_desc __dataset__bustype_vxbUsbBus_desc vxbUsbRootHub_desc __dataset__bustype_vxbUsbRootHub_desc
	IMPORTS 
	DECLARES 
	USES 
}
Module vxbUsbBus.o {
	OBJECT += libusbcore.a::vxbUsbBus.o
}
Library libusbcore.a {
	MODULES libusbcore.a::ossLib.o libusbcore.a::usbDescrCopyLib.o libusbcore.a::usbHandleLib.o libusbcore.a::usbLib.o libusbcore.a::usbListLib.o libusbcore.a::usbQueueLib.o libusbcore.a::vxbUsbBus.o
}
