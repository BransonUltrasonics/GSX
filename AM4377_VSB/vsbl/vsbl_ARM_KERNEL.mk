# Automatically generated file: do not edit

##########################
# ARM_KERNEL Section
##########################


ifdef _WRS_CONFIG_ARM_KERNEL
VSBL_ARM_KERNEL_SRC = 
VSBL_ARM_KERNEL_DEPEND = 
ifdef _WRS_CONFIG_ARM_1_1_9_1_KERNEL_1_0_12_1
ifdef _WRS_CONFIG_ARM_ARMBASE
VSBL_ARM_KERNEL_SRC += ARM_ARMBASE
endif
ifdef _WRS_CONFIG_ARM_ARMV7A
VSBL_ARM_KERNEL_SRC += ARM_ARMV7A
endif
ifdef _WRS_CONFIG_ARM_ARMV7M
VSBL_ARM_KERNEL_SRC += ARM_ARMV7M
endif
ifdef _WRS_CONFIG_ARM_ARMV8A
VSBL_ARM_KERNEL_SRC += ARM_ARMV8A
endif
ifdef _WRS_CONFIG_ARM_DRV
VSBL_ARM_KERNEL_SRC += ARM_DRV
endif
ifdef _WRS_CONFIG_ARM_MATH_fp
VSBL_ARM_KERNEL_SRC += ARM_MATH_fp
endif
VSBL_ARM_KERNEL_SRC += ARM_KERNEL
VSBL_ARM_KERNEL_DEPEND += CORE_KERNEL
VSBL_ARM_KERNEL_DEPEND += CORE_KERNEL
ARM_KERNEL_FASTBUILD = YES
VSBL_ARM_KERNEL_PATH = $(WIND_BASE)/pkgs/os/arch/arm-1.1.9.1/kernel
VSBL_ARM_KERNEL_VERSION = ARM_1_1_9_1_KERNEL_1_0_12_1
endif
endif
