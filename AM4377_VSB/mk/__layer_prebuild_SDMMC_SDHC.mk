
	
# this file is automatically generated by mk/krnl/defs.layers.mk. Please do not edit

__FILE_COPIED += $(__FILES_COPIED_SDMMC_SDHC)




	
	

	
# copying directory h to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/vxbSdhcCtrl.h

__FILES_TO_COPY_SDMMC_SDHC += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/vxbSdhcCtrl.h





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/vxbSdhcCtrl.h : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/host/sdhc-1.0.3.0/h/vxbSdhcCtrl.h | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif




	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


	
	
	
	
	
	
	
BUILD : SDMMC_SDHC_src_diab__BUILD

.PHONY : SDMMC_SDHC_src_diab__BUILD

__BUILD_FB_IDS += kernel_top_SDMMC_SDHC_src_diab

SDMMC_SDHC_src_diab__BUILD : 
	@ echo building SDMMC_SDHC directory src
	+ @ cd C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/host/sdhc-1.0.3.0/src && C:/WindRiverSR500/vxworks-7/host/binutils/x86-win32/bin/make _mk CPU=ARMARCH7 TOOL=diab TOOL_VERSION=diab_5_9_6_1 _CC_VERSION=diab_5_9_6_1 SPACE=kernel _VSB_CONFIG_ADDEDCFLAGS="" _WRS_CONFIG_APP_TOOL=diab VSBL_NAME=SDMMC_SDHC VSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB _WRS_CONFIG_FP=vector _WRS_CLI_CFLAGS="-I. " _FB_ID=kernel_top_SDMMC_SDHC_src_diab BSPNAME=ti_sitara_ctxa9_branson_1_0_0_0 BUILDSTAGE=BUILD


	
	
	

	
	
	

	

	


	
# copying directory cdf to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbZynqSdhcCtrl.cdf

__FILES_TO_COPY_SDMMC_SDHC += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbZynqSdhcCtrl.cdf





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbZynqSdhcCtrl.cdf : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/host/sdhc-1.0.3.0/cdf/40vxbZynqSdhcCtrl.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFslImxSdhcCtrl.cdf

__FILES_TO_COPY_SDMMC_SDHC += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFslImxSdhcCtrl.cdf





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFslImxSdhcCtrl.cdf : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/host/sdhc-1.0.3.0/cdf/40vxbFslImxSdhcCtrl.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFslSdhcCtrl.cdf

__FILES_TO_COPY_SDMMC_SDHC += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFslSdhcCtrl.cdf





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFslSdhcCtrl.cdf : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/host/sdhc-1.0.3.0/cdf/40vxbFslSdhcCtrl.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbPciSdhcCtrl.cdf

__FILES_TO_COPY_SDMMC_SDHC += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbPciSdhcCtrl.cdf





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbPciSdhcCtrl.cdf : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/host/sdhc-1.0.3.0/cdf/40vxbPciSdhcCtrl.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif


	
cdf_PRENOBUILD_SDMMC_SDHC_FILES = cdf/40vxbZynqSdhcCtrl.cdf cdf/40vxbFslImxSdhcCtrl.cdf cdf/40vxbFslSdhcCtrl.cdf cdf/40vxbPciSdhcCtrl.cdf

cdf_PRENOBUILD_SDMMC_SDHC_DIR = C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/host/sdhc-1.0.3.0

PRENOBUILD_FILES += $(addprefix $(cdf_PRENOBUILD_SDMMC_SDHC_DIR)/,$(cdf_PRENOBUILD_SDMMC_SDHC_FILES))

PRENOBUILD : PRENOBUILD_SDMMC_SDHC

PRENOBUILD_SDMMC_SDHC : cdf_PRENOBUILD_SDMMC_SDHC

cdf_PRENOBUILD_SDMMC_SDHC : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SDMMC_SDHC.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SDMMC_SDHC.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SDMMC_SDHC.tmp
	$(CDFCOMP) -i $^ -o $@ -cpp "$(CPP) $(__OPTION_LANG_CDF)" -cpu ARMARCH7 -layer SDMMC_SDHC

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_SDMMC_SDHC.tmp : | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
	$(file >$@,$(call __vsb_tmpl_cdf_inter,$(addprefix $(cdf_PRENOBUILD_SDMMC_SDHC_DIR)/,$(sort $(cdf_PRENOBUILD_SDMMC_SDHC_FILES))),SDMMC_SDHC))



	

	
# copying directory cfg to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette/usrSdMmc.c

__FILES_TO_COPY_SDMMC_SDHC += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette/usrSdMmc.c





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette/usrSdMmc.c : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/sdmmc-1.0.1.3/host/sdhc-1.0.3.0/cfg/usrSdMmc.c | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif



	
__DIR_TARGETS += $(__DIR_TARGETS_SDMMC_SDHC)

__DIR_TARGETS_SDMMC_SDHC += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/share/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/vsblCdf

