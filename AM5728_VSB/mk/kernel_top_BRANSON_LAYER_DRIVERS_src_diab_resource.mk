
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






LIBOBJS = $(LIBOBJS_BRANSON_DRV)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_BRANSON_DRV_kernel_top_BRANSON_LAYER_DRIVERS_src_diab_resource =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objBRANSON_DRV/vxbLed.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objBRANSON_DRV/vxbMdioLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objBRANSON_DRV/vxbMcSpiLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objBRANSON_DRV/vxbCdce913Lib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objBRANSON_DRV/vxbEhrPwmLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objBRANSON_DRV/vxbPwmssLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objBRANSON_DRV/vxbTps62362Lib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objBRANSON_DRV/vxbEqepLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objBRANSON_DRV/ecatSlavePalLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objBRANSON_DRV/vxbPru.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objBRANSON_DRV/vxbAm5728EhrPwmLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objBRANSON_DRV/vxbAm5728PwmssLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objBRANSON_DRV/vxbSp25QspiFlash.o 

arcmdfile_krnl_ARMARCH7_common_BRANSON_DRV : $(LIBOBJS_krnl_ARMARCH7_common_BRANSON_DRV_kernel_top_BRANSON_LAYER_DRIVERS_src_diab_resource)

LIBOBJS_krnl_ARMARCH7_common_BRANSON_DRV_BRANSON_LAYER_DRIVERS += $(LIBOBJS_krnl_ARMARCH7_common_BRANSON_DRV_kernel_top_BRANSON_LAYER_DRIVERS_src_diab_resource)

LIBOBJS_krnl_ARMARCH7_common_BRANSON_DRV += $(LIBOBJS_krnl_ARMARCH7_common_BRANSON_DRV_kernel_top_BRANSON_LAYER_DRIVERS_src_diab_resource)

__OBJS_TO_CLEAN_BRANSON_LAYER_DRIVERS += $(LIBOBJS_krnl_ARMARCH7_common_BRANSON_DRV_kernel_top_BRANSON_LAYER_DRIVERS_src_diab_resource) $(LIBOBJS_krnl_ARMARCH7_common_BRANSON_DRV_kernel_top_BRANSON_LAYER_DRIVERS_src_diab_resource:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_BRANSON_DRV

_DONEBASE_LIB_krnl_ARMARCH7_common_BRANSON_DRV = TRUE

# LIBBASES += BRANSON_DRV

__LIBS_TO_BUILD += krnl_ARMARCH7_common_BRANSON_DRV

__LIBS_TO_BUILD_BRANSON_LAYER_DRIVERS += krnl_ARMARCH7_common_BRANSON_DRV

__BUILT_LIBS += krnl_ARMARCH7_common_BRANSON_DRV

__LIB_krnl_ARMARCH7_common_BRANSON_DRV = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libBRANSON_DRV$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objBRANSON_DRV :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libBRANSON_DRV$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libBRANSON_DRV.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libBRANSON_DRV.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libBRANSON_DRV$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libBRANSON_DRV$(OPT).a libBRANSON_DRV$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libBRANSON_DRV.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libBRANSON_DRV.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libBRANSON_DRV.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_BRANSON_DRV

arcmdfile_krnl_ARMARCH7_common_BRANSON_DRV :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_BRANSON_DRV))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libBRANSON_DRV$(OPT).a : arcmdfile_krnl_ARMARCH7_common_BRANSON_DRV
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_BRANSON_DRV


clean _layer_clean_BRANSON_LAYER_DRIVERS : _clean_LIB_BASE_krnl_ARMARCH7_common_BRANSON_DRV

_clean_LIB_BASE_krnl_ARMARCH7_common_BRANSON_DRV :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libBRANSON_DRV$(OPT).a


endif

ifndef _DONEBASE_LIB_BRANSON_LAYER_DRIVERS_krnl_ARMARCH7_common_BRANSON_DRV

_DONEBASE_LIB_BRANSON_LAYER_DRIVERS_krnl_ARMARCH7_common_BRANSON_DRV = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerBRANSON_LAYER_DRIVERS_libBRANSON_DRV.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerBRANSON_LAYER_DRIVERS_libBRANSON_DRV.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libBRANSON_DRV$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,BRANSON_LAYER_DRIVERS,libBRANSON_DRV.a,$(LIBOBJS_krnl_ARMARCH7_common_BRANSON_DRV)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libBRANSON_DRV$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_BRANSON_DRV)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
