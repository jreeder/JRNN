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

	class KBCCNetwork : CCNetwork{
	protected:
		NetworkNodeList SubNetworkNodes;

		virtual NetworkPtr Clone() 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void CreateCandLayer( int numCand ) 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual NodePtr AppendNewInputNode() 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void AppendInputNode( NodePtr node ) 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void InsertInputNode( NodePtr newNode, int pos ) 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual NodePtr InsertNewInputNode( int pos ) 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void ConnectToHiddenNodes( NodePtr node, conType cType = OUT ) 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void ConnectToHiddenNodes( NodeList nodes, conType cType = OUT ) 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void InstallCandidate( NodePtr node, vecDouble outWeights = vecDouble(0 ) )
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void Build( int numIn, int numOut, bool cloneouts = false, bool useSDCC = false, bool varyActFunc = false ) 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void Reset() 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void CandFullyConnectBack( LayerPtr layer ) 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void FullyConnectOut( LayerPtr layer ) 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void FullyConnectOut( LayerPtr layer, vecDouble outWeights ) 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void RemoveUnConnectedNodes() 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void RemoveHiddenLayer( LayerPtr layer ) 
		{
			throw std::exception("The method or operation is not implemented.");
		}

		virtual void CandConnectOut( NodePtr node, vecDouble outWeights = vecDouble(0 ) )
		{
			throw std::exception("The method or operation is not implemented.");
		}

	};
}
#endif