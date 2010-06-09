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
		//TODO: need to add methods for removing connections. 
		//This means making methods to remove them in nodes, 
		//and having the destructor of connections take care of it. 
        network();
//        network(const network& orig);
        virtual ~network();
        void activate(vecDouble inputs);
		void activate(layerPtr layer);
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
		static networkPtr createMinFFNetwork(int numIn, int numOut);
        
    private:
        int numIn;
        int numOut;
        int numHid; //TODO: need to abstract this out. 
		int numHidLayers;
        layerMap layers;
        conList inToHid;
        conList hidToOut;
        conList connections;
        vecDouble inputs;
        vecDouble outputs;
        vecDouble desiredOut;

        void fullyConnectFFMLP();
		void fullyConnectMinFF();
		void fullyConnectBack(layerPtr layer);
		void fullyConnectOut(layerPtr Layer);

    };

}

#endif	/* _NETWORK_H */

