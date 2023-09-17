
/**
 * ecatSlaveAppl.c
 *
*/

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#if (!defined(POWER_CONTROLLER_BUILD) && !defined(ACTUATION_CONTROLLER_BUILD))
#include <ecat_def.h>

#if ECAT_SLAVE_APPL

#if COE_SUPPORTED
#include <objdef.h>
#endif
#include <ecatappl.h>

#define _PRUESC_HW_
#include <ecatslv.h>
#ifdef POWER_CONTROLLER_BUILD
#include <PcEcAppl.h>
#else
#ifdef ACTUATION_CONTROLLER_BUILD
#include <AcEcAppl.h>
#else
#include <ecatSlaveAppl.h>
#endif
#endif

#undef _PRUESC_HW_

#define TXPDO_BUFFER_DATA_LENGTH 2
#define TXPDO_SIZE_BYTES 8

Uint32 txPdoBuffData[TXPDO_BUFFER_DATA_LENGTH][TXPDO_SIZE_BYTES] = {
		{0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88},
		{0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89}
};
/*--------------------------------------------------------------------------------------
------
------    local types and defines
------
--------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------    local variables and constants
------
-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------    application specific functions
------
-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------
------
------    generic functions
------
-----------------------------------------------------------------------------------------*/
/////////////////////////////////////////////////////////////////////////////////////////
/**

\brief This function resets the outputs
*////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    Function resets the RxPDO object contents.

*////////////////////////////////////////////////////////////////////////////////////////
void PDO_ResetOutputs(void)
{
#if !defined(POWER_CONTROLLER_BUILD) && !defined(ACTUATION_CONTROLLER_BUILD)
	sRxPDOPowerDataStructure1.LaserChannelTemp = 0x0 ;
	sRxPDOPowerDataStructure2.LaserChannelTemp = 0x0 ;
	sRxPDOPowerDataStructure3.LaserChannelTemp = 0x0 ;
	sRxPDOPowerDataStructure4.LaserChannelTemp = 0x0 ;
	sRxPDOPowerDataStructure5.LaserChannelTemp = 0x0 ;
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    The function is called when an error state was acknowledged by the master

*////////////////////////////////////////////////////////////////////////////////////////

void APPL_AckErrorInd(Uint16 stateTrans)
{
	return ;

}
/*ECATCHANGE_START(V4.42.1) ECAT2*/
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from INIT to PREOP when
             all general settings were checked to start the mailbox handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
            The return code NOERROR_INWORK can be used, if the application cannot confirm
            the state transition immediately, in that case this function will be called cyclically
            until a value unequal NOERROR_INWORK is returned

*////////////////////////////////////////////////////////////////////////////////////////

Uint16 APPL_StartMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from PREEOP to INIT
             to stop the mailbox handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

Uint16 APPL_StopMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    pIntMask    pointer to the AL Event Mask which will be written to the AL event Mask
                        register (0x204) when this function is succeeded. The event mask can be adapted
                        in this function
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from PREOP to SAFEOP when
             all general settings were checked to start the input handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
             When returning ALSTATUSCODE_NOERROR, the inputs has to be updated once before return.
            The return code NOERROR_INWORK can be used, if the application cannot confirm
            the state transition immediately, in that case the application need to be complete 
            the transition by calling ECAT_StateChange
*////////////////////////////////////////////////////////////////////////////////////////

Uint16 APPL_StartInputHandler(Uint16 *pIntMask)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from SAFEOP to PREEOP
             to stop the input handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

Uint16 APPL_StopInputHandler(void)
{
    return ALSTATUSCODE_NOERROR;

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from SAFEOP to OP when
             all general settings were checked to start the output handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
            The return code NOERROR_INWORK can be used, if the application cannot confirm
            the state transition immediately, in that case this function will be called cyclically
            until a value unequal NOERROR_INWORK is returned
*////////////////////////////////////////////////////////////////////////////////////////

Uint16 APPL_StartOutputHandler(void)
{
    /*always return NOERROR_INWORK the state transition will be finished by calling AL_ControlRes*/
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from OP to SAFEOP
             to stop the output handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

Uint16 APPL_StopOutputHandler(void)
{
    PDO_ResetOutputs();
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0(ALSTATUSCODE_NOERROR), NOERROR_INWORK
\param      pInputSize  pointer to save the input process data length
\param      pOutputSize  pointer to save the output process data length

\brief    This function calculates the process data sizes from the actual SM-PDO-Assign
            and PDO mapping
            The function also displays onto console the slave physical address and the
            RxPDO output and TxPDO input size in bytes.
*////////////////////////////////////////////////////////////////////////////////////////
Uint16 APPL_GenerateMapping(Uint16* pInputSize, Uint16* pOutputSize)
{	
	Uint16 result = 0;
	Uint16 PDOAssignEntryCnt = 0;
	OBJCONST TOBJECT OBJMEM * pPDO = NULL;
	Uint16 PDOSubindex0 = 0;
	Uint32 *pPDOEntry = NULL;
	Uint16 PDOEntryCnt = 0;
	Uint16 InputSize = 0;
	Uint16 OutputSize = 0;
	Uint16 ecatAddr = 0 ;
	Int8 printVar[150] ;

	/*Scan object 0x1C12 RXPDO assign*/
	for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sRxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
	{
		pPDO = OBJ_GetObjectHandle(sRxPDOassign.aEntries[PDOAssignEntryCnt]);
		if(pPDO != NULL)
		{
			PDOSubindex0 = *((Uint16 *)pPDO->pVarPtr);
			for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
			{
				pPDOEntry = (Uint32 *)((Uint8 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3));        //goto PDO entry
				// we increment the expected output size depending on the mapped Entry 
				OutputSize += (Uint16) ((*pPDOEntry) & 0xFF);
			}
		}
	}
		
	OutputSize = (OutputSize + 7) >> 3;

	/*Scan Object 0x1C13 TXPDO assign*/
	for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sTxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
	{
		pPDO = OBJ_GetObjectHandle(sTxPDOassign.aEntries[PDOAssignEntryCnt]);
		if(pPDO != NULL)
		{
			PDOSubindex0 = *((Uint16 *)pPDO->pVarPtr);
			for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
			{
				pPDOEntry = (Uint32 *)((Uint8 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3));        //goto PDO entry
				// we increment the expected output size depending on the mapped Entry 
				InputSize += (Uint16) ((*pPDOEntry) & 0xFF);
			}
		}
	}
	InputSize = (InputSize + 7) >> 3;

	/* Display on console the RxPDO input and TxPDO output size calculated from slave PDO mapping objects */

	*pInputSize = InputSize;
	*pOutputSize = OutputSize;
	return result;

}


/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to input process data
\brief      This function will copy the TxPDO inputs from the local memory to the ESC memory
			from where it is sent to master through EtherCAT packet.
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_InputMapping(Uint16* pData)
{
#if !defined(POWER_CONTROLLER_BUILD) && !defined(ACTUATION_CONTROLLER_BUILD)
    Uint16 j = 0;
    Uint16 k = 0 ;
    Uint8 *pTmpData = (Uint8 *)pData;


   for (j = 0; j < sTxPDOassign.u16SubIndex0; j++)
   {
      switch (sTxPDOassign.aEntries[j])
      {
      /* TxPDO 1 */
      /* Copying each 1 byte data from TxPDO object content(0x200E- bank diagnostic information)
       * onto local memory using the TxPDO-1 mapping entries values */
      case TXPDO1_INDEX:
    	  *pTmpData++ = sTxPDOBankDiagInfo.OperationalStateBitField ;
    	  *pTmpData++ = sTxPDOBankDiagInfo.Humidity ;
    	  *pTmpData++ = sTxPDOBankDiagInfo.BankTemperature ;
         break;

         /* TxPDO-2 */

         /* Copying each 1 byte data from TxPDO object contents(Laser Channel Status Structure)
          * onto local memory using the TxPDO-2 mapping entries values */
      case TXPDO2_INDEX: // attention: maybe a non-aligned byte access...
		
    	  *pTmpData++ = sTxPDOLaserChState1.LaserChannelState ;
    	  *pTmpData++ = sTxPDOLaserChState2.LaserChannelState ;
    	  *pTmpData++ = sTxPDOLaserChState3.LaserChannelState ;
    	  *pTmpData++ = sTxPDOLaserChState4.LaserChannelState ;
    	  *pTmpData++ = sTxPDOLaserChState5.LaserChannelState ;
         break;
      }
   }
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to output process data

\brief    This function will copies the outputs from the ESC memory to the local memory
           from where ethercat slave copies the output data to RxPDO object contents
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_OutputMapping(Uint16* pData)
{
#if !defined(POWER_CONTROLLER_BUILD) && !defined(ACTUATION_CONTROLLER_BUILD)
    Uint16 j = 0;

    Uint16 k = 0;
    Uint8 *pTmpData = (Uint8 *)pData;// allow byte processing

    for (j = 0; j < sRxPDOassign.u16SubIndex0; j++)
    {
        switch (sRxPDOassign.aEntries[j])
        {
        /* RxPDO */
		/* Copy from local memory output data onto RxPDO object(Power data structure)
		 * according to the values of RxPDO map entries */
        case RXPDO1_INDEX:
            sRxPDOPowerDataStructure1.LaserChannelTemp = *pTmpData++ ;
            sRxPDOPowerDataStructure2.LaserChannelTemp = *pTmpData++ ;
            sRxPDOPowerDataStructure3.LaserChannelTemp = *pTmpData++ ;
            sRxPDOPowerDataStructure4.LaserChannelTemp = *pTmpData++ ;
            sRxPDOPowerDataStructure5.LaserChannelTemp = *pTmpData++ ;
            break;
        }
    }
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    This function displays the RxPDO object contents onto console(when modified)
		   and populates the TxPDO object contents with predefined values from buffer in slave.
		   The TxPDO object contents are displayed before the TxPDO input data is sent from slave.
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_Application(void)
{
#if !defined(POWER_CONTROLLER_BUILD) && !defined(ACTUATION_CONTROLLER_BUILD)
	static Uint8 txPdoBuffInd = 0 ; /* pingPongBuffer index */
	static Uint8 txPdoCycleCount = 0 ;
	static Uint8 prevTxPdoBuffVal = 0xFF ;

	if(TRUE == bEcatOutputUpdateRunning)
	{
#ifdef CONSOLE_PDO_DEBUG
		printf("\nEcSlave:0x2208:03-	LaserChannelTemp1	=	0x%x",sRxPDOPowerDataStructure1.LaserChannelTemp);
		printf("\nEcSlave:0x2308:03-	LaserChannelTemp2	=	0x%x",sRxPDOPowerDataStructure2.LaserChannelTemp);
		printf("\nEcSlave:0x2408:03-	LaserChannelTemp3	=	0x%x",sRxPDOPowerDataStructure3.LaserChannelTemp);
		printf("\nEcSlave:0x2508:03-	LaserChannelTemp4	=	0x%x",sRxPDOPowerDataStructure4.LaserChannelTemp);
		printf("\nEcSlave:0x2608:03-	LaserChannelTemp5	=	0x%x\n",sRxPDOPowerDataStructure5.LaserChannelTemp);
#endif
	}

	
	sTxPDOLaserChState1.LaserChannelState				= txPdoBuffData[txPdoBuffInd][0];
	sTxPDOLaserChState2.LaserChannelState				= txPdoBuffData[txPdoBuffInd][1];
	sTxPDOLaserChState3.LaserChannelState				= txPdoBuffData[txPdoBuffInd][2];
	sTxPDOLaserChState4.LaserChannelState				= txPdoBuffData[txPdoBuffInd][3];
	sTxPDOLaserChState5.LaserChannelState				= txPdoBuffData[txPdoBuffInd][4];
	sTxPDOBankDiagInfo.OperationalStateBitField			= txPdoBuffData[txPdoBuffInd][5];
	sTxPDOBankDiagInfo.Humidity							= txPdoBuffData[txPdoBuffInd][6];
	sTxPDOBankDiagInfo.BankTemperature					= txPdoBuffData[txPdoBuffInd][7];


	if((TRUE == bEcatInputUpdateRunning) && (prevTxPdoBuffVal != txPdoBuffInd))
	{
#ifdef CONSOLE_PDO_DEBUG
		printf("\nEcSlave:0x200E:05-	Operational State Bit Field			= 0x%x",sTxPDOBankDiagInfo.OperationalStateBitField) ;
		printf("\nEcSlave:0x200E:02-	Humidity							= 0x%x",sTxPDOBankDiagInfo.Humidity) ;
		printf("\nEcSlave:0x200E:03-	Bank Temperature					= 0x%x",sTxPDOBankDiagInfo.BankTemperature) ;
		printf("\nEcSlave:0x2204:02-	LaserChannelState1					= 0x%x",sTxPDOLaserChState1.LaserChannelState) ;
		printf("\nEcSlave:0x2304:02-	LaserChannelState2					= 0x%x",sTxPDOLaserChState2.LaserChannelState) ;
		printf("\nEcSlave:0x2404:02-	LaserChannelState3					= 0x%x",sTxPDOLaserChState3.LaserChannelState) ;
		printf("\nEcSlave:0x2504:02-	LaserChannelState4					= 0x%x",sTxPDOLaserChState4.LaserChannelState) ;
		printf("\nEcSlave:0x2604:02-	LaserChannelState5					= 0x%x\n",sTxPDOLaserChState5.LaserChannelState) ;
#endif
		prevTxPdoBuffVal = txPdoBuffInd ;
	}

	txPdoCycleCount++ ;
	
	if(txPdoCycleCount == TXPDO_BUFFER_DATA_LENGTH)
	{
		txPdoBuffInd++ ;
		txPdoBuffInd %= TXPDO_BUFFER_DATA_LENGTH ;
		txPdoCycleCount = 0 ;
	}
#endif
	
}

/*ECATCHANGE_END(V4.42.1) ECAT2*/
#endif //#if PRUESC_HW
#endif
/** @} */
