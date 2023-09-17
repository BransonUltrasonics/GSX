
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



include kernel_top_BDM_MTD_src_diab_nand.mk 
include kernel_top_BDM_MTD_src_diab_gpmc.mk 
include kernel_top_BDM_MTD_src_diab_fcm.mk 
include kernel_top_BDM_MTD_src_diab_gpmi.mk 
include kernel_top_BDM_MTD_src_diab_norflash.mk 
include kernel_top_BDM_MTD_src_diab_spiflash.mk 
include kernel_top_BDM_MTD_src_diab_alt5qspiflash.mk 
include kernel_top_BDM_MTD_src_diab_nfc.mk 
include kernel_top_BDM_MTD_src_diab_emif16.mk



LIBOBJS = $(LIBOBJS_bdmMtd)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_bdmMtd_kernel_top_BDM_MTD_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbdmMtd/mtdCore.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbdmMtd/mtdShow.o 

arcmdfile_krnl_ARMARCH7_common_bdmMtd : $(LIBOBJS_krnl_ARMARCH7_common_bdmMtd_kernel_top_BDM_MTD_src_diab)

LIBOBJS_krnl_ARMARCH7_common_bdmMtd_BDM_MTD += $(LIBOBJS_krnl_ARMARCH7_common_bdmMtd_kernel_top_BDM_MTD_src_diab)

LIBOBJS_krnl_ARMARCH7_common_bdmMtd += $(LIBOBJS_krnl_ARMARCH7_common_bdmMtd_kernel_top_BDM_MTD_src_diab)

__OBJS_TO_CLEAN_BDM_MTD += $(LIBOBJS_krnl_ARMARCH7_common_bdmMtd_kernel_top_BDM_MTD_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_bdmMtd_kernel_top_BDM_MTD_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_bdmMtd

_DONEBASE_LIB_krnl_ARMARCH7_common_bdmMtd = TRUE

# LIBBASES += bdmMtd

__LIBS_TO_BUILD += krnl_ARMARCH7_common_bdmMtd

__LIBS_TO_BUILD_BDM_MTD += krnl_ARMARCH7_common_bdmMtd

__BUILT_LIBS += krnl_ARMARCH7_common_bdmMtd

__LIB_krnl_ARMARCH7_common_bdmMtd = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbdmMtd$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbdmMtd :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbdmMtd$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbdmMtd.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbdmMtd.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbdmMtd$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common && $(NM) libbdmMtd$(OPT).a libbdmMtd$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbdmMtd.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbdmMtd.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libbdmMtd.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_bdmMtd

arcmdfile_krnl_ARMARCH7_common_bdmMtd :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_bdmMtd))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbdmMtd$(OPT).a : arcmdfile_krnl_ARMARCH7_common_bdmMtd
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_bdmMtd


clean _layer_clean_BDM_MTD : _clean_LIB_BASE_krnl_ARMARCH7_common_bdmMtd

_clean_LIB_BASE_krnl_ARMARCH7_common_bdmMtd :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbdmMtd$(OPT).a


endif

ifndef _DONEBASE_LIB_BDM_MTD_krnl_ARMARCH7_common_bdmMtd

_DONEBASE_LIB_BDM_MTD_krnl_ARMARCH7_common_bdmMtd = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerBDM_MTD_libbdmMtd.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerBDM_MTD_libbdmMtd.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbdmMtd$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,BDM_MTD,libbdmMtd.a,$(LIBOBJS_krnl_ARMARCH7_common_bdmMtd)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbdmMtd$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_bdmMtd)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
