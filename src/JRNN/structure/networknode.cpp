/********************************************************************
	created:	2013/03/28
	created:	28:3:2013   3:07
	filename: 	networknode.cpp
	author:		jreeder
	
	purpose:	Implementation for network node
*********************************************************************/

#include "JRNN.h"
#include "structure/networknode.h"

namespace JRNN {
	double NetworkNode::GetOut()
	{
		//throw std::exception("The method or operation is not implemented.");
		assert(0);
		return 0;
	}

	double NetworkNode::GetPrime()
	{
		//throw std::exception("The method or operation is not implemented.");
		assert(0);
		return 0;
	}

	ConList& NetworkNode::GetConnections( conType type )
	{
		return Node::GetConnections(type);
	}

	string NetworkNode::GetActFuncType()
	{
		return "IntNet";
	}

	bool NetworkNode::AddConnection( conType type, ConPtr newCon )
	{
		//throw std::exception("The method or operation is not implemented.");
		assert(0);
		return false;
	}

	void NetworkNode::Activate()
	{
		LayerMap netLayers = intNet->GetLayers();
		netLayers["input"]->Activate();
		netLayers["bias"]->Activate();
		intNet->Activate(netLayers["input"]->GetNextLayer());
		intNet->SetOutputs(netLayers["out"]->GetOutput());
	}

	vecDouble NetworkNode::GetOuts()
	{
		return this->intNet->GetOutputs();
	}

	JRNN::hashedDoubleMap NetworkNode::GetPrimes()
	{
		//This is done by propagating down the network while setting the gradients of each node 
		//from each input. Start with the bias node and inputs being set, then propagate down the
		//network to the outputs. 
		calcGradients();
		
		return inputGradients;
	}

	void calcGradients() {
		//Backprop the gradients through the network.
	}
}



