
	
# this file is automatically generated by mk/krnl/defs.layers.mk. Please do not edit

__FILE_COPIED += $(__FILES_COPIED_IPNET_RADIUS)




	
	

	
# copying directory h to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/ipradius_config.h

__FILES_TO_COPY_IPNET_RADIUS += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/ipradius_config.h





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/ipradius_config.h : C:/WindRiverSR500/vxworks-7/pkgs/net/ipnet-1.1.1.2/aaa-1.0.1.8/radius/h/ipradius_config.h | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/ipradius.h

__FILES_TO_COPY_IPNET_RADIUS += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/ipradius.h





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public/ipradius.h : C:/WindRiverSR500/vxworks-7/pkgs/net/ipnet-1.1.1.2/aaa-1.0.1.8/radius/h/ipradius.h | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif




	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


	
	
	
	
	
	
	
BUILD : IPNET_RADIUS_src_diab__BUILD

.PHONY : IPNET_RADIUS_src_diab__BUILD

__BUILD_FB_IDS += kernel_top_IPNET_RADIUS_src_diab

IPNET_RADIUS_src_diab__BUILD : 
	@ echo building IPNET_RADIUS directory src
	+ @ cd C:/WindRiverSR500/vxworks-7/pkgs/net/ipnet-1.1.1.2/aaa-1.0.1.8/radius/src && C:/WindRiverSR500/vxworks-7/host/binutils/x86-win32/bin/make _mk CPU=ARMARCH7 TOOL=diab TOOL_VERSION=diab_5_9_6_1 _CC_VERSION=diab_5_9_6_1 SPACE=kernel _VSB_CONFIG_ADDEDCFLAGS="" _WRS_CONFIG_APP_TOOL=diab VSBL_NAME=IPNET_RADIUS VSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB _WRS_CONFIG_FP=vector _WRS_CLI_CFLAGS="-I. " _FB_ID=kernel_top_IPNET_RADIUS_src_diab BSPNAME=ti_sitara_ctxa9_branson_1_0_0_0 BUILDSTAGE=BUILD


	
	
	

	
	
	

	

	


	
# copying directory cdf to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/01folder_ipnet_radius.cdf

__FILES_TO_COPY_IPNET_RADIUS += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/01folder_ipnet_radius.cdf





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/01folder_ipnet_radius.cdf : C:/WindRiverSR500/vxworks-7/pkgs/net/ipnet-1.1.1.2/aaa-1.0.1.8/radius/cdf/01folder_ipnet_radius.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/02comp_ipnet_radius.cdf

__FILES_TO_COPY_IPNET_RADIUS += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/02comp_ipnet_radius.cdf





C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/02comp_ipnet_radius.cdf : C:/WindRiverSR500/vxworks-7/pkgs/net/ipnet-1.1.1.2/aaa-1.0.1.8/radius/cdf/02comp_ipnet_radius.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif


	
cdf_PRENOBUILD_IPNET_RADIUS_FILES = cdf/01folder_ipnet_radius.cdf cdf/02comp_ipnet_radius.cdf

cdf_PRENOBUILD_IPNET_RADIUS_DIR = C:/WindRiverSR500/vxworks-7/pkgs/net/ipnet-1.1.1.2/aaa-1.0.1.8/radius

PRENOBUILD_FILES += $(addprefix $(cdf_PRENOBUILD_IPNET_RADIUS_DIR)/,$(cdf_PRENOBUILD_IPNET_RADIUS_FILES))

PRENOBUILD : PRENOBUILD_IPNET_RADIUS

PRENOBUILD_IPNET_RADIUS : cdf_PRENOBUILD_IPNET_RADIUS

cdf_PRENOBUILD_IPNET_RADIUS : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_IPNET_RADIUS.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_IPNET_RADIUS.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_IPNET_RADIUS.tmp
	$(CDFCOMP) -i $^ -o $@ -cpp "$(CPP) $(__OPTION_LANG_CDF)" -cpu ARMARCH7 -layer IPNET_RADIUS

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf/cdf_PRENOBUILD_IPNET_RADIUS.tmp : | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf
	$(file >$@,$(call __vsb_tmpl_cdf_inter,$(addprefix $(cdf_PRENOBUILD_IPNET_RADIUS_DIR)/,$(sort $(cdf_PRENOBUILD_IPNET_RADIUS_FILES))),IPNET_RADIUS))



	

	
# copying directory cfg to C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette/ipradius_config.c

__FILES_TO_COPY_IPNET_RADIUS += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette/ipradius_config.c





 

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette/ipradius_config.c : C:/WindRiverSR500/vxworks-7/pkgs/net/ipnet-1.1.1.2/aaa-1.0.1.8/radius/cfg/ipradius_config.c | C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif



	
__DIR_TARGETS += $(__DIR_TARGETS_IPNET_RADIUS)

__DIR_TARGETS_IPNET_RADIUS += C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/cdf C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/configlette C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/share/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/h C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/vsblCdf

