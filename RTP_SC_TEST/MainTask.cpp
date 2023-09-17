/*
 * MainTask.cpp
 *
 *  Created on: Sep 9, 2022
 *      Author: rvite
 *      
 *  This RTP project has been developed for testing Ethernet IP features
 *  Uses a queue to exchange information with other RTP's
 */

/* Includes */
#include <MainTask.h>
#include <msgQLib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern "C"
{
	#include <customSystemCall.h>
}


/**************************************************************************//**
* 
* \brief   - main. Executes specific test depending on the arguments that receive from the command line. 
*              All tests use a queue to communicate with RTP_SC.
*
* \param   - int argc: number of arguments
* 			 char * argv[]: array of arguments { RTP name, Test_ID, Command_ID, params list
*
* \return  - 0 if success -1 if fails.
******************************************************************************/
int main (int argc,	char * argv[]) // , 
{
	int i;
	int Command_ID = 0;
	TEST_INDEX Test_ID = NONE_TEST;
		
	printf("%sTest Unit: Starting Test Application @ %s%s\n", KYEL, argv[0], KNRM);
	
#if 0    
	printf("Test Unit: Num. arguments: %d\n", argc);
	for (i= 0; i < argc; i++)
		printf("argument[%d]: %s\n", i, argv[i]);
#endif

	if (argc <= ARGV_TEST_ID) // If execute main() without input params, will Auto Run ALL TEST programs with there default test Command  
	{
		Test_ID= NONE_TEST;
	}
	else // argc > ARGV_TEST_ID
	{
		Test_ID= (TEST_INDEX)atoi(argv[ARGV_TEST_ID]);
	}
#if 0    
	if (argc > ARGV_COMMAND_ID) // If execute main() with only one input param, will run the selected test program with its default test Command  
	{
		Command_ID = atoi(argv[ARGV_COMMAND_ID]);	
		printf("RTP_Test Main: Input Command ID = %d\n", Command_ID);
	}
	
	if (argc > ARGV_PARAM_LIST)// The Parameter List starts from the third input
	{
		for(int i = ARGV_PARAM_LIST ; i < argc; i++)
		{
			Paramlist.push_back(atoi(argv[i]));
			printf("RTP_Test Main: Input Parameter = %d\n", atoi(argv[i]));
		}
	}
#endif
	printf("%sTest Unit: Test ID: %d\n%s", KYEL, Test_ID, KNRM);
	switch (Test_ID)
	{
		case MAILBOX_TEST:
			printf("%sTest Unit: MailBox will be Tested..\n%s", KYEL, KNRM);
			MailBoxTest();
			break;
		case NONE_TEST:
			printf("%sTest Unit: No arguments. None Test will be performed. Finishing RTP\n%s", KRED, KNRM);
			break;
		default:
			break;
	}

	return 0;
}


int MailBoxTest()
{	
	MESSAGE		MailQueue, MailQueueReceive;
	STATUS		status = ERROR;
	MSG_Q_ID	msgQId= MSG_Q_ID_NULL, msgReceiveQId; // message queue on which to send
	char		*buffer; // message to send
	size_t		nBytes; // length of message */
	_Vx_ticks_t	timeout; // ticks to wait */
	int			msgQNum, priority; // MSG_PRI_NORMAL or MSG_PRI_URGENT */

	printf("%s- Test Unit:  MailBox is being Tested \n%s", KYEL, KNRM);

	msgQId=  msgQOpen("/RTP_SC_TaskQ", MAX_MSG, MAX_MSG_LEN, MSG_Q_FIFO, OM_CREATE, 0);
	msgReceiveQId=  msgQOpen("/RTP_SC_TaskQ", MAX_MSG, MAX_MSG_LEN, MSG_Q_FIFO, OM_CREATE, 0);
	if (msgQId != MSG_Q_ID_NULL)
	{
		printf("%s- Test Unit: Queue Creation OK.ID #:%X \n%s", KGRN, msgQId, KNRM);
	}
	else
	{
		printf("%s- Test Unit: Queue Creation Failed.\n%s", KRED, KNRM);
		return -1;
	}
	
	MailQueue.msgID= 10;
	strcpy(MailQueue.Buffer, "HOLA");
	if (msgQSend(msgQId, reinterpret_cast<char*>(&MailQueue), sizeof(MailQueue), NO_WAIT, MSG_PRI_NORMAL) == OK)
	{	
		printf("%s- Test Unit: msgQSend Ok.\n%s", KGRN, KNRM);
	}
	else
		printf("%s- Test Unit: msgQSend Error.\n%s", KRED, KNRM);


	msgQNum= msgQNumMsgs(msgQId);
	printf("%s- Test Unit: Messages Num: %d\n%s", KGRN, msgQNum, KNRM);
	
	// Place holder
	// receive the response for the message sent to the queue
	
/*
	MailQueueReceive.msgID= 0;
	strcpy(MailQueueReceive.Buffer, "ADIOS");
	if (msgQReceive(msgQId, (char*) &MailQueueReceive, sizeof(MailQueueReceive), WAIT_FOREVER ) != ERROR)
	{
		printf("%s- Test Unit: msgQReceive Ok. \n   ID: %d\n   Msg: %s\n%s", KGRN, MailQueueReceive.msgID, MailQueueReceive.Buffer, KNRM);
		
	}
	else
	{
		printf("%s- Test Unit: msgQReceive Error.\n%s", KRED, KNRM);
	}
*/
	
	printf("- Test Unit: MailBox has been Tested.\n");
	return status;
}

