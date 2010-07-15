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
		void CreateCandLayer(int numCand);
		void InstallCandidate(NodePtr node);
		void Build(int numIn, int numOut);

	private:
		//int numIn;
		//int numOut;
		ConList candConnections;
		LayerPtr candLayer;
		void CandFullyConnectBack(LayerPtr layer);
		LayerPtr AddHiddenLayer();
		void FullyConnectOut(LayerPtr layer);
		void FullyConnect();
		
	};
}

#endif