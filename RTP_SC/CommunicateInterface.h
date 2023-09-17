/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef COMMUNICATEINTERFACE_H_
#define COMMUNICATEINTERFACE_H_

#include "SasSocket.h"

class CommunicateInterface:public SasSocket {

	
	private :
	CommunicateInterface();
	public:
		static CommunicateInterface*  getinstance();
		
		virtual ~CommunicateInterface();
	private:
		static CommunicateInterface *obj;
		static int count;
	
};

#endif /* COMMUNICATEINTERFACE_H_ */
