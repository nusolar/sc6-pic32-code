#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/t_can.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/t_can.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/319723389/can.o ${OBJECTDIR}/_ext/319723389/crc.o ${OBJECTDIR}/_ext/319723389/error_reporting.o ${OBJECTDIR}/_ext/319723389/errorcodes.o ${OBJECTDIR}/_ext/319723389/led.o ${OBJECTDIR}/_ext/319723389/nu32.o ${OBJECTDIR}/_ext/319723389/safestring.o ${OBJECTDIR}/_ext/319723389/serial.o ${OBJECTDIR}/_ext/319723389/timer.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/319723389/can.o.d ${OBJECTDIR}/_ext/319723389/crc.o.d ${OBJECTDIR}/_ext/319723389/error_reporting.o.d ${OBJECTDIR}/_ext/319723389/errorcodes.o.d ${OBJECTDIR}/_ext/319723389/led.o.d ${OBJECTDIR}/_ext/319723389/nu32.o.d ${OBJECTDIR}/_ext/319723389/safestring.o.d ${OBJECTDIR}/_ext/319723389/serial.o.d ${OBJECTDIR}/_ext/319723389/timer.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/319723389/can.o ${OBJECTDIR}/_ext/319723389/crc.o ${OBJECTDIR}/_ext/319723389/error_reporting.o ${OBJECTDIR}/_ext/319723389/errorcodes.o ${OBJECTDIR}/_ext/319723389/led.o ${OBJECTDIR}/_ext/319723389/nu32.o ${OBJECTDIR}/_ext/319723389/safestring.o ${OBJECTDIR}/_ext/319723389/serial.o ${OBJECTDIR}/_ext/319723389/timer.o ${OBJECTDIR}/main.o


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/t_can.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

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
${OBJECTDIR}/_ext/319723389/can.o: ../../libs/src/can.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/can.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/can.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/can.o.d" -o ${OBJECTDIR}/_ext/319723389/can.o ../../libs/src/can.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/crc.o: ../../libs/src/crc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/crc.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/crc.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/crc.o.d" -o ${OBJECTDIR}/_ext/319723389/crc.o ../../libs/src/crc.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/error_reporting.o: ../../libs/src/error_reporting.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/error_reporting.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/error_reporting.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/error_reporting.o.d" -o ${OBJECTDIR}/_ext/319723389/error_reporting.o ../../libs/src/error_reporting.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/errorcodes.o: ../../libs/src/errorcodes.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/errorcodes.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/errorcodes.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/errorcodes.o.d" -o ${OBJECTDIR}/_ext/319723389/errorcodes.o ../../libs/src/errorcodes.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/led.o: ../../libs/src/led.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/led.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/led.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/led.o.d" -o ${OBJECTDIR}/_ext/319723389/led.o ../../libs/src/led.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/nu32.o: ../../libs/src/nu32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/nu32.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/nu32.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/nu32.o.d" -o ${OBJECTDIR}/_ext/319723389/nu32.o ../../libs/src/nu32.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/safestring.o: ../../libs/src/safestring.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/safestring.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/safestring.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/safestring.o.d" -o ${OBJECTDIR}/_ext/319723389/safestring.o ../../libs/src/safestring.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/serial.o: ../../libs/src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/serial.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/serial.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/serial.o.d" -o ${OBJECTDIR}/_ext/319723389/serial.o ../../libs/src/serial.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/timer.o: ../../libs/src/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/timer.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/timer.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/timer.o.d" -o ${OBJECTDIR}/_ext/319723389/timer.o ../../libs/src/timer.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
else
${OBJECTDIR}/_ext/319723389/can.o: ../../libs/src/can.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/can.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/can.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/can.o.d" -o ${OBJECTDIR}/_ext/319723389/can.o ../../libs/src/can.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/crc.o: ../../libs/src/crc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/crc.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/crc.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/crc.o.d" -o ${OBJECTDIR}/_ext/319723389/crc.o ../../libs/src/crc.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/error_reporting.o: ../../libs/src/error_reporting.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/error_reporting.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/error_reporting.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/error_reporting.o.d" -o ${OBJECTDIR}/_ext/319723389/error_reporting.o ../../libs/src/error_reporting.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/errorcodes.o: ../../libs/src/errorcodes.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/errorcodes.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/errorcodes.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/errorcodes.o.d" -o ${OBJECTDIR}/_ext/319723389/errorcodes.o ../../libs/src/errorcodes.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/led.o: ../../libs/src/led.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/led.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/led.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/led.o.d" -o ${OBJECTDIR}/_ext/319723389/led.o ../../libs/src/led.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/nu32.o: ../../libs/src/nu32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/nu32.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/nu32.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/nu32.o.d" -o ${OBJECTDIR}/_ext/319723389/nu32.o ../../libs/src/nu32.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/safestring.o: ../../libs/src/safestring.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/safestring.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/safestring.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/safestring.o.d" -o ${OBJECTDIR}/_ext/319723389/safestring.o ../../libs/src/safestring.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/serial.o: ../../libs/src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/serial.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/serial.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/serial.o.d" -o ${OBJECTDIR}/_ext/319723389/serial.o ../../libs/src/serial.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/319723389/timer.o: ../../libs/src/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/319723389 
	@${RM} ${OBJECTDIR}/_ext/319723389/timer.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/319723389/timer.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/_ext/319723389/timer.o.d" -o ${OBJECTDIR}/_ext/319723389/timer.o ../../libs/src/timer.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../libs/include/" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   -Wall -Wextra -Wno-missing-braces -Wno-missing-field-initializers -fshort-enums -fno-common -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual  -Wchar-subscripts -Wcomment -Wconversion -Wdeclaration-after-statement -Wdisabled-optimization -Wfloat-equal -Wformat=2 -Wimport -Winit-self -Winline -Winvalid-pch -Wjump-misses-init -Wunsafe-loop-optimizations -Wlogical-op -Wlong-long -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wpointer-arith -Wshadow -Wstack-protector -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wsync-nand  -Wundef -Wuninitialized -Wunreachable-code -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/t_can.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/t_can.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}       -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1 
else
dist/${CND_CONF}/${IMAGE_TYPE}/t_can.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/t_can.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}       -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\pic32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/t_can.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
