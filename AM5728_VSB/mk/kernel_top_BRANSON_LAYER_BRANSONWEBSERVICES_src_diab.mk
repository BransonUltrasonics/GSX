
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






LIBOBJS = $(LIBOBJS_webclidemo)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_webclidemo_kernel_top_BRANSON_LAYER_BRANSONWEBSERVICES_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/rcc_handler.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/sampleHttpUserFuncs.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/sampleJSUser.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/sampleLiveCtrlApp.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/sampleDynamicTable.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/sampleSmtpApp.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/sampleRamDisk.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/sampleUserCgi.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/sampleDevice.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/sampleCheckbox.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/sampleUserRpm.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/wmw_httpconf.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/webCliCommonInit.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/webCliHttpInit.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/webCliCliInit.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/wmc_webclidemo.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/wmb_webclidemo.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo/wm_filesys.o 

arcmdfile_krnl_ARMARCH7_common_webclidemo : $(LIBOBJS_krnl_ARMARCH7_common_webclidemo_kernel_top_BRANSON_LAYER_BRANSONWEBSERVICES_src_diab)

LIBOBJS_krnl_ARMARCH7_common_webclidemo_BRANSON_LAYER_BRANSONWEBSERVICES += $(LIBOBJS_krnl_ARMARCH7_common_webclidemo_kernel_top_BRANSON_LAYER_BRANSONWEBSERVICES_src_diab)

LIBOBJS_krnl_ARMARCH7_common_webclidemo += $(LIBOBJS_krnl_ARMARCH7_common_webclidemo_kernel_top_BRANSON_LAYER_BRANSONWEBSERVICES_src_diab)

__OBJS_TO_CLEAN_BRANSON_LAYER_BRANSONWEBSERVICES += $(LIBOBJS_krnl_ARMARCH7_common_webclidemo_kernel_top_BRANSON_LAYER_BRANSONWEBSERVICES_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_webclidemo_kernel_top_BRANSON_LAYER_BRANSONWEBSERVICES_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_webclidemo

_DONEBASE_LIB_krnl_ARMARCH7_common_webclidemo = TRUE

# LIBBASES += webclidemo

__LIBS_TO_BUILD += krnl_ARMARCH7_common_webclidemo

__LIBS_TO_BUILD_BRANSON_LAYER_BRANSONWEBSERVICES += krnl_ARMARCH7_common_webclidemo

__BUILT_LIBS += krnl_ARMARCH7_common_webclidemo

__LIB_krnl_ARMARCH7_common_webclidemo = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libwebclidemo$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objwebclidemo :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libwebclidemo$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libwebclidemo.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libwebclidemo.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libwebclidemo$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libwebclidemo$(OPT).a libwebclidemo$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libwebclidemo.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libwebclidemo.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libwebclidemo.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_webclidemo

arcmdfile_krnl_ARMARCH7_common_webclidemo :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_webclidemo))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libwebclidemo$(OPT).a : arcmdfile_krnl_ARMARCH7_common_webclidemo
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_webclidemo


clean _layer_clean_BRANSON_LAYER_BRANSONWEBSERVICES : _clean_LIB_BASE_krnl_ARMARCH7_common_webclidemo

_clean_LIB_BASE_krnl_ARMARCH7_common_webclidemo :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libwebclidemo$(OPT).a


endif

ifndef _DONEBASE_LIB_BRANSON_LAYER_BRANSONWEBSERVICES_krnl_ARMARCH7_common_webclidemo

_DONEBASE_LIB_BRANSON_LAYER_BRANSONWEBSERVICES_krnl_ARMARCH7_common_webclidemo = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerBRANSON_LAYER_BRANSONWEBSERVICES_libwebclidemo.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerBRANSON_LAYER_BRANSONWEBSERVICES_libwebclidemo.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libwebclidemo$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,BRANSON_LAYER_BRANSONWEBSERVICES,libwebclidemo.a,$(LIBOBJS_krnl_ARMARCH7_common_webclidemo)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libwebclidemo$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_webclidemo)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
