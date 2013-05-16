/********************************************************************
	created:	26/07/2012 16:39
	filename: 	ccnetwork.cpp
	author:		jreeder
	
	purpose:	This will wrap the CCNetwork class for python
*********************************************************************/


#include "networks/pynetworkshelper.h"
#include <boost/python.hpp>

using namespace JRNN;
using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CCNetwork_buildoverloads,Build,2,6);

void exportCCNetwork(){

	NetworkPtr (CCNetwork::*CCClonePtr)() = &CCNetwork::Clone;

	class_<CCNetWrap, CCNetworkPtr, bases<Network> >("CCNetwork").
		def("Create", &CCNetwork::Create).staticmethod("Create").
		def("Clone", CCClonePtr).
		def("Build", &CCNetwork::Build, CCNetwork_buildoverloads()).
		add_property("useSDCC",&CCNetwork::GetSDCC,&CCNetwork::SetUseSDCC).
		add_property("varyActFunc", &CCNetwork::GetVaryActFunc, &CCNetwork::SetVaryActFunc).
		add_property("cloneOuts", &CCNetwork::GetCloneOuts, &CCNetwork::SetCloneOuts).
		add_property("defaultCandType", &CCNetwork::GetDefaultCandType, &CCNetwork::SetDefaultCandType)
		;

}
