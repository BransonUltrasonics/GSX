
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






LIBOBJS = $(LIBOBJS_erfLib)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_erfLib_kernel_top_ERF_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objerfLib/erfLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objerfLib/erfShow.o 

arcmdfile_krnl_ARMARCH7_common_erfLib : $(LIBOBJS_krnl_ARMARCH7_common_erfLib_kernel_top_ERF_src_diab)

LIBOBJS_krnl_ARMARCH7_common_erfLib_ERF += $(LIBOBJS_krnl_ARMARCH7_common_erfLib_kernel_top_ERF_src_diab)

LIBOBJS_krnl_ARMARCH7_common_erfLib += $(LIBOBJS_krnl_ARMARCH7_common_erfLib_kernel_top_ERF_src_diab)

__OBJS_TO_CLEAN_ERF += $(LIBOBJS_krnl_ARMARCH7_common_erfLib_kernel_top_ERF_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_erfLib_kernel_top_ERF_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_erfLib

_DONEBASE_LIB_krnl_ARMARCH7_common_erfLib = TRUE

# LIBBASES += erfLib

__LIBS_TO_BUILD += krnl_ARMARCH7_common_erfLib

__LIBS_TO_BUILD_ERF += krnl_ARMARCH7_common_erfLib

__BUILT_LIBS += krnl_ARMARCH7_common_erfLib

__LIB_krnl_ARMARCH7_common_erfLib = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/liberfLib$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objerfLib :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/liberfLib$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/liberfLib.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/liberfLib.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/liberfLib$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common && $(NM) liberfLib$(OPT).a liberfLib$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/liberfLib.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/liberfLib.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a liberfLib.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_erfLib

arcmdfile_krnl_ARMARCH7_common_erfLib :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_erfLib))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/liberfLib$(OPT).a : arcmdfile_krnl_ARMARCH7_common_erfLib
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_erfLib


clean _layer_clean_ERF : _clean_LIB_BASE_krnl_ARMARCH7_common_erfLib

_clean_LIB_BASE_krnl_ARMARCH7_common_erfLib :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/liberfLib$(OPT).a


endif

ifndef _DONEBASE_LIB_ERF_krnl_ARMARCH7_common_erfLib

_DONEBASE_LIB_ERF_krnl_ARMARCH7_common_erfLib = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerERF_liberfLib.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerERF_liberfLib.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/liberfLib$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,ERF,liberfLib.a,$(LIBOBJS_krnl_ARMARCH7_common_erfLib)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/liberfLib$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_erfLib)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
