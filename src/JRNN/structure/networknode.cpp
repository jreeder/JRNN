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
		throw std::exception("The method or operation is not implemented.");
	}

	double NetworkNode::GetPrime()
	{
		throw std::exception("The method or operation is not implemented.");
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
		throw std::exception("The method or operation is not implemented.");
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

	JRNN::matDouble NetworkNode::GetPrimes()
	{
		matDouble retPrimes;
		//calculate the partial derivatives.
		//first index is input nodes
		//second index in output nodes. 
		return retPrimes;
	}

}



