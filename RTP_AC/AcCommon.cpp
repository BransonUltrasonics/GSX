/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     This file Contains Common functions for the AC Module
 
**********************************************************************************************************/


/* Header files and Macros */
#include "AcCommon.h"


/* Function Definitions */

/*		
**********************************************************************************************

*      Function    :   PrintMsg
*      Description :   Prints the received message 
*      Input       :   char *msg - Message to be printed
*      				   _Vx_usr_arg_t arg1 to arg6 - Arguments to print, NULL if nothing	
*      Output      :   No return type
*      Called by   :   LogInfo :: PrintMsg

**********************************************************************************************
*/

void LogInfo :: PrintMsg(char *msg, _Vx_usr_arg_t arg1, _Vx_usr_arg_t arg2, _Vx_usr_arg_t arg3, _Vx_usr_arg_t arg4, _Vx_usr_arg_t arg5, _Vx_usr_arg_t arg6)
{
	printf(msg, arg1, arg2, arg3, arg4, arg5, arg6);		// API to print the message
}


/*		
******************************************************************************

*      Function    :   printWRecipe
*      Description :   Prints Weld Recipe data
*      Input       :   AcCtrltaskWeldRecipe& acWeldRecipe - The object to be printed    
*      Output      :   No Return type
*      Called by   :   LogInfo :: printWRecipe

*******************************************************************************
*/

void LogInfo :: printWRecipe(AcCtrltaskWeldRecipe& acWeldRecipe)
{	
	#ifdef DEBUG
	LOG("WeldForce:				%d\n", acWeldRecipe.WeldForce);		
	LOG("ForceRampTime:				%d\n", acWeldRecipe.ForceRampTime);		
	LOG("HoldMode:				%d\n", acWeldRecipe.HoldMode);
	LOG("TotalCollapseTarget:			%d\n", acWeldRecipe.TotalCollapseTarget);				
	LOG("HoldForce:				%d\n", acWeldRecipe.HoldForce);		
	LOG("HoldForceRampTime:			%d\n", acWeldRecipe.HoldForceRampTime);		
	LOG("ExpectedPartContactPosition:		%d\n", acWeldRecipe.ExpectedPartContactPosition);		
	LOG("ReadyOffset:				%d\n", acWeldRecipe.ReadyOffset);		
	LOG("DownAcceleration:			%d\n", acWeldRecipe.DownAcceleration);		
	LOG("DownMaxVelocity:			%d\n", acWeldRecipe.DownMaxVelocity);		
	LOG("DownDeceleration:			%d\n", acWeldRecipe.DownDeceleration);		
	LOG("ReturnAcceleration:			%d\n", acWeldRecipe.ReturnAcceleration);		
	LOG("ReturnMaxVelocity:			%d\n", acWeldRecipe.ReturnMaxVelocity);		
	LOG("ReturnDeceleration:			%d\n", acWeldRecipe.ReturnDeceleration);
	LOG("ExpectedPartContactOffset:			%d\n", acWeldRecipe.ExpectedPartContactOffset);
	LOG("PartContactWindowMinus:			%d\n", acWeldRecipe.PartContactWindowMinus);
	LOG("PartContactWindowPlus:			%d\n", acWeldRecipe.PartContactWindowPlus);
	LOG("NumForceSteps:				%d\n", acWeldRecipe.NumForceSteps);
	LOG("ReadyPositionToggle:				%d\n", acWeldRecipe.ReadyPositionToggle);
	#endif

	for (int index = 0; index < MAX_NUM_OF_FORCE_STEPS; index++)
	{
#ifdef DEBUG	
		LOG("ForceStepRampTime[%d]:			%d\n", index, acWeldRecipe.ForceStepRampTime[index]);						
#endif
	} 	

	for (int index = 0;  index < MAX_NUM_OF_FORCE_STEPS; index++)
	{
#ifdef DEBUG		
		LOG("ForceStepForce[%d]:			%d\n", index, acWeldRecipe.ForceStepForce[index]);					
#endif
	}		
}




