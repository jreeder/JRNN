/********************************************************************
	created:	2013/05/29
	created:	29:5:2013   21:49
	filename: 	pyrevkbccnetwork.cpp
	author:		jreeder
	
	purpose:	python wrapper for revkbccnetwork
*********************************************************************/

#include "networks/pynetworkshelper.h"
#include <networks/RevKBCCNetwork.h>
#include <boost/python.hpp>

using namespace JRNN;
using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(RevKBCCNetwork_buildoverloads, Build, 2, 7);



void exportRevKBCCNetwork(){

	NetworkPtr (RevKBCCNetwork::*ClonePtr)() = &RevKBCCNetwork::Clone;

	class_<RevKBCCNetwork, RevKBCCNetworkPtr, bases<KBCCNetwork> >("RevKBCCNetwork").
		def("Create", &RevKBCCNetwork::Create).staticmethod("Create").
		def("Build", &RevKBCCNetwork::Build, RevKBCCNetwork_buildoverloads()).
		def("GetOutputs", &RevKBCCNetwork::GetOutputs).
		def("GetTrueOutputs", &RevKBCCNetwork::GetTrueOutputs).
		def("Reset", &RevKBCCNetwork::Reset).
		def_readwrite("getTrueOuts", &RevKBCCNetwork::getTrueOuts).
		def("Clone", ClonePtr).
		def("CloneToKBCC", &RevKBCCNetwork::CloneToKBCC)
		;
}