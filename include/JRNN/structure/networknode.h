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
		static const string ActType;

		NetworkNode();

		NetworkNode(const NetworkNode& orig);

		virtual NodePtr Clone();
		
		NetworkNode(int inHeight, string nodeName, NetworkPtr net);

		NetworkNode(int inHeight, string nodeName);
		
		virtual void Activate();

		static NetworkNodePtr Create(int inHeight, string nodeName);
		
		vecDouble GetOuts();
		
		hashedVecDoubleMap GetPrimes();
		
		virtual int GetNumConnections();

		virtual double GetOut();

		virtual double GetPrime();

		virtual ConList& GetConnections( conType type );

		virtual string GetActFuncType();
		
		NetworkPtr GetIntNet() const;
		
		void SetIntNet(NetworkPtr val);

		NodeList GetInputNodes();
		NodeList GetOutputNodes();
		virtual void SetName( string newName );

	private:
		NetworkPtr intNet;
		
		int numInputs;
		int numOutputs;
		//hashedDoubleMap inputGradients;
		hashedVecDoubleMap localGradients;
		hashedVecDoubleMap outGradients;
		double pOffset;

		void calcGradients();
		void cascadeGradients( LayerPtr layer );
		double AddOffset( double Prime);
	};

}

#endif
