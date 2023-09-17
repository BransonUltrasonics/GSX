/* dsmArmLib.h - ARM disassembler header file */

/*
 * Copyright (c) 1996-1997, 2005, 2010, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
10oct13,h_k  moved S_dsmLib_UNKNOWN_INSTRUCTION to dsmLib.h.
             moved dsmInst to dsmLibP.h.
19oct10,jmp  Updated INSTR_IS() macro to use VXDBG_INSTR_CMP()
             (CQ:WIND00185617).
29apr10,pad  Moved extern C statement after include statements.
14jan05,rec  add print function to dsmInst
09oct97,jpd  added more Thumb instructions.
09sep97,cdp  added more instructions.
09may96,cdp  created.
*/

#ifndef	__INCdsmArmLibh
#define	__INCdsmArmLibh

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Macro and definitions for simple decoding of instuctions.
 * To check an instruction, it is ANDed with the IMASK_ and
 * the result is compared with the IOP_. The macro INSTR_IS
 * does this and returns !0 to indicate a match.
 */

#define INSTR_IS(pInstr,o)	VXDBG_INSTR_CMP(pInstr,(IOP_##o),(IMASK_##o))

#define IMASK_BL		0x0F000000	/* BL xxx */
#define IOP_BL			0x0B000000

#define IMASK_STMDB_SPP_FP_IP_LR_PC 0xFFFFF800	/* STMDB sp!,{..fp,ip,lr,pc} */
#define IOP_STMDB_SPP_FP_IP_LR_PC   0xE92DD800

#define IMASK_SUB_FP_IP_4	0xFFFFFFFF	/* SUB fp,ip,#4 */
#define IOP_SUB_FP_IP_4		0xE24CB004

#define IMASK_SUB_FP_IP_4PLUS	0xFFFFF000	/* SUB fp,ip,#4+ */
#define IOP_SUB_FP_IP_4PLUS	0xE24CB000

#define IMASK_MOV_IP_SP		0xFFFFFFFF	/* MOV ip,sp */
#define IOP_MOV_IP_SP		0xE1A0C00D

#define IMASK_MOVXX_LR_PC	0x0FFFFFFF	/* MOVxx lr,pc */
#define IOP_MOVXX_LR_PC		0x01A0E00F

#define IMASK_STMDB_SPP_AREGS	0xFFFFFFF0	/* STMDB sp!,{a1-a4} */
#define IOP_STMDB_SPP_AREGS	0xE92D0000

#define IMASK_SUB_SP_SP		0xFFFFF000	/* SUB sp,sp,#n */
#define IOP_SUB_SP_SP		0xE24DD000

#define	IMASK_MOVS_PC_LR	0xFFFFFFFF	/* MOVS pc,lr */
#define IOP_MOVS_PC_LR		0xE1B0F00E

/* Thumb instructions */

#define IMASK_T_BL0		0xF800		/* BL prefix */
#define IOP_T_BL0		0xF000

#define IMASK_T_BL1		0xF800		/* BL */
#define IOP_T_BL1		0xF800

#define IMASK_T_MOV_LR_PC	0xFFFF		/* MOV lr,pc */
#define IOP_T_MOV_LR_PC		0x46FE

#define IMASK_T_SUB_SP_16	0xFFFF		/* SUB sp,#16 */
#define IOP_T_SUB_SP_16		0xB084

#define IMASK_T_MOV_FP_LO	0xFFC7		/* MOV fp,loreg */
#define	IOP_T_MOV_FP_LO		0x4683

#define IMASK_T_MOV_SP_LO	0xFFC7		/* MOV sp,loreg */
#define	IOP_T_MOV_SP_LO		0x4685

#define IMASK_T_ADD_LO_SP	0xF800		/* ADD loreg, sp, #Imm */
#define IOP_T_ADD_LO_SP		0xA800

#define IMASK_T_POP_LO		0xFF00		/* POP {reglist} (not PC) */
#define IOP_T_POP_LO		0xBC00

#define IMASK_T_PUSH_LO		0xFF00		/* PUSH {reglist} (not LR) */
#define IOP_T_PUSH_LO		0xB400

#define IMASK_T_PUSH		0xFF00		/* PUSH {reglist} (inc LR) */
#define IOP_T_PUSH		0xB500

#define IMASK_T_BX_LO		0xFFC0		/* BX loreg */
#define IOP_T_BX_LO		0x4700

#define IMASK_T_MOV_LO_PC	0xFFF8		/* MOV loreg,pc */
#define IOP_T_MOV_LO_PC		0x4678

#define IMASK_T_MOV_LO_FP	0xFFF8		/* MOV loreg,fp */
#define IOP_T_MOV_LO_FP		0x4658

IMPORT int dsmNbytes(FAST INSTR *binInst);

#ifdef __cplusplus
}
#endif

#endif	/* __INCdsmArmLibh */
