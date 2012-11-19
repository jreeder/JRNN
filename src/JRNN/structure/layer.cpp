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
	shallowLayer = false;
}

Layer::Layer(layerType type, int inLayerSize, int height, string name, bool shallow /*= false*/) {
    this->type = type;
    this->layerSize = inLayerSize;
    this->height = height;
    this->name = name;
	this->shallowLayer = shallow;
}

LayerPtr Layer::CreateLayer(layerType type, int inLayerSize, int height, string name, bool shallow /*= false*/){
	LayerPtr lp(new Layer(type,inLayerSize,height, name, shallow));
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

bool Layer::HasNextL(){
	return nextLayer.get() == 0 ? false : true;
}

void Layer::SetPrevLayer(LayerPtr prevLayer) {
    this->prevLayer = prevLayer;
}
LayerPtr Layer::GetPrevLayer() const {
    return prevLayer;
}

bool Layer::HasPrevL(){
	return prevLayer.get() == 0 ? false : true;
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
	if (layerSize != nodes.size()){
		layerSize = nodes.size(); //just in case it gets out of sync. 
	}
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

void Layer::AddNode( NodePtr node, bool shallow /*= false*/ ){
	if (!shallow & !this->shallowLayer){
		string tmpName = name + "_";
		tmpName += lexical_cast<string>(layerSize); //Numbers should start with zero so size is the next number
		node->SetName(tmpName);
		node->SetHeight(height);
	}
	nodes.push_back(node);
	layerSize++;
}

//void Layer::AddNode( NodePtr node, bool createName){
//	if (createName){
//		AddNode(node);
//	}
//	else {
//		nodes.push_back(node);
//		layerSize++;
//	}
//}

void Layer::InsertNode( NodePtr node, int pos, bool shallow /*= false*/ )
{
	if (!shallow & !this->shallowLayer){
		string tmpName = name + "_" + lexical_cast<string>(pos);
		node->SetName(tmpName);
		node->SetHeight(this->height);
	}
	NodeList::iterator it = nodes.begin();
	nodes.insert(it+pos, node);
	layerSize++;
}

NodeList Layer::ResetNodeNames()
{
	NodeList nodesChanged;

	if (!this->shallowLayer){
		for (uint i = 0; i < this->nodes.size(); i++){
			NodePtr node = this->nodes[i];
			string nodeName = this->name + "_" + lexical_cast<string>(i);
			if (nodeName != node->GetName()){
				node->SetName(nodeName);
				nodesChanged.push_back(node);
			}
		}
	}
	return nodesChanged;
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
		layerSize++;
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

void Layer::SetTypeByName( string type )
{
	if (type == "bias"){
		this->type = Layer::bias;
	} 
	else if (type == "hidden"){
		this->type = Layer::hidden;
	} 
	else if (type == "input"){
		this->type = Layer::input;
	} 
	else if (type == "out"){
		this->type = Layer::out;
	}
	else if (type == "spec"){
		this->type = Layer::spec;
	}
	else {
		this->type = Layer::hidden;
	}
}

string Layer::GetTypeName()
{
	switch (type)
	{
	case Layer::bias:
		return "bias";
	case Layer::hidden:
		return "hidden";
	case Layer::input:
		return "input";
	case Layer::out:
		return "out";
	case Layer::spec:
		return "spec";
	default:
		return "";
	}
}

bool JRNN::Layer::GetShallowLayer() const
{
	return shallowLayer;
}

void JRNN::Layer::SetShallowLayer( bool val )
{
	shallowLayer = val;
}
