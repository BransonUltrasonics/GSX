/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef SEEKRECIPE_H_
#define SEEKRECIPE_H_

#include  <sys/types.h>
#include  <iostream>

#define SEEK_RAMPTIME_DEFAULT				80
#define SEEK_TIME_DEFAULT					500
#define SEEK_FREQUENCY_OFFSET_20KDEFAULT	0
#define SEEK_FREQUENCY_OFFSET_30KDEFAULT	0
#define SEEK_FREQUENCY_OFFSET_40KDEFAULT	0
#define SEEK_MEMORY_CLEAR					0

typedef struct seekRecipe
{
	INT32  SeekAmplitudeLoopC1;
	INT32  SeekAmplitudeLoopC2;
	INT32  SeekFreqLimitHigh;
	INT32  SeekFreqLimitLow;
	INT32  SeekPhaseloopI;
	INT32  SeekPhaseloopCF;
	INT32  SeekRampTime;
	INT32  SeekFreqOffset;
	INT8   SeekMemClearBeforeSeek;
	UINT32 SeekTime;				/* If new parameter going to add and if it is part of seek recipe OD then add it before SeekTime */
}seekRecipe;

class SeekRecipe
{
public:
	SeekRecipe();
	~SeekRecipe();
	
	UINT32 		GetSeekTime() const;
	void		UpdateSeekRecipe(seekRecipe& recipe);
	seekRecipe&	GetSeekRecipe();
	
private:
	seekRecipe	m_Recipe;
};

#endif /* SEEKRECIPE_H_ */
