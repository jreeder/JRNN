/* 
 * File:   connection.cpp
 * Author: jreeder
 * 
 * Created on May 14, 2010, 2:32 PM
 */

#include "JRNN.h"

using namespace JRNN;

connection::connection() {
    weight = randomWeight();
    value = 0;
    weightedValue = 0;
    name = "NONE";
    locked = false;
    inNode = nodePtr();
    outNode = nodePtr();
}

connection::connection(const connection& orig) {
    weight = orig.weight;
    value = orig.value;
    weightedValue = orig.weightedValue;
    locked = orig.locked;
    name = orig.name;
    inNode = orig.inNode;
    outNode = orig.outNode;
}

connection::connection(double newDouble){
    weight = newDouble;
    value = 0;
    weightedValue = 0;
    locked = false;
    name = "NONE";
    inNode = nodePtr();
    outNode = nodePtr();
}

conPtr connection::connect(nodePtr newInNode, nodePtr newOutNode){
    conPtr p(new connection());
    p->inNode = newInNode;
    p->outNode = newOutNode;
    p->inNode->addConnection(node::OUT,p);
    p->outNode->addConnection(node::IN,p);
    p->setName();
    return p;
}

conPtr connection::connect(nodePtr newInNode, nodePtr newOutNode, double weight){
    conPtr p(new connection(weight));
    p->inNode = newInNode;
    p->outNode = newOutNode;
    p->inNode->addConnection(node::OUT,p);
    p->outNode->addConnection(node::IN,p);
    p->setName();
    return p;
}

void connection::disconnect(){
    inNode->removeConnection(this->getName());
    outNode->removeConnection(this->getName());
}

void connection::setRandomSeed(){
    srand( time(NULL));
}

void connection::setName(){
    std::string tmpName = "";
    tmpName += inNode->getName();
    tmpName += "->";
    tmpName += outNode->getName();
    this->setName(tmpName);
}

connection::~connection() {
}

double connection::getWeightedValue(){
    return weightedValue;
}

double connection::getValue(){
    return value;
}

void connection::setInNode(nodePtr newInNode){
    inNode = newInNode;
    inNode->addConnection(node::OUT,conPtr(this));
}

void connection::setOutNode(nodePtr newOutNode){
    outNode = newOutNode;
    outNode->addConnection(node::IN, conPtr(this));
}

void connection::setLocked(bool lock){
    locked = lock;
}

void connection::update(){
    value = inNode->getOut();
    weightedValue = weight * value;
}

void connection::reset(){
    weight = this->randomWeight();
}

void connection::setWeight(double weight) {
    this->weight = weight;
}
double connection::getWeight() const {
    return weight;
}

connection connection::operator +(const connection& rhs) {
    connection tmp;
    tmp.weight = this->weight + rhs.weight;
    return tmp;
}

connection connection::operator *(const connection& rhs) {
    connection tmp;
    tmp.weight = this->weight * rhs.weight;
    return tmp;
}

double connection::randomWeight(){
    double tmpDouble;
    tmpDouble = ((rand() % 10000 + 1)/(double)10000)-0.5;
    return tmpDouble;
}

const std::string& connection::getName(){
    return this->name;
}

void connection::setName(std::string newName){
    this->name = newName;
}

const std::string& connection::getOutNodeName(){
    return this->outNode->getName();
}

const std::string& connection::getInNodeName(){
    return this->inNode->getName();
}
