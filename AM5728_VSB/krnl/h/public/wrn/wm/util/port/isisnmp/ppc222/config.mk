#* @(#) pSOSystem PowerPC/V2.2.2: configs/ppc/config.mk 1.59 97/09/12 10:00:37 *
#***********************************************************************
#*                                                                     *
#*   MODULE:  config.mk                                                *
#*   DATE:    97/09/12                                                 *
#*   PURPOSE: Macro definitions, linker commands, etc, for "make"      *
#*                                                                     *
#*---------------------------------------------------------------------*
#*                                                                     *
#*        Copyright 2000-2001 Wind River Systems, Inc.
#*        Copyright 1991 - 1997, Integrated Systems, Inc.              *
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
# Add following option to COPTS_FILE2 to generate listing files
# -W:as:,-l -Xpass-source
#------------------------------------------------------------------------
CC           = dcc
COMP_OPTS1   = -O -t$(DTARGET)E$(DFP):psos -Xsmall-data=8
COMP_OPTS2   = -Xstruct-best-align=0 -Xansi -D__DIAB -DPPC
COPTS_FILE1  = -I. -I$(PSS_BSP) -I$(PSS_ROOT)/include
COPTS_FILE2  = $(PSS_APPINCS) -c
COPTS        = -@c.opt

#------------------------------------------------------------------------
# Assembler command-line options
#------------------------------------------------------------------------
AS      = das

#------------------------------------------------------------------------
# Added target switch for the assembler
# The target definition is located in bsp.mk of the bsp you are using
#------------------------------------------------------------------------
# Add following option to AOPTS_FILE1 to generate listing files  -l
#------------------------------------------------------------------------
AOPTS_FILE1  = -I. -I$(PSS_BSP) -I$(PSS_ROOT)/include
AOPTS_FILE2  = $(PSS_APPINCS) -t$(DTARGET)E$(DFP)
AOPTS        = -@a.opt

#------------------------------------------------------------------------
# Linker command name
#------------------------------------------------------------------------
LD      = dcc
LOPTS   = -Ws -t$(DTARGET)E$(DFP) -Wl,-Xsupress-dot -Xreloc-bug -m2

#------------------------------------------------------------------------
# Librarian command name and options
#------------------------------------------------------------------------
LIB          = dar
LIBOPTS      = -qf
LIBOPTS_SORT = -sR

#========================================================================
# Link everything together 
#========================================================================
CFG1_OBJS= sysinit.o  philecfg.o pnacfg.o   prepccfg.o dialog.o
CFG2_OBJS= probecfg.o prpccfg.o  psecfg.o   psoscfg.o  pmontcfg.o
CFG3_OBJS= pollio.o   bspcfg.o   gsblkcfg.o rdi_ser.o

ram.elf: c.opt a.opt begin.o \
	makefile \
	$(CFG1_OBJS) \
	$(CFG2_OBJS) \
	$(CFG3_OBJS) \
        prepcv.o \
	$(PSS_DRVOBJS) \
	$(DRV_LIB1) $(DRV_LIB2) $(DRV_LIB3) $(DRV_LIB4) $(DRV_LIB5) \
	$(PSS_APPOBJS) \
	$(APP_LIB1) $(APP_LIB2) $(APP_LIB3) $(APP_LIB4) $(APP_LIB5) \
	$(PSS_BSP)/libbsp.a \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_BSP)/ram_di.lnk
	@cat $(PSS_BSP)/ram_di.lnk >cmd.lnk
	@echo $(LOPTS) -o ram.elf -Wm cmd.lnk begin.o    > l.opt
	@echo $(CFG1_OBJS)                              >> l.opt
	@echo $(CFG2_OBJS)                              >> l.opt
	@echo $(CFG3_OBJS)                              >> l.opt
	@echo prepcv.o                                  >> l.opt
	@echo $(PSS_APPOBJS)                            >> l.opt
	@echo -m3 $(APP_LIB1)                           >> l.opt
	@echo -m3 $(APP_LIB2)                           >> l.opt
	@echo -m3 $(APP_LIB3)                           >> l.opt
	@echo -m3 $(APP_LIB4)                           >> l.opt
	@echo -m3 $(APP_LIB5)                           >> l.opt
	@echo $(PSS_DRVOBJS)                            >> l.opt
	@echo -m3 $(DRV_LIB1)                           >> l.opt
	@echo -m3 $(DRV_LIB2)                           >> l.opt
	@echo -m3 $(DRV_LIB3)                           >> l.opt
	@echo -m3 $(DRV_LIB4)                           >> l.opt
	@echo -m3 $(DRV_LIB5)                           >> l.opt
	@echo $(PSS_BSP)/libbsp.a                       >> l.opt
	@echo $(PSS_ROOT)/sys/os/libsys.a               >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc -lprepc           >> l.opt
	@echo -m3 $(FIX_LIB)                            >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > ram.map
	ddump -tv ram.elf >> ram.map

ram.hex: ram.elf
	ddump -Rv ram.elf -o ram.hex

rom.elf: c.opt a.opt begin.o \
	makefile \
	$(CFG1_OBJS) \
	$(CFG2_OBJS) \
	$(CFG3_OBJS) \
        prepcv.o \
	$(PSS_DRVOBJS) \
        $(DRV_LIB1) $(DRV_LIB2) $(DRV_LIB3) $(DRV_LIB4) $(DRV_LIB5) \
	$(APP_LIB1) $(APP_LIB2) $(APP_LIB3) $(APP_LIB4) $(APP_LIB5) \
	$(PSS_BSP)/libbsp.a \
	$(PSS_APPOBJS) \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_BSP)/rom_di.lnk
	@cat $(PSS_BSP)/rom_di.lnk >cmd.lnk
	@echo $(LOPTS) -o rom.elf -Wm cmd.lnk begin.o    > l.opt
	@echo $(CFG1_OBJS)                              >> l.opt
	@echo $(CFG2_OBJS)                              >> l.opt
	@echo $(CFG3_OBJS)                              >> l.opt
	@echo prepcv.o                                  >> l.opt
	@echo $(PSS_APPOBJS)                            >> l.opt
	@echo -m3 $(APP_LIB1)                           >> l.opt
	@echo -m3 $(APP_LIB2)                           >> l.opt
	@echo -m3 $(APP_LIB3)                           >> l.opt
	@echo -m3 $(APP_LIB4)                           >> l.opt
	@echo -m3 $(APP_LIB5)                           >> l.opt
	@echo $(PSS_DRVOBJS)                            >> l.opt
	@echo -m3 $(DRV_LIB1)                           >> l.opt
	@echo -m3 $(DRV_LIB2)                           >> l.opt
	@echo -m3 $(DRV_LIB3)                           >> l.opt
	@echo -m3 $(DRV_LIB4)                           >> l.opt
	@echo -m3 $(DRV_LIB5)                           >> l.opt
	@echo $(PSS_BSP)/libbsp.a                       >> l.opt
	@echo $(PSS_ROOT)/sys/os/libsys.a               >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc -lprepc           >> l.opt
	@echo -m3 $(FIX_LIB)                            >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > rom.map
	ddump -tv rom.elf >> rom.map

rom.hex: rom.elf
	ddump -Rv rom.elf -o rom.hex

os.elf: c.opt a.opt begin.o \
	makefile \
	$(CFG1_OBJS) \
	$(CFG2_OBJS) \
	$(CFG3_OBJS) \
        prepcv.o \
	$(PSS_DRVOBJS) \
        $(DRV_LIB1) $(DRV_LIB2) $(DRV_LIB3) $(DRV_LIB4) $(DRV_LIB5) \
	$(PSS_BSP)/libbsp.a \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_BSP)/os.lnk
	@cat $(PSS_BSP)/os.lnk >cmd.lnk
	@echo $(LOPTS) -o os.elf -Wm cmd.lnk begin.o     > l.opt
	@echo $(CFG1_OBJS)                              >> l.opt
	@echo $(CFG2_OBJS)                              >> l.opt
	@echo $(CFG3_OBJS)                              >> l.opt
	@echo prepcv.o                                  >> l.opt
	@echo $(PSS_DRVOBJS)                            >> l.opt
	@echo -m3 $(DRV_LIB1)                           >> l.opt
	@echo -m3 $(DRV_LIB2)                           >> l.opt
	@echo -m3 $(DRV_LIB3)                           >> l.opt
	@echo -m3 $(DRV_LIB4)                           >> l.opt
	@echo -m3 $(DRV_LIB5)                           >> l.opt
	@echo $(PSS_BSP)/libbsp.a                       >> l.opt
	@echo $(PSS_ROOT)/sys/os/libsys.a               >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc -lprepc           >> l.opt
	@echo -m3 $(FIX_LIB)                            >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > os.map
	ddump -tv os.elf >> os.map

os.hex: os.elf
	ddump -Rv os.elf -o os.hex

app.elf: c.opt a.opt begina.o \
	makefile \
	$(PSS_APPOBJS) \
	$(APP_LIB1) $(APP_LIB2) $(APP_LIB3) $(APP_LIB4) $(APP_LIB5) \
        prepcv.o \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_BSP)/app.lnk
	@cat $(PSS_BSP)/app.lnk >cmd.lnk
	@echo $(LOPTS) -o app.elf -Wm cmd.lnk begina.o   > l.opt
	@echo $(PSS_APPOBJS)                            >> l.opt
	@echo -m3 $(APP_LIB1)                           >> l.opt
	@echo -m3 $(APP_LIB2)                           >> l.opt
	@echo -m3 $(APP_LIB3)                           >> l.opt
	@echo -m3 $(APP_LIB4)                           >> l.opt
	@echo -m3 $(APP_LIB5)                           >> l.opt
	@echo prepcv.o                                  >> l.opt
	@echo $(PSS_ROOT)/sys/os/libsys.a               >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc -lprepc           >> l.opt
	@echo -m3 $(FIX_LIB)                            >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > app.map
	ddump -tv app.elf >> app.map

app.hex: app.elf
	ddump -Rv app.elf -o app.hex


#------------------------------------------------------------------------
# Compile or assemble modules which are always used.
#------------------------------------------------------------------------
begin.o: $(PSS_CONFIG)/begin.s makefile \
	$(PSS_CONFIG)/config.mk
	$(AS) $(AOPTS) -o begin.o $(PSS_CONFIG)/begin.s

begina.o: $(PSS_CONFIG)/begina.s makefile \
	$(PSS_CONFIG)/config.mk
	$(AS) $(AOPTS) -o begina.o $(PSS_CONFIG)/begina.s

gsblkcfg.o: $(PSS_CONFIG)/gsblkcfg.c \
	makefile \
	$(PSS_ROOT)/include/gsblk.h \
	$(PSS_CONFIG)/config.mk     \
	sys_conf.h
	$(CC) $(COPTS) $(PSS_CONFIG)/gsblkcfg.c

sysinit.o: $(PSS_CONFIG)/sysinit.c \
	makefile                         \
	sys_conf.h                       \
	$(PSS_BSP)/bsp.h                 \
	$(PSS_ROOT)/include/bspfuncs.h   \
	$(PSS_ROOT)/include/apdialog.h   \
	$(PSS_ROOT)/include/pna.h        \
	$(PSS_ROOT)/include/configs.h    \
	$(PSS_ROOT)/include/sysvars.h	\
	$(PSS_CONFIG)/config.mk          
	$(CC) $(COPTS) $(PSS_CONFIG)/sysinit.c

philecfg.o: $(PSS_CONFIG)/philecfg.c \
	makefile \
	sys_conf.h \
	$(PSS_ROOT)/include/configs.h \
	$(PSS_BSP)/bsp.h \
	$(PSS_ROOT)/include/bspfuncs.h \
	$(PSS_CONFIG)/config.mk
	$(CC) $(COPTS) $(PSS_CONFIG)/philecfg.c

pnacfg.o: $(PSS_CONFIG)/pnacfg.c \
	makefile \
	sys_conf.h \
	$(PSS_BSP)/bsp.h \
	$(PSS_ROOT)/include/bspfuncs.h \
	$(PSS_ROOT)/include/pna.h \
	$(PSS_ROOT)/include/configs.h \
	$(PSS_ROOT)/include/sysvars.h \
	$(PSS_CONFIG)/config.mk
	$(CC) $(COPTS) $(PSS_CONFIG)/pnacfg.c

prepccfg.o: $(PSS_CONFIG)/prepccfg.c \
	makefile \
	sys_conf.h \
	$(PSS_BSP)/bsp.h \
	$(PSS_ROOT)/include/bspfuncs.h \
	$(PSS_ROOT)/include/configs.h  \
	$(PSS_CONFIG)/config.mk
	$(CC) $(COPTS) $(PSS_CONFIG)/prepccfg.c

probecfg.o: $(PSS_CONFIG)/probecfg.c \
	makefile \
	sys_conf.h \
	$(PSS_BSP)/bsp.h \
	$(PSS_ROOT)/include/bspfuncs.h \
	$(PSS_ROOT)/include/configs.h \
	$(PSS_ROOT)/include/sysvars.h \
	$(PSS_CONFIG)/config.mk
	$(CC) $(COPTS) $(PSS_CONFIG)/probecfg.c

prpccfg.o: $(PSS_CONFIG)/prpccfg.c \
	makefile \
	sys_conf.h \
	$(PSS_BSP)/bsp.h \
	$(PSS_ROOT)/include/bspfuncs.h \
	$(PSS_ROOT)/include/configs.h \
	$(PSS_CONFIG)/config.mk
	$(CC) $(COPTS) $(PSS_CONFIG)/prpccfg.c

psecfg.o: $(PSS_CONFIG)/psecfg.c \
	makefile \
	sys_conf.h \
	$(PSS_BSP)/bsp.h \
	$(PSS_ROOT)/include/bspfuncs.h \
	$(PSS_ROOT)/include/configs.h \
	$(PSS_CONFIG)/config.mk
	$(CC) $(COPTS) $(PSS_CONFIG)/psecfg.c

psoscfg.o: $(PSS_CONFIG)/psoscfg.c \
	makefile \
	sys_conf.h \
	$(PSS_BSP)/bsp.h \
	$(PSS_ROOT)/include/bspfuncs.h \
	$(PSS_ROOT)/include/configs.h \
	$(PSS_ROOT)/include/sysvars.h \
	$(PSS_CONFIG)/config.mk
	$(CC) $(COPTS) $(PSS_CONFIG)/psoscfg.c

pmontcfg.o: $(PSS_CONFIG)/pmontcfg.c \
	makefile \
	sys_conf.h \
	$(PSS_BSP)/bsp.h \
	$(PSS_ROOT)/include/bspfuncs.h \
	$(PSS_ROOT)/include/configs.h \
	$(PSS_CONFIG)/config.mk
	$(CC) $(COPTS) $(PSS_CONFIG)/pmontcfg.c

dialog.o: $(PSS_CONFIG)/dialog.c \
	makefile \
	sys_conf.h \
	$(PSS_BSP)/bsp.h \
	$(PSS_ROOT)/include/bspfuncs.h \
	$(PSS_ROOT)/include/apdialog.h \
	$(PSS_ROOT)/include/sysvars.h \
	$(PSS_ROOT)/include/version.h \
	$(PSS_ROOT)/include/stdarg.h \
	$(PSS_ROOT)/include/pna.h  \
	$(PSS_CONFIG)/config.mk
	$(CC) $(COPTS) $(PSS_CONFIG)/dialog.c

pollio.o: $(PSS_CONFIG)/pollio.c makefile \
	$(PSS_BSP)/bsp.h \
	$(PSS_ROOT)/include/stdarg.h \
	$(PSS_CONFIG)/config.mk
	$(CC) $(COPTS) $(PSS_CONFIG)/pollio.c

prepcv.o: $(PSS_ROOT)/sys/os/prepcv.c makefile \
	$(PSS_CONFIG)/config.mk
	$(CC) $(COPTS) $(PSS_ROOT)/sys/os/prepcv.c

bspcfg.o: $(PSS_BSP)/bspcfg.c makefile \
        $(PSS_CONFIG)/config.mk        \
        sys_conf.h \
        $(PSS_BSP)/bsp.h
	@echo -I$(PSS_ROOT)/bsps/devices > bsp.opt
	$(CC) $(COPTS) -@bsp.opt $(PSS_BSP)/bspcfg.c
	@rm bsp.opt

rdi_ser.o: $(PSS_CONFIG)/rdi_ser.c makefile \
	sys_conf.h \
	$(PSS_BSP)/bsp.h \
	$(PSS_ROOT)/include/bspfuncs.h \
	$(PSS_ROOT)/include/configs.h \
	$(PSS_ROOT)/include/sysvars.h \
	$(PSS_CONFIG)/config.mk
	$(CC) $(COPTS) $(PSS_CONFIG)/rdi_ser.c

c.opt: makefile
	@echo $(COMP_OPTS1)   >  c.opt
	@echo $(COMP_OPTS2)   >> c.opt
	@echo $(COPTS_FILE1)  >> c.opt
	@echo $(COPTS_FILE2)  >> c.opt

a.opt: makefile
	@echo $(AOPTS_FILE1)  > a.opt
	@echo $(AOPTS_FILE2)  >> a.opt
