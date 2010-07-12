/* 
 * File:   network.cpp
 * Author: jreeder
 * 
 * Created on May 17, 2010, 5:47 PM
 */
#include "JRNN.h"
#include "structure/network.h"

using namespace JRNN;

Network::Network() {
//    numIn = numIn;
//    numOut = numOut;
//    numHid = numHid;
//    layers.insert(std::pair<std::string, layerPtr>("input", Layer::CreateLayer(layer::input,numIn,0,"input")));
//    layers.insert(std::pair<std::string, layerPtr>("hidden", Layer::CreateLayer(layer::hidden, numHid, 1,"hidden")));
//    layers.insert(std::pair<std::string, layerPtr>("out", Layer::CreateLayer(layer::out,numOut, 2,"out")));
//    layers.insert(std::pair<std::string, layerPtr>("bias", Layer::CreateLayer(layer::bias, 1, -1, "bias")));
//    layers["input"]->BuildLayer(node::linear);
//    layers["hidden"]->BuildLayer(node::sigmoid);
//    layers["out"]->BuildLayer(node::sigmoid);
//    layers["bias"]->BuildLayer(node::bias);
//    fullyConnect();
}

NetworkPtr Network::CreateFFMLPNetwork(int numIn, int numHid, int numOut){
    NetworkPtr np(new Network());
    np->numIn = numIn;
    np->numOut = numOut;
    np->numHid = numHid;
	np->numHidLayers = 1;
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
    np->FullyConnectFFMLP();
    return np;
}

NetworkPtr Network::CreateMinFFNetwork(int numIn, int numOut){
	NetworkPtr np(new Network());
	np->numIn = numIn;
	np->numOut = numOut;
	np->numHidLayers = 0;
	np->layers.insert(std::pair<std::string, LayerPtr>("input", Layer::CreateLayer(Layer::input,numIn,0,"input")));
	np->layers.insert(std::pair<std::string, LayerPtr>("out", Layer::CreateLayer(Layer::out, numOut, 2, "out")));
	np->layers.insert(std::pair<std::string, LayerPtr>("bias", Layer::CreateLayer(Layer::bias, 1, -1, "bias")));
	//np->layers["input"]->BuildLayer(Node::linear);
	//np->layers["hidden"]->BuildLayer(Node::sigmoid);
	//np->layers["bias"]->BuildLayer(Node::bias);
	np->layers["input"]->BuildLayer<Linear>();
	np->layers["out"]->BuildLayer<ASigmoid>();
	np->layers["bias"]->BuildLayer<Bias>();
	np->FullyConnectMinFF();
	return np;
}


void Network::FullyConnectFFMLP(){
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

void Network::FullyConnectMinFF(){
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

void Network::FullyConnectBack(LayerPtr layer){
	NodeList layerNodes = layer->GetNodes();
	LayerPtr prevLayer = layer->GetPrevLayer();

	Connection::SetRandomSeed();
	while(prevLayer){
		NodeList prevNodes = prevLayer->GetNodes();
		BOOST_FOREACH(NodePtr n, prevNodes){
			BOOST_FOREACH(NodePtr n2, layerNodes){
				connections.push_back(Connection::Connect(n,n2));
			}
		}
		prevLayer = prevLayer->GetPrevLayer();
	}
}

void Network::FullyConnectOut( LayerPtr layer ){
	NodeList layerNodes = layer->GetNodes();
	NodeList outNodes = layers["out"]->GetNodes();

	Connection::SetRandomSeed();
	BOOST_FOREACH(NodePtr n, layerNodes){
		BOOST_FOREACH(NodePtr n2, outNodes){
			connections.push_back(Connection::Connect(n,n2));
		}
	}
}

//network::network(const network& orig) {
//}

Network::~Network() {
    connections.clear();
    layers.clear();
}

vecDouble Network::GetOutputs(){
    return outputs;
}

LayerPtr Network::GetLayer(std::string layerName){
    return layers[layerName];
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

ConList& Network::GetConnections(){
    return connections;
}

 void Network::Reset(){
     BOOST_FOREACH(ConPtr con, connections){
         con->Reset();
     }
 }

 hashedDoubleMap Network::GetWeights(){
     hashedDoubleMap weights;
     BOOST_FOREACH(ConPtr con, connections){
         weights[con->GetName()] = con->GetWeight();
     }
     return weights;
 }

 void Network::SetWeights(hashedDoubleMap weights){
     //TODO: need to do some error handling in here this could potentially be dangerous
     BOOST_FOREACH(ConPtr con, connections){
         double tmp = weights[con->GetName()];
         //cout << "setting weight for " << con->getName() << " " << tmp << endl;
         con->SetWeight(weights[con->GetName()]);
     }
 }

 void Network::PrintConnections(){
     cout << "Connections:" << endl;
     BOOST_FOREACH(ConPtr con, connections){
         cout << con->GetName() << " " << con->GetWeight() << endl;
     }
 }
