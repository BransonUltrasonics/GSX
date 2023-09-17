 
/* this file is generated */

Layer Layer::CORE_KERNEL {

LIBRARIES += libspl.a

OBJECTS libspl.a::spinLockUpLib.o libspl.a::spinLockIsrNdLib.o libspl.a::spinLockNdTimedLib.o

}


ObjectFile libspl.a::spinLockUpLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objspl/
}
 
ObjectFile libspl.a::spinLockIsrNdLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objspl/
}
 
ObjectFile libspl.a::spinLockNdTimedLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objspl/
}



