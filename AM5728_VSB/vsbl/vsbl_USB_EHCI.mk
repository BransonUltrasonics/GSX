# Automatically generated file: do not edit

##########################
# USB_EHCI Section
##########################


ifdef _WRS_CONFIG_USB_EHCI
VSBL_USB_EHCI_SRC = 
VSBL_USB_EHCI_DEPEND = 
ifdef _WRS_CONFIG_USB_1_1_0_6_CTLR_1_2_0_4_EHCI_1_0_2_0
VSBL_USB_EHCI_SRC += USB_EHCI
VSBL_USB_EHCI_DEPEND += USB_CCORE
VSBL_USB_EHCI_DEPEND += USB_HCORE
ifdef _WRS_CONFIG_VXBUS
VSBL_USB_EHCI_DEPEND += USB_PHY
endif
USB_EHCI_FASTBUILD = YES
VSBL_USB_EHCI_PATH = $(WIND_BASE)/pkgs/connectivity/usb-1.1.0.6/ctlr/ehci-1.0.2.0
VSBL_USB_EHCI_VERSION = USB_1_1_0_6_CTLR_1_2_0_4_EHCI_1_0_2_0
endif
endif

