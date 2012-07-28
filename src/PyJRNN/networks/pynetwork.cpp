/********************************************************************
	created:	26/07/2012 16:37
	filename: 	network.cpp
	author:		jreeder
	
	purpose:	This will wrap the network class for python
*********************************************************************/

#include <boost/python.hpp>
#include <structure/network.h>

using namespace boost::python;
using namespace JRNN;

void exportNetworks(){
	object module(handle<>(borrowed(PyImport_AddModule("PyJRNN.networks"))));
	scope().attr("networks") = module;
	scope module_scope = module;

	class_<Network, NetworkPtr, boost::noncopyable>("Network",no_init).
		def("Create",&Network::Create).staticmethod("Create").
		add_property("numIn", &Network::GetNumIn, &Network::SetNumIn).
		add_property("numOut", &Network::GetNumOut, &Network::SetNumOut).
		add_property("numHidLayers", &Network::GetNumHidLayers, &Network::SetNumHidLayers).
		def("SetScaleAndOffset",&Network::SetScaleAndOffset)
		;

}