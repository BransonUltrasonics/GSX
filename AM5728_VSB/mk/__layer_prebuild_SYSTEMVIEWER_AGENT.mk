
	
# this file is automatically generated by mk/krnl/defs.layers.mk. Please do not edit

__FILE_COPIED += $(__FILES_COPIED_SYSTEMVIEWER_AGENT)




	
	

	


	
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/sysviewer.h

__FILES_TO_COPY_SYSTEMVIEWER_AGENT += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/sysviewer.h





 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/sysviewer.h : C:/WindRiverSR500/vxworks-7/pkgs/rttools/systemviewer-1.0.0.10/src/src/sysviewer.h | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/sv_symbol.h

__FILES_TO_COPY_SYSTEMVIEWER_AGENT += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/sv_symbol.h





C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/sv_symbol.h : C:/WindRiverSR500/vxworks-7/pkgs/rttools/systemviewer-1.0.0.10/src/src/sv_symbol.h | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


	
	
	
	
	
	
	
BUILD : SYSTEMVIEWER_AGENT_src_diab__BUILD

.PHONY : SYSTEMVIEWER_AGENT_src_diab__BUILD

__BUILD_FB_IDS += kernel_top_SYSTEMVIEWER_AGENT_src_diab

SYSTEMVIEWER_AGENT_src_diab__BUILD : 
	@ echo building SYSTEMVIEWER_AGENT directory src
	+ @ cd C:/WindRiverSR500/vxworks-7/pkgs/rttools/systemviewer-1.0.0.10/src && C:/WindRiverSR500/vxworks-7/host/binutils/x86-win32/bin/make _mk CPU=ARMARCH7 TOOL=diab TOOL_VERSION=diab_5_9_6_1 _CC_VERSION=diab_5_9_6_1 SPACE=kernel _VSB_CONFIG_ADDEDCFLAGS="" _WRS_CONFIG_APP_TOOL=diab VSBL_NAME=SYSTEMVIEWER_AGENT VSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB _WRS_CONFIG_FP=vector _WRS_CLI_CFLAGS="-I. " _FB_ID=kernel_top_SYSTEMVIEWER_AGENT_src_diab BSPNAME=ti_sitara_ctxa15_branson_1_0_0_0 BUILDSTAGE=BUILD


	
	
	

	
	
	

	

	


	
# copying directory src/osconfig/vxworks/cdf to C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/02comp_systemviewer_agent.cdf

__FILES_TO_COPY_SYSTEMVIEWER_AGENT += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/02comp_systemviewer_agent.cdf





 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/02comp_systemviewer_agent.cdf : C:/WindRiverSR500/vxworks-7/pkgs/rttools/systemviewer-1.0.0.10/src/osconfig/vxworks/cdf/02comp_systemviewer_agent.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif


	
src_osconfig_vxworks_cdf_PRENOBUILD_SYSTEMVIEWER_AGENT_FILES = src/osconfig/vxworks/cdf/02comp_systemviewer_agent.cdf

src_osconfig_vxworks_cdf_PRENOBUILD_SYSTEMVIEWER_AGENT_DIR = C:/WindRiverSR500/vxworks-7/pkgs/rttools/systemviewer-1.0.0.10

PRENOBUILD_FILES += $(addprefix $(src_osconfig_vxworks_cdf_PRENOBUILD_SYSTEMVIEWER_AGENT_DIR)/,$(src_osconfig_vxworks_cdf_PRENOBUILD_SYSTEMVIEWER_AGENT_FILES))

PRENOBUILD : PRENOBUILD_SYSTEMVIEWER_AGENT

PRENOBUILD_SYSTEMVIEWER_AGENT : src/osconfig/vxworks/cdf_PRENOBUILD_SYSTEMVIEWER_AGENT

src/osconfig/vxworks/cdf_PRENOBUILD_SYSTEMVIEWER_AGENT : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/src_osconfig_vxworks_cdf_PRENOBUILD_SYSTEMVIEWER_AGENT.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/src_osconfig_vxworks_cdf_PRENOBUILD_SYSTEMVIEWER_AGENT.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/src_osconfig_vxworks_cdf_PRENOBUILD_SYSTEMVIEWER_AGENT.tmp
	$(CDFCOMP) -i $^ -o $@ -cpp "$(CPP) $(__OPTION_LANG_CDF)" -cpu ARMARCH7 -layer SYSTEMVIEWER_AGENT

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/src_osconfig_vxworks_cdf_PRENOBUILD_SYSTEMVIEWER_AGENT.tmp : | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf
	$(file >$@,$(call __vsb_tmpl_cdf_inter,$(addprefix $(src_osconfig_vxworks_cdf_PRENOBUILD_SYSTEMVIEWER_AGENT_DIR)/,$(sort $(src_osconfig_vxworks_cdf_PRENOBUILD_SYSTEMVIEWER_AGENT_FILES))),SYSTEMVIEWER_AGENT))



	

	
# copying directory src/osconfig/vxworks/src to C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette/usrSystemViewer.c

__FILES_TO_COPY_SYSTEMVIEWER_AGENT += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette/usrSystemViewer.c





 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette/usrSystemViewer.c : C:/WindRiverSR500/vxworks-7/pkgs/rttools/systemviewer-1.0.0.10/src/osconfig/vxworks/src/usrSystemViewer.c | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif



	
__DIR_TARGETS += $(__DIR_TARGETS_SYSTEMVIEWER_AGENT)

__DIR_TARGETS_SYSTEMVIEWER_AGENT += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/share/h C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/vsblCdf

