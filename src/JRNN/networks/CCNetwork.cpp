/* 
 * File:   CCNetwork.cpp
 * Author: jreeder
 * 
 * Created on July 13, 2010
 */

#include "JRNN.h"
#include "networks/CCNetwork.h"

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
		candConnections.clear();
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
		//TODO Added some static strings for input output and bias layers so I don't have
		//to worry about mistyping errors. 
		layers.insert(LayerPair("input", Layer::CreateLayer(Layer::input, numIn,0,"input")));
		layers.insert(LayerPair("out", Layer::CreateLayer(Layer::out, numOut, 1, "out")));
		layers.insert(LayerPair("bias", Layer::CreateLayer(Layer::bias, 1, -1, "bias")));
		layers.insert(LayerPair("cand", Layer::CreateLayer(Layer::hidden,0,0,"cand")));
		//layers["input"]->BuildLayer(Node::linear);
		//layers["hidden"]->BuildLayer(Node::sigmoid);
		//layers["bias"]->BuildLayer(Node::bias);
		layers["input"]->BuildLayer<Linear>();
		layers["out"]->BuildLayer<ASigmoid>();
		layers["bias"]->BuildLayer<Bias>();
		candLayer = layers["cand"];
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
		candLayer->Clear();
		candLayer->SetLayerSize(numCand);
		candLayer->SetHeight(tmpHeight);
		candLayer->BuildLayer<ASigmoid>();
		candLayer->SetPrevLayer(out->GetPrevLayer());
		CandFullyConnectBack(candLayer);
	}

	void CCNetwork::InstallCandidate(NodePtr node)
	{
		LayerPtr lp = AddHiddenLayer();
		lp->AddNode(node);
		BOOST_FOREACH(ConPtr con, node->GetConnections(conType::IN)){
			connections.push_back(con);
		}
		FullyConnectOut(lp);
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
					candConnections.push_back(Connection::Connect(n,n2));
				}
			}
			prevLayer = prevLayer->GetPrevLayer();
		}

		NodeList biasNodes = layers["bias"]->GetNodes();
		BOOST_FOREACH(NodePtr n, biasNodes){
			BOOST_FOREACH(NodePtr n2, layerNodes){
				candConnections.push_back(Connection::Connect(n,n2));
			}
		}

	}

	LayerPtr CCNetwork::AddHiddenLayer()
	{
		std::string name = "hidden-";
		name += lexical_cast<std::string>(numHidLayers);
		LayerPtr lp = Layer::CreateLayer(Layer::hidden,0,numHidLayers + 1,name);
		layers.insert(LayerPair(name,lp));
		LayerPtr out = layers["out"];
		LayerPtr prevLayer = out->GetPrevLayer();
		lp->SetPrevLayer(prevLayer);
		out->SetPrevLayer(lp);
		lp->SetNextLayer(out);
		prevLayer->SetNextLayer(lp);
		numHidLayers++;
		return lp;
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