/********************************************************************
	created:	2013/03/28
	created:	28:3:2013   21:36
	filename: 	KBCCNetwork.h
	author:		jreeder
	
	purpose:	Network for KBCC. Mostly needed to change how the candidate
				is added if it is a subnetwork. 
*********************************************************************/

#ifndef _KBCCNETWORK_H
#define _KBCCNETWORK_H

#include "JRNN.h"
#include "networks/CCNetwork.h"
#include "structure/networknode.h"

namespace JRNN {
	class KBCCNetwork;

	typedef boost::shared_ptr<KBCCNetwork> KBCCNetworkPtr;
	typedef vector<NetworkNodePtr> NetworkNodeList;
	typedef std::pair<string, NetworkPtr> NetPtrPair;
	typedef map<string, NetworkPtr> NetPtrList;
	typedef derived_shared_from_this<KBCCNetwork> KBCCSharedFromThis;

	class KBCCNetwork : public CCNetwork, public KBCCSharedFromThis{
	public:
		KBCCNetwork();
		KBCCNetwork(int numIn, int numOut);
		
		static const string Type;		
		virtual string GetType() {
			return Type;
		}

		static KBCCNetworkPtr Create();
		static KBCCNetworkPtr Clone(KBCCNetworkPtr net);

		virtual NetworkPtr Clone();

		virtual void CreateCandLayer( int numCand, NetPtrList candNets = NetPtrList(), int numCopies = 0);

		virtual NodePtr AppendNewInputNode();

		virtual void AppendInputNode( NodePtr node );

		virtual void InsertInputNode( NodePtr newNode, int pos );

		virtual NodePtr InsertNewInputNode( int pos );

		virtual void ConnectToHiddenNodes( NodePtr node, conType cType = OUT );

		virtual void ConnectToHiddenNodes( NodeList nodes, conType cType = OUT );

		virtual void InstallCandidate( NodePtr node, vecDouble outWeights = vecDouble(0 ) );
		
		void InstallCandidate( NodePtr node, hashedVecDoubleMap outWeights = hashedVecDoubleMap());
		
		virtual void Build( int numIn, int numOut, bool cloneouts = false, bool useSDCC = false, bool varyActFunc = false );

		virtual void Reset();

		NetworkNodeList& GetSubNetworkNodes();

	protected:

		NetworkNodeList SubNetworkNodes;
		NetworkNodeList CandSubNetNodes;
		
		virtual void CandFullyConnectBack( LayerPtr layer );

		virtual void FullyConnectOut( LayerPtr layer );

		virtual void FullyConnectOut( LayerPtr layer, vecDouble outWeights );

		virtual void RemoveUnConnectedNodes();

		virtual void RemoveHiddenLayer( LayerPtr layer );

		virtual void CandConnectOut( NodePtr node, vecDouble outWeights = vecDouble(0 ) );

		virtual ConPtr Connect( NodePtr n1, NodePtr n2 );

		virtual ConPtr Connect( NodePtr n1, NodePtr n2, double conweight );
		void CandNetFullyConnectBack( NetworkNodePtr newNetNode, bool onlyDirect, bool reduceHeight );
	};
}
#endif