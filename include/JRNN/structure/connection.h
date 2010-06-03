/* 
 * File:   connection.h
 * Author: jreeder
 *
 * Created on May 14, 2010, 2:32 PM
 */

#ifndef _CONNECTION_H
#define	_CONNECTION_H

#include "JRNN.h"

namespace JRNN {

    class connection {
    public:

        typedef double value_type;
        typedef connection self_type;

        connection();
        static conPtr connect(nodePtr newInNode, nodePtr newOutNode);
        static conPtr connect(nodePtr newInNode, nodePtr newOutNode, double weight);
        static void setRandomSeed();
        connection(const connection& orig);
        connection(double newDouble);
        virtual ~connection();
        void update();
        void reset();
        double getWeightedValue();
        double getValue();
        void setInNode(nodePtr newInnode);
        void setOutNode(nodePtr newOutnode);
        void setLocked(bool lock);
        void setWeight(double weight);
        double getWeight() const;
        const std::string& getName();
        void setName(std::string newName);
        void setName();
        const std::string& getOutNodeName();
        const std::string& getInNodeName();
        connection operator+ (const connection &rhs);
        connection operator* (const connection &rhs);
        connection& operator= (const connection& rhs){
            this->inNode = rhs.inNode;
            this->outNode = rhs.outNode;
            this->value = rhs.value;
            this->weightedValue = rhs.weightedValue;
            this->weight = rhs.weight;
            this->name = rhs.name;
            return *this;
        }
        connection& operator+= (const connection& rhs){
            this->weight += rhs.weight;
            return *this;
        }

        connection& operator+= (const double rhs){
            this->weight += rhs;
            return *this;
        }

        operator double () const {
            return weight;
        }

    private:
        double weight;
        double value;
        double weightedValue;
        bool locked;
        std::string name;
        nodePtr inNode;
        nodePtr outNode;

        double randomWeight();


    };

}
#endif	/* _CONNECTION_H */

