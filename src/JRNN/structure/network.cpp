/* 
 * File:   network.cpp
 * Author: jreeder
 * 
 * Created on May 17, 2010, 5:47 PM
 */

#include "structure/network.h"

using namespace JRNN;

network::network(int numIn, int numHid, int numOut) {
    this->numIn = numIn;
    this->numOut = numOut;
    this->numHid = numHid;
    layers.insert(std::pair<std::string, layerPtr>("input", layerPtr(new layer(layer::input,numIn,0,"input"))));
    layers.insert(std::pair<std::string, layerPtr>("hidden", layerPtr(new layer(layer::hidden, numHid, 1,"hidden"))));
    layers.insert(std::pair<std::string, layerPtr>("out", layerPtr(new layer(layer::out,numOut, 2,"out"))));
    layers.insert(std::pair<std::string, layerPtr>("bias", layerPtr(new layer(layer::bias, 1, -1, "bias"))));
    layers["input"]->buildLayer(node::linear);
    layers["hidden"]->buildLayer(node::sigmoid);
    layers["out"]->buildLayer(node::sigmoid);
    layers["bias"]->buildLayer(node::bias);
    fullyConnect();
}

void network::fullyConnect(){
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
            this->connections.push_back(connection::connect(n,n2));
        }
    }

    BOOST_FOREACH(nodePtr n, hidNodes){
        BOOST_FOREACH(nodePtr n2, outNodes){
            this->connections.push_back(connection::connect(n,n2));
        }
    }

    BOOST_FOREACH(nodePtr n, biasNodes){
        BOOST_FOREACH(nodePtr n2, hidNodes){
            this->connections.push_back(connection::connect(n,n2));
        }
        BOOST_FOREACH(nodePtr n3, outNodes){
            this->connections.push_back(connection::connect(n,n3));
        }
    }
}

//network::network(const network& orig) {
//}

network::~network() {
    this->connections.clear();
    this->layers.clear();
}

vecDouble network::getOutputs(){
    return this->outputs;
}

layerPtr network::getLayer(std::string layerName){
    return this->layers[layerName];
}

void network::activate(vecDouble inputs){
    this->setInputs(inputs);
    layers["input"]->activate(this->inputs);
    layers["bias"]->activate();
    layers["hidden"]->activate();
    layers["out"]->activate();
    this->outputs = layers["out"]->getOutput();
}

void network::setInputs(vecDouble inputs){
    this->inputs = inputs;
}

conList& network::getConnections(){
    return this->connections;
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