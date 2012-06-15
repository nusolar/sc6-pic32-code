#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile

# Environment
SHELL=cmd.exe
# Adding MPLAB X bin directory to path
PATH:=C:/Program Files (x86)/Microchip/MPLABX/mplab_ide/mplab_ide/modules/../../bin/:$(PATH)
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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/can_proxy.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/can_proxy.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/319723389/NU32v2.o ${OBJECTDIR}/_ext/319723389/NUCAN.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/319723389/Nokia5110.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/319723389/NU32v2.o.d ${OBJECTDIR}/_ext/319723389/NUCAN.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/319723389/Nokia5110.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/319723389/NU32v2.o ${OBJECTDIR}/_ext/319723389/NUCAN.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/319723389/Nokia5110.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

# Path to java used to run MPLAB X when this makefile was created
MP_JAVA_PATH="C:\Program Files\Java\jre6/bin/"
OS_CURRENT="$(shell uname -s)"
############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
MP_CC="C:\Program Files (x86)\Microchip\mplabc32\v2.01\bin\pic32-gcc.exe"
# MP_BC is not defined
MP_AS="C:\Program Files (x86)\Microchip\mplabc32\v2.01\bin\pic32-as.exe"
MP_LD="C:\Program Files (x86)\Microchip\mplabc32\v2.01\bin\pic32-ld.exe"
MP_AR="C:\Program Files (x86)\Microchip\mplabc32\v2.01\bin\pic32-ar.exe"
DEP_GEN=${MP_JAVA_PATH}java -jar "C:/Program Files (x86)/Microchip/MPLABX/mplab_ide/mplab_ide/modules/../../bin/extractobjectdependencies.jar" 
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps
MP_CC_DIR="C:\Program Files (x86)\Microchip\mplabc32\v2.01\bin"
# MP_BC_DIR is not defined
MP_AS_DIR="C:\Program Files (x86)\Microchip\mplabc32\v2.01\bin"
MP_LD_DIR="C:\Program Files (x86)\Microchip\mplabc32\v2.01\bin"
MP_AR_DIR="C:\Program Files (x86)\Microchip\mplabc32\v2.01\bin"
# MP_BC_DIR is not defined

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/can_proxy.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/319723389/NU32v2.o: ../../libs/src/NU32v2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/NU32v2.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/NU32v2.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include" -MMD -MF "${OBJECTDIR}/_ext/319723389/NU32v2.o.d" -o ${OBJECTDIR}/_ext/319723389/NU32v2.o ../../libs/src/NU32v2.c  
	
${OBJECTDIR}/_ext/319723389/NUCAN.o: ../../libs/src/NUCAN.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/NUCAN.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/NUCAN.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include" -MMD -MF "${OBJECTDIR}/_ext/319723389/NUCAN.o.d" -o ${OBJECTDIR}/_ext/319723389/NUCAN.o ../../libs/src/NUCAN.c  
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include" -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  
	
${OBJECTDIR}/_ext/319723389/Nokia5110.o: ../../libs/src/Nokia5110.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/Nokia5110.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/Nokia5110.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include" -MMD -MF "${OBJECTDIR}/_ext/319723389/Nokia5110.o.d" -o ${OBJECTDIR}/_ext/319723389/Nokia5110.o ../../libs/src/Nokia5110.c  
	
else
${OBJECTDIR}/_ext/319723389/NU32v2.o: ../../libs/src/NU32v2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/NU32v2.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/NU32v2.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include" -MMD -MF "${OBJECTDIR}/_ext/319723389/NU32v2.o.d" -o ${OBJECTDIR}/_ext/319723389/NU32v2.o ../../libs/src/NU32v2.c  
	
${OBJECTDIR}/_ext/319723389/NUCAN.o: ../../libs/src/NUCAN.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/NUCAN.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/NUCAN.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include" -MMD -MF "${OBJECTDIR}/_ext/319723389/NUCAN.o.d" -o ${OBJECTDIR}/_ext/319723389/NUCAN.o ../../libs/src/NUCAN.c  
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include" -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  
	
${OBJECTDIR}/_ext/319723389/Nokia5110.o: ../../libs/src/Nokia5110.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/Nokia5110.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/Nokia5110.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include" -MMD -MF "${OBJECTDIR}/_ext/319723389/Nokia5110.o.d" -o ${OBJECTDIR}/_ext/319723389/Nokia5110.o ../../libs/src/Nokia5110.c  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/can_proxy.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/can_proxy.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}        -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-L"..",-Map="${DISTDIR}/can_proxy.X.${IMAGE_TYPE}.map" 
else
dist/${CND_CONF}/${IMAGE_TYPE}/can_proxy.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/can_proxy.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}        -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-L"..",-Map="${DISTDIR}/can_proxy.X.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\pic32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/can_proxy.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  
endif


# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
