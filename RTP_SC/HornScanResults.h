/*
 * HornScanResults.h
 *
 *  Created on: Jan 31, 2018
 *      Author: lzhu
 */

#ifndef HORNSCANRESULTS_H_
#define HORNSCANRESULTS_H_

#include "Common.h"
#include "WeldRecipe.h"

using namespace std;

typedef enum  
{
	SECOND_PARALLEL_FREQ,
	MAIN_PARALLEL_FREQ,
	TERT_PARALLEL_FREQ,
	FIRST_SERIES_FREQ,
	SECOND_SERIES_FREQ,
	TERT_SERIES_FREQ
}SCAN_RESULT_IDX;


class HornScanResults
{
public:
	HornScanResults();
	UINT32 	GetResonantFrequency(SCAN_RESULT_IDX idx);
	void 	SetResonantFrequency(SCAN_RESULT_IDX idx, UINT32 Frequency);
	void	ResetHornScanResult();
	~HornScanResults();
	
public:
	static SCAN_RESULT_IDX	scanResultIdx;
	
private:
	ScanResult scanResult;
};



#endif /* HORNSCANRESULTS_H_ */
