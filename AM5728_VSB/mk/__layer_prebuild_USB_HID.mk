
	
# this file is automatically generated by mk/krnl/defs.layers.mk. Please do not edit

__FILE_COPIED += $(__FILES_COPIED_USB_HID)




	
	

	
# copying directory h to C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public




PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/usbhid.h

__FILES_TO_COPY_USB_HID += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/usbhid.h





 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/usbhid.h : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/usb-1.1.0.6/host/class/hid-1.0.0.3/h/usbhid.h | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/usb_hid_usages

__FILES_TO_COPY_USB_HID += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/usb_hid_usages





C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/usb_hid_usages : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/usb-1.1.0.6/host/class/hid-1.0.0.3/h/usb_hid_usages | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif
 
PRENOBUILD : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/usb2Hid.h

__FILES_TO_COPY_USB_HID += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/usb2Hid.h





C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public/usb2Hid.h : C:/WindRiverSR500/vxworks-7/pkgs/connectivity/usb-1.1.0.6/host/class/hid-1.0.0.3/h/usb2Hid.h | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public
ifdef __LAYER_DEPENDENCY_TEST
	cp -rfLs $< $@
else
	cp -rfL $< $@
endif




	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


	
	
	
	
	
	
	
BUILD : USB_HID_src_diab__BUILD

.PHONY : USB_HID_src_diab__BUILD

__BUILD_FB_IDS += kernel_top_USB_HID_src_diab

USB_HID_src_diab__BUILD : 
	@ echo building USB_HID directory src
	+ @ cd C:/WindRiverSR500/vxworks-7/pkgs/connectivity/usb-1.1.0.6/host/class/hid-1.0.0.3/src && C:/WindRiverSR500/vxworks-7/host/binutils/x86-win32/bin/make _mk CPU=ARMARCH7 TOOL=diab TOOL_VERSION=diab_5_9_6_1 _CC_VERSION=diab_5_9_6_1 SPACE=kernel _VSB_CONFIG_ADDEDCFLAGS="" _WRS_CONFIG_APP_TOOL=diab VSBL_NAME=USB_HID VSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB _WRS_CONFIG_FP=vector _WRS_CLI_CFLAGS="-I. " _FB_ID=kernel_top_USB_HID_src_diab BSPNAME=ti_sitara_ctxa15_branson_1_0_0_0 BUILDSTAGE=BUILD


	
	
	

	
	
	

	

	


	
	
	

	

	
__DIR_TARGETS += $(__DIR_TARGETS_USB_HID)

__DIR_TARGETS_USB_HID += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/configlette C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/h/public C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/share/h C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/vsblCdf

