# Automatically generated file: do not edit

##########################
# SOCKET Section
##########################


ifdef _WRS_CONFIG_SOCKET
VSBL_SOCKET_SRC = 
VSBL_SOCKET_DEPEND = 
ifdef _WRS_CONFIG_SOCKET_1_0_3_2
VSBL_SOCKET_SRC += SOCKET
VSBL_SOCKET_DEPEND += CORE_IO
ifdef _WRS_CONFIG_SYSCALLS_VXWORKS
VSBL_SOCKET_DEPEND += SYSCALLS_VXWORKS
endif
ifdef _WRS_CONFIG_SYSCALLS_VXWORKS*
VSBL_SOCKET_DEPEND += SYSCALLS_VXWORKS
endif
SOCKET_FASTBUILD = YES
VSBL_SOCKET_PATH = $(WIND_BASE)/pkgs/os/service/socket-1.0.3.2
VSBL_SOCKET_VERSION = SOCKET_1_0_3_2
endif
endif

