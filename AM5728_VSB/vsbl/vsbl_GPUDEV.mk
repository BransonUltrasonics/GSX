# Automatically generated file: do not edit

##########################
# GPUDEV Section
##########################


ifdef _WRS_CONFIG_GPUDEV
VSBL_GPUDEV_SRC = 
VSBL_GPUDEV_DEPEND = 
ifdef _WRS_CONFIG_GPUDEV_1_0_0_2
ifdef _WRS_CONFIG_GPUDEV_COMMON
VSBL_GPUDEV_SRC += GPUDEV_COMMON
endif
ifdef _WRS_CONFIG_GPUDEV_DRM
VSBL_GPUDEV_SRC += GPUDEV_DRM
endif
ifdef _WRS_CONFIG_GPUDEV_FSLVIVGPU
VSBL_GPUDEV_SRC += GPUDEV_FSLVIVGPU
endif
ifdef _WRS_CONFIG_GPUDEV_FSLVIVGPU_DEMOS
VSBL_GPUDEV_SRC += GPUDEV_FSLVIVGPU_DEMOS
endif
ifdef _WRS_CONFIG_GPUDEV_ITLI915
VSBL_GPUDEV_SRC += GPUDEV_ITLI915
endif
ifdef _WRS_CONFIG_GPUDEV_LIBDRM
VSBL_GPUDEV_SRC += GPUDEV_LIBDRM
endif
ifdef _WRS_CONFIG_GPUDEV_LIBDRM_DEMOS
VSBL_GPUDEV_SRC += GPUDEV_LIBDRM_DEMOS
endif
ifdef _WRS_CONFIG_GPUDEV_SAMPLEDRM
VSBL_GPUDEV_SRC += GPUDEV_SAMPLEDRM
endif
VSBL_GPUDEV_SRC += GPUDEV
GPUDEV_FASTBUILD = YES
VSBL_GPUDEV_PATH = $(WIND_BASE)/pkgs/ui/gpudev-1.0.0.2
VSBL_GPUDEV_VERSION = GPUDEV_1_0_0_2
endif
endif

