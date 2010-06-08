/* 
 * File:   network.h
 * Author: jreeder
 *
 * Created on May 17, 2010, 5:47 PM
 */

#ifndef _NETWORK_H
#define	_NETWORK_H

#include "JRNN.h"

using namespace std;

namespace JRNN{

    class network {
    public:
        network();
//        network(const network& orig);
        virtual ~network();
        void activate(vecDouble inputs);
        void setInputs(vecDouble inputs);
        vecDouble getOutputs();
        void setDesiredOut(vecDouble desiredOut);
        layerPtr getLayer(std::string layerName);
        conList& getConnections();
        hashedDoubleMap getWeights();
        void setWeights(hashedDoubleMap weights);
        void reset();
        void printConnections();
        static networkPtr createFFMLPNetwork(int numIn, int numHid, int numOut);
        
    private:
        int numIn;
        int numOut;
        int numHid;
        layerMap layers;
        conList inToHid;
        conList hidToOut;
        conList connections;
        vecDouble inputs;
        vecDouble outputs;
        vecDouble desiredOut;

        void fullyConnect();

    };

}

#endif	/* _NETWORK_H */

