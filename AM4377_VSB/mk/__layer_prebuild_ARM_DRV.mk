
	
# this file is automatically generated by mk/krnl/defs.layers.mk. Please do not edit

__FILE_COPIED += $(__FILES_COPIED_ARM_DRV)




	
	

	


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


	
	
	
	
	
	
	
BUILD : ARM_DRV_src_diab__BUILD

.PHONY : ARM_DRV_src_diab__BUILD

__BUILD_FB_IDS += kernel_top_ARM_DRV_src_diab

ARM_DRV_src_diab__BUILD : 
	@ echo building ARM_DRV directory src
	+ @ cd C:/WindRiverSR500/vxworks-7/pkgs/os/arch/arm-1.1.9.1/kernel/drv/src && C:/WindRiverSR500/vxworks-7/host/binutils/x86-win32/bin/make _mk CPU=ARMARCH7 TOOL=diab TOOL_VERSION=diab_5_9_6_1 _CC_VERSION=diab_5_9_6_1 SPACE=kernel _VSB_CONFIG_ADDEDCFLAGS="" _WRS_CONFIG_APP_TOOL=diab VSBL_NAME=ARM_DRV VSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB _WRS_CONFIG_FP=vector _WRS_CLI_CFLAGS="-I. " _FB_ID=kernel_top_ARM_DRV_src_diab BSPNAME=ti_sitara_ctxa9_branson_1_0_0_0 BUILDSTAGE=BUILD


	
	
	

	
	
	

	

	


	
# copying directory cdf to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtPl310L2Cache.cdf

__FILES_TO_COPY_ARM_DRV += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtPl310L2Cache.cdf





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtPl310L2Cache.cdf : C:/WindRiverSR500/vxworks-7/pkgs/os/arch/arm-1.1.9.1/kernel/drv/cdf/40vxbFdtPl310L2Cache.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmGenTimer.cdf

__FILES_TO_COPY_ARM_DRV += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmGenTimer.cdf





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmGenTimer.cdf : C:/WindRiverSR500/vxworks-7/pkgs/os/arch/arm-1.1.9.1/kernel/drv/cdf/40vxbFdtArmGenTimer.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtPl330Dma.cdf

__FILES_TO_COPY_ARM_DRV += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtPl330Dma.cdf





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtPl330Dma.cdf : C:/WindRiverSR500/vxworks-7/pkgs/os/arch/arm-1.1.9.1/kernel/drv/cdf/40vxbFdtPl330Dma.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmGlobalTimer.cdf

__FILES_TO_COPY_ARM_DRV += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmGlobalTimer.cdf





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmGlobalTimer.cdf : C:/WindRiverSR500/vxworks-7/pkgs/os/arch/arm-1.1.9.1/kernel/drv/cdf/40vxbFdtArmGlobalTimer.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtKinetisCache.cdf

__FILES_TO_COPY_ARM_DRV += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtKinetisCache.cdf





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtKinetisCache.cdf : C:/WindRiverSR500/vxworks-7/pkgs/os/arch/arm-1.1.9.1/kernel/drv/cdf/40vxbFdtKinetisCache.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtCortexMSysTick.cdf

__FILES_TO_COPY_ARM_DRV += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtCortexMSysTick.cdf





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtCortexMSysTick.cdf : C:/WindRiverSR500/vxworks-7/pkgs/os/arch/arm-1.1.9.1/kernel/drv/cdf/40vxbFdtCortexMSysTick.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmGenIntCtlr.cdf

__FILES_TO_COPY_ARM_DRV += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmGenIntCtlr.cdf





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmGenIntCtlr.cdf : C:/WindRiverSR500/vxworks-7/pkgs/os/arch/arm-1.1.9.1/kernel/drv/cdf/40vxbFdtArmGenIntCtlr.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmNvic.cdf

__FILES_TO_COPY_ARM_DRV += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmNvic.cdf





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmNvic.cdf : C:/WindRiverSR500/vxworks-7/pkgs/os/arch/arm-1.1.9.1/kernel/drv/cdf/40vxbFdtArmNvic.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmMpCoreTimer.cdf

__FILES_TO_COPY_ARM_DRV += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmMpCoreTimer.cdf





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/40vxbFdtArmMpCoreTimer.cdf : C:/WindRiverSR500/vxworks-7/pkgs/os/arch/arm-1.1.9.1/kernel/drv/cdf/40vxbFdtArmMpCoreTimer.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif


	
cdf_PRENOBUILD_ARM_DRV_FILES = cdf/40vxbFdtPl310L2Cache.cdf cdf/40vxbFdtArmGenTimer.cdf cdf/40vxbFdtPl330Dma.cdf cdf/40vxbFdtArmGlobalTimer.cdf cdf/40vxbFdtKinetisCache.cdf cdf/40vxbFdtCortexMSysTick.cdf cdf/40vxbFdtArmGenIntCtlr.cdf cdf/40vxbFdtArmNvic.cdf cdf/40vxbFdtArmMpCoreTimer.cdf

cdf_PRENOBUILD_ARM_DRV_DIR = C:/WindRiverSR500/vxworks-7/pkgs/os/arch/arm-1.1.9.1/kernel/drv

PRENOBUILD_FILES += $(addprefix $(cdf_PRENOBUILD_ARM_DRV_DIR)/,$(cdf_PRENOBUILD_ARM_DRV_FILES))

PRENOBUILD : PRENOBUILD_ARM_DRV

PRENOBUILD_ARM_DRV : cdf_PRENOBUILD_ARM_DRV

cdf_PRENOBUILD_ARM_DRV : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_ARM_DRV.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_ARM_DRV.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_ARM_DRV.tmp
	$(CDFCOMP) -i $^ -o $@ -cpp "$(CPP) $(__OPTION_LANG_CDF)" -cpu ARMARCH7 -layer ARM_DRV

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_ARM_DRV.tmp : | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
	$(file >$@,$(call __vsb_tmpl_cdf_inter,$(addprefix $(cdf_PRENOBUILD_ARM_DRV_DIR)/,$(sort $(cdf_PRENOBUILD_ARM_DRV_FILES))),ARM_DRV))



	

	
# copying directory configlette to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette/pl310Access.c

__FILES_TO_COPY_ARM_DRV += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette/pl310Access.c





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette/pl310Access.c : C:/WindRiverSR500/vxworks-7/pkgs/os/arch/arm-1.1.9.1/kernel/drv/configlette/pl310Access.c | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif



	
__DIR_TARGETS += $(__DIR_TARGETS_ARM_DRV)

__DIR_TARGETS_ARM_DRV += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/share/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/vsblCdf

