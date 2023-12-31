
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






LIBOBJS = $(LIBOBJS_vxsnmp)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_vxsnmp_kernel_top_SNMP_AGENT_src_diab_ttmib =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxsnmp/ttmth.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxsnmp/ttmib.o 

arcmdfile_krnl_ARMARCH7_common_vxsnmp : $(LIBOBJS_krnl_ARMARCH7_common_vxsnmp_kernel_top_SNMP_AGENT_src_diab_ttmib)

LIBOBJS_krnl_ARMARCH7_common_vxsnmp_SNMP_AGENT += $(LIBOBJS_krnl_ARMARCH7_common_vxsnmp_kernel_top_SNMP_AGENT_src_diab_ttmib)

LIBOBJS_krnl_ARMARCH7_common_vxsnmp += $(LIBOBJS_krnl_ARMARCH7_common_vxsnmp_kernel_top_SNMP_AGENT_src_diab_ttmib)

__OBJS_TO_CLEAN_SNMP_AGENT += $(LIBOBJS_krnl_ARMARCH7_common_vxsnmp_kernel_top_SNMP_AGENT_src_diab_ttmib) $(LIBOBJS_krnl_ARMARCH7_common_vxsnmp_kernel_top_SNMP_AGENT_src_diab_ttmib:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_vxsnmp

_DONEBASE_LIB_krnl_ARMARCH7_common_vxsnmp = TRUE

# LIBBASES += vxsnmp

__LIBS_TO_BUILD += krnl_ARMARCH7_common_vxsnmp

__LIBS_TO_BUILD_SNMP_AGENT += krnl_ARMARCH7_common_vxsnmp

__BUILT_LIBS += krnl_ARMARCH7_common_vxsnmp

__LIB_krnl_ARMARCH7_common_vxsnmp = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxsnmp$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxsnmp :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxsnmp$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxsnmp.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxsnmp.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxsnmp$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libvxsnmp$(OPT).a libvxsnmp$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxsnmp.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxsnmp.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libvxsnmp.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_vxsnmp

arcmdfile_krnl_ARMARCH7_common_vxsnmp :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_vxsnmp))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxsnmp$(OPT).a : arcmdfile_krnl_ARMARCH7_common_vxsnmp
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_vxsnmp


clean _layer_clean_SNMP_AGENT : _clean_LIB_BASE_krnl_ARMARCH7_common_vxsnmp

_clean_LIB_BASE_krnl_ARMARCH7_common_vxsnmp :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxsnmp$(OPT).a


endif

ifndef _DONEBASE_LIB_SNMP_AGENT_krnl_ARMARCH7_common_vxsnmp

_DONEBASE_LIB_SNMP_AGENT_krnl_ARMARCH7_common_vxsnmp = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerSNMP_AGENT_libvxsnmp.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerSNMP_AGENT_libvxsnmp.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxsnmp$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,SNMP_AGENT,libvxsnmp.a,$(LIBOBJS_krnl_ARMARCH7_common_vxsnmp)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxsnmp$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_vxsnmp)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
