
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






LIBOBJS = $(LIBOBJS_DHCPC)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_DHCPC_kernel_top_IPNET_DHCPC_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objDHCPC/ipdhcpc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objDHCPC/ipdhcpc_cmd_dhcpc.o 

arcmdfile_krnl_ARMARCH7_common_DHCPC : $(LIBOBJS_krnl_ARMARCH7_common_DHCPC_kernel_top_IPNET_DHCPC_src_diab)

LIBOBJS_krnl_ARMARCH7_common_DHCPC_IPNET_DHCPC += $(LIBOBJS_krnl_ARMARCH7_common_DHCPC_kernel_top_IPNET_DHCPC_src_diab)

LIBOBJS_krnl_ARMARCH7_common_DHCPC += $(LIBOBJS_krnl_ARMARCH7_common_DHCPC_kernel_top_IPNET_DHCPC_src_diab)

__OBJS_TO_CLEAN_IPNET_DHCPC += $(LIBOBJS_krnl_ARMARCH7_common_DHCPC_kernel_top_IPNET_DHCPC_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_DHCPC_kernel_top_IPNET_DHCPC_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_DHCPC

_DONEBASE_LIB_krnl_ARMARCH7_common_DHCPC = TRUE

# LIBBASES += DHCPC

__LIBS_TO_BUILD += krnl_ARMARCH7_common_DHCPC

__LIBS_TO_BUILD_IPNET_DHCPC += krnl_ARMARCH7_common_DHCPC

__BUILT_LIBS += krnl_ARMARCH7_common_DHCPC

__LIB_krnl_ARMARCH7_common_DHCPC = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libDHCPC$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objDHCPC :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libDHCPC$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libDHCPC.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libDHCPC.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libDHCPC$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common && $(NM) libDHCPC$(OPT).a libDHCPC$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libDHCPC.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libDHCPC.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libDHCPC.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_DHCPC

arcmdfile_krnl_ARMARCH7_common_DHCPC :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_DHCPC))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libDHCPC$(OPT).a : arcmdfile_krnl_ARMARCH7_common_DHCPC
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_DHCPC


clean _layer_clean_IPNET_DHCPC : _clean_LIB_BASE_krnl_ARMARCH7_common_DHCPC

_clean_LIB_BASE_krnl_ARMARCH7_common_DHCPC :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libDHCPC$(OPT).a


endif

ifndef _DONEBASE_LIB_IPNET_DHCPC_krnl_ARMARCH7_common_DHCPC

_DONEBASE_LIB_IPNET_DHCPC_krnl_ARMARCH7_common_DHCPC = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerIPNET_DHCPC_libDHCPC.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerIPNET_DHCPC_libDHCPC.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libDHCPC$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,IPNET_DHCPC,libDHCPC.a,$(LIBOBJS_krnl_ARMARCH7_common_DHCPC)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libDHCPC$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_DHCPC)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 