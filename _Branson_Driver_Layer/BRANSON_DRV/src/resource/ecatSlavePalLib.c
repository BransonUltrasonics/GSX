
#include <ecatSlavePalLib.h>
#include <stdlib.h>
#include <subsys/gpio/vxbGpioLib.h>
#include <vxbFdtTiPrussEnd.h>

#define NUM_BYTES_PER_KB 	1024
#define SHARED_RAM_SIZE 	12	

typedef struct 
    {
    VXB_DEV_ID       pInst;
    }vxbessHandle;

/* IMPORT */

vxbessHandle essHandle ;
vxbessHandle essHandle_dram_iep ;


/*******************************************************************************
*
* ecatSlave_Register - Register Driver Instance with library.
*               
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
void ecatSlave_Register
    (
    VXB_DEV_ID pDev
    )
    {
	essHandle_dram_iep.pInst = pDev;
    }

/*******************************************************************************
*
* ecatSlave_Register_core - Register vxbFdtTiCoreEnd Driver Instance with library..
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
void ecatSlave_Register_core
    (
    VXB_DEV_ID pDev
    )
    {
	essHandle.pInst = pDev;
    }

/*******************************************************************************
*
* bsp_read_Revision - Function to find Firmware Revision ID
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
uint32_t bsp_read_Revision(void)
	{
	
	UINT8 * buf;
	uint32_t  rev_id;
	rev_id = pruSharedMemTransfer(essHandle.pInst,2);
	return rev_id;
	}


/*******************************************************************************
*
* bsp_read_ControllerId - Function to find Type of Slave Controller
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
uint32_t bsp_read_ControllerId(void)
	{
	
	UINT8 * buf;
	uint32_t  slave_cont_id;
	slave_cont_id  = pruSharedMemTransfer(essHandle.pInst,1);
	return slave_cont_id;
	
	}

/*******************************************************************************
*
* bsp_read_buildVersion - Function to find Firmware Build Version.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
uint32_t bsp_read_buildVersion(void)
	{
	
	UINT8 * buf;
	uint32_t  build_version;
	build_version  = pruSharedMemTransfer(essHandle.pInst,3);
	return build_version;
	
	}

/*******************************************************************************
*
* bsp_read_al_control - Function to read AL Control Register.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
uint32_t bsp_read_al_control(void) 
	{
		
		UINT8 * buf;	
		uint32_t tmpVal = 0;
	  tmpVal = pruSharedMemTransfer(essHandle.pInst, 4);
	  return tmpVal;
	 
	}
	
/*******************************************************************************
*
* bsp_read_al_event - Function to read AL Event Register.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
uint32_t bsp_read_al_event(void) 
	{
	
	UINT8 * buf;	
	uint32_t tmpVal = 0;
	tmpVal = pruSharedMemTransfer(essHandle.pInst, 5);
	return tmpVal;
	
	}

/*******************************************************************************
*
* bsp_read_al_status -  Function to read AL Status Register.
*               
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
uint32_t bsp_read_al_status(void) 
	{
	
	UINT8 * buf;	
	uint32_t tmpVal = 0;
	tmpVal = pruSharedMemTransfer(essHandle.pInst, 6);
	return tmpVal;
	
	}

/*******************************************************************************
*
* bsp_read_al_status_code -  Function to read AL Status Code
*               
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
uint32_t bsp_read_al_status_code(void) 
	{
	
	UINT8 * buf;	
	uint32_t tmpVal = 0;
	tmpVal = pruSharedMemTransfer(essHandle.pInst, 7);
	return tmpVal;
	
	}

/*******************************************************************************
*
* smtrans - Function to read AL Event, AL Status, AL Status code registers
*               
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
char smtrans () 
{
	/* AL Event, AL Status, AL Status code registers */
	unsigned short AlEventReg = 0 ;
	unsigned short AlStatusReg = 0 ;
	unsigned char AlControlReg = 0 ;
	unsigned short AlStatusCodeReg = 0 ;
	unsigned short currSlaveState = 0 ;
	uint32_t tmpVal = 0;
	uint32_t * Revaddress;
	uint8_t     tSlvCtrlr, revID, fwBldVer1,val;
	uint16_t    fwBldVer ;
	
	val = ReadByte_shdram(0x110);
	printf("The shdram 0x110 register value= 0x%x\n",val);
	
	val = ReadByte_shdram(0x111);
	printf("The shdram 0x111 register value= 0x%x\n",val);
	
	val = ReadByte_shdram(0x510);
	printf("The shdram 0x510 register value= 0x%x\n",val);
									   
	val = ReadByte_shdram(0x511);
	printf("The shdram 0x511 register value= 0x%x\n",val);
	
	val = ReadByte_shdram(0x512);
	printf("The shdram 0x512 register value= 0x%x\n",val);
									   
	val = ReadByte_shdram(0x513);
	printf("The shdram 0x513 register value= 0x%x\n",val);
	
	val = ReadByte_shdram(0xE08);
	printf("The shdram 0xE08 register value= 0x%x\n",val);
									   
	val = ReadByte_shdram(0xE09);
	printf("The shdram 0xE09 register value= 0x%x\n",val);
									   
	return OK;		                                   
	}

/*******************************************************************************
*
* ReadByte_dram_iep - Read byte from the DRAM 0, 1 or IEP at the offset 
* 					provided.
*               
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
uint8_t  ReadByte_dram_iep(uint32_t offset, uint8_t op)
{
	return(ReadByte_Dram_Iep(essHandle_dram_iep.pInst,offset,op));
}

/*******************************************************************************
*
* ReadWord_dram_iep - Read two bytes(a word) from the DRAM 0, 1 or IEP at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
uint16_t  ReadWord_dram_iep(uint32_t offset, uint8_t op)
{
	return(ReadWord_Dram_Iep(essHandle_dram_iep.pInst,offset,op));
}

/*******************************************************************************
*
* ReadDword_dram_iep - Read four byte (dword)  from the DRAM 0, 1 or IEP at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
uint32_t  ReadDword_dram_iep(uint32_t offset, uint8_t op)
{
	return(ReadDword_Dram_Iep(essHandle_dram_iep.pInst,offset,op));
}

/*******************************************************************************
*
* WriteByte_dram_iep - Write byte to the DRAM 0, 1 or IEP at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
void  WriteByte_dram_iep(uint32_t offset, uint8_t data, uint8_t op)
{
	WriteByte_Dram_Iep(essHandle_dram_iep.pInst,offset, data,op);
}

/*******************************************************************************
*
* WriteWord_dram_iep - Write two byte(word) to the DRAM 0, 1 or IEP at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
void  WriteWord_dram_iep(uint32_t offset, uint16_t data, uint8_t op)
{
	WriteWord_Dram_Iep(essHandle_dram_iep.pInst,offset, data,op);
}

/*******************************************************************************
*
* WriteDword_dram_iep - Write four byte(dword) to the DRAM 0, 1 or IEP at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
void  WriteDword_dram_iep(uint32_t offset, uint32_t data, uint8_t op)
{
	WriteDword_Dram_Iep(essHandle_dram_iep.pInst,offset, data, op);
}

/*******************************************************************************
*
* ReadBlock_dram_iep -Read a block from the DRAM 0, 1 or IEP at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
void ReadBlock_dram_iep(uint32_t offset, uint8_t *pData, uint32_t len, uint8_t op)
{
	ReadBlock_Dram_Iep(essHandle_dram_iep.pInst,offset, pData, len, op);
}

/*******************************************************************************
*
* WriteBlock_dram_iep - Write a block to the DRAM 0, 1 or IEP at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
void WriteBlock_dram_iep(uint32_t offset, uint8_t *pData, uint32_t len, uint8_t op)
{
	WriteBlock_Dram_Iep(essHandle_dram_iep.pInst,offset,pData,len, op);
}


/*******************************************************************************
*
* ReadByte_shdram - Read byte from the SHDRAM at the offset 
* 					provided.
*               
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
uint8_t  ReadByte_shdram(uint32_t offset)
{
	return(ReadByte_SHdram(essHandle.pInst,offset));
}

/*******************************************************************************
*
* ReadWord_shdram - Read two bytes(a word) from the SHDRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
uint16_t  ReadWord_shdram(uint32_t offset)
{
	return(ReadWord_SHdram(essHandle.pInst,offset));
}

/*******************************************************************************
*
* ReadDword_shdram - Read four byte (dword)  from the SHDRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
uint32_t  ReadDword_shdram(uint32_t offset)
{
	return(ReadDword_SHdram(essHandle.pInst,offset));
}

/*******************************************************************************
*
* WriteByte_shdram - Write byte to the SHDRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
void  WriteByte_shdram(uint32_t offset, uint8_t data)
{
	WriteByte_SHdram(essHandle.pInst,offset,data);
}

/*******************************************************************************
*
* WriteWord_shdram - Write two byte(word) to the SHDRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
void  WriteWord_shdram(uint32_t offset, uint16_t data)
{
	WriteWord_SHdram(essHandle.pInst,offset,data);
}

/*******************************************************************************
*
* WriteDword_shdram - Write four byte(dword) to the SHDRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
void  WriteDword_shdram(uint32_t offset, uint32_t data)
{
	WriteDword_SHdram(essHandle.pInst,offset,data);
}

/*******************************************************************************
*
* ReadBlock_shdram - Read four byte (dword)  from the SHDRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
void ReadBlock_shdram(uint32_t offset, uint8_t *pData, uint32_t len)
{
	ReadBlock_SHdram(essHandle.pInst,offset, pData, len);
}

/*******************************************************************************
*
* WriteBlock_shdram - Write four byte(dword) to the SDRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
void WriteBlock_shdram(uint32_t offset, uint8_t *pData, uint32_t len)
{
	WriteBlock_SHdram(essHandle.pInst,offset,pData, len);
}

/*******************************************************************************
*
* ReadRange_shdram - Read range of bytes  from the SharedRAM at the offset 
* 					provided.
*
* RETURNS: OK or ERROR when failed.
*
* ERRNO: N/A
*/
void ReadRange_shdram(uint32_t offset, uint32_t len)
{
	ReadRange_SHdram(essHandle.pInst,offset, len);
}

/*******************************************************************************
*
* vxbGpioSetValueLED - Sets Tricolor Led
*
* RETURNS: N/A
*
* ERRNO: N/A
*/
void vxbGpioSetValueLED(uint32_t ledType, uint8_t value) {
	
	if(ledType & BOARD_TRICOLOR0_RED) 
				vxbGpioSetValue(GPIO_BANK2_PIN24, value);
	
	else if(ledType & BOARD_TRICOLOR0_GREEN)
		vxbGpioSetValue(GPIO_BANK2_PIN25, value);
					
	else if(ledType & BOARD_TRICOLOR0_YELLOW)
		vxbGpioSetValue(GPIO_BANK2_PIN22, value);
	
	else if(ledType & BOARD_TRICOLOR1_RED) 
		vxbGpioSetValue(GPIO_BANK2_PIN23, value);
	
	else if(ledType & BOARD_TRICOLOR1_GREEN) 
		vxbGpioSetValue(GPIO_BANK4_PIN13, value);
	
	else if(ledType & BOARD_TRICOLOR1_YELLOW) 
		vxbGpioSetValue(GPIO_BANK4_PIN16, value);
}

