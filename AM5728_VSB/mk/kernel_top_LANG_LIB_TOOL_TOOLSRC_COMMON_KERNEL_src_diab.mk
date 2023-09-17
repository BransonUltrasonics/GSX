
#
# This file is automatically generated by mk/krnl/defs.fastlibgen.mk .
#
# build configuration :
#	VSB : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB
#	CPU : ARMARCH7
#	TOOL : diab
#	FP : vector
# 	ENDIAN : little
#	LIB_DIR_TAG = 
#	LIBDIRBASE = krnl/ARMARCH7/common
#	LIBDIRBASE = krnl/$(CPU)$(CPU_OPTION_SUFFIX)/$(TOOL_COMMON_DIR)$(LIB_DIR_TAG)$(MINOR_OPTION_SUFFIX)



include kernel_top_LANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL_src_diab_misc.mk



LIBOBJS = $(LIBOBJS_commoncc)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_commoncc_kernel_top_LANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objcommoncc/arm_call_via.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objcommoncc/__common_intrinsics.o

arcmdfile_krnl_ARMARCH7_common_commoncc : $(LIBOBJS_krnl_ARMARCH7_common_commoncc_kernel_top_LANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL_src_diab)

LIBOBJS_krnl_ARMARCH7_common_commoncc_LANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_commoncc_kernel_top_LANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL_src_diab)

LIBOBJS_krnl_ARMARCH7_common_commoncc += $(LIBOBJS_krnl_ARMARCH7_common_commoncc_kernel_top_LANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL_src_diab)

__OBJS_TO_CLEAN_LANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_commoncc_kernel_top_LANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_commoncc_kernel_top_LANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_commoncc

_DONEBASE_LIB_krnl_ARMARCH7_common_commoncc = TRUE

# LIBBASES += commoncc

__LIBS_TO_BUILD += krnl_ARMARCH7_common_commoncc

__LIBS_TO_BUILD_LANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL += krnl_ARMARCH7_common_commoncc

__BUILT_LIBS += krnl_ARMARCH7_common_commoncc

__LIB_krnl_ARMARCH7_common_commoncc = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libcommoncc$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objcommoncc :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libcommoncc$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libcommoncc.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libcommoncc.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libcommoncc$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libcommoncc$(OPT).a libcommoncc$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libcommoncc.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libcommoncc.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libcommoncc.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_commoncc

arcmdfile_krnl_ARMARCH7_common_commoncc :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_commoncc))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libcommoncc$(OPT).a : arcmdfile_krnl_ARMARCH7_common_commoncc
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_commoncc


clean _layer_clean_LANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL : _clean_LIB_BASE_krnl_ARMARCH7_common_commoncc

_clean_LIB_BASE_krnl_ARMARCH7_common_commoncc :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libcommoncc$(OPT).a


endif

ifndef _DONEBASE_LIB_LANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL_krnl_ARMARCH7_common_commoncc

_DONEBASE_LIB_LANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL_krnl_ARMARCH7_common_commoncc = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerLANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL_libcommoncc.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerLANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL_libcommoncc.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libcommoncc$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,LANG_LIB_TOOL_TOOLSRC_COMMON_KERNEL,libcommoncc.a,$(LIBOBJS_krnl_ARMARCH7_common_commoncc)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libcommoncc$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_commoncc)




 

################
# rules defined locally in Makefile
##


ifneq (,)
 :  C:/WindRiverSR500/vxworks-7/pkgs/os/lang-lib/tool-1.0.0.3/common-1.0.3.4/kernel/src/Makefile |  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objcommoncc
	@echo Wrapping $@ ...
	rm -f $@
# This just adds a symbol __object_o to object.o. We chose
# to use a one-line C fragment rather than using the linker
# to directly add a symbol because this way we don't have
# to worry about whether or not the compiler prepends an underscore.
	echo "char __$(subst +,_,$(subst -,_,$(notdir $*)))_o = 0;" > $(basename $@)_tmp_wrapper.c
	(cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objcommoncc ; C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar -x  $(patsubst %,%,$(notdir $@)) ; mv $(patsubst %,%,$(notdir $@)) $@)
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dcc -c -tARMCORTEXA15MV:vxworks7 -Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP  -Xfp-fast  -Xc-new -Xdialect-c89   -XO -Xsize-opt  -ei4177,4301,4550 -ei4177,4550,2273,5387,5388,1546,5849,1824 -ei4111,4171,4188,4191,4513,5457 -Xforce-declarations     -DCPU=_VX_ARMARCH7 -DTOOL_FAMILY=diab -DTOOL=diab -D_WRS_KERNEL -DARMEL -DINET -DARM_USE_VFP  -D_WRS_LIB_BUILD     -D_VSB_CONFIG_FILE=\"C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/h/config/vsbConfig.h\"  -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/system -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/share/h   -I.  $(basename $@)_tmp_wrapper.c -o $(basename $@)_tmp_obj.o
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dld -tARMCORTEXA15MV:vxworks7 -X -r5 -r $(basename $@)_tmp_obj.o $@ -o $@_tmp
	cp $@_tmp $@
	@(if [ "diab" = "gnu" -a "arm" = "arm" ]; then objcopyarm -R .ARM.attributes $@; fi)
	rm -f $(basename $@)_tmp_wrapper.c $(basename $@)_tmp_obj.o $@_tmp

endif #  defined

ifneq (__common_intrinsics.o,)
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objcommoncc/__common_intrinsics.o :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objcommoncc/arm_call_via.o  |  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objcommoncc
	tclsh C:/WindRiverSR500/vxworks-7/build/mk/tcl/genConfig.tcl common_intrinsics arm_call_via.o > C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objcommoncc/__common_intrinsics.c
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dcc -c -tARMCORTEXA15MV:vxworks7 -Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP  -Xfp-fast  -Xc-new -Xdialect-c89   -XO -Xsize-opt  -ei4177,4301,4550 -ei4177,4550,2273,5387,5388,1546,5849,1824 -ei4111,4171,4188,4191,4513,5457 -Xforce-declarations     -DCPU=_VX_ARMARCH7 -DTOOL_FAMILY=diab -DTOOL=diab -D_WRS_KERNEL -DARMEL -DINET -DARM_USE_VFP  -D_WRS_LIB_BUILD     -D_VSB_CONFIG_FILE=\"C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/h/config/vsbConfig.h\"  -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/system -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/share/h   -I.  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objcommoncc/__common_intrinsics.c -o $@
	@(if [ "diab" = "gnu" -a "arm" = "arm" ]; then objcopyarm -R .ARM.attributes $@; fi)
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objcommoncc/__common_intrinsics.c
endif


################

objs : $(LIBOBJS)
 
# this is the end of the file 