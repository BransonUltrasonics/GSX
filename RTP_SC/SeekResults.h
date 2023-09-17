/*
 * SeekResults.h
 *
 *  Created on: Jan 31, 2018
 *      Author: lzhu
 */

#ifndef SEEKRESULTS_H_
#define SEEKRESULTS_H_

#include "Common.h"
#include "WeldRecipe.h"

using namespace std;

class SeekResults 
{
public:

	typedef enum  {
		RECIPE_NUM,
		PEAK_POWER,
		START_FREQ,
		END_FREQ,
		FREQ_CHANGE,
	} SEEK_RESULT_IDX;
	
	SeekResults();
	SeekResults(UINT32 peakPower, UINT32 freqChange, UINT32 startFreq, UINT32 endFreq, UINT32 recipeNum);
	~SeekResults();
	
	UINT32 	Get		(SEEK_RESULT_IDX idx);
	void 	Set		(SEEK_RESULT_IDX idx, UINT32 data);
	
	SeekResult GetSeekData();

private:
	
	SeekResult		results;
	string  		userName;
};




#endif /* SEEKRESULTS_H_ */
