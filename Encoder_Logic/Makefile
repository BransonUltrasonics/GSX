# $Header: \Branson/AC/Makefile,v 1.50 2011/06/28 17:46:33 satya Exp $
#****************************************************************************/
#*                                                                          */
#*                                                                          */
#*      Copyright (c) Branson Ultrasonics Corporation, 1995,96, 2009        */
#*     This program is the property of Branson Ultrasonics Corporation      */
#*   Copying of this software is expressly forbidden, without the prior     */
#*   written consent of Branson Ultrasonics Corporation.                    */
#*                                                                          */
#*                                                                          */
#*                                                                          */
#*************************                         **************************/
#*--------------------------- MODULE DESCRIPTION ---------------------------
# LPC17xx .Contains all functions support for ADC firmware library on LPC17xx
# Module name: lpc17xx_adc.c
# File name: lpc17xx_adc.c
# -------------------------- TECHNICAL NOTES --------------------------------

#------------------------------ REVISIONS ---------------------------------
# $Log: Makefile,v $
# Revision 1.50  2011/06/28 17:46:33  satya
# Updated for review comments
#
# Revision 1.49  2011/05/10 10:43:49  Ratnesh
# Removed LED Module functions.
#
# Revision 1.48  2011/05/09 13:40:08  Ratnesh
# Updated Integration Step 10 for BIST and include the missing Cross monitoring module.
#
# Revision 1.47  2011/05/03 09:24:09  Ratnesh
# Added Integration Step 9 in makefile.
#
# Revision 1.46  2011/05/03 09:20:51  Ratnesh
# Added changes for RAM Test so that it could be use  in BIST.
#
# Revision 1.45  2011/04/29 23:29:49  satya
# IntegrationStep 8 with WD added...
#
# Revision 1.44  2011/04/29 14:58:23  Ratnesh
# Added changes for Integration Step 6.
#
# Revision 1.43  2011/04/28 14:48:36  Noor
# Added changes for integration step6 and Part contact module level testing.
#
# Revision 1.42  2011/04/27 20:37:32  satya
# makefile updated for integration tests.System init now works for both integration and unit testing...
#
# Revision 1.41  2011/04/27 18:12:24  satya
# makefile updated for integration tests
#
# Revision 1.40  2011/04/26 15:30:18  Noor
# Updated for integration test.
#
# Revision 1.39  2011/04/20 12:46:20  Noor
# Added code for integration step 4.
#
# Revision 1.38  2011/04/19 22:16:00  Ashish
# Cleanup for integration
#
# Revision 1.37  2011/04/15 13:19:20  Noor
# Added for Devicediagnostics.
#
# Revision 1.36  2011/04/12 13:45:30  Noor
# Added code Intergration Test 2(Two hand operation)
#
# Revision 1.35  2011/04/06 14:55:08  Noor
# Update after sanity test.
#
# Revision 1.34  2011/04/05 14:23:08  Noor
# Added PBDiagnostics.c
#
# Revision 1.33  2011/04/04 23:25:34  satya
# PartContact and ADCTest Updated.
# For LUTest FlashTest was failing.
#
# Revision 1.32  2011/03/25 15:03:01  Noor
# Added File header and function headers.
#
#-------------------------------------------------------------------------*/

PRJ = AC

STARTUP = Vectors.s

ASM = Reset.s

ASMUNIT = RegTest.s

C = Diagnostics.c system_LPC17xx.c Printf.c String.c Global.c 
	

LIBC =   lpc17xx_clkpwr.c core_cm3.c

CUNIT = RamTest.c FlashTest.c PartContact.c InterruptTest.c Input.c  \
	StateMachine.c TwoHandOperation.c DriveDiagnostics.c EStopDiagnostics.c SonicsEnableDiagnostics.c\
	ProgramCounterTest.c TRSDiagnostics.c SBeamDiagnostics.c ULSLEDiagnostics.c \
	RTC.c SetOutputs.c QEI.c Serial.c DeviceDiagnostics.c IOInit.c \
	ADCTest.c SystemInitialization.c  UFail24VDiagnostics.c LUTest.c \
	CrossMonitoring.c TestWD.c PBDiagnostics.c SSP.c StateMachineEntryFunc.c \
	StateMachineExitFunc.c

CINT = En13849.c IntegrationStep1.c IntegrationStep2.c IntegrationStep3.c \
       IntegrationStep4.c IntegrationStep5.c IntegrationStep6.c \
       IntegrationStep7.c IntegrationStep8.c IntegrationStep9.c \
       IntegrationStep10.c IntegrationStep10_CpuStuck.c

OBJ_DIR = obj

STARTOBJ = $(addprefix $(OBJ_DIR)/,$(STARTUP:.s=_s.o))
OBJ = $(addprefix $(OBJ_DIR)/,$(C:.c=_c.o) $(ASM:.s=_s.o) $(LIBC:.c=_c.o) $(CUNIT:.c=_c.o) $(ASMUNIT:.s=_s.o) $(CINT:.c=_c.o))
UNITOBJ = $(addprefix $(OBJ_DIR)/, $(ASMUNIT:.s=_s_ut.o) $(CUNIT:.c=_c_ut.o) $(CINT:.c=_c_int.o))

UT_ELF = $(ASMUNIT:.s=_s_ut.elf) $(CUNIT:.c=_c_ut.elf)
UT_HEX = $(ASMUNIT:.s=_s_ut.hex) $(CUNIT:.c=_c_ut.hex)
UT_BIN = $(ASMUNIT:.s=_s_ut.bin) $(CUNIT:.c=_c_ut.bin)

INT_ELF = $(CINT:.c=_c_int.elf)
INT_HEX = $(CINT:.c=_c_int.hex)
INT_BIN = $(CINT:.c=_c_int.bin)

LIB = $(OBJ_DIR)/lib$(PRJ).a
DLD = $(PRJ).ld
ELF = $(PRJ).elf
BIN = $(PRJ).bin
HEX = $(PRJ).hex

$(OBJ_DIR)/%_c.o : lib/%.c inc/*.h app/*.h
	$(CC) -I app $(CCOPT) $(CCINC) -o $@ $<

$(OBJ_DIR)/%_c.o : %.c inc/*.h app/*.h
	$(CC) -I app $(CCOPT) $(CCINC) -o $@ $<

$(OBJ_DIR)/%_s.o : %.s app/*.inc
	$(AS) -I app $(ASOPT) -o $@ $<

$(OBJ_DIR)/%_c_ut.o : %.c inc/*.h uut/*.h
	$(CC) -I uut $(CCOPT) $(CCINC) -o $@ $<

$(OBJ_DIR)/%_s_ut.o : %.s uut/*.inc
	$(AS) -I uut $(ASOPT) -o $@ $<

$(OBJ_DIR)/%_c_int.o : %.c inc/*.h app/*.h
	$(CC) -I app $(CCOPT) $(CCINC) -o $@ $<

all: dirs $(STARTOBJ) $(UNITOBJ) $(LIB) $(BIN) $(HEX) $(UT_ELF) $(UT_BIN) $(UT_HEX) $(INT_ELF) $(INT_HEX) $(INT_BIN)
	@echo Done building 'all'...

clean:
	@rm $(OBJ_DIR)/*.o $(OBJ_DIR)/*.a *.hex *.bin *.elf *.map

dirs:
	@cmd /c if not exist $(OBJ_DIR) md $(OBJ_DIR)

$(LIB) : $(OBJ)
	@cmd /c if exist $@ rm $@
	$(AR) $(AROPT) $@ $^

$(ELF) : $(STARTOBJ) $(LIB)
	$(LD) $(LDOPT) -L$(OBJ_DIR) -o $@ $^ -T $(DLD) -Map $(PRJ).map

%.bin : %.elf
	$(OC) $(OCBIN) $< $@
	@CrcCal3 $@ 

%.hex : %.bin
	$(OC) $(OCHEX) $< $@

%_c_ut.elf : $(STARTOBJ) $(OBJ_DIR)/%_c_ut.o $(LIB)
	$(LD) $(LDOPT) -L$(OBJ_DIR) -o $@ $^ -T $(DLD) -Map $(basename $@).map

%_s_ut.elf : $(STARTOBJ) $(OBJ_DIR)/%_s_ut.o $(LIB)
	$(LD) $(LDOPT) -L$(OBJ_DIR) -o $@ $^ -T $(DLD) -Map $(basename $@).map

%_c_int.elf : $(STARTOBJ) $(OBJ_DIR)/%_c_int.o $(LIB)
	$(LD) $(LDOPT) -L$(OBJ_DIR) -o $@ $^ -T $(DLD) -Map $(basename $@).map
	
CC = arm-none-eabi-gcc
CCOPT = -O2 -Wall -c -mcpu=cortex-m3 -mthumb -mapcs-frame -mthumb-interwork \
	-fmessage-length=0
CCINC = -I inc
AS = arm-none-eabi-as
ASOPT = -mcpu=cortex-m3 -mthumb -mthumb-interwork -I inc
LD = arm-none-eabi-ld
LDOPT = -s -M --warn-once
AR = arm-none-eabi-ar
AROPT = -rv
OC = arm-none-eabi-objcopy
OCBIN = -I elf32-littlearm -O binary 
OCHEX = -I binary -O ihex 
