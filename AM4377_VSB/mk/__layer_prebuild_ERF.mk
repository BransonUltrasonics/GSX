
	
# this file is automatically generated by mk/krnl/defs.layers.mk. Please do not edit

__FILE_COPIED += $(__FILES_COPIED_ERF)




	
	

	
# copying directory h to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public




# copying directory h/drv to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv




# copying directory h/drv/erf to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf/erfLibP.h

__FILES_TO_COPY_ERF += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf/erfLibP.h





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf/erfLibP.h : C:/WindRiverSR500/vxworks-7/pkgs/os/service/erf-1.0.1.5/h/drv/erf/erfLibP.h | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf/erfLib.h

__FILES_TO_COPY_ERF += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf/erfLib.h





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf/erfLib.h : C:/WindRiverSR500/vxworks-7/pkgs/os/service/erf-1.0.1.5/h/drv/erf/erfLib.h | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf/ownership.txt

__FILES_TO_COPY_ERF += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf/ownership.txt





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf/ownership.txt : C:/WindRiverSR500/vxworks-7/pkgs/os/service/erf-1.0.1.5/h/drv/erf/ownership.txt | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf/erfShow.h

__FILES_TO_COPY_ERF += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf/erfShow.h





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf/erfShow.h : C:/WindRiverSR500/vxworks-7/pkgs/os/service/erf-1.0.1.5/h/drv/erf/erfShow.h | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif






	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


	
	
	
	
	
	
	
BUILD : ERF_src_diab__BUILD

.PHONY : ERF_src_diab__BUILD

__BUILD_FB_IDS += kernel_top_ERF_src_diab

ERF_src_diab__BUILD : 
	@ echo building ERF directory src
	+ @ cd C:/WindRiverSR500/vxworks-7/pkgs/os/service/erf-1.0.1.5/src && C:/WindRiverSR500/vxworks-7/host/binutils/x86-win32/bin/make _mk CPU=ARMARCH7 TOOL=diab TOOL_VERSION=diab_5_9_6_1 _CC_VERSION=diab_5_9_6_1 SPACE=kernel _VSB_CONFIG_ADDEDCFLAGS="" _WRS_CONFIG_APP_TOOL=diab VSBL_NAME=ERF VSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB _WRS_CONFIG_FP=vector _WRS_CLI_CFLAGS="-I. " _FB_ID=kernel_top_ERF_src_diab BSPNAME=ti_sitara_ctxa9_branson_1_0_0_0 BUILDSTAGE=BUILD


	
	
	

	
	
	

	

	


	
# copying directory cdf to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/01comp_erf.cdf

__FILES_TO_COPY_ERF += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/01comp_erf.cdf





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/01comp_erf.cdf : C:/WindRiverSR500/vxworks-7/pkgs/os/service/erf-1.0.1.5/cdf/01comp_erf.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif


	
cdf_PRENOBUILD_ERF_FILES = cdf/01comp_erf.cdf

cdf_PRENOBUILD_ERF_DIR = C:/WindRiverSR500/vxworks-7/pkgs/os/service/erf-1.0.1.5

PRENOBUILD_FILES += $(addprefix $(cdf_PRENOBUILD_ERF_DIR)/,$(cdf_PRENOBUILD_ERF_FILES))

PRENOBUILD : PRENOBUILD_ERF

PRENOBUILD_ERF : cdf_PRENOBUILD_ERF

cdf_PRENOBUILD_ERF : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_ERF.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_ERF.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_ERF.tmp
	$(CDFCOMP) -i $^ -o $@ -cpp "$(CPP) $(__OPTION_LANG_CDF)" -cpu ARMARCH7 -layer ERF

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_ERF.tmp : | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
	$(file >$@,$(call __vsb_tmpl_cdf_inter,$(addprefix $(cdf_PRENOBUILD_ERF_DIR)/,$(sort $(cdf_PRENOBUILD_ERF_FILES))),ERF))



	

	

	
__DIR_TARGETS += $(__DIR_TARGETS_ERF)

__DIR_TARGETS_ERF += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/drv/erf C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/share/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/vsblCdf
