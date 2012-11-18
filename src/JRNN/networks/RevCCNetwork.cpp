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
	
	RevCCNetwork::RevCCNetwork(){
		getTrueOuts = false;
	}

	RevCCNetwork::RevCCNetwork(int numIn, int numOut){
		this->numIn = numIn;
		this->numOut = numOut;
		getTrueOuts = false;
	}

	RevCCNetwork::~RevCCNetwork(){
		autoAssocLayer->Clear(false);
		normOutLayer->Clear(false);
	}

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
		revccnet->normOutLayer = revccnet->layers["normout"]; 
		return revccnet;
	}

	NetworkPtr RevCCNetwork::Clone() {
		RevCCNetworkPtr oldP = RevCCSharedFromThis::shared_from_this();
		return RevCCNetwork::Clone(oldP);
	}

	const LayerPtr RevCCNetwork::GetAutoAssocLayer(){
		return autoAssocLayer;
	}

	const LayerPtr RevCCNetwork::GetNormOutLayer(){
		return normOutLayer;
	}

	void RevCCNetwork::Build( int numIn, int numOut, bool cloneouts /*= false*/, bool useSDCC /*= false*/, bool varyActFunc /*= false*/ )
	{
		CCNetwork::Build(numIn, numOut, cloneouts, useSDCC, varyActFunc);
		//The way I'm doing things right now will mean that I'll treat them as normal outs
		//and will have a second special layer pointing to the original outs. 
		layers.insert(LayerPair("autoassoc", Layer::CreateLayer(Layer::spec, numIn,-1,"autoassoc", true)));
		layers["autoassoc"]->BuildLayer<ASigmoid>();
		autoAssocLayer = layers["autoassoc"];
		FullyConnectAutoAssoc(layers["bias"]);
		layers.insert(LayerPair("normout", Layer::CreateLayer(Layer::spec, 0, -1, "normout", true))); //size will be set by the shallowcopy. 
		normOutLayer = layers["normout"];
		LayerPtr tmpOutLayer = layers["out"];
		normOutLayer->ShallowCopy(tmpOutLayer);
		BOOST_FOREACH(NodePtr node, autoAssocLayer->GetNodes()){
			tmpOutLayer->AddNode(node);
			this->numOut++;
		}
		ResetConMap();
	}

	void RevCCNetwork::InstallCandidate(NodePtr node, vecDouble outWeights /* = vecDouble */, vecDouble assocWeights /*=vecDouble*/){
		CCNetwork::InstallCandidate(node, outWeights);
		//LayerPtr lp = AddHiddenLayer();
		//lp->AddNode(node);
		//candLayer->RemoveNode(node);
		//BOOST_FOREACH(ConPtr con, node->GetConnections(IN)){
		//	AddConnection(con);
		//}
		//if (outWeights.size() == 0){
		//	FullyConnectOut(lp);
		//}
		//else {
		//	FullyConnectOut(lp, outWeights);	
		//}
		////I don't need this right now since I've added them to the normal output layer. 
		///*
		//if (assocWeights.size() == 0){
		//	FullyConnectAutoAssoc(lp);
		//}
		//else {
		//	FullyConnectAutoAssoc(lp, assocWeights);
		//}
		//*/
		//hiddenLayers.push_back(lp);
		//numUnits++;
	}

	//doesn't do anything right now. 
	void RevCCNetwork::Reset(){
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

	vecDouble RevCCNetwork::GetOutputs()
	{
		if (getTrueOuts){
			return normOutLayer->GetOutput();
		}
		else {
			return Network::GetOutputs();
		}
	}

	vecDouble RevCCNetwork::GetTrueOutputs()
	{
		return normOutLayer->GetOutput();
	}

	void RevCCNetwork::AppendInputNode( NodePtr node )
	{
		LayerPtr inputLayer = layers["input"];
		inputLayer->AddNode(node);
		this->numIn++;
		//connect this new input node to all the outputs. 
		//The hidden layer nodes if they are connected will be handled by the trainer. 
		NodeList outNodes = normOutLayer->GetNodes(); //normouts are the regular outputs.
		BOOST_FOREACH(NodePtr outNode, outNodes){
			AddConnection(Connect(node, outNode));
		}
		AppendNewAutoAssocNode();

	}

	void RevCCNetwork::InsertInputNode( NodePtr newNode, int pos )
	{
		LayerPtr inputLayer = layers["input"];
		inputLayer->InsertNode(newNode, pos);
		this->numIn++;
		NodeList outNodes = normOutLayer->GetNodes();
		ResetNames();
		BOOST_FOREACH(NodePtr outNode, outNodes){
			AddConnection(Connect(newNode, outNode));
		}
		InsertNewAutoAssocNode(pos);
	}

	void RevCCNetwork::AppendNewAutoAssocNode()
	{
		int height = layers["out"]->GetHeight();
		string name = "tmpName";
		NodePtr newOutNode = Node::CreateNode<ASigmoid>(height, name);
		layers["out"]->AddNode(newOutNode);
		autoAssocLayer->AddNode(newOutNode, false);
		ConnectNodeToLayer(newOutNode, layers["bias"], IN);
		ConnectToHiddenNodes(newOutNode, IN);
		this->numOut++;
	}

	void RevCCNetwork::InsertNewAutoAssocNode( int pos )
	{
		LayerPtr outLayer = layers["out"];
		int height = outLayer->GetHeight();
		string name = "tmpName";
		NodePtr newOutNode = Node::CreateNode<ASigmoid>(height,name);
		int normOutSize = normOutLayer->GetSize();
		int outPos = normOutSize + pos;
		outLayer->InsertNode(newOutNode, outPos);
		autoAssocLayer->InsertNode(newOutNode, pos, true);
		ResetNames();
		ConnectNodeToLayer(newOutNode, layers["bias"], IN);
		ConnectToHiddenNodes(newOutNode, IN);
		this->numOut++;
	}

	void RevCCNetwork::SetAutoAssocLayerByName( string name )
	{
		this->autoAssocLayer = GetLayer(name);
	}

	void RevCCNetwork::SetNormOutLayerByName( string name )
	{
		this->normOutLayer = GetLayer(name);
	}

}