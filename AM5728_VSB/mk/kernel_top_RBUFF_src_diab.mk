
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






LIBOBJS = $(LIBOBJS_rbuff)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_rbuff_kernel_top_RBUFF_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objrbuff/rBuffLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objrbuff/rBuffShow.o 

arcmdfile_krnl_ARMARCH7_common_rbuff : $(LIBOBJS_krnl_ARMARCH7_common_rbuff_kernel_top_RBUFF_src_diab)

LIBOBJS_krnl_ARMARCH7_common_rbuff_RBUFF += $(LIBOBJS_krnl_ARMARCH7_common_rbuff_kernel_top_RBUFF_src_diab)

LIBOBJS_krnl_ARMARCH7_common_rbuff += $(LIBOBJS_krnl_ARMARCH7_common_rbuff_kernel_top_RBUFF_src_diab)

__OBJS_TO_CLEAN_RBUFF += $(LIBOBJS_krnl_ARMARCH7_common_rbuff_kernel_top_RBUFF_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_rbuff_kernel_top_RBUFF_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_rbuff

_DONEBASE_LIB_krnl_ARMARCH7_common_rbuff = TRUE

# LIBBASES += rbuff

__LIBS_TO_BUILD += krnl_ARMARCH7_common_rbuff

__LIBS_TO_BUILD_RBUFF += krnl_ARMARCH7_common_rbuff

__BUILT_LIBS += krnl_ARMARCH7_common_rbuff

__LIB_krnl_ARMARCH7_common_rbuff = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/librbuff$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objrbuff :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/librbuff$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/librbuff.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/librbuff.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/librbuff$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) librbuff$(OPT).a librbuff$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/librbuff.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/librbuff.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a librbuff.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_rbuff

arcmdfile_krnl_ARMARCH7_common_rbuff :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_rbuff))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/librbuff$(OPT).a : arcmdfile_krnl_ARMARCH7_common_rbuff
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_rbuff


clean _layer_clean_RBUFF : _clean_LIB_BASE_krnl_ARMARCH7_common_rbuff

_clean_LIB_BASE_krnl_ARMARCH7_common_rbuff :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/librbuff$(OPT).a


endif

ifndef _DONEBASE_LIB_RBUFF_krnl_ARMARCH7_common_rbuff

_DONEBASE_LIB_RBUFF_krnl_ARMARCH7_common_rbuff = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerRBUFF_librbuff.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerRBUFF_librbuff.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/librbuff$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,RBUFF,librbuff.a,$(LIBOBJS_krnl_ARMARCH7_common_rbuff)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/librbuff$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_rbuff)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
