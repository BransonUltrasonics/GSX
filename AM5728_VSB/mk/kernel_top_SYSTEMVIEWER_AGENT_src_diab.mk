
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






LIBOBJS = $(LIBOBJS_tcfsvservice)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_tcfsvservice_kernel_top_SYSTEMVIEWER_AGENT_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/uploadmode.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/um_postmortem_pm.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/sv_upload_file.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/tipc_events.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/sv_upload_tcf.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/usb_events.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/um_deferred.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/logging_events.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/um_postmortem.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/net_events.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/rtp_events.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/sv_objinfo.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/logging.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/sv_upload.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/lib_events.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/sv_events.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/um_continuous.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/sysviewer.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/buf_stat.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/sv_symbol.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/wol_events.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice/stack_trace.o 

arcmdfile_krnl_ARMARCH7_common_tcfsvservice : $(LIBOBJS_krnl_ARMARCH7_common_tcfsvservice_kernel_top_SYSTEMVIEWER_AGENT_src_diab)

LIBOBJS_krnl_ARMARCH7_common_tcfsvservice_SYSTEMVIEWER_AGENT += $(LIBOBJS_krnl_ARMARCH7_common_tcfsvservice_kernel_top_SYSTEMVIEWER_AGENT_src_diab)

LIBOBJS_krnl_ARMARCH7_common_tcfsvservice += $(LIBOBJS_krnl_ARMARCH7_common_tcfsvservice_kernel_top_SYSTEMVIEWER_AGENT_src_diab)

__OBJS_TO_CLEAN_SYSTEMVIEWER_AGENT += $(LIBOBJS_krnl_ARMARCH7_common_tcfsvservice_kernel_top_SYSTEMVIEWER_AGENT_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_tcfsvservice_kernel_top_SYSTEMVIEWER_AGENT_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_tcfsvservice

_DONEBASE_LIB_krnl_ARMARCH7_common_tcfsvservice = TRUE

# LIBBASES += tcfsvservice

__LIBS_TO_BUILD += krnl_ARMARCH7_common_tcfsvservice

__LIBS_TO_BUILD_SYSTEMVIEWER_AGENT += krnl_ARMARCH7_common_tcfsvservice

__BUILT_LIBS += krnl_ARMARCH7_common_tcfsvservice

__LIB_krnl_ARMARCH7_common_tcfsvservice = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libtcfsvservice$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objtcfsvservice :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libtcfsvservice$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libtcfsvservice.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libtcfsvservice.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libtcfsvservice$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libtcfsvservice$(OPT).a libtcfsvservice$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libtcfsvservice.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libtcfsvservice.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libtcfsvservice.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_tcfsvservice

arcmdfile_krnl_ARMARCH7_common_tcfsvservice :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_tcfsvservice))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libtcfsvservice$(OPT).a : arcmdfile_krnl_ARMARCH7_common_tcfsvservice
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_tcfsvservice


clean _layer_clean_SYSTEMVIEWER_AGENT : _clean_LIB_BASE_krnl_ARMARCH7_common_tcfsvservice

_clean_LIB_BASE_krnl_ARMARCH7_common_tcfsvservice :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libtcfsvservice$(OPT).a


endif

ifndef _DONEBASE_LIB_SYSTEMVIEWER_AGENT_krnl_ARMARCH7_common_tcfsvservice

_DONEBASE_LIB_SYSTEMVIEWER_AGENT_krnl_ARMARCH7_common_tcfsvservice = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerSYSTEMVIEWER_AGENT_libtcfsvservice.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerSYSTEMVIEWER_AGENT_libtcfsvservice.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libtcfsvservice$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,SYSTEMVIEWER_AGENT,libtcfsvservice.a,$(LIBOBJS_krnl_ARMARCH7_common_tcfsvservice)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libtcfsvservice$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_tcfsvservice)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
