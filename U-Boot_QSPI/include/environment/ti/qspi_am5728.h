/*
 * Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com
 *
 * Environment variable definitions for MMC/SD on TI boards.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#define DEFAULT_QSPI_AM5728_ARGS \
	"envboot=sf probe 0; " \
			"echo qspi flash found on device;" \
					"go 0x84100000;\0" \

