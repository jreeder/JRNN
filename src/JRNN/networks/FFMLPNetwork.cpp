/* 
 * File:   FFMLPNetwork.cpp
 * Author: jreeder
 * 
 * Created on July 13, 2010
 */
#include "JRNN.h"
#include "networks/FFMLPNetwork.h"

namespace JRNN {
	//FFMLPBuilder::FFMLPBuilder( int numIn, int numHid, int numOut )
	//{
	//	this->numIn = numIn;
	//	this->numHid = numHid;
	//	this->numOut = numOut;
	//}

	FFMLPNetwork::FFMLPNetwork(){}

	FFMLPNetPtr FFMLPNetwork::Create()
	{
		FFMLPNetPtr np(new FFMLPNetwork());
		return np;
	}

	void FFMLPNetwork::Build(int numIn, int numHid, int numOut){
		this->numIn = numIn;
		this->numOut = numOut;
		this->numHid = numHid;
		this->numHidLayers = 1;
		layers.insert(LayerPair("input", Layer::CreateLayer(Layer::input,numIn,0,"input")));
		layers.insert(LayerPair("hidden", Layer::CreateLayer(Layer::hidden, numHid, 1,"hidden")));
		layers.insert(LayerPair("out", Layer::CreateLayer(Layer::out,numOut, 2,"out")));
		layers.insert(LayerPair("bias", Layer::CreateLayer(Layer::bias, 1, -1, "bias")));
		//np->layers["input"]->BuildLayer(Node::linear);
		//np->layers["hidden"]->BuildLayer(Node::sigmoid);
		//np->layers["out"]->BuildLayer(Node::sigmoid);
		//np->layers["bias"]->BuildLayer(Node::bias);
		layers["input"]->BuildLayer<Linear>();
		layers["hidden"]->BuildLayer<ASigmoid>();
		layers["out"]->BuildLayer<ASigmoid>();
		layers["bias"]->BuildLayer<Bias>();
		FullyConnect();
		SetLocked(true);
	}

	FFMLPNetPtr FFMLPNetwork::Clone( FFMLPNetPtr net){
		FFMLPNetPtr np(new FFMLPNetwork());
		
		//NetworkPtr newP(np);
		//NetworkPtr oldP(net);
		Network::Clone(np, net);
		//np->numHidLayers = net->numHidLayers;
		//np->numIn = net->numIn;
		//np->numOut = net->numOut;
		//BOOST_FOREACH(LayerPair newLP, net->layers){
		//	np->layers.insert(LayerPair(newLP.first, Layer::Clone(newLP.second)));
		//}
		//BOOST_FOREACH(LayerPair lp, net->layers){
		//	LayerPtr layer = np->layers[lp.first];
		//	if (lp.second->GetPrevLayer() != 0){
		//		layer->SetPrevLayer(np->layers[lp.second->GetPrevLayer()->GetName()]);
		//	}
		//	if (lp.second->GetNextLayer() != 0){
		//		layer->SetNextLayer(np->layers[lp.second->GetNextLayer()->GetName()]);
		//	}
		//}
		//BOOST_FOREACH(ConPair conP, net->connections){
		//	ConPtr con = conP.second;
		//	np->AddConnection(Connection::Clone(con, np->GetNode(con->GetInNodeName()), np->GetNode(con->GetOutNodeName())));
		//}
		np->numHid = net->numHid;
		return np;
	}

	JRNN::NetworkPtr FFMLPNetwork::Clone()
	{
		FFMLPNetPtr oldP = FFMLPSharedFromThis::shared_from_this();
		return FFMLPNetwork::Clone(oldP);
	}

	void FFMLPNetwork::FullyConnect()
	{
		NodeList inputNodes = layers["input"]->GetNodes();
		NodeList hidNodes = layers["hidden"]->GetNodes();
		NodeList outNodes = layers["out"]->GetNodes();
		NodeList biasNodes = layers["bias"]->GetNodes();

		layers["input"]->SetNextLayer(layers["hidden"]);
		layers["hidden"]->SetPrevLayer(layers["input"]);
		layers["hidden"]->SetNextLayer(layers["out"]);
		layers["out"]->SetPrevLayer(layers["hidden"]);

		Connection::SetRandomSeed();

		BOOST_FOREACH(NodePtr n, inputNodes){
			BOOST_FOREACH(NodePtr n2, hidNodes){
				AddConnection(Connection::Connect(n,n2));
			}
		}

		BOOST_FOREACH(NodePtr n, hidNodes){
			BOOST_FOREACH(NodePtr n2, outNodes){
				AddConnection(Connection::Connect(n,n2));
			}
		}

		BOOST_FOREACH(NodePtr n, biasNodes){
			BOOST_FOREACH(NodePtr n2, hidNodes){
				AddConnection(Connection::Connect(n,n2));
			}
			BOOST_FOREACH(NodePtr n3, outNodes){
				AddConnection(Connection::Connect(n,n3));
			}
		}
	}

}