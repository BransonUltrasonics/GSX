# Automatically generated file: do not edit

##########################
# ACPI Section
##########################


ifdef _WRS_CONFIG_ACPI
VSBL_ACPI_SRC = 
VSBL_ACPI_DEPEND = 
ifdef _WRS_CONFIG_ACPI_5_1_1_0_0_2
VSBL_ACPI_SRC += ACPI
ifdef _WRS_CONFIG_IA_KERNEL
VSBL_ACPI_DEPEND += IA_KERNEL
endif
ifdef _WRS_CONFIG_ARCH_i86*
VSBL_ACPI_DEPEND += IA_KERNEL
endif
ACPI_FASTBUILD = YES
VSBL_ACPI_PATH = $(WIND_BASE)/pkgs/os/firmware/acpi/acpi_5_1-1.0.0.2
VSBL_ACPI_VERSION = ACPI_5_1_1_0_0_2
endif
ifdef _WRS_CONFIG_ACPI_6_1_1_0_0_0
VSBL_ACPI_SRC += ACPI
ifdef _WRS_CONFIG_IA_KERNEL
VSBL_ACPI_DEPEND += IA_KERNEL
endif
ifdef _WRS_CONFIG_ARCH_i86*
VSBL_ACPI_DEPEND += IA_KERNEL
endif
ACPI_FASTBUILD = YES
VSBL_ACPI_PATH = $(WIND_BASE)/pkgs/os/firmware/acpi/acpi_6_1-1.0.0.0
VSBL_ACPI_VERSION = ACPI_6_1_1_0_0_0
endif
ifdef _WRS_CONFIG_ACPI_4_1_0_0_2
VSBL_ACPI_SRC += ACPI
ifdef _WRS_CONFIG_IA_KERNEL
VSBL_ACPI_DEPEND += IA_KERNEL
endif
ifdef _WRS_CONFIG_ARCH_i86*
VSBL_ACPI_DEPEND += IA_KERNEL
endif
ACPI_FASTBUILD = YES
VSBL_ACPI_PATH = $(WIND_BASE)/pkgs/os/firmware/acpi/acpi_4-1.0.0.2
VSBL_ACPI_VERSION = ACPI_4_1_0_0_2
endif
endif

