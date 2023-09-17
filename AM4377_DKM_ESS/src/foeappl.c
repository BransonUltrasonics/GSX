/**
\addtogroup FoE FileTransfer over EtherCAT
@{
*/

/**
\file foeappl.c
\author EthercatSSC@beckhoff.com
\brief Implementation
This file contains an example how to use the FoE services

\version 5.11

<br>Changes to version V5.10:<br>
V5.11 FOE2: FoE_Read() check File name buffer before store the requested file name, add string terminator<br>
V5.11 TEST5: send a FoE busy on a FoE read request<br>
V5.11 TEST9: "add behaviour 0x2020.7 (SDO requests on 0x3006.0 are set to pending until an FoE read request on ""UnlockSdoResp"" is received or in case that no mbx queue is supported when a new mbx request was received)"<br>
<br>Changes to version V5.0:<br>
V5.10 FOE2: Update file handling to support File sizes greater than 0x8000 Bytes (change value of "FOE_MAXDATA")<br>
<br>Changes to version V4.20:<br>
V5.0 FOE3: Add additional file name copy operation to provide name byte swapping<br>
V5.0 FOE5: Add FoE test operation (FOE_SAVE_FILES need to be disabled). On download only the files size is save. On upload a file with incrementing content and the previous downloaded size will be created.<br>
V5.0 FOE6: Save file if MBX 16Bit access is enabled<br>
<br>Changes to version V4.11:<br>
V4.20 FOE 2: Add File store compiler switch<br>
V4.20 FOE 1: Removed FILESUPPORT<br>
V4.11 FOE 1: nFileWriteOffset is now a 32bit variable to extend the maximum file size<br>
<br>Changes to version V3.20:<br>
V4.10 FOE 1-14: Filetransfer over FoE: Files can be read and stored to harddisk<br>
                      without any filesizelimitation (FILESUPPORT must defined)
*/

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/

#include <ecat_def.h>

#if FOE_SUPPORTED

#include <ecatslv.h>
#include <sdLib.h>
#include <ecatfoe.h>

#define _FOEAPPL_ 1
    #include <foeappl.h>
#undef _FOEAPPL_
#define _FOEAPPL_ 0

#if BOOTSTRAPMODE_SUPPORTED
#include <bootmode.h>
#endif

#include "firmwareInfo.h"

#define FIRMWARE_UPGRADE_ACK_REQ 			1
#define FIRMWARE_UPGRADE_START_REQ  			2


MSG_Q_ID msgQId_f;

#if TEST_APPLICATION
/* ECATCHANGE_START(V5.11) TEST5*/
#include <testappl.h>
/* ECATCHANGE_END(V5.11) TEST5*/
#endif
/*-----------------------------------------------------------------------------------------
------
------    internal Types and Defines
------
-----------------------------------------------------------------------------------------*/

#define    MAX_FILE_NAME_SIZE    16

/*-----------------------------------------------------------------------------------------
------
------    Module internal function declarations
------
-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------    Module internal variable definitions
------
-----------------------------------------------------------------------------------------*/
ESS_UINT32             nFileSize;
ESS_UINT32             nFileWriteOffset;
#if FOE_SAVE_FILES
ESS_CHAR             aFileName[MAX_FILE_NAME_SIZE];
#if MBX_16BIT_ACCESS
ESS_UINT16 MBXMEM     aFileData[(MAX_FILE_SIZE >> 1)];
#else
ESS_UINT8 MBXMEM     aFileData[MAX_FILE_SIZE];
#endif
#else
ESS_UINT8            DataValue = 0x00; //Used to fill upload file with dummy data
#endif
#if BOOTSTRAPMODE_SUPPORTED
#if ACTUATION_CONTROLLER_BUILD
const ESS_UINT16 HUGE aFirmwareDownloadHeader[2] = {}; // "AC_"
#else
const ESS_UINT16 HUGE aFirmwareDownloadHeader[2] = {}; // "PC_"
#endif   /*ACTUATION_CONTROLLER_BUILD*/
#endif	/*BOOTSTRAPMODE_SUPPORTED */



const unsigned short crc16tab[256]= {
        0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
        0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
        0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
        0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
        0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
        0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
        0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
        0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
        0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
        0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
        0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
        0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
        0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
        0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
        0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
        0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
        0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
        0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
        0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
        0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
        0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
        0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
        0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
        0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
        0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
        0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
        0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
        0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
        0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
        0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
        0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
        0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

/*-----------------------------------------------------------------------------------------
------
------    Functions
------
-----------------------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pName         Pointer to the name of the file
 \param     nameSize      Length of the file name
 \param     pData         Pointer to the response data
 \param     password      Password for the file read

 \return size of the data to be sent, busy or an error code
            1..FOE_MAXBUSY-101 (size of data to be sent)
            FOE_MAXBUSY-100 (0%)    (busy)
            FOE_MAXBUSY (100%) (busy)
            ECAT_FOE_ERRCODE_NOTDEFINED (error)
            ECAT_FOE_ERRCODE_NOTFOUND (error)
            ECAT_FOE_ERRCODE_ACCESS    (error)
            ECAT_FOE_ERRCODE_DISKFULL (error)
            ECAT_FOE_ERRCODE_ILLEGAL (error)
            ECAT_FOE_ERRCODE_PACKENO (error)
            ECAT_FOE_ERRCODE_EXISTS    (error)
            ECAT_FOE_ERRCODE_NOUSER    (error)

 \brief    The function starts a file reading

*////////////////////////////////////////////////////////////////////////////////////////

ESS_UINT16 FOE_Read(ESS_UINT16 MBXMEM * pName, ESS_UINT16 nameSize, ESS_UINT16 MBXMEM * pData, ESS_UINT32 password)
{
    ESS_UINT16 size = 0;
    ESS_UINT16 i = 0;

#if FOE_SAVE_FILES || TEST_APPLICATION
    ESS_CHAR aReadFileName[MAX_FILE_NAME_SIZE];

    /* ECATCHANGE_START(V5.11) FOE2*/
    if ((nameSize + 1) > MAX_FILE_NAME_SIZE)
    {
        return ECAT_FOE_ERRCODE_DISKFULL;
    }

    /*Read requested file name to endianess conversion if required*/
    MBXSTRCPY(aReadFileName, pName, nameSize);
    aReadFileName[nameSize] = '\0';
    /* ECATCHANGE_END(V5.11) FOE2*/

#endif

#if TEST_APPLICATION
/*ECATCHANGE_START(V5.11) TEST9*/
    if (IS_TEST_ACTIVE(DelaySdoResponse0x3006))
    {
        bUnlockSdoRequest = TRUE;
        ESS_CHAR aUnlock[] = "UnlockSdoResp";
        for (i = 0; i < nameSize; i++)
        {
            if (aUnlock[i] != aReadFileName[i])
            {
                bUnlockSdoRequest = FALSE;
                break;
            }
            
        }

        if (bUnlockSdoRequest)
        {
            nFileSize = 10;
            MBXMEMSET(pData, 0x00, 10);
            return 10;
        }

    }
/*ECATCHANGE_END(V5.11) TEST9*/

    /* ECATCHANGE_START(V5.11) TEST5*/
    if(IS_TEST_ACTIVE(FoE0_ReadBusy))
    {
        INC_TEST_CNT(FoE0_ReadBusy);
        size = (FOE_MAXBUSY-50); /* Return 50% busy*/
    }
    else
/* ECATCHANGE_END(V5.11) TEST5*/
#endif
#if FOE_SAVE_FILES
    {
    /* for test only the written file name can be read */
    for (i = 0; i < nameSize; i++)
    {
        if (aReadFileName[i] != aFileName[i] )
            /* file name not found */
            return ECAT_FOE_ERRCODE_NOTFOUND;
    }

    if ( nFileSize >= (u16SendMbxSize-SIZEOF(TFOEHEADER)-MBX_HEADER_SIZE) )
        size = (u16SendMbxSize-SIZEOF(TFOEHEADER)-MBX_HEADER_SIZE);
    else
        size = (unsigned short) nFileSize;

    MBXMEMCPY(pData, aFileData, size);
    }
#else
    if(nFileSize > 0)
    {
        if ( nFileSize >= (u16SendMbxSize-SIZEOF(TFOEHEADER)-MBX_HEADER_SIZE) )
            size = (u16SendMbxSize-SIZEOF(TFOEHEADER)-MBX_HEADER_SIZE);
        else
            size = (unsigned short) nFileSize;
        
        MBXMEMSET(pData,DataValue,size);

        DataValue++;
    }
    else
    {
        return ECAT_FOE_ERRCODE_NOTFOUND;
    }
#endif
    return size;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pName         Pointer to the name of the file
 \param     nameSize      Length of the file name
 \param     password      Password for the file read

 \return okay, busy or an error code
            0 (okay)
            ECAT_FOE_ERRCODE_NOTDEFINED (error)
            ECAT_FOE_ERRCODE_NOTFOUND (error)
            ECAT_FOE_ERRCODE_ACCESS    (error)
            ECAT_FOE_ERRCODE_DISKFULL (error)
            ECAT_FOE_ERRCODE_ILLEGAL (error)
            ECAT_FOE_ERRCODE_PACKENO (error)
            ECAT_FOE_ERRCODE_EXISTS    (error)
            ECAT_FOE_ERRCODE_NOUSER    (error)

            (no busy response shall be returned by this function.
            If the slave stack requires some time to handle the incoming data the function FOE_Data() should return busy)

 \brief    The function starts a file writing

*////////////////////////////////////////////////////////////////////////////////////////

#define FOE_RECV_FILE_SIZE 

char *fileName;
ESS_UINT16 FOE_Write(ESS_UINT16 MBXMEM * pName, ESS_UINT16 nameSize, ESS_UINT32 password)
{
	/* Message Queue for Firmware upgrade Seq*/
	char msg[4];
	
	#ifdef ENABLE_ONLINE_FIRMWARE_UPGRADE
	msg[0] = FIRMWARE_UPGRADE_ACK_REQ;
	msg[1] = 0;
	msgQId_f = msgQOpen("/Fw_Msgq_Notify",0,0,0,0,0);
	
	printf("\nSending Notification to FirmwareUpgrade Task From FOE_Write \n");
	
	if(msgQSend(msgQId_f,msg,2,0,0) != OK)
	{
			printf("\nSending Notification to FirmwareUpgrade From FOE_Write Failed\n");
	}
	msgQClose(msgQId_f);
	#endif
	
#if BOOTSTRAPMODE_SUPPORTED
	fileName = (char *)malloc(nameSize);
    if ( ( nameSize >= SIZEOF(aFirmwareDownloadHeader) )
        //&&( pName[0] == aFirmwareDownloadHeader[0] )
        //&&( pName[1] == aFirmwareDownloadHeader[1] )
		)
    {
    	FirmwareUpgradeFileBufferLenght =0;
		
		FirmwareUpgradeFileBuffer = (char *)malloc(FOE_DOWNLOAD_MAX_SIZE);
		if(FirmwareUpgradeFileBuffer == NULL)
		{
			printf("not sufficient memory \n");
		}
		else{
			printf("Allocated memory, sufficient memory is available \n");
		}
        if ( bBootMode )
        {
        	printf ("\n Starting the FW Download\n");
			strncpy(fileName,(char *)pName,nameSize);

			printf("Downloading file  %s\n",fileName);
            BL_StartDownload(password);
            return 0;
        }
        else
            return ECAT_FOE_ERRCODE_BOOTSTRAPONLY;
    }
    else
    if ( bBootMode )
        return ECAT_FOE_ERRCODE_NOTINBOOTSTRAP;
    else
#endif
#if FOE_SAVE_FILES
    if ( nameSize < MAX_FILE_NAME_SIZE )
    {
        /* for test every file name can be written */
        MBXSTRCPY(aFileName, pName, nameSize);
        MBXSTRCPY(aFileName+nameSize, "\0", 1); //string termination


        nFileWriteOffset = 0;
        nFileSize = 0;
        return 0;
    }
    else
        return ECAT_FOE_ERRCODE_DISKFULL;
#else
        DataValue = 0x00;
        nFileSize = 0;
        return 0;
#endif
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pData         Received file data
 \param 	Size          Length of received file data

 \return okay, busy or an error code
            0 (okay)
            FOE_MAXBUSY-100 (0%)    (busy)
            FOE_MAXBUSY (100%) (busy)
            ECAT_FOE_ERRCODE_NOTDEFINED (error)
            ECAT_FOE_ERRCODE_NOTFOUND (error)
            ECAT_FOE_ERRCODE_ACCESS    (error)
            ECAT_FOE_ERRCODE_DISKFULL (error)
            ECAT_FOE_ERRCODE_ILLEGAL (error)
            ECAT_FOE_ERRCODE_PACKENO (error)
            ECAT_FOE_ERRCODE_EXISTS    (error)
            ECAT_FOE_ERRCODE_NOUSER    (error)

 \brief    The function is called to write the next part of a file

*////////////////////////////////////////////////////////////////////////////////////////

ESS_UINT16 FOE_Data(ESS_UINT16 MBXMEM * pData, ESS_UINT16 Size)
{
	
	int val;
	char msg[4];
	FirmWareUpgradeInfo *FirmWareBuff;
	FirmWareUpgradeInfo FirmWareTempBuff;
	
#if BOOTSTRAPMODE_SUPPORTED
    if ( bBootMode )
    {
    
		int ret = BL_Data(pData, Size);
		if(Size < FOE_PACKET_MAX_SIZE)
		{
			#ifdef ENABLE_ONLINE_FIRMWARE_UPGRADE
			/* Message Queue for Firmware Seq*/
			msgQId_f = msgQOpen("/Fw_Msgq_Notify",0,0,0,0,0);	
			SD_ID FWSD = sdOpen("/FirmwareUpgradeBuff", 0, 0, 0, 0, SD_ATTR_RWX | SD_CACHE_OFF, (void **) &FirmWareBuff);
			
			if(SD_ID_NULL == FWSD)
			{
				kprintf("/FirmwareUpgradeBuff!!!\n");
				exit(-1);		
			}

			FirmWareTempBuff.FW_CRC_Checksum = CRC_Check(FirmwareUpgradeFileBuffer,FirmwareUpgradeFileBufferLenght);
			FirmWareTempBuff.FW_Length = FirmwareUpgradeFileBufferLenght;
			FirmWareTempBuff.FW_FileStartAddress = FirmwareUpgradeFileBuffer;
			FirmWareBuff->FW_CRC_Checksum = FirmWareTempBuff.FW_CRC_Checksum;
			FirmWareBuff->FW_Length = FirmWareTempBuff.FW_Length;
			FirmWareBuff->FW_FileStartAddress = FirmWareTempBuff.FW_FileStartAddress;
			
			msg[0] = FIRMWARE_UPGRADE_START_REQ;
			if(msgQSend(msgQId_f,msg,1,0,0) != OK)
			{
				kprintf("FIRMWARE_UPGRADE_START_REQ Failed\n");
			}
			msgQClose(msgQId_f);
			#endif
			return ret;
		}
    }
    else
#endif
#if FOE_SAVE_FILES
        if((nFileWriteOffset + Size) > MAX_FILE_SIZE)
            return ECAT_FOE_ERRCODE_DISKFULL;

        if ( Size )
        {

#if MBX_16BIT_ACCESS
                MBXMEMCPY(&aFileData[(nFileWriteOffset >> 1)], pData, Size);
#else
                MBXMEMCPY(&aFileData[nFileWriteOffset], pData, Size);
#endif

        }
#endif
        if ( Size == (u16ReceiveMbxSize - MBX_HEADER_SIZE - FOE_HEADER_SIZE) )
        {
            /* FoE-Data services will follow */
            nFileWriteOffset += Size;
            return 0;
        }
        else
        {
            /* last part of the file is written */
            nFileSize = nFileWriteOffset + Size;
            nFileWriteOffset = 0;

            return FOE_ACKFINISHED;
        }


}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     fileOffset    Offset of the next file data to be sent
 \param     pData         Buffer for file data to be sent

 \return size of the data to be sent, busy or an error code
            0 (all data was sent before)
            1..MAX_FILE_SIZE (size of data to be sent)
            FOE_MAXBUSY-100 (0%)    (busy)
            FOE_MAXBUSY (100%) (busy)
            ECAT_FOE_ERRCODE_NOTDEFINED (error)
            ECAT_FOE_ERRCODE_NOTFOUND (error)
            ECAT_FOE_ERRCODE_ACCESS    (error)
            ECAT_FOE_ERRCODE_DISKFULL (error)
            ECAT_FOE_ERRCODE_ILLEGAL (error)
            ECAT_FOE_ERRCODE_PACKENO (error)
            ECAT_FOE_ERRCODE_EXISTS    (error)
            ECAT_FOE_ERRCODE_NOUSER    (error)

 \brief    The function is called when the reading of a part of a file is acknowledged

*////////////////////////////////////////////////////////////////////////////////////////

ESS_UINT16 FOE_Ack(ESS_UINT32 fileOffset, ESS_UINT16 MBXMEM * pData)
{
	
    if ( fileOffset < nFileSize )
    {
        /* send next part of the file */
        ESS_UINT32 size;
        ESS_UINT32 sendSize = nFileSize-fileOffset;

        if ( sendSize >= (u16SendMbxSize-SIZEOF(TFOEHEADER)-MBX_HEADER_SIZE) )
        {
            size = (u16SendMbxSize-SIZEOF(TFOEHEADER)-MBX_HEADER_SIZE);
        }
        else
        {
            size = sendSize;
        }
#if FOE_SAVE_FILES
#if MBX_16BIT_ACCESS
        MBXMEMCPY(pData, &aFileData[(fileOffset >> 1)], size);
#else
        MBXMEMCPY(pData, &aFileData[fileOffset], size);
#endif

#else
        MBXMEMSET(pData,DataValue,size);

        DataValue++;
#endif
        return ((ESS_UINT16) size);
    }
    else if ( fileOffset == nFileSize )
    {
        /* file transfer is finished */
        return 0; // size = 0
    }
    else
    {
        return ECAT_FOE_ERRCODE_ILLEGAL;
    }

}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     done          Indicates how much of the busy action is done
 \param     fileOffset    Offset of the next file data to be sent
 \param     pData         Buffer for file data to be sent

 \return size of the data to be sent, busy or an error code
            0 (all data was sent before)
            1..MAX_FILE_SIZE (size of data to be sent)
            FOE_MAXBUSY-100 (0%)    (busy)
            FOE_MAXBUSY (100%) (busy)
            ECAT_FOE_ERRCODE_NOTDEFINED (error)
            ECAT_FOE_ERRCODE_NOTFOUND (error)
            ECAT_FOE_ERRCODE_ACCESS    (error)
            ECAT_FOE_ERRCODE_DISKFULL (error)
            ECAT_FOE_ERRCODE_ILLEGAL (error)
            ECAT_FOE_ERRCODE_PACKENO (error)
            ECAT_FOE_ERRCODE_EXISTS    (error)
            ECAT_FOE_ERRCODE_NOUSER    (error)

 \brief    The function is called when the reading of a part of a file should be repeated

*////////////////////////////////////////////////////////////////////////////////////////

ESS_UINT16 FOE_Busy(ESS_UINT16 done, ESS_UINT32 fileOffset, ESS_UINT16 MBXMEM * pData)
{
    /* send last part again */
    return FOE_Ack(fileOffset, pData);
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     errorCode    error code

 \brief    The function is called when a file transfer is aborted from the other station

*////////////////////////////////////////////////////////////////////////////////////////

void FOE_Error(ESS_UINT32 errorCode)
{
    if ( nFileWriteOffset )
    {
#if FOE_SAVE_FILES
        /* no file is stored */
        memset(aFileName, 0, MAX_FILE_NAME_SIZE);
#endif
        nFileWriteOffset = 0;
        nFileSize = 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
Function to check CRC for Firmware upgrade file
*////////////////////////////////////////////////////////////////////////////////////////
ESS_UINT16 	CRC_Check(const void *buf,ESS_UINT32 len)
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

#endif // FOE_SUPPORTED
/** @} */

