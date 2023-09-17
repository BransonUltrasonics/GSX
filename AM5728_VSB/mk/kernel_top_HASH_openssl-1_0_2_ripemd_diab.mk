
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






LIBOBJS = $(LIBOBJS_HASH)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_HASH_kernel_top_HASH_openssl-1_0_2_ripemd_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objHASH/rmd_dgst.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objHASH/rmd_one.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objHASH/rmdtest.o 

arcmdfile_krnl_ARMARCH7_common_HASH : $(LIBOBJS_krnl_ARMARCH7_common_HASH_kernel_top_HASH_openssl-1_0_2_ripemd_diab)

LIBOBJS_krnl_ARMARCH7_common_HASH_HASH += $(LIBOBJS_krnl_ARMARCH7_common_HASH_kernel_top_HASH_openssl-1_0_2_ripemd_diab)

LIBOBJS_krnl_ARMARCH7_common_HASH += $(LIBOBJS_krnl_ARMARCH7_common_HASH_kernel_top_HASH_openssl-1_0_2_ripemd_diab)

__OBJS_TO_CLEAN_HASH += $(LIBOBJS_krnl_ARMARCH7_common_HASH_kernel_top_HASH_openssl-1_0_2_ripemd_diab) $(LIBOBJS_krnl_ARMARCH7_common_HASH_kernel_top_HASH_openssl-1_0_2_ripemd_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_HASH

_DONEBASE_LIB_krnl_ARMARCH7_common_HASH = TRUE

# LIBBASES += HASH

__LIBS_TO_BUILD += krnl_ARMARCH7_common_HASH

__LIBS_TO_BUILD_HASH += krnl_ARMARCH7_common_HASH

__BUILT_LIBS += krnl_ARMARCH7_common_HASH

__LIB_krnl_ARMARCH7_common_HASH = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libHASH$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objHASH :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libHASH$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libHASH.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libHASH.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libHASH$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libHASH$(OPT).a libHASH$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libHASH.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libHASH.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libHASH.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_HASH

arcmdfile_krnl_ARMARCH7_common_HASH :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_HASH))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libHASH$(OPT).a : arcmdfile_krnl_ARMARCH7_common_HASH
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_HASH


clean _layer_clean_HASH : _clean_LIB_BASE_krnl_ARMARCH7_common_HASH

_clean_LIB_BASE_krnl_ARMARCH7_common_HASH :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libHASH$(OPT).a


endif

ifndef _DONEBASE_LIB_HASH_krnl_ARMARCH7_common_HASH

_DONEBASE_LIB_HASH_krnl_ARMARCH7_common_HASH = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerHASH_libHASH.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerHASH_libHASH.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libHASH$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,HASH,libHASH.a,$(LIBOBJS_krnl_ARMARCH7_common_HASH)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libHASH$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_HASH)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
