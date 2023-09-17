Symbol atexitInit {
}
Symbol semMCreate {
}
Symbol semTake {
}
Symbol malloc {
}
Symbol semGive {
}
Symbol free {
}
Symbol __cxa_atexit {
}
Symbol __cxa_finalize {
}
Symbol __do_atexit_work {
}
Symbol __arm_call_via_o {
}
Symbol __common_intrinsicsInit {
}
ObjectFile libcommoncc.a::cxa_atexit.o {
	NAME Object file cxa_atexit.o from archive libcommoncc.a
	EXPORTS atexitInit __cxa_atexit __cxa_finalize __do_atexit_work
	IMPORTS semMCreate semTake malloc semGive free
	DECLARES 
	USES 
}
Module cxa_atexit.o {
	OBJECT += libcommoncc.a::cxa_atexit.o
}
ObjectFile libcommoncc.a::arm_call_via.o {
	NAME Object file arm_call_via.o from archive libcommoncc.a
	EXPORTS __arm_call_via_o
	IMPORTS 
	DECLARES 
	USES 
}
Module arm_call_via.o {
	OBJECT += libcommoncc.a::arm_call_via.o
}
ObjectFile libcommoncc.a::__common_intrinsics.o {
	NAME Object file __common_intrinsics.o from archive libcommoncc.a
	EXPORTS __common_intrinsicsInit
	IMPORTS __arm_call_via_o
	DECLARES 
	USES 
}
Module __common_intrinsics.o {
	OBJECT += libcommoncc.a::__common_intrinsics.o
}
Library libcommoncc.a {
	MODULES libcommoncc.a::cxa_atexit.o libcommoncc.a::arm_call_via.o libcommoncc.a::__common_intrinsics.o
}
