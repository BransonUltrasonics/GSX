@ $Header: $
@****************************************************************************/
@*                                                                          */
@*                                                                          */
@*      Copyright (c) Branson Ultrasonics Corporation, 1995,96, 2009        */
@*     This program is the property of Branson Ultrasonics Corporation      */
@*   Copying of this software is expressly forbidden, without the prior     */
@*   written consent of Branson Ultrasonics Corporation.                    */
@*                                                                          */
@*                                                                          */
@*                                                                          */
@*************************                         **************************/
@*--------------------------- MODULE DESCRIPTION ---------------------------
@
@ These macros are used to work around some Cortex-M3 limitations such as long
@ conditional jumps etc.
@------------------------------ REVISIONS ---------------------------------
@$Log: $
@-------------------------------------------------------------------------*/

.macro bnel label
		beq		1f
		b		\label
1:
		.endm
