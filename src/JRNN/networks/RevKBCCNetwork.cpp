/********************************************************************
	created:	2013/05/28
	created:	28:5:2013   3:45
	filename: 	RevKBCCNetwork.cpp
	author:		jreeder
	
	purpose:	Reverberated KBCCNetwork
*********************************************************************/

#include "JRNN.h"
#include "networks/RevKBCCNetwork.h"

namespace JRNN {

	const string RevKBCCNetwork::Type = "RevKBCC";

	RevKBCCNetwork::RevKBCCNetwork(){
		getTrueOuts = false;
	}

	RevKBCCNetwork::RevKBCCNetwork(int numIn, int numOut) : KBCCNetwork(numIn, numOut) {
		getTrueOuts = false;
	}

	RevKBCCNetwork::~RevKBCCNetwork(){
		autoAssocLayer->Clear(false);
		normOutLayer->Clear(false);
	}

	RevKBCCNetworkPtr RevKBCCNetwork::Create() {
		RevKBCCNetworkPtr np(new RevKBCCNetwork());
		return np;
	}

	JRNN::RevKBCCNetworkPtr RevKBCCNetwork::Clone( RevKBCCNetworkPtr net )
	{
		RevKBCCNetworkPtr revkbccnet(new RevKBCCNetwork());
		KBCCNetwork::Clone(revkbccnet, net);
		revkbccnet->autoAssocLayer = revkbccnet->layers["autoassoc"];
		revkbccnet->normOutLayer = revkbccnet->layers["normout"];
		return revkbccnet;
	}

	JRNN::NetworkPtr RevKBCCNetwork::Clone()
	{
		RevKBCCNetworkPtr oldP = RevKBCCSharedFromThis::shared_from_This();
		return RevKBCCNetwork::Clone(oldP);
	}

	const LayerPtr RevKBCCNetwork::GetAutoAssocLayer()
	{
		return autoAssocLayer;
	}

	const LayerPtr RevKBCCNetwork::GetNormOutLayer()
	{
		return normOutLayer;
	}

	void RevKBCCNetwork::Build( int numIn, int numOut, bool cloneouts /*= false*/, bool useSDCC /*= false*/, bool varyActFunc /*= false*/, string outNodeType /*= ASigmoid::_type*/, string autoAssocType /*= ASigmoid::_type*/ )
	{
		KBCCNetwork::Build(numIn, numOut, cloneouts, useSDCC, varyActFunc, outNodeType);

		layers.insert(LayerPair("autoassoc", Layer::CreateLayer(Layer::spec, numIn,-1,"autoassoc", true)));
		//layers["autoassoc"]->BuildLayer<ASigmoid>();

		if (autoAssocType == Sigmoid::_type){
			layers["autoassoc"]->BuildLayer<Sigmoid>();
		}
		else if (autoAssocType == Gaussian::_type){
			layers["autoassoc"]->BuildLayer<Gaussian>();
		}
		else if (autoAssocType == Linear::_type){
			layers["autoassoc"]->BuildLayer<Linear>();
		}
		else {
			layers["autoassoc"]->BuildLayer<ASigmoid>();
		}

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

	void RevKBCCNetwork::Reset()
	{
		KBCCNetwork::Reset();
	}

	void RevKBCCNetwork::FullyConnectAutoAssoc(LayerPtr layer){
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

	void RevKBCCNetwork::FullyConnectAutoAssoc(LayerPtr layer, vecDouble outWeights){
		NodeList layerNodes = layer->GetNodes();
		NodeList assocNodes = layers["autoassoc"]->GetNodes();

		BOOST_FOREACH(NodePtr n, layerNodes){
			for (unsigned int i = 0; i < assocNodes.size(); i++){
				AddConnection(Connect(n,assocNodes[i], outWeights[i]));
			}
		}
	}

	vecDouble RevKBCCNetwork::GetOutputs()
	{
		if (getTrueOuts){
			return normOutLayer->GetOutput();
		}
		else {
			return Network::GetOutputs();
		}
	}

	int RevKBCCNetwork::GetNumOut()
	{
		if (getTrueOuts){
			return normOutLayer->GetSize();
		}
		else return Network::GetNumOut();
	}

	vecDouble RevKBCCNetwork::GetTrueOutputs()
	{
		return normOutLayer->GetOutput();
	}

	void RevKBCCNetwork::SetAutoAssocLayerByName( string name )
	{
		this->autoAssocLayer = GetLayer(name);
	}

	void RevKBCCNetwork::SetNormOutLayerByName( string name )
	{
		this->normOutLayer = GetLayer(name);
	}
}