
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

 
_STATIC_LIBOBJS_common_OPENSSL_user_static_top_OPENSSL_openssl-1_0_2_crypto_diab_des = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/cbc_cksm.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/cbc_enc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/cfb64ede.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/cfb64enc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/cfb_enc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/des_old.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/des_old2.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/destest.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/ecb3_enc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/ecb_enc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/ede_cbcm_enc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/enc_read.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/enc_writ.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/fcrypt.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/ofb64ede.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/ofb64enc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/ofb_enc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/pcbc_enc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/qud_cksm.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/rand_key.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/read2pwd.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/rpc_enc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/set_key.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/str2key.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/xcbc_enc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/des_enc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objOPENSSL/fcrypt_b.o

_usrstarcmdfile_common_OPENSSL : $(_STATIC_LIBOBJS_common_OPENSSL_user_static_top_OPENSSL_openssl-1_0_2_crypto_diab_des)

_STATIC_LIBOBJS_common_OPENSSL += $(_STATIC_LIBOBJS_common_OPENSSL_user_static_top_OPENSSL_openssl-1_0_2_crypto_diab_des)

__OBJS_TO_CLEAN_OPENSSL += $(_STATIC_LIBOBJS_common_OPENSSL_user_static_top_OPENSSL_openssl-1_0_2_crypto_diab_des)

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libOPENSSL.a : $(_STATIC_LIBOBJS_common_OPENSSL_user_static_top_OPENSSL_openssl-1_0_2_crypto_diab_des)

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
