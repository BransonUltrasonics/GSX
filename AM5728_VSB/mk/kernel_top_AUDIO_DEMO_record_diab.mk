
#
# This file is automatically generated by mk/krnl/defs.fastlibgen.mk .
#
# build configuration :
#	VSB : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB
#	CPU : ARMARCH7
#	TOOL : diab
#	FP : vector
# 	ENDIAN : little
#	LIB_DIR_TAG = 
#	LIBDIRBASE = krnl/ARMARCH7/common
#	LIBDIRBASE = krnl/$(CPU)$(CPU_OPTION_SUFFIX)/$(TOOL_COMMON_DIR)$(LIB_DIR_TAG)$(MINOR_OPTION_SUFFIX)






LIBOBJS = $(LIBOBJS_audioDemoRecord)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_audioDemoRecord_kernel_top_AUDIO_DEMO_record_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objaudioDemoRecord/audioDemoRecord.o 

arcmdfile_krnl_ARMARCH7_common_audioDemoRecord : $(LIBOBJS_krnl_ARMARCH7_common_audioDemoRecord_kernel_top_AUDIO_DEMO_record_diab)

LIBOBJS_krnl_ARMARCH7_common_audioDemoRecord_AUDIO_DEMO += $(LIBOBJS_krnl_ARMARCH7_common_audioDemoRecord_kernel_top_AUDIO_DEMO_record_diab)

LIBOBJS_krnl_ARMARCH7_common_audioDemoRecord += $(LIBOBJS_krnl_ARMARCH7_common_audioDemoRecord_kernel_top_AUDIO_DEMO_record_diab)

__OBJS_TO_CLEAN_AUDIO_DEMO += $(LIBOBJS_krnl_ARMARCH7_common_audioDemoRecord_kernel_top_AUDIO_DEMO_record_diab) $(LIBOBJS_krnl_ARMARCH7_common_audioDemoRecord_kernel_top_AUDIO_DEMO_record_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_audioDemoRecord

_DONEBASE_LIB_krnl_ARMARCH7_common_audioDemoRecord = TRUE

# LIBBASES += audioDemoRecord

__LIBS_TO_BUILD += krnl_ARMARCH7_common_audioDemoRecord

__LIBS_TO_BUILD_AUDIO_DEMO += krnl_ARMARCH7_common_audioDemoRecord

__BUILT_LIBS += krnl_ARMARCH7_common_audioDemoRecord

__LIB_krnl_ARMARCH7_common_audioDemoRecord = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libaudioDemoRecord$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objaudioDemoRecord :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libaudioDemoRecord$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libaudioDemoRecord.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libaudioDemoRecord.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libaudioDemoRecord$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libaudioDemoRecord$(OPT).a libaudioDemoRecord$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libaudioDemoRecord.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libaudioDemoRecord.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libaudioDemoRecord.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_audioDemoRecord

arcmdfile_krnl_ARMARCH7_common_audioDemoRecord :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_audioDemoRecord))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libaudioDemoRecord$(OPT).a : arcmdfile_krnl_ARMARCH7_common_audioDemoRecord
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_audioDemoRecord


clean _layer_clean_AUDIO_DEMO : _clean_LIB_BASE_krnl_ARMARCH7_common_audioDemoRecord

_clean_LIB_BASE_krnl_ARMARCH7_common_audioDemoRecord :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libaudioDemoRecord$(OPT).a


endif

ifndef _DONEBASE_LIB_AUDIO_DEMO_krnl_ARMARCH7_common_audioDemoRecord

_DONEBASE_LIB_AUDIO_DEMO_krnl_ARMARCH7_common_audioDemoRecord = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerAUDIO_DEMO_libaudioDemoRecord.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerAUDIO_DEMO_libaudioDemoRecord.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libaudioDemoRecord$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,AUDIO_DEMO,libaudioDemoRecord.a,$(LIBOBJS_krnl_ARMARCH7_common_audioDemoRecord)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libaudioDemoRecord$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_audioDemoRecord)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
