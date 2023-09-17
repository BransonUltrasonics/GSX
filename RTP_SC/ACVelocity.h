#ifndef ACVELOCITY_H_
#define ACVELOCITY_H_

#define VELOCITY_BUFFER_SIZE 40

#include "Common.h"

class ACVelocity {
public:
	ACVelocity();
	virtual ~ACVelocity();
	static void ClearVelocityBuffer();
	static INT32 GetVelocity();
	static void CalculateVelocity();
private:
	static INT32 instVelocities[VELOCITY_BUFFER_SIZE];
	static UINT8 i;
	static INT32 lastPos;
	static INT32 movingAverage;
};

#endif /* ACVELOCITY_H_ */
