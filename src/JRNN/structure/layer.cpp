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

Layer::Layer(layerType type, int inLayerSize, int height, string name) {
    this->type = type;
    this->layerSize = inLayerSize;
    this->height = height;
    this->name = name;
}

LayerPtr Layer::CreateLayer(layerType type, int inLayerSize, int height, string name){
	LayerPtr lp(new Layer(type,inLayerSize,height, name));
	return lp;
}

Layer::Layer(const Layer& orig) {
	type = orig.type;
	layerSize = orig.layerSize;
	height = orig.height;
	name = orig.name;
	NodePtr np;
	BOOST_FOREACH(NodePtr node, orig.nodes){
		np.reset(new Node((*node)));
		nodes.push_back(np);
	}
	np.reset();
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
//    string baseName = name + "_";
//    for (int i = 0; i < layerSize; i++){
//        string num = lexical_cast<string>(i);
//        string name = baseName + num;
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

void Layer::Activate(vecDouble inputs){
	assert(inputs.size() == layerSize);
	for(unsigned int i = 0; i < inputs.size(); i++){
		nodes[i]->Activate(inputs[i]);
		string tmp = nodes[i]->GetName();
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

vecDouble Layer::GetPrimes()
{
	vecDouble tmpVec;
	tmpVec.resize(nodes.size());
	int i = 0;
	BOOST_FOREACH(NodePtr n, nodes){
		tmpVec[i] = n->GetPrime();
		i++;
	}
	return tmpVec;
}

const string& Layer::GetName(){
    return name;
}

void Layer::SetName(string newName){
    name = newName;
}

int Layer::GetSize(){
    return layerSize;
}

Layer::~Layer() {
    Clear();
}

int Layer::GetHeight(){
	return height;
}

void Layer::SetHeight( int newHeight ){
	height = newHeight;
	BOOST_FOREACH(NodePtr n, nodes){
		n->SetHeight(newHeight);
	}
}

void Layer::AddNode( NodePtr node ){
	string tmpName = name + "_";
	tmpName += lexical_cast<string>(layerSize + 1);
	node->SetName(tmpName);
	node->SetHeight(height);
	nodes.push_back(node);
	layerSize++;
}

void Layer::RemoveNode(NodePtr node){
	NodeList::iterator it = nodes.begin();
	while (it != nodes.end()){
		if ((*it)->GetName() == node->GetName()){
			nodes.erase(it);
			layerSize--;
			break;
		}
		it++;
	}
}

void Layer::Clear(bool disconnect /*= true*/)
{
	if (disconnect){
		BOOST_FOREACH(NodePtr node, nodes){
			node->Disconnect();
		}
	}
	nodes.clear();
	layerSize = 0;
}

ConList Layer::GetConnections()
{
	ConList layerCons; 
	BOOST_FOREACH(NodePtr node, nodes){
		ConList inCons = node->GetConnections(IN);
		ConList outCons = node->GetConnections(OUT);
		layerCons.insert(layerCons.end(),inCons.begin(),inCons.end());
		layerCons.insert(layerCons.end(),outCons.begin(),outCons.end());
	}
	return layerCons;
}

int Layer::RemoveUnconnectedNodes()
{
	NodeList nodesToRemove;
	int nodesRemoved = 0;
	BOOST_FOREACH(NodePtr node, nodes){
		if (node->GetNumConnections() == 0){
			nodesToRemove.push_back(node);
		}
	}
	BOOST_FOREACH(NodePtr node, nodesToRemove){
		RemoveNode(node);
		nodesRemoved++;
	}
	nodesToRemove.clear();
	return nodesRemoved;
}

LayerPtr Layer::Clone( LayerPtr layer )
{
	LayerPtr lp(new Layer((*layer)));
	return lp;
}

void Layer::ShallowCopy(LayerPtr layer){
	//This function is meant to be called on an empty layer in order to make a 
	//reference copy. I'm using this to point to a subset of another layer. 
	nodes.clear();
	NodeList origNodes = layer->GetNodes();
	BOOST_FOREACH(NodePtr node, origNodes){
		nodes.push_back(node);
	}
}

NodePtr Layer::GetNodeByName( string name )
{
	NodePtr retNode;
	int pos = name.rfind("_");
	string nodeIndex = "";
	if (pos != string::npos){
		nodeIndex = name.substr(pos+1);
	}
	if (nodeIndex.size() > 0){
		int index = lexical_cast<int>(nodeIndex);
		retNode = nodes[index];
	}
	return retNode;
}
