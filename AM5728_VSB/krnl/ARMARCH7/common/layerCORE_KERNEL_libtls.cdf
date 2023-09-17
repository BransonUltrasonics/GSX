 
/* this file is generated */

Layer Layer::CORE_KERNEL {

LIBRARIES += libtls.a

OBJECTS libtls.a::tlsLib.o libtls.a::tlsLookupLib.o

}


ObjectFile libtls.a::tlsLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtls/
}
 
ObjectFile libtls.a::tlsLookupLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtls/
}



