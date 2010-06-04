#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN_test/main.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../JRNN/dist/Debug/GNU-MacOSX/libjrnn.a ../JRNN/dist/Release/GNU-MacOSX/libjrnn.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-MacOSX/jrnn_test

dist/Release/GNU-MacOSX/jrnn_test: ../JRNN/dist/Debug/GNU-MacOSX/libjrnn.a

dist/Release/GNU-MacOSX/jrnn_test: ../JRNN/dist/Release/GNU-MacOSX/libjrnn.a

dist/Release/GNU-MacOSX/jrnn_test: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-MacOSX
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/jrnn_test ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN_test/main.o: nbproject/Makefile-${CND_CONF}.mk ../../../src/JRNN_test/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN_test
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../include/JRNN -I../../../../boost_1_43_0 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN_test/main.o ../../../src/JRNN_test/main.cpp

# Subprojects
.build-subprojects:
	cd ../JRNN && ${MAKE}  -f Makefile CONF=Debug
	cd ../JRNN && ${MAKE}  -f Makefile CONF=Release
	cd ../JRNN && ${MAKE}  -f Makefile CONF=Debug
	cd ../JRNN && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-MacOSX/jrnn_test

# Subprojects
.clean-subprojects:
	cd ../JRNN && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../JRNN && ${MAKE}  -f Makefile CONF=Release clean
	cd ../JRNN && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../JRNN && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
