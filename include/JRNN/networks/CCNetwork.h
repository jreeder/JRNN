/* 
 * File:   CCBuilder.h
 * Author: jreeder
 * 
 * Created on July 13, 2010
 */

#pragma once 
#ifndef _CCBUILDER_H
#define _CCBUILDER_H

#include "JRNN.h"
#include "structure/network.h"

namespace JRNN {
	class CCNetwork;
	typedef NetworkPtrHelper<CCNetwork>::Ptr CCNetworkPtr;
	typedef boost::enable_shared_from_this<CCNetwork> CCSharedFromThis;

	class CCNetwork : public Network, public CCSharedFromThis {
	//This might be slightly inconsistent with my other use of builder
	public: 
		CCNetwork();
		CCNetwork(int numIn, int numOut);
		//CCNetwork(NetworkPtr network);
		~CCNetwork();
		static CCNetworkPtr Create();
		static CCNetworkPtr Clone(CCNetworkPtr net);
		virtual NetworkPtr Clone();
		//NetworkPtr GetNetwork();
		//void SetNetwork(NetworkPtr net);
		const LayerPtr GetCandLayer();
		void CreateCandLayer(int numCand);
		virtual void InstallCandidate(NodePtr node, vecDouble outWeights = vecDouble(0));
		virtual void Build(int numIn, int numOut, bool cloneouts = false, bool useSDCC = false, bool varyActFunc = false);
		virtual void Reset();
		void SetWeights(hashedDoubleMap weights);
		int GetNumUnits();
		void SetUseSDCC(bool inUseSDCC);
		void SetVaryActFunc(bool inVaryActFunc);

	protected:
		//int numIn;
		//int numOut;
		int numUnits;
		ConList candConnections;
		LayerPtr candLayer;
		LayerPtr currentLayer;
		LayerList hiddenLayers;
		bool cloneOuts;
		bool useSDCC;
		bool varyActFunc;
		void CandFullyConnectBack(LayerPtr layer);
		//LayerPtr AddHiddenLayer(); Moved to parent
		void FullyConnectOut(LayerPtr layer);
		void FullyConnectOut(LayerPtr layer, vecDouble outWeights);
		void FullyConnect();
		void RemoveUnConnectedNodes();
		virtual void RemoveHiddenLayer(LayerPtr layer);
		void BuildVariedLayer( LayerPtr candLayer );


	};
}

#endif