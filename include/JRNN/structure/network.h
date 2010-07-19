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

	typedef NetworkPtrHelper<>::Ptr NetworkPtr;

    class Network {
    public:
		//TODO: need to add methods for removing connections. 
		//This means making methods to remove them in nodes, 
		//and having the destructor of connections take care of it. 
        
//        network(const network& orig);
        virtual ~Network();

		static NetworkPtr Create(){
			NetworkPtr np(new Network());
			return np;
		}

        void Activate(vecDouble inputs);
		void Activate(LayerPtr layer);
		void SetInputs(vecDouble inputs);
		void SetLocked(bool lock);
		bool GetLocked();

		void SetNumIn(int numIn);
		int GetNumIn();
		void SetNumOut(int numOut);
		int GetNumOut();
		void SetNumHidLayers(int numHidLayers);
		int GetNumHidLayers();

        vecDouble GetOutputs();
		vecDouble GetPrimes(std::string layerName);
        void SetDesiredOut(vecDouble desiredOut);
        LayerPtr GetLayer(std::string layerName);
        ConMap& GetConnections();
		void AddConnection(ConPtr con);
		void RemoveConnection(ConPtr con);
		void RemoveConnections(ConList cons);
        hashedDoubleMap GetWeights();
        virtual void SetWeights(hashedDoubleMap weights);
        virtual void Reset();
        void PrintConnections();
		//bool AddHiddenLayer(LayerPtr newLayer);
		//bool AddCascadedLayer(LayerPtr newLayer);
        //static NetworkPtr CreateFFMLPNetwork(int numIn, int numHid, int numOut);
		//static NetworkPtr CreateMinFFNetwork(int numIn, int numOut);
        
    protected:
		Network();

        int numIn;
        int numOut;
		LayerMap layers;
		ConMap connections;
        //int numHid; //TODO: need to abstract this out. 
		int numHidLayers;
		bool locked;
        /*ConList inToHid;
        ConList hidToOut;*/
        vecDouble inputs;
        vecDouble outputs;
        vecDouble desiredOut;

		LayerPtr AddHiddenLayer();
		void RemoveHiddenLayer(LayerPtr layer);
  //      void FullyConnectFFMLP();
		//void FullyConnectMinFF();
		//void FullyConnectBack(LayerPtr layer);
		//void FullyConnectOut(LayerPtr Layer);

    };

}

#endif	/* _NETWORK_H */

