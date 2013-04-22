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

	JRNN::hashedVecDoubleMap NetworkNode::GetPrimes()
	{
		//This is done by propagating down the network while setting the gradients of each node 
		//from each input. Start with the bias node and inputs being set, then propagate down the
		//network to the outputs. 
		calcGradients();
		
		return localGradients;
	}

	void NetworkNode::calcGradients() {
		//Backprop the gradients through the network.
		LayerMap netLayers = this->intNet->GetLayers();
		NodeList biasNodes = netLayers["bias"]->GetNodes();
		NodeList inputNodes = netLayers["input"]->GetNodes();
		//int numInputs = inputNodes.size();
		localGradients = hashedVecDoubleMap(numInputs);
		BOOST_FOREACH(NodePtr node, biasNodes){
			localGradients[node->GetName()];
		}
		for (int i = 0; i < inputNodes.size(); i++){
			localGradients[inputNodes[i]->GetName()][i] = inputNodes[i]->GetPrime();
		}

		cascadeGradients(netLayers["input"]->GetNextLayer());
	}

	void NetworkNode::cascadeGradients( LayerPtr layer )
	{
		LayerPtr curLayer = layer;
		while(curLayer.get() != 0){
			BOOST_FOREACH(NodePtr node, curLayer->GetNodes()){
				vecDouble inputSums = vecDouble(numInputs);
				BOOST_FOREACH(ConPtr con, node->GetConnections(IN)){
					inputSums += localGradients[con->GetInNodeName()] * con->GetWeight();
				}
				localGradients[node->GetName()] = inputSums * node->GetPrime();
			}
			if (curLayer->HasNextL()){
				curLayer = curLayer->GetNextLayer();
			}
			else {
				curLayer.reset();
			}
		}
	}

	NetworkNode::NetworkNode(int inHeight, string nodeName, NetworkPtr net ) : Node(inHeight, nodeName)
	{
		intNet = net;
		numInputs = net->GetNumIn();
		numOutputs = net->GetNumOut();
		localGradients.clear();
		intNet->SetNetPrefix(nodeName);
	}

	NetworkPtr NetworkNode::GetIntNet() const
	{
		return intNet;
	}

	void NetworkNode::SetIntNet( NetworkPtr val )
	{
		intNet = val;
		numInputs = val->GetNumIn();
		numOutputs = val->GetNumOut();
		localGradients.clear();
		intNet->SetNetPrefix(this->GetName());
	}

}



