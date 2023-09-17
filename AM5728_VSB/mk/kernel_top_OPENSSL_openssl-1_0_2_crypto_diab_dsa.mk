
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






LIBOBJS = $(LIBOBJS_OPENSSL)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_OPENSSL_kernel_top_OPENSSL_openssl-1_0_2_crypto_diab_dsa =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL/dsa_ameth.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL/dsa_asn1.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL/dsa_depr.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL/dsa_err.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL/dsa_gen.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL/dsa_key.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL/dsa_lib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL/dsa_ossl.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL/dsa_pmeth.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL/dsa_prn.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL/dsa_sign.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL/dsa_vrf.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL/dsagen.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL/dsatest.o 

arcmdfile_krnl_ARMARCH7_common_OPENSSL : $(LIBOBJS_krnl_ARMARCH7_common_OPENSSL_kernel_top_OPENSSL_openssl-1_0_2_crypto_diab_dsa)

LIBOBJS_krnl_ARMARCH7_common_OPENSSL_OPENSSL += $(LIBOBJS_krnl_ARMARCH7_common_OPENSSL_kernel_top_OPENSSL_openssl-1_0_2_crypto_diab_dsa)

LIBOBJS_krnl_ARMARCH7_common_OPENSSL += $(LIBOBJS_krnl_ARMARCH7_common_OPENSSL_kernel_top_OPENSSL_openssl-1_0_2_crypto_diab_dsa)

__OBJS_TO_CLEAN_OPENSSL += $(LIBOBJS_krnl_ARMARCH7_common_OPENSSL_kernel_top_OPENSSL_openssl-1_0_2_crypto_diab_dsa) $(LIBOBJS_krnl_ARMARCH7_common_OPENSSL_kernel_top_OPENSSL_openssl-1_0_2_crypto_diab_dsa:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_OPENSSL

_DONEBASE_LIB_krnl_ARMARCH7_common_OPENSSL = TRUE

# LIBBASES += OPENSSL

__LIBS_TO_BUILD += krnl_ARMARCH7_common_OPENSSL

__LIBS_TO_BUILD_OPENSSL += krnl_ARMARCH7_common_OPENSSL

__BUILT_LIBS += krnl_ARMARCH7_common_OPENSSL

__LIB_krnl_ARMARCH7_common_OPENSSL = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libOPENSSL$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objOPENSSL :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libOPENSSL$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libOPENSSL.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libOPENSSL.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libOPENSSL$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libOPENSSL$(OPT).a libOPENSSL$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libOPENSSL.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libOPENSSL.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libOPENSSL.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_OPENSSL

arcmdfile_krnl_ARMARCH7_common_OPENSSL :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_OPENSSL))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libOPENSSL$(OPT).a : arcmdfile_krnl_ARMARCH7_common_OPENSSL
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_OPENSSL


clean _layer_clean_OPENSSL : _clean_LIB_BASE_krnl_ARMARCH7_common_OPENSSL

_clean_LIB_BASE_krnl_ARMARCH7_common_OPENSSL :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libOPENSSL$(OPT).a


endif

ifndef _DONEBASE_LIB_OPENSSL_krnl_ARMARCH7_common_OPENSSL

_DONEBASE_LIB_OPENSSL_krnl_ARMARCH7_common_OPENSSL = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerOPENSSL_libOPENSSL.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerOPENSSL_libOPENSSL.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libOPENSSL$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,OPENSSL,libOPENSSL.a,$(LIBOBJS_krnl_ARMARCH7_common_OPENSSL)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libOPENSSL$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_OPENSSL)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
