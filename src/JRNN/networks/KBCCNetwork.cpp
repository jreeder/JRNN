/********************************************************************
	created:	2013/05/05
	created:	5:5:2013   5:19
	filename: 	KBCCNetwork.cpp
	author:		jreeder
	
	purpose:	Network that can hold other networks. 
*********************************************************************/

#include "JRNN.h"
#include "networks/KBCCNetwork.h"

namespace JRNN {


	JRNN::NetworkPtr KBCCNetwork::Clone()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::CreateCandLayer( int numCand )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	JRNN::NodePtr KBCCNetwork::AppendNewInputNode()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::AppendInputNode( NodePtr node )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::InsertInputNode( NodePtr newNode, int pos )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	JRNN::NodePtr KBCCNetwork::InsertNewInputNode( int pos )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::ConnectToHiddenNodes( NodePtr node, conType cType /*= OUT */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::ConnectToHiddenNodes( NodeList nodes, conType cType /*= OUT */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::InstallCandidate( NodePtr node, vecDouble outWeights /*= vecDouble(0 ) */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::Build( int numIn, int numOut, bool cloneouts /*= false*/, bool useSDCC /*= false*/, bool varyActFunc /*= false */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::Reset()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::CandFullyConnectBack( LayerPtr layer )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::FullyConnectOut( LayerPtr layer )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::FullyConnectOut( LayerPtr layer, vecDouble outWeights )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::RemoveUnConnectedNodes()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::RemoveHiddenLayer( LayerPtr layer )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCNetwork::CandConnectOut( NodePtr node, vecDouble outWeights /*= vecDouble(0 ) */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

}