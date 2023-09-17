#* @(#) pSOSystem SPL/V2.5.0-F8: configs/spl/config.mk 4.7 00/02/03 14:36:27 *
#***********************************************************************
#*                                                                     *
#*   MODULE:  configs/std/config.mk                                    *
#*   DATE:    00/02/03                                                 *
#*   PURPOSE: Macro definitions, linker commands, etc, for "make"      *
#*                                                                     *
#*---------------------------------------------------------------------*
#*                                                                     *
#*        Copyright 1991 - 2000, Integrated Systems, Inc.              *
#*                      ALL RIGHTS RESERVED                            *
#*                                                                     *
#*   Permission is hereby granted to licensees of Integrated Systems,  *
#*   Inc. products to use or abstract this computer program for the    *
#*   sole purpose of implementing a product based on Integrated        *
#*   Systems, Inc. products.   No other rights to reproduce, use,      *
#*   or disseminate this computer program, whether in part or in       *
#*   whole, are granted.                                               *
#*                                                                     *
#*   Integrated Systems, Inc. makes no representation or warranties    *
#*   with respect to the performance of this computer program, and     *
#*   specifically disclaims any responsibility for any damages,        *
#*   special or consequential, connected with the use of this program. *
#*                                                                     *
#*---------------------------------------------------------------------*
#***********************************************************************
#
#------------------------------------------------------------------------
# Compiler commands and command-line options
#------------------------------------------------------------------------

#------------------------------------------------------------------------
# Note the CC and CXX commands are continued on several lines.
#
# Only echo the "Making..." string if the silent flags was not given.
#
# The command `echo ${MAKEFLAGS} | sed 's/[^s]//g'` echoes the make flags
# to the standard input of the sed command. The sed command replaces
# every character that is not an s with nothing. So if the silent flag
# was given the left side of the equals results in "Xs". If the silent
# flag was not given it would result in "X".
#------------------------------------------------------------------------
CC           = @if [ "X`echo ${MAKEFLAGS} | sed 's/[^s]//g'`" = "X" ]; \
                        then \
                        echo "Making     $@  \tfrom   $<"; \
                        fi; \
                dcc
 
CXX          = @if [ "X`echo ${MAKEFLAGS} | sed 's/[^s]//g'`" = "X" ]; \
                        then \
                        echo "Making     $@  \tfrom   $<"; \
                        fi; \
                dplus

#------------------------------------------------------------------------
# Add following option to COPTS_FILE2 to generate listing files
# -W:as:,-l -Xpass-source
#------------------------------------------------------------------------
COMP_OPTS1   = -O -g -t$(DTARGET)E$(DFP):psos -Xstruct-best-align=0
COMP_OPTS2   = -Xforce-prototypes -Xstrict-ansi $(PSS_APPCOPTS) -D__DIAB -DSPL
COPTS_FILE1  = -I. -I$(PSS_ROOT)/include -I$(PSS_BSP) $(PSS_APPINCS)
COPTS_FILE2  = -I$(PSS_ROOT)/bsps/devices $(PSS_VINCLUDE) -I@ -c
CXX_OPTS     = -Xexception

#------------------------------------------------------------------------
# COPT_PER_FILE is used to provide a convenient way to add per
# file compile options. If a .cop file for a source file exist then
# content of this file is passed as the compile options.
#------------------------------------------------------------------------
COPT_PER_FILE=`if [ -f ./$(*F).cop ]; then cat ./$(*F).cop; fi`

COPTS        = -@c.opt $(COPT_PER_FILE)
CXXOPTS      = -@cxx.opt $(COPT_PER_FILE)

#------------------------------------------------------------------------
# TARGET_CPU is used in application makefiles to pass on to incremental
# linking and other tools.
#------------------------------------------------------------------------
TARGET_CPU   = -t$(DTARGET)E$(DFP):psos

#------------------------------------------------------------------------
# DEVICES_CPU_DIR is used by application makefiles to make them generic
# across platforms.
#------------------------------------------------------------------------
DEVICES_CPU_DIR = $(PSS_ROOT)/bsps/devices/powerpc

#------------------------------------------------------------------------
# Assembler command-line options
#
# Note the AS command is continued on several lines.
#
# Only echo the "Making..." string if the silent flags was not given.
#
# The command `echo ${MAKEFLAGS} | sed 's/[^s]//g'` echoes the make flags
# to the standard input of the sed command. The sed command replaces
# every character that is not an s with nothing. So if the silent flag
# was given the left side of the equals results in "Xs". If the silent
# flag was not given it would result in "X".
#------------------------------------------------------------------------
AS           = @if [ "X`echo ${MAKEFLAGS} | sed 's/[^s]//g'`" = "X" ]; \
                        then \
                        echo "Making     $@  \tfrom   $<"; \
                        fi; \
                das

#------------------------------------------------------------------------
# Added target switch for the assembler
# The target definition is located in bsp.mk of the bsp you are using
#------------------------------------------------------------------------
# Add following option to AOPTS_FILE1 to generate listing files  -l
#------------------------------------------------------------------------
AOPTS_FILE1  = -I. $(PSS_APPINCS) -I$(PSS_ROOT)/include -I$(PSS_BSP)
AOPTS_FILE2  = -I$(PSS_ROOT)/bsps/devices $(PSS_VINCLUDE) -I@
AOPTS_FILE3  = -t$(DTARGET)E$(DFP):psos
AOPTS        = -@a.opt $(COPT_PER_FILE)

#------------------------------------------------------------------------
# Linker command name
#------------------------------------------------------------------------
LD      = dld
LOPTS   = -t$(DTARGET)E$(DFP):psos -Xsuppress-dot -Xreloc-bug -m2 $(PSS_APPLOPTS)

#------------------------------------------------------------------------
# Librarian command name and options
#------------------------------------------------------------------------
LIB          = dar
LIBOPTS      = -qf
LIBOPTS_SORT = -sR

#========================================================================
# Link everything together 
#========================================================================
CFG1_OBJS= sysinit.o  bspcfg.o   pnacfg.o   prepccfg.o dialog.o
CFG2_OBJS= probecfg.o prpccfg.o  psecfg.o   psoscfg.o  pmontcfg.o
CFG3_OBJS= pollio.o   philecfg.o gsblkcfg.o plmcfg.o   pnetcfg.o
CFG4_OBJS= ldcfg.o    posixcfg.o rtacfg.o   

ram.elf: cxx.opt c.opt a.opt ill.opt begin.o compiler.o \
	makefile \
        ram.dld \
	$(CFG1_OBJS) \
	$(CFG2_OBJS) \
	$(CFG3_OBJS) \
	$(CFG4_OBJS) \
	$(PSS_DRVOBJS) \
	$(PSS_APPOBJS) \
	$(PSS_BSP)/libbsp.a \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_ROOT)/sys/libc/libprepc$(DFP).a \
	$(PSS_ROOT)/sys/libc/libpnet.a
	@echo $(LOPTS) -o ram.elf -Wm ram.dld begin.o    > l.opt
	@echo -L $(PSS_BSP) -L $(PSS_ROOT)/sys/os       >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc $(PSS_APPLINKS)   >> l.opt
	@echo $(PSS_VLINK)                              >> l.opt
	@echo compiler.o                                >> l.opt
	@echo $(CFG1_OBJS)                              >> l.opt
	@echo $(CFG2_OBJS)                              >> l.opt
	@echo $(CFG3_OBJS)                              >> l.opt
	@echo $(CFG4_OBJS)                              >> l.opt
	@echo $(PSS_APPOBJS)                            >> l.opt
	@echo $(APP_LIB1)                               >> l.opt
	@echo $(APP_LIB2)                               >> l.opt
	@echo $(APP_LIB3)                               >> l.opt
	@echo $(APP_LIB4)                               >> l.opt
	@echo $(APP_LIB5)                               >> l.opt
	@echo $(PSS_DRVOBJS)                            >> l.opt
	@echo $(DRV_LIB1)                               >> l.opt
	@echo $(DRV_LIB2)                               >> l.opt
	@echo $(DRV_LIB3)                               >> l.opt
	@echo $(DRV_LIB4)                               >> l.opt
	@echo $(DRV_LIB5)                               >> l.opt
	@echo -lbsp -ldrv                               >> l.opt
	@echo -lsys                                     >> l.opt
	@echo -lprepc$(DFP) -lpnet -lquery              >> l.opt
	@echo $(FIX_LIB)                                >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > ram.map
	ddump -tv ram.elf >> ram.map

ram.dld: $(PSS_BSP)/anchor.dld \
        $(PSS_BSP)/ram.dld
	@cat $(PSS_BSP)/ram.dld       > ram.dld
	@cat $(PSS_BSP)/anchor.dld   >> ram.dld

ram.hex: ram.elf
	ddump -Rv ram.elf -o ram.hex

rom.elf: cxx.opt c.opt a.opt ill.opt rombegin.o compiler.o \
	makefile \
        rom.dld \
	$(CFG1_OBJS) \
	$(CFG2_OBJS) \
	$(CFG3_OBJS) \
	$(CFG4_OBJS) \
	$(PSS_DRVOBJS) \
	$(PSS_BSP)/libbsp.a \
	$(PSS_APPOBJS) \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_ROOT)/sys/libc/libprepc$(DFP).a \
	$(PSS_ROOT)/sys/libc/libpnet.a
	@echo $(LOPTS) -o rom.elf -Wm rom.dld rombegin.o    > l.opt
	@echo -L $(PSS_BSP) -L $(PSS_ROOT)/sys/os       >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc $(PSS_APPLINKS)   >> l.opt
	@echo $(PSS_VLINK)                              >> l.opt
	@echo compiler.o                                >> l.opt
	@echo $(CFG1_OBJS)                              >> l.opt
	@echo $(CFG2_OBJS)                              >> l.opt
	@echo $(CFG3_OBJS)                              >> l.opt
	@echo $(CFG4_OBJS)                              >> l.opt
	@echo $(PSS_APPOBJS)                            >> l.opt
	@echo $(APP_LIB1)                               >> l.opt
	@echo $(APP_LIB2)                               >> l.opt
	@echo $(APP_LIB3)                               >> l.opt
	@echo $(APP_LIB4)                               >> l.opt
	@echo $(APP_LIB5)                               >> l.opt
	@echo $(PSS_DRVOBJS)                            >> l.opt
	@echo $(DRV_LIB1)                               >> l.opt
	@echo $(DRV_LIB2)                               >> l.opt
	@echo $(DRV_LIB3)                               >> l.opt
	@echo $(DRV_LIB4)                               >> l.opt
	@echo $(DRV_LIB5)                               >> l.opt
	@echo -lbsp -ldrv                               >> l.opt
	@echo -lsys                                     >> l.opt
	@echo -lprepc$(DFP) -lpnet -lquery              >> l.opt
	@echo $(FIX_LIB)                                >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > rom.map
	ddump -tv rom.elf >> rom.map

rom.dld: $(PSS_BSP)/anchor.dld \
        $(PSS_BSP)/rom.dld
	@cat $(PSS_BSP)/rom.dld       > rom.dld
	@cat $(PSS_BSP)/anchor.dld   >> rom.dld

rom.hex: rom.elf
	ddump -Rv rom.elf -o rom.hex

os.elf: cxx.opt c.opt a.opt ill.opt begin.o \
	makefile \
        os.dld \
	$(CFG1_OBJS) \
	$(CFG2_OBJS) \
	$(CFG3_OBJS) \
	$(CFG4_OBJS) \
	$(PSS_DRVOBJS) \
	$(PSS_BSP)/libbsp.a \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_ROOT)/sys/libc/libprepc$(DFP).a \
	$(PSS_ROOT)/sys/libc/libpnet.a
	@echo $(LOPTS) -o os.elf -Wm os.dld begin.o      > l.opt
	@echo -L $(PSS_BSP) -L $(PSS_ROOT)/sys/os       >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc $(PSS_APPLINKS)   >> l.opt
	@echo $(PSS_VLINK)                              >> l.opt
	@echo $(CFG1_OBJS)                              >> l.opt
	@echo $(CFG2_OBJS)                              >> l.opt
	@echo $(CFG3_OBJS)                              >> l.opt
	@echo $(CFG4_OBJS)                              >> l.opt
	@echo $(PSS_DRVOBJS)                            >> l.opt
	@echo $(DRV_LIB1)                               >> l.opt
	@echo $(DRV_LIB2)                               >> l.opt
	@echo $(DRV_LIB3)                               >> l.opt
	@echo $(DRV_LIB4)                               >> l.opt
	@echo $(DRV_LIB5)                               >> l.opt
	@echo -lbsp -ldrv                               >> l.opt
	@echo -lsys                                     >> l.opt
	@echo -lprepc$(DFP) -lpnet -lquery              >> l.opt
	@echo $(FIX_LIB)                                >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > os.map
	ddump -tv os.elf >> os.map

os.dld: $(PSS_BSP)/anchor.dld \
        $(PSS_BSP)/os.dld
	@cat $(PSS_BSP)/os.dld        > os.dld
	@cat $(PSS_BSP)/anchor.dld   >> os.dld

os.hex: os.elf
	ddump -Rv os.elf -o os.hex

app.elf: cxx.opt c.opt a.opt ill.opt beginapp.o bspcalls.o compiler.o \
	makefile \
        app.dld \
	$(PSS_APPOBJS) \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_ROOT)/sys/libc/libprepc$(DFP).a
	@echo $(LOPTS) -o app.elf -Wm app.dld beginapp.o > l.opt
	@echo bspcalls.o                                >> l.opt
	@echo -L $(PSS_BSP) -L $(PSS_ROOT)/sys/os       >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc $(PSS_APPLINKS)   >> l.opt
	@echo $(PSS_VLINK)                              >> l.opt
	@echo compiler.o                                >> l.opt
	@echo $(PSS_APPOBJS)                            >> l.opt
	@echo $(APP_LIB1)                               >> l.opt
	@echo $(APP_LIB2)                               >> l.opt
	@echo $(APP_LIB3)                               >> l.opt
	@echo $(APP_LIB4)                               >> l.opt
	@echo $(APP_LIB5)                               >> l.opt
	@echo -lsys                                     >> l.opt
	@echo -lprepc$(DFP) -lquery                     >> l.opt
	@echo $(FIX_LIB)                                >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > app.map
	ddump -tv app.elf >> app.map

app.dld: $(PSS_BSP)/anchor.dld \
        $(PSS_BSP)/app.dld
	@cat $(PSS_BSP)/app.dld       > app.dld
	@cat $(PSS_BSP)/anchor.dld   >> app.dld

app.hex: app.elf
	ddump -R app.elf -o app.hex

app_ld.elf: cxx.opt c.opt a.opt ill.opt ldbegin.o bspcalls.o compiler.o \
	ldblcfg.o makefile \
        app_ld.dld \
	$(PSS_APPOBJS) \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_ROOT)/sys/libc/libprepc$(DFP).a
	@echo $(LOPTS) -o app_tmp.elf -Wm app_ld.dld     > l.opt
	@echo ldbegin.o bspcalls.o                      >> l.opt
	@echo -L $(PSS_BSP) -L $(PSS_ROOT)/sys/os       >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc $(PSS_APPLINKS)   >> l.opt
	@echo $(PSS_VLINK)                              >> l.opt
	@echo compiler.o                                >> l.opt
	@echo $(PSS_APPOBJS)                            >> l.opt
	@echo $(APP_LIB1)                               >> l.opt
	@echo $(APP_LIB2)                               >> l.opt
	@echo $(APP_LIB3)                               >> l.opt
	@echo $(APP_LIB4)                               >> l.opt
	@echo $(APP_LIB5)                               >> l.opt
	@echo -lsys                                     >> l.opt
	@echo -lprepc$(DFP) -lquery                     >> l.opt
	@echo $(FIX_LIB)                                >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > app_ld.map
	ddump -tv app_tmp.elf >> app_ld.map
	@echo $(PSS_LDBOPTS) -p 180 -n LDBL -mAT -u 1024 -s 8192 > ld_prep.def
	@ld_prep -v -d ld_prep.def -o app_ld.elf app_tmp.elf
	@rm -f ld_prep.def

app_ld.hex: cxx.opt c.opt a.opt ill.opt ldbegin.o bspcalls.o compiler.o \
	ldblcfg.o makefile \
        app_ld.dld \
	$(PSS_APPOBJS) \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_ROOT)/sys/libc/libprepc$(DFP).a
	@echo $(LOPTS) -o app_tmp.elf -Wm app_ld.dld     > l.opt
	@echo ldbegin.o bspcalls.o                      >> l.opt
	@echo -L $(PSS_BSP) -L $(PSS_ROOT)/sys/os       >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc $(PSS_APPLINKS)   >> l.opt
	@echo $(PSS_VLINK)                              >> l.opt
	@echo compiler.o                                >> l.opt
	@echo $(PSS_APPOBJS)                            >> l.opt
	@echo $(APP_LIB1)                               >> l.opt
	@echo $(APP_LIB2)                               >> l.opt
	@echo $(APP_LIB3)                               >> l.opt
	@echo $(APP_LIB4)                               >> l.opt
	@echo $(APP_LIB5)                               >> l.opt
	@echo -lsys                                     >> l.opt
	@echo -lprepc$(DFP) -lquery                     >> l.opt
	@echo $(FIX_LIB)                                >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > app_ld.map
	ddump -tv app_tmp.elf >> app_ld.map
	ddump -R app_tmp.elf -o app_tmp.hex
	@echo $(PSS_LDBOPTS) -p 180 -n LDBL -mAT -u 1024 -s 8192 > ld_prep.def
	@ld_prep -v -d ld_prep.def -o app_ld.hex app_tmp.hex
	@rm -f ld_prep.def

app_ld.dld: $(PSS_BSP)/anchor.dld \
        $(PSS_BSP)/app.dld
	@cat $(PSS_BSP)/app.dld       > app_ld.dld
	@cat $(PSS_BSP)/anchor.dld   >> app_ld.dld

driver.elf: cxx.opt c.opt a.opt ill.opt lddriver.o bspcalls.o compiler.o \
	ldblcfg.o \
	makefile \
        driver.dld \
	$(PSS_APPOBJS) \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_ROOT)/sys/libc/libprepc$(DFP).a
	@echo $(LOPTS) -o driver.elf driver.dld lddriver.o  > l.opt
	@echo bspcalls.o                                   >> l.opt
	@echo -L $(PSS_BSP) -L $(PSS_ROOT)/sys/os          >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc $(PSS_APPLINKS)      >> l.opt
	@echo $(PSS_VLINK)                                 >> l.opt
	@echo compiler.o                                   >> l.opt
	@echo $(PSS_APPOBJS)                               >> l.opt
	@echo $(APP_LIB1)                                  >> l.opt
	@echo $(APP_LIB2)                                  >> l.opt
	@echo $(APP_LIB3)                                  >> l.opt
	@echo $(APP_LIB4)                                  >> l.opt
	@echo $(APP_LIB5)                                  >> l.opt
	@echo -lsys                                        >> l.opt
	@echo -lprepc$(DFP)                                >> l.opt
	@echo $(FIX_LIB)                                   >> l.opt
	@echo $(PSS_COMPLIB)                               >> l.opt
	$(LD) -@l.opt > driver.map
	@ddump -tv driver.elf >> driver.map

driver.dld: $(PSS_BSP)/anchor.dld \
        $(PSS_BSP)/app.dld
	@cat $(PSS_BSP)/app.dld       > driver.dld
	@cat $(PSS_BSP)/anchor.dld   >> driver.dld

lddriver.elf: driver.elf
	@echo $(PSS_LDBOPTS) -p 180 -n LDBL -mAT -u 1024 -s 4096 > ld_prep.def
	@ld_prep -v -d ld_prep.def -o lddriver.elf driver.elf
	@rm -f ld_prep.def

lib.elf: cxx.opt c.opt a.opt ill.opt bspcalls.o compiler.o \
	makefile \
        lib.dld \
	$(PSS_APPOBJS) \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_ROOT)/sys/libc/libprepc$(DFP).a
	@echo $(LOPTS) -o lib.elf lib.dld                > l.opt
	@echo bspcalls.o                                >> l.opt
	@echo -L $(PSS_BSP) -L $(PSS_ROOT)/sys/os       >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc $(PSS_APPLINKS)   >> l.opt
	@echo $(PSS_VLINK)                              >> l.opt
	@echo compiler.o                                >> l.opt
	@echo $(PSS_APPOBJS)                            >> l.opt
	@echo $(APP_LIB1)                               >> l.opt
	@echo $(APP_LIB2)                               >> l.opt
	@echo $(APP_LIB3)                               >> l.opt
	@echo $(APP_LIB4)                               >> l.opt
	@echo $(APP_LIB5)                               >> l.opt
	@echo -lsys                                     >> l.opt
	@echo -lprepc$(DFP) -lquery                     >> l.opt
	@echo $(FIX_LIB)                                >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > lib.map
	@ddump -tv lib.elf >> lib.map

lib.dld: $(PSS_BSP)/anchor.dld \
        $(PSS_BSP)/app.dld
	@cat $(PSS_BSP)/app.dld       > lib.dld
	@cat $(PSS_BSP)/anchor.dld   >> lib.dld

lib_ld.elf: lib.elf
	@echo $(PSS_LDBOPTS) > ld_prep.def
	@ld_prep -v -d ld_prep.def -o lib_ld.elf lib.elf
	@rm -f ld_prep.def

lib.hex: lib.elf
	ddump -R lib.elf -o lib.hex

lib_ld.hex: lib.hex
	@echo $(PSS_LDBOPTS) > ld_prep.def
	@ld_prep -v -d ld_prep.def -o lib_ld.hex lib.hex
	@rm -f ld_prep.def

#------------------------------------------------------------------------
# Compile or assemble modules which are always used.
#------------------------------------------------------------------------
begin.o: $(PSS_CONFIG)/begin.s \
         makefile $(PSS_CONFIG)/config.mk
	$(AS) $(AOPTS) -o $@      $<

rombegin.o: $(PSS_CONFIG)/rombegin.s \
         makefile $(PSS_CONFIG)/config.mk
	$(AS) $(AOPTS) -o $@      $<

beginapp.o: $(PSS_CONFIG)/beginapp.s \
            makefile $(PSS_CONFIG)/config.mk
	$(AS) $(AOPTS) -o $@   $<

ldbegin.o: $(PSS_CONFIG)/ldbegin.s \
            makefile $(PSS_CONFIG)/config.mk
	$(AS) $(AOPTS) -o $@   $<

lddriver.o: $(PSS_CONFIG)/lddriver.s \
            makefile $(PSS_CONFIG)/config.mk
	$(AS) $(AOPTS) -o $@   $<

bspcalls.o: $(PSS_BSP)/bspcalls.s \
            makefile $(PSS_CONFIG)/config.mk
	$(AS) $(AOPTS) -o $@   $<

gsblkcfg.o: $(PSS_CONFIG)/gsblkcfg.c \
            makefile $(PSS_CONFIG)/config.mk sys_conf.h
	$(CC) $(COPTS) -o $@   $<

sysinit.o: $(PSS_CONFIG)/sysinit.c \
           qpsos.tmp \
           makefile $(PSS_CONFIG)/config.mk sys_conf.h \
           $(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o sysinit.tmp  $<
	@$(LD) $(LOPTS)  -r -Xsuppress-dot -Xreloc-bug -o sysinit.o sysinit.tmp qpsos.tmp > qpsos.map
	@rm -f qpsos.o qmpsos.o qpsos.map sysinit.tmp

qpsos.tmp: $(PSS_ROOT)/sys/os/qpsos.s
	@echo "UNIPROC=1" > qpsos_tmp.s
	@cat $< >> qpsos_tmp.s
	@$(AS) $(AOPTS) -o qpsos.o qpsos_tmp.s
	@echo "UNIPROC=0" > qpsos_tmp.s
	@cat $< >> qpsos_tmp.s
	@das $(AOPTS) -I$(PSS_ROOT)/sys/os -o qmpsos.o qpsos_tmp.s
	@rm -f qpsos.tmp qpsos_tmp.s
	@$(LIB) cr qpsos.tmp qpsos.o qmpsos.o

philecfg.o: $(PSS_CONFIG)/philecfg.c \
            makefile $(PSS_CONFIG)/config.mk sys_conf.h \
           	$(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@   $<

pnacfg.o: $(PSS_CONFIG)/pnacfg.c \
          makefile $(PSS_CONFIG)/config.mk sys_conf.h \
          $(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@     $<

pnetcfg.o: $(PSS_CONFIG)/pnetcfg.c \
           makefile $(PSS_CONFIG)/config.mk sys_conf.h \
           $(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@    $<

prepccfg.o: $(PSS_CONFIG)/prepccfg.c \
            makefile $(PSS_CONFIG)/config.mk sys_conf.h \
           	$(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@   $<

probecfg.o: $(PSS_CONFIG)/probecfg.c \
            makefile $(PSS_CONFIG)/config.mk sys_conf.h \
           	$(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@   $<

prpccfg.o: $(PSS_CONFIG)/prpccfg.c \
           makefile $(PSS_CONFIG)/config.mk sys_conf.h \
           $(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@    $<

psecfg.o: $(PSS_CONFIG)/psecfg.c \
          makefile $(PSS_CONFIG)/config.mk sys_conf.h \
          $(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@     $<

psoscfg.o: $(PSS_CONFIG)/psoscfg.c \
           makefile $(PSS_CONFIG)/config.mk sys_conf.h \
           $(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@    $<

pmontcfg.o: $(PSS_CONFIG)/pmontcfg.c \
            makefile $(PSS_CONFIG)/config.mk sys_conf.h \
           	$(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@   $<

dialog.o: $(PSS_CONFIG)/dialog.c \
          makefile $(PSS_CONFIG)/config.mk sys_conf.h \
          $(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@     $<

pollio.o: $(PSS_CONFIG)/pollio.c \
          makefile $(PSS_CONFIG)/config.mk \
          $(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@     $<

bspcfg.o: $(PSS_BSP)/bspcfg.c \
          makefile $(PSS_CONFIG)/config.mk sys_conf.h \
          $(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@     $<

plmcfg.o: $(PSS_CONFIG)/plmcfg.c \
          makefile $(PSS_CONFIG)/config.mk sys_conf.h \
          $(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@     $<

ldcfg.o: $(PSS_CONFIG)/ldcfg.c \
         makefile $(PSS_CONFIG)/config.mk sys_conf.h
	$(CC) $(COPTS) -o $@      $<

compiler.o: $(PSS_CONFIG)/compiler.c \
         makefile $(PSS_CONFIG)/config.mk
	$(CC) $(COPTS) -o $@      $<

posixcfg.o: $(PSS_CONFIG)/posixcfg.c \
            makefile $(PSS_CONFIG)/config.mk sys_conf.h \
            $(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@   $<

rtacfg.o: $(PSS_CONFIG)/rtacfg.c \
           makefile $(PSS_CONFIG)/config.mk sys_conf.h \
           $(PSS_BSP)/bsp.h
	$(CC) $(COPTS) -o $@   $<

c.opt: makefile $(PSS_CONFIG)/config.mk
	@echo $(COMP_OPTS1)   >  c.opt
	@echo $(COMP_OPTS2)   >> c.opt
	@echo $(COPTS_FILE1)  >> c.opt
	@echo $(COPTS_FILE2)  >> c.opt

cxx.opt: makefile $(PSS_CONFIG)/config.mk
	@echo $(COMP_OPTS1)   >  cxx.opt
	@echo $(COMP_OPTS2)   >> cxx.opt
	@echo $(COPTS_FILE1)  >> cxx.opt
	@echo $(COPTS_FILE2)  >> cxx.opt
	@echo $(CXX_OPTS)     >> cxx.opt

a.opt: makefile $(PSS_CONFIG)/config.mk
	@echo $(AOPTS_FILE1)  > a.opt
	@echo $(AOPTS_FILE2)  >> a.opt
	@echo $(AOPTS_FILE3)  >> a.opt

ill.opt: makefile $(PSS_CONFIG)/config.mk
	@echo $(LOPTS)  > ill.opt

#------------------------------------------------------------------------
# Following target is used in psosmake to ensure that .opt files exist
# when target for a single file is made. 
#------------------------------------------------------------------------
create_opt_files: c.opt cxx.opt a.opt $(PSS_APP_OPTFILES)
