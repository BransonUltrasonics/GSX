
	
# this file is automatically generated by mk/krnl/defs.layers.mk. Please do not edit

__FILE_COPIED += $(__FILES_COPIED_SEC_HASH)




	
	

	
# copying directory h to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/secHash.h

__FILES_TO_COPY_SEC_HASH += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/secHash.h





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/secHash.h : C:/WindRiverSR500/vxworks-7/pkgs/security/secHash-1.0.2.1/h/secHash.h | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/secHmac.h

__FILES_TO_COPY_SEC_HASH += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/secHmac.h





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/secHmac.h : C:/WindRiverSR500/vxworks-7/pkgs/security/secHash-1.0.2.1/h/secHmac.h | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/secPbkdf2.h

__FILES_TO_COPY_SEC_HASH += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/secPbkdf2.h





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/secPbkdf2.h : C:/WindRiverSR500/vxworks-7/pkgs/security/secHash-1.0.2.1/h/secPbkdf2.h | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif




	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


	
	
	
	
	
	
	
BUILD : SEC_HASH_src_diab__BUILD

.PHONY : SEC_HASH_src_diab__BUILD

__BUILD_FB_IDS += kernel_top_SEC_HASH_src_diab

SEC_HASH_src_diab__BUILD : 
	@ echo building SEC_HASH directory src
	+ @ cd C:/WindRiverSR500/vxworks-7/pkgs/security/secHash-1.0.2.1/src && C:/WindRiverSR500/vxworks-7/host/binutils/x86-win32/bin/make _mk CPU=ARMARCH7 TOOL=diab TOOL_VERSION=diab_5_9_6_1 _CC_VERSION=diab_5_9_6_1 SPACE=kernel _VSB_CONFIG_ADDEDCFLAGS="" _WRS_CONFIG_APP_TOOL=diab VSBL_NAME=SEC_HASH VSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB _WRS_CONFIG_FP=vector _WRS_CLI_CFLAGS="-I. " _FB_ID=kernel_top_SEC_HASH_src_diab BSPNAME=ti_sitara_ctxa9_branson_1_0_0_0 BUILDSTAGE=BUILD


	
	
	

	
	
	

	

	


	
# copying directory cdf to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/00comp_secHash.cdf

__FILES_TO_COPY_SEC_HASH += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/00comp_secHash.cdf





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/00comp_secHash.cdf : C:/WindRiverSR500/vxworks-7/pkgs/security/secHash-1.0.2.1/cdf/00comp_secHash.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif


	
cdf_PRENOBUILD_SEC_HASH_FILES = cdf/00comp_secHash.cdf

cdf_PRENOBUILD_SEC_HASH_DIR = C:/WindRiverSR500/vxworks-7/pkgs/security/secHash-1.0.2.1

PRENOBUILD_FILES += $(addprefix $(cdf_PRENOBUILD_SEC_HASH_DIR)/,$(cdf_PRENOBUILD_SEC_HASH_FILES))

PRENOBUILD : PRENOBUILD_SEC_HASH

PRENOBUILD_SEC_HASH : cdf_PRENOBUILD_SEC_HASH

cdf_PRENOBUILD_SEC_HASH : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SEC_HASH.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SEC_HASH.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SEC_HASH.tmp
	$(CDFCOMP) -i $^ -o $@ -cpp "$(CPP) $(__OPTION_LANG_CDF)" -cpu ARMARCH7 -layer SEC_HASH

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SEC_HASH.tmp : | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
	$(file >$@,$(call __vsb_tmpl_cdf_inter,$(addprefix $(cdf_PRENOBUILD_SEC_HASH_DIR)/,$(sort $(cdf_PRENOBUILD_SEC_HASH_FILES))),SEC_HASH))



	

	
# copying directory cfg to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette






	
__DIR_TARGETS += $(__DIR_TARGETS_SEC_HASH)

__DIR_TARGETS_SEC_HASH += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/share/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/vsblCdf

