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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/owTest.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/owTest.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/_ext/300852818/crc.o ${OBJECTDIR}/_ext/300852818/errorcodes.o ${OBJECTDIR}/_ext/300852818/nokia5110.o ${OBJECTDIR}/_ext/300852818/nu32.o ${OBJECTDIR}/_ext/300852818/onewire.o ${OBJECTDIR}/_ext/300852818/spi.o ${OBJECTDIR}/_ext/300852818/timer.o ${OBJECTDIR}/_ext/300852818/ds18x20.o ${OBJECTDIR}/_ext/300852818/error_reporting.o ${OBJECTDIR}/_ext/300852818/safestring.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/_ext/300852818/crc.o.d ${OBJECTDIR}/_ext/300852818/errorcodes.o.d ${OBJECTDIR}/_ext/300852818/nokia5110.o.d ${OBJECTDIR}/_ext/300852818/nu32.o.d ${OBJECTDIR}/_ext/300852818/onewire.o.d ${OBJECTDIR}/_ext/300852818/spi.o.d ${OBJECTDIR}/_ext/300852818/timer.o.d ${OBJECTDIR}/_ext/300852818/ds18x20.o.d ${OBJECTDIR}/_ext/300852818/error_reporting.o.d ${OBJECTDIR}/_ext/300852818/safestring.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/_ext/300852818/crc.o ${OBJECTDIR}/_ext/300852818/errorcodes.o ${OBJECTDIR}/_ext/300852818/nokia5110.o ${OBJECTDIR}/_ext/300852818/nu32.o ${OBJECTDIR}/_ext/300852818/onewire.o ${OBJECTDIR}/_ext/300852818/spi.o ${OBJECTDIR}/_ext/300852818/timer.o ${OBJECTDIR}/_ext/300852818/ds18x20.o ${OBJECTDIR}/_ext/300852818/error_reporting.o ${OBJECTDIR}/_ext/300852818/safestring.o


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/owTest.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

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
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/crc.o: ../libs/src/crc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/crc.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/crc.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/crc.o.d" -o ${OBJECTDIR}/_ext/300852818/crc.o ../libs/src/crc.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/errorcodes.o: ../libs/src/errorcodes.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/errorcodes.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/errorcodes.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/errorcodes.o.d" -o ${OBJECTDIR}/_ext/300852818/errorcodes.o ../libs/src/errorcodes.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/nokia5110.o: ../libs/src/nokia5110.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/nokia5110.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/nokia5110.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/nokia5110.o.d" -o ${OBJECTDIR}/_ext/300852818/nokia5110.o ../libs/src/nokia5110.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/nu32.o: ../libs/src/nu32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/nu32.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/nu32.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/nu32.o.d" -o ${OBJECTDIR}/_ext/300852818/nu32.o ../libs/src/nu32.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/onewire.o: ../libs/src/onewire.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/onewire.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/onewire.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/onewire.o.d" -o ${OBJECTDIR}/_ext/300852818/onewire.o ../libs/src/onewire.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/spi.o: ../libs/src/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/spi.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/spi.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/spi.o.d" -o ${OBJECTDIR}/_ext/300852818/spi.o ../libs/src/spi.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/timer.o: ../libs/src/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/timer.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/timer.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/timer.o.d" -o ${OBJECTDIR}/_ext/300852818/timer.o ../libs/src/timer.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/ds18x20.o: ../libs/src/ds18x20.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/ds18x20.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/ds18x20.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/ds18x20.o.d" -o ${OBJECTDIR}/_ext/300852818/ds18x20.o ../libs/src/ds18x20.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/error_reporting.o: ../libs/src/error_reporting.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/error_reporting.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/error_reporting.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/error_reporting.o.d" -o ${OBJECTDIR}/_ext/300852818/error_reporting.o ../libs/src/error_reporting.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/safestring.o: ../libs/src/safestring.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/safestring.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/safestring.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/safestring.o.d" -o ${OBJECTDIR}/_ext/300852818/safestring.o ../libs/src/safestring.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
else
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/crc.o: ../libs/src/crc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/crc.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/crc.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/crc.o.d" -o ${OBJECTDIR}/_ext/300852818/crc.o ../libs/src/crc.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/errorcodes.o: ../libs/src/errorcodes.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/errorcodes.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/errorcodes.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/errorcodes.o.d" -o ${OBJECTDIR}/_ext/300852818/errorcodes.o ../libs/src/errorcodes.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/nokia5110.o: ../libs/src/nokia5110.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/nokia5110.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/nokia5110.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/nokia5110.o.d" -o ${OBJECTDIR}/_ext/300852818/nokia5110.o ../libs/src/nokia5110.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/nu32.o: ../libs/src/nu32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/nu32.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/nu32.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/nu32.o.d" -o ${OBJECTDIR}/_ext/300852818/nu32.o ../libs/src/nu32.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/onewire.o: ../libs/src/onewire.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/onewire.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/onewire.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/onewire.o.d" -o ${OBJECTDIR}/_ext/300852818/onewire.o ../libs/src/onewire.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/spi.o: ../libs/src/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/spi.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/spi.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/spi.o.d" -o ${OBJECTDIR}/_ext/300852818/spi.o ../libs/src/spi.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/timer.o: ../libs/src/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/timer.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/timer.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/timer.o.d" -o ${OBJECTDIR}/_ext/300852818/timer.o ../libs/src/timer.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/ds18x20.o: ../libs/src/ds18x20.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/ds18x20.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/ds18x20.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/ds18x20.o.d" -o ${OBJECTDIR}/_ext/300852818/ds18x20.o ../libs/src/ds18x20.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/error_reporting.o: ../libs/src/error_reporting.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/error_reporting.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/error_reporting.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/error_reporting.o.d" -o ${OBJECTDIR}/_ext/300852818/error_reporting.o ../libs/src/error_reporting.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
${OBJECTDIR}/_ext/300852818/safestring.o: ../libs/src/safestring.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/300852818 
	@${RM} ${OBJECTDIR}/_ext/300852818/safestring.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/300852818/safestring.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/300852818/safestring.o.d" -o ${OBJECTDIR}/_ext/300852818/safestring.o ../libs/src/safestring.c   -fshort-enums -fno-common  -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimplicit  -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-prototypes -Wnested-externs -Wold-style-definition -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wstrict-prototypes -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/owTest.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/owTest.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}       -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1 
else
dist/${CND_CONF}/${IMAGE_TYPE}/owTest.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/owTest.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}       -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\pic32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/owTest.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  
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