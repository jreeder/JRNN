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
		hiddenLayers.clear();
	}

	CCNetworkPtr CCNetwork::Create()
	{
		CCNetworkPtr np(new CCNetwork());
		return np;
	}

	CCNetworkPtr CCNetwork::Clone( CCNetworkPtr net )
	{
		CCNetworkPtr ccnet(new CCNetwork());
		/*ccnet->numHidLayers = net->numHidLayers;
		ccnet->numIn = net->numIn;
		ccnet->numOut = net->numOut;
		
		BOOST_FOREACH(LayerPair newLP, net->layers){
			ccnet->layers.insert(LayerPair(newLP.first, Layer::Clone(newLP.second)));
		}
		ccnet->candLayer = ccnet->layers["cand"];
		BOOST_FOREACH(LayerPair lp, net->layers){
			LayerPtr layer = ccnet->layers[lp.first];
			if(lp.second->GetPrevLayer() != 0){
				layer->SetPrevLayer(ccnet->layers[lp.second->GetPrevLayer()->GetName()]);
			}
			if(lp.second->GetNextLayer() != 0){
				layer->SetNextLayer(ccnet->layers[lp.second->GetNextLayer()->GetName()]);
			}
		}
		BOOST_FOREACH(ConPair conP, net->connections){
			ConPtr con = conP.second;
			ccnet->AddConnection(Connection::Clone(con,ccnet->GetNode(con->GetInNodeName()),ccnet->GetNode(con->GetOutNodeName())));
		}*/
		//NetworkPtr newP(ccnet);
		//NetworkPtr oldP(net);

		Network::Clone(ccnet, net);
		ccnet->candLayer = ccnet->layers["cand"];
		BOOST_FOREACH(LayerPair lp, ccnet->layers){
			if (lp.second->GetType() == Layer::hidden){
				ccnet->hiddenLayers.push_back(lp.second);
			}
		}
		ccnet->numUnits = net->numUnits;
		return ccnet;
	}

	NetworkPtr CCNetwork::Clone()
	{
		CCNetworkPtr oldP = CCSharedFromThis::shared_from_this();
		return CCNetwork::Clone(oldP);
	}

	void CCNetwork::Build( int numIn, int numOut, bool cloneouts /*= false*/, bool useSDCC /*= false*/, bool varyActFunc /*= false*/ )
	{
		this->numIn	= numIn;
		this->numOut = numOut;
		this->numHidLayers = 0;
		this->numUnits = numIn;
		this->cloneOuts = cloneouts;
		this->useSDCC = useSDCC;
		this->varyActFunc = varyActFunc;
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
		currentLayer = layers["input"];
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
			if (cloneOuts){ //This is done for etaMTL though it doesn't generalize well if each task has more than one output. 
				double conWeight = Connection::GetRandWeight(this->conScale, this->conOffset);
				BOOST_FOREACH(NodePtr n2, outNodes){
					AddConnection(Connect(n,n2,conWeight));
				}
			}
			else {
				BOOST_FOREACH(NodePtr n2,outNodes){
					AddConnection(Connect(n,n2));
				}
			}
		}

		BOOST_FOREACH(NodePtr n, biasNodes){
			if (cloneOuts){
				double conWeight = Connection::GetRandWeight(this->conScale, this->conOffset);
				BOOST_FOREACH(NodePtr n2, outNodes){
					AddConnection(Connect(n,n2,conWeight));
				}
			}
			else {
				BOOST_FOREACH(NodePtr n2,outNodes){
					AddConnection(Connect(n,n2));
				}
			}
		}
	}

	void CCNetwork::CreateCandLayer(int numCand)
	{
		//TODO allow for varied node types-- for now it's homogeneous
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
	}

	void CCNetwork::InstallCandidate(NodePtr node, vecDouble outWeights)
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
		if (outWeights.size() == 0){
			
			CandConnectOut(node);
			//FullyConnectOut(lp);
		}
		else {
			
			CandConnectOut(node, outWeights);
			//FullyConnectOut(lp, outWeights);
		}

		if (newLayer)
		{
			this->currentLayer = lp;
			hiddenLayers.push_back(lp);
		}
		numUnits++;
	}

	void CCNetwork::CandFullyConnectBack( LayerPtr layer )
	{
		NodeList layerNodes = layer->GetNodes();
		LayerPtr prevLayer = layer->GetPrevLayer();
		
		Connection::SetRandomSeed();

		if (useSDCC && prevLayer->GetType() != Layer::input){ //Use the sibling/descendant mechanism to reduce network depth
			NodeList prevNodes = prevLayer->GetNodes();
			int count = 0;
			int tmpHeight = layer->GetHeight();
			BOOST_FOREACH(NodePtr n, prevNodes){
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
			prevLayer = prevLayer->GetPrevLayer();
		}

		while(prevLayer){
			NodeList prevNodes = prevLayer->GetNodes();
			BOOST_FOREACH(NodePtr n, prevNodes){
				BOOST_FOREACH(NodePtr n2, layerNodes){
					candConnections.push_back(Connect(n,n2));
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

	//LayerPtr CCNetwork::AddHiddenLayer()
	//{
	//	string name = "hidden-";
	//	name += lexical_cast<string>(numHidLayers);
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

	//NOT used anymore but left here if I need it. 
	void CCNetwork::FullyConnectOut( LayerPtr layer )
	{
		NodeList layerNodes = layer->GetNodes();
		NodeList outNodes = layers["out"]->GetNodes();
		
		Connection::SetRandomSeed();
		BOOST_FOREACH(NodePtr n, layerNodes){
			if (cloneOuts){
				double conWeight = Connection::GetRandWeight(this->conScale, this->conOffset);
				BOOST_FOREACH(NodePtr n2, outNodes){
					AddConnection(Connect(n,n2,conWeight));
				}
			}
			else {
				BOOST_FOREACH(NodePtr n2,outNodes){
					AddConnection(Connect(n,n2));
				}
			}
		}
	}

	//NOT used anymore but left here if I need it. 
	void CCNetwork::FullyConnectOut( LayerPtr layer, vecDouble outWeights)
	{
		NodeList layerNodes = layer->GetNodes();
		NodeList outNodes = layers["out"]->GetNodes();

		//Connection::SetRandomSeed();
		BOOST_FOREACH(NodePtr n, layerNodes){
			for (unsigned int i = 0; i < outNodes.size(); i++){
				AddConnection(Connect(n,outNodes[i], outWeights[i]));
			}
		}
	}

	void CCNetwork::CandConnectOut( NodePtr node, vecDouble outWeights )
	{
		NodeList outNodes = layers["out"]->GetNodes();
		if (outWeights.size() > 0){
			assert(outNodes.size() == outWeights.size());
			for (uint i = 0; i < outNodes.size(); i++)
			{
				AddConnection(Connect(node,outNodes[i], outWeights[i]));
			}
		}
		else {
			Connection::SetRandomSeed();
			if (cloneOuts){
				double conWeight = Connection::GetRandWeight(this->conScale, this->conOffset);
				BOOST_FOREACH(NodePtr n, outNodes){
					AddConnection(Connect(node, n, conWeight));
				}
			}
			else {
				BOOST_FOREACH(NodePtr n, outNodes){
					AddConnection(Connect(node, n));
				}
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
		currentLayer = layers["input"];
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
			string name = con.first;
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

	void CCNetwork::SetUseSDCC( bool inUseSDCC )
	{
		this->useSDCC = inUseSDCC;
	}

	void CCNetwork::SetVaryActFunc( bool inVaryActFunc )
	{
		this->varyActFunc = inVaryActFunc;
	}

	void CCNetwork::BuildVariedLayer( LayerPtr candLayer )
	{
		int layerSize = candLayer->GetSize();
		int layerHeight = candLayer->GetHeight();
		string baseName = candLayer->GetName() + "_";
		NodeList& layerNodes = candLayer->GetNodes();
		for (int i = 0; i < layerSize; i++)
		{
			string num = lexical_cast<string>(i);
			string name = baseName + num;
			NodePtr np;
			switch (i % 4)
			{
			case 0:
				np = Node::CreateNode<ASigmoid>(layerHeight, name);
				break;
			case 1:
				np = Node::CreateNode<Sigmoid>(layerHeight, name);
				break;
			case 2:
				np = Node::CreateNode<Gaussian>(layerHeight, name);
				break;
			/*case 3:
				np = Node::CreateNode<Linear>(layerHeight, name);
				break;*/
			default:
				np = Node::CreateNode<ASigmoid>(layerHeight, name);
			}
			layerNodes.push_back(np);
		}
	}

	void CCNetwork::SetNumUnits( int newNumUnits )
	{
		this->numUnits = newNumUnits;
	}

	void CCNetwork::SetCandLayerByName(string name){
		this->candLayer = GetLayer(name);
	}

	void CCNetwork::SetCurrentLayerByName( string currentLayerName )
	{
		this->currentLayer = GetLayer(currentLayerName);
	}

	void CCNetwork::SetCloneOuts( bool cloneOuts )
	{
		this->cloneOuts = cloneOuts;
	}

	LayerList& CCNetwork::GetHiddenLayers()
	{
		return hiddenLayers;
	}

	LayerPtr CCNetwork::GetCurrentLayer()
	{
		return currentLayer;
	}

	bool CCNetwork::GetCloneOuts()
	{
		return cloneOuts;
	}

	bool CCNetwork::GetSDCC()
	{
		return useSDCC;
	}

	bool CCNetwork::GetVaryActFunc()
	{
		return varyActFunc;
	}

}