/*
 * (C) Copyright 2000-2010
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2001 Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Andreas Heppel <aheppel@sysgo.de>
 *
 * (C) Copyright 2008 Atmel Corporation
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <environment.h>
#include <malloc.h>
#include <spi.h>
#include <spi_flash.h>
#include <search.h>
#include <errno.h>
#include <dm/device-internal.h>

#ifndef CONFIG_ENV_SPI_BUS
# define CONFIG_ENV_SPI_BUS	0
#endif
#ifndef CONFIG_ENV_SPI_CS
# define CONFIG_ENV_SPI_CS	0
#endif
#ifndef CONFIG_ENV_SPI_MAX_HZ
# define CONFIG_ENV_SPI_MAX_HZ	1000000
#endif
#ifndef CONFIG_ENV_SPI_MODE
# define CONFIG_ENV_SPI_MODE	CONFIG_SF_DEFAULT_MODE
#endif

#if 0
#define BRANSON_ACTIVE_BOOTMODE        	0x00000000
#define BRANSON_PASSIVE_BOOTMODE       	0x00000001

#define BRANSON_NORMAL_BOOT      	0x00000000
#define BRANSON_FWUPGRADE		0x00000001

#define BRANSON_ACTIVE_BOOTPART     	0x00A00000
#define BRANSON_PASSIVE_BOOTPART    	0x00A00000

#define FIRMWARE_PARTITION_INFO		0x00100000	/* assigned size  64KB , block no 1023 */
#define VXWORKS_KERNEL_MAX_SIZE 	0x00500000 	/* Allocated size for vxworks bin is 10 MB,
								 from block 160 to 319*/
#define FW_LOAD_RAM_ADDR 		0x80100000	/* firmware loading address in RAM */

#endif

#if 1 
#define BRANSON_ACTIVE_BOOTMODE        	0x00000000
#define BRANSON_PASSIVE_BOOTMODE       	0x00000001

#define BRANSON_NORMAL_BOOT      	0x00000000
#define BRANSON_FWUPGRADE		0x00000001

#define BRANSON_ACTIVE_BOOTPART     	0x1E0000
#define BRANSON_PASSIVE_BOOTPART    	0x2000000
#define PARTITION_BOUNDARY		0x10000		/* 40 KB boundary between Active & Passive partition */

#define FIRMWARE_PARTITION_INFO		0x1C0000	/* assigned size  64KB , block no 1023 */

							/* Max allocated size for vxworks bin is ~31 MB */
#define VXWORKS_KERNEL_MAX_SIZE 	(BRANSON_PASSIVE_BOOTPART - BRANSON_ACTIVE_BOOTPART) - PARTITION_BOUNDARY 
					
#define FW_LOAD_RAM_ADDR 		0x84100000	/* firmware loading address in RAM */

#define CONFIG_ENV_SECT_SIZE             (0x40000)

#endif

void load_firmware(void);


typedef struct fwInfo {
	int bootMode;
	int fwUpgrade;
	int activePart;
	int passivePart;

} FW_INFO;



