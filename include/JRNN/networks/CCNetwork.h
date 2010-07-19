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

	class CCNetwork : public Network{
	//This might be slightly inconsistent with my other use of builder
	public: 
		CCNetwork();
		CCNetwork(int numIn, int numOut);
		//CCNetwork(NetworkPtr network);
		~CCNetwork();
		static CCNetworkPtr Create();
		//NetworkPtr GetNetwork();
		//void SetNetwork(NetworkPtr net);
		const LayerPtr GetCandLayer();
		void CreateCandLayer(int numCand);
		void InstallCandidate(NodePtr node, vecDouble outWeights = vecDouble(0));
		void Build(int numIn, int numOut);
		virtual void Reset();
		virtual void SetWeights(hashedDoubleMap weights);
		int GetNumUnits();

	private:
		//int numIn;
		//int numOut;
		int numUnits;
		ConList candConnections;
		LayerPtr candLayer;
		LayerList hiddenLayers;
		void CandFullyConnectBack(LayerPtr layer);
		//LayerPtr AddHiddenLayer(); Moved to parent
		void FullyConnectOut(LayerPtr layer);
		void FullyConnectOut(LayerPtr layer, vecDouble outWeights);
		void FullyConnect();
		void RemoveUnConnectedNodes();
		

	};
}

#endif