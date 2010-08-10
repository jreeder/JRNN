/* 
 * File:   connection.cpp
 * Author: jreeder
 * 
 * Created on May 14, 2010, 2:32 PM
 */
#include "JRNN.h"
#include "structure/connection.h"

using namespace JRNN;

RandomGenerator Connection::cRand = RandomGenerator();

Connection::Connection() {
    weight = RandomWeight();
    value = 0;
    weightedValue = 0;
    name = "NONE";
    locked = false;
    inNode = NodePtr();
    outNode = NodePtr();
}

Connection::Connection(const Connection& orig) {
    weight = orig.weight;
    value = orig.value;
    weightedValue = orig.weightedValue;
    locked = orig.locked;
    name = orig.name;
    //inNode = orig.inNode;
    //outNode = orig.outNode;
	inNode = NodePtr();
	outNode = NodePtr();
}

Connection::Connection(double newDouble){
    weight = newDouble;
    value = 0;
    weightedValue = 0;
    locked = false;
    name = "NONE";
    inNode = NodePtr();
    outNode = NodePtr();
}

ConPtr Connection::Connect(NodePtr newInNode, NodePtr newOutNode){
    ConPtr p(new Connection());
    p->inNode = newInNode;
    p->outNode = newOutNode;
    p->inNode->AddConnection(OUT,p);
    p->outNode->AddConnection(IN,p);
    p->SetName();
    return p;
}

ConPtr Connection::Connect(NodePtr newInNode, NodePtr newOutNode, double weight){
    ConPtr p(new Connection(weight));
    p->inNode = newInNode;
    p->outNode = newOutNode;
    p->inNode->AddConnection(OUT,p);
    p->outNode->AddConnection(IN,p);
    p->SetName();
    return p;
}

ConPtr Connection::Clone( ConPtr con, NodePtr inNode, NodePtr outNode )
{
	ConPtr cp(new Connection((*con)));
	cp->SetInNode(inNode);
	cp->SetOutNode(outNode);
	cp->inNode->AddConnection(OUT,cp);
	cp->outNode->AddConnection(IN,cp);
	return cp;
}

void Connection::Disconnect(){
    inNode->RemoveConnection(name);
	inNode.reset();
    outNode->RemoveConnection(name);
	outNode.reset();
}

void Connection::Disconnect(string nodeName){
	if (inNode->GetName() == nodeName){
		outNode->RemoveConnection(name);
	}
	else{
		inNode->RemoveConnection(name);
	}
	inNode.reset();
	outNode.reset();
}

void Connection::SetRandomSeed(){
    //srand( time(NULL));
	cRand.gen.seed(static_cast<unsigned int>(time(NULL)));
}

void Connection::SetName(){
    string tmpName = "";
    tmpName += inNode->GetName();
    tmpName += "->";
    tmpName += outNode->GetName();
    SetName(tmpName);
}

Connection::~Connection() {
}

double Connection::GetWeightedValue(){
	Update();
    return weightedValue;
}

double Connection::GetValue(){
    return value;
}

void Connection::SetInNode(NodePtr newInNode){
    inNode = newInNode;
    inNode->AddConnection(OUT,ConPtr(this));
}

void Connection::SetOutNode(NodePtr newOutNode){
    outNode = newOutNode;
    outNode->AddConnection(IN, ConPtr(this));
}

void Connection::SetLocked(bool lock){
    locked = lock;
}

void Connection::Update(){
    value = inNode->GetOut();
    weightedValue = weight * value;
}

void Connection::Reset(){
    weight = RandomWeight();
}

void Connection::SetWeight(double weight) {
    this->weight = weight;
}
double Connection::GetWeight() const {
    return weight;
}

Connection Connection::operator +(const Connection& rhs) {
    Connection tmp;
    tmp.weight = weight + rhs.weight;
    return tmp;
}

Connection Connection::operator *(const Connection& rhs) {
    Connection tmp;
    tmp.weight = weight * rhs.weight;
    return tmp;
}

double Connection::RandomWeight(double scale, double offset){
    double tmpDouble;
	//UniformDist01 dist;
	//RandGenerator randgen(gen,dist);
    //tmpDouble = (((rand() % 10000 + 1)/(double)10000) * scale) - offset;
	tmpDouble = ((cRand() * scale)) - offset;
    return tmpDouble;
}

const string& Connection::GetName(){
    return name;
}

void Connection::SetName(string newName){
    name = newName;
}

const string& Connection::GetOutNodeName(){
    return outNode->GetName();
}

const string& Connection::GetInNodeName(){
    return inNode->GetName();
}
