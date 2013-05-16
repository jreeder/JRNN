/********************************************************************
	created:	2013/05/07
	created:	7:5:2013   1:33
	filename: 	pykbccnetwork.cpp
	author:		jreeder
	
	purpose:	Python interface for a kbccnetwork. 
*********************************************************************/

#include "networks/pynetworkshelper.h"
#include <networks/KBCCNetwork.h>
#include <boost/python.hpp>

using namespace JRNN;
using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(KBCCNetwork_buildoverloads, Build, 2,6);

void exportKBCCNetwork() {

	NetworkPtr (KBCCNetwork::*KBCCClonePtr)() = &KBCCNetwork::Clone;

	class_<KBCCNetwork, KBCCNetworkPtr, bases<CCNetwork> >("KBCCNetwork").
		def("Create", &KBCCNetwork::Create).staticmethod("Create").
		def("Build", &KBCCNetwork::Build, KBCCNetwork_buildoverloads()).
		def("Reset", &KBCCNetwork::Reset).
		def("Clone", KBCCClonePtr);
}