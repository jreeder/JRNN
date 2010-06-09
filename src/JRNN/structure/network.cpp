/* 
 * File:   network.cpp
 * Author: jreeder
 * 
 * Created on May 17, 2010, 5:47 PM
 */

#include "structure/network.h"

using namespace JRNN;

network::network() {
//    this->numIn = numIn;
//    this->numOut = numOut;
//    this->numHid = numHid;
//    layers.insert(std::pair<std::string, layerPtr>("input", layerPtr(new layer(layer::input,numIn,0,"input"))));
//    layers.insert(std::pair<std::string, layerPtr>("hidden", layerPtr(new layer(layer::hidden, numHid, 1,"hidden"))));
//    layers.insert(std::pair<std::string, layerPtr>("out", layerPtr(new layer(layer::out,numOut, 2,"out"))));
//    layers.insert(std::pair<std::string, layerPtr>("bias", layerPtr(new layer(layer::bias, 1, -1, "bias"))));
//    layers["input"]->buildLayer(node::linear);
//    layers["hidden"]->buildLayer(node::sigmoid);
//    layers["out"]->buildLayer(node::sigmoid);
//    layers["bias"]->buildLayer(node::bias);
//    fullyConnect();
}

networkPtr network::createFFMLPNetwork(int numIn, int numHid, int numOut){
    networkPtr np(new network());
    np->numIn = numIn;
    np->numOut = numOut;
    np->numHid = numHid;
	np->numHidLayers = 1;
    np->layers.insert(std::pair<std::string, layerPtr>("input", layerPtr(new layer(layer::input,numIn,0,"input"))));
    np->layers.insert(std::pair<std::string, layerPtr>("hidden", layerPtr(new layer(layer::hidden, numHid, 1,"hidden"))));
    np->layers.insert(std::pair<std::string, layerPtr>("out", layerPtr(new layer(layer::out,numOut, 2,"out"))));
    np->layers.insert(std::pair<std::string, layerPtr>("bias", layerPtr(new layer(layer::bias, 1, -1, "bias"))));
    np->layers["input"]->buildLayer(node::linear);
    np->layers["hidden"]->buildLayer(node::sigmoid);
    np->layers["out"]->buildLayer(node::sigmoid);
    np->layers["bias"]->buildLayer(node::bias);
    np->fullyConnectFFMLP();
    return np;
}

networkPtr network::createMinFFNetwork(int numIn, int numOut){
	networkPtr np(new network());
	np->numIn = numIn;
	np->numOut = numOut;
	np->numHidLayers = 0;
	np->layers.insert(std::pair<std::string, layerPtr>("input", layerPtr(new layer(layer::input,numIn,0,"input"))));
	np->layers.insert(std::pair<std::string, layerPtr>("out", layerPtr(new layer(layer::out, numOut, 2, "out"))));
	np->layers.insert(std::pair<std::string, layerPtr>("bias", layerPtr(new layer(layer::bias, 1, -1, "bias"))));
	np->layers["input"]->buildLayer(node::linear);
	np->layers["hidden"]->buildLayer(node::sigmoid);
	np->layers["bias"]->buildLayer(node::bias);
	np->fullyConnectMinFF();
	return np;
}


void network::fullyConnectFFMLP(){
    nodeList inputNodes = layers["input"]->getNodes();
    nodeList hidNodes = layers["hidden"]->getNodes();
    nodeList outNodes = layers["out"]->getNodes();
    nodeList biasNodes = layers["bias"]->getNodes();

    layers["input"]->setNextLayer(layers["hidden"]);
    layers["hidden"]->setPrevLayer(layers["input"]);
    layers["hidden"]->setNextLayer(layers["out"]);
    layers["out"]->setPrevLayer(layers["hidden"]);

    connection::setRandomSeed();

    BOOST_FOREACH(nodePtr n, inputNodes){
        BOOST_FOREACH(nodePtr n2, hidNodes){
            connections.push_back(connection::connect(n,n2));
        }
    }

    BOOST_FOREACH(nodePtr n, hidNodes){
        BOOST_FOREACH(nodePtr n2, outNodes){
            connections.push_back(connection::connect(n,n2));
        }
    }

    BOOST_FOREACH(nodePtr n, biasNodes){
        BOOST_FOREACH(nodePtr n2, hidNodes){
            connections.push_back(connection::connect(n,n2));
        }
        BOOST_FOREACH(nodePtr n3, outNodes){
            connections.push_back(connection::connect(n,n3));
        }
    }
}

void network::fullyConnectMinFF(){
	nodeList inputNodes = layers["input"]->getNodes();
	nodeList outNodes = layers["out"]->getNodes();
	nodeList biasNodes = layers["bias"]->getNodes();
	
	layers["input"]->setNextLayer(layers["out"]);
	layers["out"]->setPrevLayer(layers["input"]);
	connection::setRandomSeed();

	BOOST_FOREACH(nodePtr n, inputNodes){
		BOOST_FOREACH(nodePtr n2,outNodes){
			connections.push_back(connection::connect(n,n2));
		}
	}

	BOOST_FOREACH(nodePtr n, biasNodes){
		BOOST_FOREACH(nodePtr n2, outNodes){
			connections.push_back(connection::connect(n,n2));
		}
	}
}

void network::fullyConnectBack(layerPtr layer){
	nodeList layerNodes = layer->getNodes();
	layerPtr prevLayer = layer->getPrevLayer();

	connection::setRandomSeed();
	while(prevLayer){
		nodeList prevNodes = prevLayer->getNodes();
		BOOST_FOREACH(nodePtr n, prevNodes){
			BOOST_FOREACH(nodePtr n2, layerNodes){
				connections.push_back(connection::connect(n,n2));
			}
		}
		prevLayer = prevLayer->getPrevLayer();
	}
}

void network::fullyConnectOut( layerPtr layer ){
	nodeList layerNodes = layer->getNodes();
	nodeList outNodes = layers["out"]->getNodes();

	connection::setRandomSeed();
	BOOST_FOREACH(nodePtr n, layerNodes){
		BOOST_FOREACH(nodePtr n2, outNodes){
			connections.push_back(connection::connect(n,n2));
		}
	}
}

//network::network(const network& orig) {
//}

network::~network() {
    connections.clear();
    layers.clear();
}

vecDouble network::getOutputs(){
    return outputs;
}

layerPtr network::getLayer(std::string layerName){
    return layers[layerName];
}

void network::activate(vecDouble inputs){
    setInputs(inputs);
    layers["input"]->activate(this->inputs);
    layers["bias"]->activate();
    /*layers["hidden"]->activate();
    layers["out"]->activate();*/
	activate(layers["input"]->getNextLayer());
    outputs = layers["out"]->getOutput();
}

void network::activate(layerPtr layer){
	layer->activate();
	if (layer->getName() != "out"){
		activate(layer->getNextLayer());
	}
}

void network::setInputs(vecDouble inputs){
    inputs = inputs;
}

conList& network::getConnections(){
    return connections;
}

 void network::reset(){
     BOOST_FOREACH(conPtr con, this->connections){
         con->reset();
     }
 }

 hashedDoubleMap network::getWeights(){
     hashedDoubleMap weights;
     BOOST_FOREACH(conPtr con, this->connections){
         weights[con->getName()] = con->getWeight();
     }
     return weights;
 }

 void network::setWeights(hashedDoubleMap weights){
     //TODO: need to do some error handling in here this could potentially be dangerous
     BOOST_FOREACH(conPtr con, this->connections){
         double tmp = weights[con->getName()];
         //cout << "setting weight for " << con->getName() << " " << tmp << endl;
         con->setWeight(weights[con->getName()]);
     }
 }

 void network::printConnections(){
     cout << "Connections:" << endl;
     BOOST_FOREACH(conPtr con, this->connections){
         cout << con->getName() << " " << con->getWeight() << endl;
     }
 }
