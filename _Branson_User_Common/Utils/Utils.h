/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
/*
  * Utils.h
 *
 *  Created on: Feb 6, 2018
 *      Author: DShilonie
 *      
 *  Prototypes for various utilities and quick type safe useful macros that are used
 *  for all components, should be here.
 *  
 */

#ifndef UTILS_H_
#define UTILS_H_

// helper macros for boundary checking
#define _CHECKMAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
     
#define _CHECKMIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

// helper macros for function entry conditions 
#define _CHECK(expr) \
    if (!(expr)) \
		return;

#define _CHECKRETURN(expr,rtn) \
    if (!(expr)) \
		return rtn;


class Utils
{
  public: 
      /* Function to convert metric to imperial or vice versa */
	    float convertNtolbs(float);
	    float convertmillimetretoinches(float);
	    float convertMicrometerToMillimeter(float);
	    float convertMicrometerToInches(float);
	    float convertMMPerSecToInchPerSec(float);
	    float convertMsecToSec(float);
	    float convertlbstoN(float);
	    float convertInchesToMicrometre(float);
	    float convertInchesToMillimetre(float);
    
};

#endif
