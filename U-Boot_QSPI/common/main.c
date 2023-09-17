/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/* #define	DEBUG	*/

#include <common.h>
#include <autoboot.h>
#include <cli.h>
#include <console.h>
#include <version.h>
#include <malloc.h>
#include "fw_validation.h"
#include "fw_update.h"

int reset_fwInfo(FW_INFO *fwlw);
DECLARE_GLOBAL_DATA_PTR;

/*
 * Board-specific Platform code can reimplement show_boot_progress () if needed
 */
__weak void show_boot_progress(int val) {}

static void run_preboot_environment_command(void)
{
#ifdef CONFIG_PREBOOT
	char *p;

	p = getenv("preboot");
	if (p != NULL) {
# ifdef CONFIG_AUTOBOOT_KEYED
		int prev = disable_ctrlc(1);	/* disable Control C checking */
# endif

		run_command_list(p, -1, 0);

# ifdef CONFIG_AUTOBOOT_KEYED
		disable_ctrlc(prev);	/* restore Control C checking */
# endif
	}
#endif /* CONFIG_PREBOOT */
}

/* We come here after U-Boot is initialised and ready to process commands */
void main_loop(void)
{
	const char *s;

	bootstage_mark_name(BOOTSTAGE_ID_MAIN_LOOP, "main_loop");

#ifdef CONFIG_VERSION_VARIABLE
	setenv("ver", version_string);  /* set version variable */
#endif /* CONFIG_VERSION_VARIABLE */

	cli_init();

	run_preboot_environment_command();

#if defined(CONFIG_UPDATE_TFTP)
	update_tftp(0UL, NULL, NULL);
#endif /* CONFIG_UPDATE_TFTP */
        
        /* Verifying vxworks checksum*/  
        am5728_os_checksum_validate();     
  
	s = bootdelay_process();
	if (cli_process_fdt(&s))
		cli_secure_boot_cmd(s);

	autoboot_command(s);

	cli_loop();
	panic("No CLI available");
}

unsigned short crc16_ccittt(const void *buf, int len)
{
        int counter;
        unsigned short crc = 0;
        for( counter = 0; counter < len; counter++)
        {
          crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *(char *)buf)&0x00FF];
          buf = ((char *)buf) +1;
        }
        return crc;
}

static struct spi_flash *env_flash;

void am5728_os_checksum_validate()
{
  #define FALSE   0
  #define TRUE    1
  #define REFERNCE_INFO_FLASH_OFFSET   0x1C0000
 
  unsigned int bus = CONFIG_SF_DEFAULT_BUS;
  unsigned int cs = CONFIG_SF_DEFAULT_CS;
  unsigned int speed = CONFIG_SF_DEFAULT_SPEED;
  unsigned int mode = CONFIG_SF_DEFAULT_MODE;

  struct spi_flash   *new;
  binInfo_t *bInfo = NULL;
  FW_INFO  *fwlw;
  char * buffer=NULL;
  int ret, osCrcResult = FALSE;
  unsigned short   osCRC;
  unsigned int     osSize;
  unsigned int     osOffset;
  unsigned short   calculated_crc;
unsigned int testSize = 0;

  int loadAddr =0;
  if (env_flash)
  {
    spi_flash_free(env_flash);
  }

  new = spi_flash_probe(bus, cs, speed, mode);
  env_flash = new;

  if (!new)
  {
   printf("Failed to initialize SPI flash at %u:%u\n", bus, cs);
   return;
  }

  fwlw = (FW_INFO *)memalign(ARCH_DMA_MINALIGN, sizeof(FW_INFO));
  if(fwlw==NULL)
  {
    printf("Refernce struct allocation failed");
  }

  ret = spi_flash_read(env_flash, REFERNCE_INFO_FLASH_OFFSET,
                                   sizeof(FW_INFO), (void *)fwlw);
  if (ret) 
  {
     printf("spi_flash_read binInfo failed\n");
  }

#if 0
  printf("Before reset....\n");
  printf("fwlw->bootMode %x\n",fwlw->bootMode);
  printf("fwlw->fwUpgrade %x\n",fwlw->fwUpgrade);
  printf("fwlw->activePart %x\n",fwlw->activePart);
  printf("fwlw->passivePart %x\n",fwlw->passivePart);
#endif

   if((fwlw->bootMode == 0xffffffff)|| (fwlw->bootMode == 0xeeeeeeee))
     {
       reset_fwInfo(fwlw);
       #if 1
	printf("Reset Partitioning information @ 0x1C0000....\n");
       #endif
     }

#if 0
   printf("After Reset.....\n");
   printf("fwlw->bootMode %x\n",fwlw->bootMode);
   printf("fwlw->fwUpgrade %x\n",fwlw->fwUpgrade);
   printf("fwlw->activePart %x\n",fwlw->activePart);
   printf("fwlw->passivePart %x\n",fwlw->passivePart);
#endif

   if((fwlw->bootMode == BRANSON_ACTIVE_BOOTMODE && fwlw->fwUpgrade == BRANSON_NORMAL_BOOT)
       ||(fwlw->bootMode == BRANSON_PASSIVE_BOOTMODE && fwlw->fwUpgrade == BRANSON_FWUPGRADE))
     {
	#if 0
       printf("If Hitted.....\n");
	#endif
                        loadAddr = fwlw->activePart;
                        fwlw->bootMode = BRANSON_ACTIVE_BOOTMODE;
                        fwlw->fwUpgrade = BRANSON_NORMAL_BOOT;
     }
   else if (fwlw->bootMode == BRANSON_ACTIVE_BOOTMODE && fwlw->fwUpgrade == BRANSON_FWUPGRADE)
           {
	#if 0
           printf("Elseif hitted....\n");
	#endif
                        loadAddr = fwlw->passivePart;
	                fwlw->bootMode =BRANSON_PASSIVE_BOOTMODE;
	   }
   ret = spi_flash_erase(env_flash, REFERNCE_INFO_FLASH_OFFSET, 0x10000);
   if (ret)
{
         printf("Erasing Flash Info from QSPI flash failed.\n");
         return;
}
	#if 0
   printf("Erased flash Info from SPI flash....\n");
	#endif


   ret = spi_flash_write(env_flash, FIRMWARE_PARTITION_INFO,sizeof(FW_INFO), fwlw);
   if (ret)
{
        printf("Boot config write failed.\n");
return;
}

#if 0
   printf("After If and Else....\n");
   printf("fwlw->bootMode %x\n",fwlw->bootMode);
   printf("fwlw->fwUpgrade %x\n",fwlw->fwUpgrade);
   printf("fwlw->activePart %x\n",fwlw->activePart);
   printf("fwlw->passivePart %x\n",fwlw->passivePart);
#endif

  free(fwlw);
  fwlw = NULL;

#if 0
  buffer = (char *)memalign(ARCH_DMA_MINALIGN,osSize);
  if (!buffer)
  {
     printf("memory allocation for vxworks buffer failed\n");
  }

  ret = spi_flash_read(env_flash, osOffset,osSize , buffer);
  if (ret)
  {
     printf("reading vxworks details failed\n");
  }
#endif

	testSize = VXWORKS_KERNEL_MAX_SIZE;
/*printf("testSize: 0x%X\n", testSize);*/


  printf("vxworks bin loading into 0x84100000 from : %x QSPI flash \n",loadAddr);
  ret = spi_flash_read(env_flash, loadAddr, VXWORKS_KERNEL_MAX_SIZE,(void *)FW_LOAD_RAM_ADDR);
  if (ret) {
            printf("Boot config reading failed.\n");
return;
  }

#if 0
  calculated_crc = crc16_ccittt((void *)buffer, osSize);
  
  printf("VxWorks Image CRC %x\n",osCRC);
  printf("Calculated Image CRC 0x%x\n",calculated_crc);

  if(calculated_crc == osCRC)
  {
     printf("\nVxworks CRC matched\n");
  }
  else
  {
     printf("\n VxWorks CRC MisMatched\n");
     printf("\n Please re-flash binaries using JTAG \n");
   
     /* CRC failed.Don't go further. Stuck here*/
     osCrcResult = TRUE;
     while(osCrcResult);
  }
#endif
 //free(buffer);
 return;
}



int reset_fwInfo(FW_INFO *fwlw) {
	        if(fwlw->bootMode !=BRANSON_ACTIVE_BOOTMODE && fwlw->bootMode != BRANSON_PASSIVE_BOOTMODE) {
			                fwlw->bootMode = BRANSON_ACTIVE_BOOTMODE;
			                fwlw->fwUpgrade = BRANSON_NORMAL_BOOT;
			                fwlw->activePart = BRANSON_ACTIVE_BOOTPART;
			                fwlw->passivePart = BRANSON_PASSIVE_BOOTPART;
											        }
		        return 0;
}

