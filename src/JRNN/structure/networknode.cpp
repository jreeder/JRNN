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

	hashedDoubleMap NetworkNode::GetPrimes(hashedDoubleMap outGradients) {
		localGradients = outGradients;
		calcGradients();
		//calculate the partial derivatives.
		//first index is input nodes
		//second index in output nodes.
		//I might be able to skip doing it this way if I setup the change value as the local
		//gradient at the output then propagate that back to the inputs. At which point I should be able
		//to use the primes there to calculate the change in input weights into the subnetwork.
		//If that doesn't work then I have to do something like the autodiff to get the partial derivatives.
		//for each output in regards to each output.
		return inputGradients;
	}

	void calcGradients() {
		//Backprop the gradients through the network.
	}
}



