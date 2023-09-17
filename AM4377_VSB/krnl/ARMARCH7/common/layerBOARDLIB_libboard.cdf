 
/* this file is generated */

Layer Layer::BOARDLIB {

LIBRARIES += libboard.a

OBJECTS libboard.a::boardLib.o libboard.a::warmBootLib.o

}


ObjectFile libboard.a::boardLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objboard/
}
 
ObjectFile libboard.a::warmBootLib.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objboard/
}



