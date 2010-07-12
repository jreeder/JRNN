/* 
 * File:   node.cpp
 * Author: jreeder
 * 
 * Created on May 14, 2010, 3:00 AM
 */
#include "JRNN.h"
#include "structure/node.h"

using namespace JRNN;


Node::Node() {
    height = 0;
//    numIn = 0;
//    numOut = 0;
//    out = 0;
//    sumOfIn = 0;
//    sigSteepness = 5;
    name = "NONE";
//    nextIn = 0;
    //nextOut = 0;
    //type = Node::sigmoid;
    SetDefaults();
//    inputs = vector<double>(0);
//    inputWeights = vector<double>(0);
//    inConnections = vector<connection>(0);
//    outConnections = vector<connection>(0);
}


Node::Node(int inHeight, std::string nodeName){
	//Node::Node(int inHeight, nodeType nType, std::string nodeName){
    height = inHeight;
//    numIn = inNumIn;
//    numOut = inNumOut;
//    out = 0;
//    sumOfIn = 0;
//    sigSteepness = 5;
    name = nodeName;
//    nextIn = 0;
    //nextOut = 0;
    //type = nType;
    SetDefaults();
//    inputs = vector<double>(numIn);
//    inputWeights = vector<double>(numOut);
//    inConnections = vector<connection>(numIn);
//    outConnections = vector<connection>(numOut);
}


void Node::SetDefaults(){
    out = 0;
    sumOfIn = 0;
    sigSteepness = 1;
}


Node::Node(const Node& orig) {
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
	//isBias = orig.isBias;
    inConnections = orig.inConnections;
    outConnections = orig.outConnections;
}


Node::~Node() {
//    inConnections.clear();
//    outConnections.clear();
//    inputs.clear();
//    inputWeights.clear();
}


void Node::Activate() {
    //TODO:standard sigmoid right now will abstract later
//    int i = 0;
//    BOOST_FOREACH(node n, inputNodes){
//        inputs[i] = n.getOut();
//        i++;
//    }
    //if(type != Node::bias){
    //    sumOfIn = SumWeightedIn();
    //}
	sumOfIn = SumWeightedIn();
    UpdateOut();
}


void Node::Activate(double input){
    sumOfIn = input;
    UpdateOut();
}


void Node::UpdateOut(){
    /*switch(type){
        case Node::sigmoid:
            out = 1 / (1 + exp(-sigSteepness*sumOfIn));
			if (out == 0){
				out = 0.0000001;
			}
			else if (out == 1){
				out = 0.9999999;
			}
            break;
        case Node::linear:
            out = sumOfIn;
            break;
        default:
            out = 1;
            break;
    }*/
	out = actFunction->activate(sumOfIn);
    BOOST_FOREACH(ConPtr con, outConnections){
        con->Update();
    }
}


double Node::SumWeightedIn(){
    double tmpSum = 0;
    BOOST_FOREACH(ConPtr con, inConnections){
        tmpSum += con->GetWeightedValue();
    }
    return tmpSum;
}


void Node::SetHeight(int newHeight){
    height = newHeight;
}


double Node::GetOut(){
    return out;
}


double Node::GetPrime(){
	return actFunction->prime(out, sumOfIn);
}


const std::string& Node::GetName(){
    return name;
}

//double Node::GetSigSteepness(){
//    return sigSteepness;
//}


void Node::SetName(std::string newName){
    name = newName;
}

//void Node::setNumIn(int newNumIn){
//    numIn = newNumIn;
//    inputs.resize(newNumIn,true);
//    inputWeights.resize(newNumIn, true);
//}
//
//void Node::setNumOut(int newNumOut){
//    numOut = newNumOut;
//    inputs.resize(newNumOut, true);
//    inputWeights.resize(newNumOut, true);
//}


bool Node::AddConnection(conType type, ConPtr newCon){
    bool returnVal = false;
    switch(type){
        case IN:
            inConnections.push_back(newCon);
            returnVal = true;
            break;
        case OUT:
            outConnections.push_back(newCon);
            returnVal = true;
            break;
        default:
            returnVal = false;
    }
    return returnVal;
}


void Node::RemoveConnection(std::string name){
    ConList::iterator incons = inConnections.begin();
    bool found = false;
    while(incons != inConnections.end()){
        if ((*incons)->GetName() == name){
            found = true;
            inConnections.erase(incons);
            break;
        }
        else {
            incons++;
        }
    }
    if (!found) {
        ConList::iterator outcons = outConnections.begin();
        while(outcons != outConnections.end()){
            if ((*outcons)->GetName() == name){
                outConnections.erase(outcons);
                break;
            }
            else {
                outcons++;
            }
        }
    }
}


ConList& Node::GetConnections(conType type){
    return type == IN ? inConnections : outConnections;
}

//bool Node::addConnection(node* newNodeCon, double conWeight){
//    //TODO: add tests to make sure size constraints are met.
//    bool returnVal = true;
//    inputNodes.push_back(newNodeCon);
//    inputWeights[nextIn] = conWeight;
//    nextIn++;
//    return returnVal;
//}
//
//bool Node::addConnections(nodeList inNodes, vector<double> conWeights){
//    //TODO: add tests to make sure size constraints are met.
//    inputNodes = inNodes;
//    inputWeights = conWeights;
//    return true;
//}

//vector<double> Node::vecMul(vector<double> vec1, vector<double> vec2){
//    vector<double> tmp(vec1.size());
//    for (int i = 0; i < vec1.size(); i++){
//        tmp[i] = vec1[i] * vec2[i];
//    }
//    return tmp;
//}
