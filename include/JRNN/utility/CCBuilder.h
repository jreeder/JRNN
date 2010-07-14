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
#include "utility/NetworkBuilder.h"

namespace JRNN {
	
	class CCBuilder : public NetworkBuilder {
	//This might be slightly inconsistent with my other use of builder
	public: 
		CCBuilder(int numIn, int numOut);
		CCBuilder(NetworkPtr network);
		~CCBuilder();
		NetworkPtr Create();
		NetworkPtr GetNetwork();
		void SetNetwork(NetworkPtr net);
		void CreateCandLayer(int numCand);
		void InstallCandidate(NodePtr node);

	private:
		int numIn;
		int numOut;
		ConList tmpConnections;
		LayerPtr candLayer;
		NetworkPtr np;
		void CandFullyConnectBack(LayerPtr layer);
		void AddHiddenLayer();
		void FullyConnectOut(LayerPtr layer);
		void FullyConnect();
	};
}

#endif