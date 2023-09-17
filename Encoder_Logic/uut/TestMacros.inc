@ $Header: \Branson/AC/uut/TestMacros.inc,v 1.8 2011/01/31 20:53:45 satya Exp $

@ UUT implementation of test macros

.set	UnitTesting, 1

@ This macro is used right at the start of reset code. It needs to initialize
@ the serial port so that we can see what is going on in the module.
.macro InitDiagnostics
		bl		DiagInit
		.endm

@ This macro is used to initiate unit test instead of normal operation. Unit
@ test code must be close to the routine being tested, because of 'b' branch
@ limits.
.macro UnitTest label
		b		UnitTest\label
UT\label:
		push	{lr}		@ need to save LR because we call other functions
							@ from the unit test code (checkpoints etc)
		.endm

@ Macro is used in any place in assembly code when we have unconditional
@ checkpoint. Macro calls C function, so need to preserve R0-R3.
.macro Checkpoint number
		push	{r0-r3}
		mov		r0,r12
		mov		r1,r14
		push	{r0-r1}
		ldr		r0,=\number
		bl		DiagCheckpoint
		pop		{r0-r1}
		mov		r12,r0
		mov		r14,r1
		pop		{r0-r3}
		.endm

@ Macro is used for diagnostics output in transitions on 'not equal' condition.
@ Macro should be placed before the branch, since multiple assembly code
@ branches could transfer to the same label, and we need to know which
@ transition took us there.
@ Macro must preserve condition flags so that the branch after it will see the
@ same condition. Also, macro only prints checkpoint number if condition is NE
@ for the jump that follows.
.macro CheckpointNE number
		beq		1f
		push	{r0-r3}
		mrs		r0,apsr
		mov		r1,r12
		mov		r2,r14
		push	{r0-r2}
		ldr		r0,=\number
		bl		DiagCheckpoint
		pop		{r0-r2}
		mov		r14,r2
		mov		r12,r1
		msr		apsr,r0
		pop		{r0-r3}
1:
		.endm

@ Macro is used to inject a fault at specific location. Check for 'not equal'
@ must normally pass, but here to simulate failure we will flip 'not equal' bit
@ so that the conditional jump 'bne' will take us to the failure label instead.
@ This macro should be followed by CheckpointNE macro so that we can see which
@ branch will be taken when we reach 'bne' instruction.
@ Macro uses 32-bit memory location 'testPass' to determine whether or not
@ fault should be injected. If value in that memory location is same as the
@ parameter provided, 'zero' bit in status register is flipped.
.macro FaultNE test
		push	{r0-r3}
		mrs		r0,apsr
		@ ldr	r1,=testPass		
		movw	r1, #:lower16:testPass	@ can't use simple format because of
		movt	r1, #:upper16:testPass	@ PC-relative constant addressing limits
		ldr		r2,[r1]
		cmp		r2,#\test
		bne		1f
		@ ldr	r1,=0x40000000
		movw	r1, #0x0000
		movt	r1, #0x4000
		eor		r0,r1				@ flip the 'zero' bit
1:
		msr		apsr,r0
		pop		{r0-r3}
		.endm

@ This macro is used for diagnostics unit test code to return from the failed
@ routine once we detected failure, so we can continue with next test.
.macro ReturnFailed
		ldr		r0,=0
		pop		{pc}
		.endm

.macro	RunTest message, pass, ok, fail, UTLabel
		@ldr		r0,=\message		@ load and display the test progress message
		movw r0, #:lower16:\message
   		movt r0, #:upper16:\message
		bl		SerialOut
		@ldr		r0,=testPass		@ set up the test pass number
		movw r0, #:lower16:testPass
   		movt r0, #:upper16:testPass
		ldr		r1,=\pass
		str		r1,[r0]
		bl		\UTLabel			@ execute the test
		@ldr		r1,=\ok				@ if r0 > 0, it was ok
		movw r1, #:lower16:\ok
   		movt r1, #:upper16:\ok
		cmp		r0,#0
		bne		1f
		@ldr		r1,=\fail			@ if r0 = 0, test failed
		movw r1, #:lower16:\fail
   		movt r1, #:upper16:\fail
1:		mov		r0,r1				@ display the result message
		bl		SerialOut
.endm
