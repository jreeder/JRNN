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

	const string KBCCNetwork::Type = "KBCC";

	JRNN::NetworkPtr KBCCNetwork::Clone()
	{
		KBCCNetworkPtr oldP = KBCCSharedFromThis::shared_from_This();
		return KBCCNetwork::Clone(oldP);
	}

	JRNN::KBCCNetworkPtr KBCCNetwork::Create()
	{
		KBCCNetworkPtr np(new KBCCNetwork());
		return np;
	}

	JRNN::KBCCNetworkPtr KBCCNetwork::Clone( KBCCNetworkPtr net )
	{
		KBCCNetworkPtr kbccnet(new KBCCNetwork());
		CCNetwork::Clone(kbccnet, net);
		BOOST_FOREACH(NodePtr node, net->CandSubNetNodes){
			kbccnet->CandSubNetNodes.push_back(dynamic_pointer_cast<NetworkNode>(kbccnet->GetNode(node->GetName())));
		}
		BOOST_FOREACH(NodePtr node, net->SubNetworkNodes){
			kbccnet->SubNetworkNodes.push_back(dynamic_pointer_cast<NetworkNode>(kbccnet->GetNode(node->GetName())));
		}
		return kbccnet;
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
			if(defaultCandType == Sigmoid::_type){
				candLayer->BuildLayer<Sigmoid>();
			}
			else if(defaultCandType == Gaussian::_type){
				candLayer->BuildLayer<Gaussian>();
			}
			else if(defaultCandType == Linear::_type){
				candLayer->BuildLayer<Linear>();
			}
			else {
				candLayer->BuildLayer<ASigmoid>();
			}
		} 
		else {
			BuildVariedLayer(candLayer, numCand);
		}

		candLayer->SetPrevLayer(out->GetPrevLayer());
		CandFullyConnectBack(candLayer);
		CandSubNetNodes.clear();
		BOOST_FOREACH(NetPtrPair net, candNets){
			NetworkNodePtr newNetNode = NetworkNode::Create(tmpHeight, "tmpName");
			candLayer->AddNode(newNetNode);
			newNetNode->SetIntNet(net.second->Clone(), net.first);
			CandNetFullyConnectBack(newNetNode, true, false);
			CandSubNetNodes.push_back(newNetNode);
			for (int i = 0; i < numCopies; i++)
			{
				NetworkNodePtr newNetNode = NetworkNode::Create(tmpHeight, "tmpName");
				candLayer->AddNode(newNetNode);
				bool reduceHeight = i % 2 == 0 ? false : true;
				newNetNode->SetIntNet(net.second->Clone(), net.first);
				CandNetFullyConnectBack(newNetNode, false, reduceHeight);
				CandSubNetNodes.push_back(newNetNode);
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
				if (n->GetActFuncType() != NetworkNode::ActType){
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
				else {
					NodeList prevOutNodes = dynamic_pointer_cast<NetworkNode>(n)->GetOutputNodes();
					BOOST_FOREACH(NodePtr n1, prevOutNodes){
						BOOST_FOREACH(NodePtr n2, layerNodes){
							if(!onlyDirect){
								candConnections.push_back(Connect(n1,n2));
								conMade = true;
							}
							else {
								if (n2->GetName().find(n->GetName()) != string::npos){
									candConnections.push_back(Connect(n1,n2,1.0));
									conMade = true;
								}
							}
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

	void KBCCNetwork::CandFullyConnectBack( LayerPtr layer )
	{
		NodeList layerNodes = layer->GetNodes();
		LayerPtr prevLayer = layer->GetPrevLayer();

		Connection::SetRandomSeed();

		if (useSDCC && prevLayer->GetType() != Layer::input){ //Use the sibling/descendant mechanism to reduce network depth
			NodeList prevNodes = prevLayer->GetNodes();
			int count = 0;
			int tmpHeight = layer->GetHeight();
			BOOST_FOREACH(NodePtr n, prevNodes){
				if (n->GetActFuncType() != NetworkNode::ActType)
				{
					BOOST_FOREACH(NodePtr n2, layerNodes){
						if (count++ % 2 == 0)
						{
							//Only connect half of the candidates to the last layer as descendants.
							candConnections.push_back(Connect(n,n2));
						}
						else {
							n2->SetHeight(tmpHeight - 1); //Don't connect and reduce the height This will be a sibling node;
						}
					}
				} 
				else
				{
					NodeList prevOutNodes = dynamic_pointer_cast<NetworkNode>(n)->GetOutputNodes();
					BOOST_FOREACH(NodePtr n2, layerNodes){
						if (count++ % 2 == 0){
							BOOST_FOREACH(NodePtr n1, prevOutNodes){
								candConnections.push_back(Connect(n1,n2));
							}
						}
						else {
							n2->SetHeight(tmpHeight - 1);
						}
					}
					
				}
			}
			prevLayer = prevLayer->GetPrevLayer();
		}

		while(prevLayer){
			NodeList prevNodes = prevLayer->GetNodes();
			BOOST_FOREACH(NodePtr n, prevNodes){
				if (n->GetActFuncType() != NetworkNode::ActType){
					BOOST_FOREACH(NodePtr n2, layerNodes){
						candConnections.push_back(Connect(n,n2));
					}
				}
				else {
					NodeList prevOutNodes = dynamic_pointer_cast<NetworkNode>(n)->GetOutputNodes();
					BOOST_FOREACH(NodePtr n1, prevOutNodes){
						BOOST_FOREACH(NodePtr n2, layerNodes){
							candConnections.push_back(Connect(n1,n2));
						}
					}
				}
			}
			prevLayer = prevLayer->GetPrevLayer();
		}

		NodeList biasNodes = layers["bias"]->GetNodes();
		BOOST_FOREACH(NodePtr n, biasNodes){
			BOOST_FOREACH(NodePtr n2, layerNodes){
				candConnections.push_back(Connect(n,n2));
			}
		}

	}

	void KBCCNetwork::InstallCandidate( NodePtr node, vecDouble outWeights /*= vecDouble(0 ) */ )
	{
		CCNetwork::InstallCandidate(node, outWeights);
	}

	void KBCCNetwork::InstallCandidate( NodePtr node, hashedVecDoubleMap outWeights /*= hashedVecDoubleMap()*/)
	{
		//Have to do this backwards from normal because the name changes when it's added. to the new layer. 
		NodeList candOutNodes = dynamic_pointer_cast<NetworkNode>(node)->GetOutputNodes();
		BOOST_FOREACH(NodePtr oNode, candOutNodes){
			vecDouble tmpOutWeights = outWeights[oNode->GetName()];
			if (tmpOutWeights.size() == 0){

				CandConnectOut(oNode);
				//FullyConnectOut(lp);
			}
			else {

				CandConnectOut(oNode, tmpOutWeights);
				//FullyConnectOut(lp, outWeights);
			}
		}

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
		

		if (newLayer)
		{
			this->currentLayer = lp;
			hiddenLayers.push_back(lp);
		}
		numUnits++;
		SubNetworkNodes.push_back(dynamic_pointer_cast<NetworkNode>(node));
	}

	void KBCCNetwork::Build( int numIn, int numOut, bool cloneouts /*= false*/, bool useSDCC /*= false*/, bool varyActFunc /*= false*/, string outNodeType /*= ASigmoid::_type */ )
	{
		CCNetwork::Build(numIn, numOut, cloneouts, useSDCC, varyActFunc, outNodeType);
	}

	void KBCCNetwork::Reset()
	{
		SubNetworkNodes.clear();
		CandSubNetNodes.clear();
		CCNetwork::Reset();
	}

	void KBCCNetwork::FullyConnectOut( LayerPtr layer )
	{
		CCNetwork::FullyConnectOut(layer);
	}

	void KBCCNetwork::FullyConnectOut( LayerPtr layer, vecDouble outWeights )
	{
		CCNetwork::FullyConnectOut(layer);
	}

	void KBCCNetwork::RemoveUnConnectedNodes()
	{
		CCNetwork::RemoveUnConnectedNodes();
	}

	void KBCCNetwork::RemoveHiddenLayer( LayerPtr layer )
	{
		CCNetwork::RemoveHiddenLayer(layer);
	}

	void KBCCNetwork::CandConnectOut( NodePtr node, vecDouble outWeights /*= vecDouble(0 ) */ )
	{
		CCNetwork::CandConnectOut(node, outWeights);
	}

	JRNN::ConPtr KBCCNetwork::Connect( NodePtr n1, NodePtr n2 )
	{
		return Network::Connect(n1,n2);
	}

	JRNN::ConPtr KBCCNetwork::Connect( NodePtr n1, NodePtr n2, double conweight )
	{
		return Network::Connect(n1,n2,conweight);
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

	KBCCNetwork::KBCCNetwork(){}

	KBCCNetwork::KBCCNetwork( int numIn, int numOut ) : CCNetwork(numIn, numOut) 
	{}

	NetworkNodeList& KBCCNetwork::GetSubNetworkNodes()
	{
		return SubNetworkNodes;
	}

}