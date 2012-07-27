/********************************************************************
	created:	26/07/2012 16:37
	filename: 	network.cpp
	author:		jreeder
	
	purpose:	This will wrap the network class for python
*********************************************************************/

#include <boost/python.hpp>
#include <structure/network.h>

using namespace boost::python;

void exportNetworks(){
	object module(handle<>(borrowed(PyImport_AddModule("PyJRNN.networks"))));
	scope().attr("networks") = module;
	scope module_scope = module;

	class_<JRNN::Network, JRNN::NetworkPtr>("Network");

}