/********************************************************************
	created:	2013/05/28
	created:	28:5:2013   3:15
	filename: 	RevKBCCNetwork.h
	author:		jreeder
	
	purpose:	Reverberated KBCCNetwork
*********************************************************************/

#ifndef _REVKBCCNETWORK_H
#define _REVKBCCNETWORK_H

#include "JRNN.h"
#include "networks/KBCCNetwork.h"

namespace JRNN {
	class RevKBCCNetwork;
	typedef boost::shared_ptr<RevKBCCNetwork> RevKBCCNetworkPtr;
	typedef derived_shared_from_this<RevKBCCNetwork> RevKBCCSharedFromThis;

	class RevKBCCNetwork : public KBCCNetwork, public RevKBCCSharedFromThis {
	public:
		static const string Type;
		virtual string GetType() {
			return Type;
		}
		RevKBCCNetwork();
		RevKBCCNetwork(int numIn, int numOut);
		~RevKBCCNetwork();
		static RevKBCCNetworkPtr Create();
		static RevKBCCNetworkPtr Clone(RevKBCCNetworkPtr net);
		virtual NetworkPtr Clone();
		KBCCNetworkPtr CloneToKBCC();
		const LayerPtr GetAutoAssocLayer();
		const LayerPtr GetNormOutLayer();
		void SetAutoAssocLayerByName(string name);
		void SetNormOutLayerByName(string name);
		virtual vecDouble GetOutputs();
		virtual int GetNumOut();
		vecDouble GetTrueOutputs();
		virtual void Build(int numIn, int numOut, bool cloneouts = false, bool useSDCC = false, bool varyActFunc = false, string outNodeType = ASigmoid::_type, string autoAssocType = ASigmoid::_type);
		virtual void Reset();

		bool getTrueOuts;

	protected:
		LayerPtr autoAssocLayer;
		LayerPtr normOutLayer;

		void FullyConnectAutoAssoc(LayerPtr layer);
		void FullyConnectAutoAssoc(LayerPtr layer, vecDouble outWeights);
	};
}

#endif