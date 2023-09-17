# Diab-ASM.cmake -- Diab Assembler definitions for VxWorks
# Copyright (c) 2016 Wind River Systems, Inc. All Rights Reserved.
#
# For ASM compilers, cmake overwrites definitions from the toolchain
# file with default GNU settings. Therefore the toolchain file alone
# is not sufficient. By providing this compiler definition, the GNU
# override is suppressed, and toolchain file settings are preserved.
#
# modification history
# --------------------
# 18oct16,mob  written

set(CMAKE_ASM_OUTPUT_EXTENSION .o)
set(CMAKE_ASM_OUTPUT_EXTENSION_REPLACE 1)

set(CMAKE_ASM_VERBOSE_FLAG -v )
set(CMAKE_ASM_COMPILE_OPTIONS_PIC -Xpic )
set(CMAKE_SHARED_LIBRARY_ASM_FLAGS -Xpic )
set(CMAKE_SHARED_LIBRARY_CREATE_ASM_FLAGS "-Xshared -Xdynamic" )
get_property(_IN_TC GLOBAL PROPERTY IN_TRY_COMPILE )
if(NOT _IN_TC OR CMAKE_FORCE_DEPFILES )
  set(CMAKE_DEPFILE_FLAGS_ASM -Xmake-dependency=0xd -Xmake-dependency-savefile=<OBJECT> -Xmake-dependency-savefile=<DEPFILE> )
endif()
set(CMAKE_ASM_CREATE_PREPROCESSED_SOURCE <CMAKE_ASM_COMPILER> <DEFINES> <FLAGS> -E <SOURCE> > <PREPROCESSED_SOURCE> )
set(CMAKE_ASM_CREATE_ASSEMBLY_SOURCE <CMAKE_ASM_COMPILER> <DEFINES> <FLAGS> -S <SOURCE> -o <ASSEMBLY_SOURCE> )
set(CMAKE_INCLUDE_SYSTEM_FLAG_ASM -I )
