
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
#	LIBDIRBASE = krnl/ARMARCH7/common
#	LIBDIRBASE = krnl/$(CPU)$(CPU_OPTION_SUFFIX)/$(TOOL_COMMON_DIR)$(LIB_DIR_TAG)$(MINOR_OPTION_SUFFIX)






LIBOBJS = $(LIBOBJS_bootapp)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_bootapp_kernel_top_BOOTAPP_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootAddMem.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootApp.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootAppExc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootAppShell.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootAppTestFuncs.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootDhcpcLoad.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootEdrSupport.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootEndSupport.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootEthAdrSet.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootEthMacHandler.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootFsLoad.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootFtpLoad.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootMemCmds.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootNetLoad.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootRshLoad.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootTftpLoad.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootUsbShow.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/inflateLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/uncompress.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootElfFuncBind.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootElfLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootLoadLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/bootMmu.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp/armAKernelEntry.o 

arcmdfile_krnl_ARMARCH7_common_bootapp : $(LIBOBJS_krnl_ARMARCH7_common_bootapp_kernel_top_BOOTAPP_src_diab)

LIBOBJS_krnl_ARMARCH7_common_bootapp_BOOTAPP += $(LIBOBJS_krnl_ARMARCH7_common_bootapp_kernel_top_BOOTAPP_src_diab)

LIBOBJS_krnl_ARMARCH7_common_bootapp += $(LIBOBJS_krnl_ARMARCH7_common_bootapp_kernel_top_BOOTAPP_src_diab)

__OBJS_TO_CLEAN_BOOTAPP += $(LIBOBJS_krnl_ARMARCH7_common_bootapp_kernel_top_BOOTAPP_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_bootapp_kernel_top_BOOTAPP_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_bootapp

_DONEBASE_LIB_krnl_ARMARCH7_common_bootapp = TRUE

# LIBBASES += bootapp

__LIBS_TO_BUILD += krnl_ARMARCH7_common_bootapp

__LIBS_TO_BUILD_BOOTAPP += krnl_ARMARCH7_common_bootapp

__BUILT_LIBS += krnl_ARMARCH7_common_bootapp

__LIB_krnl_ARMARCH7_common_bootapp = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbootapp$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objbootapp :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbootapp$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbootapp.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbootapp.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbootapp$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common && $(NM) libbootapp$(OPT).a libbootapp$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbootapp.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbootapp.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libbootapp.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_bootapp

arcmdfile_krnl_ARMARCH7_common_bootapp :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_bootapp))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbootapp$(OPT).a : arcmdfile_krnl_ARMARCH7_common_bootapp
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_bootapp


clean _layer_clean_BOOTAPP : _clean_LIB_BASE_krnl_ARMARCH7_common_bootapp

_clean_LIB_BASE_krnl_ARMARCH7_common_bootapp :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbootapp$(OPT).a


endif

ifndef _DONEBASE_LIB_BOOTAPP_krnl_ARMARCH7_common_bootapp

_DONEBASE_LIB_BOOTAPP_krnl_ARMARCH7_common_bootapp = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerBOOTAPP_libbootapp.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerBOOTAPP_libbootapp.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbootapp$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,BOOTAPP,libbootapp.a,$(LIBOBJS_krnl_ARMARCH7_common_bootapp)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libbootapp$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_bootapp)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
