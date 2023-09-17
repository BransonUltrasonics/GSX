/*
 * StateMachineExitFunc.h
 *
 *  Created on: Mar 2, 2018
 *      Author: eguajardo
 */

#ifndef INC_STATEMACHINEEXITFUNC_H_
#define INC_STATEMACHINEEXITFUNC_H_

void PreReadyExit(void);
void ReadyExit(void);
void FaultExit(void);
void FaultResetExit(void);
void DriveOnExit(void);
void PCDetectedExit(void);
void PowerUpExit(void);

#endif /* INC_STATEMACHINEEXITFUNC_H_ */
