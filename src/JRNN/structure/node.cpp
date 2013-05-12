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


Node::Node(int inHeight, string nodeName){
	//Node::Node(int inHeight, nodeType nType, string nodeName){
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
	numConnections = 0;
}


Node::Node(const Node& orig) {
    height = orig.height;
//    numIn = orig.numIn;
//    numOut = orig.numOut;
    out = orig.out;
    sumOfIn = orig.sumOfIn;
    sigSteepness = orig.sigSteepness;
	//numConnections = orig.numConnections;
    name = orig.name;
	actFunction.reset(ActivationFunction::Create(orig.actFunction->getType()));
//    nextIn = orig.nextIn;
//    nextOut = orig.nextOut;
//    inputs = orig.inputs;
//    inputWeights = orig.inputWeights;
	//isBias = orig.isBias;
    //inConnections = orig.inConnections;
    //outConnections = orig.outConnections;
}


NodePtr Node::Clone()
{
	NodePtr np(new Node((*this)));
	return np;
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
	//this was a redundant loop. I'm already looping through them on the other side. 
	//I just do the update there now. 
    /*BOOST_FOREACH(ConPtr con, outConnections){
        con->Update();
    }*/
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


const string& Node::GetName(){
    return name;
}

//double Node::GetSigSteepness(){
//    return sigSteepness;
//}


void Node::RenameConnections() 
{
	BOOST_FOREACH(ConPtr con, inConnections){
		con->SetName();
	}
	BOOST_FOREACH(ConPtr con, outConnections){
		con->SetName();
	}
}

void Node::SetName(string newName){
    name = newName;
	RenameConnections();
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
			numConnections++;
            break;
        case OUT:
            outConnections.push_back(newCon);
            returnVal = true;
			numConnections++;
            break;
        default:
            returnVal = false;
    }
    return returnVal;
}


void Node::RemoveConnection(string name){
    ConList::iterator incons = inConnections.begin();
    bool found = false;
    while(incons != inConnections.end()){
        if ((*incons)->GetName() == name){
            found = true;
            inConnections.erase(incons);
			numConnections--;
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
				numConnections--;
                break;
            }
            else {
                outcons++;
            }
        }
    }
}

void Node::Disconnect()
{
	BOOST_FOREACH(ConPtr con, inConnections){
		con->Disconnect(name);
	}
	BOOST_FOREACH(ConPtr con, outConnections){
		con->Disconnect(name);
	}
	inConnections.clear();
	outConnections.clear();
	numConnections = 0;
}

ConList& Node::GetConnections(conType type){
    return type == IN ? inConnections : outConnections;
}

int Node::GetNumConnections()
{
	return numConnections;
}

string Node::GetActFuncType()
{
	return actFunction->getType();
}

int Node::GetHeight()
{
	return height;
}

bool Node::operator==( const Node& rhs ) const
{
	if (this->name != rhs.name){
		return false;
	}
	else if (this->inConnections.size() != rhs.inConnections.size() || this->outConnections.size() != rhs.outConnections.size()){
		return false;
	}
	else if (this->actFunction->getType() != rhs.actFunction->getType()){
		return false;
	}
	else {
		ConList::const_iterator thisIT = this->inConnections.begin();
		ConList::const_iterator rhsIT = rhs.inConnections.begin();
		for (;thisIT != this->inConnections.end(); thisIT++, rhsIT++){
			if (**thisIT != **rhsIT) {
				return false;
			}
		}
		thisIT = this->outConnections.begin();
		rhsIT = rhs.outConnections.begin();
		for (;thisIT != this->outConnections.end(); thisIT++, rhsIT++){
			if (**thisIT != **rhsIT){
				return false;
			}
		}
		return true;
	}
}

bool Node::operator != ( const Node& rhs) const
{
	return !(*this == rhs);
}

hashedDoubleMap Node::GetInConWeights()
{
	hashedDoubleMap retMap;
	ConList inCons = this->GetConnections(IN);
	BOOST_FOREACH(ConPtr con, inCons){
		retMap[con->GetName()] = con->GetWeight();
	}
	return retMap;
}

void Node::SetInConWeights( hashedDoubleMap inWeights )
{
	ConList inCons = this->GetConnections(IN);
	BOOST_FOREACH(ConPtr con, inCons){
		con->SetWeight(inWeights[con->GetName()]);
	}
}
