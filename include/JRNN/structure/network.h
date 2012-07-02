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
	typedef boost::enable_shared_from_this<Network> NetworkSharedFromThis;

    class Network: public NetworkSharedFromThis{
    public:
        
		//Network(const Network& orig);
        virtual ~Network();

		static NetworkPtr Create(){
			NetworkPtr np(new Network());
			return np;
		}
		
		static void Clone(NetworkPtr newP, NetworkPtr oldP);
		virtual NetworkPtr Clone();
        virtual void Activate(vecDouble inputs);
		virtual void Activate(LayerPtr layer);
		void SetInputs(vecDouble inputs);
		void SetLocked(bool lock);
		bool GetLocked();

		void SetNumIn(int numIn);
		int GetNumIn();
		void SetNumOut(int numOut);
		int GetNumOut();
		void SetNumHidLayers(int numHidLayers);
		int GetNumHidLayers();
		void SetScaleAndOffset(double scale, double offset);

        virtual vecDouble GetOutputs();
		vecDouble GetPrimes(string layerName);
        void SetDesiredOut(vecDouble desiredOut);
        LayerPtr GetLayer(string layerName);
		NodePtr GetNode(string nodeName);
        ConMap& GetConnections();
		LayerMap& GetLayers();
		void AddConnection(ConPtr con);
		void RemoveConnection(ConPtr con);
		void RemoveConnections(ConList cons);
        hashedDoubleMap GetWeights();
        virtual void SetWeights(hashedDoubleMap weights);
        virtual void Reset();
		void ResetHeights();
        void PrintConnections();
		//bool AddHiddenLayer(LayerPtr newLayer);
		//bool AddCascadedLayer(LayerPtr newLayer);
        //static NetworkPtr CreateFFMLPNetwork(int numIn, int numHid, int numOut);
		//static NetworkPtr CreateMinFFNetwork(int numIn, int numOut);
        
    protected:
		Network();
		Network(const Network& orig);

        int numIn;
        int numOut;
		LayerMap layers;
		ConMap connections;
        //int numHid; //TODO: need to abstract this out. 
		int numHidLayers;
		bool locked;
		double conScale;
		double conOffset;
        /*ConList inToHid;
        ConList hidToOut;*/
        vecDouble inputs;
        vecDouble outputs;
        //vecDouble desiredOut;

		LayerPtr AddHiddenLayer();
		virtual void RemoveHiddenLayer(LayerPtr layer);
		ConPtr Connect(NodePtr n1, NodePtr n2);
		ConPtr Connect(NodePtr n1, NodePtr n2, double conweight);
  //      void FullyConnectFFMLP();
		//void FullyConnectMinFF();
		//void FullyConnectBack(LayerPtr layer);
		//void FullyConnectOut(LayerPtr Layer);

    };

}

#endif	/* _NETWORK_H */

