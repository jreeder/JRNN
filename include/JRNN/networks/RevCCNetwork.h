/********************************************************************
	created:	2012/03/05
	created:	5:3:2012   18:19
	filename: 	RevCCNetwork.h
	author:		jreeder
	
	purpose:	RevCCNetwork class. Meant to add the reverberating 
				process to the CC network. This will be the building block
				of the life long learning system. 
*********************************************************************/

#ifndef _REVCCNETWORK_H
#define _REVCCNETWORK_H

#include "JRNN.h"
#include "networks/CCNetwork.h"

namespace JRNN {
	class RevCCNetwork;
	typedef boost::shared_ptr<RevCCNetwork> RevCCNetworkPtr;
	typedef boost::enable_shared_from_this<RevCCNetwork> RevCCSharedFromThis;

	class RevCCNetwork : public CCNetwork, public RevCCSharedFromThis {
	public:
		RevCCNetwork();
		RevCCNetwork(int numIn, int numOut);
		~RevCCNetwork();
		static RevCCNetworkPtr Create();
		static RevCCNetworkPtr Clone(RevCCNetworkPtr net);
		virtual NetworkPtr	Clone();
		const LayerPtr GetAutoAssocLayer();
		const LayerPtr GetNormOutLayer();
		void SetAutoAssocLayerByName(string name);
		void SetNormOutLayerByName(string name);
		virtual vecDouble GetOutputs();
		vecDouble GetTrueOutputs();
		virtual void InstallCandidate(NodePtr node, vecDouble outWeights = vecDouble(0), vecDouble assocWeights = vecDouble(0));
		virtual void Build(int numIn, int numOut, bool cloneouts = false, bool useSDCC = false, bool varyActFunc = false);
		virtual void Reset();
		
		//Add new inputs. Need to be overridden so that we handle the auto associative stuff correctly.
		virtual void AppendInputNode( NodePtr node );
		void AppendNewAutoAssocNode();

		virtual void InsertInputNode( NodePtr newNode, int pos );

		bool getTrueOuts;

	protected:		
		LayerPtr autoAssocLayer;
		LayerPtr normOutLayer;
		//TODO:might make this network context are soon. 
		void FullyConnectAutoAssoc(LayerPtr layer);
		void FullyConnectAutoAssoc(LayerPtr layer, vecDouble outWeights);
		void InsertNewAutoAssocNode( int pos );
		

	};

}

#endif