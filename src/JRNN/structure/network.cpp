/* 
 * File:   network.cpp
 * Author: jreeder
 * 
 * Created on May 17, 2010, 5:47 PM
 */
#include "JRNN.h"
#include "structure/network.h"

namespace JRNN{

	Network::Network() {
	//    numIn = numIn;
	//    numOut = numOut;
	//    numHid = numHid;
	//    layers.insert(LayerPair("input", Layer::CreateLayer(layer::input,numIn,0,"input")));
	//    layers.insert(LayerPair("hidden", Layer::CreateLayer(layer::hidden, numHid, 1,"hidden")));
	//    layers.insert(LayerPair("out", Layer::CreateLayer(layer::out,numOut, 2,"out")));
	//    layers.insert(LayerPair("bias", Layer::CreateLayer(layer::bias, 1, -1, "bias")));
	//    layers["input"]->BuildLayer(node::linear);
	//    layers["hidden"]->BuildLayer(node::sigmoid);
	//    layers["out"]->BuildLayer(node::sigmoid);
	//    layers["bias"]->BuildLayer(node::bias);
	//    fullyConnect();
		conScale = 2.0;
		conOffset = 1.0;
	}

	//NetworkPtr Network::CreateFFMLPNetwork(int numIn, int numHid, int numOut){
	//    NetworkPtr np(new Network());
	//    np->numIn = numIn;
	//    np->numOut = numOut;
	//    //np->numHid = numHid;
	//	np->numHidLayers = 1;
	//    np->layers.insert(LayerPair("input", Layer::CreateLayer(Layer::input,numIn,0,"input")));
	//    np->layers.insert(LayerPair("hidden", Layer::CreateLayer(Layer::hidden, numHid, 1,"hidden")));
	//    np->layers.insert(LayerPair("out", Layer::CreateLayer(Layer::out,numOut, 2,"out")));
	//    np->layers.insert(LayerPair("bias", Layer::CreateLayer(Layer::bias, 1, -1, "bias")));
	//    //np->layers["input"]->BuildLayer(Node::linear);
	//    //np->layers["hidden"]->BuildLayer(Node::sigmoid);
	//    //np->layers["out"]->BuildLayer(Node::sigmoid);
	//    //np->layers["bias"]->BuildLayer(Node::bias);
	//	np->layers["input"]->BuildLayer<Linear>();
	//	np->layers["hidden"]->BuildLayer<Sigmoid>();
	//	np->layers["out"]->BuildLayer<ASigmoid>();
	//	np->layers["bias"]->BuildLayer<Bias>();
	//    np->FullyConnectFFMLP();
	//    return np;
	//}
	//
	//NetworkPtr Network::CreateMinFFNetwork(int numIn, int numOut){
	//	NetworkPtr np(new Network());
	//	np->numIn = numIn;
	//	np->numOut = numOut;
	//	np->numHidLayers = 0;
	//	np->layers.insert(LayerPair("input", Layer::CreateLayer(Layer::input,numIn,0,"input")));
	//	np->layers.insert(LayerPair("out", Layer::CreateLayer(Layer::out, numOut, 1, "out")));
	//	np->layers.insert(LayerPair("bias", Layer::CreateLayer(Layer::bias, 1, -1, "bias")));
	//	//np->layers["input"]->BuildLayer(Node::linear);
	//	//np->layers["hidden"]->BuildLayer(Node::sigmoid);
	//	//np->layers["bias"]->BuildLayer(Node::bias);
	//	np->layers["input"]->BuildLayer<Linear>();
	//	np->layers["out"]->BuildLayer<ASigmoid>();
	//	np->layers["bias"]->BuildLayer<Bias>();
	//	np->FullyConnectMinFF();
	//	return np;
	//}
	//
	//
	//void Network::FullyConnectFFMLP(){
	//	NodeList inputNodes = layers["input"]->GetNodes();
	//	NodeList hidNodes = layers["hidden"]->GetNodes();
	//	NodeList outNodes = layers["out"]->GetNodes();
	//	NodeList biasNodes = layers["bias"]->GetNodes();
	//
	//	layers["input"]->SetNextLayer(layers["hidden"]);
	//	layers["hidden"]->SetPrevLayer(layers["input"]);
	//	layers["hidden"]->SetNextLayer(layers["out"]);
	//	layers["out"]->SetPrevLayer(layers["hidden"]);
	//
	//	Connection::SetRandomSeed();
	//
	//	BOOST_FOREACH(NodePtr n, inputNodes){
	//		BOOST_FOREACH(NodePtr n2, hidNodes){
	//			connections.push_back(Connection::Connect(n,n2));
	//		}
	//	}
	//
	//	BOOST_FOREACH(NodePtr n, hidNodes){
	//		BOOST_FOREACH(NodePtr n2, outNodes){
	//			connections.push_back(Connection::Connect(n,n2));
	//		}
	//	}
	//
	//	BOOST_FOREACH(NodePtr n, biasNodes){
	//		BOOST_FOREACH(NodePtr n2, hidNodes){
	//			connections.push_back(Connection::Connect(n,n2));
	//		}
	//		BOOST_FOREACH(NodePtr n3, outNodes){
	//			connections.push_back(Connection::Connect(n,n3));
	//		}
	//	}
	//}
	//
	//void Network::FullyConnectMinFF(){
	//	NodeList inputNodes = layers["input"]->GetNodes();
	//	NodeList outNodes = layers["out"]->GetNodes();
	//	NodeList biasNodes = layers["bias"]->GetNodes();
	//	
	//	layers["input"]->SetNextLayer(layers["out"]);
	//	layers["out"]->SetPrevLayer(layers["input"]);
	//	Connection::SetRandomSeed();
	//
	//	BOOST_FOREACH(NodePtr n, inputNodes){
	//		BOOST_FOREACH(NodePtr n2,outNodes){
	//			connections.push_back(Connection::Connect(n,n2));
	//		}
	//	}
	//
	//	BOOST_FOREACH(NodePtr n, biasNodes){
	//		BOOST_FOREACH(NodePtr n2, outNodes){
	//			connections.push_back(Connection::Connect(n,n2));
	//		}
	//	}
	//}

	//void Network::FullyConnectBack(LayerPtr layer){
	//	NodeList layerNodes = layer->GetNodes();
	//	LayerPtr prevLayer = layer->GetPrevLayer();
	//
	//	Connection::SetRandomSeed();
	//	while(prevLayer){
	//		NodeList prevNodes = prevLayer->GetNodes();
	//		BOOST_FOREACH(NodePtr n, prevNodes){
	//			BOOST_FOREACH(NodePtr n2, layerNodes){
	//				connections.push_back(Connection::Connect(n,n2));
	//			}
	//		}
	//		prevLayer = prevLayer->GetPrevLayer();
	//	}
	//}
	//
	//void Network::FullyConnectOut( LayerPtr layer ){
	//	NodeList layerNodes = layer->GetNodes();
	//	NodeList outNodes = layers["out"]->GetNodes();
	//
	//	Connection::SetRandomSeed();
	//	BOOST_FOREACH(NodePtr n, layerNodes){
	//		BOOST_FOREACH(NodePtr n2, outNodes){
	//			connections.push_back(Connection::Connect(n,n2));
	//		}
	//	}
	//}

	Network::Network(const Network& orig) {
		locked = orig.locked;
		numHidLayers = orig.numHidLayers;
		numIn = orig.numIn;
		numOut = orig.numOut;
		conScale = orig.conScale;
		conOffset = orig.conOffset;
	}

	Network::~Network() {
		connections.clear();
		layers.clear();
	}

	vecDouble Network::GetOutputs(){
		return outputs;
	}

	vecDouble Network::GetPrimes( string layerName )
	{
		return layers[layerName]->GetPrimes();
	}

	LayerPtr Network::GetLayer(string layerName){
		return layers[layerName];
	}

	int Network::GetNumHidLayers(){
		return numHidLayers;
	}

	void Network::Activate(vecDouble inputs){
		SetInputs(inputs);
		layers["input"]->Activate(inputs);
		layers["bias"]->Activate();
		/*layers["hidden"]->Activate();
		layers["out"]->Activate();*/
		Activate(layers["input"]->GetNextLayer());
		outputs = layers["out"]->GetOutput();
	}

	void Network::Activate(LayerPtr layer){
		layer->Activate();
		if (layer->GetName() != "out"){
			Activate(layer->GetNextLayer());
		}
	}

	void Network::SetInputs(vecDouble inputs){
		this->inputs = inputs;
	}

	void Network::SetLocked( bool lock )
	{
		this->locked = lock;
	}

	bool Network::GetLocked()
	{
		return locked;
	}

	void Network::SetNumIn( int numIn )
	{
		this->numIn = numIn;
	}

	int Network::GetNumIn(){
		return numIn;
	}

	void Network::SetNumOut(int numOut){
		this->numOut = numOut;
	}

	int Network::GetNumOut(){
		return numOut;
	}

	void Network::SetNumHidLayers( int numHidLayers )
	{
		this->numHidLayers = numHidLayers;
	}


	ConMap& Network::GetConnections(){
		return connections;
	}

	 void Network::Reset(){
		 BOOST_FOREACH(ConPair con, connections){
			 con.second->Reset();
		 }
	 }

	 hashedDoubleMap Network::GetWeights(){
		 hashedDoubleMap weights;
		 BOOST_FOREACH(ConPair con, connections){
			 weights[con.second->GetName()] = con.second->GetWeight();
		 }
		 return weights;
	 }

	 void Network::SetWeights(hashedDoubleMap weights){
		 //TODO: need to do some error handling in here this could potentially be dangerous
		 BOOST_FOREACH(ConPair con, connections){
			 //double tmp = weights[con->GetName()];
			 //cout << "setting weight for " << con->getName() << " " << tmp << endl;
			 con.second->SetWeight(weights[con.second->GetName()]);
		 }
	 }

	 void Network::PrintConnections(){
		 cout << "Connections:" << endl;
		 BOOST_FOREACH(ConPair con, connections){
			 cout << con.second->GetName() << " " << con.second->GetWeight() << endl;
		 }
	 }

	 /*bool Network::AddHiddenLayer(LayerPtr newLayer){
		if (!this->locked)
		{
			string tmpName = "hidden-";
			numHidLayers += 1;
			tmpName += lexical_cast<string>(numHidLayers);
			layers.insert(LayerPair(tmpName,newLayer));
			return true;
		} 
		else
		{
			return false;
		}
	 }*/

	 void Network::AddConnection(ConPtr con)
	 {
		 //con->SetScaleAndOffset(conScale, conOffset);
		 //if (reset){
			//  con->Reset(); //without this the original scale and offset would have been used. 
		 //}
		 connections.insert(ConPair(con->GetName(),con));
	 }

	 void Network::RemoveConnection( ConPtr con )
	 {
		 con->Disconnect();
		 connections.erase(con->GetName());
	 }

	 void Network::RemoveConnections( ConList cons )
	 {
		 BOOST_FOREACH(ConPtr con, cons){
			con->Disconnect();
			connections.erase(con->GetName());
		 }
	 }

	 LayerPtr Network::AddHiddenLayer()
	 {
		 string name = "hidden-";
		 name += lexical_cast<string>(numHidLayers);
		 LayerPtr lp = Layer::CreateLayer(Layer::hidden,0,numHidLayers + 1,name);
		 layers.insert(LayerPair(name,lp));
		 LayerPtr out = layers["out"];
		 LayerPtr prevLayer = out->GetPrevLayer();
		 lp->SetPrevLayer(prevLayer);
		 out->SetPrevLayer(lp);
		 lp->SetNextLayer(out);
		 prevLayer->SetNextLayer(lp);
		 numHidLayers++;
		 out->SetHeight(numHidLayers + 1);
		 return lp;
	 }

	 void Network::RemoveHiddenLayer( LayerPtr layer )
	 {
		 if (layer->GetType() == Layer::hidden){
			 RemoveConnections(layer->GetConnections());
			 LayerPtr next = layer->GetNextLayer();
			 LayerPtr prev = layer->GetPrevLayer();
			 prev->SetNextLayer(next);
			 next->SetPrevLayer(prev);
			 numHidLayers--;
			 layer->Clear();
			 layers.erase(layer->GetName());
			 ResetHeights();
		 }
	 }

	 NodePtr Network::GetNode( string nodeName )
	 {
		 NodePtr retNode;
		 int pos = nodeName.find("_");
		 string layerName = "";
		 if (pos != string::npos){
			 layerName = nodeName.substr(0,pos);
		 }
		 if (layerName.size() > 0){
			retNode = layers[layerName]->GetNodeByName(nodeName);
		 }
		 return retNode;
	 }

	 void Network::Clone(NetworkPtr newP, NetworkPtr oldP){
		 newP->numHidLayers = oldP->numHidLayers;
		 newP->numIn = oldP->numIn;
		 newP->numOut = oldP->numOut;
		 BOOST_FOREACH(LayerPair newLP, oldP->layers){
			 newP->layers.insert(LayerPair(newLP.first, Layer::Clone(newLP.second)));
		 }
		 BOOST_FOREACH(LayerPair lp, oldP->layers){
			 LayerPtr layer = newP->layers[lp.first];
			 if (lp.second->GetPrevLayer() != 0){
				 layer->SetPrevLayer(newP->layers[lp.second->GetPrevLayer()->GetName()]);
			 }
			 if (lp.second->GetNextLayer() != 0){
				 layer->SetNextLayer(newP->layers[lp.second->GetNextLayer()->GetName()]);
			 }
		 }
		 BOOST_FOREACH(ConPair conP, oldP->connections){
			 ConPtr con = conP.second;
			 newP->AddConnection(Connection::Clone(con, newP->GetNode(con->GetInNodeName()), newP->GetNode(con->GetOutNodeName())));
		 }
	 }

	NetworkPtr Network::Clone()
	 {
		 NetworkPtr np(new Network());
		 NetworkPtr oldP = shared_from_this();
		 Network::Clone(np, oldP);
		 return np;
	 }

	void Network::SetScaleAndOffset( double scale, double offset )
	{
		conScale = scale;
		conOffset = offset;
	}

	ConPtr Network::Connect( NodePtr n1, NodePtr n2 )
	{
		ConPtr cp = Connection::Connect(n1,n2);
		cp->SetScaleAndOffset(conScale, conOffset);
		cp->Reset();
		return cp;
	}

	ConPtr Network::Connect( NodePtr n1, NodePtr n2, double conweight )
	{
		ConPtr cp = Connection::Connect(n1,n2,conweight);
		cp->SetScaleAndOffset(conScale, conOffset);
		return cp;
	}

	void Network::ResetHeights()
	{
		LayerPtr current = layers["input"];
		LayerPtr next = current->GetNextLayer();
		while(next){
			int tmpHeight = current->GetHeight();
			next->SetHeight(tmpHeight + 1);
			current = next;
			next = next->GetNextLayer();
		}
	}

}