/* 
 * File:   CCBuilder.cpp
 * Author: jreeder
 * 
 * Created on July 13, 2010
 */

#include "JRNN.h"
#include "utility/CCBuilder.h"

namespace JRNN {


	CCBuilder::CCBuilder( int numIn, int numOut )
	{
		this->numIn = numIn;
		this->numOut = numOut;
	}

	CCBuilder::CCBuilder( NetworkPtr network )
	{
		np = network;
		this->numIn = np->GetNumIn();
		this->numOut = np->GetNumOut();
	}

	CCBuilder::~CCBuilder()
	{
		tmpConnections.clear();
	}

	JRNN::NetworkPtr CCBuilder::Create()
	{
		NetworkPtr np(new Network());
		np->SetNumIn(numIn);
		np->SetNumOut(numOut);
		np->SetNumHidLayers(0);
		np->layers.insert(std::pair<std::string, LayerPtr>("input", Layer::CreateLayer(Layer::input,numIn,0,"input")));
		np->layers.insert(std::pair<std::string, LayerPtr>("out", Layer::CreateLayer(Layer::out, numOut, 1, "out")));
		np->layers.insert(std::pair<std::string, LayerPtr>("bias", Layer::CreateLayer(Layer::bias, 1, -1, "bias")));
		np->layers.insert(std::pair<std::string, LayerPtr>("cand", LayerPtr()));
		//np->layers["input"]->BuildLayer(Node::linear);
		//np->layers["hidden"]->BuildLayer(Node::sigmoid);
		//np->layers["bias"]->BuildLayer(Node::bias);
		np->layers["input"]->BuildLayer<Linear>();
		np->layers["out"]->BuildLayer<ASigmoid>();
		np->layers["bias"]->BuildLayer<Bias>();
		FullyConnect();
		return np;
	}

	void CCBuilder::FullyConnect()
	{
		NodeList inputNodes = np->layers["input"]->GetNodes();
		NodeList outNodes = np->layers["out"]->GetNodes();
		NodeList biasNodes = np->layers["bias"]->GetNodes();

		np->layers["input"]->SetNextLayer(np->layers["out"]);
		np->layers["out"]->SetPrevLayer(np->layers["input"]);
		Connection::SetRandomSeed();

		BOOST_FOREACH(NodePtr n, inputNodes){
			BOOST_FOREACH(NodePtr n2,outNodes){
				np->connections.push_back(Connection::Connect(n,n2));
			}
		}

		BOOST_FOREACH(NodePtr n, biasNodes){
			BOOST_FOREACH(NodePtr n2, outNodes){
				np->connections.push_back(Connection::Connect(n,n2));
			}
		}
	}

	NetworkPtr CCBuilder::GetNetwork()
	{
		return np;
	}

	void CCBuilder::SetNetwork( NetworkPtr net )
	{
		this->np = net;
	}

	void CCBuilder::CreateCandLayer(int numCand)
	{
		//TODO allow for varied node types-- for now it's homogeneous
		LayerPtr out = np->layers["out"];
		int tmpHeight = out->GetHeight();
		candLayer = Layer::CreateLayer(Layer::hidden,numCand,tmpHeight,"cand");
		candLayer->BuildLayer<ASigmoid>();
		np->layers["cand"] = candLayer;
		candLayer->SetPrevLayer(out->GetPrevLayer());
		CandFullyConnectBack(candLayer);
	}

	void CCBuilder::InstallCandidate(NodePtr node)
	{

	}

	void CCBuilder::CandFullyConnectBack( LayerPtr layer )
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

		NodeList biasNodes = np->layers["bias"]->GetNodes();
		BOOST_FOREACH(NodePtr n, biasNodes){
			BOOST_FOREACH(NodePtr n2, layerNodes){
				tmpConnections.push_back(Connection::Connect(n,n2));
			}
		}

	}

	void CCBuilder::AddHiddenLayer()
	{

	}

	void CCBuilder::FullyConnectOut( LayerPtr layer )
	{
		NodeList layerNodes = layer->GetNodes();
		NodeList outNodes = np->layers["out"]->GetNodes();
		
		Connection::SetRandomSeed();
		BOOST_FOREACH(NodePtr n, layerNodes){
			BOOST_FOREACH(NodePtr n2, outNodes){
				np->connections.push_back(Connection::Connect(n,n2));
			}
		}
	}

}