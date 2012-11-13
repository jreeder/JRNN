/* 
 * File:   connection.h
 * Author: jreeder
 *
 * Created on May 14, 2010, 2:32 PM
 */

#ifndef _CONNECTION_H
#define	_CONNECTION_H

#include "JRNN.h"
#include "structure/node.h"

namespace JRNN {

    class Connection {
    public:

        typedef double value_type;
        typedef Connection self_type;

        Connection();
        static ConPtr Connect(NodePtr newInNode, NodePtr newOutNode);
        static ConPtr Connect(NodePtr newInNode, NodePtr newOutNode, double weight);
		static ConPtr Clone(ConPtr con, NodePtr inNode, NodePtr outNode);
        static void SetRandomSeed();
		static double GetRandWeight(double inScale, double inOffset);
		Connection(const Connection& orig);
        Connection(double newDouble);
        virtual ~Connection();
        void Update();
        void Reset();
        double GetWeightedValue();
        double GetValue();
        void SetInNode(NodePtr newInnode);
        void SetOutNode(NodePtr newOutnode);
        void SetLocked(bool lock);
		bool GetLocked();
        void SetWeight(double weight);
		void SetScaleAndOffset(double scale, double offset);
        void Disconnect();
		void Disconnect(string nodeName);
		double GetWeight() const;
        const string& GetName();
        void SetName(string newName);
        void SetName();
        const string& GetOutNodeName();
        const string& GetInNodeName();
        Connection operator+ (const Connection &rhs);
        Connection operator* (const Connection &rhs);
        Connection& operator= (const Connection& rhs){
            inNode = rhs.inNode;
            outNode = rhs.outNode;
            value = rhs.value;
            weightedValue = rhs.weightedValue;
            weight = rhs.weight;
            name = rhs.name;
            return *this;
        }
        Connection& operator+= (const Connection& rhs){
            weight += rhs.weight;
            return *this;
        }

        Connection& operator+= (const double rhs){
            weight += rhs;
            return *this;
        }

        operator double () const {
            return weight;
        }

    private:
        double weight;
        double value;
        double weightedValue;
		double scale;
		double offset;
        bool locked;
        string name;
        NodePtr inNode;
        NodePtr outNode;
		static RandomGenerator01 cRand;

        double RandomWeight();


    };

}
#endif	/* _CONNECTION_H */

