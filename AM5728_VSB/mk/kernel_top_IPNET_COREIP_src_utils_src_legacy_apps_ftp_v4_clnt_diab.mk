
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






LIBOBJS = $(LIBOBJS_VXCOREIP)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_VXCOREIP_kernel_top_IPNET_COREIP_src_utils_src_legacy_apps_ftp_v4_clnt_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objVXCOREIP/ftpLib.o 

arcmdfile_krnl_ARMARCH7_common_VXCOREIP : $(LIBOBJS_krnl_ARMARCH7_common_VXCOREIP_kernel_top_IPNET_COREIP_src_utils_src_legacy_apps_ftp_v4_clnt_diab)

LIBOBJS_krnl_ARMARCH7_common_VXCOREIP_IPNET_COREIP += $(LIBOBJS_krnl_ARMARCH7_common_VXCOREIP_kernel_top_IPNET_COREIP_src_utils_src_legacy_apps_ftp_v4_clnt_diab)

LIBOBJS_krnl_ARMARCH7_common_VXCOREIP += $(LIBOBJS_krnl_ARMARCH7_common_VXCOREIP_kernel_top_IPNET_COREIP_src_utils_src_legacy_apps_ftp_v4_clnt_diab)

__OBJS_TO_CLEAN_IPNET_COREIP += $(LIBOBJS_krnl_ARMARCH7_common_VXCOREIP_kernel_top_IPNET_COREIP_src_utils_src_legacy_apps_ftp_v4_clnt_diab) $(LIBOBJS_krnl_ARMARCH7_common_VXCOREIP_kernel_top_IPNET_COREIP_src_utils_src_legacy_apps_ftp_v4_clnt_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_VXCOREIP

_DONEBASE_LIB_krnl_ARMARCH7_common_VXCOREIP = TRUE

# LIBBASES += VXCOREIP

__LIBS_TO_BUILD += krnl_ARMARCH7_common_VXCOREIP

__LIBS_TO_BUILD_IPNET_COREIP += krnl_ARMARCH7_common_VXCOREIP

__BUILT_LIBS += krnl_ARMARCH7_common_VXCOREIP

__LIB_krnl_ARMARCH7_common_VXCOREIP = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libVXCOREIP$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objVXCOREIP :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libVXCOREIP$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libVXCOREIP.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libVXCOREIP.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libVXCOREIP$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libVXCOREIP$(OPT).a libVXCOREIP$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libVXCOREIP.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libVXCOREIP.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libVXCOREIP.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_VXCOREIP

arcmdfile_krnl_ARMARCH7_common_VXCOREIP :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_VXCOREIP))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libVXCOREIP$(OPT).a : arcmdfile_krnl_ARMARCH7_common_VXCOREIP
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_VXCOREIP


clean _layer_clean_IPNET_COREIP : _clean_LIB_BASE_krnl_ARMARCH7_common_VXCOREIP

_clean_LIB_BASE_krnl_ARMARCH7_common_VXCOREIP :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libVXCOREIP$(OPT).a


endif

ifndef _DONEBASE_LIB_IPNET_COREIP_krnl_ARMARCH7_common_VXCOREIP

_DONEBASE_LIB_IPNET_COREIP_krnl_ARMARCH7_common_VXCOREIP = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerIPNET_COREIP_libVXCOREIP.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerIPNET_COREIP_libVXCOREIP.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libVXCOREIP$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,IPNET_COREIP,libVXCOREIP.a,$(LIBOBJS_krnl_ARMARCH7_common_VXCOREIP)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libVXCOREIP$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_VXCOREIP)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
