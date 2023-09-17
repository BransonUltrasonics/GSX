// $Header:   D:/databases/VMdb/archives/EN13849/FlashTest.c_v   1.12   09 Apr 2015 17:08:04   ewomack  $
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

This is the module for the Flash Test Diagnostics.

Module name: FlashTest

Filename:    FlashTest.c

--------------------------- TECHNICAL NOTES -------------------------------

This code Calculate the CRC for the Flash contents and compare that with the stored one.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/FlashTest.c_v  $
 * 
 *    Rev 1.12   09 Apr 2015 17:08:04   ewomack
 * Split FlashTest into 4 parts so it would consume less than a millisecond.
 * 
 *    Rev 1.11   20 Sep 2011 17:23:46   ASharma6
 * CRC of the executable is printed out on hyperterminal when program boots up.
 * 
 *    Rev 1.10   20 Jul 2011 16:56:52   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.9   05 Jul 2011 10:59:30   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.6   22 Jun 2011 18:09:18   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   31 Mar 2011 12:11:12   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:04:50   ygupta
 * Initial Version with File and Function Headers. 
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "TestMacros.h"
#include <stdio.h>
#include "Global.h"
#include "ProgramCounterTest.h"

#define WIDTH  (8 * sizeof(unsigned int))
#define TOPBIT (1 << (WIDTH - 1))
#define Startaddress 0x00000000UL
#define TESTLEN   9
#define CRCTABLELEN 256

tUINT32 FlashTestFault, flashTest, genSoftSig;
tUINT32 StoredFlashCRC;
tUINT32 FlashCRCRemainder;//get updated while the flash test is run four parts
tUINT32 FlashMemOffset;//get updated while the flash test is run four parts

//CRC Table.
const tUINT32 crcTable[CRCTABLELEN] = { 0x00000000, 0x04C11DB7, 0x09823B6E,
        0x0D4326D9, 0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005, 0x2608EDB8,
        0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 0x350C9B64, 0x31CD86D3, 0x3C8EA00A,
        0x384FBDBD, 0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9, 0x5F15ADAC,
        0x5BD4B01B, 0x569796C2, 0x52568B75, 0x6A1936C8, 0x6ED82B7F, 0x639B0DA6,
        0x675A1011, 0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD, 0x9823B6E0,
        0x9CE2AB57, 0x91A18D8E, 0x95609039, 0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52,
        0x8664E6E5, 0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81, 0xAD2F2D84,
        0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D, 0xD4326D90, 0xD0F37027, 0xDDB056FE,
        0xD9714B49, 0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95, 0xF23A8028,
        0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1, 0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A,
        0xEC7DD02D, 0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE, 0x278206AB,
        0x23431B1C, 0x2E003DC5, 0x2AC12072, 0x128E9DCF, 0x164F8078, 0x1B0CA6A1,
        0x1FCDBB16, 0x18AEB13, 0x54BF6A4, 0x808D07D, 0xCC9CDCA, 0x7897AB07,
        0x7C56B6B0, 0x71159069, 0x75D48DDE, 0x6B93DDDB, 0x6F52C06C, 0x6211E6B5,
        0x66D0FB02, 0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066, 0x4D9B3063,
        0x495A2DD4, 0x44190B0D, 0x40D816BA, 0xACA5C697, 0xA864DB20, 0xA527FDF9,
        0xA1E6E04E, 0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692, 0x8AAD2B2F,
        0x8E6C3698, 0x832F1041, 0x87EE0DF6, 0x99A95DF3, 0x9D684044, 0x902B669D,
        0x94EA7B2A, 0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E, 0xF3B06B3B,
        0xF771768C, 0xFA325055, 0xFEF34DE2, 0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31,
        0xCBFFD686, 0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A, 0x690CE0EE,
        0x6DCDFD59, 0x608EDB80, 0x644FC637, 0x7A089632, 0x7EC98B85, 0x738AAD5C,
        0x774BB0EB, 0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F, 0x5C007B8A,
        0x58C1663D, 0x558240E4, 0x51435D53, 0x251D3B9E, 0x21DC2629, 0x2C9F00F0,
        0x285E1D47, 0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B, 0x315D626,
        0x7D4CB91, 0xA97ED48, 0xE56F0FF, 0x1011A0FA, 0x14D0BD4D, 0x19939B94,
        0x1D528623, 0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2,
        0xE6EA3D65, 0xEBA91BBC, 0xEF68060B, 0xD727BBB6, 0xD3E6A601, 0xDEA580D8,
        0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3, 0xBD3E8D7E,
        0xB9FF90C9, 0xB4BCB610, 0xB07DABA7, 0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC,
        0xA379DD7B, 0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F, 0x8832161A,
        0x8CF30BAD, 0x81B02D74, 0x857130C3, 0x5D8A9099, 0x594B8D2E, 0x5408ABF7,
        0x50C9B640, 0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C, 0x7B827D21,
        0x7F436096, 0x7200464F, 0x76C15BF8, 0x68860BFD, 0x6C47164A, 0x61043093,
        0x65C52D24, 0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30, 0x29F3D35,
        0x65E2082, 0xB1D065B, 0xFDC1BEC, 0x3793A651, 0x3352BBE6, 0x3E119D3F,
        0x3AD08088, 0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654, 0xC5A92679,
        0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0, 0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB,
        0xDBEE767C, 0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18, 0xF0A5BD1D,
        0xF464A0AA, 0xF9278673, 0xFDE69BC4, 0x89B8FD09, 0x8D79E0BE, 0x803AC667,
        0x84FBDBD0, 0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C, 0xAFB010B1,
        0xAB710D06, 0xA6322BDF, 0xA2F33668, 0xBCB4666D, 0xB8757BDA, 0xB5365D03,
        0xB1F740B4 };

/**************************************************************************//**
* \brief -		EndFlashLocation
*
* \brief -		Calculate the Flash Address up to which data is stored.
*
* \param -		none
* 
* \return -	k: End of data address.
*
*****************************************************************************/
tUINT32 EndFlashLocation(void)
{
    extern tUINT32 __exidx_start, _edata;
    tUINT32 i, j, k= 0x10000000;

        i = (tUINT32)&__exidx_start;
        j = (tUINT32)&_edata;
        k = i+j-k;
        return k;
}

/**************************************************************************//**
* \brief -       	GenerateSoftwareSignature
*
* \brief -        	Generate the CRC for the passed data for passed length with reminder.
*
* \param -			message: Data for which CRC needs to be calculated.
*					nBytes: Length for which CRC needs to be calculated.
* 					remainder: Last Reminder.
*                      
* \return -      reminder1: Resultant CRC.
*
*****************************************************************************/
tUINT32 GenerateSoftwareSignature(tUINT8 const message[],
        tUINT32 nBytes, tUINT32 remainder)
{
    tUINT8 data;
    tUINT32 remainder1 = remainder;
    tUINT32 byte;

    FUNCTION1(genSoftSig, "GenerateSoftwareSignature()");
    CHECKPOINT(genSoftSig,BRANCH_1);
    /*
     * Divide the message by the polynomial, a byte at a time.
     */
    for (byte = 0; byte < nBytes; ++byte)
    {
        CHECKPOINT(genSoftSig,BRANCH_2);
        data = message[byte] ^ (remainder1 >> (WIDTH - 8));
        remainder1 = crcTable[data] ^ (remainder1 << 8);
    }

    FUNCTION(genSoftSig, "\nRemainder1%lX\n",remainder1);
    CHECKPOINT(genSoftSig,BRANCH_3);
    /*
     * The final remainder is the CRC.
     */
    return (remainder1);
}

/**************************************************************************//**
* \brief -		FlashTest1
*
* \brief -		Calculate 1/4 of the Flash CRC
*
* \param -		none
* 
* \return -		Result Pass
*
*****************************************************************************/
tUINT32 FlashTest1(void)
{
	tUINT32 ResultantCRC = 0, size2;
	tUINT32 size;
	tUINT32 Result = PASS;
	FUNCTION1(flashTest, "\nFlashTest1()");
	CHECKPOINT(flashTest,BRANCH_1);
	size = (tUINT32 ) EndFlashLocation();
	size2 = (tUINT32) (size - 1);
	FlashMemOffset = 0;
	FlashCRCRemainder = 0;
	if (!FlashTestFault && !genSoftSig) {
		CHECKPOINT(flashTest,BRANCH_2);
		size = size / 4;
		ResultantCRC = GenerateSoftwareSignature((tUINT8 *) Startaddress + FlashMemOffset, (tUINT32) size, FlashCRCRemainder);
		FlashMemOffset += size;
	}
	else if (!genSoftSig) {
		CHECKPOINT(flashTest,BRANCH_3);
		size2 = size2 / 4;
		ResultantCRC = GenerateSoftwareSignature((tUINT8 *) Startaddress + FlashMemOffset, size2, FlashCRCRemainder);
		FlashMemOffset += size2;
	}
	FlashCRCRemainder = ResultantCRC;
	CHECKPOINT(flashTest,BRANCH_4);
	return (Result);
}

/**************************************************************************//**
* \brief -		FlashTest2
*
* \brief -		Calculate 1/4 of the Flash CRC
*
* \param -		none
* 
* \return -	Result Pass
*
*****************************************************************************/
tUINT32 FlashTest2(void)
{
	tUINT32 ResultantCRC = 0, size2;
	tUINT32 size;
	tUINT32 Result = PASS;
	FUNCTION1(flashTest, "\nFlashTest2()");
	CHECKPOINT(flashTest,BRANCH_5);
	size = (tUINT32 ) EndFlashLocation();
	size2 = (tUINT32) (size - 1);
	if (!FlashTestFault && !genSoftSig) {
		CHECKPOINT(flashTest,BRANCH_6);
		size = size / 4;
		ResultantCRC = GenerateSoftwareSignature((tUINT8 *) Startaddress + FlashMemOffset, (tUINT32) size, FlashCRCRemainder);
		FlashMemOffset += size;
	}
	else if (!genSoftSig) {
		CHECKPOINT(flashTest,BRANCH_7);
		size2 = size2 / 4;
		ResultantCRC = GenerateSoftwareSignature((tUINT8 *) Startaddress + FlashMemOffset, size2, FlashCRCRemainder);
		FlashMemOffset += size2;
	}
	FlashCRCRemainder = ResultantCRC;
	CHECKPOINT(flashTest,BRANCH_8);
	return (Result);
}

/**************************************************************************//**
* \brief -		FlashTest3
*
* \brief -		Calculate 1/4 of the Flash CRC
*
* \param -		none
* 
* \return -	Result Pass
*
*****************************************************************************/
tUINT32 FlashTest3(void)
{
	tUINT32 ResultantCRC = 0, size2;
	tUINT32 size;
	tUINT32 Result = PASS;
	FUNCTION1(flashTest, "\nFlashTest3()");
	CHECKPOINT(flashTest,BRANCH_9);
	size = (tUINT32 ) EndFlashLocation();
	size2 = (tUINT32) (size - 1);
	if (!FlashTestFault && !genSoftSig) {
		CHECKPOINT(flashTest,BRANCH_10);
		size = size / 4;
		ResultantCRC = GenerateSoftwareSignature((tUINT8 *) Startaddress + FlashMemOffset, (tUINT32) size, FlashCRCRemainder);
		FlashMemOffset += size;
	}
	else if (!genSoftSig) {
		CHECKPOINT(flashTest,BRANCH_11);
		size2 = size2 / 4;
		ResultantCRC = GenerateSoftwareSignature((tUINT8 *) Startaddress + FlashMemOffset, size2, FlashCRCRemainder);
		FlashMemOffset += size2;
	}
	FlashCRCRemainder = ResultantCRC;
	CHECKPOINT(flashTest,BRANCH_12);
	return (Result);
}

/**************************************************************************//**
* \brief -		FlashTest4
*
* \brief -		Calculate the Flash CRC and compare it with the stored one.
*
* \param -       none
* 
* \return -      Result Pass/Fail.
*
*****************************************************************************/
tUINT32 FlashTest4(void)
{
	tUINT32 ResultantCRC = 0, size2;
	tUINT32 size, *SavedCrc;
	tUINT8 *data = (tUINT8 *) "123456789";
	tUINT32 Result = PASS;

	FUNCTION1(flashTest, "\nFlashTest4()");
	CHECKPOINT(flashTest,BRANCH_13);
	size = (tUINT32 ) EndFlashLocation();
	SavedCrc = (tUINT32 *)size;
	size2 = (tUINT32) (size - 1);
	if (!FlashTestFault && !genSoftSig) {
		CHECKPOINT(flashTest,BRANCH_14);
		ResultantCRC = GenerateSoftwareSignature((tUINT8 *) Startaddress + FlashMemOffset, size - FlashMemOffset, FlashCRCRemainder);
	}
	else if (!genSoftSig) {
		CHECKPOINT(flashTest,BRANCH_15);
		ResultantCRC = GenerateSoftwareSignature((tUINT8 *) Startaddress + FlashMemOffset, size2 - FlashMemOffset, FlashCRCRemainder);
	}

	if (genSoftSig) {
		CHECKPOINT(flashTest,BRANCH_16);
		ResultantCRC = GenerateSoftwareSignature(data, TESTLEN, 0);
	}
	StoredFlashCRC = *SavedCrc;
	CHECKPOINT(flashTest,BRANCH_17);
	if (ResultantCRC != StoredFlashCRC) {
		CHECKPOINT(flashTest,BRANCH_18);
		Result = FAIL;
	}
	else {
		CHECKPOINT(flashTest,BRANCH_19);
	}
	return (Result);
}

#if UNIT_TESTING

#include <stdio.h>
/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for Flash test unit testing.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void ResetHandler(void)
{
    tSINT32 result;

    DiagInit();
    FlashTestFault = 0;
    flashTest = 1;
    genSoftSig = 0;
    FlashTest1();
    FlashTest2();
    FlashTest3();
    result = FlashTest4();
    DiagPrint(result, PASS);
    FlashTestFault = 1;
    FlashTest1();
    FlashTest2();
    FlashTest3();
    result = FlashTest4();
    DiagPrint(result, FAIL);
    FlashTestFault = 0;
    flashTest = 0;
    genSoftSig = 1;
    FlashTest1();
    FlashTest2();
    FlashTest3();
    result = FlashTest4();
    DiagPrint(result, FAIL);
    DiagSummary();
    return;
}
#endif
#if 0
void BISTFlashTest()
{
    static unsigned int FlastStartAddress = IEC60335_FLASH_START;
    static unsigned int FlastEndAddress = (IEC60335_FLASH_START + FLASH_SIZE);//(unsigned char*)0x0000FFFB;
    static unsigned int ResultantCRC;

    if(FlastStartAddress == IEC60335_FLASH_START)
    {
        ResultantCRC = 0;
        printf("\n BIST Flash test starts");
        ResultantCRC = GenerateSoftwareSignature((unsigned char *)FlastStartAddress,16,ResultantCRC);
        FlastStartAddress +=16;
    }
    else
    {
        if(FlastStartAddress + 16 == FlastEndAddress)
        {
            ResultantCRC = GenerateSoftwareSignature((unsigned char *)FlastStartAddress,12,ResultantCRC);
            if(ResultantCRC == StoredFlashCRC)
            {
                printf("\n BIST flash test passed,ResultantCRC = %x , StoredFlashCRC = %x",ResultantCRC,StoredFlashCRC);
                FlastStartAddress = IEC60335_FLASH_START;
            }
            else
            {
                printf("\n BIST  flash test failed, ResultantCRC = %x , StoredFlashCRC = %x",ResultantCRC,StoredFlashCRC);
                while (1)
                ;
            }
        }
        else
        {
            //          printf(".");
            ResultantCRC = GenerateSoftwareSignature((unsigned char *)FlastStartAddress,16,ResultantCRC);
            FlastStartAddress +=16;
        }

    }
}
#endif

/**************************************************************************//**
* \brief -		PCDummy2
*
* \brief -		Dummy function for ProgramCounter test.
*
* \param -		void
* 
* \return -	Function2_Return(needs to be a known value)
*
*****************************************************************************/
tSINT32 PCDummy2 (void)
{
    return Function2_Return;
}

