 
/* this file is generated */

Layer Layer::CORE_KERNEL {

LIBRARIES += libspl.a

OBJECTS libspl.a::kernelLockLib.o libspl.a::spinLockIsrNdLib.o libspl.a::spinLockSmpLib.o libspl.a::spinLockNdTimedLib.o

}


ObjectFile libspl.a::kernelLockLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objspl/
}
 
ObjectFile libspl.a::spinLockIsrNdLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objspl/
}
 
ObjectFile libspl.a::spinLockSmpLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objspl/
}
 
ObjectFile libspl.a::spinLockNdTimedLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objspl/
}



