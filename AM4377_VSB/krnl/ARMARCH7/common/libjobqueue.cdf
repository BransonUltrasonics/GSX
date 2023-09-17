Symbol jobQueueLibInit {
}
Symbol jobQueuePost {
}
Symbol semGive {
}
Symbol jobQueueStdPost {
}
Symbol jobQueueProcess {
}
Symbol taskIdSelf {
}
Symbol semTake {
}
Symbol ffs32Msb {
}
Symbol jobQueuePriorityMask {
}
Symbol jobQueueInit {
}
Symbol memset {
}
Symbol semBInit {
}
Symbol jobQueueCreate {
}
Symbol cacheArchAlignSize {
}
Symbol memalign {
}
Symbol free {
}
Symbol jobQueueTask {
}
Symbol jobQueueStdPoolInit {
}
Symbol jobQueueStdJobsAlloc {
}
Symbol _func_netDaemonIxToQId {
}
Symbol _func_netDaemonQIdToIx {
}
Symbol jobQueueProcessFunc {
}
Symbol jobQueueUtilInit {
}
Symbol jobQueueProcessWrapper {
}
Symbol jobQueueQuit {
}
Symbol longjmp {
}
Symbol setjmp {
}
Symbol jobQueueTerminate {
}
Symbol semTerminate {
}
Symbol jobQueueDelete {
}
Symbol jobQueueStdPoolJobsFree {
}
ObjectFile libjobqueue.a::jobQueueLib.o {
	NAME Object file jobQueueLib.o from archive libjobqueue.a
	EXPORTS jobQueueLibInit jobQueuePost jobQueueStdPost jobQueueProcess jobQueuePriorityMask jobQueueInit jobQueueCreate jobQueueTask jobQueueStdPoolInit jobQueueStdJobsAlloc jobQueueProcessFunc
	IMPORTS semGive taskIdSelf semTake ffs32Msb memset semBInit cacheArchAlignSize memalign free
	DECLARES _func_netDaemonIxToQId _func_netDaemonQIdToIx
	USES 
}
Module jobQueueLib.o {
	OBJECT += libjobqueue.a::jobQueueLib.o
}
ObjectFile libjobqueue.a::jobQueueUtilLib.o {
	NAME Object file jobQueueUtilLib.o from archive libjobqueue.a
	EXPORTS jobQueueUtilInit jobQueueProcessWrapper jobQueueQuit jobQueueTerminate jobQueueDelete jobQueueStdPoolJobsFree
	IMPORTS jobQueueProcessFunc jobQueuePost longjmp setjmp jobQueueProcess semTerminate free
	DECLARES 
	USES 
}
Module jobQueueUtilLib.o {
	OBJECT += libjobqueue.a::jobQueueUtilLib.o
}
Library libjobqueue.a {
	MODULES libjobqueue.a::jobQueueLib.o libjobqueue.a::jobQueueUtilLib.o
}
