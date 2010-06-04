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
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure/connection.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure/node.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure/layer.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/trainers/BackPropTrainer.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure/network.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/utility/dataset.o

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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-MacOSX/libjrnn.a

dist/Release/GNU-MacOSX/libjrnn.a: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-MacOSX
	${RM} dist/Release/GNU-MacOSX/libjrnn.a
	${AR} rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjrnn.a ${OBJECTFILES} 
	$(RANLIB) dist/Release/GNU-MacOSX/libjrnn.a

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure/connection.o: nbproject/Makefile-${CND_CONF}.mk ../../../src/JRNN/structure/connection.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure/connection.o ../../../src/JRNN/structure/connection.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure/node.o: nbproject/Makefile-${CND_CONF}.mk ../../../src/JRNN/structure/node.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure/node.o ../../../src/JRNN/structure/node.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure/layer.o: nbproject/Makefile-${CND_CONF}.mk ../../../src/JRNN/structure/layer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure/layer.o ../../../src/JRNN/structure/layer.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/trainers/BackPropTrainer.o: nbproject/Makefile-${CND_CONF}.mk ../../../src/JRNN/trainers/BackPropTrainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/trainers
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/trainers/BackPropTrainer.o ../../../src/JRNN/trainers/BackPropTrainer.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure/network.o: nbproject/Makefile-${CND_CONF}.mk ../../../src/JRNN/structure/network.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/structure/network.o ../../../src/JRNN/structure/network.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/utility/dataset.o: nbproject/Makefile-${CND_CONF}.mk ../../../src/JRNN/utility/dataset.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/utility
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/src/JRNN/utility/dataset.o ../../../src/JRNN/utility/dataset.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-MacOSX/libjrnn.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
