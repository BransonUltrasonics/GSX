#* @(#) pSOSystem PowerPC/V2.5.0-BETA2: configs/ppc/config.mk 4.2 98/07/01 08:29:48 *
#***********************************************************************
#*                                                                     *
#*   MODULE:  configs/std/config.mk                                    *
#*   DATE:    98/07/01                                                 *
#*   PURPOSE: Macro definitions, linker commands, etc, for "make"      *
#*                                                                     *
#*---------------------------------------------------------------------*
#*                                                                     *
#*        Copyright 1991 - 1998, Integrated Systems, Inc.              *
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
CXX          = dplus
COMP_OPTS1   = -O -t$(DTARGET)E$(DFP):psos -Xstruct-best-align=0
COMP_OPTS2   = -Xstrict-ansi $(PSS_APPCOPTS) -D__DIAB -DPPC
COPTS_FILE1  = -I. $(PSS_APPINCS) -I$(PSS_ROOT)/include -I$(PSS_BSP)
COPTS_FILE2  = -I$(PSS_ROOT)/bsps/devices  $(PSS_VINCLUDE) -I@ -c
CXX_OPTS     = -Xexception
COPTS        = -@c.opt
CXXOPTS      = -@cxx.opt

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
AOPTS_FILE1  = -I. $(PSS_APPINCS) -I$(PSS_ROOT)/include -I$(PSS_BSP)
AOPTS_FILE2  = -I$(PSS_ROOT)/bsps/devices $(PSS_VINCLUDE) -I@
AOPTS_FILE3  = -t$(DTARGET)E$(DFP)
AOPTS        = -@a.opt

#------------------------------------------------------------------------
# Linker command name
#------------------------------------------------------------------------
LD      = dld
LOPTS   = -t$(DTARGET)E$(DFP) -Xsupress-dot -Xreloc-bug -m2 $(PSS_APPLOPTS)

#------------------------------------------------------------------------
# Librarian command name and options
#------------------------------------------------------------------------
LIB          = dar
LIBOPTS      = -qf
LIBOPTS_SORT = -sR

#========================================================================
# Link everything together 
#========================================================================
CFG1_OBJS= bspcfg.o   philecfg.o pnacfg.o   prepccfg.o dialog.o rarp.o
CFG2_OBJS= probecfg.o prpccfg.o  psecfg.o   psoscfg.o  pmontcfg.o
CFG3_OBJS= pollio.o   sysinit.o  gsblkcfg.o plmcfg.o pnetcfg.o ldcfg.o

ram.elf: cxx.opt c.opt a.opt ill.opt begin.o \
	makefile \
	$(CFG1_OBJS) \
	$(CFG2_OBJS) \
	$(CFG3_OBJS) \
	$(PSS_DRVOBJS) \
	$(PSS_APPOBJS) \
	$(PSS_BSP)/libbsp.a \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_ROOT)/sys/libc/libprepc$(DFP).a \
	$(PSS_ROOT)/sys/libc/libpnet.a \
	$(PSS_BSP)/anchor.lnk \
	$(PSS_BSP)/ram.lnk
	@cat $(PSS_BSP)/ram.lnk  >cmd.lnk
	@cat $(PSS_BSP)/anchor.lnk >>cmd.lnk
	@echo $(LOPTS) -o ram.elf -Wm cmd.lnk begin.o    > l.opt
	@echo -L $(PSS_BSP) -L $(PSS_ROOT)/sys/os       >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc $(PSS_APPLINKS)   >> l.opt
	@echo $(PSS_VLINK)                              >> l.opt
	@echo $(CFG1_OBJS)                              >> l.opt
	@echo $(CFG2_OBJS)                              >> l.opt
	@echo $(CFG3_OBJS)                              >> l.opt
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
	@echo -lbsp                                     >> l.opt
	@echo -lsys                                     >> l.opt
	@echo -lprepc$(DFP) -lpnet -lquery              >> l.opt
	@echo -m3 $(FIX_LIB)                            >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > ram.map
	ddump -tv ram.elf >> ram.map

ram.hex: ram.elf
	ddump -Rv ram.elf -o ram.hex

rom.elf: cxx.opt c.opt a.opt ill.opt begin.o \
	makefile \
	$(CFG1_OBJS) \
	$(CFG2_OBJS) \
	$(CFG3_OBJS) \
	$(PSS_DRVOBJS) \
	$(PSS_BSP)/libbsp.a \
	$(PSS_APPOBJS) \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_ROOT)/sys/libc/libprepc$(DFP).a \
	$(PSS_ROOT)/sys/libc/libpnet.a \
	$(PSS_BSP)/anchor.lnk \
	$(PSS_BSP)/rom.lnk
	@cat $(PSS_BSP)/rom.lnk  >cmd.lnk
	@cat $(PSS_BSP)/anchor.lnk >>cmd.lnk
	@echo $(LOPTS) -o rom.elf -Wm cmd.lnk begin.o    > l.opt
	@echo -L $(PSS_BSP) -L $(PSS_ROOT)/sys/os       >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc $(PSS_APPLINKS)   >> l.opt
	@echo $(PSS_VLINK)                              >> l.opt
	@echo $(CFG1_OBJS)                              >> l.opt
	@echo $(CFG2_OBJS)                              >> l.opt
	@echo $(CFG3_OBJS)                              >> l.opt
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
	@echo -lbsp                                     >> l.opt
	@echo -lsys                                     >> l.opt
	@echo -lprepc$(DFP) -lpnet -lquery              >> l.opt
	@echo -m3 $(FIX_LIB)                            >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > rom.map
	ddump -tv rom.elf >> rom.map

rom.hex: rom.elf
	ddump -Rv rom.elf -o rom.hex

os.elf: cxx.opt c.opt a.opt ill.opt begin.o \
	makefile \
	$(CFG1_OBJS) \
	$(CFG2_OBJS) \
	$(CFG3_OBJS) \
	$(PSS_DRVOBJS) \
	$(PSS_BSP)/libbsp.a \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_ROOT)/sys/libc/libprepc$(DFP).a \
	$(PSS_ROOT)/sys/libc/libpnet.a \
	$(PSS_BSP)/anchor.lnk \
	$(PSS_BSP)/os.lnk
	@cat $(PSS_BSP)/os.lnk  >cmd.lnk
	@cat $(PSS_BSP)/anchor.lnk >>cmd.lnk
	@echo $(LOPTS) -o os.elf -Wm cmd.lnk begin.o     > l.opt
	@echo -L $(PSS_BSP) -L $(PSS_ROOT)/sys/os       >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc $(PSS_APPLINKS)   >> l.opt
	@echo $(PSS_VLINK)                              >> l.opt
	@echo $(CFG1_OBJS)                              >> l.opt
	@echo $(CFG2_OBJS)                              >> l.opt
	@echo $(CFG3_OBJS)                              >> l.opt
	@echo $(PSS_DRVOBJS)                            >> l.opt
	@echo -m3 $(DRV_LIB1)                           >> l.opt
	@echo -m3 $(DRV_LIB2)                           >> l.opt
	@echo -m3 $(DRV_LIB3)                           >> l.opt
	@echo -m3 $(DRV_LIB4)                           >> l.opt
	@echo -m3 $(DRV_LIB5)                           >> l.opt
	@echo -lbsp                                     >> l.opt
	@echo -lsys                                     >> l.opt
	@echo -lprepc$(DFP) -lpnet -lquery              >> l.opt
	@echo -m3 $(FIX_LIB)                            >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > os.map
	ddump -tv os.elf >> os.map

os.hex: os.elf
	ddump -Rv os.elf -o os.hex

app.elf: cxx.opt c.opt a.opt ill.opt beginapp.o bspcalls.o \
	makefile \
	$(PSS_APPOBJS) \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_ROOT)/sys/libc/libprepc$(DFP).a \
	$(PSS_BSP)/anchor.lnk \
	$(PSS_BSP)/app.lnk
	@cat $(PSS_BSP)/app.lnk  >cmd.lnk
	@cat $(PSS_BSP)/anchor.lnk >>cmd.lnk
	@echo $(LOPTS) -o app.elf -Wm cmd.lnk beginapp.o > l.opt
	@echo bspcalls.o                                >> l.opt
	@echo -L $(PSS_BSP) -L $(PSS_ROOT)/sys/os       >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc $(PSS_APPLINKS)   >> l.opt
	@echo $(PSS_VLINK)                              >> l.opt
	@echo $(PSS_APPOBJS)                            >> l.opt
	@echo -m3 $(APP_LIB1)                           >> l.opt
	@echo -m3 $(APP_LIB2)                           >> l.opt
	@echo -m3 $(APP_LIB3)                           >> l.opt
	@echo -m3 $(APP_LIB4)                           >> l.opt
	@echo -m3 $(APP_LIB5)                           >> l.opt
	@echo -lsys                                     >> l.opt
	@echo -lprepc$(DFP) -lquery                     >> l.opt
	@echo -m3 $(FIX_LIB)                            >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > app.map
	ddump -tv app.elf >> app.map

app_ld.elf: app.elf
	@echo $(PSS_LDBOPTS) -p 180 -n LDBL -mAT -u 1024 -s 4096 > ld_prep.def
	@ld_prep -v -d ld_prep.def -o app_ld.elf app.elf
	@rm -f ld_prep.def

app.hex: app.elf
	ddump -Rv app.elf -o app.hex

app_ld.hex: app.hex
	@echo $(PSS_LDBOPTS) -p 180 -n LDBL -mAT -u 1024 -s 4096 > ld_prep.def
	@ld_prep -v -d ld_prep.def -o app_ld.hex app.hex
	@rm -f ld_prep.def

driver.elf: cxx.opt c.opt a.opt ill.opt lddriver.o bspcalls.o \
	ldblcfg.o \
	makefile \
	$(PSS_APPOBJS) \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_ROOT)/sys/libc/libprepc$(DFP).a \
	$(PSS_BSP)/anchor.lnk \
	$(PSS_BSP)/app.lnk
	@cat $(PSS_BSP)/app.lnk  >cmd.lnk
	@cat $(PSS_BSP)/anchor.lnk >>cmd.lnk
	@echo $(LOPTS) -o driver.elf cmd.lnk lddriver.o  > l.opt
	@echo bspcalls.o                                >> l.opt
	@echo -L $(PSS_BSP) -L $(PSS_ROOT)/sys/os       >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc $(PSS_APPLINKS)   >> l.opt
	@echo $(PSS_VLINK)                              >> l.opt
	@echo $(PSS_APPOBJS)                            >> l.opt
	@echo -m3 $(APP_LIB1)                           >> l.opt
	@echo -m3 $(APP_LIB2)                           >> l.opt
	@echo -m3 $(APP_LIB3)                           >> l.opt
	@echo -m3 $(APP_LIB4)                           >> l.opt
	@echo -m3 $(APP_LIB5)                           >> l.opt
	@echo -lsys                                     >> l.opt
	@echo -lprepc$(DFP)                             >> l.opt
	@echo -m3 $(FIX_LIB)                            >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > driver.map
	@ddump -tv driver.elf >> driver.map

lddriver.elf: driver.elf
	@echo $(PSS_LDBOPTS) -p 180 -n LDBL -mAT -u 1024 -s 4096 > ld_prep.def
	@ld_prep -v -d ld_prep.def -o lddriver.elf driver.elf
	@rm -f ld_prep.def

lib.elf: cxx.opt c.opt a.opt ill.opt bspcalls.o \
	makefile \
	$(PSS_APPOBJS) \
	$(PSS_ROOT)/sys/os/libsys.a \
	$(PSS_ROOT)/sys/libc/libprepc$(DFP).a \
	$(PSS_BSP)/anchor.lnk \
	$(PSS_BSP)/app.lnk
	@cat $(PSS_BSP)/app.lnk  >cmd.lnk
	@cat $(PSS_BSP)/anchor.lnk >>cmd.lnk
	@echo $(LOPTS) -o lib.elf cmd.lnk                > l.opt
	@echo bspcalls.o                                >> l.opt
	@echo -L $(PSS_BSP) -L $(PSS_ROOT)/sys/os       >> l.opt
	@echo -L $(PSS_ROOT)/sys/libc $(PSS_APPLINKS)   >> l.opt
	@echo $(PSS_VLINK)                              >> l.opt
	@echo $(PSS_APPOBJS)                            >> l.opt
	@echo -m3 $(APP_LIB1)                           >> l.opt
	@echo -m3 $(APP_LIB2)                           >> l.opt
	@echo -m3 $(APP_LIB3)                           >> l.opt
	@echo -m3 $(APP_LIB4)                           >> l.opt
	@echo -m3 $(APP_LIB5)                           >> l.opt
	@echo -lsys                                     >> l.opt
	@echo -lprepc$(DFP) -lquery                     >> l.opt
	@echo -m3 $(FIX_LIB)                            >> l.opt
	@echo $(PSS_COMPLIB)                            >> l.opt
	$(LD) -@l.opt > lib.map
	@ddump -tv lib.elf >> lib.map

lib_ld.elf: lib.elf
	@echo $(PSS_LDBOPTS) > ld_prep.def
	@ld_prep -v -d ld_prep.def -o lib_ld.elf lib.elf
	@rm -f ld_prep.def

lib.hex: lib.elf
	ddump -Rv lib.elf -o lib.hex

lib_ld.hex: lib.hex
	@echo $(PSS_LDBOPTS) > ld_prep.def
	@ld_prep -v -d ld_prep.def -o lib_ld.hex lib.hex
	@rm -f ld_prep.def

#------------------------------------------------------------------------
# Compile or assemble modules which are always used.
#------------------------------------------------------------------------
begin.o: makefile $(PSS_CONFIG)/config.mk
begin.o: $(PSS_CONFIG)/begin.s
	$(AS) $(AOPTS) -o $@      $<

beginapp.o: makefile $(PSS_CONFIG)/config.mk
beginapp.o: $(PSS_CONFIG)/beginapp.s
	$(AS) $(AOPTS) -o $@   $<

lddriver.o: makefile $(PSS_CONFIG)/config.mk
lddriver.o: $(PSS_CONFIG)/lddriver.s
	$(AS) $(AOPTS) -o $@   $<

bspcalls.o: makefile $(PSS_CONFIG)/config.mk
bspcalls.o: $(PSS_BSP)/bspcalls.s
	$(AS) $(AOPTS) -o $@   $<

gsblkcfg.o: makefile $(PSS_CONFIG)/config.mk sys_conf.h
gsblkcfg.o: $(PSS_CONFIG)/gsblkcfg.c
	$(CC) $(COPTS) -o $@   $<

sysinit.o: qpsos.tmp \
	makefile $(PSS_CONFIG)/config.mk sys_conf.h \
	$(PSS_BSP)/bsp.h
sysinit.o: $(PSS_CONFIG)/sysinit.c
	$(CC) $(COPTS) -o sysinit.tmp  $<
	@$(LD) $(LOPTS)  -r -Xsupress-dot -Xreloc-bug -o sysinit.o sysinit.tmp qpsos.tmp > qpsos.map
	@rm -f sysinit.tmp qpsos.o qpsos.map

qpsos.tmp: $(PSS_ROOT)/sys/os/qpsos.s
	@$(AS) $(AOPTS) -o qpsos.o $<
	@$(LIB) r qpsos.tmp qpsos.o

philecfg.o: makefile $(PSS_CONFIG)/config.mk sys_conf.h \
	$(PSS_BSP)/bsp.h
philecfg.o: $(PSS_CONFIG)/philecfg.c
	$(CC) $(COPTS) -o $@   $<

pnacfg.o: makefile $(PSS_CONFIG)/config.mk sys_conf.h \
	$(PSS_BSP)/bsp.h
pnacfg.o: $(PSS_CONFIG)/pnacfg.c
	$(CC) $(COPTS) -o $@     $<

pnetcfg.o: makefile $(PSS_CONFIG)/config.mk sys_conf.h \
	$(PSS_BSP)/bsp.h
pnetcfg.o: $(PSS_CONFIG)/pnetcfg.c
	$(CC) $(COPTS) -o $@    $<

prepccfg.o: makefile $(PSS_CONFIG)/config.mk sys_conf.h \
	$(PSS_BSP)/bsp.h
prepccfg.o: $(PSS_CONFIG)/prepccfg.c
	$(CC) $(COPTS) -o $@   $<

probecfg.o: makefile $(PSS_CONFIG)/config.mk sys_conf.h \
	$(PSS_BSP)/bsp.h
probecfg.o: $(PSS_CONFIG)/probecfg.c
	$(CC) $(COPTS) -o $@   $<

prpccfg.o: makefile $(PSS_CONFIG)/config.mk sys_conf.h \
	$(PSS_BSP)/bsp.h
prpccfg.o: $(PSS_CONFIG)/prpccfg.c
	$(CC) $(COPTS) -o $@    $<

psecfg.o: makefile $(PSS_CONFIG)/config.mk sys_conf.h \
	$(PSS_BSP)/bsp.h
psecfg.o: $(PSS_CONFIG)/psecfg.c
	$(CC) $(COPTS) -o $@     $<

psoscfg.o: makefile $(PSS_CONFIG)/config.mk sys_conf.h \
	$(PSS_BSP)/bsp.h
psoscfg.o: $(PSS_CONFIG)/psoscfg.c
	$(CC) $(COPTS) -o $@    $<

pmontcfg.o: makefile $(PSS_CONFIG)/config.mk sys_conf.h \
	$(PSS_BSP)/bsp.h
pmontcfg.o: $(PSS_CONFIG)/pmontcfg.c
	$(CC) $(COPTS) -o $@   $<

dialog.o: makefile $(PSS_CONFIG)/config.mk sys_conf.h \
	$(PSS_BSP)/bsp.h
dialog.o: $(PSS_CONFIG)/dialog.c
	$(CC) $(COPTS) -o $@     $<

pollio.o: makefile $(PSS_CONFIG)/config.mk \
	$(PSS_BSP)/bsp.h
pollio.o: $(PSS_CONFIG)/pollio.c
	$(CC) $(COPTS) -o $@     $<

bspcfg.o: makefile $(PSS_CONFIG)/config.mk sys_conf.h \
	$(PSS_BSP)/bsp.h
bspcfg.o: $(PSS_BSP)/bspcfg.c
	$(CC) $(COPTS) -o $@     $<

plmcfg.o: makefile $(PSS_CONFIG)/config.mk sys_conf.h \
	$(PSS_BSP)/bsp.h
plmcfg.o: $(PSS_CONFIG)/plmcfg.c
	$(CC) $(COPTS) -o $@     $<

ldcfg.o: makefile $(PSS_CONFIG)/config.mk sys_conf.h
ldcfg.o: $(PSS_CONFIG)/ldcfg.c
	$(CC) $(COPTS) -o $@      $<

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
