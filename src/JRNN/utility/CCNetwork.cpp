/* 
 * File:   CCNetwork.cpp
 * Author: jreeder
 * 
 * Created on July 13, 2010
 */

#include "JRNN.h"
#include "utility/CCNetwork.h"

namespace JRNN {

	CCNetwork::CCNetwork(){}

	CCNetwork::CCNetwork( int numIn, int numOut )
	{
		this->numIn = numIn;
		this->numOut = numOut;
	}

	//CCNetwork::CCNetwork( NetworkPtr network )
	//{
	//	np = network;
	//	this->numIn = np->GetNumIn();
	//	this->numOut = np->GetNumOut();
	//}

	CCNetwork::~CCNetwork()
	{
		tmpConnections.clear();
	}

	CCNetworkPtr CCNetwork::Create()
	{
		CCNetworkPtr np(new CCNetwork());
		return np;
	}

	void CCNetwork::Build(int numIn, int numOut){
		this->numIn	= numIn;
		this->numOut = numOut;
		this->numHidLayers = 0;
		layers.insert(std::pair<std::string, LayerPtr>("input", Layer::CreateLayer(Layer::input, numIn,0,"input")));
		layers.insert(std::pair<std::string, LayerPtr>("out", Layer::CreateLayer(Layer::out, numOut, 1, "out")));
		layers.insert(std::pair<std::string, LayerPtr>("bias", Layer::CreateLayer(Layer::bias, 1, -1, "bias")));
		layers.insert(std::pair<std::string, LayerPtr>("cand", LayerPtr()));
		//layers["input"]->BuildLayer(Node::linear);
		//layers["hidden"]->BuildLayer(Node::sigmoid);
		//layers["bias"]->BuildLayer(Node::bias);
		layers["input"]->BuildLayer<Linear>();
		layers["out"]->BuildLayer<ASigmoid>();
		layers["bias"]->BuildLayer<Bias>();
		FullyConnect();
	}

	void CCNetwork::FullyConnect()
	{
		NodeList inputNodes = layers["input"]->GetNodes();
		NodeList outNodes = layers["out"]->GetNodes();
		NodeList biasNodes = layers["bias"]->GetNodes();

		layers["input"]->SetNextLayer(layers["out"]);
		layers["out"]->SetPrevLayer(layers["input"]);
		Connection::SetRandomSeed();

		BOOST_FOREACH(NodePtr n, inputNodes){
			BOOST_FOREACH(NodePtr n2,outNodes){
				connections.push_back(Connection::Connect(n,n2));
			}
		}

		BOOST_FOREACH(NodePtr n, biasNodes){
			BOOST_FOREACH(NodePtr n2, outNodes){
				connections.push_back(Connection::Connect(n,n2));
			}
		}
	}

	//NetworkPtr CCNetwork::GetNetwork()
	//{
	//	return np;
	//}

	//void CCNetwork::SetNetwork( NetworkPtr net )
	//{
	//	this->np = net;
	//}

	void CCNetwork::CreateCandLayer(int numCand)
	{
		//TODO allow for varied node types-- for now it's homogeneous
		LayerPtr out = layers["out"];
		int tmpHeight = out->GetHeight();
		candLayer = Layer::CreateLayer(Layer::hidden,numCand,tmpHeight,"cand");
		candLayer->BuildLayer<ASigmoid>();
		layers["cand"] = candLayer;
		candLayer->SetPrevLayer(out->GetPrevLayer());
		CandFullyConnectBack(candLayer);
	}

	void CCNetwork::InstallCandidate(NodePtr node)
	{

	}

	void CCNetwork::CandFullyConnectBack( LayerPtr layer )
	{
		NodeList layerNodes = layer->GetNodes();
		LayerPtr prevLayer = layer->GetPrevLayer();
		
		Connection::SetRandomSeed();
		while(prevLayer){
			NodeList prevNodes = prevLayer->GetNodes();
			BOOST_FOREACH(NodePtr n, prevNodes){
				BOOST_FOREACH(NodePtr n2, layerNodes){
					tmpConnections.push_back(Connection::Connect(n,n2));
				}
			}
			prevLayer = prevLayer->GetPrevLayer();
		}

		NodeList biasNodes = layers["bias"]->GetNodes();
		BOOST_FOREACH(NodePtr n, biasNodes){
			BOOST_FOREACH(NodePtr n2, layerNodes){
				tmpConnections.push_back(Connection::Connect(n,n2));
			}
		}

	}

	void CCNetwork::AddHiddenLayer()
	{

	}

	void CCNetwork::FullyConnectOut( LayerPtr layer )
	{
		NodeList layerNodes = layer->GetNodes();
		NodeList outNodes = layers["out"]->GetNodes();
		
		Connection::SetRandomSeed();
		BOOST_FOREACH(NodePtr n, layerNodes){
			BOOST_FOREACH(NodePtr n2, outNodes){
				connections.push_back(Connection::Connect(n,n2));
			}
		}
	}

}