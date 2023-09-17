/* sampleRamDisk.c */

#include  "vxWorks.h"
#include "wmw_httpconf.h"
#ifndef WEBCLI_STANDALONE_WEBSERVER

#include  "xbdRamDisk.h"											
#include  "dosFsLib.h"

int ramDiskCreate
(
	char *pDevName,				
	unsigned int  memSize		
)
{
	int ramDiskSize = 128 * 1024 ; 
	char *ramDiskName = "/ram0" ;
	device_t device;

	ramDiskName = pDevName;
	ramDiskSize = memSize;

    device = xbdRamDiskDevCreate (128, ramDiskSize, 0, ramDiskName);

	if( device == NULLDEV ) 
        return ERROR;                              

	dosFsVolFormat( ramDiskName, DOS_OPT_DEFAULT, NULL );

	return OK;
}
#endif

