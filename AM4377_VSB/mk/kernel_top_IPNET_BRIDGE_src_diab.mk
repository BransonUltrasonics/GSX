
#
# This file is automatically generated by mk/krnl/defs.fastlibgen.mk .
#
# build configuration :
#	VSB : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB
#	CPU : ARMARCH7
#	TOOL : diab
#	FP : vector
# 	ENDIAN : little
#	LIB_DIR_TAG = 
#	LIBDIRBASE = krnl/ARMARCH7/common
#	LIBDIRBASE = krnl/$(CPU)$(CPU_OPTION_SUFFIX)/$(TOOL_COMMON_DIR)$(LIB_DIR_TAG)$(MINOR_OPTION_SUFFIX)






LIBOBJS = $(LIBOBJS_BRIDGE)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_BRIDGE_kernel_top_IPNET_BRIDGE_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objBRIDGE/bridge.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objBRIDGE/mirrorEnd.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objBRIDGE/mirrorUtils.o 

arcmdfile_krnl_ARMARCH7_common_BRIDGE : $(LIBOBJS_krnl_ARMARCH7_common_BRIDGE_kernel_top_IPNET_BRIDGE_src_diab)

LIBOBJS_krnl_ARMARCH7_common_BRIDGE_IPNET_BRIDGE += $(LIBOBJS_krnl_ARMARCH7_common_BRIDGE_kernel_top_IPNET_BRIDGE_src_diab)

LIBOBJS_krnl_ARMARCH7_common_BRIDGE += $(LIBOBJS_krnl_ARMARCH7_common_BRIDGE_kernel_top_IPNET_BRIDGE_src_diab)

__OBJS_TO_CLEAN_IPNET_BRIDGE += $(LIBOBJS_krnl_ARMARCH7_common_BRIDGE_kernel_top_IPNET_BRIDGE_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_BRIDGE_kernel_top_IPNET_BRIDGE_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_BRIDGE

_DONEBASE_LIB_krnl_ARMARCH7_common_BRIDGE = TRUE

# LIBBASES += BRIDGE

__LIBS_TO_BUILD += krnl_ARMARCH7_common_BRIDGE

__LIBS_TO_BUILD_IPNET_BRIDGE += krnl_ARMARCH7_common_BRIDGE

__BUILT_LIBS += krnl_ARMARCH7_common_BRIDGE

__LIB_krnl_ARMARCH7_common_BRIDGE = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libBRIDGE$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objBRIDGE :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libBRIDGE$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libBRIDGE.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libBRIDGE.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libBRIDGE$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common && $(NM) libBRIDGE$(OPT).a libBRIDGE$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libBRIDGE.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libBRIDGE.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libBRIDGE.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_BRIDGE

arcmdfile_krnl_ARMARCH7_common_BRIDGE :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_BRIDGE))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libBRIDGE$(OPT).a : arcmdfile_krnl_ARMARCH7_common_BRIDGE
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_BRIDGE


clean _layer_clean_IPNET_BRIDGE : _clean_LIB_BASE_krnl_ARMARCH7_common_BRIDGE

_clean_LIB_BASE_krnl_ARMARCH7_common_BRIDGE :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libBRIDGE$(OPT).a


endif

ifndef _DONEBASE_LIB_IPNET_BRIDGE_krnl_ARMARCH7_common_BRIDGE

_DONEBASE_LIB_IPNET_BRIDGE_krnl_ARMARCH7_common_BRIDGE = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerIPNET_BRIDGE_libBRIDGE.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerIPNET_BRIDGE_libBRIDGE.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libBRIDGE$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,IPNET_BRIDGE,libBRIDGE.a,$(LIBOBJS_krnl_ARMARCH7_common_BRIDGE)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libBRIDGE$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_BRIDGE)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
