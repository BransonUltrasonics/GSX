
	
# this file is automatically generated by mk/krnl/defs.layers.mk. Please do not edit

__FILE_COPIED += $(__FILES_COPIED_VXDBG)




	
	

	
# copying directory h to C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/vxdbgLib.h

__FILES_TO_COPY_VXDBG += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/vxdbgLib.h





 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/vxdbgLib.h : C:/WindRiverSR500/vxworks-7/pkgs/os/debug/vxdbg-1.0.6.6/h/vxdbgLib.h | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/vxdbgLibInit.h

__FILES_TO_COPY_VXDBG += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/vxdbgLibInit.h





C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/vxdbgLibInit.h : C:/WindRiverSR500/vxworks-7/pkgs/os/debug/vxdbg-1.0.6.6/h/vxdbgLibInit.h | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
# copying directory h/private to C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private/vxdbgBpMsgQLibP.h

__FILES_TO_COPY_VXDBG += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private/vxdbgBpMsgQLibP.h





 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private/vxdbgBpMsgQLibP.h : C:/WindRiverSR500/vxworks-7/pkgs/os/debug/vxdbg-1.0.6.6/h/private/vxdbgBpMsgQLibP.h | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private/vxdbgUtilLibP.h

__FILES_TO_COPY_VXDBG += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private/vxdbgUtilLibP.h





C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private/vxdbgUtilLibP.h : C:/WindRiverSR500/vxworks-7/pkgs/os/debug/vxdbg-1.0.6.6/h/private/vxdbgUtilLibP.h | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private/vxdbgCpuLibP.h

__FILES_TO_COPY_VXDBG += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private/vxdbgCpuLibP.h





C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private/vxdbgCpuLibP.h : C:/WindRiverSR500/vxworks-7/pkgs/os/debug/vxdbg-1.0.6.6/h/private/vxdbgCpuLibP.h | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private/vxdbgLibP.h

__FILES_TO_COPY_VXDBG += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private/vxdbgLibP.h





C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private/vxdbgLibP.h : C:/WindRiverSR500/vxworks-7/pkgs/os/debug/vxdbg-1.0.6.6/h/private/vxdbgLibP.h | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif





	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


	
	
	
	
	
	
	
BUILD : VXDBG_src_diab__BUILD

.PHONY : VXDBG_src_diab__BUILD

__BUILD_FB_IDS += kernel_top_VXDBG_src_diab

VXDBG_src_diab__BUILD : 
	@ echo building VXDBG directory src
	+ @ cd C:/WindRiverSR500/vxworks-7/pkgs/os/debug/vxdbg-1.0.6.6/src && C:/WindRiverSR500/vxworks-7/host/binutils/x86-win32/bin/make _mk CPU=ARMARCH7 TOOL=diab TOOL_VERSION=diab_5_9_6_1 _CC_VERSION=diab_5_9_6_1 SPACE=kernel _VSB_CONFIG_ADDEDCFLAGS="" _WRS_CONFIG_APP_TOOL=diab VSBL_NAME=VXDBG VSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB _WRS_CONFIG_FP=vector _WRS_CLI_CFLAGS="-I. " _FB_ID=kernel_top_VXDBG_src_diab BSPNAME=ti_sitara_ctxa15_branson_1_0_0_0 BUILDSTAGE=BUILD


	
	
	

	
	
	

	

	


	
# copying directory cdf to C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/01vxdbg.cdf

__FILES_TO_COPY_VXDBG += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/01vxdbg.cdf





 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/01vxdbg.cdf : C:/WindRiverSR500/vxworks-7/pkgs/os/debug/vxdbg-1.0.6.6/cdf/01vxdbg.cdf | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif


	
cdf_PRENOBUILD_VXDBG_FILES = cdf/01vxdbg.cdf

cdf_PRENOBUILD_VXDBG_DIR = C:/WindRiverSR500/vxworks-7/pkgs/os/debug/vxdbg-1.0.6.6

PRENOBUILD_FILES += $(addprefix $(cdf_PRENOBUILD_VXDBG_DIR)/,$(cdf_PRENOBUILD_VXDBG_FILES))

PRENOBUILD : PRENOBUILD_VXDBG

PRENOBUILD_VXDBG : cdf_PRENOBUILD_VXDBG

cdf_PRENOBUILD_VXDBG : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/cdf_PRENOBUILD_VXDBG.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/cdf_PRENOBUILD_VXDBG.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/cdf_PRENOBUILD_VXDBG.tmp
	$(CDFCOMP) -i $^ -o $@ -cpp "$(CPP) $(__OPTION_LANG_CDF)" -cpu ARMARCH7 -layer VXDBG

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/cdf_PRENOBUILD_VXDBG.tmp : | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf
	$(file >$@,$(call __vsb_tmpl_cdf_inter,$(addprefix $(cdf_PRENOBUILD_VXDBG_DIR)/,$(sort $(cdf_PRENOBUILD_VXDBG_FILES))),VXDBG))



	

	
# copying directory configlette to C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette/usrVxdbg.c

__FILES_TO_COPY_VXDBG += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette/usrVxdbg.c





 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette/usrVxdbg.c : C:/WindRiverSR500/vxworks-7/pkgs/os/debug/vxdbg-1.0.6.6/configlette/usrVxdbg.c | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette/usrBreakpoint.c

__FILES_TO_COPY_VXDBG += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette/usrBreakpoint.c





C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette/usrBreakpoint.c : C:/WindRiverSR500/vxworks-7/pkgs/os/debug/vxdbg-1.0.6.6/configlette/usrBreakpoint.c | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif



	
__DIR_TARGETS += $(__DIR_TARGETS_VXDBG)

__DIR_TARGETS_VXDBG += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/private C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/share/h C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/vsblCdf
