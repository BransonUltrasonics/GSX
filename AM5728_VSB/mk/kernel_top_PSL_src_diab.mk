
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






LIBOBJS = $(LIBOBJS_pslTiAm3xxx)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_pslTiAm3xxx_kernel_top_PSL_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/am3xxx.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/am4xxx.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/am5xxx.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/vxbAm3Rtc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/vxbFdtTiAM572xPcie.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/vxbFdtTiClkAm4.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/vxbOmapGpioCtlr.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/vxbAm38xxI2c.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/vxbTiAm335xSpi.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/vxbTiAm3Sio.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/vxbFdtTiAm5Clk.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/vxbAm3Prcm.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/vxbOmap3IntCtlr.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/vxbOmap3Timer.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx/vxbAm3Pads.o 

arcmdfile_krnl_ARMARCH7_common_pslTiAm3xxx : $(LIBOBJS_krnl_ARMARCH7_common_pslTiAm3xxx_kernel_top_PSL_src_diab)

LIBOBJS_krnl_ARMARCH7_common_pslTiAm3xxx_PSL += $(LIBOBJS_krnl_ARMARCH7_common_pslTiAm3xxx_kernel_top_PSL_src_diab)

LIBOBJS_krnl_ARMARCH7_common_pslTiAm3xxx += $(LIBOBJS_krnl_ARMARCH7_common_pslTiAm3xxx_kernel_top_PSL_src_diab)

__OBJS_TO_CLEAN_PSL += $(LIBOBJS_krnl_ARMARCH7_common_pslTiAm3xxx_kernel_top_PSL_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_pslTiAm3xxx_kernel_top_PSL_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_pslTiAm3xxx

_DONEBASE_LIB_krnl_ARMARCH7_common_pslTiAm3xxx = TRUE

# LIBBASES += pslTiAm3xxx

__LIBS_TO_BUILD += krnl_ARMARCH7_common_pslTiAm3xxx

__LIBS_TO_BUILD_PSL += krnl_ARMARCH7_common_pslTiAm3xxx

__BUILT_LIBS += krnl_ARMARCH7_common_pslTiAm3xxx

__LIB_krnl_ARMARCH7_common_pslTiAm3xxx = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpslTiAm3xxx$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpslTiAm3xxx :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpslTiAm3xxx$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpslTiAm3xxx.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpslTiAm3xxx.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpslTiAm3xxx$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libpslTiAm3xxx$(OPT).a libpslTiAm3xxx$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpslTiAm3xxx.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpslTiAm3xxx.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libpslTiAm3xxx.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_pslTiAm3xxx

arcmdfile_krnl_ARMARCH7_common_pslTiAm3xxx :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_pslTiAm3xxx))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpslTiAm3xxx$(OPT).a : arcmdfile_krnl_ARMARCH7_common_pslTiAm3xxx
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_pslTiAm3xxx


clean _layer_clean_PSL : _clean_LIB_BASE_krnl_ARMARCH7_common_pslTiAm3xxx

_clean_LIB_BASE_krnl_ARMARCH7_common_pslTiAm3xxx :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpslTiAm3xxx$(OPT).a


endif

ifndef _DONEBASE_LIB_PSL_krnl_ARMARCH7_common_pslTiAm3xxx

_DONEBASE_LIB_PSL_krnl_ARMARCH7_common_pslTiAm3xxx = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerPSL_libpslTiAm3xxx.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerPSL_libpslTiAm3xxx.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpslTiAm3xxx$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,PSL,libpslTiAm3xxx.a,$(LIBOBJS_krnl_ARMARCH7_common_pslTiAm3xxx)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpslTiAm3xxx$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_pslTiAm3xxx)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
