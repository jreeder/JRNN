/********************************************************************
	created:	26/07/2012 16:37
	filename: 	network.cpp
	author:		jreeder
	
	purpose:	This will wrap the network class for python
*********************************************************************/

#include <boost/python.hpp>
#include <structure/network.h>
#include <networks/CCNetwork.h>

using namespace boost::python;
using namespace JRNN;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CCNetwork_buildoverloads,Build,2,5);

void exportNetworks(){
	object module(handle<>(borrowed(PyImport_AddModule("PyJRNN.networks"))));
	scope().attr("networks") = module;
	scope module_scope = module;

	NetworkPtr (Network::*ClonePtr)() = &Network::Clone;
	void (Network::*ActivatePtr)(vecDouble) = &Network::Activate;

	struct NetWrap : Network, wrapper<Network>
	{
		NetworkPtr Clone(){
			if (override Clone = this->get_override("Clone"))
				return Clone();
			return Network::Clone();
		}
		
		NetworkPtr default_Clone() {return this->Network::Clone();}

		void Activate(vecDouble inputs) {
			if (override Activate = this->get_override("Activate")){
				Activate(inputs);
			}
			else {
				Network::Activate(inputs);
			}
		}

		void default_Activate(vecDouble inputs) {this->Network::Activate(inputs);} 

		vecDouble GetOutputs() {
			if (override GetOutputs = this->get_override("GetOutputs"))
				return GetOutputs();
			return Network::GetOutputs();
		}

		vecDouble default_GetOutputs() {return this->Network::GetOutputs();}
	};

	class_<NetWrap, NetworkPtr, boost::noncopyable>("Network",no_init).
		def("Create",&Network::Create).staticmethod("Create").
		add_property("numIn", &Network::GetNumIn, &Network::SetNumIn).
		add_property("numOut", &Network::GetNumOut, &Network::SetNumOut).
		add_property("numHidLayers", &Network::GetNumHidLayers, &Network::SetNumHidLayers).
		def("SetScaleAndOffset",&Network::SetScaleAndOffset).
		def("Clone", ClonePtr, &NetWrap::default_Clone).
		def("Activate", ActivatePtr, &NetWrap::default_Activate).
		def("GetOutputs", &Network::GetOutputs, &NetWrap::default_GetOutputs).
		def("PrintConnectsion", &Network::PrintConnections)
		;

	NetworkPtr (CCNetwork::*CCClonePtr)() = &CCNetwork::Clone;

	class_<CCNetwork, CCNetworkPtr, bases<NetWrap> >("CCNetwork").
		def("Create", &CCNetwork::Create).staticmethod("Create").
		def("Clone", CCClonePtr).
		def("Build", &CCNetwork::Build, CCNetwork_buildoverloads());
		;

}