# Automatically generated file: do not edit

##########################
# USB_CLASS Section
##########################


ifdef _WRS_CONFIG_USB_CLASS
VSBL_USB_CLASS_SRC = 
VSBL_USB_CLASS_DEPEND = 
ifdef _WRS_CONFIG_USB_1_1_0_6_HOST_1_0_0_4_CLASS_1_0_0_3
ifdef _WRS_CONFIG_USB_HELPER
VSBL_USB_CLASS_SRC += USB_HELPER
endif
ifdef _WRS_CONFIG_USB_HID
VSBL_USB_CLASS_SRC += USB_HID
endif
ifdef _WRS_CONFIG_USB_KEYBOARD
VSBL_USB_CLASS_SRC += USB_KEYBOARD
endif
ifdef _WRS_CONFIG_USB_MOUSE
VSBL_USB_CLASS_SRC += USB_MOUSE
endif
ifdef _WRS_CONFIG_USB_NETWORK
VSBL_USB_CLASS_SRC += USB_NETWORK
endif
ifdef _WRS_CONFIG_USB_PRINTER
VSBL_USB_CLASS_SRC += USB_PRINTER
endif
ifdef _WRS_CONFIG_USB_SERIAL
VSBL_USB_CLASS_SRC += USB_SERIAL
endif
ifdef _WRS_CONFIG_USB_STORAGE
VSBL_USB_CLASS_SRC += USB_STORAGE
endif
ifdef _WRS_CONFIG_USB_TOUCHSCREEN
VSBL_USB_CLASS_SRC += USB_TOUCHSCREEN
endif
VSBL_USB_CLASS_SRC += USB_CLASS
USB_CLASS_FASTBUILD = YES
VSBL_USB_CLASS_PATH = $(WIND_BASE)/pkgs/connectivity/usb-1.1.0.6/host/class
VSBL_USB_CLASS_VERSION = USB_1_1_0_6_HOST_1_0_0_4_CLASS_1_0_0_3
endif
endif

