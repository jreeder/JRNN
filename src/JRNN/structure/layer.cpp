/* 
 * File:   layer.cpp
 * Author: jreeder
 * 
 * Created on May 16, 2010, 8:35 PM
 */
#include "JRNN.h"
#include "structure/layer.h"

using namespace JRNN;

Layer::Layer(){
    type = Layer::hidden;
    layerSize = 0;
    height = 0;
    name = "NONE";
}

Layer::Layer(layerType type, int inLayerSize, int height, std::string name) {
    this->type = type;
    this->layerSize = inLayerSize;
    this->height = height;
    this->name = name;
}

LayerPtr Layer::CreateLayer(layerType type, int inLayerSize, int height, std::string name){
	LayerPtr lp(new Layer(type,inLayerSize,height, name));
	return lp;
}

Layer::Layer(const Layer& orig) {
}

void Layer::SetNextLayer(LayerPtr nextLayer) {
    this->nextLayer = nextLayer;
}
LayerPtr Layer::GetNextLayer() const {
    return nextLayer;
}
void Layer::SetPrevLayer(LayerPtr prevLayer) {
    this->prevLayer = prevLayer;
}
LayerPtr Layer::GetPrevLayer() const {
    return prevLayer;
}
void Layer::SetLayerSize(int layerSize) {
    this->layerSize = layerSize;
}
int Layer::GetLayerSize() const {
    return layerSize;
}
void Layer::SetType(layerType type) {
    this->type = type;
}
Layer::layerType Layer::GetType() const {
    return type;
}

//void Layer::BuildLayer(Node::nodeType nType){
//    NodePtr np;
//    Node::nodeType nt = nType;
//    
////    nt = (type != layer::input) ? node::sigmoid : node::linear;
//    std::string baseName = name + "_";
//    for (int i = 0; i < layerSize; i++){
//        std::string num = lexical_cast<std::string>(i);
//        std::string name = baseName + num;
//        np.reset(new Node(height,nt,name));
//        nodes.push_back(np);
//    }
//}

NodeList& Layer::GetNodes(){
    return nodes;
}

void Layer::Activate(){
    BOOST_FOREACH(NodePtr n, nodes){
        n->Activate();
    }
}

vecDouble Layer::GetOutput(){
    vecDouble tmpVec;
    tmpVec.resize(nodes.size());
    int i = 0;
    BOOST_FOREACH(NodePtr n, nodes){
        tmpVec[i] = n->GetOut();
        i++;
    }
    return tmpVec;
}

void Layer::Activate(vecDouble inputs){
    for(unsigned int i = 0; i < inputs.size(); i++){
        nodes[i]->Activate(inputs[i]);
        std::string tmp = nodes[i]->GetName();
    }
}

const std::string& Layer::GetName(){
    return name;
}

void Layer::SetName(std::string newName){
    name = newName;
}

int Layer::GetSize(){
    return layerSize;
}

Layer::~Layer() {
    //nodes.clear();
}
