#ifndef ACDGTIOCTRL_H_
#define ACDGTIOCTRL_H_
#include "ProductionTest.h"

SEM_ID AcDgtInputTaskSem;

void AcDgtCtlInTask();
UINT8 GetDigitalIn();

#ifdef NEW_AC
INT16 DgtGetDinPort(UINT8* val);
// Digital IO Input Task
void AcDgtInputTask();	
void AcSetDinValue(UINT8 in_val);

//For IO manager
UINT8 AcGetDinValue(void);
#endif  //#ifdef NEW_AC

#endif /* ACDGTIOCTRL_H_ */
