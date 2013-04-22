/********************************************************************
	created:	2013/03/28
	created:	28:3:2013   2:38
	filename: 	networknode.h
	author:		jreeder
	
	purpose:	This will define a special node that will contain a
				subnet. This will be used for KBCC. It will behave 
				like a node in that a layer can contain it, and it will
				activate like a node. 
*********************************************************************/

#ifndef	_NETWORKNODE_H
#define _NETWORKNODE_H

#include "JRNN.h"
#include "structure/network.h"
#include "structure/node.h"

namespace JRNN {
	
	class NetworkNode;
	typedef boost::shared_ptr<NetworkNode> NetworkNodePtr;

	class NetworkNode: public Node {

	public:
		NetworkNode();
		
		NetworkNode(int inHeight, string nodeName, NetworkPtr net);
		
		virtual void Activate();
		
		vecDouble GetOuts();
		
		hashedVecDoubleMap GetPrimes();
		
		virtual int GetNumConnections();

		virtual double GetOut();

		virtual double GetPrime();

		virtual ConList& GetConnections( conType type );

		virtual string GetActFuncType();
		
		NetworkPtr GetIntNet() const;
		
		void SetIntNet(NetworkPtr val);
		
		virtual bool AddConnection( conType type, ConPtr newCon );

	private:
		NetworkPtr intNet;
		
		int numInputs;
		int numOutputs;
		//hashedDoubleMap inputGradients;
		hashedVecDoubleMap localGradients;


		void calcGradients();
		void cascadeGradients( LayerPtr layer );
	};

}

#endif
