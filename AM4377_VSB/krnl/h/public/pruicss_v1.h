/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** ============================================================================
 *  @file       pruicss_v1.h
 *
 *  @brief      PRUICSS driver implementation for a am57x PRUICSS controller
 *
 *
 *  ============================================================================
 */

#ifndef ti_drivers_pruss_PRUICSSv1_include
#define ti_drivers_pruss_PRUICSSv1_include

#include <pruicss.h>

#ifdef __cplusplus
extern "C" {
#endif


/*!
 *  @brief      PRUICSS Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct PRUICSS_V1_Object
{


	VXB_DEV_ID			pDev;         /* PRUICSS write semaphore*/
    void*               pruBinBuff[2];         /* Buffer data pointer */
    unsigned int        buffLen[2];
    PRUICSS_IrqFunMap   pruEvntOutFnMapArray[PRUICSS_MAX_WAIT_EVENTS];

 }PRUICSS_V1_Object;

/*!
 *  @brief      PRUICSS hardware attributes
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct PRUICSS_HWAttrs {
    /*PRUICSS Peripheral's base address */
    unsigned int      baseAddr;
    
    unsigned int      version;

} PRUICSS_HwAttrs;



typedef enum PRUICSS_MaxInstances
{
   PRUICCSS_INSTANCE_ONE=1,
   PRUICCSS_INSTANCE_TWO=2
}PRUICSS_MaxInstances;

typedef enum PRUSS_PruCores
{
	PRUICCSS_PRU0,
	PRUICCSS_PRU1,
	PRUICSS_MAX_PRU
}PRUSS_PruCores;


#define PRU_ICSS_DATARAM(n)        		(0x00000 + ((n) * 0x02000))
#define PRU_ICSS_SHARED_RAM		        0x10000
#define PRU_ICSS_IRAM(n) 		        (0x34000 + ((n) * 0x04000))


//***********************************************************************
//  Macros representing Interrupt-Channel-Host sizes
//***********************************************************************/

//***********************************************************************
//  Macros to support dynamic chip detecting feature
//***********************************************************************/


#ifdef __cplusplus
}
#endif

#endif
