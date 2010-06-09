/* 
 * File:   connection.cpp
 * Author: jreeder
 * 
 * Created on May 14, 2010, 2:32 PM
 */

#include "JRNN.h"

using namespace JRNN;

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
    inNode = orig.inNode;
    outNode = orig.outNode;
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
    p->inNode->AddConnection(Node::OUT,p);
    p->outNode->AddConnection(Node::IN,p);
    p->SetName();
    return p;
}

ConPtr Connection::Connect(NodePtr newInNode, NodePtr newOutNode, double weight){
    ConPtr p(new Connection(weight));
    p->inNode = newInNode;
    p->outNode = newOutNode;
    p->inNode->AddConnection(Node::OUT,p);
    p->outNode->AddConnection(Node::IN,p);
    p->SetName();
    return p;
}

void Connection::Disconnect(){
    inNode->RemoveConnection(name);
    outNode->RemoveConnection(name);
}

void Connection::SetRandomSeed(){
    srand( time(NULL));
}

void Connection::SetName(){
    std::string tmpName = "";
    tmpName += inNode->GetName();
    tmpName += "->";
    tmpName += outNode->GetName();
    SetName(tmpName);
}

Connection::~Connection() {
}

double Connection::GetWeightedValue(){
    return weightedValue;
}

double Connection::GetValue(){
    return value;
}

void Connection::SetInNode(NodePtr newInNode){
    inNode = newInNode;
    inNode->AddConnection(Node::OUT,ConPtr(this));
}

void Connection::SetOutNode(NodePtr newOutNode){
    outNode = newOutNode;
    outNode->AddConnection(Node::IN, ConPtr(this));
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
    weight = weight;
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

double Connection::RandomWeight(){
    double tmpDouble;
    tmpDouble = ((rand() % 10000 + 1)/(double)10000)-0.5;
    return tmpDouble;
}

const std::string& Connection::GetName(){
    return name;
}

void Connection::SetName(std::string newName){
    name = newName;
}

const std::string& Connection::GetOutNodeName(){
    return outNode->GetName();
}

const std::string& Connection::GetInNodeName(){
    return inNode->GetName();
}
