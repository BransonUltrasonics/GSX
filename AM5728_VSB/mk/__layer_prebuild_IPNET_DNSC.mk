
	
# this file is automatically generated by mk/krnl/defs.layers.mk. Please do not edit

__FILE_COPIED += $(__FILES_COPIED_IPNET_DNSC)




	
	

	
# copying directory h to C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/ipdnsc_config.h

__FILES_TO_COPY_IPNET_DNSC += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/ipdnsc_config.h





 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/ipdnsc_config.h : C:/WindRiverSR500/vxworks-7/pkgs/net/ipnet-1.1.1.3/app/dnsc-1.0.1.5/h/ipdnsc_config.h | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/ipdnsc.h

__FILES_TO_COPY_IPNET_DNSC += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/ipdnsc.h





C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/ipdnsc.h : C:/WindRiverSR500/vxworks-7/pkgs/net/ipnet-1.1.1.3/app/dnsc-1.0.1.5/h/ipdnsc.h | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif




	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


	
	
	
	
	
	
	
BUILD : IPNET_DNSC_src_diab__BUILD

.PHONY : IPNET_DNSC_src_diab__BUILD

__BUILD_FB_IDS += kernel_top_IPNET_DNSC_src_diab

IPNET_DNSC_src_diab__BUILD : 
	@ echo building IPNET_DNSC directory src
	+ @ cd C:/WindRiverSR500/vxworks-7/pkgs/net/ipnet-1.1.1.3/app/dnsc-1.0.1.5/src && C:/WindRiverSR500/vxworks-7/host/binutils/x86-win32/bin/make _mk CPU=ARMARCH7 TOOL=diab TOOL_VERSION=diab_5_9_6_1 _CC_VERSION=diab_5_9_6_1 SPACE=kernel _VSB_CONFIG_ADDEDCFLAGS="" _WRS_CONFIG_APP_TOOL=diab VSBL_NAME=IPNET_DNSC VSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB _WRS_CONFIG_FP=vector _WRS_CLI_CFLAGS="-I. " _FB_ID=kernel_top_IPNET_DNSC_src_diab BSPNAME=ti_sitara_ctxa15_branson_1_0_0_0 BUILDSTAGE=BUILD


	
	
	

	
	
	

	

	


	
# copying directory cdf to C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/02comp_ipnet_appl_dnsc.cdf

__FILES_TO_COPY_IPNET_DNSC += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/02comp_ipnet_appl_dnsc.cdf





 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/02comp_ipnet_appl_dnsc.cdf : C:/WindRiverSR500/vxworks-7/pkgs/net/ipnet-1.1.1.3/app/dnsc-1.0.1.5/cdf/02comp_ipnet_appl_dnsc.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif


	
cdf_PRENOBUILD_IPNET_DNSC_FILES = cdf/02comp_ipnet_appl_dnsc.cdf

cdf_PRENOBUILD_IPNET_DNSC_DIR = C:/WindRiverSR500/vxworks-7/pkgs/net/ipnet-1.1.1.3/app/dnsc-1.0.1.5

PRENOBUILD_FILES += $(addprefix $(cdf_PRENOBUILD_IPNET_DNSC_DIR)/,$(cdf_PRENOBUILD_IPNET_DNSC_FILES))

PRENOBUILD : PRENOBUILD_IPNET_DNSC

PRENOBUILD_IPNET_DNSC : cdf_PRENOBUILD_IPNET_DNSC

cdf_PRENOBUILD_IPNET_DNSC : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/cdf_PRENOBUILD_IPNET_DNSC.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/cdf_PRENOBUILD_IPNET_DNSC.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/cdf_PRENOBUILD_IPNET_DNSC.tmp
	$(CDFCOMP) -i $^ -o $@ -cpp "$(CPP) $(__OPTION_LANG_CDF)" -cpu ARMARCH7 -layer IPNET_DNSC

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/cdf_PRENOBUILD_IPNET_DNSC.tmp : | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf
	$(file >$@,$(call __vsb_tmpl_cdf_inter,$(addprefix $(cdf_PRENOBUILD_IPNET_DNSC_DIR)/,$(sort $(cdf_PRENOBUILD_IPNET_DNSC_FILES))),IPNET_DNSC))



	
# copying directory sample_cfg to C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/subprojects




# copying directory sample_cfg/etc to C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/subprojects/etc




POSTBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/subprojects/etc/dnsc.conf

__FILES_TO_COPY_IPNET_DNSC += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/subprojects/etc/dnsc.conf





 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/subprojects/etc/dnsc.conf : C:/WindRiverSR500/vxworks-7/pkgs/net/ipnet-1.1.1.3/app/dnsc-1.0.1.5/sample_cfg/etc/dnsc.conf | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/subprojects/etc
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif




	
# copying directory cfg to C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette/ipdnsc_config.c

__FILES_TO_COPY_IPNET_DNSC += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette/ipdnsc_config.c





 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette/ipdnsc_config.c : C:/WindRiverSR500/vxworks-7/pkgs/net/ipnet-1.1.1.3/app/dnsc-1.0.1.5/cfg/ipdnsc_config.c | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif



	
__DIR_TARGETS += $(__DIR_TARGETS_IPNET_DNSC)

__DIR_TARGETS_IPNET_DNSC += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/subprojects/etc C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/share/h C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/vsblCdf

