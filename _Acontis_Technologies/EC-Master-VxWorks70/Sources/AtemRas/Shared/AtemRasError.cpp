/*-----------------------------------------------------------------------------
 * AtemRasError.cpp		file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Willig, Andreas
 * Description              description of file
 * Date                     2007/5/9::7:26
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include <AtEmRasType.h>
#include <AtEmRasError.h>
#include <AtEthercat.h>

/*-DEFINES-------------------------------------------------------------------*/

/*-TYPEDEFS------------------------------------------------------------------*/

/*-CLASS---------------------------------------------------------------------*/

/*-FUNCTION DECLARATION------------------------------------------------------*/

/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-FUNCTION-DEFINITIONS------------------------------------------------------*/
/***************************************************************************************************/
/**
\brief  RAS Error Text.
\return Error Text.
*/
const EC_T_CHAR* emRasErrorText(
    EC_T_DWORD dwError      /**< [in]   Error number */
                                    )
{
    return ecatGetText(dwError);
}

/***************************************************************************************************/
/**
\brief  RAS Event Text.
\return Event Text.
*/
const EC_T_CHAR*  emRasEventText(
    EC_T_DWORD dwEvent      /**< [in]   Event number */
                                     )
{
    return ecatGetText(dwEvent);
}

 
/*-END OF SOURCE FILE--------------------------------------------------------*/
