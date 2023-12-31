
#
# This file is automatically generated by mk/krnl/defs.fastlibgen.mk .
#
# build configuration :
#	VSB : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB
#	CPU : ARMARCH7
#	TOOL : diab
#	FP : vector
# 	ENDIAN : little
#	LIB_DIR_TAG = 
#	LIBDIRBASE = krnl/diab
#	LIBDIRBASE = krnl/$(TOOL_SPECIFIC_DIR)$(LIB_DIR_TAG)$(MINOR_OPTION_SUFFIX)






LIBOBJS = $(LIBOBJS_diabcplus)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_diab_diabcplus_kernel_top_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_src_diab_libd =  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_del.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_new.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_nodel.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_nonew.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_pdel.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_cppalloc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_cxa_eh.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_cxa_eh_diag.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_cxa_except_tab.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_delete.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_delnothrow.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_error.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_guard.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_new.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_newhandler.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_newnothrow.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_nothrow.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_placedel.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_pure_virt.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_rtti.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_set_new.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_setterm.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_setunexp.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_terminate.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_typeinfo.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_unexpected.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_vec_newdel.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/__dplusplus_intrinsics.o

arcmdfile_krnl_diab_diabcplus : $(LIBOBJS_krnl_diab_diabcplus_kernel_top_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_src_diab_libd)

LIBOBJS_krnl_diab_diabcplus_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL += $(LIBOBJS_krnl_diab_diabcplus_kernel_top_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_src_diab_libd)

LIBOBJS_krnl_diab_diabcplus += $(LIBOBJS_krnl_diab_diabcplus_kernel_top_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_src_diab_libd)

__OBJS_TO_CLEAN_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL += $(LIBOBJS_krnl_diab_diabcplus_kernel_top_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_src_diab_libd) $(LIBOBJS_krnl_diab_diabcplus_kernel_top_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_src_diab_libd:.o=.d)

ifndef _DONEBASE_LIB_krnl_diab_diabcplus

_DONEBASE_LIB_krnl_diab_diabcplus = TRUE

# LIBBASES += diabcplus

__LIBS_TO_BUILD += krnl_diab_diabcplus

__LIBS_TO_BUILD_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL += krnl_diab_diabcplus

__BUILT_LIBS += krnl_diab_diabcplus

__LIB_krnl_diab_diabcplus = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/libdiabcplus$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/libdiabcplus$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/libdiabcplus.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/libdiabcplus.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/libdiabcplus$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab && $(NM) libdiabcplus$(OPT).a libdiabcplus$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/libdiabcplus.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/libdiabcplus.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libdiabcplus.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_diab_diabcplus

arcmdfile_krnl_diab_diabcplus :
	$(file >$@,$(LIBOBJS_krnl_diab_diabcplus))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/libdiabcplus$(OPT).a : arcmdfile_krnl_diab_diabcplus
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_diab_diabcplus


clean _layer_clean_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL : _clean_LIB_BASE_krnl_diab_diabcplus

_clean_LIB_BASE_krnl_diab_diabcplus :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/libdiabcplus$(OPT).a


endif

ifndef _DONEBASE_LIB_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_krnl_diab_diabcplus

_DONEBASE_LIB_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_krnl_diab_diabcplus = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/layerLANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_libdiabcplus.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/layerLANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_libdiabcplus.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/libdiabcplus$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL,libdiabcplus.a,$(LIBOBJS_krnl_diab_diabcplus)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/libdiabcplus$(OPT).a : $(LIBOBJS_krnl_diab_diabcplus)




 

################
# rules defined locally in Makefile
##


ifneq (C:/WINDRI~1/COMPIL~1/DIAB-5~1.1/ARMM/vxworks7/libd.a,)
 C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_del.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_new.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_nodel.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_nonew.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_pdel.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_cppalloc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_cxa_eh.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_cxa_eh_diag.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_cxa_except_tab.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_delete.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_delnothrow.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_error.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_guard.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_new.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_newhandler.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_newnothrow.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_nothrow.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_placedel.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_pure_virt.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_rtti.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_set_new.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_setterm.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_setunexp.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_terminate.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_typeinfo.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_unexpected.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_vec_newdel.o : C:/WINDRI~1/COMPIL~1/DIAB-5~1.1/ARMM/vxworks7/libd.a C:/WindRiverSR500/vxworks-7/pkgs/os/lang-lib/tool-1.0.0.3/toolsrc_diab-20.0.4.0/kernel/src/libd/Makefile |  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus
	@echo Wrapping $@ ...
	rm -f $@
# This just adds a symbol __object_o to object.o. We chose
# to use a one-line C fragment rather than using the linker
# to directly add a symbol because this way we don't have
# to worry about whether or not the compiler prepends an underscore.
	echo "char __$(subst +,_,$(subst -,_,$(notdir $*)))_o = 0;" > $(basename $@)_tmp_wrapper.c
	(cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus ; C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar -x C:/WINDRI~1/COMPIL~1/DIAB-5~1.1/ARMM/vxworks7/libd.a $(patsubst _x_diab_%,%,$(notdir $@)) ; mv $(patsubst _x_diab_%,%,$(notdir $@)) $@)
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dcc -c -tARMCORTEXA9MV:vxworks7 -Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP  -Xfp-fast  -Xc-new -Xdialect-c89   -XO -Xsize-opt  -ei4177,4301,4550 -ei4177,4550,2273,5387,5388,1546,5849,1824 -ei4111,4171,4188,4191,4513,5457 -Xforce-declarations     -DCPU=_VX_ARMARCH7 -DTOOL_FAMILY=diab -DTOOL=diab -D_WRS_KERNEL -DARMEL -DARMEL -DARM_USE_VFP -DINET -DARM_USE_VFP  -D_WRS_LIB_BUILD     -D_VSB_CONFIG_FILE=\"C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/h/config/vsbConfig.h\"  -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/system -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/share/h   -I.  $(basename $@)_tmp_wrapper.c -o $(basename $@)_tmp_obj.o
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dld -tARMCORTEXA9MV:vxworks7 -X -r5 -r $(basename $@)_tmp_obj.o $@ -o $@_tmp
	cp $@_tmp $@
	@(if [ "diab" = "gnu" -a "arm" = "arm" ]; then objcopyarm -R .ARM.attributes $@; fi)
	rm -f $(basename $@)_tmp_wrapper.c $(basename $@)_tmp_obj.o $@_tmp

endif # C:/WINDRI~1/COMPIL~1/DIAB-5~1.1/ARMM/vxworks7/libd.a defined

ifneq (__dplusplus_intrinsics.o,)
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/__dplusplus_intrinsics.o :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_del.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_new.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_nodel.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_nonew.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_array_pdel.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_cppalloc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_cxa_eh.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_cxa_eh_diag.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_cxa_except_tab.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_delete.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_delnothrow.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_error.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_guard.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_new.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_newhandler.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_newnothrow.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_nothrow.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_placedel.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_pure_virt.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_rtti.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_set_new.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_setterm.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_setunexp.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_terminate.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_typeinfo.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_unexpected.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/_x_diab_vec_newdel.o  |  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus
	tclsh C:/WindRiverSR500/vxworks-7/build/mk/tcl/genConfig.tcl dplusplus_intrinsics _x_diab_array_del.o _x_diab_array_new.o _x_diab_array_nodel.o _x_diab_array_nonew.o _x_diab_array_pdel.o _x_diab_cppalloc.o _x_diab_cxa_eh.o _x_diab_cxa_eh_diag.o _x_diab_cxa_except_tab.o _x_diab_delete.o _x_diab_delnothrow.o _x_diab_error.o _x_diab_guard.o _x_diab_new.o _x_diab_newhandler.o _x_diab_newnothrow.o _x_diab_nothrow.o _x_diab_placedel.o _x_diab_pure_virt.o _x_diab_rtti.o _x_diab_set_new.o _x_diab_setterm.o _x_diab_setunexp.o _x_diab_terminate.o _x_diab_typeinfo.o _x_diab_unexpected.o _x_diab_vec_newdel.o  > C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/__dplusplus_intrinsics.c
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dcc -c -tARMCORTEXA9MV:vxworks7 -Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP  -Xfp-fast  -Xc-new -Xdialect-c89   -XO -Xsize-opt  -ei4177,4301,4550 -ei4177,4550,2273,5387,5388,1546,5849,1824 -ei4111,4171,4188,4191,4513,5457 -Xforce-declarations     -DCPU=_VX_ARMARCH7 -DTOOL_FAMILY=diab -DTOOL=diab -D_WRS_KERNEL -DARMEL -DARMEL -DARM_USE_VFP -DINET -DARM_USE_VFP  -D_WRS_LIB_BUILD     -D_VSB_CONFIG_FILE=\"C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/h/config/vsbConfig.h\"  -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/system -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/share/h   -I.  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/__dplusplus_intrinsics.c -o $@
	@(if [ "diab" = "gnu" -a "arm" = "arm" ]; then objcopyarm -R .ARM.attributes $@; fi)
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab/objdiabcplus/__dplusplus_intrinsics.c
endif


################

objs : $(LIBOBJS)
 
# this is the end of the file 
