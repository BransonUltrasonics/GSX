
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






LIBOBJS = $(LIBOBJS_bdmXbd)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_bdmXbd_kernel_top_BDM_XBD_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objbdmXbd/xbd.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objbdmXbd/bio.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objbdmXbd/xbdBioSchedCore.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objbdmXbd/xbdBioSchedDeadline.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objbdmXbd/xbdBioSchedPolicySSD.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objbdmXbd/xbdBioSchedPolicyNoop.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objbdmXbd/xbdRequestCache.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objbdmXbd/xbdBlkCache.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objbdmXbd/device.o 

arcmdfile_krnl_ARMARCH7_common_bdmXbd : $(LIBOBJS_krnl_ARMARCH7_common_bdmXbd_kernel_top_BDM_XBD_src_diab)

LIBOBJS_krnl_ARMARCH7_common_bdmXbd_BDM_XBD += $(LIBOBJS_krnl_ARMARCH7_common_bdmXbd_kernel_top_BDM_XBD_src_diab)

LIBOBJS_krnl_ARMARCH7_common_bdmXbd += $(LIBOBJS_krnl_ARMARCH7_common_bdmXbd_kernel_top_BDM_XBD_src_diab)

__OBJS_TO_CLEAN_BDM_XBD += $(LIBOBJS_krnl_ARMARCH7_common_bdmXbd_kernel_top_BDM_XBD_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_bdmXbd_kernel_top_BDM_XBD_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_bdmXbd

_DONEBASE_LIB_krnl_ARMARCH7_common_bdmXbd = TRUE

# LIBBASES += bdmXbd

__LIBS_TO_BUILD += krnl_ARMARCH7_common_bdmXbd

__LIBS_TO_BUILD_BDM_XBD += krnl_ARMARCH7_common_bdmXbd

__BUILT_LIBS += krnl_ARMARCH7_common_bdmXbd

__LIB_krnl_ARMARCH7_common_bdmXbd = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libbdmXbd$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objbdmXbd :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libbdmXbd$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libbdmXbd.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libbdmXbd.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libbdmXbd$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libbdmXbd$(OPT).a libbdmXbd$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libbdmXbd.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libbdmXbd.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libbdmXbd.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_bdmXbd

arcmdfile_krnl_ARMARCH7_common_bdmXbd :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_bdmXbd))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libbdmXbd$(OPT).a : arcmdfile_krnl_ARMARCH7_common_bdmXbd
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_bdmXbd


clean _layer_clean_BDM_XBD : _clean_LIB_BASE_krnl_ARMARCH7_common_bdmXbd

_clean_LIB_BASE_krnl_ARMARCH7_common_bdmXbd :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libbdmXbd$(OPT).a


endif

ifndef _DONEBASE_LIB_BDM_XBD_krnl_ARMARCH7_common_bdmXbd

_DONEBASE_LIB_BDM_XBD_krnl_ARMARCH7_common_bdmXbd = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerBDM_XBD_libbdmXbd.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerBDM_XBD_libbdmXbd.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libbdmXbd$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,BDM_XBD,libbdmXbd.a,$(LIBOBJS_krnl_ARMARCH7_common_bdmXbd)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libbdmXbd$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_bdmXbd)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
