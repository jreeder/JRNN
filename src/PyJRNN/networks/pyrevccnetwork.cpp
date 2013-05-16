/********************************************************************
	created:	2012/08/18
	created:	18:8:2012   17:59
	filename: 	pyrevccnetwork.cpp
	author:		jreeder
	
	purpose:	python wrapper for the revccnetwork class. 
*********************************************************************/

#include "networks/pynetworkshelper.h"
#include <networks/RevCCNetwork.h>
#include <boost/python.hpp>

using namespace JRNN;
using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(RevCCNetwork_buildoverloads, Build, 2, 7);



void exportRevCCNetwork(){

	NetworkPtr (RevCCNetwork::*ClonePtr)() = &RevCCNetwork::Clone;

	class_<RevCCNetwork, RevCCNetworkPtr, bases<CCNetwork> >("RevCCNetwork").
		def("Create", &RevCCNetwork::Create).staticmethod("Create").
		def("Build", &RevCCNetwork::Build, RevCCNetwork_buildoverloads()).
		def("GetOutputs", &RevCCNetwork::GetOutputs).
		def("GetTrueOutputs", &RevCCNetwork::GetTrueOutputs).
		def("Reset", &RevCCNetwork::Reset).
		def_readwrite("getTrueOuts", &RevCCNetwork::getTrueOuts).
		def("Clone", ClonePtr)
		;
}