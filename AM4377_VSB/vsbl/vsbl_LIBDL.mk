# Automatically generated file: do not edit

##########################
# LIBDL Section
##########################


ifdef _WRS_CONFIG_LIBDL
VSBL_LIBDL_SRC = 
VSBL_LIBDL_DEPEND = 
ifdef _WRS_CONFIG_LIBDL_1_0_7_5
ifdef _WRS_CONFIG_LIBDL_6X
VSBL_LIBDL_SRC += LIBDL_6X
endif
ifdef _WRS_CONFIG_LIBDL_7X
VSBL_LIBDL_SRC += LIBDL_7X
endif
VSBL_LIBDL_SRC += LIBDL
VSBL_LIBDL_DEPEND += CORE_KERNEL
VSBL_LIBDL_DEPEND += CORE_RTP
ifdef _WRS_CONFIG_IA
VSBL_LIBDL_DEPEND += IA
endif
ifdef _WRS_CONFIG_ARCH_i86*
VSBL_LIBDL_DEPEND += IA
endif
ifdef _WRS_CONFIG_IA
VSBL_LIBDL_DEPEND += IA
endif
ifdef _WRS_CONFIG_ARCH_i86*
VSBL_LIBDL_DEPEND += IA
endif
VSBL_LIBDL_DEPEND += SYSCALLS_VXWORKS
VSBL_LIBDL_DEPEND += CORE_USER
LIBDL_FASTBUILD = YES
VSBL_LIBDL_PATH = $(WIND_BASE)/pkgs/os/core/ldso-1.0.7.5
VSBL_LIBDL_VERSION = LIBDL_1_0_7_5
endif
endif

