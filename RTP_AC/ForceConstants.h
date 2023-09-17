/*
 * ForceCalibration.h
 *
 *  Created on: Nov 21, 2017
 *      Author: slurie
 */

#ifndef SRC_FORCECALIBRATION_H_
#define SRC_FORCECALIBRATION_H_
#define FORCE_CALIBRATION_POLYNOMIAL_ORDER 1
#define CAL_FILE "/sd0:1/ForceConstants.txt"

#define FORCECONST_BASE 				0x5F
#define FORCECONST_ADDR 				0x60
#define FORCECONST_SIG 	  				0x46
#define FORCECONST_IDENTIFIER_SIZE 		1
#define FORCECONST_VALUE_SIZE			(FORCE_CALIBRATION_POLYNOMIAL_ORDER+1)*sizeof(float)

#define FORCECONSTANT_ONE				-32
#define FORCECONSTANT_TWO               0.3125

#include <vxworks.h>
#include <string>


class ForceConstants {
public:
	ForceConstants();
	virtual ~ForceConstants();
	float GetForceConstant(UINT16);
	void LoadValuesFromFile();
	void LoadValuesFromEeprom();
	void CalculateTwoPointCalibration(UINT32, UINT32, UINT32, UINT32);

private:
	//constants for nth order polynomial force approximations
	float forceConstants[(FORCE_CALIBRATION_POLYNOMIAL_ORDER + 1)];
};

#endif /* SRC_FORCECALIBRATION_H_ */
