/*
 * ecatSlaveBspSource.c
 *
 *  Created on: Apr 21, 2017
 */

#include <string.h> // For memset/memcpy
#include <ecatSlave_eeprom.h> // header equivalent of ESI bin file

#ifndef __ECATSLAVE_HW__
#include <ecatSlaveHw.h>
#endif

#ifndef _ECATSLAVEBSPSOURCE_H_
#include <ecatSlaveBspSource.h>
#endif

#ifndef __INCvxbPruIntcLibh
#include <vxbPruIntcLib.h>
#endif

static t_sm_properties sm_properties[MAX_SYNC_MAN];

static Uint32 current_low, prev_low;//Current and previous IEP time stamps

Uint32 pd_read_addr_err, pd_write_addr_err;
Uint32 pdi_read_fail_cnt,pdi_write_fail_cnt;

extern Uint8 *pEEPROM;
static Uint8 eeprom_updated = 0;
static Uint32 eeprom_updated_time = 0;
Uint8 eeprom_cache[ECAT_SLAVE_EEPROM_SIZE];

inline Uint32 bsp_get_timer_register()
{
    Uint32 ret;
    bsp_get_local_sys_time (&current_low, 0);
    if (current_low > prev_low)
        ret = current_low - prev_low;
    else
        ret = (0xFFFFFFFF - prev_low)+current_low;
    return ret;
}

inline void bsp_clear_timer_register()
{
    bsp_get_local_sys_time (&prev_low, 0);
}

void bsp_get_local_sys_time( Uint32 *systime_low, Uint32 *systime_high)
{
	Uint32 ecat_systime_low, ecat_systime_high;
	    
	    if (systime_high)
	    {
	        bsp_send_command_to_firmware( CMD_DL_USER_READ_SYS_TIME, 0, 0);
	        bsp_global_mutex_lock();
	        ecat_systime_low = ReadDword_dram_iep(HOST2PRU_SYSTEM_TIME_LOW , PRUSS_DRAM0_ACCESS);
	        ecat_systime_high = ReadDword_dram_iep(HOST2PRU_SYSTEM_TIME_HIGH , PRUSS_DRAM0_ACCESS);
	        bsp_global_mutex_unlock();
	    } else {
	       ecat_systime_low = bsp_pruss_iepreg_read(HW_ICSS_IEP_COUNT);
	    }
	    if (systime_low)
	        *systime_low =  ecat_systime_low;

	    if (systime_high)
	        *systime_high = ecat_systime_high;
}

void bsp_eeprom_emulation_init(void)
{
    Uint32 itr = 0;

    #ifdef EEPROM_SPI // To be supported while implementing EEPROM with SPI flash
    Uint16 u16Crc = 0x00FF, i, j;
    int invalid_crc_flag = 0;

    Board_readFlashStorage(SPI_EEPROM_DATA_OFFSET, ECAT_SLAVE_EEPROM_SIZE, (uint8_t*)eeprom_cache);

    if(E_FAIL == Board_readFlashStorage(SPI_EEPROM_DATA_OFFSET, ECAT_SLAVE_EEPROM_SIZE, (uint8_t*)eeprom_cache))
    {
        invalid_crc_flag = 1;
    }
    else
    {
        Uint32 temp_reg = 0;
        for(i = 0; i < 14; i++ )
        {
            u16Crc ^= eeprom_cache[i];
            for(j=0; j<8; j++ )
            {
                if( u16Crc & 0x80 )
                    u16Crc = (u16Crc<<1) ^ 0x07;
                else
                    u16Crc <<= 1;
             }
        }
        /*only low Byte shall be written*/
        u16Crc &= 0x00FF;

        if (u16Crc != eeprom_cache[14])
        {
           invalid_crc_flag = 1;
        }
        temp_reg = *(Uint32*)((Uint8*)(eeprom_cache + (0xC<<1)));

        temp_reg = SWAPDWORD(temp_reg);
        if(temp_reg != (Uint32)PRUESC_REVISION_NUMBER)
        {
            invalid_crc_flag = 1;
        }
    }
    if(1 == invalid_crc_flag)
    {
    #endif

    for (itr = 0; itr < ECAT_SLAVE_EEPROM_SIZE; itr++)
        eeprom_cache[itr] = ecatSlave_eeprom[itr];

    #ifdef EEPROM_SPI
    }
    #endif
    pEEPROM = eeprom_cache;
}


Int32 bsp_eeprom_load_esc_registers ( Int32 reload_flag)
{
	
    //Validate CRC before loading to ESC registers
    Uint16 u16Crc = 0x00FF, i, j;
    Int32 invalid_crc_flag = 0;
    for(i = 0; i < 14; i++ )
    {
        u16Crc ^= eeprom_cache[i];
        for(j=0; j<8; j++ )
        {
            if( u16Crc & 0x80 )
                u16Crc = (u16Crc<<1) ^ 0x07;
            else
                u16Crc <<= 1;
        }
    }
    /*only low Byte shall be written*/
    u16Crc &= 0x00FF;

    if (u16Crc != eeprom_cache[14])
    {
        invalid_crc_flag = 1;
    }
    /*write new claculated Crc to Esc Config area*/
    if (!reload_flag)
    {
        //Not a reload operation - but initial loading
        //0x150 and 0x152 not reloaded according to beckhoff
        if (!invalid_crc_flag)
        {
            bsp_write_byte( eeprom_cache[8], ESC_ADDR_CONFIG_STATION_ALIAS);
            bsp_write_byte( eeprom_cache[9], ESC_ADDR_CONFIG_STATION_ALIAS+1);
            bsp_write_byte( eeprom_cache[0], ESC_ADDR_PDI_CONTROL);
            bsp_write_byte( eeprom_cache[1], ESC_ADDR_PDI_CONTROL+1);
            if (bsp_read_byte( ESC_ADDR_PDI_CONTROL+1) & ESC_PDI_CONTROL_ELD_ALL_PORTS_MASK )
                bsp_write_byte( (bsp_read_byte( ESC_ADDR_DLSTATUS) | 0x4), ESC_ADDR_DLSTATUS);
            else
                bsp_write_byte( (bsp_read_byte( ESC_ADDR_DLSTATUS) & ~(0x4)), ESC_ADDR_DLSTATUS);
        }else {
            return -1;
        }
    }
    if (!invalid_crc_flag)
    {
        bsp_write_byte( eeprom_cache[4], ESC_ADDR_SYNC_PULSE_LENGTH);
        bsp_write_byte( eeprom_cache[5], ESC_ADDR_SYNC_PULSE_LENGTH+1);
    } else {
        return -1;
    }
    return 0;
}

Int32 bsp_eeprom_emulation_reload()
{
    return bsp_eeprom_load_esc_registers ( 1);
}

void bsp_eeprom_emulation_command_ack()
{
    bsp_send_command_to_firmware( CMD_DL_USER_EEPROM_CMD_ACK, 0, 0);
}

void bsp_eeprom_emulation_flush(void)
{
#ifdef EEPROM_SPI
    /* To be implemented when EEPROM emulation with SPI is implemented */
#endif
}

void bsp_eeprom_emulation_exit(void)
{
	
    bsp_eeprom_emulation_flush();
}

void bsp_init_sharedRam()
{
	
    current_low = 0;
    pd_read_addr_err = pd_write_addr_err = 0;
    pdi_read_fail_cnt = pdi_write_fail_cnt = 0;
    prev_low = 0;

    /* initialize ESC DPRAM pointer micro controller specific to the beginning of the physical memory of the ESC,
       the macro MAKE_PTR_TO_ESC should be defined in ecatSlavehw.h */
    memset ((void*)sm_properties, 0, sizeof(sm_properties));

    bsp_eeprom_emulation_init();        //Load eeprom file to memory
    if(bsp_eeprom_load_esc_registers( 0) == -1)
    {
        Uint16 EEPROMReg = 0;
        HW_EscReadWord(EEPROMReg,ESC_ADDR_EEPROM_CTRL);
        EEPROMReg = SWAPWORD(EEPROMReg);
        EEPROMReg |= ESC_EEPROM_ERROR_CRC;
        EEPROMReg = SWAPWORD(EEPROMReg);
        bsp_write_word ( EEPROMReg, ESC_ADDR_EEPROM_CTRL);
    }
}

void bsp_init_dramIep()
{	
	WriteWord_dram_iep( HOST2PRU_CMD_LOW , 0xFF, PRUSS_DRAM0_ACCESS);
}

void bsp_exit()
{

    bsp_eeprom_emulation_exit();        //Flush the EEPROM cache to file

}

void bsp_send_command_to_firmware( Uint32 command, Uint16 param1, Uint16 param2)
{
    uint32_t evtoutNum = 0;
	Int16	intLockKey = 0 ;
	Uint8 accessMemory = 0 ;
	Uint32 pollCount = 0 ;
    {
	
	/* Set memory access to  DRAM 0 */
	accessMemory = PRUSS_DRAM0_ACCESS ;
	
	/* Disable the interrupts */
		intLockKey = intLock();

	/* Write the parameters onto DRAM 0 */
		WriteWord_dram_iep( HOST2PRU_CMD_LOW , command, accessMemory);
		WriteWord_dram_iep( HOST2PRU_PARAM1LOW , param1, accessMemory);
		WriteWord_dram_iep( HOST2PRU_PARAM2LOW , param2, accessMemory);
	
#ifdef SUPPORT_CMDACK_POLL_MODE
		WriteWord_dram_iep( HOST2PRU_CMDLOW_ACK ,1 , accessMemory);
        
#endif
	/* Send ARM to PRU Event 1. Need PRUSS driver call for the same */

        vxbSendEvent(ARM_PRU_EVENT1);
        {
#ifdef SUPPORT_CMDACK_POLL_MODE
            volatile Uint16 ack;
            do {
                ack = ReadWord_dram_iep( HOST2PRU_CMDLOW_ACK, accessMemory ); 
                pollCount++ ;
            } while (ack && pollCount !=0xFFFF );
#else
            /* Currently using CMDACK_POLL_MODE */
		/* Wait for Event from PRUSS */
            /*
            evtoutNum = HOST_CMD_LOW_ACK_EVENT - 20;
            vxbWaitEvent(evtoutNum);
            */`
#endif
        }
		/* Clear the command */
		WriteWord_dram_iep( HOST2PRU_CMD_LOW , 0xFF, accessMemory);
		
		/* Enable all interrupts */
		intUnlock(intLockKey);
    }
}

void bsp_pdi_post_read_indication( Uint16 address)
{
    switch (address) {
    case ESC_ADDR_SM_WD_STATUS:
        {
            bsp_send_command_to_firmware( CMD_DL_USER_READ_PD_WD_STATUS, 0, 0);
        }
        break;
    case ESC_ADDR_SYNC_STATUS:
        {
            bsp_send_command_to_firmware( CMD_DL_USER_READ_SYNC_STATUS, SYNC0, 0);
        }
        break;
    case ESC_ADDR_SYNC_STATUS + 1:
        {
            bsp_send_command_to_firmware( CMD_DL_USER_READ_SYNC_STATUS, SYNC1, 0);
        }
        break;
    case ESC_ADDR_ALCONTROL:
        {
            bsp_send_command_to_firmware( CMD_DL_USER_READ_AL_CONTROL, 0, 0);
        }
        break;
    case ESC_ADDR_SM0_ACTIVATE:
    case ESC_ADDR_SM1_ACTIVATE:
    case ESC_ADDR_SM2_ACTIVATE:
    case ESC_ADDR_SM3_ACTIVATE:
    case ESC_ADDR_SM4_ACTIVATE:
    case ESC_ADDR_SM5_ACTIVATE:
    case ESC_ADDR_SM6_ACTIVATE:
    case ESC_ADDR_SM7_ACTIVATE:
        {
            bsp_send_command_to_firmware( CMD_DL_USER_READ_SM_ACTIVATE, (address-ESC_ADDR_SYNCMAN)>>3, 0);
        }
        break;
    case ESC_ADDR_LATCH0_POS_EDGE:
        {
            bsp_send_command_to_firmware( CMD_DL_USER_READ_LATCH_TIME, LATCH0_POS_EDGE, 0);
        }
        break;

    case ESC_ADDR_LATCH0_NEG_EDGE:
        {
            bsp_send_command_to_firmware( CMD_DL_USER_READ_LATCH_TIME, LATCH0_NEG_EDGE, 0);
        }
        break;

    case ESC_ADDR_LATCH1_POS_EDGE:
        {
            bsp_send_command_to_firmware( CMD_DL_USER_READ_LATCH_TIME, LATCH1_POS_EDGE, 0);
        }
        break;

    case ESC_ADDR_LATCH1_NEG_EDGE:
        {
            bsp_send_command_to_firmware( CMD_DL_USER_READ_LATCH_TIME, LATCH1_NEG_EDGE, 0);
        }
        break;

    default:
        break;
    }
    if (address == sm_properties[MAILBOX_WRITE].physical_start_addr)
    {
        bsp_pdi_mbx_read_start();
    }

}


void bsp_pdi_write_indication( Uint16 address,Uint16 value)
{
    Uint16 regval = 0;
    switch (address) {
    case ESC_ADDR_ALSTATUS:
        {
            bsp_send_command_to_firmware( CMD_DL_USER_WRITE_AL_STATUS, 0, 0);
        }
        break;
    case ESC_ADDR_SM0_PDI_CONTROL:
    case ESC_ADDR_SM1_PDI_CONTROL:
    case ESC_ADDR_SM2_PDI_CONTROL:
    case ESC_ADDR_SM3_PDI_CONTROL:
    case ESC_ADDR_SM4_PDI_CONTROL:
    case ESC_ADDR_SM5_PDI_CONTROL:
    case ESC_ADDR_SM6_PDI_CONTROL:
    case ESC_ADDR_SM7_PDI_CONTROL:
        {
            Uint8 channel = (address-ESC_ADDR_SYNCMAN)>>3;
            Uint16 sm_address = ESC_ADDR_SYNCMAN + (channel * SIZEOF_SM_REGISTER);
            bsp_send_command_to_firmware( CMD_DL_USER_WRITE_SM_PDI_CTRL, (channel << 8 )| value, 0);
            while (bsp_pdi_sm_config_ongoing());
            bsp_set_sm_properties(channel, bsp_read_word( sm_address), bsp_read_word( (sm_address+2)));
        }
        break;
    case ESC_ADDR_EEPROM_CTRL:
        {
            /* Note - Here we are assuming that, Command is always two byte  */
            regval = SWAPWORD(value);
            if((regval & ESC_EEPROM_CMD_WRITE_MASK) &&
               !(regval & ESC_EEPROM_ERROR_CMD_ACK))
            {
                bsp_set_eeprom_update_status(1);
                bsp_set_eeprom_updated_time();
            }
            bsp_eeprom_emulation_command_ack();
        }
        break;
    default:
        break;
    }
    if (address == sm_properties[MAILBOX_READ].physical_start_addr)
    {
        bsp_pdi_mbx_write_start();
    }

}

void bsp_pdi_mbx_read_start()
{
    Uint16  ALEvent = HW_GetALEventRegister_Isr();
   if (ALEvent & MBX_WRITE_EVENT)
   {
       bsp_send_command_to_firmware( CMD_DL_USER_CLEAR_AL_EVENT_LOW, ~MBX_WRITE_EVENT, 0);
   }
}

void bsp_pdi_mbx_read_complete()
{
    /* get address of the receive mailbox sync manager */
    bsp_send_command_to_firmware( CMD_DL_USER_ACK_MBX_READ, sm_properties[MAILBOX_WRITE].physical_start_addr, sm_properties[MAILBOX_WRITE].length);
}
void bsp_pdi_mbx_write_start()
{
   Uint16  ALEvent = HW_GetALEventRegister_Isr();
   if (ALEvent & MBX_READ_EVENT)
   {
       bsp_send_command_to_firmware( CMD_DL_USER_CLEAR_AL_EVENT_LOW, ~MBX_READ_EVENT, 0);
   }
}
void bsp_pdi_mbx_write_complete()
{
    /* get address of the send mailbox sync manager */
    bsp_send_command_to_firmware( CMD_DL_USER_ACK_MBX_WRITE, sm_properties[MAILBOX_READ].physical_start_addr, sm_properties[MAILBOX_READ].length);
}

__inline Int16 bsp_get_sm_index (Uint16 address, Uint16 len)
{
   Int16 sm_index = -1, i;
   for (i=2 ; i < MAX_SYNC_MAN; i++)
   {
        if ((address >= sm_properties[i].physical_start_addr)  && (address+len <= sm_properties[i].physical_start_addr+sm_properties[i].length))
        {
            sm_index = i;
            break;
        }
   }
   return sm_index;
}

__inline void bsp_process_data_access_complete( Uint16 address, Uint16 len, Int16 sm_index)
{
	Uint16 smProcessOffset = HOST2PRU_SM_PROCESS_DATA_0  ;
	t_sm_processdata_1 smProcessData ;
	Uint32 val;

    //Find corresponding SYNC manager and store index in sm_index
    if (((sm_index < 0) || (sm_index >= MAX_SYNC_MAN)))
        return;
    

    if ((address >= sm_properties[sm_index].physical_start_addr) &&
        (address+len == sm_properties[sm_index].  physical_start_addr+sm_properties[sm_index].length))
    {

		smProcessOffset += (((sm_index-2)% MAX_SM_PROCESS_DATA)* sizeof(Uint32)); 

		val = ReadDword_dram_iep(smProcessOffset , PRUSS_DRAM0_ACCESS);
		
			 
		smProcessData.sm_buf_index = ((val) & 0xff);
			 		
			 		smProcessData.lock_state = ((val & 0xff00) >> 8) ;
			 		smProcessData.addr = ((val &  0xffff0000) >> 16) ;	
		

        if (smProcessData.lock_state == LOCK_PD_BUF_HOST_ACCESS_START)
        {
            smProcessData.lock_state = LOCK_PD_BUF_HOST_ACCESS_FINISH;
            WriteByte_dram_iep((smProcessOffset+1) , smProcessData.lock_state , PRUSS_DRAM0_ACCESS);
        }

    }

}

__inline Uint16 bsp_get_process_data_address( Uint16 address, Uint16 len, Int16* p_sm_index)
{
    Uint16 addr = 0;
    Int16 sm_index = bsp_get_sm_index(address, len);
    Uint32 val;
	
	Uint16 smProcessOffset = HOST2PRU_SM_PROCESS_DATA_0  ;
	t_sm_processdata_1 smProcessData ;
    //Find corresponding SYNC manager and store index in sm_index
    if (sm_index == -1) {
        return 0;
    }
    if (p_sm_index)
    {
         *p_sm_index = sm_index;
    }

    /* called while SM is disabled ?! should never happen; just in case... */
    if (sm_properties[sm_index].physical_start_addr == 0)
    {
        return 0;
    }

    /* are we already accessing this sm ?! in this case "lock_state" will be
     * "LOCK_PD_BUF_HOST_ACCESS_START" and we do not need the "while()"-loop... */
	 smProcessOffset += (((sm_index-2)% MAX_SM_PROCESS_DATA)* sizeof(Uint32)); 	 
	 val = ReadDword_dram_iep(smProcessOffset , PRUSS_DRAM0_ACCESS);
	 
	 smProcessData.sm_buf_index = ((val) & 0xff);
	 		
	 smProcessData.lock_state = ((val & 0xff00) >> 8) ;
	 smProcessData.addr = ((val &  0xffff0000) >> 16) ;
    if (smProcessData.lock_state != LOCK_PD_BUF_HOST_ACCESS_START)
    {
        do {
        	val = ReadDword_dram_iep(smProcessOffset , PRUSS_DRAM0_ACCESS);
        		 
        	smProcessData.sm_buf_index = ((val) & 0xff);
        		 		
        		 		smProcessData.lock_state = ((val & 0xff00) >> 8) ;
        		 		smProcessData.addr = ((val &  0xffff0000) >> 16) ; 
        			

        	
            /* May we access the buffer (LOCK_PD_BUF_AVAILABLE_FOR_HOST)? */
            if (smProcessData.lock_state == LOCK_PD_BUF_AVAILABLE_FOR_HOST)
            {
                smProcessData.lock_state = LOCK_PD_BUF_HOST_ACCESS_START;
                WriteByte_dram_iep((smProcessOffset+1) , smProcessData.lock_state , PRUSS_DRAM0_ACCESS);
                ASSERT_DMB();
                break;
            } 
            else
            {
            	taskDelay(0);
            }
        }while(1);
    }
	
    val = ReadDword_dram_iep(smProcessOffset , PRUSS_DRAM0_ACCESS);
    smProcessData.sm_buf_index = ((val) & 0xff);
    	 		
    smProcessData.lock_state = ((val & 0xff00) >> 8) ;
    smProcessData.addr = ((val &  0xffff0000) >> 16) ;    		////printf("smProcessData.sm_buf_index = %x lock_state = %x addr = %x\n",smProcessData.sm_buf_index, smProcessData.lock_state,smProcessData.addr);
    		


    if (smProcessData.lock_state == LOCK_PD_BUF_HOST_ACCESS_START)
    {
       addr = smProcessData.addr;
       addr = addr + (address - sm_properties[sm_index].physical_start_addr);
    }
    return addr;
}

inline void bsp_set_sm_properties (Uint8 sm, Uint16 address, Uint16 length)
{
    sm_properties[sm].physical_start_addr = address;
    sm_properties[sm].length = length;
}

inline t_sm_properties *bsp_get_sm_properties (Uint8 sm)
{
    return &sm_properties[sm];
}

/* PDI access permission to be implemented later as enhancement. Currently returning TRUE */

inline Uint32 bsp_get_pdi_read_access_fail_cnt()
{
    return pdi_read_fail_cnt;
}

inline Uint32 bsp_get_pdi_write_access_fail_cnt()
{
    return pdi_write_fail_cnt;
}

inline Uint8  bsp_get_pdi_access_perm(Uint16 address, Uint8 access)
{    return 1;
}

inline Uint8  bsp_pdi_access_perm_word(Uint16 address, Uint8 access)
{
    return 1;
}

inline Uint8  bsp_pdi_access_perm_dword(Uint16 address, Uint8 access)
{
    return 1;

}

inline Uint8  bsp_pdi_access_perm_array(Uint16 address, Uint8 access, Uint16 size)
{
    return 1;
}


Uint32 bsp_read_dword( Uint16 address)
{
    Uint32 DWordValue, end_addr = sm_properties[MAILBOX_WRITE].physical_start_addr+sm_properties[MAILBOX_WRITE].length;

    if(0 == bsp_pdi_access_perm_dword(address, PRUESC_PERM_READ))
    {
        pdi_read_fail_cnt++;
        return 0;
    }
    DWordValue = ReadDword_shdram( address);

    if ((address >= (end_addr-4)) &&
        (address < end_addr)) {
        bsp_pdi_mbx_read_complete();
    } else
        bsp_pdi_post_read_indication( address);
    return DWordValue;
}

Uint32 bsp_read_dword_isr( Uint16 address)
{
    Uint32 DWordValue;
    DWordValue = ReadDword_shdram( address);
    return DWordValue;
}

Uint16 bsp_read_word( Uint16 address)
{
    Uint16 WordValue, end_addr = sm_properties[MAILBOX_WRITE].physical_start_addr+sm_properties[MAILBOX_WRITE].length;

    if(0 == bsp_pdi_access_perm_word(address, PRUESC_PERM_READ))
    {
        pdi_read_fail_cnt++;
        return 0;
    }

    WordValue = ReadWord_shdram(address);

    if ((address >= (end_addr-2)) &&
        (address < end_addr)) {
        bsp_pdi_mbx_read_complete();
    } else
        bsp_pdi_post_read_indication( address);
   return WordValue;
}

Uint16 bsp_read_word_isr( Uint16 address)
{
    Uint16 WordValue;
    WordValue = ReadWord_shdram(address);
    return WordValue;
}

Uint8 bsp_read_byte( Uint16 address)
{
    Uint8 ByteValue;

    if(0 == bsp_get_pdi_access_perm(address, PRUESC_PERM_READ))
    {
        pdi_read_fail_cnt++;
        return 0;
    }

    ByteValue = ReadByte_shdram(address);

    if (address == (sm_properties[MAILBOX_WRITE].physical_start_addr+sm_properties[MAILBOX_WRITE].length-1)) {
        bsp_pdi_mbx_read_complete();
    } else
        bsp_pdi_post_read_indication( address);
    return ByteValue;
}

inline Uint8 bsp_read_byte_isr( Uint16 address)
{
    Uint8 ByteValue;
    ByteValue = ReadByte_shdram(address);
    return ByteValue;
}

void bsp_read( Uint8 *pdata, Uint16 address, Uint16 len)
{
    if(0 == bsp_pdi_access_perm_array(address, PRUESC_PERM_READ, len))
    {
        pdi_read_fail_cnt++;
        return;
    }

	ReadBlock_shdram( address, pdata, len);
}
void bsp_write_dword( Uint32 val, Uint16 address)
{
    if(0 == bsp_pdi_access_perm_dword(address, PRUESC_PERM_WRITE))
    {
        pdi_write_fail_cnt++;
        return;
    }

	WriteDword_shdram(address, val);
	
    ASSERT_DMB();
}
void bsp_write_word( Uint16 val, Uint16 address)
{
    if(0 == bsp_pdi_access_perm_word(address, PRUESC_PERM_WRITE))
    {
        pdi_write_fail_cnt++;
        return;
    }

	WriteWord_shdram(address, val); 
    
    ASSERT_DMB();
}
 void bsp_write_byte( Uint8 val, Uint16 address)
{
    if(0 == bsp_get_pdi_access_perm(address, PRUESC_PERM_WRITE))
    {
        pdi_write_fail_cnt++;
        return;
    }

	WriteByte_shdram( address, val);
    
    ASSERT_DMB();
}
void bsp_write( Uint8 *pdata, Uint16 address, Uint16 len)
{
    if(0 == bsp_pdi_access_perm_array(address, PRUESC_PERM_WRITE, len))
    {
        pdi_write_fail_cnt++;
        return;
    }

	WriteBlock_shdram( address, pdata, len);
	
    ASSERT_DMB();
    ASSERT_DSB();
}

Uint32 bsp_pruss_iepreg_read ( Uint32 regoffset)
{
    Uint32 regval = 0 ;
	regval = ReadDword_dram_iep( regoffset, PRUSS_IEP_ACCESS) ;
    return regval;
}

inline void bsp_pruss_iepreg_write ( Uint32 val, Uint32 regoffset)
{
    WriteDword_dram_iep( regoffset, val , PRUSS_IEP_ACCESS);
    ASSERT_DMB();
}


inline Uint16 bsp_pruss_cmd_intfc_read_word (volatile Uint16 *ptr)
{
    Uint16 val;
    val = *ptr;
    return val;
}
inline void bsp_pruss_cmd_intfc_write_word (Uint16 val, volatile Uint16 *ptr)
{
    *ptr = val;
    ASSERT_DMB();
}

inline Uint8 bsp_pdi_sm_config_ongoing ()
{
	/* Read the SM config Ongoing from DRAM0 */
	return ReadByte_dram_iep( HOST2PRU_SM_CONFIG_ONGOING , PRUSS_DRAM0_ACCESS);
}

void bsp_global_mutex_lock (void)
{
    //Disable PDI and SYNC0 ISR at ARM INTC (rather than global IRQ disable)
	
	intDisable(HOST_AL_EVENT+32);
	intDisable(HOST_SYNC0_EVENT+32);
}

void bsp_global_mutex_unlock (void)
{
	//Enable back PDI and SYNC0 ISR at ARM INTC
	intEnable(HOST_AL_EVENT + 32);
	intEnable(HOST_SYNC0_EVENT + 32);
}

void bsp_set_eeprom_update_status(Uint8 status)
{
    eeprom_updated = status;
}

inline Uint8 bsp_get_eeprom_update_status(void)
{
    return eeprom_updated;
}

inline Uint8 *bsp_get_eeprom_cache_base(void)
{
    return eeprom_cache;
}

inline void bsp_set_eeprom_updated_time()
{
    bsp_get_local_sys_time(&eeprom_updated_time, NULL);
}

inline Uint32 bsp_get_eeprom_updated_time()
{
    return eeprom_updated_time;
}
