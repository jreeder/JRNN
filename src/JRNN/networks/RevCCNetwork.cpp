/********************************************************************
	created:	2012/03/20
	created:	20:3:2012   21:40
	filename: 	RevCCNetwork.cpp
	author:		jreeder
	
	purpose:	RevCCNetwork class. Meant to add the reverberating 
				process to the CC network. This will be the building block
				of the life long learning system.
*********************************************************************/
#include "JRNN.h"
#include "networks/RevCCNetwork.h"

namespace JRNN {
	
	RevCCNetwork::RevCCNetwork(){}

	RevCCNetwork::RevCCNetwork(int numIn, int numOut){
		this->numIn = numIn;
		this->numOut = numOut;
	}

	RevCCNetwork::~RevCCNetwork(){}

	RevCCNetworkPtr RevCCNetwork::Create() {
		RevCCNetworkPtr np(new RevCCNetwork());
		return np;
	}

	RevCCNetworkPtr RevCCNetwork::Clone(RevCCNetworkPtr net){
		RevCCNetworkPtr revccnet(new RevCCNetwork());
		Network::Clone(revccnet, net);
		
		revccnet->candLayer = revccnet->layers["cand"];
		BOOST_FOREACH(LayerPair lp, revccnet->layers){
			if (lp.second->GetType() == Layer::hidden){
				revccnet->hiddenLayers.push_back(lp.second);
			}
		}
		revccnet->numUnits = net->numUnits;
		revccnet->autoAssocLayer = revccnet->layers["autoassoc"];
		return revccnet;
	}

	NetworkPtr RevCCNetwork::Clone() {
		RevCCNetworkPtr oldP = RevCCSharedFromThis::shared_from_this();
		return RevCCNetwork::Clone(oldP);
	}

	const LayerPtr RevCCNetwork::GetAutoAssocLayer(){
		return autoAssocLayer;
	}

	void RevCCNetwork::Build(int numIn, int numOut, bool cloneouts /*= false*/){
		CCNetwork::Build(numIn, numOut, cloneouts);
		layers.insert(LayerPair("autoassoc", Layer::CreateLayer(Layer::out, numIn,-1,"autoassoc")));
		layers["autoassoc"]->BuildLayer<ASigmoid>();
		autoAssocLayer = layers["autoassoc"];
	}

	void RevCCNetwork::InstallCandidate(NodePtr node, vecDouble outWeights /* = vecDouble */, vecDouble assocWeights /*=vecDouble*/){
		LayerPtr lp = AddHiddenLayer();
		lp->AddNode(node);
		candLayer->RemoveNode(node);
		BOOST_FOREACH(ConPtr con, node->GetConnections(IN)){
			AddConnection(con);
		}
		if (outWeights.size() == 0){
			FullyConnectOut(lp);
		}
		else {
			FullyConnectOut(lp, outWeights);	
		}
		if (assocWeights.size() == 0){
			FullyConnectAutoAssoc(lp);
		}
		else {
			FullyConnectAutoAssoc(lp, assocWeights);
		}
		hiddenLayers.push_back(lp);
		numUnits++;
	}

	void RevCCNetwork::Reset(){
		autoAssocLayer.reset();
		CCNetwork::Reset();
	}

	void RevCCNetwork::FullyConnectAutoAssoc(LayerPtr layer){
		NodeList layerNodes = layer->GetNodes();
		NodeList assocNodes = layers["autoassoc"]->GetNodes();

		Connection::SetRandomSeed();
		BOOST_FOREACH(NodePtr n, layerNodes){
			if (cloneOuts){
				double conWeight = Connection::GetRandWeight(this->conScale, this->conOffset);
				BOOST_FOREACH(NodePtr n2, assocNodes){
					AddConnection(Connect(n,n2,conWeight));
				}
			}
			else {
				BOOST_FOREACH(NodePtr n2, assocNodes){
					AddConnection(Connect(n,n2));
				}
			}
		}
	}

	void RevCCNetwork::FullyConnectAutoAssoc(LayerPtr layer, vecDouble outWeights){
		NodeList layerNodes = layer->GetNodes();
		NodeList assocNodes = layers["autoassoc"]->GetNodes();

		BOOST_FOREACH(NodePtr n, layerNodes){
			for (unsigned int i = 0; i < assocNodes.size(); i++){
				AddConnection(Connect(n,assocNodes[i], outWeights[i]));
			}
		}
	}
}