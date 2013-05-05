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
		virtual void CreateCandLayer(int numCand);
		
		//Functions for adding new inputs
		virtual NodePtr AppendNewInputNode();
		virtual void AppendInputNode(NodePtr node);
		virtual void InsertInputNode( NodePtr newNode, int pos );
		virtual NodePtr InsertNewInputNode(int pos);
		virtual void ConnectToHiddenNodes(NodePtr node, conType cType = OUT);
		virtual void ConnectToHiddenNodes(NodeList nodes, conType cType = OUT);

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
		virtual void CandFullyConnectBack(LayerPtr layer);
		//LayerPtr AddHiddenLayer(); Moved to parent
		virtual void FullyConnectOut(LayerPtr layer);
		virtual void FullyConnectOut(LayerPtr layer, vecDouble outWeights);
		void FullyConnect();
		virtual void RemoveUnConnectedNodes();
		virtual void RemoveHiddenLayer(LayerPtr layer);
		void BuildVariedLayer( LayerPtr candLayer );
		virtual void CandConnectOut( NodePtr node, vecDouble outWeights = vecDouble(0));	
	};
}

#endif