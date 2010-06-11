/* 
 * File:   network.h
 * Author: jreeder
 *
 * Created on May 17, 2010, 5:47 PM
 */

#ifndef _NETWORK_H
#define	_NETWORK_H

#include "JRNN.h"
#include "structure/node.h"
#include "structure/connection.h"
#include "structure/layer.h"

using namespace std;

namespace JRNN{

    class Network {
    public:
		//TODO: need to add methods for removing connections. 
		//This means making methods to remove them in nodes, 
		//and having the destructor of connections take care of it. 
        Network();
//        network(const network& orig);
        virtual ~Network();
        void Activate(vecDouble inputs);
		void Activate(LayerPtr layer);
		void SetInputs(vecDouble inputs);
        vecDouble GetOutputs();
        void SetDesiredOut(vecDouble desiredOut);
        LayerPtr GetLayer(std::string layerName);
        ConList& GetConnections();
        hashedDoubleMap GetWeights();
        void SetWeights(hashedDoubleMap weights);
        void Reset();
        void PrintConnections();
        static NetworkPtr CreateFFMLPNetwork(int numIn, int numHid, int numOut);
		static NetworkPtr CreateMinFFNetwork(int numIn, int numOut);
        
    private:
        int numIn;
        int numOut;
        int numHid; //TODO: need to abstract this out. 
		int numHidLayers;
        LayerMap layers;
        ConList inToHid;
        ConList hidToOut;
        ConList connections;
        vecDouble inputs;
        vecDouble outputs;
        vecDouble desiredOut;

        void FullyConnectFFMLP();
		void FullyConnectMinFF();
		void FullyConnectBack(LayerPtr layer);
		void FullyConnectOut(LayerPtr Layer);

    };

}

#endif	/* _NETWORK_H */

