
	
# this file is automatically generated by mk/krnl/defs.layers.mk. Please do not edit

__FILE_COPIED += $(__FILES_COPIED_SDMMC_CORE)




	
	

	
# copying directory h to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/vxbSdMmcLib.h

__FILES_TO_COPY_SDMMC_CORE += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/vxbSdMmcLib.h





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/vxbSdMmcLib.h : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/core-1.0.0.12/h/vxbSdMmcLib.h | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif




	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


	
PREBUILD_GENHEADERS : SDMMC_CORE_genh_diab__PREBUILD_GENHEADERS



SDMMC_CORE_genh_diab__PREBUILD_GENHEADERS : 
	@ echo building SDMMC_CORE directory genh
	+ @ cd C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/core-1.0.0.12/genh && C:/WindRiverSR500/vxworks-7/host/binutils/x86-win32/bin/make CPU=ARMARCH7 TOOL=diab TOOL_VERSION=diab_5_9_6_1 _CC_VERSION=diab_5_9_6_1 SPACE=kernel _VSB_CONFIG_ADDEDCFLAGS="" _WRS_CONFIG_APP_TOOL=diab VSBL_NAME=SDMMC_CORE VSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB _WRS_CONFIG_FP=vector _WRS_CLI_CFLAGS="-I. " _FB_ID=_PREBUILD_GENHEADERS_SDMMC_CORE_genh_diab __WRS_TARGET=PREBUILD_GENHEADERS BSPNAME=ti_sitara_ctxa9_branson_1_0_0_0  BUILDSTAGE=PREBUILD_GENHEADERS

	
	
	
	
	
	
BUILD : SDMMC_CORE_src_diab__BUILD

.PHONY : SDMMC_CORE_src_diab__BUILD

__BUILD_FB_IDS += kernel_top_SDMMC_CORE_src_diab

SDMMC_CORE_src_diab__BUILD : 
	@ echo building SDMMC_CORE directory src
	+ @ cd C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/core-1.0.0.12/src && C:/WindRiverSR500/vxworks-7/host/binutils/x86-win32/bin/make _mk CPU=ARMARCH7 TOOL=diab TOOL_VERSION=diab_5_9_6_1 _CC_VERSION=diab_5_9_6_1 SPACE=kernel _VSB_CONFIG_ADDEDCFLAGS="" _WRS_CONFIG_APP_TOOL=diab VSBL_NAME=SDMMC_CORE VSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB _WRS_CONFIG_FP=vector _WRS_CLI_CFLAGS="-I. " _FB_ID=kernel_top_SDMMC_CORE_src_diab BSPNAME=ti_sitara_ctxa9_branson_1_0_0_0 BUILDSTAGE=BUILD


	
	
	

	
	
	

	

	


	
# copying directory cdf to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbSdMmc.cdf

__FILES_TO_COPY_SDMMC_CORE += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbSdMmc.cdf





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbSdMmc.cdf : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/core-1.0.0.12/cdf/40vxbSdMmc.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif


	
cdf_PRENOBUILD_SDMMC_CORE_FILES = cdf/40vxbSdMmc.cdf

cdf_PRENOBUILD_SDMMC_CORE_DIR = C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/core-1.0.0.12

PRENOBUILD_FILES += $(addprefix $(cdf_PRENOBUILD_SDMMC_CORE_DIR)/,$(cdf_PRENOBUILD_SDMMC_CORE_FILES))

PRENOBUILD : PRENOBUILD_SDMMC_CORE

PRENOBUILD_SDMMC_CORE : cdf_PRENOBUILD_SDMMC_CORE

cdf_PRENOBUILD_SDMMC_CORE : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SDMMC_CORE.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SDMMC_CORE.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SDMMC_CORE.tmp
	$(CDFCOMP) -i $^ -o $@ -cpp "$(CPP) $(__OPTION_LANG_CDF)" -cpu ARMARCH7 -layer SDMMC_CORE

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SDMMC_CORE.tmp : | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
	$(file >$@,$(call __vsb_tmpl_cdf_inter,$(addprefix $(cdf_PRENOBUILD_SDMMC_CORE_DIR)/,$(sort $(cdf_PRENOBUILD_SDMMC_CORE_FILES))),SDMMC_CORE))



	

	
# copying directory cfg to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette






	
__DIR_TARGETS += $(__DIR_TARGETS_SDMMC_CORE)

__DIR_TARGETS_SDMMC_CORE += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/share/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/vsblCdf

