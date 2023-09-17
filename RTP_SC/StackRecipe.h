/*
 * StackRecipe.h
 *
 *  Created on: Jan 31, 2018
 *      Author: lzhu
 */

#ifndef STACKRECIPE_H_
#define STACKRECIPE_H_

#include "Common.h"
#include "WeldRecipe.h"

class StackRecipe
{
public:
	typedef enum  {MEMORY_OFFSET, MEMORY_CLEAR, DIGITAL_TUNE, MID_BAND, CLEARMEMORY_AT_POWERUP, MEMORY
		  ,INTERNAL_OFFSET_FLAG,INTERNAL_FREQ_OFFSET, END_OF_WELD_STORE,CLEARMEMORY_WITH_RESET,HORN_SCAN_PASSED} Digital_Tune_IDX;

	StackRecipe();
	UINT32 	GetData	( Digital_Tune_IDX idx);
	void 	SetData	( Digital_Tune_IDX, UINT32 data);
	~StackRecipe();

private:
	StackRecipeSC ScStackRecipe;
	INT32  DigitalTune;	
	INT32  MemOffset;
	INT32  InternalOffset;
	UINT32 DigitalTuneMemory;
	static Digital_Tune_IDX	dititaltuneIdx;
};



#endif /* STACKRECIPE_H_ */
