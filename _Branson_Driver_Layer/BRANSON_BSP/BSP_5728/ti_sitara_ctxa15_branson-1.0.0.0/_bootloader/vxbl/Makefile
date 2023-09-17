# Makefile - makefile for VXBL 
#
# Copyright (c) 2016-2017 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 15mar16,jnl  created (US75898)
#
# DESCRIPTION
# This file contains the makefile rules for building VXBL
#
#

VSB_DIR = $(OUTDIR)

all:MLO	

ifeq (gnu,$(TOOL))
EXTRA_DEFINE += -ffreestanding -Os -Wall -fdata-sections -ffunction-sections
EXTRA_LDFLAGS += --gc-sections
else
EXTRA_DEFINE += -Xsection-split
EXTRA_LDFLAGS += -Xremove-unused-sections
endif

EXTRA_INCLUDE += -I.\
				 -I../../../include \
				 -I$(ARMARCHDIR)/src/arm/include \
                 -I$(ARMARCHDIR)/h \
				 -I$(VXBLDIR)/h \
				 -I$(VXBLDIR)/src/common \
				 -I$(VXBLDIR)/src/drv \
				 -I$(VSB_DIR) \
				 -I$(LIBCBOOTDIR)/h \
				 -I$(KERNELDIR)/h

MYLDPATH = $(VSB_DIR)/krnl/$(CPU)/common

include $(VSB_DIR)/vxbl.config

OBJS = romInit.o exception.o bootLib.o

MYOBJS = $(addprefix $(VSB_DIR)/, $(OBJS))

LNK_OBJS := $(wildcard $(MYLDPATH)/objdrvcore/*.o) \
            $(wildcard $(MYLDPATH)/objfdt/*.o) \
            $(wildcard $(MYLDPATH)/objc_wr/*.o) \
            $(wildcard $(MYLDPATH)/objbootc/*.o) \
            $(wildcard $(MYLDPATH)/objutils/*.o) \
            $(wildcard $(MYLDPATH)/objarch/*.o) \
            $(wildcard $(MYLDPATH)/objzlib/*.o) \
            $(wildcard $(MYLDPATH)/objcoreutil/*.o)

vxboot: $(OBJS)
ifeq (gnu,$(TOOL))
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(EXTRA_INCLUDE) -x c \
	-E $(VXBLDIR)/h/tools/gnu/link.BOOT.gnu -o $(VSB_DIR)/link.BOOT
	$(SED) -i -re "/^#/d" $(VSB_DIR)/link.BOOT
	$(LD) $(LDFLAGS) -EL -e _romInit $(EXTRA_LDFLAGS) -Ttext=$(_WRS_CONFIG_LINK_ADDR) \
		 -o $(VSB_DIR)/$@ $(MYOBJS)  \
	       	-defsym _VX_DATA_ALIGN=1 \
         -T $(VSB_DIR)/link.BOOT\
	     -L$(MYLDPATH) -L$(VSB_DIR)/usr/lib/common --start-group $(LNK_OBJS) --end-group
else
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(EXTRA_INCLUDE) -E $(VXBLDIR)/h/tools/diab/link.BOOT.diab | cat > $(VSB_DIR)/link.BOOT
	$(SED) -i -re "/^#/d" $(VSB_DIR)/link.BOOT
	$(LD) $(LDFLAGS) -e _romInit $(EXTRA_LDFLAGS) -D_VX_START_TEXT=$(_WRS_CONFIG_LINK_ADDR) \
		 -o $(VSB_DIR)/$@ $(MYOBJS)  \
         $(VSB_DIR)/link.BOOT \
	     -D_VX_DATA_ALIGN=1 \
	     -L$(MYLDPATH) -L$(VSB_DIR)/usr/lib/common $(LNK_OBJS)
endif

vxboot.bin: vxboot 
	objcopy$(ARCH) -O binary $(VSB_DIR)/$< $(VSB_DIR)/$@

MLO: vxboot.bin
	tclsh mkimage $(VSB_DIR)/vxboot.bin $(VSB_DIR)/MLO

include $(WIND_KRNL_MK)/rules.library.mk

ifeq (gnu,$(TOOL))
CC_OPTIM += -std=c99
else
CC_OPTIM += -Xdialect-c99
endif

clean:
	$(RM) $(VSB_DIR)/*.o $(VSB_DIR)/*.d $(VSB_DIR)/vxboot*

%.o : %.c
ifeq (gnu,$(TOOL))
	$(CC) $(CFLAGS) -o $(VSB_DIR)/$@ $<
else
	$(CC) -c $(CFLAGS) -o $(VSB_DIR)/$@ $<
endif


%.o : %.s
ifeq (gnu,$(TOOL))
	$(CC) $(CFLAGS_AS) -o $(VSB_DIR)/$@ $<
else
	$(CC) -c $(CFLAGS_AS) -o $(VSB_DIR)/$@ $<
endif
