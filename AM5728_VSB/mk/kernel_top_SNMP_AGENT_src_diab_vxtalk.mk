
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






LIBOBJS = $(LIBOBJS_vxtalk)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_vxtalk_kernel_top_SNMP_AGENT_src_diab_vxtalk =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/arp.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/drt.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/ifstat.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/key_chg.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/mibutils.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/nprint.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/readmib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/snmpint.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/snmptalk.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/sockets.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/stdf.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/ttmth.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/strdup.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/snmpconf.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk/parse.o 

arcmdfile_krnl_ARMARCH7_common_vxtalk : $(LIBOBJS_krnl_ARMARCH7_common_vxtalk_kernel_top_SNMP_AGENT_src_diab_vxtalk)

LIBOBJS_krnl_ARMARCH7_common_vxtalk_SNMP_AGENT += $(LIBOBJS_krnl_ARMARCH7_common_vxtalk_kernel_top_SNMP_AGENT_src_diab_vxtalk)

LIBOBJS_krnl_ARMARCH7_common_vxtalk += $(LIBOBJS_krnl_ARMARCH7_common_vxtalk_kernel_top_SNMP_AGENT_src_diab_vxtalk)

__OBJS_TO_CLEAN_SNMP_AGENT += $(LIBOBJS_krnl_ARMARCH7_common_vxtalk_kernel_top_SNMP_AGENT_src_diab_vxtalk) $(LIBOBJS_krnl_ARMARCH7_common_vxtalk_kernel_top_SNMP_AGENT_src_diab_vxtalk:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_vxtalk

_DONEBASE_LIB_krnl_ARMARCH7_common_vxtalk = TRUE

# LIBBASES += vxtalk

__LIBS_TO_BUILD += krnl_ARMARCH7_common_vxtalk

__LIBS_TO_BUILD_SNMP_AGENT += krnl_ARMARCH7_common_vxtalk

__BUILT_LIBS += krnl_ARMARCH7_common_vxtalk

__LIB_krnl_ARMARCH7_common_vxtalk = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxtalk$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxtalk :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxtalk$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxtalk.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxtalk.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxtalk$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libvxtalk$(OPT).a libvxtalk$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxtalk.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxtalk.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libvxtalk.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_vxtalk

arcmdfile_krnl_ARMARCH7_common_vxtalk :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_vxtalk))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxtalk$(OPT).a : arcmdfile_krnl_ARMARCH7_common_vxtalk
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_vxtalk


clean _layer_clean_SNMP_AGENT : _clean_LIB_BASE_krnl_ARMARCH7_common_vxtalk

_clean_LIB_BASE_krnl_ARMARCH7_common_vxtalk :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxtalk$(OPT).a


endif

ifndef _DONEBASE_LIB_SNMP_AGENT_krnl_ARMARCH7_common_vxtalk

_DONEBASE_LIB_SNMP_AGENT_krnl_ARMARCH7_common_vxtalk = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerSNMP_AGENT_libvxtalk.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerSNMP_AGENT_libvxtalk.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxtalk$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,SNMP_AGENT,libvxtalk.a,$(LIBOBJS_krnl_ARMARCH7_common_vxtalk)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxtalk$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_vxtalk)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
