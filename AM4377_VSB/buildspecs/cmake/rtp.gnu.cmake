 
# Toolchain file for building RTP applications against this VSB.
# Copyright (c) 2016 Wind River Systems, Inc. All Rights Reserved.
#
# Must be used from a Wind River environment, i.e. WIND_HOME and
# related environment variables must be set for the build to succeed.
# This file is designed to reside in (VSB_DIR)/cmake
#
# Usage:
# WIND_HOME/wrenv.linux -p vxworks-7
# cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/this/file.cmake
# make
#
# Note: This file is GENERATED from a template.
# cmake caches the output of toolchain files. So if a VSB is
# reconfigured to create a new toolchain file, the cmake
# build directory must be cleaned entirely.
#
#
# modification history
# --------------------
# 18oct16,mob  written

# The syntax of this file has been validated to work with cmake-2.8.7
# - cmake-2.8.9 or later is required for VxWorks Sharedlibs with Diab
# - cmake-3.3.0 or later is required for CROSSCOMPILING_EMULATOR to work
# - cmake-3.6.2 is recommended and has received most testing.
cmake_minimum_required(VERSION 2.8.7)

#########################################
# WindRiver global configuration:
# Auto-detect the VSB_DIR, and convert environment variables to
# cmake macros with forward slashes as appropriate. This base 
# config is necessary for everything else, and must be done early.
#########################################
file(TO_CMAKE_PATH "$ENV{WIND_HOME}" _WIND_HOME)
file(TO_CMAKE_PATH "$ENV{WIND_BASE}" _WIND_BASE)
file(TO_CMAKE_PATH "$ENV{WIND_BUILD_TOOL}" _WIND_BUILD_TOOL)
file(TO_CMAKE_PATH "${CMAKE_CURRENT_LIST_DIR}" MY_DIR)

# cmake requires the executable extension
# because it likes to check for existance of the actual files
if("${CMAKE_HOST_SYSTEM_NAME}" MATCHES "Windows")
set(CMAKE_HOST_EXECUTABLE_EXTENSION .exe)
else()
set(CMAKE_HOST_EXECUTABLE_EXTENSION)
endif()

# Save variables in the cache, such that they can be inspected by users
# and re-configuring is allowed (within some limits) even outside wrenv.
# This also allows throwing an error when the WIND_HOME environment changed.
set(WIND_HOME ${_WIND_HOME} CACHE PATH "The WIND_HOME installation directory [Read-only].")
set(WIND_BASE ${_WIND_BASE} CACHE PATH "The WIND_BASE Platform Root [Read-only].")
set(WIND_BUILD_TOOL ${_WIND_BUILD_TOOL} CACHE PATH "The WIND_BUILD_TOOL Compile tool binding directory [Read-only].")
set(WIND_PLATFORM $ENV{WIND_PLATFORM} CACHE STRING "The WIND_PLATFORM for wrenv [Read-Only].")
mark_as_advanced(WIND_HOME WIND_BASE WIND_PLATFORM WIND_BASE_TOOL)

# Validate that the environment is sane: WIND_HOME in environment *or* cache but not both
if((NOT DEFINED WIND_HOME) OR (NOT DEFINED WIND_BASE) OR (NOT EXISTS ${WIND_HOME}) OR (NOT EXISTS ${WIND_BASE}))
  message(FATAL_ERROR "WIND_HOME or WIND_BASE environment is invalid. Please run cmake from a wrenv Environment for VxWorks!")
endif()
if(NOT DEFINED WIND_PLATFORM)
  message(FATAL_ERROR "WIND_PLATFORM environment is not set. Please run cmake from a wrenv Environment for VxWorks!")
endif()
if(  ((DEFINED ENV{WIND_HOME}) AND NOT ("${WIND_HOME}" STREQUAL "${_WIND_HOME}"))
  OR ((DEFINED ENV{WIND_BASE}) AND NOT ("${WIND_BASE}" STREQUAL "${_WIND_BASE}")) )
  message(FATAL_ERROR "WIND_HOME or WIND_BASE has changed in environment since the last invocation! "
    "Please delete your cache, or clean your environment, and restart.")
endif()

# Support building outside wrenv context, by generating a build command
# that wraps the original one inside "wrenv -p platformname".
if(WIN32)
  set(EXTENSION ".exe")
  set(WRENV "${WIND_HOME}/wrenv.exe")
else()
  set(WRENV "${WIND_HOME}/wrenv.linux")
endif()
set(WIND_CMAKE_MAKE_PROGRAM ${CMAKE_MAKE_PROGRAM} CACHE STRING "Original Make Program before redirecting via wrenv")
mark_as_advanced(WIND_CMAKE_MAKE_PROGRAM)
if((DEFINED CMAKE_MAKE_PROGRAM) AND (NOT ${CMAKE_MAKE_PROGRAM} MATCHES ".*/wrbuild\\..*"))
  if(WIN32)
    file(TO_NATIVE_PATH ${WIND_HOME} WIND_HOME_WIN32)
    file(WRITE ${CMAKE_BINARY_DIR}/wrbuild.bat
      "@echo off\r\n"
      "${WIND_HOME_WIN32}\\wrenv.exe -p ${WIND_PLATFORM} ${CMAKE_MAKE_PROGRAM} %*\r\n")
    set(CMAKE_MAKE_PROGRAM "${CMAKE_BINARY_DIR}/wrbuild.bat" CACHE STRING "Program used to build." FORCE)
  else() #WIN32
    file(WRITE ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/wrbuild.sh "#!/bin/sh\n"
      "exec ${WIND_HOME}/wrenv.linux -p ${WIND_PLATFORM} ${CMAKE_MAKE_PROGRAM} \"\$@\"\n")
    # now copy the temporary into the final destination, setting the permissions
    file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/wrbuild.sh
      DESTINATION ${CMAKE_BINARY_DIR}
      FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
      GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    set(CMAKE_MAKE_PROGRAM "${CMAKE_BINARY_DIR}/wrbuild.sh" CACHE STRING "Program used to build." FORCE)
  endif()
endif()

## for now, we keep the VxWorks generic / Platform definitions local with the VSB.
## once this is stabilized, those definitions may be moved to the VxWorks installation.
#list(APPEND CMAKE_MODULE_PATH "${WIND_BASE}/VxWorksCMakeModules")
list(APPEND CMAKE_MODULE_PATH "${MY_DIR}")

# By moving environment variables into the CMake cache below, we support
# re-configuring (within some limits) even outside the wrenv environment.
set(WIND_HOST_TYPE $ENV{WIND_HOST_TYPE} CACHE STRING "WIND Host Type")
set(WIND_VX7_GNU_HOST_TYPE $ENV{WIND_VX7_GNU_HOST_TYPE} CACHE STRING "WIND Host Type for GNU Compilers")
set(WIND_VX7_DIAB_HOST_TYPE $ENV{WIND_VX7_DIAB_HOST_TYPE} CACHE STRING "WIND Host Type for Diab Compilers")
set(WIND_VX7_ICC12_HOST_TYPE $ENV{WIND_VX7_ICC12_HOST_TYPE} CACHE STRING "WIND Host Type for Intel ICC12 Compiler")
set(WIND_VX7_ICC_HOST_TYPE $ENV{WIND_VX7_ICC_HOST_TYPE} CACHE STRING "WIND Host Type for Intel Compilers")
mark_as_advanced(WIND_HOST_TYPE WIND_VX7_GNU_HOST_TYPE WIND_VX7_DIAB_HOST_TYPE WIND_VX7_ICC12_HOST_TYPE WIND_VX7_ICC_HOST_TYPE)

# Determine the VSB_DIR, if not specified on command-line:
if(NOT VXWORKS_VSB_DIR)
  get_filename_component(VSB_DIR "${MY_DIR}/../.." ABSOLUTE)
else()
  ## TODO how do we force the VSB_DIR into the try-compile run if it was specified on command-line ?
  get_filename_component(VSB_DIR "${VXWORKS_VSB_DIR}" ABSOLUTE)
endif()
# Put the VXWORKS_VSB_DIR as read-only into the cache, for user information.
set(VXWORKS_VSB_DIR C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB CACHE PATH "The VxWorks VSB_DIR [Read-Only]")
mark_as_advanced(VXWORKS_VSB_DIR)

#########################################
# Wind River bdgen counterpart variables:
# This part is designed to be auto-generated inside the VSB during a VSB build.
# Note: When these settings change due to a VSB reconfiguration, the full application
# build tree must be cleaned since cmake caches the toolchain settings.
#
# TODO: These should perhaps be qualified with WRS_ to avoid polluting the namespace.
# But for now, we keep the namespace the same as in Workbench Managed Build,
# to stay close to the WindRiver original macros in Workbench. It should not cause
# much problems since these variables are only needed during cmake initialization phase.
# Once the Platform/VxWorks.cmake has assigned variables to CMAKE properties, the
# "Workbench style macros" should no longer be needed.
#########################################
set(PROJECT_TYPE RTP)
set(DO_STRIP 0)
set(EXPAND_DBG 0)
set(APICHECK_OPTIONS "-warning")

set(VX_CPU_FAMILY arm)
set(CPU ARMARCH7)
set(TOOL_FAMILY gnu)
set(TOOL_VERSION gnu_4_8_1_8)
set(_CC_VERSION gnu_4_8_1_9)
set(TOOL gnu)
set(TOOL_PATH "${WIND_HOME}/compilers/gnu-4.8.1.9/x86-win32/bin")
set(CC_ARCH_SPEC "-t7 -mfpu=vfp -mfloat-abi=softfp")
set(VSB_CONFIG_FILE C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/h/config/vsbConfig.h)
set(LIBPATH "")
set(LIBS "")
set(APICHECK_CMD "apicheck.sh -r \"${VSB_DIR}/usr/apicheck/forbidden_api.lst\" -n \"nmpentium\" -u -s \"$<\" \"$@\" ${APICHECK_OPTIONS}")
set(APICHECK 0)
set(SHAREDLIB_VERSION "")

# Fallback for TOOL_PATH - Allow compiling even if a user doesn't
# have the exact compiler version installed that the toolchain requests.
# Can be enabled by setting the following cache variables:
# - WIND_ALLOW_COMPILER_FALLBACK -- use environment vars for fallback compiler
# - WIND_TOOL_PATH -- use specified TOOL_PATH for fallback compiler
option(WIND_ALLOW_COMPILER_FALLBACK "Allow fallback to installed compiler if the requested version is not found" OFF)
mark_as_advanced(WIND_ALLOW_COMPILER_FALLBACK)
if(WIND_TOOL_PATH AND (NOT WIND_TOOL_PATH STREQUAL TOOL_PATH))
  # An external cache value overrides the toolchain's TOOL_PATH.
  # This makes sense for locally testing a different compiler version,
  # or if the version requested by the toolchain (VSB) is not installed,
  # or to re-use an environment fallback from previous build when the
  # environment is no longer available at reconfigure time.
  message(WARNING "Warning: WIND_TOOL_PATH override:\n"
    "  Toolchain: ${TOOL_PATH}\n"
    "  Override:  ${WIND_TOOL_PATH}\n"
  )
  set(TOOL_PATH ${WIND_TOOL_PATH})
elseif(EXISTS ${TOOL_PATH})
  # Remember original TOOL_PATH used for this build
  set(WIND_TOOL_PATH ${TOOL_PATH} CACHE PATH "Primary Toolchain (compiler) path.")
  mark_as_advanced(WIND_TOOL_PATH)
elseif(WIND_ALLOW_COMPILER_FALLBACK)
  file(TO_CMAKE_PATH ${TOOL_PATH_FALLBACK} _WIND_TOOL_PATH)
  set(WIND_TOOL_PATH ${_WIND_TOOL_PATH} CACHE PATH "Fallback Toolchain (compiler) path.")
  mark_as_advanced(WIND_TOOL_PATH)
  message(WARNING "Warning: Compiler version requested by toolchain not found, using fallback!\n"
    "  Toolchain: ${TOOL_PATH}\n"
    "  Fallback:  ${WIND_TOOL_PATH}\n"
  )
  set(TOOL_PATH ${WIND_TOOL_PATH})
endif()

include_directories(${VSB_DIR}/share/h)
include_directories(SYSTEM ${VSB_DIR}/usr/h/public)
include_directories(SYSTEM ${VSB_DIR}/usr/h)

add_definitions(-D_VX_CPU=_VX_${CPU})
add_definitions(-D_VX_TOOL_FAMILY=${TOOL_FAMILY})
add_definitions(-D_VX_TOOL=${TOOL})
add_definitions(-D_VSB_CONFIG_FILE=\"${VSB_DIR}/h/config/vsbConfig.h\")

# Essential flags for try-compile to work
set(CMAKE_REQUIRED_FLAGS "${CC_ARCH_SPEC} -mrtp -LC:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/usr/lib/common -D_VSB_CONFIG_FILE=\\\"C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/h/config/vsbConfig.h\\\"")
set(CMAKE_REQUIRED_INCLUDES ${VSB_DIR}/share/h ${VSB_DIR}/usr/h/public ${VSB_DIR}/usr/h)
set(CMAKE_REQUIRED_DEFINITIONS -D_VX_CPU=_VX_${CPU} -D_VX_TOOL_FAMILY=${TOOL_FAMILY} -D_VX_TOOL=${TOOL})

# Map the multiple CMAKE_BUILD_TYPE to just two options of VXWORKS_BUILD_TYPE
# This is needed for propoer DKM Link commands.
string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
if(uppercase_CMAKE_BUILD_TYPE MATCHES "^(DEBUG|RELWITHDEBINFO)$")
  set(VXWORKS_BUILD_TYPE DEBUG)
elseif(uppercase_CMAKE_BUILD_TYPE MATCHES "^(|RELEASE|MINSIZEREL)$")
  set(VXWORKS_BUILD_TYPE RELEASE)
else()
  set(VXWORKS_BUILD_TYPE RELEASE)
  message(WARNING "Unknown CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE} !\nSetting VXWORKS_BUILD_TYPE = ${VXWORKS_BUILD_TYPE}")
endif()


# In general, there's a potential issue where the project may want to overwrite the defaults, or append the defaults.
# Note: The -MD -MP must be stripped from original bdgen since cmake does dependencies itself internally.
# Definitions from here will be used in CMAKE_*_FLAGS, in the Platform/VxWorks.cmake module.
# Compilers need to be defined here, since they need to be known early.
set(WIND_C_FLAGS_DEBUG "-gdwarf-3")
set(WIND_C_FLAGS_RELEASE " -O2  -fstrength-reduce -fno-builtin")
set(WIND_C_FLAGS "${CC_ARCH_SPEC} -mrtp -fno-strict-aliasing -D_C99 -D_HAS_C9X -std=c99 -fasm  -O2  -fstrength-reduce -fno-builtin -Wall     -D_VX_CPU=_VX_ARMARCH7 -D_VX_TOOL_FAMILY=gnu -D_VX_TOOL=gnu -DARMEL -DARMEL     ")
set(CMAKE_C_FLAGS_DEBUG_INIT ${WIND_C_FLAGS_DEBUG})
set(CMAKE_C_FLAGS_RELEASE_INIT ${WIND_C_FLAGS_RELEASE})
set(CMAKE_C_FLAGS_INIT ${WIND_C_FLAGS})
set(CMAKE_C_COMPILER  ${WIND_HOME}/compilers/gnu-4.8.1.9/x86-win32/bin/ccarm${CMAKE_HOST_EXECUTABLE_EXTENSION})
# GNU and Intel are auto-discovered, but Diab needs manual specification
if("${TOOL}" STREQUAL "diab")
  set(CMAKE_C_COMPILER_ID Diab)
  set(CMAKE_C_PLATFORM_ID VxWorks)
  set(CMAKE_C_COMPILER_FORCED TRUE)
endif()

set(WIND_CXX_FLAGS_DEBUG "-gdwarf-3")
set(WIND_CXX_FLAGS_RELEASE " -O2  -fstrength-reduce -fno-builtin")
set(WIND_CXX_FLAGS "${CC_ARCH_SPEC} -ansi -mrtp -fno-strict-aliasing -D_C99 -D_HAS_C9X   -O2  -fstrength-reduce -fno-builtin -Wall     -D_VX_CPU=_VX_ARMARCH7 -D_VX_TOOL_FAMILY=gnu -D_VX_TOOL=gnu -DARMEL -DARMEL     ")
set(CMAKE_CXX_FLAGS_DEBUG_INIT ${WIND_CXX_FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_RELEASE_INIT ${WIND_CXX_FLAGS_RELEASE})
set(CMAKE_CXX_FLAGS_INIT ${WIND_CXX_FLAGS})
set(CMAKE_CXX_COMPILER ${WIND_HOME}/compilers/gnu-4.8.1.9/x86-win32/bin/c++arm${CMAKE_HOST_EXECUTABLE_EXTENSION})
# GNU and Intel are auto-discovered, but Diab needs manual specification
if("${TOOL}" STREQUAL "diab")
  set(CMAKE_CXX_COMPILER_ID Diab)
  set(CMAKE_CXX_PLATFORM_ID VxWorks)
  set(CMAKE_CXX_COMPILER_FORCED TRUE)
endif()

# VxWorks always preprocesses assembly, even with the .s extension.
# To support that, more variables need to be overridden than for other languages.
set(ASM_DIALECT)
set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS s;S;asm)
set(CMAKE_ASM_FLAGS_DEBUG_INIT "-gdwarf-3")
set(CMAKE_ASM_FLAGS_RELEASE_INIT " -O2  -fstrength-reduce -fno-builtin")
set(CMAKE_ASM_FLAGS_MINSIZEREL_INIT "-XO -Xsize-opt -DNDEBUG")
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO_INIT "-XO -Xsize-opt -g3 -Xdebug-dwarf3 -Xdebug-inline-on -DNDEBUG")
set(CMAKE_ASM_FLAGS_INIT "${CC_ARCH_SPEC} -mrtp -fno-strict-aliasing -D_C99 -D_HAS_C9X -std=c99 -fasm  -O2  -fstrength-reduce -fno-builtin    -D_VX_CPU=_VX_ARMARCH7 -D_VX_TOOL_FAMILY=gnu -D_VX_TOOL=gnu -DARMEL -DARMEL  -P -xassembler-with-cpp    -DARMEL")
set(CMAKE_ASM_COMPILER ${WIND_HOME}/compilers/gnu-4.8.1.9/x86-win32/bin/ccarm${CMAKE_HOST_EXECUTABLE_EXTENSION})
set(CMAKE_ASM_COMPILER_FORCED TRUE)
# Fake compiler ID allows us to avoid GNU overriding our settings again.
if("${TOOL}" STREQUAL "diab")
  set(CMAKE_ASM_COMPILER_ID Diab)
else()
  set(CMAKE_ASM_COMPILER_ID GNUVxWorks)
endif()
set(CMAKE_ASM_PLATFORM_ID VxWorks)

## We put these into the CMAKE_C_LINK_EXECUTABLE rule to avoid conflict with -Wl,--start-group for GNU and Intel
#link_directories(${VSB_DIR}/usr/lib/common)
## This variable must be set "late" in the Platform file, because compiler inspection may override it.
set(WIND_EXECUTABLE_SUFFIX ".vxe")
## These go into the cache. Cmake will add $ENV{LDFLAGS} to this!!
set(CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT "")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE_INIT "")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${CC_ARCH_SPEC} -mrtp -fno-strict-aliasing -D_C99 -D_HAS_C9X -std=c99 -fasm  -O2  -fstrength-reduce -fno-builtin -Wall     -D_VX_CPU=_VX_ARMARCH7 -D_VX_TOOL_FAMILY=gnu -D_VX_TOOL=gnu -DARMEL -DARMEL      -Wl,--defsym,__wrs_rtp_base=0x80000000 -mrtp  -Wl,-T${WIND_BUILD_TOOL}/gnu_4_8_1_8/usr/ldscript.vxe.arm -Wl,-u,__tls__")
# Need to define this here due to -Wl,--start-group. Removed CMAKE_C_LINK_FLAGS to avoid duplication
set(CMAKE_C_LINK_EXECUTABLE
    "${CMAKE_CXX_COMPILER} <FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES> -L${VSB_DIR}/usr/lib/common -Wl,--start-group ${LIBS} ${ADDED_LIBS} -Wl,--end-group")
set(CMAKE_CXX_LINK_EXECUTABLE
    "${CMAKE_CXX_COMPILER}  <FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES> -L${VSB_DIR}/usr/lib/common -Wl,--start-group ${LIBS} ${ADDED_LIBS} -Wl,--end-group")
#set(CMAKE_C_LINK_EXECUTABLE
#	"<CMAKE_C_COMPILER> ${LINK_FLAGS_RTP} <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")

# CMAKE_AR gets overridden by CMakeFindBinUtils.cmake, therefore set WIND_AR here and replace in VxWorks.cmake
set(WIND_AR "${WIND_HOME}/compilers/gnu-4.8.1.9/x86-win32/bin/ararm")
# Help finding CMAKE_STRIP and others in CMakeFindBinUtils.cmake, by specifying our prefix and suffix
set(_CMAKE_TOOLCHAIN_PREFIX "")
set(_CMAKE_TOOLCHAIN_SUFFIX "arm")
set(CMAKE_RANLIB "")
## TODO on Windows, response files would be preferred due to commandline length limit
## TODO extract the crus flags into a FLAGS macro so users can change it
#set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> crus <TARGET> <OBJECTS>")
set(CMAKE_C_CREATE_STATIC_LIBRARY "${WIND_AR} crus <TARGET> <OBJECTS>")
set(CMAKE_CXX_CREATE_STATIC_LIBRARY "${WIND_AR} crus <TARGET> <OBJECTS>")
set(CMAKE_ASM_CREATE_STATIC_LIBRARY "${WIND_AR} crus <TARGET> <OBJECTS>")

#### Sharedlib section - see also cmake Platform/Linux.cmake
# Below comes from bdgen .settings/SharedLibraryProject.ref
foreach(lang C CXX ASM)
    # Default flag for linking a sharedlib
    set(CMAKE_SHARED_LIBRARY_LINK_${lang}_FLAGS "-non-static")
    # Default flags for compiling objects into a sharedlib
    # Note: icc Compile FAILS on Linux host if the args are quoted here!
    set(CMAKE_${lang}_COMPILE_OPTIONS_PIC -fpic -D__SO_PICABILINUX__ )
    set(CMAKE_SHARED_LIBRARY_CREATE_${lang}_FLAGS "-fpic -D__SO_PICABILINUX__  -shared")
    #This gets overridden in CMakeGenericSystem.cmake(16) so need to re-override in VxWorks
    set(WIND_SHARED_LIBRARY_CREATE_${lang}_FLAGS ${CMAKE_SHARED_LIBRARY_CREATE_${lang}_FLAGS})
    set(WRS_SHARED_LIBRARY_CREATE_${lang}_FLAGS ${CMAKE_SHARED_LIBRARY_CREATE_${lang}_FLAGS})
    set(CMAKE_SHARED_LIBRARY_RUNTIME_${lang}_FLAG "-Wl,-rpath,")
    # Intel and GNU use empty FLAG_SEP, which works because above _FLAG ends not with a space.
    # For Diab with Ninja on Windows, we need "\\;" FLAG_SEP, to avoid sending rpath with backslash.
    # TODO icc with Ninja on Windows also fails, but the "\\;" workaround doesnt work either.
    set(CMAKE_SHARED_LIBRARY_RUNTIME_${lang}_FLAG_SEP "")
    #set(CMAKE_SHARED_LIBRARY_RPATH_LINK_${lang}_FLAG "-Wl,-rpath-link,")
    set(CMAKE_SHARED_LIBRARY_SONAME_${lang}_FLAG "-Wl,-soname,")
    ## the SONAME_FLAG rule placeholder was added in cmake-2.8.9,
    ## and also the _COMPILE_OPTIONS_PIC were introduced then.
    ## Support for cmake older than 2.8.9 is very incomplete !
    ## http://cmake.org/gitweb?p=cmake.git;a=commitdiff;h=e1409ac5
    if(${CMAKE_VERSION} VERSION_LESS "2.8.9")
      set(CMAKE_SHARED_LIBRARY_${lang}_FLAGS ${CMAKE_${lang}_COMPILE_OPTIONS_PIC})
      set(CMAKE_${lang}_CREATE_SHARED_LIBRARY
      "${CMAKE_CXX_COMPILER} <CMAKE_SHARED_LIBRARY_C_FLAGS> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_${lang}_FLAGS> <CMAKE_SHARED_LIBRARY_SONAME_${lang}_FLAG><TARGET_SONAME> -o <TARGET> <OBJECTS> ${ADDED_OBJECTS} ${LIBPATH} -L${VSB_DIR}/usr/lib/common/PIC ${ADDED_LIBPATH} <LINK_LIBRARIES> ${LIBS} ${ADDED_LIBS}")
    else()
      #Use ${CMAKE_CXX_COMPILER} instead of <CMAKE_CXX_COMPILER> in order to force CXX for linking even C-only sharedlibs
      set(CMAKE_${lang}_CREATE_SHARED_LIBRARY
      "${CMAKE_CXX_COMPILER} <CMAKE_SHARED_LIBRARY_C_FLAGS> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_${lang}_FLAGS> <SONAME_FLAG><TARGET_SONAME> -o <TARGET> <OBJECTS> ${ADDED_OBJECTS} ${LIBPATH} -L${VSB_DIR}/usr/lib/common/PIC ${ADDED_LIBPATH} <LINK_LIBRARIES> ${LIBS} ${ADDED_LIBS}")
    endif()
endforeach()


#########################################
# CMake Cross-Compile Configuration (specific)
#########################################
set (CMAKE_SYSTEM_NAME VxWorks)
set (CMAKE_SYSTEM_VERSION 7)
set (CMAKE_CROSSCOMPILING ON)

# Compute default values for VXWORKS_TEST_TARGET and CMAKE_SYSTEM_PROCESSOR
# this is not perfect yet, eg does not consider SMP and LP64.
# By exposing the settings in the cache, we encourage users to set 
# correct values from the outside if necessary.
set(_TEST_TARGET "vxworks7:10.0.0.1:1534")
if(VX_CPU_FAMILY MATCHES simlinux)
  set(_TEST_KERNEL ${WIND_HOME}/vxworks-7/samples/prebuilt_projects/vip_vxsim_linux_gnu/default/vxWorks)
  set(_TEST_TARGET "vxsim")
  set(_CMAKE_SYSTEM_PROCESSOR x86)
elseif(VX_CPU_FAMILY MATCHES simpc)
  set(_TEST_KERNEL ${WIND_HOME}/vxworks-7/samples/prebuilt_projects/vip_vxsim_windows_gnu/default/vxWorks)
  set(_TEST_TARGET "vxsim")
  set(_CMAKE_SYSTEM_PROCESSOR x86)
elseif(VX_CPU_FAMILY MATCHES pentium)
  set(_CMAKE_SYSTEM_PROCESSOR x86)
elseif(VX_CPU_FAMILY MATCHES arm)
  set(_CMAKE_SYSTEM_PROCESSOR arm)
elseif(VX_CPU_FAMILY MATCHES ppc)
  set(_CMAKE_SYSTEM_PROCESSOR ppc)
endif()
set(VXWORKS_CMAKE_SYSTEM_PROCESSOR ${_CMAKE_SYSTEM_PROCESSOR} CACHE STRING "The CMAKE_SYSTEM_PROCESSOR to set for cmake.")
set(VXWORKS_TEST_KERNEL ${_TEST_KERNEL} CACHE FILEPATH "The VxWorks Kernel Image for running tests. Can be overridden by Environment Variable during builds.")
set(VXWORKS_TEST_TARGET ${_TEST_TARGET} CACHE STRING "The VxWorks Target URI for running tests. Can be overridden by Environment Variable during builds.")
set(CMAKE_SYSTEM_PROCESSOR ${VXWORKS_CMAKE_SYSTEM_PROCESSOR})
mark_as_advanced(VXWORKS_CMAKE_SYSTEM_PROCESSOR)

# Emulator for Testing
if("${CMAKE_HOST_SYSTEM_NAME}" MATCHES "Windows")
  # Need a .bat to properly deal with backslash paths
  set(CMAKE_CROSSCOMPILING_EMULATOR ${MY_DIR}/emu_rtp.bat ${CMAKE_CURRENT_BINARY_DIR} ${VXWORKS_TEST_TARGET} ${VXWORKS_TEST_KERNEL})
else()
  set(CMAKE_CROSSCOMPILING_EMULATOR /bin/sh ${MY_DIR}/emu_rtp.sh ${CMAKE_CURRENT_BINARY_DIR} ${VXWORKS_TEST_TARGET} ${VXWORKS_TEST_KERNEL})
endif()

set (VXWORKS TRUE)
# where is the target environment located
set(CMAKE_FIND_ROOT_PATH ${VSB_DIR}/usr)
# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# these are also set in VxWorks.cmake
set(CMAKE_SYSTEM_INCLUDE_PATH "/h/public")
set(CMAKE_SYSTEM_LIBRARY_PATH "/lib/common")
set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so;.a")


#########################################
# Message early if the project type or the VSB appears not to be sane
# to ease troubleshooting, since find_package would fail without VSB
#########################################
get_property(_IN_TC GLOBAL PROPERTY IN_TRY_COMPILE )
if(NOT _IN_TC)
  if(NOT _WRS_TC_LOADED)
    message(STATUS "WIND_HOME:      ${WIND_HOME}")
    message(STATUS "WIND_TOOL_PATH: ${WIND_TOOL_PATH}")
    message(STATUS "VSB_DIR:        ${VSB_DIR}")
    message(STATUS "PROJECT_TYPE:   ${PROJECT_TYPE}")
    set(_WRS_TC_LOADED 1)
  endif()
  if(NOT IS_DIRECTORY ${VSB_DIR})
    message(FATAL_ERROR "VSB is not a directory: ${VSB_DIR}")
  endif()
  if("${PROJECT_TYPE}" MATCHES "RTP")
    if(NOT IS_DIRECTORY ${VSB_DIR}/usr/root/${TOOL_FAMILY}/bin)
      message(FATAL_ERROR "VSB seems not to be built for RTPs: missing ${VSB_DIR}/usr/root/${TOOL_FAMILY}/bin")
    endif()
  elseif("${PROJECT_TYPE}" MATCHES "DKM")
    if(NOT IS_DIRECTORY ${VSB_DIR}/krnl/${CPU}/common)
      message(FATAL_ERROR "VSB seems not to be built: missing ${VSB_DIR}/krnl/${CPU}/common")
    endif()
  else()
	message(FATAL_ERROR "No project type specified use: -DPROJECT_TYPE=[RTP|DKM]")
  endif()
  if(NOT EXISTS ${CMAKE_C_COMPILER})
    message(FATAL_ERROR "Compiler not found:\n"
      "  ${CMAKE_C_COMPILER}\n"
      "Compilers selected by your VSB / CMake Toolchain were not found in your WIND_HOME installation! "
      "Please check TOOL_PATH and _CC_VERSION in the cmake toolchain file:\n"
      "  ${CMAKE_CURRENT_LIST_FILE}\n"
      "Or, add \"-DWIND_ALLOW_COMPILER_FALLBACK=1\" on the cmake command-line for fallback.\n"
    )
  endif()

  #cmake_policy(SET CMP0056 NEW)
endif()
