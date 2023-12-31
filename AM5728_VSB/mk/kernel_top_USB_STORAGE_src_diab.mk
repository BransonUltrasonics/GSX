
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






LIBOBJS = $(LIBOBJS_usbstorage)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_usbstorage_kernel_top_USB_STORAGE_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objusbstorage/usb2Msc.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objusbstorage/usb2MscBBB.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objusbstorage/usb2MscCBI.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objusbstorage/usb2MscUAS.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objusbstorage/usb2MscBLK.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objusbstorage/usb2MscCommandSet.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objusbstorage/usb2MscXBD.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objusbstorage/usb2MscDirectAccess.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objusbstorage/usb2MscCompliance.o 

arcmdfile_krnl_ARMARCH7_common_usbstorage : $(LIBOBJS_krnl_ARMARCH7_common_usbstorage_kernel_top_USB_STORAGE_src_diab)

LIBOBJS_krnl_ARMARCH7_common_usbstorage_USB_STORAGE += $(LIBOBJS_krnl_ARMARCH7_common_usbstorage_kernel_top_USB_STORAGE_src_diab)

LIBOBJS_krnl_ARMARCH7_common_usbstorage += $(LIBOBJS_krnl_ARMARCH7_common_usbstorage_kernel_top_USB_STORAGE_src_diab)

__OBJS_TO_CLEAN_USB_STORAGE += $(LIBOBJS_krnl_ARMARCH7_common_usbstorage_kernel_top_USB_STORAGE_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_usbstorage_kernel_top_USB_STORAGE_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_usbstorage

_DONEBASE_LIB_krnl_ARMARCH7_common_usbstorage = TRUE

# LIBBASES += usbstorage

__LIBS_TO_BUILD += krnl_ARMARCH7_common_usbstorage

__LIBS_TO_BUILD_USB_STORAGE += krnl_ARMARCH7_common_usbstorage

__BUILT_LIBS += krnl_ARMARCH7_common_usbstorage

__LIB_krnl_ARMARCH7_common_usbstorage = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libusbstorage$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objusbstorage :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libusbstorage$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libusbstorage.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libusbstorage.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libusbstorage$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libusbstorage$(OPT).a libusbstorage$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libusbstorage.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libusbstorage.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libusbstorage.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_usbstorage

arcmdfile_krnl_ARMARCH7_common_usbstorage :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_usbstorage))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libusbstorage$(OPT).a : arcmdfile_krnl_ARMARCH7_common_usbstorage
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_usbstorage


clean _layer_clean_USB_STORAGE : _clean_LIB_BASE_krnl_ARMARCH7_common_usbstorage

_clean_LIB_BASE_krnl_ARMARCH7_common_usbstorage :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libusbstorage$(OPT).a


endif

ifndef _DONEBASE_LIB_USB_STORAGE_krnl_ARMARCH7_common_usbstorage

_DONEBASE_LIB_USB_STORAGE_krnl_ARMARCH7_common_usbstorage = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerUSB_STORAGE_libusbstorage.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerUSB_STORAGE_libusbstorage.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libusbstorage$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,USB_STORAGE,libusbstorage.a,$(LIBOBJS_krnl_ARMARCH7_common_usbstorage)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libusbstorage$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_usbstorage)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
