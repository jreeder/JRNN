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
	${OBJECTDIR}/_ext/607673378/JRNN.o \
	${OBJECTDIR}/_ext/384699334/connection.o \
	${OBJECTDIR}/_ext/1976599979/BackPropTrainer.o \
	${OBJECTDIR}/_ext/1062213505/activationfunctions.o \
	${OBJECTDIR}/_ext/384699334/layer.o \
	${OBJECTDIR}/_ext/2013951826/CCNetwork.o \
	${OBJECTDIR}/_ext/1062213505/mtldataset.o \
	${OBJECTDIR}/_ext/384699334/network.o \
	${OBJECTDIR}/_ext/1062213505/nodebuffer.o \
	${OBJECTDIR}/_ext/1976599979/CCTrainer.o \
	${OBJECTDIR}/_ext/1062213505/dataset.o \
	${OBJECTDIR}/_ext/1976599979/RPropTrainer.o \
	${OBJECTDIR}/_ext/384699334/node.o \
	${OBJECTDIR}/_ext/2013951826/FFMLPNetwork.o


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
	"${MAKE}"  -f nbproject/Makefile-Release.mk dist/Release/GNU-MacOSX/libjrnn.a

dist/Release/GNU-MacOSX/libjrnn.a: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-MacOSX
	${RM} dist/Release/GNU-MacOSX/libjrnn.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjrnn.a ${OBJECTFILES} 
	$(RANLIB) dist/Release/GNU-MacOSX/libjrnn.a

${OBJECTDIR}/_ext/607673378/JRNN.o: ../../../src/JRNN/JRNN.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/607673378
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/607673378/JRNN.o ../../../src/JRNN/JRNN.cpp

${OBJECTDIR}/_ext/384699334/connection.o: ../../../src/JRNN/structure/connection.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/384699334
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/384699334/connection.o ../../../src/JRNN/structure/connection.cpp

${OBJECTDIR}/_ext/1976599979/BackPropTrainer.o: ../../../src/JRNN/trainers/BackPropTrainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1976599979
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1976599979/BackPropTrainer.o ../../../src/JRNN/trainers/BackPropTrainer.cpp

${OBJECTDIR}/_ext/1062213505/activationfunctions.o: ../../../src/JRNN/utility/activationfunctions.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1062213505
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1062213505/activationfunctions.o ../../../src/JRNN/utility/activationfunctions.cpp

${OBJECTDIR}/_ext/384699334/layer.o: ../../../src/JRNN/structure/layer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/384699334
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/384699334/layer.o ../../../src/JRNN/structure/layer.cpp

${OBJECTDIR}/_ext/2013951826/CCNetwork.o: ../../../src/JRNN/networks/CCNetwork.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2013951826
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2013951826/CCNetwork.o ../../../src/JRNN/networks/CCNetwork.cpp

${OBJECTDIR}/_ext/1062213505/mtldataset.o: ../../../src/JRNN/utility/mtldataset.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1062213505
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1062213505/mtldataset.o ../../../src/JRNN/utility/mtldataset.cpp

${OBJECTDIR}/_ext/384699334/network.o: ../../../src/JRNN/structure/network.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/384699334
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/384699334/network.o ../../../src/JRNN/structure/network.cpp

${OBJECTDIR}/_ext/1062213505/nodebuffer.o: ../../../src/JRNN/utility/nodebuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1062213505
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1062213505/nodebuffer.o ../../../src/JRNN/utility/nodebuffer.cpp

${OBJECTDIR}/_ext/1976599979/CCTrainer.o: ../../../src/JRNN/trainers/CCTrainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1976599979
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1976599979/CCTrainer.o ../../../src/JRNN/trainers/CCTrainer.cpp

${OBJECTDIR}/_ext/1062213505/dataset.o: ../../../src/JRNN/utility/dataset.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1062213505
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1062213505/dataset.o ../../../src/JRNN/utility/dataset.cpp

${OBJECTDIR}/_ext/1976599979/RPropTrainer.o: ../../../src/JRNN/trainers/RPropTrainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1976599979
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1976599979/RPropTrainer.o ../../../src/JRNN/trainers/RPropTrainer.cpp

${OBJECTDIR}/_ext/384699334/node.o: ../../../src/JRNN/structure/node.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/384699334
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/384699334/node.o ../../../src/JRNN/structure/node.cpp

${OBJECTDIR}/_ext/2013951826/FFMLPNetwork.o: ../../../src/JRNN/networks/FFMLPNetwork.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2013951826
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../boost_1_43_0 -I../../../include/JRNN -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2013951826/FFMLPNetwork.o ../../../src/JRNN/networks/FFMLPNetwork.cpp

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
