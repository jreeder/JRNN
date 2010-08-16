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
GREP=grep
NM=nm
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
	${OBJECTDIR}/_ext/2033849469/JRNN_test2.o


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
LDLIBSOPTIONS=-L../../../../boost_1_43_0/stage/lib ../JRNN/dist/Debug/GNU-MacOSX/libjrnn.a ../JRNN/dist/Release/GNU-MacOSX/libjrnn.a -lboost_thread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk dist/Release/GNU-MacOSX/jrnn_test2

dist/Release/GNU-MacOSX/jrnn_test2: ../JRNN/dist/Debug/GNU-MacOSX/libjrnn.a

dist/Release/GNU-MacOSX/jrnn_test2: ../JRNN/dist/Release/GNU-MacOSX/libjrnn.a

dist/Release/GNU-MacOSX/jrnn_test2: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-MacOSX
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/jrnn_test2 ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/2033849469/JRNN_test2.o: ../../../src/JRNN_test2/JRNN_test2.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2033849469
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2033849469/JRNN_test2.o ../../../src/JRNN_test2/JRNN_test2.cpp

# Subprojects
.build-subprojects:
	cd ../JRNN && ${MAKE}  -f Makefile CONF=Debug
	cd ../JRNN && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-MacOSX/jrnn_test2

# Subprojects
.clean-subprojects:
	cd ../JRNN && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../JRNN && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
