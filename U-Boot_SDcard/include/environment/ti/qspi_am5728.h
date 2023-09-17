/*
 * Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com
 *
 * Environment variable definitions for MMC/SD on TI boards.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#define DEFAULT_QSPI_AM5728_ARGS \
	"envboot=sf probe 0;"\
	"echo ERASING THE FLASH. TAKES A WHILE..;"\
	"sf erase 0 0x4000000;"	\
	"echo ERASE COMPLETED. COPYING BOOTLOADER(MLO)..;"\
	"fatload mmc 0 0x84100000 QSPIFLASH/MLO;"\
	"sf write 0x84100000 0 0x20000;"\
	"echo COPYING BOOTLOADER(uboot.img)..;"\
	"fatload mmc 0 0x84100000 QSPIFLASH/u-boot.img;"\
	"sf write 0x84100000 0x40000 0x70000;"\
	"echo COPYING VXWORKS IMAGE..;"\
	"fatload mmc 0 0x84100000 QSPIFLASH/vxWorks.bin;"\
	"sf write 0x84100000 0x1E0000 0x14EF000;"\
	"echo FLASHING COMPLETE..;"\
	"echo REMOVE SD CARD..;"\

