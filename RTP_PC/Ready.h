/*
 * Ready.h
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */

#ifndef READY_H_
#define READY_H_

#include "PCState.h"

class Ready : public PCState
{
	public:
		Ready();
		~Ready();
		
	protected:
	void  Enter();
	void  Loop();
	void  Exit() ;
	
	private:
};

#endif /* READY_H_ */
