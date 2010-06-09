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

//using namespace boost::numeric::ublas;

namespace JRNN {

class Node {
    public:

//        typedef boost::ptr_list<node> nodeList;

        enum conType {
            IN,
            OUT
        };

        enum nodeType {
            sigmoid,
            linear,
            bias
        };

        Node();
        Node(int height, nodeType nType, std::string name);
        Node(const Node& orig);
        virtual ~Node();
        virtual void Activate();
        virtual void Activate(double input);
        void SetHeight(int newHeight);
        double GetOut();
        const std::string& GetName();
        double GetSigSteepness();
        ConList& GetConnections(conType type);
        
        void SetName(std::string newName);
//        void setNumIn(int newNumIn);
//        void setNumOut(int newNumOut);
        bool AddConnection(conType type, ConPtr newCon);
        void RemoveConnection(std::string name);
//        bool addConnection(node* newNodeCon, double conWeight);
//        bool addConnections(nodeList inNodes, vector<double> conWeights);
        void SetType(nodeType inType);
//        static vector<double> vecMul(vector<double> vec1, vector<double> vec2);

    private:
        double sumOfIn;
        double out;
        double sigSteepness;
        std::string name;
        ConList inConnections;
        ConList outConnections;

        double SumWeightedIn();
        void UpdateOut();
        void SetDefaults();
//        vector<double> inputWeights;
//        vector<double> inputs;
//        nodeList inputNodes;

        nodeType type;
        int height;
//        int numIn;
//        int numOut;
//        int nextIn;
//        int nextOut;
        

};

}

#endif	/* _NODE_H */

