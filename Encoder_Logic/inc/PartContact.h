// $Header:   D:/databases/VMdb/archives/EN13849/inc/PartContact.h_v   1.20   09 Apr 2015 17:18:22   ewomack  $
/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 2010-11              */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------
This module has the code that identifies the Part Contact.

 Module name: PartContact
 File name: PartContact.c
 -------------------------- TECHNICAL NOTES --------------------------------
Updates the status of the detection of the part contact.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/PartContact.h_v  $
 * 
 *    Rev 1.20   09 Apr 2015 17:18:22   ewomack
 * Change FORCESAMPLETIME to 1 so force is updated every millisecond instead of every 3.  Changed unit test defines.
 * 
 *    Rev 1.19   02 Jan 2015 14:57:54   ewomack
 * Unit Test Fixes
 * 
 *    Rev 1.18   19 Sep 2011 10:35:50   ASharma6
 * For IW+ only: After ULS, the Trigger switch is not looked at for declaring part contact until the horn goes 1/8 of an inch past the ULS.
 * 
 *    Rev 1.17   24 Aug 2011 11:18:28   ASharma6
 * For IW+ only - if in part contact state, if palm buttons are still pressed then stay in that state and the horn position becomes the new reference for collapse until the hands are released.
 * 
 *    Rev 1.15   03 Aug 2011 10:35:22   ASharma6
 * For machine configuration C, the part contact lost algorithm shall declare lost 150ms after trigger goes away.
 * 
 *    Rev 1.14   20 Jul 2011 16:47:56   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.13   12 Jul 2011 18:32:50   ASharma6
 * Revision 19 with modifications in Part contact lost algorithm - Loss of force alongwith horn retraction 
 * 
 *    Rev 1.12   05 Jul 2011 10:57:04   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.9   22 Jun 2011 18:14:44   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.4   01 Apr 2011 11:29:46   PDwivedi
 * Fixex after review.
 * 
 *    Rev 1.3   30 Mar 2011 11:22:16   ASharma6
 * Converted tabs to 3 spaces each
 *
 *    Rev 1.2   29 Mar 2011 19:19:10   ASharma6
 * Modifications done for compliance to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:18:40   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef PARTCONTACT_H_
#define PARTCONTACT_H_

#define PARTCONTACT_COUNTER        10
#define SENSING_INTERVAL           6 // Was 10
#define MINIMAL_POSITION           20000
#define FORCE_MULFAC               1000
#define MIMIMAL_VELOCITY           -20
#define PCLOST_COUNTER             180 // As per the weld controller
#define PCFOUND_COUNTER            2
#define SPEED_FACTOR               3 // Was 3
#define FORCE_FACTOR               123 // Half of force factor
#define SCALING_FACTOR				100
#define DISTANCE_INTERVAL          5 // Was 9
#define MAXFORCESAMPLES            10
#define VELOCITY_FACTOR            2
#define FORCESAMPLETIME			     1 // Every 1ms.
#define FORCE_MULTIPLIER		     3
#define FORCE_DIVIDER              4
#define MAX_FORCE_SAMPLE 14
#define SMALL_FORCE 			 12302	// 5 lbs
#define LARGE_FORCE              24604  //10 lbs 
#define FORCE_25LBS				 61508	//25 lbs

/*
 * For 2000X:
 * Encoder changes 1 count per 1μm
 * So, per meter it is 10^6
 * Or per inch, it is 25000
 *
 * Collapse distance is 1/4th of an inch. So, it is:
 * 25000/4 = 6250
 *
 * Retract distance is 1/10th of an inch. So, it is:
 * 25000/10 = 2500
 */
#define COLLAPSEDISTGSXE_OR_P          62500 // For quarter of an inch
#define RETRACTDISTGSXE_OR_P            25000

/*
 * For IW+:
 * Encoder changes 1 count per 2.5μm
 * So, per meter it is 400,000
 * Or per inch, it is 10,000
 *
 * Collapse distance is 1/4th of an inch. So, it is:
 * 10000/4 = 2500
 *
 * Trigger bounces around ULS position, so until horn goes past
 * 1/8th of an inch, the TRS should not be looked at for declaring part
 * contact.
 * 1/8th of an inch = 10000/8 = 1250
*/
#define COLLAPSEDISTGSXI             25000
#define BOUNCYDISTGSXI               12500

// For Testing only
#define TESTFORCESAMPLEVALUE1 967
#define TESTFORCESAMPLEVALUE2 968
#define TESTFORCESAMPLEVALUE3 973
#define TESTFORCESAMPLEVALUE4 966
#define TESTFORCESAMPLEVALUE5 983
#define TESTFORCESAMPLEVALUE6 987
#define TESTFORCESAMPLEVALUE7 983
#define TESTFORCESAMPLEVALUE8 976
#define TESTFORCESAMPLEVALUE9 994
#define TESTFORCESAMPLEVALUE10 996
#define TESTFORCESAMPLEVALUE11 998
#define TESTDISTSAMPLEVALUE1 200
#define TESTDISTSAMPLEVALUE2 205
#define TESTDISTSAMPLEVALUE3 215
#define TESTDISTSAMPLEVALUE4 220
#define TESTDISTSAMPLEVALUE5 215
#define TESTDISTSAMPLEVALUE6 210
#define TESTDISTSAMPLEVALUE7 205
#define TESTDISTSAMPLEVALUE8 210
#define TESTDISTSAMPLEVALUE9 210
#define TESTDISTSAMPLEVALUE10 210
#define TESTDISTSAMPLEVALUE11 0
#define MAXSAMPLES 11

// Testing values 1
#define TESTAVGFORCETENSAMPLES1 973
#define TESTQEIPOSITION1        0
#define TESTVELOCITYTEST1       5
#define TESTPEAKVELOCITYTEST1   -2
#define TESTPARTCONTACTCOUNTER1  1
#define TESTPCINDEX1             0
#define TESTNEWVELOCITY1         25


// Testing values 2
#define TESTAVGFORCETENSAMPLES2 979
#define TESTQEIPOSITION2        37529
#define TESTVELOCITYTEST2       -5
#define TESTPEAKVELOCITYTEST2   -2
#define TESTPARTCONTACTCOUNTER2  2
#define TESTPCINDEX2             0
#define TESTNEWVELOCITY2         6

// Testing values 3
#define TESTAVGFORCETENSAMPLES3  980
#define TESTPREVFORCE3           979
#define TESTPREVDIFFFORCE3       2
#define TESTQEIPOSITION3         11000
#define TESTVELOCITYTEST3        2
#define TESTPEAKVELOCITYTEST3    7
#define TESTPARTCONTACTCOUNTER3  2
#define TESTCURRDIFFFORCE3       -1
#define TESTPCINDEX3             0
#define TESTQEIPOSITION0         22000

// Testing values 4
#define TESTAVGFORCETENSAMPLES4  972
#define TESTPREVFORCE4           974
#define TESTPREVDIFFFORCE4       4
#define TESTQEIPOSITION4         11000
#define TESTVELOCITYTEST4        2
#define TESTPEAKVELOCITYTEST4    7
#define TESTPARTCONTACTCOUNTER4  2
#define TESTPCINDEX4             0

// Testing values 5
#define TESTAVGFORCETENSAMPLES5  970
#define TESTPREVFORCE5           973
#define TESTPREVDIFFFORCE5       2
#define TESTQEIPOSITION5         12000
#define TESTVELOCITYTEST5        2
#define TESTPEAKVELOCITYTEST5    7
#define TESTPARTCONTACTCOUNTER5  6
#define TESTPCINDEX5             0

// Testing values 6
#define TESTAVGFORCETENSAMPLES6  980
#define TESTPREVFORCE6           973
#define TESTPREVDIFFFORCE6       2
#define TESTVELOCITYTEST6        2
#define TESTPEAKVELOCITYTEST6    7
#define TESTPARTCONTACTCOUNTER6  1
#define TESTPCINDEX6             0
#define TESTCURRDIFFFORCE6       3
#define TESTQEIPOSITION6         22000

// Testing values 7
#define TESTAVGFORCETENSAMPLES7  970
#define TESTPREVFORCE7           973
#define TESTPREVDIFFFORCE7       2
#define TESTQEIPOSITION7         22000
#define TESTVELOCITYTEST7        2
#define TESTPEAKVELOCITYTEST7    7
#define TESTPARTCONTACTCOUNTER7  4
#define TESTCURRDIFFFORCE7       -1
#define TESTPCINDEX7             1

// Testing values 8
#define TESTAVGFORCETENSAMPLES8  974
#define TESTPREVFORCE8           973
#define TESTPREVDIFFFORCE8       2
#define TESTQEIPOSITION8         22000
#define TESTVELOCITYTEST8        2
#define TESTPEAKVELOCITYTEST8    7
#define TESTPARTCONTACTCOUNTER8  4
#define TESTCURRDIFFFORCE8       1
#define TESTPCINDEX8             1

//// Testing values 8

//
// Testing values 9
#define TESTAVGFORCETENSAMPLES9  10
#define TESTPREVFORCE9           973
#define TESTPREVDIFFFORCE9       4
#define TESTQEIPOSITION9         963470
#define TESTVELOCITYTEST9        12
#define TESTPEAKVELOCITYTEST9    10
#define TESTPARTCONTACTCOUNTER9  0
#define TESTCURRDIFFFORCE9       3
#define TESTPCINDEX9             51

// Testing values 10
#define TESTAVGFORCETENSAMPLES10  993
#define TESTPREVFORCE10           973
#define TESTPREVDIFFFORCE10       4
#define TESTQEIPOSITION10         375380
#define TESTVELOCITYTEST10        12
#define TESTPEAKVELOCITYTEST10    10
#define TESTPARTCONTACTCOUNTER10  6
#define TESTCURRDIFFFORCE10       3
#define TESTPCINDEX10             3

// Testing values 11
#define TESTAVGFORCETENSAMPLES11  993
#define TESTPREVFORCE11           973
#define TESTPREVDIFFFORCE11       4
#define TESTQEIPOSITION11         375380
#define TESTVELOCITYTEST11        12
#define TESTPEAKVELOCITYTEST11    10
#define TESTPARTCONTACTCOUNTER11  6
#define TESTCURRDIFFFORCE11       3
#define TESTPCINDEX11             3

// Testing values 12
#define TESTAVGFORCETENSAMPLES12  957
#define TESTPREVFORCE12           973
#define TESTPREVDIFFFORCE12       4
#define TESTQEIPOSITION12         963470
#define TESTVELOCITYTEST12        4
#define TESTPEAKVELOCITYTEST12    10
#define TESTPARTCONTACTCOUNTER12  0
#define TESTCURRDIFFFORCE12       3
#define TESTPCINDEX12             6

// Testing values 13
#define TESTAVGFORCETENSAMPLES13  957
#define TESTPREVFORCE13           973
#define TESTPREVDIFFFORCE13       4
#define TESTQEIPOSITION13         963470
#define TESTVELOCITYTEST13        12
#define TESTPEAKVELOCITYTEST13    10
#define TESTPARTCONTACTCOUNTER13  0
#define TESTCURRDIFFFORCE13       3
#define TESTPCINDEX13             3

#define ADC0VOLT_IW              0
#define ADC0VOLT_200X            15

enum PARTCONTACT_STATUS
{
    NOTDETECTED,
    DETECTED
};

/* Function Prototypes */
void PartContactWithTriggerSwitch(void);
void AverageForceandDistanceReading(void);
void CalculateSpeed(void);
void PartContact(void);
tSINT32 PCDummy4(void);

enum TriggerSwitch
{
   TRSON,
   TRSOFF
};

#endif
