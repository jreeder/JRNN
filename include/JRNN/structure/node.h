/* 
 * File:   node.h
 * Author: jreeder
 *
 * Created on May 14, 2010, 3:00 AM
 */

#ifndef _NODE_H
#define	_NODE_H

//#include <boost/numeric/ublas/vector.hpp>
#include "JRNN.h"
#include "structure/connection.h"
#include "utility/activationfunctions.h"

//using namespace boost::numeric::ublas;

namespace JRNN {
	class Node {
	public:

		/*enum nodeType {
			sigmoid,
			linear,
			bias
		};*/

		Node();
		//Node(int height, nodeType nType, string name);
		Node(int height, string name);
		Node(const Node& orig);
		template<typename T>
		static NodePtr CreateNode(int height, string name);
		virtual ~Node();
		void Activate();
		void Activate(double input);
		void SetHeight(int newHeight);
		int GetHeight();
		double GetOut();
		double GetPrime();
		int GetNumConnections();
		const string& GetName();
		//double GetSigSteepness();
		ConList& GetConnections(conType type);
        
		void SetName(string newName);
//        void setNumIn(int newNumIn);
//        void setNumOut(int newNumOut);
		bool AddConnection(conType type, ConPtr newCon);
		void RemoveConnection(string name);
		void Disconnect();
//        bool addConnection(node* newNodeCon, double conWeight);
//        bool addConnections(nodeList inNodes, vector<double> conWeights);
		//void SetType(nodeType inType);
//        static vector<double> vecMul(vector<double> vec1, vector<double> vec2);

	private:
		double sumOfIn;
		double out;
		double sigSteepness;
		string name;
		ConList inConnections;
		ConList outConnections;
		ActFuncPtr actFunction;

		double SumWeightedIn();
		void UpdateOut();
		void SetDefaults();
		void RenameConnections();
//        vector<double> inputWeights;
//        vector<double> inputs;
//        nodeList inputNodes;

		//nodeType type;
		int height;
		int numConnections;
//        int numIn;
//        int numOut;
//        int nextIn;
//        int nextOut;
        

	};

	template<typename T>
	NodePtr Node::CreateNode(int height, string name){
		NodePtr np(new Node(height,name));
		np->actFunction.reset(new T);
		return np;
	}

	//template<class T>
	//Node<T>::Node() {
	//	height = 0;
	////    numIn = 0;
	////    numOut = 0;
	////    out = 0;
	////    sumOfIn = 0;
	////    sigSteepness = 5;
	//	name = "NONE";
	////    nextIn = 0;
	//	//nextOut = 0;
	//	//type = Node<T>::sigmoid;
	//	SetDefaults();
	////    inputs = vector<double>(0);
	////    inputWeights = vector<double>(0);
	////    inConnections = vector<connection>(0);
	////    outConnections = vector<connection>(0);
	//}

	//template<class T>
	//Node<T>::Node(int inHeight, string nodeName){
	//	//Node<T>::Node(int inHeight, nodeType nType, string nodeName){
	//	height = inHeight;
	////    numIn = inNumIn;
	////    numOut = inNumOut;
	////    out = 0;
	////    sumOfIn = 0;
	////    sigSteepness = 5;
	//	name = nodeName;
	////    nextIn = 0;
	//	//nextOut = 0;
	//	//type = nType;
	//	SetDefaults();
	////    inputs = vector<double>(numIn);
	////    inputWeights = vector<double>(numOut);
	////    inConnections = vector<connection>(numIn);
	////    outConnections = vector<connection>(numOut);
	//}

	//template<class T>
	//void Node<T>::SetDefaults(){
	//	out = 0;
	//	sumOfIn = 0;
	//	sigSteepness = 1;
	//}

	//template<class T>
	//Node<T>::Node(const Node<T>& orig) {
	//	height = orig.height;
	////    numIn = orig.numIn;
	////    numOut = orig.numOut;
	//	out = orig.out;
	//	sumOfIn = orig.sumOfIn;
	//	sigSteepness = orig.sigSteepness;
	//	name = orig.name;
	////    nextIn = orig.nextIn;
	////    nextOut = orig.nextOut;
	////    inputs = orig.inputs;
	////    inputWeights = orig.inputWeights;
	//	//isBias = orig.isBias;
	//	inConnections = orig.inConnections;
	//	outConnections = orig.outConnections;
	//}

	//template<class T>
	//Node<T>::~Node() {
	////    inConnections.clear();
	////    outConnections.clear();
	////    inputs.clear();
	////    inputWeights.clear();
	//}

	//template<class T>
	//void Node<T>::Activate() {
	//	//TODO:standard sigmoid right now will abstract later
	////    int i = 0;
	////    BOOST_FOREACH(node n, inputNodes){
	////        inputs[i] = n.getOut();
	////        i++;
	////    }
	//	//if(type != Node<T>::bias){
	//	//    sumOfIn = SumWeightedIn();
	//	//}
	//	sumOfIn = SumWeightedIn();
	//	UpdateOut();
	//}

	//template<class T>
	//void Node<T>::Activate(double input){
	//	sumOfIn = input;
	//	UpdateOut();
	//}

	//template<class T>
	//void Node<T>::UpdateOut(){
	//	/*switch(type){
	//		case Node<T>::sigmoid:
	//			out = 1 / (1 + exp(-sigSteepness*sumOfIn));
	//			if (out == 0){
	//				out = 0.0000001;
	//			}
	//			else if (out == 1){
	//				out = 0.9999999;
	//			}
	//			break;
	//		case Node<T>::linear:
	//			out = sumOfIn;
	//			break;
	//		default:
	//			out = 1;
	//			break;
	//	}*/
	//	out = actFunction.activate(sumOfIn);
	//	BOOST_FOREACH(ConPtr con, outConnections){
	//		con->Update();
	//	}
	//}

	//template<class T>
	//double Node<T>::SumWeightedIn(){
	//	double tmpSum = 0;
	//	BOOST_FOREACH(ConPtr con, inConnections){
	//		tmpSum += con->GetWeightedValue();
	//	}
	//	return tmpSum;
	//}

	//template<class T>
	//void Node<T>::SetHeight(int newHeight){
	//	height = newHeight;
	//}

	//template<class T>
	//double Node<T>::GetOut(){
	//	return out;
	//}

	//template<class T>
	//double Node<T>::GetPrime(){
	//	return actFunction.prime(out, sumOfIn);
	//}

	//template<class T>
	//const string& Node<T>::GetName(){
	//	return name;
	//}

	////double Node<T>::GetSigSteepness(){
	////    return sigSteepness;
	////}

	//template<class T>
	//void Node<T>::SetName(string newName){
	//	name = newName;
	//}

	////void Node<T>::setNumIn(int newNumIn){
	////    numIn = newNumIn;
	////    inputs.resize(newNumIn,true);
	////    inputWeights.resize(newNumIn, true);
	////}
	////
	////void Node<T>::setNumOut(int newNumOut){
	////    numOut = newNumOut;
	////    inputs.resize(newNumOut, true);
	////    inputWeights.resize(newNumOut, true);
	////}

	//template<class T>
	//bool Node<T>::AddConnection(conType type, ConPtr newCon){
	//	bool returnVal = false;
	//	switch(type){
	//		case IN:
	//			inConnections.push_back(newCon);
	//			returnVal = true;
	//			break;
	//		case OUT:
	//			outConnections.push_back(newCon);
	//			returnVal = true;
	//			break;
	//		default:
	//			returnVal = false;
	//	}
	//	return returnVal;
	//}

	//template<class T>
	//void Node<T>::RemoveConnection(string name){
	//	ConList::iterator incons = inConnections.begin();
	//	bool found = false;
	//	while(incons != inConnections.end()){
	//		if ((*incons)->GetName() == name){
	//			found = true;
	//			inConnections.erase(incons);
	//			break;
	//		}
	//		else {
	//			incons++;
	//		}
	//	}
	//	if (!found) {
	//		ConList::iterator outcons = outConnections.begin();
	//		while(outcons != outConnections.end()){
	//			if ((*outcons)->GetName() == name){
	//				outConnections.erase(outcons);
	//				break;
	//			}
	//			else {
	//				outcons++;
	//			}
	//		}
	//	}
	//}

	//template<class T>
	//ConList& Node<T>::GetConnections(conType type){
	//	return type == IN ? inConnections : outConnections;
	//}

}

#endif	/* _NODE_H */

