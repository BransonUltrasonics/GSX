/************************************************************************** 
     
      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef ESTOP_H_
#define ESTOP_H_

#include "PCState.h"

class Estop : public PCState
{
	public:
	Estop();
	~Estop();
		
	protected:
	void  Enter();
	void  Loop();
	void  Exit() ;
	
	private:
	UINT16 FPGAStat;
};

#endif /* ESTOP_H_ */
