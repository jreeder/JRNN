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
		layers["hidden"]->BuildLayer<Sigmoid>();
		layers["out"]->BuildLayer<ASigmoid>();
		layers["bias"]->BuildLayer<Bias>();
		FullyConnect();
		SetLocked(true);
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
				connections.push_back(Connection::Connect(n,n2));
			}
		}

		BOOST_FOREACH(NodePtr n, hidNodes){
			BOOST_FOREACH(NodePtr n2, outNodes){
				connections.push_back(Connection::Connect(n,n2));
			}
		}

		BOOST_FOREACH(NodePtr n, biasNodes){
			BOOST_FOREACH(NodePtr n2, hidNodes){
				connections.push_back(Connection::Connect(n,n2));
			}
			BOOST_FOREACH(NodePtr n3, outNodes){
				connections.push_back(Connection::Connect(n,n3));
			}
		}
	}

}