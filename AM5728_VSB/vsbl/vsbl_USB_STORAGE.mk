# Automatically generated file: do not edit

##########################
# USB_STORAGE Section
##########################


ifdef _WRS_CONFIG_USB_STORAGE
VSBL_USB_STORAGE_SRC = 
VSBL_USB_STORAGE_DEPEND = 
ifdef _WRS_CONFIG_USB_1_1_0_6_HOST_1_0_0_4_CLASS_1_0_0_3_STORAGE_1_0_0_10
VSBL_USB_STORAGE_SRC += USB_STORAGE
VSBL_USB_STORAGE_DEPEND += FS_COMMON
VSBL_USB_STORAGE_DEPEND += USB_HELPER
USB_STORAGE_FASTBUILD = YES
VSBL_USB_STORAGE_PATH = $(WIND_BASE)/pkgs/connectivity/usb-1.1.0.6/host/class/storage-1.0.0.10
VSBL_USB_STORAGE_VERSION = USB_1_1_0_6_HOST_1_0_0_4_CLASS_1_0_0_3_STORAGE_1_0_0_10
endif
endif
