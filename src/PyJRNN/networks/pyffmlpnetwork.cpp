/********************************************************************
	created:	2012/08/04
	filename: 	pyffmlpnetwork.h
	author:		jreeder
	
	purpose:	Mapping the ffmlpnetwork class. 
*********************************************************************/

#include <boost/python.hpp>
#include <networks/FFMLPNetwork.h>

using namespace JRNN;
using namespace boost::python;

NetworkPtr (FFMLPNetwork::*FFMLPClonePtr)() = &FFMLPNetwork::Clone;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FFMLPNetwork_buildoverloads, Build, 3,6);

void exportFFMLPNetwork(){

	class_<FFMLPNetwork, FFMLPNetPtr, bases<Network> >("FFMLPNetwork").
		def("Create", &FFMLPNetwork::Create).staticmethod("Create").
		def("Clone", FFMLPClonePtr).
		def("Build", &FFMLPNetwork::Build, FFMLPNetwork_buildoverloads())
		;

}