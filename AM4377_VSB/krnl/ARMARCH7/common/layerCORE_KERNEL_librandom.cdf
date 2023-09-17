 
/* this file is generated */

Layer Layer::CORE_KERNEL {

LIBRARIES += librandom.a

OBJECTS librandom.a::randomNumGenLib.o librandom.a::randomSWNumGenLib.o librandom.a::randomScLib.o

}


ObjectFile librandom.a::randomNumGenLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objrandom/
}
 
ObjectFile librandom.a::randomSWNumGenLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objrandom/
}
 
ObjectFile librandom.a::randomScLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objrandom/
}



