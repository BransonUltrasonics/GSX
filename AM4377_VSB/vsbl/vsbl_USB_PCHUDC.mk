# Automatically generated file: do not edit

##########################
# USB_PCHUDC Section
##########################


ifdef _WRS_CONFIG_USB_PCHUDC
VSBL_USB_PCHUDC_SRC = 
VSBL_USB_PCHUDC_DEPEND = 
ifdef _WRS_CONFIG_USB_1_1_0_6_CTLR_1_2_0_4_PCHUDC_1_0_0_5
VSBL_USB_PCHUDC_SRC += USB_PCHUDC
VSBL_USB_PCHUDC_DEPEND += USB_CCORE
VSBL_USB_PCHUDC_DEPEND += USB_TCORE
ifdef _WRS_CONFIG_VXBUS
VSBL_USB_PCHUDC_DEPEND += USB_PHY
endif
USB_PCHUDC_FASTBUILD = YES
VSBL_USB_PCHUDC_PATH = $(WIND_BASE)/pkgs/connectivity/usb-1.1.0.6/ctlr/pchudc-1.0.0.5
VSBL_USB_PCHUDC_VERSION = USB_1_1_0_6_CTLR_1_2_0_4_PCHUDC_1_0_0_5
endif
endif

