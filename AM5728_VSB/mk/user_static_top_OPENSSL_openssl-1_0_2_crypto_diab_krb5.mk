
#
# This file is automatically generated by mk/usr/defs.fastlibgen.mk.
#
# build configuration :
#	VSB : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB
#	CPU : ARMARCH7
#	TOOL : diab
#	FP : vector
# 	ENDIAN : little
#	LIB_DIR_TAG = 
#	LIBDIRBASE = common
#	LIBDIRBASE = $(TOOL_COMMON_DIR)$(LIB_DIR_TAG)






ifeq (,)



endif


# DEP_OBJS = (DEP_OBJS)

# LIB_BASE_NAMES = OPENSSL

 
_STATIC_LIBOBJS_common_OPENSSL_user_static_top_OPENSSL_openssl-1_0_2_crypto_diab_krb5 = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/krb5_asn.o

_usrstarcmdfile_common_OPENSSL : $(_STATIC_LIBOBJS_common_OPENSSL_user_static_top_OPENSSL_openssl-1_0_2_crypto_diab_krb5)

_STATIC_LIBOBJS_common_OPENSSL += $(_STATIC_LIBOBJS_common_OPENSSL_user_static_top_OPENSSL_openssl-1_0_2_crypto_diab_krb5)

__OBJS_TO_CLEAN_OPENSSL += $(_STATIC_LIBOBJS_common_OPENSSL_user_static_top_OPENSSL_openssl-1_0_2_crypto_diab_krb5)

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libOPENSSL.a : $(_STATIC_LIBOBJS_common_OPENSSL_user_static_top_OPENSSL_openssl-1_0_2_crypto_diab_krb5)

ifndef _DONEBASE_USRST_LIB_common_OPENSSL

_DONEBASE_USRST_LIB_common_OPENSSL = TRUE

# LIBBASES += OPENSSL

# C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common:
#	mkdir -p $@

# C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL :
#	mkdir -p $@

usrstlib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libOPENSSL.a

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libOPENSSL.a : | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common

__STATIC_BUILT_LIBS += common_OPENSSL

__STLIB_common_OPENSSL = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libOPENSSL.a

arcmdfiles : _usrstarcmdfile_common_OPENSSL

_usrstarcmdfile_common_OPENSSL :
	$(file >$@,$(sort $(_STATIC_LIBOBJS_common_OPENSSL)))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libOPENSSL.a : _usrstarcmdfile_common_OPENSSL | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @_usrstarcmdfile_common_OPENSSL

clean  _layer_clean_OPENSSL : _clean_USRST_LIB_BASE_common_OPENSSL

_clean_USRST_LIB_BASE_common_OPENSSL :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libOPENSSL.a

endif


 

 

################
# rules defined locally in Makefile
##





################

objs : $(LIBOBJS)
 
# this is the end of the file 
