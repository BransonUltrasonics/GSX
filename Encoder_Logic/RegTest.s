/*
 $Header:   D:/databases/VMdb/archives/EN13849/RegTest.s_v   1.8   20 Jul 2011 16:58:42   ASharma6  $
 */
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

This is the module for CPU Register Test.

Module name: RegTest

Filename:    RegTest.s

--------------------------- TECHNICAL NOTES -------------------------------

This code Handles the CPU Register Test.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/RegTest.s_v  $
; 
;    Rev 1.8   20 Jul 2011 16:58:42   ASharma6
; Modifications for review comments
; 
;    Rev 1.7   05 Jul 2011 11:00:54   ASharma6
; ULS, CM changes
; 
;    Rev 1.4   22 Jun 2011 18:17:40   ASharma6
; LE Fault reset allowed in horn up state
; Part contact lost made more resilient
; Cross monitoring detects other processor was reset and resets itself
; SBeam diagnostics is more resilient
; 
;    Rev 1.2   01 Apr 2011 11:13:42   PDwivedi
; Fixes after code review.
; 
;    Rev 1.1   25 Mar 2011 09:08:10   ygupta
; Initial Version with File and Function Headers. 
-------------------------------------------------------------------------*/

@ This file is included from inc folder if we are building the application or
@ from uut folder if we are compiling unit test code.
		.include "TestMacros.inc"
		.include "Macros.inc"

		.thumb
		.global RegTest
		.thumb_func
		@ r0 register test:
		@ Since r0 is the first register to be tested
		@ and no other registers may be used, r0 should be tested
		@ by only using immediate instructions, 8 bits at a time
		@ R0[7:0]
RegTest:
		push	{lr}
		InitDiagnostics
		UnitTest RegTest
		Checkpoint 1
;	   /* Since cmp can only compare with an immediate
;	   * 8-bit value, the bits to be tested needs to
;	   * be shifted to the LSB's.
;	   */
;	/* r0 - stuck at 0 test */
		movw	r0,#0xAA
;	/* r0 - test r0[7:0] */
		cmp		r0,#0xAA
		FaultNE 1
		CheckpointNE 2
		bnel	Reg0TestFail
		Checkpoint 3
;	/* r0 - test r0[15:8] */
		movw	r0,0xAA00
		lsr		r0,r0,#8
		cmp		r0,#0xAA
		FaultNE 2
		CheckpointNE 4
		bnel	Reg0TestFail
		Checkpoint 5
;	/* r0 - test r0[23:16] */
		movw	r0,#0x0000
		movt	r0,#0x00AA
		lsr		r0,r0,#16
		cmp 	r0,#0xAA
		FaultNE 3
		CheckpointNE 6
		bnel	Reg0TestFail
		Checkpoint 7
;	/* r0 - test r0[31:24] */
		movw	r0,#0x0000
		movt	r0,#0xAA00
		lsr		r0,r0,#24
		cmp 	r0,#0xAA
		FaultNE 4
		CheckpointNE 8
		bnel	Reg0TestFail
		Checkpoint 9
		@ And once more, with 55 as a pattern
		@ R0[7:0]
;	/* r0 - inverted pattern and
;	 * neighbour stuck test
;	 */
		movw	r0,#0x55
;	/* r0 - test r0[7:0] */
		cmp		r0,#0x55
		FaultNE 5
		CheckpointNE 10
		bnel	Reg0TestFail
		Checkpoint 11
;	/* r0 - test r0[15:8] */
		movw	r0,0x5500
		lsr		r0,r0,#8
		cmp		r0,#0x55
		FaultNE 6
		CheckpointNE 12
		bnel	Reg0TestFail
		Checkpoint 13
		@ R0[23:16]
;	/* r0 - test r0[23:16] */
		movw	r0,#0x0000
		movt	r0,#0x0055
		lsr		r0,r0,#16
		cmp 	r0,#0x55
		FaultNE 7
		CheckpointNE 14
		bnel	Reg0TestFail
		Checkpoint 15
		@ R0[31:24]
;	/* r0 - test r0[31:24] */
		movw	r0,#0x0000
		movt	r0,#0x5500
		lsr		r0,r0,#24
		cmp 	r0,#0x55
		FaultNE 8
		CheckpointNE 16
		bnel	Reg0TestFail
		Checkpoint 17
		ldr		r0,=1
		pop		{pc}
Reg0TestFail:
		ReturnFailed
		b	.

		.if UnitTesting

UnitTestRegTest:
		@ Test 0 - pass
		RunTest Test0, 0, TestPassOK, TestFailed, UTRegTest
		@ Test 1 - fail
		RunTest Test1, 1, TestPassed, TestFailOK, UTRegTest
		@ Test 2 - fail
		RunTest Test2, 2, TestPassed, TestFailOK, UTRegTest
		@ Test 3 - fail
		RunTest Test3, 3, TestPassed, TestFailOK, UTRegTest
		@ Test 4 - fail
		RunTest Test4, 4, TestPassed, TestFailOK, UTRegTest
		@ Test 5 - fail
		RunTest Test5, 5, TestPassed, TestFailOK, UTRegTest
		@ Test 6 - fail
		RunTest Test6, 6, TestPassed, TestFailOK, UTRegTest
		@ Test 7 - fail
		RunTest Test7, 7, TestPassed, TestFailOK, UTRegTest
		@ Test 8 - fail
		RunTest Test8, 8, TestPassed, TestFailOK, UTRegTest
		@ldr		r0,=TestDone
		movw r0, #:lower16:TestDone
   		movt r0, #:upper16:TestDone
		bl		SerialOut
		pop		{pc}

		.endif

		.global Reg1Test
		.thumb_func

		@ r1 register test:
		@ Since r1 is the first register to be tested
		@ and no other registers may be used, r1 should be tested
		@ by only using immediate instructions, 8 bits at a time
		@ r1[7:0]
Reg1Test:
		push	{lr}
		UnitTest Reg1Test
		Checkpoint 1
;	   /* Since cmp can only compare with an immediate
;	   * 8-bit value, the bits to be tested needs to
;	   * be shifted to the LSB's.
;	   */
;	   /* r1 - stuck at 0 test */
		movw	r1,#0xAA
;	/* r1 - test r1[7:0] */
		cmp		r1,#0xAA
		FaultNE 1
		CheckpointNE 2
		bnel	Reg1TestFail
		Checkpoint 3
		@ r1[15:8]
;	/* r1 - test r1[15:8] */
		movw	r1,0xAA00
		lsr		r1,r1,#8
		cmp		r1,#0xAA
		FaultNE 2
		CheckpointNE 4
		bnel	Reg1TestFail
		Checkpoint 5
		@ r1[23:16]
;	/* r1 - test r1[23:16] */
		movw	r1,#0x0000
		movt	r1,#0x00AA
		lsr		r1,r1,#16
		cmp 	r1,#0xAA
		FaultNE 3
		CheckpointNE 6
		bnel	Reg1TestFail
		Checkpoint 7
		@ r1[31:24]
;	/* r1 - test r1[31:24] */
		movw	r1,#0x0000
		movt	r1,#0xAA00
		lsr		r1,r1,#24
		cmp 	r1,#0xAA
		FaultNE 4
		CheckpointNE 8
		bnel	Reg1TestFail
		Checkpoint 9
		@ And once more, with 55 as a pattern
		@ r1[7:0]
;	/* r1 - inverted pattern and
;	 * neighbour stuck test
;	 */
		movw	r1,#0x55
;	/* r1 - test r1[7:0] */
		cmp		r1,#0x55
		FaultNE 5
		CheckpointNE 10
		bnel	Reg1TestFail
		Checkpoint 11
		@ r1[15:8]
;	/* r1 - test r1[15:8] */
		movw	r1,0x5500
		lsr		r1,r1,#8
		cmp		r1,#0x55
		FaultNE 6
		CheckpointNE 12
		bnel	Reg1TestFail
		Checkpoint 13
		@ r1[23:16]
;	/* r1 - test r1[23:16] */
		movw	r1,#0x0000
		movt	r1,#0x0055
		lsr		r1,r1,#16
		cmp 	r1,#0x55
		FaultNE 7
		CheckpointNE 14
		bnel	Reg1TestFail
		Checkpoint 15
		@ r1[31:24]
;	/* r1 - test r1[31:24] */
		movw	r1,#0x0000
		movt	r1,#0x5500
		lsr		r1,r1,#24
		cmp 	r1,#0x55
		FaultNE 8
		CheckpointNE 16
		bnel	Reg1TestFail
		Checkpoint 17
		ldr		r0,=1
		pop		{pc}
Reg1TestFail:
		ReturnFailed
		b	.

		.if UnitTesting

UnitTestReg1Test:
@ Test 0 - pass
		RunTest TestR1_0, 0, TestPassOK, TestFailed, UTReg1Test
		@ Test 1 - fail
		RunTest TestR1_1, 1, TestPassed, TestFailOK, UTReg1Test
		@ Test 2 - fail
		RunTest TestR1_2, 2, TestPassed, TestFailOK, UTReg1Test
		@ Test 3 - fail
		RunTest TestR1_3, 3, TestPassed, TestFailOK, UTReg1Test
		@ Test 4 - fail
		RunTest TestR1_4, 4, TestPassed, TestFailOK, UTReg1Test
		@ Test 5 - fail
		RunTest TestR1_5, 5, TestPassed, TestFailOK, UTReg1Test
		@ Test 6 - fail
		RunTest TestR1_6, 6, TestPassed, TestFailOK, UTReg1Test
		@ Test 7 - fail
		RunTest TestR1_7, 7, TestPassed, TestFailOK, UTReg1Test
		@ Test 8 - fail
		RunTest TestR1_8, 8, TestPassed, TestFailOK, UTReg1Test
		@ldr		r0,=TestDone1
		movw r0, #:lower16:TestDone1
   		movt r0, #:upper16:TestDone1
		bl		SerialOut
		pop		{pc}

		.endif

		.global BASEPRIREGTest
		.thumb_func

;		/* BASEPRI register tests:
; 		* The BASEPRI bit will be tested to 0 and 1.
; 		*/
BASEPRIREGTest:
		push	{lr}
		UnitTest	 BASEPRIReg
;	    /* Store the current link register */
		mrs			 r3,BASEPRI
		@ldr		 r1, 0x00000050
;	   /* Load pattern = 0x000000A8 in r1 */
		movw		 r1,#0x00A8
		movt		 r1,#0x0000
		Checkpoint 	 1
;	   /* Move pattern = 0x000000A8 in the BASEPRI */
		msr			 BASEPRI,r1
;	   /* Read pattern = 0x000000A8 from the BASEPRI */
		mrs			 r2,BASEPRI
		Checkpoint 	 2
;	    /* Compare the pattern with the input pattern */
		cmp 		 r1,r2
		FaultNE 	 1
		CheckpointNE 3
		bnel		 BASEPRITestFail
		Checkpoint 	 4
;	    /* Load pattern = 0x00000050  in r1 */
		movw		 r1,#0x0050
		movt		 r1,#0x0000
;	   /* Move pattern = 0x00000050 in the BASEPRI */
		msr			 BASEPRI,r1
;	   /* Read pattern = 0x00000050 from the BASEPRI */
		mrs			 r2,BASEPRI
		Checkpoint 	 5
;	   /* Compare the pattern with the input pattern */
		cmp			 r1,r2
		FaultNE 	 2
		CheckpointNE 6
		bnel		 BASEPRITestFail
		Checkpoint   7
;	    /* Restore the BASEPRI */
		msr			 BASEPRI,r3
		ldr			 r0,=1
		pop			 {pc}
BASEPRITestFail:
		msr		BASEPRI,r3
		ReturnFailed
		b			 .

		.if UnitTesting

UnitTestBASEPRIReg:
		RunTest BasePriTest0,0,TestPassOK,TestFailed,UTBASEPRIReg
		RunTest BasePriTest1,1,TestPassed, TestFailOK,UTBASEPRIReg
		RunTest BasePriTest2,2,TestPassed, TestFailOK,UTBASEPRIReg
		@ldr		r0,=BasePriTestDone
		movw r0, #:lower16:BasePriTestDone
   		movt r0, #:upper16:BasePriTestDone
		bl		SerialOut
		pop		{pc}

		.endif

		.global APSRRegTest
		.thumb_func

;/* APSR register tests:
; * Only the APSR register can be test, the other PSR registers,
; * IPSR and EPSR are read-only.
; * The APSR register will be written with 0xA8000000,
; * because only APSR[31:27] are readable and writeable,
; * then compared for a pass/fail. Secondly the inverse
; * of the pattern will be tested.
; */
APSRRegTest:
		push	{lr}
		@Checkpoint  20
		UnitTest 	APSRReg
;	    /* Store the current link register */
		mrs		 	r3,APSR
;	    /* Load pattern = 0xA8000000 in r1 */
		movw		r1,#0x0000
		movt		r1,#0xA800
		Checkpoint  1
;	    /* Move pattern in the APSR */
		msr			APSR,r1
;	    /* Read pattern from the APSR */
		mrs			r2,APSR
		Checkpoint  2
;	    /* Compare the pattern with the input pattern */
		cmp			r1,r2
		FaultNE 	 1
		CheckpointNE 3
		bnel		 APSRRegTestFail
		Checkpoint 	 4
;	    /* Load inverse pattern = 0x50000000 in r1 */
		movw		 r1,#0x0000
		movt		 r1,#0x5000
;	    /* Move inverse pattern in the APSR */
		msr			 APSR,r1
;	    /* Read inverse pattern from the APSR */
		mrs			 r2,APSR
		Checkpoint 	 5
;	    /* Compare the pattern with the input pattern */
		cmp			 r1,r2
		FaultNE 	 2
		CheckpointNE 6
		bnel		 APSRRegTestFail
		Checkpoint   7
;	    /* Restore the APSR */
		msr			 APSR,r3
		ldr			 r0,=1
		pop			 {pc}
APSRRegTestFail:
		msr			 APSR,r3
		ReturnFailed
		b			 .

		.if UnitTesting

UnitTestAPSRReg:
		RunTest APSRTest0,0,TestPassOK,TestFailed,UTAPSRReg
		RunTest APSRTest1,1,TestPassOK,TestFailOK,UTAPSRReg
		RunTest APSRTest2,2,TestPassOK,TestFailOK,UTAPSRReg
		movw r0, #:lower16:APSRTestDone
   		movt r0, #:upper16:APSRTestDone
		bl		SerialOut
		pop		{pc}

		.endif

		.global MSPRegTest
		.thumb_func
;		/* MSP register tests:
; 		* The MSP consists of two tests, the first test the register
; 		* through reading/writing r13 using the normal instructions.
; 		* The second test test the register by using the MSR and MRS
; 		* special register read/write instructions.
; 		* For the MSP a different pattern is used because MSP[1:0] are
; 		* always zero.
; 		*/
MSPRegTest:
		push 		 {lr}
		UnitTest 	 MSPReg
		@save Control Reg
;	    /* Store the current CONTROL value */
		mrs 		 r3,CONTROL
;	    /* Select default stack (MSP) */
		mov 		 r6,#0x00
		msr 		 CONTROL,r6
		Checkpoint   1
		@Check if r13=MSR
;	    /* Store the current MSP */
		mov 		 r4,r13
;	    /* Check whether MSP is really selected */
		mrs 		 r5,MSP
		Checkpoint   2
		cmp 		 r4,r5
		FaultNE 	 1
		CheckpointNE 3
		bnel 		 MSPRegTestFail
		Checkpoint   4
		@insert AAAAAAA8 into r13 and verify
;	    /* Load pattern = AAAAAAA8 in r0 */
		movw 		 r0,#0xAAA8
		movt 		 r0,#0xAAAA
;	   /* Move pattern in the MSP */
		mov 		 r13,r0
;	   /* Compare the pattern with the input pattern */
		cmp 		 r13,r0
		mov			 r13,r4
		FaultNE 	 2
		CheckpointNE 5
		bnel 		 MSPRegTestFail
		Checkpoint   6
		@insert 55555554 into r13 and verify
;	    /* Load pattern = 0x55555554 in r0 */
		movw 		 r0,#0x5554
		movt 		 r0,#0x5555
;	    /* Move pattern in the MSP */
		mov 		 r13,r0
;	    /* Compare the pattern with the input pattern*/
		cmp 		 r13,r0
		mov			 r13,r4
		FaultNE 	 3
		CheckpointNE 7
		bnel 		 MSPRegTestFail
		Checkpoint   8
		@insert AAAAAA8 into MSP and verify
;	    /* Load pattern = AAAAAA8 in r0 */
		movw 		 r0,#0xAAA8
		movt 		 r0,#0xAAAA
;	    /* Move pattern in the MSP */
		msr  		 MSP,r0
;	    /* Copy MSP to r1, since MSP can't be directly cmp'ed */
		mrs  		 r1, MSP
		mov			 r13,r4
		Checkpoint   9
;	    /* Compare the pattern with the input pattern */
		cmp 		 r1,r0
		FaultNE 	 4
		CheckpointNE 10
		bnel 		 MSPRegTestFail
		Checkpoint   11
		@insert 55555554 into MSP and verify
;	    /* Load pattern = 55555554 in r0 */
		movw 		 r0,#0x5554
		movt 		 r0,#0x5555
;	    /* Move pattern in the MSP */
		msr  		 MSP,r0
;	    /* Copy MSP to r1, since MSP can't be directly cmp'ed */
		mrs  		 r1, MSP
		mov			 r13,r4
		Checkpoint   12
;	    /* Compare the pattern with the input pattern */
		cmp 		 r1,r0
		FaultNE 	 5
		CheckpointNE 13
		bnel 		 MSPRegTestFail
		Checkpoint   14
;	    /* Restore the MSP and CONTROL */
		mov 		 r13, r4
		msr			 CONTROL, r3
		ldr			 r0,=1
		pop			 {pc}
MSPRegTestFail:
;	   /* Restore the MSP and CONTROL */
		mov 		r13, r4
		msr			CONTROL, r3
		ReturnFailed
		b  			.

@ Starting from here: Unit test code. Code calls register test 9 times, first
@ must pass, other 8 must fail.

		.if		UnitTesting

UnitTestMSPReg:
		RunTest 	MSPTest0,0,TestPassOK,TestFailed,UTMSPReg
		RunTest 	MSPTest1,1,TestPassed,TestFailOK,UTMSPReg
		RunTest 	MSPTest2,2,TestPassed,TestFailOK,UTMSPReg
		RunTest 	MSPTest3,3,TestPassed,TestFailOK,UTMSPReg
		RunTest	 	MSPTest4,4,TestPassed,TestFailOK,UTMSPReg
		RunTest 	MSPTest5,5,TestPassed,TestFailOK,UTMSPReg
		@ldr			r0,=MSPTestDone
		movw r0, 	#:lower16:MSPTestDone
   		movt r0, 	#:upper16:MSPTestDone

		bl			SerialOut
		pop			{pc}
		.endif

		.global PSPRegTest
		.thumb_func

;		/* PSP register tests:
; 		* The PSP consists of two tests, the first test the register
; 		* through reading/writing r13 using the normal instructions.
; 		* The second test test the register by using the MSR and MRS
; 		* special register read/write instructions.
; 		* For the PSP a different pattern is used because MSP[1:0] are
; 		* always zero.
; 		*/
PSPRegTest:
		push 		 {lr}
		UnitTest 	 PSPReg
		@save Control Reg
;		/* Store the current CONTROL value */
		mrs 		 r3,CONTROL
;	   /* Select alternitive stack (PSP) */
		mov 		 r6,#0x02
		Checkpoint   1
		msr 		 CONTROL,r6
		@Check if r13=MSR
;	    /* Store the current PSP */
		mov 		 r4,r13
;	    /* Check whether PSP is really selected */
		mrs 		 r5,PSP
		msr			 CONTROL,r3
		Checkpoint   2
		msr 		 CONTROL,r6
		cmp 		 r4,r5
		msr			 CONTROL,r3
		FaultNE 	 1
		CheckpointNE 3
		msr 		 CONTROL,r6
		bnel 		 PSPRegTestFail
		msr			 CONTROL,r3
		Checkpoint   4
		msr 		 CONTROL,r6
		@insert AAAAAAA8 into r13 and verify
;	   /* Load pattern = AAAAAAA8 in r0 */
		movw 		 r0,#0xAAA8
		movt 		 r0,#0xAAAA
;	   /* Move pattern in the PSP */
		mov 		 r13,r0
;	   /* Compare the pattern with the input pattern */
		cmp 		 r13,r0
		mov			 r13,r4
		msr			 CONTROL,r3
		FaultNE 	 2
		CheckpointNE 5
		msr 		 CONTROL,r6
		bnel 		 PSPRegTestFail
		msr			 CONTROL,r3
		Checkpoint   6
		msr 		 CONTROL,r6
		@insert 55555554 into r13 and verify
;	    /* Load pattern = 55555554 in r0 */
		movw 		 r0,#0x5554
		movt 		 r0,#0x5555
;	    /* Move pattern in the PSP */
		mov 		 r13,r0
;	   /* Compare the pattern with the input pattern */
		cmp 		 r13,r0
		mov			 r13,r4
		msr			 CONTROL,r3
		FaultNE 	 3
		CheckpointNE 7
		msr 		 CONTROL,r6
		bnel 		 PSPRegTestFail
		msr			 CONTROL,r3
		Checkpoint   8
		msr 		 CONTROL,r6
		@insert AAAAAA8 into PSP and verify
;	   /* Load pattern = AAAAAA8 in r0 */
		movw 		 r0,#0xAAA8
		movt 		 r0,#0xAAAA
;	   /* Move pattern in the PSP */
		msr  		 PSP,r0
;	   /* Copy PSP to r1, since PSP can't be directly cmp'ed */
		mrs  		 r1, PSP
		mov			 r13,r4
		msr 		 CONTROL,r3
		Checkpoint   9
		msr 		 CONTROL,r6
;	    /* Compare the pattern with the input pattern */
		cmp 		 r1,r0
		msr 		 CONTROL,r3
		FaultNE 	 4
		CheckpointNE 10
		msr 		 CONTROL,r6
		bnel 		 PSPRegTestFail
		msr 		 CONTROL,r3
		Checkpoint   11
		msr 		 CONTROL,r6
		@insert 55555554 into PSP and verify
;	    /* Load pattern = 55555554 in r0 */
		movw 		 r0,#0x5554
		movt 		 r0,#0x5555
;	    /* Move pattern in the PSP */
		msr  		 PSP,r0
;	   /* Copy PSP to r1, since PSP can't be directly cmp'ed */
		mrs  		 r1, PSP
		mov			 r13,r4
		msr 		 CONTROL,r3
		Checkpoint   12
		msr 		 CONTROL,r6
;	    /* Compare the pattern with the input pattern */
		cmp 		 r1,r0
		msr 		 CONTROL,r3
		FaultNE 	 5
		CheckpointNE 13
		msr 		 CONTROL,r6
		bnel 		 PSPRegTestFail
		msr 		 CONTROL,r3
		Checkpoint   14
		msr 		 CONTROL,r6
;	    /* Restore the PSP and CONTROL */
		mov 		 r13, r4
		msr			 CONTROL, r3
		ldr			 r0,=1
		pop			 {pc}
PSPRegTestFail:
;	   /* Restore the PSP and CONTROL */
		mov 		r13, r4
		msr			CONTROL, r3
		ReturnFailed
		b  			.

		.if		UnitTesting
UnitTestPSPReg:
		RunTest 	PSPTest0,0,TestPassOK,TestFailed,UTPSPReg
		RunTest 	PSPTest1,1,TestPassed,TestFailOK,UTPSPReg
		RunTest 	PSPTest2,2,TestPassed,TestFailOK,UTPSPReg
		RunTest 	PSPTest3,3,TestPassed,TestFailOK,UTPSPReg
		RunTest	 	PSPTest4,4,TestPassed,TestFailOK,UTPSPReg
		RunTest 	PSPTest5,5,TestPassed,TestFailOK,UTPSPReg
		@ldr			r0,=PSPTestDone
		movw r0, 	#:lower16:PSPTestDone
   		movt r0, 	#:upper16:PSPTestDone
		bl			SerialOut
		pop			{pc}
		.endif

		.global PRIMASKRegTest
		.thumb_func

;		/* PRIMASK register tests:
; 		* The PRIMASK bit will be tested to 0 and 1.
; 		*/
PRIMASKRegTest:
		push	{lr}
		@Checkpoint  20
		UnitTest 	PRIMASKReg
;		/* Store the current link register */
		mrs		 	r3,PRIMASK
;	    /* Load pattern = 0 in r0 */
		mov			r0,#0
;	    /* Move pattern in the PRIMASK */
		msr			PRIMASK,r0
;	    /* Read pattern from the PRIMASK */
		mrs			r1, PRIMASK
		Checkpoint  1
;	    /* Compare the pattern with the input pattern */
		cmp			r1, r0
		FaultNE 	 1
		CheckpointNE 2
		bnel		PRIMASKRegTestFail
		Checkpoint  3
;	    /* Load pattern = 1 in r0 */
		mov			r0, #1
;	   /* Move pattern in the PRIMASK */
		msr			PRIMASK, r0
;	   /* Read pattern from the PRIMASK */
		mrs			r1, PRIMASK
		Checkpoint  4
;	   /* Compare the pattern with the input pattern */
		cmp			r1, r0
		FaultNE 	 2
		CheckpointNE 5
		bnel		PRIMASKRegTestFail
		Checkpoint  6
;	    /* Restore the PRIMASK */
		msr 		PRIMASK, r3
		ldr			r0,=1
		pop			{pc}
PRIMASKRegTestFail:
;	    /* Restore the PRIMASK */
		msr			 PRIMASK,r3
		ReturnFailed
		b			 .

		.if		UnitTesting
UnitTestPRIMASKReg:
		RunTest 	PRIMASKTest0,0,TestPassOK,TestFailed,UTPRIMASKReg
		RunTest 	PRIMASKTest1,1,TestPassed,TestFailOK,UTPRIMASKReg
		RunTest 	PRIMASKTest2,2,TestPassed,TestFailOK,UTPRIMASKReg
		@ldr			r0,=PRIMASKTestDone
		movw r0, 	#:lower16:PRIMASKTestDone
   		movt r0, 	#:upper16:PRIMASKTestDone
		bl			SerialOut
		pop			{pc}
		.endif

		.global FAULTMASKRegTest
		.thumb_func

;		/* FAULTMASK register tests:
; 		* The FAULTMASK bit will be tested to 0 and 1.
; 		*/
FAULTMASKRegTest:
		push	{lr}
		@Checkpoint  20
		UnitTest 	FAULTMASKReg
;	    /* Store the current link register */
		mrs		 	r3,FAULTMASK
;	    /* Load pattern = 0 in r0 */
		mov			r0,#0
;	    /* Move pattern in the FAULTMASK */
		msr			FAULTMASK,r0
;	    /* Read pattern from the FAULTMASK */
		mrs			r1, FAULTMASK
		Checkpoint  1
;	    /* Compare the pattern with the input pattern */
		cmp			r1, r0
		FaultNE 	 1
		CheckpointNE 2
		bnel		FAULTMASKRegTestFail
		Checkpoint  3
;	    /* Load pattern = 1 in r0 */
		mov			r0, #1
;	    /* Move pattern in the FAULTMASK */
		msr			FAULTMASK, r0
;	    /* Read pattern from the FAULTMASK */
		mrs			r1, FAULTMASK
		Checkpoint  4
;	    /* Compare the pattern with the input pattern */
		cmp			r1, r0
		FaultNE 	 2
		CheckpointNE 5
		bnel		FAULTMASKRegTestFail
		Checkpoint  6
;	    /* Restore the FAULTMASK */
		msr 		FAULTMASK, r3
		ldr			r0,=1
		pop			{pc}
FAULTMASKRegTestFail:
;	    /* Restore the FAULTMASK */
		msr			 FAULTMASK,r3
		ReturnFailed
		b			 .

		.if		UnitTesting
UnitTestFAULTMASKReg:
		RunTest 	FAULTMASKTest0,0,TestPassOK,TestFailed,UTFAULTMASKReg
		RunTest 	FAULTMASKTest1,1,TestPassed,TestFailOK,UTFAULTMASKReg
		RunTest 	FAULTMASKTest2,2,TestPassed,TestFailOK,UTFAULTMASKReg
		@ldr			r0,=FAULTMASKTestDone
		movw r0, 	#:lower16:FAULTMASKTestDone
   		movt r0, 	#:upper16:FAULTMASKTestDone
		bl			SerialOut
		pop			{pc}
		.endif

		.global Reg2Test
		.thumb_func

;		/* Register r2 test:
; 		* The registers under test will be written with
; 		* pattern1 = 0xAAAA.AAAA. Each register will be individually
; 		* compared to r0 for a pass/fail for the test.
; 		* The second part of the test will write the inverted pattern
; 		* in the register under test and pass/fail with a cmp
; 		*/
Reg2Test:
		push	{lr}
		UnitTest 	 Reg2
		Checkpoint   1
;	    /*Load the required pattern = 0xAAAAAAAA into r0*/
		movw 		 r0,#0xAAAA
		movt 		 r0,#0xAAAA
		@Test the Register R2
		mov			 r2,r0
		Checkpoint   2
;	    /*Compare with input pattern*/
		cmp			 r2,r0
		FaultNE 	 1
		CheckpointNE 3
		bnel		 Reg2TestFail
		Checkpoint   4
		@Load the required inverse pattern into r0
;	    /*Load the required inverse pattern = 0x55555555 into r0*/
		movw 		 r0,#0x5555
		movt 		 r0,#0x5555
		@Test the Register R2
		mov			 r2,r0
		Checkpoint   5
;	    /*Compare with input pattern*/
		cmp			 r2,r0
		FaultNE 	 2
		CheckpointNE 6
		bnel		 Reg2TestFail
		Checkpoint   7
		ldr			 r0,=1
		pop			 {pc}
Reg2TestFail:
		ReturnFailed
		b			 .

		.if		UnitTesting
UnitTestReg2:
		RunTest 	REG2Test0,0,TestPassOK,TestFailed,UTReg2
		RunTest 	REG2Test1,1,TestPassed,TestFailOK,UTReg2
		RunTest 	REG2Test2,2,TestPassed,TestFailOK,UTReg2
		@ldr			r0,=REG2TestDone
		movw r0, 	#:lower16:REG2TestDone
   		movt r0, 	#:upper16:REG2TestDone
		bl			SerialOut
		pop			{pc}
		.endif

		.global Reg3Test
		.thumb_func
;		/* Register r3 test:
; 		* The registers under test will be written with
; 		* pattern1 = 0xAAAA.AAAA. Each register will be individually
; 		* compared to r0 for a pass/fail for the test.
; 		* The second part of the test will write the inverted pattern
; 		* in the register under test and pass/fail with a cmp
; 		*/
Reg3Test:
		@Load the required pattern into r0
		push	{lr}
		@Checkpoint  20
		UnitTest 	 Reg3
		Checkpoint	 1
;	    /*Load the required pattern = 0xAAAAAAAA into r0*/
		movw 		 r0,#0xAAAA
		movt 		 r0,#0xAAAA
		@Test the Register R3
		mov			 r3,r0
		Checkpoint   2
;	    /*Compare with input pattern*/
		cmp			 r3,r0
		FaultNE 	 1
		CheckpointNE 3
		bnel		 Reg3TestFail
		Checkpoint   4
		@Load the required inverse pattern into r0
;	    /*Load the required pattern = 0x55555555 into r0*/
		movw 		 r0,#0x5555
		movt 		 r0,#0x5555
		@Test the Registers R3
		mov			 r3,r0
		Checkpoint   5
;	    /*Compare with input pattern*/
		cmp			 r3,r0
		FaultNE 	 2
		CheckpointNE 6
		bnel		 Reg3TestFail
		Checkpoint   7
		ldr			 r0,=1
		pop			 {pc}
Reg3TestFail:
		ReturnFailed
		b			 .

		.if		UnitTesting
UnitTestReg3:
		RunTest 	REG3Test0,0,TestPassOK,TestFailed,UTReg3
		RunTest 	REG3Test1,1,TestPassed,TestFailOK,UTReg3
		RunTest 	REG3Test2,2,TestPassed,TestFailOK,UTReg3
		@ldr			r0,=REG2TestDone
		movw r0, 	#:lower16:REG3TestDone
   		movt r0, 	#:upper16:REG3TestDone
		bl			SerialOut
		pop			{pc}
		.endif

		.global Reg4Test
		.thumb_func

;		/* Register r4 test:
; 		* The registers under test will be written with
; 		* pattern1 = 0xAAAA.AAAA. Each register will be individually
; 		* compared to r0 for a pass/fail for the test.
; 		* The second part of the test will write the inverted pattern
; 		* in the register under test and pass/fail with a cmp
; 		*/
Reg4Test:
		@Load the required pattern into r0
		push	{lr}
		UnitTest 	 Reg4
;	    /*Load the required pattern = 0xAAAAAAAA into r0*/
		movw 		 r0,#0xAAAA
		movt 		 r0,#0xAAAA
		Checkpoint   1
		@Test the Register R4
		mov			 r4,r0
		Checkpoint   2
;	    /*Compare with input pattern*/
		cmp			 r4,r0
		FaultNE 	 1
		CheckpointNE 3
		bnel		 Reg4TestFail
		Checkpoint   4
		@Load the required inverse pattern into r0
;	    /*Load the required pattern = 0x55555555 into r0*/
		movw 		 r0,#0x5555
		movt 		 r0,#0x5555
		@Test the Register R4
		mov			 r4,r0
		Checkpoint   5
;	    /*Compare with input pattern*/
		cmp			 r4,r0
		FaultNE 	 2
		CheckpointNE 6
		bnel		 Reg4TestFail
		Checkpoint   7
		ldr			 r0,=1
		pop			 {pc}
Reg4TestFail:
		ReturnFailed
		b			 .

		.if		UnitTesting
UnitTestReg4:
		RunTest 	REG4Test0,0,TestPassOK,TestFailed,UTReg4
		RunTest 	REG4Test1,1,TestPassed,TestFailOK,UTReg4
		RunTest 	REG4Test2,2,TestPassed,TestFailOK,UTReg4
		@ldr			r0,=REG2TestDone
		movw r0, 	#:lower16:REG4TestDone
   		movt r0, 	#:upper16:REG4TestDone
		bl			SerialOut
		pop			{pc}
		.endif

		.global Reg5Test
		.thumb_func

;		/* Register r5 test:
; 		* The registers under test will be written with
; 		* pattern1 = 0xAAAA.AAAA. Each register will be individually
; 		* compared to r0 for a pass/fail for the test.
; 		* The second part of the test will write the inverted pattern
; 		* in the register under test and pass/fail with a cmp
; 		*/
Reg5Test:
		@Load the required pattern into r0
		push	{lr}
		UnitTest 	 Reg5
;	    /*Load the required pattern = 0xAAAAAAAA into r0*/
		movw 		 r0,#0xAAAA
		movt 		 r0,#0xAAAA
		Checkpoint	 1
		@Test the Register R5
		mov			 r5,r0
		Checkpoint	 2
;	    /*Compare with input pattern*/
		cmp			 r5,r0
		FaultNE 	 1
		CheckpointNE 3
		bnel		 Reg5TestFail
		Checkpoint	 4
		@Load the required inverse pattern into r0
;	    /*Load the required pattern = 0x55555555 into r0*/
		movw 		 r0,#0x5555
		movt 		 r0,#0x5555
		@Test the Register R5
		mov			 r5,r0
		Checkpoint	 5
;	    /*Compare with input pattern*/
		cmp			 r5,r0
		FaultNE 	 2
		CheckpointNE 6
		bnel		 Reg5TestFail
		Checkpoint	 7
		ldr			 r0,=1
		pop			 {pc}
Reg5TestFail:
		ReturnFailed
		b			 .

		.if		UnitTesting
UnitTestReg5:
		RunTest 	REG5Test0,0,TestPassOK,TestFailed,UTReg5
		RunTest 	REG5Test1,1,TestPassed,TestFailOK,UTReg5
		RunTest 	REG5Test2,2,TestPassed,TestFailOK,UTReg5
		@ldr			r0,=REG2TestDone
		movw r0, 	#:lower16:REG5TestDone
   		movt r0, 	#:upper16:REG5TestDone
		bl			SerialOut
		pop			{pc}
		.endif

		.global Reg6Test
		.thumb_func

;		/* Register r6 test:
; 		* The registers under test will be written with
; 		* pattern1 = 0xAAAA.AAAA. Each register will be individually
; 		* compared to r0 for a pass/fail for the test.
; 		* The second part of the test will write the inverted pattern
; 		* in the register under test and pass/fail with a cmp
; 		*/
Reg6Test:
		@Load the required pattern into r0
		push	{lr}
		UnitTest 	 Reg6
		Checkpoint  1
;	    /*Load the required pattern = 0xAAAAAAAA into r0*/
		movw 		 r0,#0xAAAA
		movt 		 r0,#0xAAAA
		@Test the Register R6
		mov			 r6,r0
		Checkpoint  2
;	    /*Compare with input pattern*/
		cmp			 r6,r0
		FaultNE 	 1
		CheckpointNE 3
		bnel		 Reg6TestFail
		Checkpoint  4
		@Load the required inverse pattern into r0
		movw 		 r0,#0x5555
		movt 		 r0,#0x5555
		@Test the Register R6
		mov			 r6,r0
		Checkpoint  5
;	    /*Compare with input pattern*/
		cmp			 r6,r0
		FaultNE 	 2
		CheckpointNE 6
		bnel		 Reg6TestFail
		Checkpoint  7
		ldr			 r0,=1
		pop			 {pc}
Reg6TestFail:
		ReturnFailed
		b			 .

		.if		UnitTesting
UnitTestReg6:
		RunTest 	REG6Test0,0,TestPassOK,TestFailed,UTReg6
		RunTest 	REG6Test1,1,TestPassed,TestFailOK,UTReg6
		RunTest 	REG6Test2,2,TestPassed,TestFailOK,UTReg6
		@ldr			r0,=REG2TestDone
		movw r0, 	#:lower16:REG6TestDone
   		movt r0, 	#:upper16:REG6TestDone
		bl			SerialOut
		pop			{pc}
		.endif

		.global Reg7Test
		.thumb_func

;		/* Register r7 test:
; 		* The registers under test will be written with
; 		* pattern1 = 0xAAAA.AAAA. Each register will be individually
; 		* compared to r0 for a pass/fail for the test.
; 		* The second part of the test will write the inverted pattern
; 		* in the register under test and pass/fail with a cmp
; 		*/
Reg7Test:
		@Load the required pattern into r0
		push	{lr}
		UnitTest 	 Reg7
		Checkpoint   1
;	    /*Load the required pattern = 0xAAAAAAAA into r0*/
		movw 		 r0,#0xAAAA
		movt 		 r0,#0xAAAA
		@Test the Register R7
		mov			 r7,r0
		Checkpoint   2
;	    /*Compare with input pattern*/
		cmp			 r7,r0
		FaultNE 	 1
		CheckpointNE 3
		bnel		 Reg7TestFail
		Checkpoint   4
		@Load the required inverse pattern into r0
;	    /*Load the required pattern = 0x55555555 into r0*/
		movw 		 r0,#0x5555
		movt 		 r0,#0x5555
		@Test the Register R7
		mov			 r7,r0
		Checkpoint   5
;	    /*Compare with input pattern*/
		cmp			 r7,r0
		FaultNE 	 2
		CheckpointNE 6
		bnel		 Reg7TestFail
		Checkpoint   7
		ldr			 r0,=1
		pop			 {pc}
Reg7TestFail:
		ReturnFailed
		b			 .

		.if		UnitTesting
UnitTestReg7:
		RunTest 	REG7Test0,0,TestPassOK,TestFailed,UTReg7
		RunTest 	REG7Test1,1,TestPassed,TestFailOK,UTReg7
		RunTest 	REG7Test2,2,TestPassed,TestFailOK,UTReg7
		@ldr			r0,=REG2TestDone
		movw r0, 	#:lower16:REG7TestDone
   		movt r0, 	#:upper16:REG7TestDone
		bl			SerialOut
		pop			{pc}
		.endif

		.global Reg8Test
		.thumb_func

;		/* Register r8 test:
; 		* The registers under test will be written with
; 		* pattern1 = 0xAAAA.AAAA. Each register will be individually
; 		* compared to r0 for a pass/fail for the test.
; 		* The second part of the test will write the inverted pattern
; 		* in the register under test and pass/fail with a cmp
; 		*/
Reg8Test:
		@Load the required pattern into r0
		push	{lr}
		UnitTest 	 Reg8
		Checkpoint   1
;	    /*Load the required pattern = 0xAAAAAAAA into r0*/
		movw 		 r0,#0xAAAA
		movt 		 r0,#0xAAAA
		@Test the Register R8
		mov			 r8,r0
		Checkpoint   2
;	    /*Compare with input pattern*/
		cmp			 r8,r0
		FaultNE 	 1
		CheckpointNE 3
		bnel		 Reg8TestFail
		Checkpoint   4
		@Load the required inverse pattern into r0
;	    /*Load the required pattern = 0x55555555 into r0*/
		movw 		 r0,#0x5555
		movt 		 r0,#0x5555
		@Test the Register R8
		mov			 r8,r0
		Checkpoint   5
;	    /*Compare with input pattern*/
		cmp			 r8,r0
		FaultNE 	 2
		CheckpointNE 6
		bnel		  Reg8TestFail
		Checkpoint  7
		ldr			 r0,=1
		pop			 {pc}
Reg8TestFail:
		ReturnFailed
		b			 .

		.if		UnitTesting
UnitTestReg8:
		RunTest 	REG8Test0,0,TestPassOK,TestFailed,UTReg8
		RunTest 	REG8Test1,1,TestPassed,TestFailOK,UTReg8
		RunTest 	REG8Test2,2,TestPassed,TestFailOK,UTReg8
		@ldr			r0,=REG2TestDone
		movw r0, 	#:lower16:REG8TestDone
   		movt r0, 	#:upper16:REG8TestDone
		bl			SerialOut
		pop			{pc}
		.endif

		.global Reg9Test
		.thumb_func

;		/* Register r9 test:
; 		* The registers under test will be written with
; 		* pattern1 = 0xAAAA.AAAA. Each register will be individually
; 		* compared to r0 for a pass/fail for the test.
; 		* The second part of the test will write the inverted pattern
; 		* in the register under test and pass/fail with a cmp
; 		*/
Reg9Test:
		@Load the required pattern into r0
		push	{lr}
		UnitTest 	 Reg9
		Checkpoint   1
;	    /*Load the required pattern = 0xAAAAAAAA into r0*/
		movw 		 r0,#0xAAAA
		movt 		 r0,#0xAAAA
		@Test the Register R9
		mov			 r9,r0
		Checkpoint   2
;	    /*Compare with input pattern*/
		cmp			 r9,r0
		FaultNE 	 1
		CheckpointNE 3
		bnel		 Reg9TestFail
		Checkpoint   4
		@Load the required inverse pattern into r0
;	    /*Load the required pattern = 0x55555555 into r0*/
		movw 		 r0,#0x5555
		movt 		 r0,#0x5555
		@Test the Register R9
		mov			 r9,r0
		Checkpoint   5
;	    /*Compare with input pattern*/
		cmp			 r9,r0
		FaultNE 	 2
		CheckpointNE 6
		bnel		 Reg9TestFail
		Checkpoint   7
		ldr			 r0,=1
		pop			 {pc}
Reg9TestFail:
		ReturnFailed
		b			 .

		.if		UnitTesting
UnitTestReg9:
		RunTest 	REG9Test0,0,TestPassOK,TestFailed,UTReg9
		RunTest 	REG9Test1,1,TestPassed,TestFailOK,UTReg9
		RunTest 	REG9Test2,2,TestPassed,TestFailOK,UTReg9
		@ldr			r0,=REG2TestDone
		movw r0, 	#:lower16:REG9TestDone
   		movt r0, 	#:upper16:REG9TestDone
		bl			SerialOut
		pop			{pc}
		.endif

		.global Reg10Test
		.thumb_func

;		/* Register r10 test:
; 		* The registers under test will be written with
; 		* pattern1 = 0xAAAA.AAAA. Each register will be individually
; 		* compared to r0 for a pass/fail for the test.
; 		* The second part of the test will write the inverted pattern
; 		* in the register under test and pass/fail with a cmp
; 		*/
Reg10Test:
		@Load the required pattern into r0
		push	{lr}
		UnitTest 	 Reg10
		Checkpoint  1
;	    /*Load the required pattern = 0xAAAAAAAA into r0*/
		movw 		 r0,#0xAAAA
		movt 		 r0,#0xAAAA
		@Test the Register R10
		mov			 r10,r0
		Checkpoint  2
;	    /*Compare with input pattern*/
		cmp			 r10,r0
		FaultNE 	 1
		CheckpointNE 3
		bnel		 Reg10TestFail
		Checkpoint  4
		@Load the required inverse pattern into r0
;	    /*Load the required pattern = 0x55555555 into r0*/
		movw 		 r0,#0x5555
		movt 		 r0,#0x5555
		@Test the Register R10
		mov			 r10,r0
		Checkpoint  5
;	    /*Compare with input pattern*/
		cmp			 r10,r0
		FaultNE 	 2
		CheckpointNE 6
		bnel		 Reg10TestFail
		Checkpoint  7
		ldr			 r0,=1
		pop			 {pc}
Reg10TestFail:
		ReturnFailed
		b			 .

		.if		UnitTesting
UnitTestReg10:
		RunTest 	REG10Test0,0,TestPassOK,TestFailed,UTReg10
		RunTest 	REG10Test1,1,TestPassed,TestFailOK,UTReg10
		RunTest 	REG10Test2,2,TestPassed,TestFailOK,UTReg10
		@ldr			r0,=REG2TestDone
		movw r0, 	#:lower16:REG10TestDone
   		movt r0, 	#:upper16:REG10TestDone
		bl			SerialOut
		pop			{pc}
		.endif

		.global Reg11Test
		.thumb_func

;		/* Register r11 test:
; 		* The registers under test will be written with
; 		* pattern1 = 0xAAAA.AAAA. Each register will be individually
; 		* compared to r0 for a pass/fail for the test.
; 		* The second part of the test will write the inverted pattern
; 		* in the register under test and pass/fail with a cmp
; 		*/
Reg11Test:
		@Load the required pattern into r0
		push	{lr}
		UnitTest 	 Reg11
		Checkpoint   1
;	    /*Load the required pattern = 0xAAAAAAAA into r0*/
		movw 		 r0,#0xAAAA
		movt 		 r0,#0xAAAA
		@Test the Register R11
		mov			 r11,r0
		Checkpoint   2
;	    /*Compare with input pattern*/
		cmp			 r11,r0
		FaultNE 	 1
		CheckpointNE 3
		bnel		 Reg11TestFail
		Checkpoint   4
		@Load the required inverse pattern into r0
;	    /*Load the required pattern = 0x55555555 into r0*/
		movw 		 r0,#0x5555
		movt 		 r0,#0x5555
		@Test the Register R11
		mov			 r11,r0
		Checkpoint   5
;	    /*Compare with input pattern*/
		cmp			 r11,r0
		FaultNE 	 2
		CheckpointNE 6
		bnel		 Reg11TestFail
		Checkpoint   7
		ldr			 r0,=1
		pop			 {pc}
Reg11TestFail:
		ReturnFailed
		b			 .

		.if		UnitTesting
UnitTestReg11:
		RunTest 	REG11Test0,0,TestPassOK,TestFailed,UTReg11
		RunTest 	REG11Test1,1,TestPassed,TestFailOK,UTReg11
		RunTest 	REG11Test2,2,TestPassed,TestFailOK,UTReg11
		@ldr			r0,=REG2TestDone
		movw r0, 	#:lower16:REG11TestDone
   		movt r0, 	#:upper16:REG11TestDone
		bl			SerialOut
		pop			{pc}
		.endif

		.global Reg12Test
		.thumb_func

;		/* Register r12 test:
; 		* The registers under test will be written with
; 		* pattern1 = 0xAAAA.AAAA. Each register will be individually
; 		* compared to r0 for a pass/fail for the test.
; 		* The second part of the test will write the inverted pattern
; 		* in the register under test and pass/fail with a cmp
; 		*/
Reg12Test:
		@Load the required pattern into r0
		push	{lr}
		@mov			 r3,r12
		UnitTest 	 Reg12
		Checkpoint   1
;	    /*Load the required pattern = 0xAAAAAAAA into r0*/
		movw 		 r0,#0xAAAA
		movt 		 r0,#0xAAAA
		@Test the Register R12
		mov			 r12,r0
		Checkpoint   2
;	    /*Compare with input pattern*/
		cmp			 r12,r0
		FaultNE 	 1
		CheckpointNE 3
		bnel		 Reg12TestFail
		Checkpoint   4
		@Load the required inverse pattern into r0
;	    /*Load the required pattern = 0x55555555 into r0*/
		movw 		 r0,#0x5555
		movt 		 r0,#0x5555
		@Test the Register R12
		mov			 r12,r0
		Checkpoint   5
;	    /*Compare with input pattern*/
		cmp			 r12,r0
		FaultNE 	 2
		CheckpointNE 6
		bnel		 Reg12TestFail
		Checkpoint   7
		@mov			 r12,r3
		ldr			 r0,=1
		pop			 {pc}
Reg12TestFail:
		@mov			 r12,r3
		ReturnFailed
		b			 .

		.if		UnitTesting
UnitTestReg12:
		RunTest 	REG12Test0,0,TestPassOK,TestFailed,UTReg12
		RunTest 	REG12Test1,1,TestPassed,TestFailOK,UTReg12
		RunTest 	REG12Test2,2,TestPassed,TestFailOK,UTReg12
		@ldr			r0,=REG2TestDone
		movw r0, 	#:lower16:REG12TestDone
   		movt r0, 	#:upper16:REG12TestDone
		bl			SerialOut
		pop			{pc}
		.endif

		.global Reg14Test
		.thumb_func

;		/* Register r14 test:
; 		* The registers under test will be written with
; 		* pattern1 = 0xAAAA.AAAA. Each register will be individually
; 		* compared to r0 for a pass/fail for the test.
; 		* The second part of the test will write the inverted pattern
; 		* in the register under test and pass/fail with a cmp
; 		*/
Reg14Test:
		@Load the required pattern into r0
		push	{lr}
		UnitTest 	 Reg14
		Checkpoint   1
;	    /*Load the required pattern = 0xAAAAAAAA into r0*/
		movw 		 r0,#0xAAAA
		movt 		 r0,#0xAAAA
		@Test the Register R14
		mov			 r14,r0
		Checkpoint   2
;	    /*Compare with input pattern*/
		cmp			 r14,r0
		FaultNE      1
		CheckpointNE 3
		bnel		 Reg14TestFail
		@Load the required inverse pattern into r0
;	    /*Load the required pattern = 0x55555555 into r0*/
		movw 		 r0,#0x5555
		movt 		 r0,#0x5555
		Checkpoint	 4
		@Test the Register R14
		mov			 r14,r0
		Checkpoint	 5
;	    /*Compare with input pattern*/
		cmp			 r14,r0
		FaultNE      2
		CheckpointNE 6
		bnel		 Reg14TestFail
		Checkpoint	 7
		ldr			 r0,=1
		pop			 {pc}
Reg14TestFail:
		ReturnFailed
		b			 .

		.if		UnitTesting
UnitTestReg14:
		RunTest 	REG14Test0,0,TestPassOK,TestFailed,UTReg14
		RunTest 	REG14Test1,1,TestPassed,TestFailOK,UTReg14
		RunTest 	REG14Test2,2,TestPassed,TestFailOK,UTReg14
		@ldr			r0,=REG2TestDone
		movw r0, 	#:lower16:REG14TestDone
   		movt r0, 	#:upper16:REG14TestDone
		bl			SerialOut
		pop			{pc}

Test0:		.asciz	"R0 Test pass 0:"
Test1:		.asciz	"R0 Test pass 1:"
Test2:		.asciz	"R0 Test pass 2:"
Test3:		.asciz	"R0 Test pass 3:"
Test4:		.asciz	"R0 Test pass 4:"
Test5:		.asciz	"R0 Test pass 5:"
Test6:		.asciz	"R0 Test pass 6:"
Test7:		.asciz	"R0 Test pass 7:"
Test8:		.asciz	"R0 Test pass 8:"
TestR1_0:	.asciz	"R1 Test pass 0:"
TestR1_1:	.asciz	"R1 Test pass 1:"
TestR1_2:	.asciz	"R1 Test pass 2:"
TestR1_3:	.asciz	"R1 Test pass 3:"
TestR1_4:	.asciz	"R1 Test pass 4:"
TestR1_5:	.asciz	"R1 Test pass 5:"
TestR1_6:	.asciz	"R1 Test pass 6:"
TestR1_7:	.asciz	"R1 Test pass 7:"
TestR1_8:	.asciz	"R1 Test pass 8:"
TestPassed:	.asciz	" - Passed, error\n"
TestFailed:	.asciz	" - Failed, error\n"
TestPassOK:	.asciz	" - Passed, correct\n"
TestFailOK:	.asciz	" - Failed, correct\n"
TestDone:	.asciz	"R0 Tests complete\n"
TestDone1:	.asciz	"R1 Tests complete\n"
BasePriTest0: .asciz "BasePri Test 0:"
BasePriTest1: .asciz "BasePri Test 1:"
BasePriTest2: .asciz "BasePri Test 2:"
BasePriTestDone: .asciz	"BasePri Tests complete\n"
APSRTest0: .asciz "APSR Test 0:"
APSRTest1: .asciz "APSR Test 1:"
APSRTest2: .asciz "APSR Test 2:"
APSRTestDone: .asciz	"APSR complete\n"
MSPTest0: .asciz "MSP Test 0:"
MSPTest1: .asciz "MSP Test 1:"
MSPTest2: .asciz "MSP Test 2:"
MSPTest3: .asciz "MSP Test 3:"
MSPTest4: .asciz "MSP Test 4:"
MSPTest5: .asciz "MSP Test 5:"
MSPTestDone: .asciz "MSP Test Complete\n"
PSPTest0: .asciz "PSP Test 0:"
PSPTest1: .asciz "PSP Test 1:"
PSPTest2: .asciz "PSP Test 2:"
PSPTest3: .asciz "PSP Test 3:"
PSPTest4: .asciz "PSP Test 4:"
PSPTest5: .asciz "PSP Test 5:"
PSPTestDone: .asciz "PSP Test Complete\n"
PRIMASKTest0: .asciz "PRIMASK Test 0:"
PRIMASKTest1: .asciz "PRIMASK Test 1:"
PRIMASKTest2: .asciz "PRIMASK Test 2:"
PRIMASKTest3: .asciz "PRIMASK Test 3:"
PRIMASKTest4: .asciz "PRIMASK Test 4:"
PRIMASKTest5: .asciz "PRIMASK Test 5:"
PRIMASKTestDone: .asciz "PRIMASK Test Complete\n"
FAULTMASKTest0: .asciz "FAULTMASK Test 0:"
FAULTMASKTest1: .asciz "FAULTMASK Test 1:"
FAULTMASKTest2: .asciz "FAULTMASK Test 2:"
FAULTMASKTest3: .asciz "FAULTMASK Test 3:"
FAULTMASKTest4: .asciz "FAULTMASK Test 4:"
FAULTMASKTest5: .asciz "FAULTMASK Test 5:"
FAULTMASKTestDone: .asciz "FAULTMASK Test Complete\n"
REG2Test0: .asciz "R2 Test 0:"
REG2Test1: .asciz "R2 Test 1:"
REG2Test2: .asciz "R2 Test 2:"
REG2TestDone: .asciz	"R2 Test Complete\n"
REG3Test0: .asciz "R3 Test 0:"
REG3Test1: .asciz "R3 Test 1:"
REG3Test2: .asciz "R3 Test 2:"
REG3TestDone: .asciz	"R3 Test Complete\n"
REG4Test0: .asciz "R4 Test 0:"
REG4Test1: .asciz "R4 Test 1:"
REG4Test2: .asciz "R4 Test 2:"
REG4TestDone: .asciz	"R4 Test Complete\n"
REG5Test0: .asciz "R5 Test 0:"
REG5Test1: .asciz "R5 Test 1:"
REG5Test2: .asciz "R5 Test 2:"
REG5TestDone: .asciz	"R5 Test Complete\n"
REG6Test0: .asciz "R6 Test 0:"
REG6Test1: .asciz "R6 Test 1:"
REG6Test2: .asciz "R6 Test 2:"
REG6TestDone: .asciz	"R6 Test Complete\n"
REG7Test0: .asciz "R7 Test 0:"
REG7Test1: .asciz "R7 Test 1:"
REG7Test2: .asciz "R7 Test 2:"
REG7TestDone: .asciz	"R7 Test Complete\n"
REG8Test0: .asciz "R8 Test 0:"
REG8Test1: .asciz "R8 Test 1:"
REG8Test2: .asciz "R8 Test 2:"
REG8TestDone: .asciz	"R8 Test Complete\n"
REG9Test0: .asciz "R9 Test 0:"
REG9Test1: .asciz "R9 Test 1:"
REG9Test2: .asciz "R9 Test 2:"
REG9TestDone: .asciz	"R9 Test Complete\n"
REG10Test0: .asciz "R10 Test 0:"
REG10Test1: .asciz "R10 Test 1:"
REG10Test2: .asciz "R10 Test 2:"
REG10TestDone: .asciz	"R10 Test Complete\n"
REG11Test0: .asciz "R11 Test 0:"
REG11Test1: .asciz "R11 Test 1:"
REG11Test2: .asciz "R11 Test 2:"
REG11TestDone: .asciz	"R11 Test Complete\n"
REG12Test0: .asciz "R12 Test 0:"
REG12Test1: .asciz "R12 Test 1:"
REG12Test2: .asciz "R12 Test 2:"
REG12TestDone: .asciz	"R12 Test Complete\n"
REG14Test0: .asciz "R14 Test 0:"
REG14Test1: .asciz "R14 Test 1:"
REG14Test2: .asciz "R14 Test 2:"
REG14TestDone: .asciz	"R14 Test Complete\n"

		.data
testPass:
		.word	0
.endif
