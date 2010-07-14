/* 
 * File:   FFMLPBuilder.cpp
 * Author: jreeder
 * 
 * Created on July 13, 2010
 */
#include "JRNN.h"
#include "utility/FFMLPBuilder.h"

namespace JRNN {
	FFMLPBuilder::FFMLPBuilder( int numIn, int numHid, int numOut )
	{
		this->numIn = numIn;
		this->numHid = numHid;
		this->numOut = numOut;
	}

	NetworkPtr FFMLPBuilder::Create()
	{
		NetworkPtr np(new Network());
		np->SetNumIn(numIn);
		np->SetNumOut(numOut);
		np->SetNumHidLayers(1);
		np->layers.insert(std::pair<std::string, LayerPtr>("input", Layer::CreateLayer(Layer::input,numIn,0,"input")));
		np->layers.insert(std::pair<std::string, LayerPtr>("hidden", Layer::CreateLayer(Layer::hidden, numHid, 1,"hidden")));
		np->layers.insert(std::pair<std::string, LayerPtr>("out", Layer::CreateLayer(Layer::out,numOut, 2,"out")));
		np->layers.insert(std::pair<std::string, LayerPtr>("bias", Layer::CreateLayer(Layer::bias, 1, -1, "bias")));
		//np->layers["input"]->BuildLayer(Node::linear);
		//np->layers["hidden"]->BuildLayer(Node::sigmoid);
		//np->layers["out"]->BuildLayer(Node::sigmoid);
		//np->layers["bias"]->BuildLayer(Node::bias);
		np->layers["input"]->BuildLayer<Linear>();
		np->layers["hidden"]->BuildLayer<Sigmoid>();
		np->layers["out"]->BuildLayer<ASigmoid>();
		np->layers["bias"]->BuildLayer<Bias>();
		FullyConnect(np);
		np->SetLocked(true);
		return np;
	}

	void FFMLPBuilder::FullyConnect(NetworkPtr np)
	{
		NodeList inputNodes = np->layers["input"]->GetNodes();
		NodeList hidNodes = np->layers["hidden"]->GetNodes();
		NodeList outNodes = np->layers["out"]->GetNodes();
		NodeList biasNodes = np->layers["bias"]->GetNodes();

		np->layers["input"]->SetNextLayer(np->layers["hidden"]);
		np->layers["hidden"]->SetPrevLayer(np->layers["input"]);
		np->layers["hidden"]->SetNextLayer(np->layers["out"]);
		np->layers["out"]->SetPrevLayer(np->layers["hidden"]);

		Connection::SetRandomSeed();

		BOOST_FOREACH(NodePtr n, inputNodes){
			BOOST_FOREACH(NodePtr n2, hidNodes){
				np->connections.push_back(Connection::Connect(n,n2));
			}
		}

		BOOST_FOREACH(NodePtr n, hidNodes){
			BOOST_FOREACH(NodePtr n2, outNodes){
				np->connections.push_back(Connection::Connect(n,n2));
			}
		}

		BOOST_FOREACH(NodePtr n, biasNodes){
			BOOST_FOREACH(NodePtr n2, hidNodes){
				np->connections.push_back(Connection::Connect(n,n2));
			}
			BOOST_FOREACH(NodePtr n3, outNodes){
				np->connections.push_back(Connection::Connect(n,n3));
			}
		}
	}

}