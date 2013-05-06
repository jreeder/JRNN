/********************************************************************
	created:	2013/05/05
	created:	5:5:2013   5:19
	filename: 	KBCCNetwork.cpp
	author:		jreeder
	
	purpose:	Network that can hold other networks. 
*********************************************************************/

#include "JRNN.h"
#include "networks/KBCCNetwork.h"

namespace JRNN {


	JRNN::NetworkPtr KBCCNetwork::Clone()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::CreateCandLayer( int numCand, NetPtrList candNets /*= NetPtrList()*/, int numCopies /*= 0*/ )
	{
		LayerPtr out = layers["out"];
		int tmpHeight = out->GetHeight();
		candLayer->Clear();
		candConnections.clear();
		candLayer->SetLayerSize(numCand);
		candLayer->SetHeight(tmpHeight);
		if(!varyActFunc){
			candLayer->BuildLayer<ASigmoid>();
		} 
		else {
			BuildVariedLayer(candLayer);
		}

		candLayer->SetPrevLayer(out->GetPrevLayer());
		CandFullyConnectBack(candLayer);
		BOOST_FOREACH(NetworkPtr net, candNets){
			NetworkNodePtr newNetNode = NetworkNode::CreateNetworkNode(tmpHeight, "tmpName");
			candLayer->AddNode(newNetNode);
			newNetNode->SetIntNet(net->Clone());
			CandNetFullyConnectBack(newNetNode, true, false);
			CandSubNetNodes.push_back(newNetNode);
			for (int i = 0; i < numCopies; i++)
			{
				NetworkNodePtr newNetNode = NetworkNode::CreateNetworkNode(tmpHeight, "tmpName");
				candLayer->AddNode(newNetNode);
				bool reduceHeight = i % 2 == 0 ? false : true;
				newNetNode->SetIntNet(net->Clone());
				CandNetFullyConnectBack(newNetNode, false, reduceHeight);
			}
		}
	}

	void KBCCNetwork::CandNetFullyConnectBack( NetworkNodePtr newNetNode, bool onlyDirect, bool reduceHeight )
	{
		NodeList layerNodes = newNetNode->GetInputNodes();
		LayerPtr prevLayer = candLayer->GetPrevLayer();
		Connection::SetRandomSeed();

		if (useSDCC && reduceHeight && prevLayer->GetType() != Layer::input) {
			newNetNode->SetHeight(newNetNode->GetHeight()-1);
			prevLayer = prevLayer->GetPrevLayer();
		}

		while(prevLayer){
			NodeList prevNodes = prevLayer->GetNodes();
			bool conMade = false;
			BOOST_FOREACH(NodePtr n, prevNodes){
				BOOST_FOREACH(NodePtr n2, layerNodes){
					if(!onlyDirect){
						candConnections.push_back(Connect(n,n2));
						conMade = true;
					}
					else {
						if (n2->GetName().find(n->GetName()) != string::npos){
							candConnections.push_back(Connect(n,n2,1.0));
							conMade = true;
						}
					}
				}
			}
			if (!conMade)
			{
				newNetNode->SetHeight(newNetNode->GetHeight()-1);
			}
			prevLayer = prevLayer->GetPrevLayer();
		}

		if(!onlyDirect){
			NodeList biasNodes = layers["bias"]->GetNodes();
			BOOST_FOREACH(NodePtr n, biasNodes){
				BOOST_FOREACH(NodePtr n2, layerNodes){
					candConnections.push_back(Connect(n,n2));
				}
			}
		}
	}

	void KBCCNetwork::InstallCandidate( NodePtr node, vecDouble outWeights /*= vecDouble(0 ) */ )
	{
		CCNetwork::InstallCandidate(node, outWeights);
	}

	void KBCCNetwork::InstallCandidate( NodePtr node, hashedVecDoubleMap outWeights = /*hashedVecDoubleMap()*/)
	{
		bool newLayer = node->GetHeight() == currentLayer->GetHeight() ? false : true;

		LayerPtr lp;

		if (newLayer){
			lp  = AddHiddenLayer();
		}
		else {
			lp = currentLayer;
		}

		lp->AddNode(node);
		candLayer->RemoveNode(node);
		BOOST_FOREACH(ConPtr con, node->GetConnections(IN)){
			AddConnection(con);
		}
		NodeList candOutNodes = dynamic_pointer_cast<NetworkNode>(node)->GetOutputNodes();
		BOOST_FOREACH(NodePtr oNode, candOutNodes){
			vecDouble tmpOutWeights = outWeights[oNode->GetName()];
			if (tmpOutWeights.size() == 0){

				CandConnectOut(node);
				//FullyConnectOut(lp);
			}
			else {

				CandConnectOut(node, tmpOutWeights);
				//FullyConnectOut(lp, outWeights);
			}
		}

		if (newLayer)
		{
			this->currentLayer = lp;
			hiddenLayers.push_back(lp);
		}
		numUnits++;
	}

	void KBCCNetwork::Build( int numIn, int numOut, bool cloneouts /*= false*/, bool useSDCC /*= false*/, bool varyActFunc /*= false */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::Reset()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::CandFullyConnectBack( LayerPtr layer )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::FullyConnectOut( LayerPtr layer )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::FullyConnectOut( LayerPtr layer, vecDouble outWeights )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::RemoveUnConnectedNodes()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::RemoveHiddenLayer( LayerPtr layer )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::CandConnectOut( NodePtr node, vecDouble outWeights /*= vecDouble(0 ) */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	JRNN::ConPtr KBCCNetwork::Connect( NodePtr n1, NodePtr n2 )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	JRNN::ConPtr KBCCNetwork::Connect( NodePtr n1, NodePtr n2, double conweight )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	JRNN::NodePtr KBCCNetwork::AppendNewInputNode()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::AppendInputNode( NodePtr node )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::InsertInputNode( NodePtr newNode, int pos )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	JRNN::NodePtr KBCCNetwork::InsertNewInputNode( int pos )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::ConnectToHiddenNodes( NodePtr node, conType cType /*= OUT */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::ConnectToHiddenNodes( NodeList nodes, conType cType /*= OUT */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

}