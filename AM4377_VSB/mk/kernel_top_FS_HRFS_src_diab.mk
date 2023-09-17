
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






LIBOBJS = $(LIBOBJS_fsHrfs)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_fsHrfs_kernel_top_FS_HRFS_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objfsHrfs/hrfsSyncerVnode.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objfsHrfs/hrfsSyncerVnodeWr.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objfsHrfs/hrfsChkDskLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objfsHrfs/hrfsFormatLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objfsHrfs/hrFsTimeLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objfsHrfs/hrFsLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objfsHrfs/hrFsSuperBlk.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objfsHrfs/hrfsWrLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objfsHrfs/hrfsVopsWriteLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objfsHrfs/hrfsVfsLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objfsHrfs/hrfsVopsLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objfsHrfs/hrfsConverters.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objfsHrfs/hrfsTrans.o 

arcmdfile_krnl_ARMARCH7_common_fsHrfs : $(LIBOBJS_krnl_ARMARCH7_common_fsHrfs_kernel_top_FS_HRFS_src_diab)

LIBOBJS_krnl_ARMARCH7_common_fsHrfs_FS_HRFS += $(LIBOBJS_krnl_ARMARCH7_common_fsHrfs_kernel_top_FS_HRFS_src_diab)

LIBOBJS_krnl_ARMARCH7_common_fsHrfs += $(LIBOBJS_krnl_ARMARCH7_common_fsHrfs_kernel_top_FS_HRFS_src_diab)

__OBJS_TO_CLEAN_FS_HRFS += $(LIBOBJS_krnl_ARMARCH7_common_fsHrfs_kernel_top_FS_HRFS_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_fsHrfs_kernel_top_FS_HRFS_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_fsHrfs

_DONEBASE_LIB_krnl_ARMARCH7_common_fsHrfs = TRUE

# LIBBASES += fsHrfs

__LIBS_TO_BUILD += krnl_ARMARCH7_common_fsHrfs

__LIBS_TO_BUILD_FS_HRFS += krnl_ARMARCH7_common_fsHrfs

__BUILT_LIBS += krnl_ARMARCH7_common_fsHrfs

__LIB_krnl_ARMARCH7_common_fsHrfs = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libfsHrfs$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objfsHrfs :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libfsHrfs$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libfsHrfs.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libfsHrfs.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libfsHrfs$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common && $(NM) libfsHrfs$(OPT).a libfsHrfs$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libfsHrfs.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libfsHrfs.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libfsHrfs.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_fsHrfs

arcmdfile_krnl_ARMARCH7_common_fsHrfs :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_fsHrfs))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libfsHrfs$(OPT).a : arcmdfile_krnl_ARMARCH7_common_fsHrfs
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_fsHrfs


clean _layer_clean_FS_HRFS : _clean_LIB_BASE_krnl_ARMARCH7_common_fsHrfs

_clean_LIB_BASE_krnl_ARMARCH7_common_fsHrfs :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libfsHrfs$(OPT).a


endif

ifndef _DONEBASE_LIB_FS_HRFS_krnl_ARMARCH7_common_fsHrfs

_DONEBASE_LIB_FS_HRFS_krnl_ARMARCH7_common_fsHrfs = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerFS_HRFS_libfsHrfs.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerFS_HRFS_libfsHrfs.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libfsHrfs$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,FS_HRFS,libfsHrfs.a,$(LIBOBJS_krnl_ARMARCH7_common_fsHrfs)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libfsHrfs$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_fsHrfs)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
