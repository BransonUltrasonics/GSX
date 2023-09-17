/*
 * StateMachineEntryFunc.h
 *
 *  Created on: Mar 2, 2018
 *      Author: eguajardo
 */

#ifndef INC_STATEMACHINEENTRYFUNC_H_
#define INC_STATEMACHINEENTRYFUNC_H_


void PreReadyEntry(void);
void ReadyEntry(void);
void FaultEntry(void);
void FaultResetEntry(void);
void DriveOnEntry(void);
void PCDetectedEntry(void);
void PowerUpEntry(void);

#endif /* INC_STATEMACHINEENTRYFUNC_H_ */

