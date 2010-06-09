/* 
 * File:   node.cpp
 * Author: jreeder
 * 
 * Created on May 14, 2010, 3:00 AM
 */

#include "JRNN.h"

using namespace JRNN;

node::node() {
    height = 0;
//    numIn = 0;
//    numOut = 0;
//    out = 0;
//    sumOfIn = 0;
//    sigSteepness = 5;
    name = "NONE";
//    nextIn = 0;
    //nextOut = 0;
    type = node::sigmoid;
    setDefaults();
//    inputs = vector<double>(0);
//    inputWeights = vector<double>(0);
//    inConnections = vector<connection>(0);
//    outConnections = vector<connection>(0);
}

node::node(int inHeight, nodeType nType, std::string nodeName){
    height = inHeight;
//    numIn = inNumIn;
//    numOut = inNumOut;
//    out = 0;
//    sumOfIn = 0;
//    sigSteepness = 5;
    name = nodeName;
//    nextIn = 0;
    //nextOut = 0;
    type = nType;
    setDefaults();
//    inputs = vector<double>(numIn);
//    inputWeights = vector<double>(numOut);
//    inConnections = vector<connection>(numIn);
//    outConnections = vector<connection>(numOut);
}

void node::setDefaults(){
    out = 0;
    sumOfIn = 0;
    sigSteepness = 1;
}

node::node(const node& orig) {
    height = orig.height;
//    numIn = orig.numIn;
//    numOut = orig.numOut;
    out = orig.out;
    sumOfIn = orig.sumOfIn;
    sigSteepness = orig.sigSteepness;
    name = orig.name;
//    nextIn = orig.nextIn;
//    nextOut = orig.nextOut;
//    inputs = orig.inputs;
//    inputWeights = orig.inputWeights;
    inConnections = orig.inConnections;
    outConnections = orig.outConnections;
}

node::~node() {
//    inConnections.clear();
//    outConnections.clear();
//    inputs.clear();
//    inputWeights.clear();
}

void node::activate() {
    //TODO:standard sigmoid right now will abstract later
//    int i = 0;
//    BOOST_FOREACH(node n, inputNodes){
//        inputs[i] = n.getOut();
//        i++;
//    }
    if(this->type != node::bias){
        sumOfIn = sumWeightedIn();
    }
    updateOut();
}

void node::activate(double input){
    sumOfIn = input;
    updateOut();
}

void node::updateOut(){
    switch(this->type){
        case node::sigmoid:
            out = 1 / (1 + exp(-sigSteepness*sumOfIn));
			if (out == 0){
				out = 0.0000001;
			}
			else if (out == 1){
				out = 0.9999999;
			}
            break;
        case node::linear:
            out = sumOfIn;
            break;
        default:
            out = 1;
            break;
    }
    BOOST_FOREACH(conPtr con, outConnections){
        con->update();
    }
}

double node::sumWeightedIn(){
    double tmpSum = 0;
    BOOST_FOREACH(conPtr con, this->inConnections){
        tmpSum += con->getWeightedValue();
    }
    return tmpSum;
}

void node::setHeight(int newHeight){
    height = newHeight;
}

double node::getOut(){
    return out;
}

const std::string& node::getName(){
    return this->name;
}

double node::getSigSteepness(){
    return this->sigSteepness;
}

void node::setName(std::string newName){
    this->name = newName;
}

//void node::setNumIn(int newNumIn){
//    this->numIn = newNumIn;
//    this->inputs.resize(newNumIn,true);
//    this->inputWeights.resize(newNumIn, true);
//}
//
//void node::setNumOut(int newNumOut){
//    this->numOut = newNumOut;
//    this->inputs.resize(newNumOut, true);
//    this->inputWeights.resize(newNumOut, true);
//}

bool node::addConnection(conType type, conPtr newCon){
    bool returnVal = false;
    switch(type){
        case node::IN:
            this->inConnections.push_back(newCon);
            returnVal = true;
            break;
        case node::OUT:
            this->outConnections.push_back(newCon);
            returnVal = true;
            break;
        default:
            returnVal = false;
    }
    return returnVal;
}

conList& node::getConnections(node::conType type){
    return type == node::IN ? this->inConnections : this->outConnections;
}

//bool node::addConnection(node* newNodeCon, double conWeight){
//    //TODO: add tests to make sure size constraints are met.
//    bool returnVal = true;
//    inputNodes.push_back(newNodeCon);
//    inputWeights[nextIn] = conWeight;
//    nextIn++;
//    return returnVal;
//}
//
//bool node::addConnections(nodeList inNodes, vector<double> conWeights){
//    //TODO: add tests to make sure size constraints are met.
//    inputNodes = inNodes;
//    inputWeights = conWeights;
//    return true;
//}

//vector<double> node::vecMul(vector<double> vec1, vector<double> vec2){
//    vector<double> tmp(vec1.size());
//    for (int i = 0; i < vec1.size(); i++){
//        tmp[i] = vec1[i] * vec2[i];
//    }
//    return tmp;
//}