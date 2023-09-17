/*
 * ecatSlaveHwSource.c
 *
 *  Created on: Apr 21, 2017
 */
#include <stdio.h>

#ifndef _ECATSLAVEBSPSOURCE_H_
#include <ecatSlaveBspSource.h>
#endif

#ifndef __ECATSLAVE_HW__
#include <ecatSlaveHw.h>
#endif


#ifndef __ECATSLAVE_SLAVE_APP__
#include <ecatSlaveUserApp.h>
#endif

//---------------------------------------------------------------------------------------
/*-----------------------------------------------------------------------------------------
------	
------	Includes
------ 
-----------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <ecat_def.h>
extern Uint32 pd_read_addr_err, pd_write_addr_err;
/*--------------------------------------------------------------------------------------
------	
------	local Types and Defines
------	
--------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------
------  
------	local variables and constants
------	
-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------  
------	local functions
------	
-----------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------
------  
------	functions
------	
-----------------------------------------------------------------------------------------*/

/**
\addtogroup ecatSlavehw
@{
*/

/////////////////////////////////////////////////////////////////////////////////////////
/**
\return	 0 if initialization was successful 

           \brief	This function intializes the EtherCAT Slave Interface.
*////////////////////////////////////////////////////////////////////////////////////////

Uint8 HW_Init(void)
{

    Uint16 u16PdiCtrl;
    /* the memory interface to the ESC, the ESC-interrupt and the ECAT-timer for the
       watchdog monitoring should be initialized here microcontroller specific */

    bsp_init_dramIep();
    bsp_init_sharedRam();

    /* we have to wait here, until the ESC is started */
    do {
        HW_EscReadWord(u16PdiCtrl, ESC_ADDR_PDI_CONTROL);
        u16PdiCtrl = SWAPWORD(u16PdiCtrl) & 0xFF;

    } while (u16PdiCtrl != 0x80);        //Looking for onchip bus

    return 0;
}

void HW_Release(void)
{

    bsp_exit();
}

void HW_EscRead(Uint8 * pData, Uint16 Address, Uint16 Len)
{
    bsp_read ( pData, Address, Len);
    bsp_pdi_post_read_indication( Address);
}

void HW_EscReadIsr(Uint8 * pData, Uint16 Address, Uint16 Len)
{
    Int16 sm_index;
    Uint16 ActAddress = bsp_get_process_data_address( Address, Len, &sm_index);
    if (ActAddress < ESC_ADDR_MEMORY)
    {
        pd_read_addr_err++;
        return;
    }	   
    bsp_read ( pData, ActAddress, Len);
    
    bsp_process_data_access_complete( Address, Len, sm_index);
}

Uint8 __HW_EscReadByteIsr(Uint16 Address)
{
    Uint8 ByteValue;
	Int16 sm_index;
    Uint16 ActAddress = bsp_get_process_data_address( Address, 1, &sm_index);
    if (ActAddress < ESC_ADDR_MEMORY)
    {
        pd_read_addr_err++;
        return 0;
    }	   
    ByteValue = bsp_read_byte_isr ( ActAddress);
    
    bsp_process_data_access_complete( Address, 1, sm_index);
	return ByteValue;
}
Uint16 __HW_EscReadWordIsr( Uint16 Address)
{
    Int16 sm_index;
    Uint16 WordValue, ActAddress = bsp_get_process_data_address( Address, 2, &sm_index);
    if (ActAddress < ESC_ADDR_MEMORY)
    {
        pd_read_addr_err++;
        return 0;
    }	   
    WordValue = bsp_read_word_isr ( ActAddress);
    
    bsp_process_data_access_complete( Address, 2, sm_index);
	return WordValue;
}

Uint32 __HW_EscReadDWordIsr(Uint16 Address)
{
    Int16 sm_index;
    Uint16 ActAddress = bsp_get_process_data_address( Address, 4, &sm_index);
    Uint32 DWordValue; 
    if (ActAddress < ESC_ADDR_MEMORY)
    {
        pd_read_addr_err++;
        return 0;
    }
    DWordValue = bsp_read_dword_isr( ActAddress);
    
    bsp_process_data_access_complete( Address, 4, sm_index);
    return DWordValue;
}
void HW_EscReadMbxMem(Uint8 * pData, Uint16 Address, Uint16 Len)
{
    t_sm_properties *p_sm_properties = bsp_get_sm_properties(MAILBOX_WRITE);
    bsp_pdi_mbx_read_start();
    bsp_read ( pData, Address, Len);
    if (Len >= p_sm_properties->length-2) {
        bsp_pdi_mbx_read_complete();
    }
}

void HW_EscWrite(Uint8 * pData, Uint16 Address, Uint16 Len)
{
    bsp_write ( pData, Address, Len);
    bsp_pdi_write_indication( Address,0);
}

void HW_EscWriteIsr(Uint8 * pData, Uint16 Address, Uint16 Len)
{
    Int16 sm_index;
    Uint16 ActualAddr = bsp_get_process_data_address( Address, Len, &sm_index);
    if (ActualAddr < ESC_ADDR_MEMORY)
    {
        pd_write_addr_err++;
        return;
    }	
    bsp_write ( pData, ActualAddr, Len);
    
    bsp_process_data_access_complete( Address, Len, sm_index);
}

void HW_EscWriteDWord(Uint32 DWordValue, Uint16 Address)
{
    t_sm_properties *p_sm_properties = bsp_get_sm_properties(MAILBOX_READ);
    bsp_write_dword( DWordValue, Address);
    if (Address == (p_sm_properties->physical_start_addr+p_sm_properties->length-4)) {
        bsp_pdi_mbx_write_complete();
    } else
        bsp_pdi_write_indication( Address,(Uint16)DWordValue);
}

void HW_EscWriteDWordIsr(Uint32 DWordValue, Uint16 Address)
{
    Int16 sm_index;
    Uint16 ActualAddr = bsp_get_process_data_address( Address, 4, &sm_index);
    if (ActualAddr < ESC_ADDR_MEMORY)
    {
        pd_write_addr_err++;
        return;
    }	
    bsp_write_dword( DWordValue, ActualAddr);
    bsp_process_data_access_complete( Address, 4, sm_index);

}
void HW_EscWriteWord(Uint16 WordValue, Uint16 Address)
{
    t_sm_properties *p_sm_properties = bsp_get_sm_properties(MAILBOX_READ);
    bsp_write_word( WordValue, Address);
    if (Address == (p_sm_properties->physical_start_addr+p_sm_properties->length-2)) {
        bsp_pdi_mbx_write_complete();
    } else
        bsp_pdi_write_indication( Address,WordValue);
}

void HW_EscWriteWordIsr(Uint16 WordValue, Uint16 Address)
{
    Int16 sm_index;
    Uint16 ActualAddr = bsp_get_process_data_address( Address, 2, &sm_index);
    if (ActualAddr < ESC_ADDR_MEMORY)
    {
        pd_write_addr_err++;
        return;
    }	
    bsp_write_word( WordValue, ActualAddr);
    bsp_process_data_access_complete( Address, 2, sm_index);
}

void HW_EscWriteByte(Uint8 ByteValue, Uint16 Address)
{
    t_sm_properties *p_sm_properties = bsp_get_sm_properties(MAILBOX_READ);
    bsp_write_byte( ByteValue, Address);
    if (Address == (p_sm_properties->physical_start_addr+p_sm_properties->length-1)) {
        bsp_pdi_mbx_write_complete();
    } else
        bsp_pdi_write_indication( Address, ByteValue);
}

void HW_EscWriteByteIsr(Uint8 ByteValue, Uint16 Address)
{
    Int16 sm_index;
    Uint16 ActualAddr  = bsp_get_process_data_address( Address, 1, &sm_index);
    if (ActualAddr < ESC_ADDR_MEMORY)
    {
        pd_write_addr_err++;
        return;
    }	
    bsp_write_byte( ByteValue, ActualAddr);

    bsp_process_data_access_complete( Address, 1, sm_index);
}

void HW_EscWriteMbxMem(Uint8 * pData, Uint16 Address, Uint16 Len)
{
    t_sm_properties *p_sm_properties = bsp_get_sm_properties(MAILBOX_READ);
    //Do not write to mailbox if already full
    if ((bsp_read_byte ( ESC_ADDR_SM1_STATUS) & SM_STATUS_MBX_FULL))
      return;

    bsp_pdi_mbx_write_start();
    bsp_write( pData, Address, Len);
    if (Len >= p_sm_properties->length-2) {
        bsp_pdi_mbx_write_complete();
    }
}

void HW_SetLed(Uint8 RunLed, Uint8 ErrLed)
{
	vxbGpioSetValueLED(BOARD_TRICOLOR0_GREEN , RunLed);
	vxbGpioSetValueLED(BOARD_TRICOLOR1_RED , ErrLed);
}


unsigned int HW_GetTimer()
{
    return bsp_get_timer_register();
}

void HW_ClearTimer()
{
    bsp_clear_timer_register();
}

Uint16 HW_EepromReload ()
{
    Uint16 retval;
    retval = bsp_eeprom_emulation_reload();
    return retval;
}
/** @} */


