
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
#	LIBDIRBASE = krnl/diab_abridged
#	LIBDIRBASE = krnl/$(TOOL_SPECIFIC_DIR)_$(CXX_STL_VARIANT)$(MINOR_OPTION_SUFFIX)






LIBOBJS = $(LIBOBJS_stl)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_diab_abridged_stl_kernel_top_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_src_diab_libstlabr =  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_fiopen.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_iomanip.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_ios.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_iostream.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_limits.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_locale.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_locale0.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_string.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_strstrea.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_throw.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wiostrea.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wlocale.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xlocale.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xstart.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xsyslock.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xgetglob.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xfvalues.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xvalues.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xlvalues.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wcslen.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemchr.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemcmp.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemcpy.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemmove.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemset.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_raisehan.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/__diab_cplusplus_std_library.o

arcmdfile_krnl_diab_abridged_stl : $(LIBOBJS_krnl_diab_abridged_stl_kernel_top_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_src_diab_libstlabr)

LIBOBJS_krnl_diab_abridged_stl_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL += $(LIBOBJS_krnl_diab_abridged_stl_kernel_top_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_src_diab_libstlabr)

LIBOBJS_krnl_diab_abridged_stl += $(LIBOBJS_krnl_diab_abridged_stl_kernel_top_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_src_diab_libstlabr)

__OBJS_TO_CLEAN_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL += $(LIBOBJS_krnl_diab_abridged_stl_kernel_top_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_src_diab_libstlabr) $(LIBOBJS_krnl_diab_abridged_stl_kernel_top_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_src_diab_libstlabr:.o=.d)

ifndef _DONEBASE_LIB_krnl_diab_abridged_stl

_DONEBASE_LIB_krnl_diab_abridged_stl = TRUE

# LIBBASES += stl

__LIBS_TO_BUILD += krnl_diab_abridged_stl

__LIBS_TO_BUILD_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL += krnl_diab_abridged_stl

__BUILT_LIBS += krnl_diab_abridged_stl

__LIB_krnl_diab_abridged_stl = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/libstl$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/libstl$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/libstl.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/libstl.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/libstl$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged && $(NM) libstl$(OPT).a libstl$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/libstl.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/libstl.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libstl.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_diab_abridged_stl

arcmdfile_krnl_diab_abridged_stl :
	$(file >$@,$(LIBOBJS_krnl_diab_abridged_stl))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/libstl$(OPT).a : arcmdfile_krnl_diab_abridged_stl
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_diab_abridged_stl


clean _layer_clean_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL : _clean_LIB_BASE_krnl_diab_abridged_stl

_clean_LIB_BASE_krnl_diab_abridged_stl :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/libstl$(OPT).a


endif

ifndef _DONEBASE_LIB_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_krnl_diab_abridged_stl

_DONEBASE_LIB_LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_krnl_diab_abridged_stl = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/layerLANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_libstl.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/layerLANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL_libstl.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/libstl$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,LANG_LIB_TOOL_TOOLSRC_DIAB_KERNEL,libstl.a,$(LIBOBJS_krnl_diab_abridged_stl)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/libstl$(OPT).a : $(LIBOBJS_krnl_diab_abridged_stl)




 

################
# rules defined locally in Makefile
##


ifneq (C:/WINDRI~1/COMPIL~1/DIAB-5~1.1/ARMMV/vxworks7/libstlabr.a,)
 C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_fiopen.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_iomanip.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_ios.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_iostream.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_limits.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_locale.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_locale0.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_string.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_strstrea.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_throw.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wiostrea.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wlocale.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xlocale.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xstart.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xsyslock.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xgetglob.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xfvalues.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xvalues.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xlvalues.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wcslen.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemchr.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemcmp.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemcpy.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemmove.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemset.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_raisehan.o : C:/WINDRI~1/COMPIL~1/DIAB-5~1.1/ARMMV/vxworks7/libstlabr.a C:/WindRiverSR500/vxworks-7/pkgs/os/lang-lib/tool-1.0.0.3/toolsrc_diab-20.0.4.0/kernel/src/libstlabr/Makefile |  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl
	@echo Wrapping $@ ...
	rm -f $@
# This just adds a symbol __object_o to object.o. We chose
# to use a one-line C fragment rather than using the linker
# to directly add a symbol because this way we don't have
# to worry about whether or not the compiler prepends an underscore.
	echo "char __$(subst +,_,$(subst -,_,$(notdir $*)))_o = 0;" > $(basename $@)_tmp_wrapper.c
	(cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl ; C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar -x C:/WINDRI~1/COMPIL~1/DIAB-5~1.1/ARMMV/vxworks7/libstlabr.a $(patsubst _x_diab_%,%,$(notdir $@)) ; mv $(patsubst _x_diab_%,%,$(notdir $@)) $@)
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dcc -c -tARMCORTEXA9MV:vxworks7 -Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP  -Xfp-fast  -Xc-new -Xdialect-c89   -XO -Xsize-opt  -ei4177,4301,4550 -ei4177,4550,2273,5387,5388,1546,5849,1824 -ei4111,4171,4188,4191,4513,5457 -Xforce-declarations     -DCPU=_VX_ARMARCH7 -DTOOL_FAMILY=diab -DTOOL=diab -D_WRS_KERNEL -DARMEL -DARMEL -DARM_USE_VFP -DINET -DARM_USE_VFP  -D_WRS_LIB_BUILD     -D_VSB_CONFIG_FILE=\"C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/h/config/vsbConfig.h\"  -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/system -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/share/h   -I.  $(basename $@)_tmp_wrapper.c -o $(basename $@)_tmp_obj.o
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dld -tARMCORTEXA9MV:vxworks7 -X -r5 -r $(basename $@)_tmp_obj.o $@ -o $@_tmp
	cp $@_tmp $@
	@(if [ "diab" = "gnu" -a "arm" = "arm" ]; then objcopyarm -R .ARM.attributes $@; fi)
	rm -f $(basename $@)_tmp_wrapper.c $(basename $@)_tmp_obj.o $@_tmp

endif # C:/WINDRI~1/COMPIL~1/DIAB-5~1.1/ARMMV/vxworks7/libstlabr.a defined

ifneq (__diab_cplusplus_std_library.o,)
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/__diab_cplusplus_std_library.o :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_fiopen.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_iomanip.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_ios.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_iostream.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_limits.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_locale.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_locale0.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_string.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_strstrea.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_throw.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wiostrea.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wlocale.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xlocale.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xstart.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xsyslock.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xgetglob.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xfvalues.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xvalues.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_xlvalues.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wcslen.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemchr.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemcmp.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemcpy.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemmove.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_wmemset.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/_x_diab_raisehan.o  |  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl
	tclsh C:/WindRiverSR500/vxworks-7/build/mk/tcl/genConfig.tcl diab_cplusplus_std_library _x_diab_fiopen.o _x_diab_iomanip.o _x_diab_ios.o _x_diab_iostream.o _x_diab_limits.o _x_diab_locale.o _x_diab_locale0.o _x_diab_string.o _x_diab_strstrea.o _x_diab_throw.o _x_diab_wiostrea.o _x_diab_wlocale.o _x_diab_xlocale.o _x_diab_xstart.o _x_diab_xsyslock.o _x_diab_xgetglob.o _x_diab_xfvalues.o _x_diab_xvalues.o _x_diab_xlvalues.o _x_diab_wcslen.o _x_diab_wmemchr.o _x_diab_wmemcmp.o _x_diab_wmemcpy.o _x_diab_wmemmove.o _x_diab_wmemset.o _x_diab_raisehan.o  > C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/__diab_cplusplus_std_library.c
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dcc -c -tARMCORTEXA9MV:vxworks7 -Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP  -Xfp-fast  -Xc-new -Xdialect-c89   -XO -Xsize-opt  -ei4177,4301,4550 -ei4177,4550,2273,5387,5388,1546,5849,1824 -ei4111,4171,4188,4191,4513,5457 -Xforce-declarations     -DCPU=_VX_ARMARCH7 -DTOOL_FAMILY=diab -DTOOL=diab -D_WRS_KERNEL -DARMEL -DARMEL -DARM_USE_VFP -DINET -DARM_USE_VFP  -D_WRS_LIB_BUILD     -D_VSB_CONFIG_FILE=\"C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/h/config/vsbConfig.h\"  -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/public -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/h/system -IC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/share/h   -I.  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/__diab_cplusplus_std_library.c -o $@
	@(if [ "diab" = "gnu" -a "arm" = "arm" ]; then objcopyarm -R .ARM.attributes $@; fi)
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/diab_abridged/objstl/__diab_cplusplus_std_library.c
endif


################

objs : $(LIBOBJS)
 
# this is the end of the file 
