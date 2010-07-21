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
		this->numUnits = numIn;
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
				AddConnection(Connection::Connect(n,n2));
			}
		}

		BOOST_FOREACH(NodePtr n, biasNodes){
			BOOST_FOREACH(NodePtr n2, outNodes){
				AddConnection(Connection::Connect(n,n2));
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
		candConnections.clear();
		candLayer->SetLayerSize(numCand);
		candLayer->SetHeight(tmpHeight);
		candLayer->BuildLayer<ASigmoid>();
		candLayer->SetPrevLayer(out->GetPrevLayer());
		CandFullyConnectBack(candLayer);
	}

	void CCNetwork::InstallCandidate(NodePtr node, vecDouble outWeights)
	{
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
		hiddenLayers.push_back(lp);
		numUnits++;
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

	//LayerPtr CCNetwork::AddHiddenLayer()
	//{
	//	std::string name = "hidden-";
	//	name += lexical_cast<std::string>(numHidLayers);
	//	LayerPtr lp = Layer::CreateLayer(Layer::hidden,0,numHidLayers + 1,name);
	//	layers.insert(LayerPair(name,lp));
	//	LayerPtr out = layers["out"];
	//	LayerPtr prevLayer = out->GetPrevLayer();
	//	lp->SetPrevLayer(prevLayer);
	//	out->SetPrevLayer(lp);
	//	lp->SetNextLayer(out);
	//	prevLayer->SetNextLayer(lp);
	//	numHidLayers++;
	//	return lp;
	//}

	void CCNetwork::FullyConnectOut( LayerPtr layer )
	{
		NodeList layerNodes = layer->GetNodes();
		NodeList outNodes = layers["out"]->GetNodes();
		
		Connection::SetRandomSeed();
		BOOST_FOREACH(NodePtr n, layerNodes){
			BOOST_FOREACH(NodePtr n2, outNodes){
				AddConnection(Connection::Connect(n,n2));
			}
		}
	}

	void CCNetwork::FullyConnectOut( LayerPtr layer, vecDouble outWeights)
	{
		NodeList layerNodes = layer->GetNodes();
		NodeList outNodes = layers["out"]->GetNodes();

		Connection::SetRandomSeed();
		BOOST_FOREACH(NodePtr n, layerNodes){
			for (unsigned int i = 0; i < outNodes.size(); i++){
				AddConnection(Connection::Connect(n,outNodes[i], outWeights[i]));
			}
		}
	}

	const LayerPtr CCNetwork::GetCandLayer()
	{
		return candLayer;
	}

	void CCNetwork::Reset()
	{
		BOOST_FOREACH(LayerPtr layer, hiddenLayers){
			Network::RemoveHiddenLayer(layer);
		}
		candConnections.clear();
		candLayer->Clear();
		hiddenLayers.clear();
		numUnits = numIn;
		Network::Reset();
	}

	int CCNetwork::GetNumUnits()
	{
		return numUnits;
	}

	void CCNetwork::SetWeights( hashedDoubleMap weights )
	{
		hashedDoubleMap::iterator weightIT;
		ConMap::iterator conIT = connections.begin();
		bool consRemoved = false;
		ConList	consToRemove;
		while(conIT != connections.end()){
			ConPair con = (*conIT);
			std::string name = con.first;
			weightIT = weights.find(name);
			if(weightIT != weights.end()){
				con.second->SetWeight((*weightIT).second);
			}
			else {
				con.second->Disconnect();
				consToRemove.push_back((*conIT).second);
				consRemoved = true;
			}
			conIT++;
		}
		if(consRemoved){
			BOOST_FOREACH(ConPtr con, consToRemove){
				connections.erase(con->GetName());
			}
			RemoveUnConnectedNodes();
		}
		
	}

	void CCNetwork::RemoveUnConnectedNodes()
	{
		int numNodesRemoved = 0;
		LayerList layersToRemove;
		BOOST_FOREACH(LayerPtr layer, hiddenLayers){
			numNodesRemoved += layer->RemoveUnconnectedNodes();
			if(layer->GetSize() == 0){
				layersToRemove.push_back(layer);
			}
		}
		if(layersToRemove.size()>0){
			BOOST_FOREACH(LayerPtr layer, layersToRemove){
				RemoveHiddenLayer(layer);
			}
		}
		numUnits -= numNodesRemoved;
	}

	void CCNetwork::RemoveHiddenLayer( LayerPtr layer )
	{
		LayerList::iterator it = hiddenLayers.begin();
		for (; it != hiddenLayers.end(); it++){
			if ((*it) == layer){
				hiddenLayers.erase(it);
				break;
			}
		}
		Network::RemoveHiddenLayer(layer);
	}

}