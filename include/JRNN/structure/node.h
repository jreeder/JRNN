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

class node {
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

        node();
        node(int height, nodeType nType, std::string name);
        node(const node& orig);
        virtual ~node();
        virtual void activate();
        virtual void activate(double input);
        void setHeight(int newHeight);
        double getOut();
        const std::string& getName();
        double getSigSteepness();
        conList& getConnections(conType type);
        
        void setName(std::string newName);
//        void setNumIn(int newNumIn);
//        void setNumOut(int newNumOut);
        bool addConnection(conType type, conPtr newCon);
        void removeConnection(std::string name);
//        bool addConnection(node* newNodeCon, double conWeight);
//        bool addConnections(nodeList inNodes, vector<double> conWeights);
        void setType(nodeType inType);
//        static vector<double> vecMul(vector<double> vec1, vector<double> vec2);

    private:
        double sumOfIn;
        double out;
        double sigSteepness;
        std::string name;
        conList inConnections;
        conList outConnections;

        double sumWeightedIn();
        void updateOut();
        void setDefaults();
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

