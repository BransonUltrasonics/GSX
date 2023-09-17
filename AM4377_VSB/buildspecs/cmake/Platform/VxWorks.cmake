# VxWorks.cmake -- Platform Definitions for VxWorks
# Copyright (c) 2016 Wind River Systems, Inc. All Rights Reserved.
#
# This file is auto-included by cmake, when CMAKE_SYSTEM = VxWorks
# It is included "late" so allows overriding improper defaults
# from the cmake auto-discovery process where appropriate.
#
# modification history
# --------------------
# 18oct16,mob  written

set(PROJECT_TYPE_OK 0)
if("${PROJECT_TYPE}" MATCHES "RTP")
	set(PROJECT_TYPE_OK 1)
elseif("${PROJECT_TYPE}" MATCHES "DKM")
	set(PROJECT_TYPE_OK 1)
endif()
if(NOT ${PROJECT_TYPE_OK})
	message(FATAL_ERROR "No project type specified use: -DPROJECT_TYPE=[RTP|DKM]")
endif()

set(CMAKE_C_OUTPUT_EXTENSION .o)
set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)
set(CMAKE_CXX_OUTPUT_EXTENSION .o)
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
set(CMAKE_ASM_OUTPUT_EXTENSION .o)
set(CMAKE_ASM_OUTPUT_EXTENSION_REPLACE 1)
# .vxe or .out is set in the toolchain file - users may override
set(CMAKE_EXECUTABLE_SUFFIX ${WIND_EXECUTABLE_SUFFIX})

# The GNU Language discovery process for C and CXX overrides our
# CMAKE_C_FLAGS_INIT and related settings from the Toolchain file.
# Since we really need OUR cross flags and not the discovered ones,
# these are force-overridden back here. Note: This may also mean
# that environment variables are not considered for the _INIT flags.
foreach(lang C CXX)
  foreach(c "" _DEBUG _RELEASE _MINSIZEREL _RELWITHDEBINFO)
    set(CMAKE_${lang}_FLAGS${c}_INIT "${WIND_${lang}_FLAGS${c}}")
  endforeach()
endforeach()

macro(cmake_to_msys_path CmakePath ResultingPath)
  #Convert c:/Foo -> /C/Foo , making sure that the drive letter is capitalized
  string(REGEX REPLACE "([a-zA-Z]):/.*" "\\1" _DriveLetter "${CmakePath}")
  if(_DriveLetter)
    string(TOUPPER "${_DriveLetter}" _upperDriveLetter)
    string(REGEX REPLACE "^([a-zA-Z]):/" "/${_upperDriveLetter}/" ${ResultingPath} "${CmakePath}")
  else()
    #string(REGEX REPLACE "^([a-zA-Z]):/" "/\\1/" ${ResultingPath} "${CmakePath}")
    set(${ResultingPath} "${CmakePath}")
  endif()
endmacro()

if("${PROJECT_TYPE}" MATCHES "RTP")
  # don't use the full RPATH for the build tree since it's not visible on the target
  SET(CMAKE_SKIP_BUILD_RPATH  TRUE)
  # when building, use the install RPATH already
  SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 

  #This is just a default to be overridden in the project
  cmake_to_msys_path(${VSB_DIR}/usr/root/${TOOL}/bin _VSB_ROOT_BIN)
  set(CMAKE_INSTALL_RPATH /romfs/bin /hostfs/root${_VSB_ROOT_BIN} /wrdbg/root${_VSB_ROOT_BIN} )

  # don't add the automatically determined parts of the RPATH
  # which point to directories outside the build tree to the install RPATH
  SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
  
  foreach(lang C CXX ASM)
    # This needs to be specified "late" or cmake would override it
    set(CMAKE_SHARED_LIBRARY_CREATE_${lang}_FLAGS "${WIND_SHARED_LIBRARY_CREATE_C_FLAGS}")
  endforeach()

  ## VxWorks Library libdl.a has dlopen (or, rather, _rtld_dlopen) and friends - but for RTP only
  ## TODO -ldl fails on SIMLINUX due to duplicate symbols with libc.a
  #set(CMAKE_DL_LIBS "dl")
  # Handled via Linker Command in --start-group section to avoid duplicate symbols
  set(CMAKE_DL_LIBS "")
	
elseif("${PROJECT_TYPE}" MATCHES "DKM")
  # No -ldl (Sharedlib / dynamic loader) for Kernel on VxWorks.
  # As a result, any Kernel libraries will be linked static.
  set(BUILD_SHARED_LIBS OFF)
  set(CMAKE_DL_LIBS "")
endif()

set(CMAKE_AR ${WIND_AR})
set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_LINK_LIBRARY_SUFFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")

# find_path and find_library support
set(CMAKE_SYSTEM_INCLUDE_PATH "/h/public")
set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
if("${PROJECT_TYPE}" MATCHES "RTP")
  set(CMAKE_SYSTEM_LIBRARY_PATH "/lib/common")
  # TODO this is not quite correct since building sharedlibs
  # is different than finding or linking sharedlibs. 
  # Also, this should be set in the toolchain file to allow user influence.
  if(BUILD_SHARED_LIBS)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".so;.a")
  else()
     set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
  endif()
else()
  set(CMAKE_SYSTEM_LIBRARY_PATH "/$(CPU)/common")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
endif()

# For Auto-discovery purpopses, VxWorks is similar enough to UNIX
# This helps porting software in most cases. It can be turned off
# in a custom toolchain with WIND_VXWORKS_IS_NOT_UNIX if necessary.
set(VXWORKS TRUE)
if(NOT WIND_VXWORKS_IS_NOT_UNIX)
set(UNIX TRUE)
endif()

# Workaround for FindOpenSSL.cmake not knowing VxWorks library names,
# and even failing when the CRYPTO library is not there.
# TODO The proper openssl library name should be contributed to cmake.
find_library(OPENSSL_SSL_LIBRARY OPENSSL)
find_library(OPENSSL_CRYPTO_LIBRARY OPENSSL)
mark_as_advanced(OPENSSL_SSL_LIBRARY OPENSSL_CRYPTO_LIBRARY)
