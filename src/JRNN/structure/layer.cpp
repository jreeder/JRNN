/* 
 * File:   layer.cpp
 * Author: jreeder
 * 
 * Created on May 16, 2010, 8:35 PM
 */

#include "JRNN.h"

using namespace JRNN;

layer::layer(){
    this->type = layer::hidden;
    this->layerSize = 0;
    this->height = 0;
    this->name = "NONE";
}

layer::layer(layerType type, int inLayerSize, int height, std::string name) {
    this->type = type;
    this->layerSize = inLayerSize;
    this->height = height;
    this->name = name;
}

layer::layer(const layer& orig) {
}

void layer::setNextLayer(layerPtr nextLayer) {
    this->nextLayer = nextLayer;
}
layerPtr layer::getNextLayer() const {
    return nextLayer;
}
void layer::setPrevLayer(layerPtr prevLayer) {
    this->prevLayer = prevLayer;
}
layerPtr layer::getPrevLayer() const {
    return prevLayer;
}
void layer::setLayerSize(int layerSize) {
    this->layerSize = layerSize;
}
int layer::getLayerSize() const {
    return layerSize;
}
void layer::setType(layerType type) {
    this->type = type;
}
layer::layerType layer::getType() const {
    return type;
}

void layer::buildLayer(node::nodeType nType){
    nodePtr np;
    node::nodeType nt = nType;
    
//    nt = (this->type != layer::input) ? node::sigmoid : node::linear;
    std::string baseName = this->name + "_";
    for (int i = 0; i < this->layerSize; i++){
        std::string num = lexical_cast<std::string>(i);
        std::string name = baseName + num;
        np.reset(new node(this->height,nt,name));
        nodes.push_back(np);
    }
}

nodeList& layer::getNodes(){
    return nodes;
}

void layer::activate(){
    BOOST_FOREACH(nodePtr n, nodes){
        n->activate();
    }
}

vecDouble layer::getOutput(){
    vecDouble tmpVec;
    tmpVec.resize(nodes.size());
    int i = 0;
    BOOST_FOREACH(nodePtr n, nodes){
        tmpVec[i] = n->getOut();
        i++;
    }
    return tmpVec;
}

void layer::activate(vecDouble inputs){
    for(int i = 0; i < inputs.size(); i++){
        nodes[i]->activate(inputs[i]);
        std::string tmp = nodes[i]->getName();
    }
}

const std::string& layer::getName(){
    return this->name;
}

void layer::setName(std::string newName){
    this->name = newName;
}

int layer::getSize(){
    return this->layerSize;
}

layer::~layer() {
    //nodes.clear();
}

