/* 
 * File:   CCNetwork.h
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
	typedef boost::shared_ptr<CCNetwork> CCNetworkPtr;
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
		void SetCandLayerByName(string name);
		void CreateCandLayer(int numCand);
		void AppendNewInputNode();
		void AppendInputNode(NodePtr node);
		void InsertInputNode( NodePtr newNode, int pos );
		void InsertNewInputNode(int pos);
		virtual void InstallCandidate(NodePtr node, vecDouble outWeights = vecDouble(0));
		virtual void Build(int numIn, int numOut, bool cloneouts = false, bool useSDCC = false, bool varyActFunc = false);
		virtual void Reset();
		void SetWeights(hashedDoubleMap weights);
		int GetNumUnits();
		void SetNumUnits(int newNumUnits);
		void SetUseSDCC(bool inUseSDCC);
		void SetVaryActFunc(bool inVaryActFunc);
		void SetCurrentLayerByName( string currentLayerName );
		void SetCloneOuts( bool cloneOuts );
		LayerList& GetHiddenLayers();
		LayerPtr GetCurrentLayer();
		bool GetCloneOuts();
		bool GetSDCC();
		bool GetVaryActFunc();

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
		void CandConnectOut( NodePtr node, vecDouble outWeights = vecDouble(0));	
	};
}

#endif