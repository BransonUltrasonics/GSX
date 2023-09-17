 
/* this file is generated */

Layer Layer::AUDIO_LIB {

LIBRARIES += libaudioLib.a

OBJECTS libaudioLib.a::audioLibCore.o libaudioLib.a::audioLibWav.o

}


ObjectFile libaudioLib.a::audioLibCore.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objaudioLib/
}
 
ObjectFile libaudioLib.a::audioLibWav.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objaudioLib/
}



