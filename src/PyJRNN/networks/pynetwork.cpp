/********************************************************************
	created:	26/07/2012 16:37
	filename: 	network.cpp
	author:		jreeder
	
	purpose:	This will wrap the network class for python
*********************************************************************/

#include <boost/python.hpp>
#include "networks/pynetworkshelper.h"


using namespace boost::python;
using namespace JRNN;

void exportCCNetwork();
void exportFFMLPNetwork();
void exportRevCCNetwork();
void exportKBCCNetwork();

void exportNetworks(){
	

	NetworkPtr (Network::*ClonePtr)() = &Network::Clone;
	void (Network::*ActivatePtr)(vecDouble) = &Network::Activate;

	class_<NetWrap, NetworkPtr, boost::noncopyable>("Network",no_init).
		def("Create",&Network::Create).staticmethod("Create").
		add_property("numIn", &Network::GetNumIn, &Network::SetNumIn).
		add_property("numOut", &Network::GetNumOut, &Network::SetNumOut).
		add_property("numHidLayers", &Network::GetNumHidLayers, &Network::SetNumHidLayers).
		def("SetScaleAndOffset",&Network::SetScaleAndOffset).
		def("Clone", ClonePtr, &NetWrap::default_Clone).
		def("Activate", ActivatePtr, &NetWrap::default_Activate).
		def("GetOutputs", &Network::GetOutputs, &NetWrap::default_GetOutputs).
		def("PrintConnections", &Network::PrintConnections).
		def("Reset", &Network::Reset, &NetWrap::default_Reset).
		def("Compare", &Network::Compare).
		add_property("type", &Network::GetType)
		;

	exportCCNetwork();

	exportFFMLPNetwork();

	exportRevCCNetwork();

	exportKBCCNetwork();
}