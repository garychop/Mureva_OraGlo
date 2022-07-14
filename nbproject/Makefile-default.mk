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
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/MU01.80.0001.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ1024GA606
MP_LINKER_FILE_OPTION=,--script=p24FJ1024GA606.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/default/96ed37d1981d4f2dbae079e02155266c492ac78 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/default/b42bd1dc30a2374c8eac8ca766fd5bcfbbaf0e4f .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/default/52e7f5e7855d6cd28f00890737397a8bb4fea940 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/default/494daec3f37da23f059c66cb9c529473e6d90dc9 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/i2c1.o: mcc_generated_files/i2c1.c  .generated_files/flags/default/8c5835c6ebb816937e5c5f636c8531b44eae3b01 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/i2c1.c  -o ${OBJECTDIR}/mcc_generated_files/i2c1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/i2c1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi1.o: mcc_generated_files/spi1.c  .generated_files/flags/default/24534a235870c1bb16625604764635795e0b58de .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi1.c  -o ${OBJECTDIR}/mcc_generated_files/spi1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi2.o: mcc_generated_files/spi2.c  .generated_files/flags/default/439481aa82cb3236ab2b945ab613a4aba0693877 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi2.c  -o ${OBJECTDIR}/mcc_generated_files/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi2.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi3.o: mcc_generated_files/spi3.c  .generated_files/flags/default/d3702c170e2b2d9dee32aa5f860600cefb3e995 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi3.c  -o ${OBJECTDIR}/mcc_generated_files/spi3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi3.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/default/977a0a9d7a9ea8a605e28bd5834bf15900ff761a .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/oc1.o: mcc_generated_files/oc1.c  .generated_files/flags/default/263608e11c855735500cb4e8b10866d6a3d5e2f3 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc1.c  -o ${OBJECTDIR}/mcc_generated_files/oc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/oc2.o: mcc_generated_files/oc2.c  .generated_files/flags/default/3c6a8b9182d1bb26ba3479869a51669a54f2a8a2 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc2.c  -o ${OBJECTDIR}/mcc_generated_files/oc2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc2.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/oc3.o: mcc_generated_files/oc3.c  .generated_files/flags/default/8fb6f79dee6d169e6406c99ce07f05a6795b157 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc3.c  -o ${OBJECTDIR}/mcc_generated_files/oc3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc3.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/adc1.o: mcc_generated_files/adc1.c  .generated_files/flags/default/b771641bfcd7e286776aeb47186b516b94916956 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/adc1.c  -o ${OBJECTDIR}/mcc_generated_files/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/adc1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/ic1.o: mcc_generated_files/ic1.c  .generated_files/flags/default/c2d6703b1f27257e17f7df946b21d029e223c33e .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ic1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ic1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/ic1.c  -o ${OBJECTDIR}/mcc_generated_files/ic1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/ic1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/rtcc.o: mcc_generated_files/rtcc.c  .generated_files/flags/default/a4175c9a33f92d04016b4f6b73a0a2f8971fbae9 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/rtcc.c  -o ${OBJECTDIR}/mcc_generated_files/rtcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/rtcc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr2.o: mcc_generated_files/tmr2.c  .generated_files/flags/default/ee00779c973f54fe1cb786ee59c106fb5417b352 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr2.c  -o ${OBJECTDIR}/mcc_generated_files/tmr2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr2.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/default/ad4bde865d42115a983883e62d7b3f52c67e97a9 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/1e1d639c2a94706b4baef8eb685e1eaf77e3fcc7 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Test_Functions.o: Test_Functions.c  .generated_files/flags/default/925c57bf262c270eed40fb4aa243b5feb34e5d31 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Test_Functions.o.d 
	@${RM} ${OBJECTDIR}/Test_Functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Test_Functions.c  -o ${OBJECTDIR}/Test_Functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Test_Functions.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/UI_Functions.o: UI_Functions.c  .generated_files/flags/default/ecd7c01a0685e4d2f84609a7f58579ab55af2192 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/UI_Functions.o.d 
	@${RM} ${OBJECTDIR}/UI_Functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  UI_Functions.c  -o ${OBJECTDIR}/UI_Functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/UI_Functions.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Support_Functions.o: Support_Functions.c  .generated_files/flags/default/6bec9d1332437329a42634ed14ad6db17ccd1422 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Support_Functions.o.d 
	@${RM} ${OBJECTDIR}/Support_Functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Support_Functions.c  -o ${OBJECTDIR}/Support_Functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Support_Functions.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/timers.o: timers.c  .generated_files/flags/default/c560917c44d904a521a25e6ed5e4350c332b79b3 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timers.o.d 
	@${RM} ${OBJECTDIR}/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  timers.c  -o ${OBJECTDIR}/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/timers.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/flashmem.o: flashmem.c  .generated_files/flags/default/bb0ce9325b7003448794d5c360a8a56b0b11d11e .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/flashmem.o.d 
	@${RM} ${OBJECTDIR}/flashmem.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  flashmem.c  -o ${OBJECTDIR}/flashmem.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/flashmem.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/pushbuttons.o: pushbuttons.c  .generated_files/flags/default/bfcf73ddb862f583d0c4d4e55713a7e147d51679 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pushbuttons.o.d 
	@${RM} ${OBJECTDIR}/pushbuttons.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pushbuttons.c  -o ${OBJECTDIR}/pushbuttons.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/pushbuttons.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/ledboard.o: ledboard.c  .generated_files/flags/default/7103cd55e31bd8aeb97e8b79e2ede197a5f8317b .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ledboard.o.d 
	@${RM} ${OBJECTDIR}/ledboard.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ledboard.c  -o ${OBJECTDIR}/ledboard.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ledboard.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/analog.o: analog.c  .generated_files/flags/default/ba63cd523b612d62d4cee40be09fc77af962996b .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/analog.o.d 
	@${RM} ${OBJECTDIR}/analog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  analog.c  -o ${OBJECTDIR}/analog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/analog.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rtc.o: rtc.c  .generated_files/flags/default/78307767da56fc6e7fb3f68f7631fe639aa410a4 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rtc.o.d 
	@${RM} ${OBJECTDIR}/rtc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  rtc.c  -o ${OBJECTDIR}/rtc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/rtc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/tachometer.o: tachometer.c  .generated_files/flags/default/aa1156fcae9c0f611b17e43a97e312f273eaaae1 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/tachometer.o.d 
	@${RM} ${OBJECTDIR}/tachometer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  tachometer.c  -o ${OBJECTDIR}/tachometer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/tachometer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/hw_watchdog.o: hw_watchdog.c  .generated_files/flags/default/6bf4f22e1fd7a2d1acf60acbba9197fe46754239 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hw_watchdog.o.d 
	@${RM} ${OBJECTDIR}/hw_watchdog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw_watchdog.c  -o ${OBJECTDIR}/hw_watchdog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/hw_watchdog.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/lcd.o: lcd.c  .generated_files/flags/default/9d4fe7ce902136fc88abf27f99d4661ff50b1189 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lcd.o.d 
	@${RM} ${OBJECTDIR}/lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  lcd.c  -o ${OBJECTDIR}/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/lcd.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/screensaver.o: screensaver.c  .generated_files/flags/default/901cc007c0ece1d90a261eebc62d20b8bb8e0334 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/screensaver.o.d 
	@${RM} ${OBJECTDIR}/screensaver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  screensaver.c  -o ${OBJECTDIR}/screensaver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/screensaver.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rgbled.o: rgbled.c  .generated_files/flags/default/776d5ebde34e4a189c1351147193df5c1011940d .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rgbled.o.d 
	@${RM} ${OBJECTDIR}/rgbled.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  rgbled.c  -o ${OBJECTDIR}/rgbled.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/rgbled.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/eeprom.o: eeprom.c  .generated_files/flags/default/4a385418621a61ad6ce16ccb8c78da3f9e885be3 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/eeprom.o.d 
	@${RM} ${OBJECTDIR}/eeprom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  eeprom.c  -o ${OBJECTDIR}/eeprom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/eeprom.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/swi.o: swi.c  .generated_files/flags/default/5c1fb0614a2b17d6213fb80829e8957306c0bed9 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/swi.o.d 
	@${RM} ${OBJECTDIR}/swi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  swi.c  -o ${OBJECTDIR}/swi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/swi.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mca.o: mca.c  .generated_files/flags/default/5dce051be347b75e38e99cf3465b1b99824cade4 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mca.o.d 
	@${RM} ${OBJECTDIR}/mca.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mca.c  -o ${OBJECTDIR}/mca.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mca.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rtcc.o: rtcc.c  .generated_files/flags/default/4855966b07451fa601410e43e09747d033975c58 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rtcc.o.d 
	@${RM} ${OBJECTDIR}/rtcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  rtcc.c  -o ${OBJECTDIR}/rtcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/rtcc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/default/2dbf6c2cc5119991f3dd419dc96bdfa6167f688a .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/default/feba1c32531ab3ab8ead448aadc4d4656fbf043e .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/default/bd2a46d6cb3efbccb99ae696309d635d770a72d0 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/default/ff4c4eef685f504f4bc58ff6157c0accbc4640fa .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/i2c1.o: mcc_generated_files/i2c1.c  .generated_files/flags/default/a409183c27c39badfe8292c5c01f60d8e15feae5 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/i2c1.c  -o ${OBJECTDIR}/mcc_generated_files/i2c1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/i2c1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi1.o: mcc_generated_files/spi1.c  .generated_files/flags/default/dae17ddc121a0c3627cf001974032618552f2a3f .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi1.c  -o ${OBJECTDIR}/mcc_generated_files/spi1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi2.o: mcc_generated_files/spi2.c  .generated_files/flags/default/6f75c7b70b48314c47151fd8ae75ea5c32e8545 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi2.c  -o ${OBJECTDIR}/mcc_generated_files/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi3.o: mcc_generated_files/spi3.c  .generated_files/flags/default/6539852968cbbda3b0ab1cc4c6e1e3bafd497e2d .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi3.c  -o ${OBJECTDIR}/mcc_generated_files/spi3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi3.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/default/ba5c45ca182acde4a7ef96ae31a89b49479bdaab .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/oc1.o: mcc_generated_files/oc1.c  .generated_files/flags/default/d7259c0d1b62d09e38210118e6de6d5f8ae8591a .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc1.c  -o ${OBJECTDIR}/mcc_generated_files/oc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/oc2.o: mcc_generated_files/oc2.c  .generated_files/flags/default/a69166f87daaff16354ca6b9ccc5ff3c9f276222 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc2.c  -o ${OBJECTDIR}/mcc_generated_files/oc2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/oc3.o: mcc_generated_files/oc3.c  .generated_files/flags/default/d531d1aba49a48945daf157974fc8755dd13c31c .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc3.c  -o ${OBJECTDIR}/mcc_generated_files/oc3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc3.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/adc1.o: mcc_generated_files/adc1.c  .generated_files/flags/default/e895d76f7ba2d8ec507ffd22a95739d48b821fa1 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/adc1.c  -o ${OBJECTDIR}/mcc_generated_files/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/adc1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/ic1.o: mcc_generated_files/ic1.c  .generated_files/flags/default/b73e9b5d5fc493b571874bca0c8ca4f63ee8c322 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ic1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ic1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/ic1.c  -o ${OBJECTDIR}/mcc_generated_files/ic1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/ic1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/rtcc.o: mcc_generated_files/rtcc.c  .generated_files/flags/default/739593c3c974f3891cfe7bd62bcfc75fd41b20d8 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/rtcc.c  -o ${OBJECTDIR}/mcc_generated_files/rtcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/rtcc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr2.o: mcc_generated_files/tmr2.c  .generated_files/flags/default/c9f07968af5280f2015cf705a5d66d442e1ebaa .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr2.c  -o ${OBJECTDIR}/mcc_generated_files/tmr2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/default/48774872ab02afb41c07514a8eddd9b724943c8 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/fb5cdfafbf37bc83f50039b3445a16c1045ed81d .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Test_Functions.o: Test_Functions.c  .generated_files/flags/default/8da7f1836ac0c13e2fec6c4bf1ba3f606b69b7e .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Test_Functions.o.d 
	@${RM} ${OBJECTDIR}/Test_Functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Test_Functions.c  -o ${OBJECTDIR}/Test_Functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Test_Functions.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/UI_Functions.o: UI_Functions.c  .generated_files/flags/default/5056ed08bbed6555bfd3796ab0ca5f83100b79a3 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/UI_Functions.o.d 
	@${RM} ${OBJECTDIR}/UI_Functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  UI_Functions.c  -o ${OBJECTDIR}/UI_Functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/UI_Functions.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Support_Functions.o: Support_Functions.c  .generated_files/flags/default/1a3f7002c3e0814dbcf8c265017e052674fee203 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Support_Functions.o.d 
	@${RM} ${OBJECTDIR}/Support_Functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Support_Functions.c  -o ${OBJECTDIR}/Support_Functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Support_Functions.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/timers.o: timers.c  .generated_files/flags/default/bdad1d6fd0d625c38f720469db50564575b30fd4 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timers.o.d 
	@${RM} ${OBJECTDIR}/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  timers.c  -o ${OBJECTDIR}/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/timers.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/flashmem.o: flashmem.c  .generated_files/flags/default/766f14f9ccdbee9c5541e06a593d9cba8e39801 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/flashmem.o.d 
	@${RM} ${OBJECTDIR}/flashmem.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  flashmem.c  -o ${OBJECTDIR}/flashmem.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/flashmem.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/pushbuttons.o: pushbuttons.c  .generated_files/flags/default/afb34e78ac73875915ddeaa784f3c51b073a6306 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pushbuttons.o.d 
	@${RM} ${OBJECTDIR}/pushbuttons.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pushbuttons.c  -o ${OBJECTDIR}/pushbuttons.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/pushbuttons.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/ledboard.o: ledboard.c  .generated_files/flags/default/71bf146fbd002b4cf1f328b18d5ea99b29a570ea .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ledboard.o.d 
	@${RM} ${OBJECTDIR}/ledboard.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ledboard.c  -o ${OBJECTDIR}/ledboard.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ledboard.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/analog.o: analog.c  .generated_files/flags/default/7c59130d867eee945aa9e93d7fb3365a8d9d7252 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/analog.o.d 
	@${RM} ${OBJECTDIR}/analog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  analog.c  -o ${OBJECTDIR}/analog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/analog.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rtc.o: rtc.c  .generated_files/flags/default/2a1c20062e1533708882fcb92ddaacacce60f6c7 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rtc.o.d 
	@${RM} ${OBJECTDIR}/rtc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  rtc.c  -o ${OBJECTDIR}/rtc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/rtc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/tachometer.o: tachometer.c  .generated_files/flags/default/821d5acf5fbeef70bb333070dc72bee53bf01561 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/tachometer.o.d 
	@${RM} ${OBJECTDIR}/tachometer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  tachometer.c  -o ${OBJECTDIR}/tachometer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/tachometer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/hw_watchdog.o: hw_watchdog.c  .generated_files/flags/default/beb3797331ac9bb3d09a2dd1867637063bc45fc8 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hw_watchdog.o.d 
	@${RM} ${OBJECTDIR}/hw_watchdog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hw_watchdog.c  -o ${OBJECTDIR}/hw_watchdog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/hw_watchdog.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/lcd.o: lcd.c  .generated_files/flags/default/f8715767c496fc18176e738b6055828b6781eca1 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lcd.o.d 
	@${RM} ${OBJECTDIR}/lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  lcd.c  -o ${OBJECTDIR}/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/lcd.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/screensaver.o: screensaver.c  .generated_files/flags/default/4761bb4b59d00f1e2cdc4b17f42eefd20848490e .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/screensaver.o.d 
	@${RM} ${OBJECTDIR}/screensaver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  screensaver.c  -o ${OBJECTDIR}/screensaver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/screensaver.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rgbled.o: rgbled.c  .generated_files/flags/default/6672208126d83772bbc7a95926a86af915992b4b .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rgbled.o.d 
	@${RM} ${OBJECTDIR}/rgbled.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  rgbled.c  -o ${OBJECTDIR}/rgbled.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/rgbled.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/eeprom.o: eeprom.c  .generated_files/flags/default/d98df49e37bfd1e044cfd74bae87465acdb0fe8c .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/eeprom.o.d 
	@${RM} ${OBJECTDIR}/eeprom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  eeprom.c  -o ${OBJECTDIR}/eeprom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/eeprom.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/swi.o: swi.c  .generated_files/flags/default/6865ad3fdf2e45e118ca6c07b4819f259e9c8d23 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/swi.o.d 
	@${RM} ${OBJECTDIR}/swi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  swi.c  -o ${OBJECTDIR}/swi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/swi.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mca.o: mca.c  .generated_files/flags/default/b1f6c8db3ef012d83cf1d6b8e0ef8a212d16810a .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mca.o.d 
	@${RM} ${OBJECTDIR}/mca.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mca.c  -o ${OBJECTDIR}/mca.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mca.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/rtcc.o: rtcc.c  .generated_files/flags/default/773fa074c1febb6c79da615eb866b338000cb5f7 .generated_files/flags/default/a17533bbe6d58954224e6f2e176f4ef0eb012303
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rtcc.o.d 
	@${RM} ${OBJECTDIR}/rtcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  rtcc.c  -o ${OBJECTDIR}/rtcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/rtcc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -mlarge-code -menable-large-arrays -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/MU01.80.0001.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt  -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/MU01.80.0001.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/MU01.80.0001.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -specs=cmd.txt -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
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
