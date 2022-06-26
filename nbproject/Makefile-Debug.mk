#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-Debug.mk)" "nbproject/Makefile-local-Debug.mk"
include nbproject/Makefile-local-Debug.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Debug
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/MU01.80.0001.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/MU01.80.0001.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=mcc_generated_files/interrupt_manager.c mcc_generated_files/traps.c mcc_generated_files/pin_manager.c mcc_generated_files/mcc.c mcc_generated_files/i2c1.c mcc_generated_files/spi1.c mcc_generated_files/spi2.c mcc_generated_files/spi3.c mcc_generated_files/uart1.c mcc_generated_files/oc1.c mcc_generated_files/oc2.c mcc_generated_files/oc3.c mcc_generated_files/adc1.c mcc_generated_files/ic1.c mcc_generated_files/rtcc.c mcc_generated_files/tmr2.c mcc_generated_files/tmr1.c main.c Test_Functions.c UI_Functions.c Support_Functions.c timers.c flashmem.c pushbuttons.c ledboard.c analog.c rtc.c tachometer.c hw_watchdog.c lcd.c screensaver.c rgbled.c eeprom.c swi.c mca.c rtcc.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/i2c1.o ${OBJECTDIR}/mcc_generated_files/spi1.o ${OBJECTDIR}/mcc_generated_files/spi2.o ${OBJECTDIR}/mcc_generated_files/spi3.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/mcc_generated_files/oc1.o ${OBJECTDIR}/mcc_generated_files/oc2.o ${OBJECTDIR}/mcc_generated_files/oc3.o ${OBJECTDIR}/mcc_generated_files/adc1.o ${OBJECTDIR}/mcc_generated_files/ic1.o ${OBJECTDIR}/mcc_generated_files/rtcc.o ${OBJECTDIR}/mcc_generated_files/tmr2.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/main.o ${OBJECTDIR}/Test_Functions.o ${OBJECTDIR}/UI_Functions.o ${OBJECTDIR}/Support_Functions.o ${OBJECTDIR}/timers.o ${OBJECTDIR}/flashmem.o ${OBJECTDIR}/pushbuttons.o ${OBJECTDIR}/ledboard.o ${OBJECTDIR}/analog.o ${OBJECTDIR}/rtc.o ${OBJECTDIR}/tachometer.o ${OBJECTDIR}/hw_watchdog.o ${OBJECTDIR}/lcd.o ${OBJECTDIR}/screensaver.o ${OBJECTDIR}/rgbled.o ${OBJECTDIR}/eeprom.o ${OBJECTDIR}/swi.o ${OBJECTDIR}/mca.o ${OBJECTDIR}/rtcc.o
POSSIBLE_DEPFILES=${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d ${OBJECTDIR}/mcc_generated_files/traps.o.d ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d ${OBJECTDIR}/mcc_generated_files/mcc.o.d ${OBJECTDIR}/mcc_generated_files/i2c1.o.d ${OBJECTDIR}/mcc_generated_files/spi1.o.d ${OBJECTDIR}/mcc_generated_files/spi2.o.d ${OBJECTDIR}/mcc_generated_files/spi3.o.d ${OBJECTDIR}/mcc_generated_files/uart1.o.d ${OBJECTDIR}/mcc_generated_files/oc1.o.d ${OBJECTDIR}/mcc_generated_files/oc2.o.d ${OBJECTDIR}/mcc_generated_files/oc3.o.d ${OBJECTDIR}/mcc_generated_files/adc1.o.d ${OBJECTDIR}/mcc_generated_files/ic1.o.d ${OBJECTDIR}/mcc_generated_files/rtcc.o.d ${OBJECTDIR}/mcc_generated_files/tmr2.o.d ${OBJECTDIR}/mcc_generated_files/tmr1.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/Test_Functions.o.d ${OBJECTDIR}/UI_Functions.o.d ${OBJECTDIR}/Support_Functions.o.d ${OBJECTDIR}/timers.o.d ${OBJECTDIR}/flashmem.o.d ${OBJECTDIR}/pushbuttons.o.d ${OBJECTDIR}/ledboard.o.d ${OBJECTDIR}/analog.o.d ${OBJECTDIR}/rtc.o.d ${OBJECTDIR}/tachometer.o.d ${OBJECTDIR}/hw_watchdog.o.d ${OBJECTDIR}/lcd.o.d ${OBJECTDIR}/screensaver.o.d ${OBJECTDIR}/rgbled.o.d ${OBJECTDIR}/eeprom.o.d ${OBJECTDIR}/swi.o.d ${OBJECTDIR}/mca.o.d ${OBJECTDIR}/rtcc.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/i2c1.o ${OBJECTDIR}/mcc_generated_files/spi1.o ${OBJECTDIR}/mcc_generated_files/spi2.o ${OBJECTDIR}/mcc_generated_files/spi3.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/mcc_generated_files/oc1.o ${OBJECTDIR}/mcc_generated_files/oc2.o ${OBJECTDIR}/mcc_generated_files/oc3.o ${OBJECTDIR}/mcc_generated_files/adc1.o ${OBJECTDIR}/mcc_generated_files/ic1.o ${OBJECTDIR}/mcc_generated_files/rtcc.o ${OBJECTDIR}/mcc_generated_files/tmr2.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/main.o ${OBJECTDIR}/Test_Functions.o ${OBJECTDIR}/UI_Functions.o ${OBJECTDIR}/Support_Functions.o ${OBJECTDIR}/timers.o ${OBJECTDIR}/flashmem.o ${OBJECTDIR}/pushbuttons.o ${OBJECTDIR}/ledboard.o ${OBJECTDIR}/analog.o ${OBJECTDIR}/rtc.o ${OBJECTDIR}/tachometer.o ${OBJECTDIR}/hw_watchdog.o ${OBJECTDIR}/lcd.o ${OBJECTDIR}/screensaver.o ${OBJECTDIR}/rgbled.o ${OBJECTDIR}/eeprom.o ${OBJECTDIR}/swi.o ${OBJECTDIR}/mca.o ${OBJECTDIR}/rtcc.o

# Source Files
SOURCEFILES=mcc_generated_files/interrupt_manager.c mcc_generated_files/traps.c mcc_generated_files/pin_manager.c mcc_generated_files/mcc.c mcc_generated_files/i2c1.c mcc_generated_files/spi1.c mcc_generated_files/spi2.c mcc_generated_files/spi3.c mcc_generated_files/uart1.c mcc_generated_files/oc1.c mcc_generated_files/oc2.c mcc_generated_files/oc3.c mcc_generated_files/adc1.c mcc_generated_files/ic1.c mcc_generated_files/rtcc.c mcc_generated_files/tmr2.c mcc_generated_files/tmr1.c main.c Test_Functions.c UI_Functions.c Support_Functions.c timers.c flashmem.c pushbuttons.c ledboard.c analog.c rtc.c tachometer.c hw_watchdog.c lcd.c screensaver.c rgbled.c eeprom.c swi.c mca.c rtcc.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-Debug.mk ${DISTDIR}/MU01.80.0001.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ1024GA606
MP_LINKER_FILE_OPTION=,--script=p24FJ1024GA606.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/Debug/8a281b3d8acbb9ebd98c46bc4edba2c56c44a688 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/Debug/beab6c1f2982dee39d4d00052e2aa8712491b5b .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/Debug/ff99c2e7885f0c263b5250f4f3214726dc15e65 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/Debug/4451410301541488db1a2741a1b6d7b2a5e2ece6 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/i2c1.o: mcc_generated_files/i2c1.c  .generated_files/flags/Debug/6baa32b14220dcc20441ac953571172c8ad4e45f .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/i2c1.c  -o ${OBJECTDIR}/mcc_generated_files/i2c1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/i2c1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi1.o: mcc_generated_files/spi1.c  .generated_files/flags/Debug/899c3e7a421727df2475d243b6b68e8e9eaee7ee .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi1.c  -o ${OBJECTDIR}/mcc_generated_files/spi1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi2.o: mcc_generated_files/spi2.c  .generated_files/flags/Debug/cbc39f7e3d0e46b8a232bc365610b856c481f3ba .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi2.c  -o ${OBJECTDIR}/mcc_generated_files/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi2.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi3.o: mcc_generated_files/spi3.c  .generated_files/flags/Debug/14a9be772ab870e769f2bb5623ffda058c36839c .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi3.c  -o ${OBJECTDIR}/mcc_generated_files/spi3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi3.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/Debug/957198a40a47988479c76590d70a5f6b1b980929 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/oc1.o: mcc_generated_files/oc1.c  .generated_files/flags/Debug/1b75d14f50a95818372b44995ed06d877d5851ea .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc1.c  -o ${OBJECTDIR}/mcc_generated_files/oc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/oc2.o: mcc_generated_files/oc2.c  .generated_files/flags/Debug/2e46f463241b24976af07f238719f2dfd8cb19b0 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc2.c  -o ${OBJECTDIR}/mcc_generated_files/oc2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc2.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/oc3.o: mcc_generated_files/oc3.c  .generated_files/flags/Debug/92f3c299da1a3a8626edcf9e100899fa2fb1ceb5 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc3.c  -o ${OBJECTDIR}/mcc_generated_files/oc3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc3.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/adc1.o: mcc_generated_files/adc1.c  .generated_files/flags/Debug/7731864e630ce174495c6a75b987511445075515 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/adc1.c  -o ${OBJECTDIR}/mcc_generated_files/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/adc1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/ic1.o: mcc_generated_files/ic1.c  .generated_files/flags/Debug/5afa64a8d5985a52ca424722cfe5a84d4bd6a8b9 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ic1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ic1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/ic1.c  -o ${OBJECTDIR}/mcc_generated_files/ic1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/ic1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/rtcc.o: mcc_generated_files/rtcc.c  .generated_files/flags/Debug/90c4c2fbe8b0791546822abb9e8e8c2aed0bc573 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/rtcc.c  -o ${OBJECTDIR}/mcc_generated_files/rtcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/rtcc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr2.o: mcc_generated_files/tmr2.c  .generated_files/flags/Debug/43bbc1db82eedcfaa3d3c73d18e9da789eebef41 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr2.c  -o ${OBJECTDIR}/mcc_generated_files/tmr2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr2.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/Debug/5fa0434e17fae67398ba1c07c0e9f4ba2f317e51 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/Debug/d357679e5409196d50152434403760ea1b9dd51 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Test_Functions.o: Test_Functions.c  .generated_files/flags/Debug/bfbaf6291dc920b5a9ff9fcea7c27132db616798 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Test_Functions.o.d 
	@${RM} ${OBJECTDIR}/Test_Functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Test_Functions.c  -o ${OBJECTDIR}/Test_Functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Test_Functions.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/UI_Functions.o: UI_Functions.c  .generated_files/flags/Debug/25820c737bc286d696dec8e2b0b7e030c772ca3b .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/UI_Functions.o.d 
	@${RM} ${OBJECTDIR}/UI_Functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  UI_Functions.c  -o ${OBJECTDIR}/UI_Functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/UI_Functions.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Support_Functions.o: Support_Functions.c  .generated_files/flags/Debug/792dd1574a432c7d6228a4a47be00e87000d3dca .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Support_Functions.o.d 
	@${RM} ${OBJECTDIR}/Support_Functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Support_Functions.c  -o ${OBJECTDIR}/Support_Functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Support_Functions.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/timers.o: timers.c  .generated_files/flags/Debug/8ae2f22340c5727a99d154b811e20d3a7f4dd76a .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timers.o.d 
	@${RM} ${OBJECTDIR}/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  timers.c  -o ${OBJECTDIR}/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/timers.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/flashmem.o: flashmem.c  .generated_files/flags/Debug/759bee358d61d801f4b0de02fb97dba649537a7a .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/flashmem.o.d 
	@${RM} ${OBJECTDIR}/flashmem.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  flashmem.c  -o ${OBJECTDIR}/flashmem.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/flashmem.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/pushbuttons.o: pushbuttons.c  .generated_files/flags/Debug/de0e9afe6285a98fa0fad9b5f4a7fc824935a4fc .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pushbuttons.o.d 
	@${RM} ${OBJECTDIR}/pushbuttons.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pushbuttons.c  -o ${OBJECTDIR}/pushbuttons.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/pushbuttons.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/ledboard.o: ledboard.c  .generated_files/flags/Debug/58a287660d0fe2adedd5b1a3d64f58231c4c67a8 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ledboard.o.d 
	@${RM} ${OBJECTDIR}/ledboard.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ledboard.c  -o ${OBJECTDIR}/ledboard.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ledboard.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/analog.o: analog.c  .generated_files/flags/Debug/cbadb02d60a13f6da4935f7cc11960fea8e6b1e5 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/analog.o.d 
	@${RM} ${OBJECTDIR}/analog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  analog.c  -o ${OBJECTDIR}/analog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/analog.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rtc.o: rtc.c  .generated_files/flags/Debug/2819c44ad5516e024130dc0025269eed4cb22143 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rtc.o.d 
	@${RM} ${OBJECTDIR}/rtc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  rtc.c  -o ${OBJECTDIR}/rtc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/rtc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/tachometer.o: tachometer.c  .generated_files/flags/Debug/6b6dcea43696cd45736de857d566666ebcdb3885 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/tachometer.o.d 
	@${RM} ${OBJECTDIR}/tachometer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  tachometer.c  -o ${OBJECTDIR}/tachometer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/tachometer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/hw_watchdog.o: hw_watchdog.c  .generated_files/flags/Debug/2bf69066b042cd4797370e17415c75559276439 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hw_watchdog.o.d 
	@${RM} ${OBJECTDIR}/hw_watchdog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw_watchdog.c  -o ${OBJECTDIR}/hw_watchdog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/hw_watchdog.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/lcd.o: lcd.c  .generated_files/flags/Debug/ba3a4f606328cea0173825535753c84836fcceeb .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lcd.o.d 
	@${RM} ${OBJECTDIR}/lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  lcd.c  -o ${OBJECTDIR}/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/lcd.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/screensaver.o: screensaver.c  .generated_files/flags/Debug/a65c9d8fe0da7e10b81e9916955b387b6707f793 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/screensaver.o.d 
	@${RM} ${OBJECTDIR}/screensaver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  screensaver.c  -o ${OBJECTDIR}/screensaver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/screensaver.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rgbled.o: rgbled.c  .generated_files/flags/Debug/4da74eafb60bdb11391cdd67a5041cb9d50c3668 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rgbled.o.d 
	@${RM} ${OBJECTDIR}/rgbled.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  rgbled.c  -o ${OBJECTDIR}/rgbled.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/rgbled.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/eeprom.o: eeprom.c  .generated_files/flags/Debug/20bd63e903d7dfc068e8f30f6f8d3c083dd99444 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/eeprom.o.d 
	@${RM} ${OBJECTDIR}/eeprom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  eeprom.c  -o ${OBJECTDIR}/eeprom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/eeprom.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/swi.o: swi.c  .generated_files/flags/Debug/62efdca45796300887b532f8c8807333a8508745 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/swi.o.d 
	@${RM} ${OBJECTDIR}/swi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  swi.c  -o ${OBJECTDIR}/swi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/swi.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mca.o: mca.c  .generated_files/flags/Debug/e1a06c4e5e96482b746179b8d5dac9f61e21db6e .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mca.o.d 
	@${RM} ${OBJECTDIR}/mca.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mca.c  -o ${OBJECTDIR}/mca.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mca.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rtcc.o: rtcc.c  .generated_files/flags/Debug/2f90335a259a89e4850c75a0cebdd98ac3c4edea .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rtcc.o.d 
	@${RM} ${OBJECTDIR}/rtcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  rtcc.c  -o ${OBJECTDIR}/rtcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/rtcc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/Debug/41ee1d8b48a6fa1b2c3640a9fde5755dd1b2b48b .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/Debug/527bba92551dcd60f7f75a424cbe31e7084c1550 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/Debug/dffc6400e8d7628421d4852e74d5df99ed943035 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/Debug/f1fc803e2fd6f4fe8bf37cf58b524e277b0e5749 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/i2c1.o: mcc_generated_files/i2c1.c  .generated_files/flags/Debug/4489ae4e0c01b72fdd277b9be1aeb06f40071e47 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/i2c1.c  -o ${OBJECTDIR}/mcc_generated_files/i2c1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/i2c1.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi1.o: mcc_generated_files/spi1.c  .generated_files/flags/Debug/89506e8527e078d94a4ff2588a75d8d4afce2f19 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi1.c  -o ${OBJECTDIR}/mcc_generated_files/spi1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi1.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi2.o: mcc_generated_files/spi2.c  .generated_files/flags/Debug/a3d4677ea6822447e355226ab8091231a39ca3be .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi2.c  -o ${OBJECTDIR}/mcc_generated_files/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi2.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi3.o: mcc_generated_files/spi3.c  .generated_files/flags/Debug/510159750585fb9f6d241f90d115771243cdafc0 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi3.c  -o ${OBJECTDIR}/mcc_generated_files/spi3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi3.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/Debug/3386e3cf100c9e5ef31b3fbc5a462573b6f2b797 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/oc1.o: mcc_generated_files/oc1.c  .generated_files/flags/Debug/72057b318185cd9e80e821866c58226a1e514c43 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc1.c  -o ${OBJECTDIR}/mcc_generated_files/oc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc1.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/oc2.o: mcc_generated_files/oc2.c  .generated_files/flags/Debug/c40aad63d9eae8fef2194a4ed09635fa163789f3 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc2.c  -o ${OBJECTDIR}/mcc_generated_files/oc2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc2.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/oc3.o: mcc_generated_files/oc3.c  .generated_files/flags/Debug/a1984e61ea84a55ee918c0fd3d4e94769a9c8d9d .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc3.c  -o ${OBJECTDIR}/mcc_generated_files/oc3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc3.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/adc1.o: mcc_generated_files/adc1.c  .generated_files/flags/Debug/47ee4f1a33574501db9e2e8afe297896fa21571f .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/adc1.c  -o ${OBJECTDIR}/mcc_generated_files/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/adc1.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/ic1.o: mcc_generated_files/ic1.c  .generated_files/flags/Debug/725d8c4267beba6709c630bc834f90dd1300a28a .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ic1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ic1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/ic1.c  -o ${OBJECTDIR}/mcc_generated_files/ic1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/ic1.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/rtcc.o: mcc_generated_files/rtcc.c  .generated_files/flags/Debug/8bbe7b8854fae2a02ef918a11882ec9ce3d313b7 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/rtcc.c  -o ${OBJECTDIR}/mcc_generated_files/rtcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/rtcc.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr2.o: mcc_generated_files/tmr2.c  .generated_files/flags/Debug/3c369ff360d52a7ff06a3a93e2f2a4cf523c95af .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr2.c  -o ${OBJECTDIR}/mcc_generated_files/tmr2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr2.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/Debug/dc584e32b4f0e0da29a8b1ee3dc7b87f6d78e4e4 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/Debug/6df69a777c0a766ef1f4475071e1191b1849c0aa .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Test_Functions.o: Test_Functions.c  .generated_files/flags/Debug/409696f6107bb5c924f66114cebb33603e340ca4 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Test_Functions.o.d 
	@${RM} ${OBJECTDIR}/Test_Functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Test_Functions.c  -o ${OBJECTDIR}/Test_Functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Test_Functions.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/UI_Functions.o: UI_Functions.c  .generated_files/flags/Debug/b0f1d15e17075dcdb8e937e79e3a2cb902e8c7c3 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/UI_Functions.o.d 
	@${RM} ${OBJECTDIR}/UI_Functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  UI_Functions.c  -o ${OBJECTDIR}/UI_Functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/UI_Functions.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Support_Functions.o: Support_Functions.c  .generated_files/flags/Debug/4ebbc1e7def26d08eec2fb5ea95754aaa4e1cda8 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Support_Functions.o.d 
	@${RM} ${OBJECTDIR}/Support_Functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Support_Functions.c  -o ${OBJECTDIR}/Support_Functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Support_Functions.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/timers.o: timers.c  .generated_files/flags/Debug/f94cff36decada11a337a77659dca0b893e448e3 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timers.o.d 
	@${RM} ${OBJECTDIR}/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  timers.c  -o ${OBJECTDIR}/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/timers.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/flashmem.o: flashmem.c  .generated_files/flags/Debug/2b7c13315530e149af837f5590c9e22ffe4a1f2c .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/flashmem.o.d 
	@${RM} ${OBJECTDIR}/flashmem.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  flashmem.c  -o ${OBJECTDIR}/flashmem.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/flashmem.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/pushbuttons.o: pushbuttons.c  .generated_files/flags/Debug/9609e4e261c842b0d5742aeba6c99df2b6f9ccb .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pushbuttons.o.d 
	@${RM} ${OBJECTDIR}/pushbuttons.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pushbuttons.c  -o ${OBJECTDIR}/pushbuttons.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/pushbuttons.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/ledboard.o: ledboard.c  .generated_files/flags/Debug/2a82ec9eb3d4ffc87d017fee03c326e690f82d32 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ledboard.o.d 
	@${RM} ${OBJECTDIR}/ledboard.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ledboard.c  -o ${OBJECTDIR}/ledboard.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ledboard.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/analog.o: analog.c  .generated_files/flags/Debug/b49267bc9a008060d1ad71c4ce37df0eae7a3091 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/analog.o.d 
	@${RM} ${OBJECTDIR}/analog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  analog.c  -o ${OBJECTDIR}/analog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/analog.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rtc.o: rtc.c  .generated_files/flags/Debug/cf5b8d6c3a58a532a00d7987cd0868f9ce27839 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rtc.o.d 
	@${RM} ${OBJECTDIR}/rtc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  rtc.c  -o ${OBJECTDIR}/rtc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/rtc.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/tachometer.o: tachometer.c  .generated_files/flags/Debug/2a414dd4054634d8f48e35526ecd0a7b0b7ef072 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/tachometer.o.d 
	@${RM} ${OBJECTDIR}/tachometer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  tachometer.c  -o ${OBJECTDIR}/tachometer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/tachometer.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/hw_watchdog.o: hw_watchdog.c  .generated_files/flags/Debug/dcefa93ed3df2f0fc90dfc1f57c6602ce52313bf .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hw_watchdog.o.d 
	@${RM} ${OBJECTDIR}/hw_watchdog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw_watchdog.c  -o ${OBJECTDIR}/hw_watchdog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/hw_watchdog.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/lcd.o: lcd.c  .generated_files/flags/Debug/9fdd31186a7cc5f5471821932c65fe1b8cb56fd1 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lcd.o.d 
	@${RM} ${OBJECTDIR}/lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  lcd.c  -o ${OBJECTDIR}/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/lcd.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/screensaver.o: screensaver.c  .generated_files/flags/Debug/62ad4c06ac37eb80a76f1994f4c1dbda77a2865a .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/screensaver.o.d 
	@${RM} ${OBJECTDIR}/screensaver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  screensaver.c  -o ${OBJECTDIR}/screensaver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/screensaver.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rgbled.o: rgbled.c  .generated_files/flags/Debug/1cdecddf9e3593409b648ddcbc687477eebed91c .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rgbled.o.d 
	@${RM} ${OBJECTDIR}/rgbled.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  rgbled.c  -o ${OBJECTDIR}/rgbled.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/rgbled.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/eeprom.o: eeprom.c  .generated_files/flags/Debug/397698d2327391653b0cdd748017b5f4d69f93ef .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/eeprom.o.d 
	@${RM} ${OBJECTDIR}/eeprom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  eeprom.c  -o ${OBJECTDIR}/eeprom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/eeprom.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/swi.o: swi.c  .generated_files/flags/Debug/8849233e6268aac258f465fb54c4c8dab797c3a0 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/swi.o.d 
	@${RM} ${OBJECTDIR}/swi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  swi.c  -o ${OBJECTDIR}/swi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/swi.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mca.o: mca.c  .generated_files/flags/Debug/157ecb8558b01268649ff58213eabdc515c2ec55 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mca.o.d 
	@${RM} ${OBJECTDIR}/mca.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mca.c  -o ${OBJECTDIR}/mca.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mca.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rtcc.o: rtcc.c  .generated_files/flags/Debug/6455f693008864e1fe09eaffadabffe494f70bc6 .generated_files/flags/Debug/ac52dec29e2e92f963a8c3c02887d0fc68dd6acf
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rtcc.o.d 
	@${RM} ${OBJECTDIR}/rtcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  rtcc.c  -o ${OBJECTDIR}/rtcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/rtcc.o.d"      -mno-eds-warn  -g -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/MU01.80.0001.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/MU01.80.0001.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt  -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/MU01.80.0001.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/MU01.80.0001.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DDEBUG -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/MU01.80.0001.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
