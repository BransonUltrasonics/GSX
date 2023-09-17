Symbol slots {
}
Symbol __rem32 {
}
Symbol malloc {
}
Symbol free {
}
Symbol vxAtomicSet {
}
Symbol vxAtomicGet {
}
Symbol memset {
}
Symbol sysClkRateGet {
}
Symbol __div32 {
}
Symbol taskDelay {
}
Symbol cafe_mem_demo_start {
}
Symbol __stderr {
}
Symbol fputs {
}
Symbol taskSpawn {
}
Symbol cafe_mem_demo_stop {
}
ObjectFile libruntime_analysis_memdemo.a::cafe_mem_demo.o {
	NAME Object file cafe_mem_demo.o from archive libruntime_analysis_memdemo.a
	EXPORTS cafe_mem_demo_start cafe_mem_demo_stop
	IMPORTS __rem32 malloc free vxAtomicSet vxAtomicGet memset sysClkRateGet __div32 taskDelay __stderr fputs taskSpawn
	DECLARES slots
	USES 
}
Module cafe_mem_demo.o {
	OBJECT += libruntime_analysis_memdemo.a::cafe_mem_demo.o
}
Library libruntime_analysis_memdemo.a {
	MODULES libruntime_analysis_memdemo.a::cafe_mem_demo.o
}
