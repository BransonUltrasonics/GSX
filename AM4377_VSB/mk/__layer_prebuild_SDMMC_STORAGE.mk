
	
# this file is automatically generated by mk/krnl/defs.layers.mk. Please do not edit

__FILE_COPIED += $(__FILES_COPIED_SDMMC_STORAGE)




	
	

	
# copying directory h to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/vxbSdStorage.h

__FILES_TO_COPY_SDMMC_STORAGE += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/vxbSdStorage.h





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/vxbSdStorage.h : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/device/storage-1.0.1.11/h/vxbSdStorage.h | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/blkXbd.h

__FILES_TO_COPY_SDMMC_STORAGE += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/blkXbd.h





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/blkXbd.h : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/device/storage-1.0.1.11/h/blkXbd.h | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/vxbMmcStorage.h

__FILES_TO_COPY_SDMMC_STORAGE += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/vxbMmcStorage.h





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/vxbMmcStorage.h : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/device/storage-1.0.1.11/h/vxbMmcStorage.h | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif




	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


	
	
	
	
	
	
	
BUILD : SDMMC_STORAGE_src_diab__BUILD

.PHONY : SDMMC_STORAGE_src_diab__BUILD

__BUILD_FB_IDS += kernel_top_SDMMC_STORAGE_src_diab

SDMMC_STORAGE_src_diab__BUILD : 
	@ echo building SDMMC_STORAGE directory src
	+ @ cd C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/device/storage-1.0.1.11/src && C:/WindRiverSR500/vxworks-7/host/binutils/x86-win32/bin/make _mk CPU=ARMARCH7 TOOL=diab TOOL_VERSION=diab_5_9_6_1 _CC_VERSION=diab_5_9_6_1 SPACE=kernel _VSB_CONFIG_ADDEDCFLAGS="" _WRS_CONFIG_APP_TOOL=diab VSBL_NAME=SDMMC_STORAGE VSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB _WRS_CONFIG_FP=vector _WRS_CLI_CFLAGS="-I. " _FB_ID=kernel_top_SDMMC_STORAGE_src_diab BSPNAME=ti_sitara_ctxa9_branson_1_0_0_0 BUILDSTAGE=BUILD


	
	
	

	
	
	

	

	


	
# copying directory cdf to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbSdStorage.cdf

__FILES_TO_COPY_SDMMC_STORAGE += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbSdStorage.cdf





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbSdStorage.cdf : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/device/storage-1.0.1.11/cdf/40vxbSdStorage.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbMmcStorage.cdf

__FILES_TO_COPY_SDMMC_STORAGE += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbMmcStorage.cdf





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbMmcStorage.cdf : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/device/storage-1.0.1.11/cdf/40vxbMmcStorage.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif


	
cdf_PRENOBUILD_SDMMC_STORAGE_FILES = cdf/40vxbSdStorage.cdf cdf/40vxbMmcStorage.cdf

cdf_PRENOBUILD_SDMMC_STORAGE_DIR = C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/device/storage-1.0.1.11

PRENOBUILD_FILES += $(addprefix $(cdf_PRENOBUILD_SDMMC_STORAGE_DIR)/,$(cdf_PRENOBUILD_SDMMC_STORAGE_FILES))

PRENOBUILD : PRENOBUILD_SDMMC_STORAGE

PRENOBUILD_SDMMC_STORAGE : cdf_PRENOBUILD_SDMMC_STORAGE

cdf_PRENOBUILD_SDMMC_STORAGE : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SDMMC_STORAGE.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SDMMC_STORAGE.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SDMMC_STORAGE.tmp
	$(CDFCOMP) -i $^ -o $@ -cpp "$(CPP) $(__OPTION_LANG_CDF)" -cpu ARMARCH7 -layer SDMMC_STORAGE

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SDMMC_STORAGE.tmp : | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
	$(file >$@,$(call __vsb_tmpl_cdf_inter,$(addprefix $(cdf_PRENOBUILD_SDMMC_STORAGE_DIR)/,$(sort $(cdf_PRENOBUILD_SDMMC_STORAGE_FILES))),SDMMC_STORAGE))



	

	

	
__DIR_TARGETS += $(__DIR_TARGETS_SDMMC_STORAGE)

__DIR_TARGETS_SDMMC_STORAGE += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/share/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/vsblCdf
