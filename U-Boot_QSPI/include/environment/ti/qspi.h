/*
 * Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com
 *
 * Environment variable definitions for MMC/SD on TI boards.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#define DEFAULT_QSPI_TI_ARGS \
	"bootscript=echo Running bootscript from mmc${mmcdev} ...; " \
		"source ${loadaddr}\0" \
	"bootenvfile=uEnv.txt\0" \
	"importbootenv_qspi=echo Importing environment from qspi flash ...; " \
                "env import -t 0x84100000 0x132\0" \
	"loadbootenv_qspi=sf read 0x84100000 0x1d0000 0x132;\0" \
	"loadimage=load ${devtype} ${bootpart} ${loadaddr} ${bootdir}/${bootfile}\0" \
	"loadfdt=load ${devtype} ${bootpart} ${fdtaddr} ${bootdir}/${fdtfile}\0" \
	"envboot=sf probe 0; " \
			"echo qspi flash found on device;" \
				"if run loadbootenv_qspi; then " \
                                        "echo Loaded env from ${bootenvfile};" \
                                        "run importbootenv_qspi;" \
                                "fi;" \
				"if test -n $uenvcmd; then " \
					"echo Running uenvcmd ...;" \
					"run uenvcmd;" \
				"fi;" \

