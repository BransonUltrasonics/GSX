
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






LIBOBJS = $(LIBOBJS_tcfagent)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_tcfagent_kernel_top_DEBUG_AGENT_hostfs_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objtcfagent/hostFsLib.o 

arcmdfile_krnl_ARMARCH7_common_tcfagent : $(LIBOBJS_krnl_ARMARCH7_common_tcfagent_kernel_top_DEBUG_AGENT_hostfs_src_diab)

LIBOBJS_krnl_ARMARCH7_common_tcfagent_DEBUG_AGENT += $(LIBOBJS_krnl_ARMARCH7_common_tcfagent_kernel_top_DEBUG_AGENT_hostfs_src_diab)

LIBOBJS_krnl_ARMARCH7_common_tcfagent += $(LIBOBJS_krnl_ARMARCH7_common_tcfagent_kernel_top_DEBUG_AGENT_hostfs_src_diab)

__OBJS_TO_CLEAN_DEBUG_AGENT += $(LIBOBJS_krnl_ARMARCH7_common_tcfagent_kernel_top_DEBUG_AGENT_hostfs_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_tcfagent_kernel_top_DEBUG_AGENT_hostfs_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_tcfagent

_DONEBASE_LIB_krnl_ARMARCH7_common_tcfagent = TRUE

# LIBBASES += tcfagent

__LIBS_TO_BUILD += krnl_ARMARCH7_common_tcfagent

__LIBS_TO_BUILD_DEBUG_AGENT += krnl_ARMARCH7_common_tcfagent

__BUILT_LIBS += krnl_ARMARCH7_common_tcfagent

__LIB_krnl_ARMARCH7_common_tcfagent = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libtcfagent$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objtcfagent :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libtcfagent$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libtcfagent.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libtcfagent.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libtcfagent$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common && $(NM) libtcfagent$(OPT).a libtcfagent$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libtcfagent.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libtcfagent.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libtcfagent.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_tcfagent

arcmdfile_krnl_ARMARCH7_common_tcfagent :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_tcfagent))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libtcfagent$(OPT).a : arcmdfile_krnl_ARMARCH7_common_tcfagent
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_tcfagent


clean _layer_clean_DEBUG_AGENT : _clean_LIB_BASE_krnl_ARMARCH7_common_tcfagent

_clean_LIB_BASE_krnl_ARMARCH7_common_tcfagent :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libtcfagent$(OPT).a


endif

ifndef _DONEBASE_LIB_DEBUG_AGENT_krnl_ARMARCH7_common_tcfagent

_DONEBASE_LIB_DEBUG_AGENT_krnl_ARMARCH7_common_tcfagent = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerDEBUG_AGENT_libtcfagent.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerDEBUG_AGENT_libtcfagent.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libtcfagent$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,DEBUG_AGENT,libtcfagent.a,$(LIBOBJS_krnl_ARMARCH7_common_tcfagent)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libtcfagent$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_tcfagent)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
