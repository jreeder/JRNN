/********************************************************************
	created:	2012/04/26
	created:	26:4:2012   23:03
	filename: 	CSLLLFFMLPNetwork.cpp
	author:		jreeder
	
	purpose:	Meant to be an analog to Silvers CSLLL system. This 
				network should do most of the work of the mimicry. Should 
				only be used with the CSMTL paradigm. 
*********************************************************************/

#include "JRNN.h"
#include "networks/CSLLLFFMLPNetwork.h"

namespace JRNN {


	void CSLLFFMLPNetwork::Build( int numIn, int numHid, int numOut, NetworkPtr cdkNetwork )
	{
		FFMLPNetwork::Build(numIn, numHid, numOut);
		this->cdkNetwork = cdkNetwork;
		ConnectCDK();
	}

	void CSLLFFMLPNetwork::ConnectCDK()
	{
		NodeList outNodes = layers["out"]->GetNodes();
		NodeList cdkHiddenNodes = cdkNetwork->GetLayer("hidden")->GetNodes();
		Connection::SetRandomSeed();

		BOOST_FOREACH(NodePtr n, cdkHiddenNodes){
			BOOST_FOREACH(NodePtr n2, outNodes){
				AddConnection(ConnectExt(n,n2));
			}
		}
	}

	CSLLFFMLPNetPtr CSLLFFMLPNetwork::Clone( CSLLFFMLPNetPtr net )
	{
		CSLLFFMLPNetPtr np(new CSLLFFMLPNetwork());
		Network::Clone(np, net);
		np->cdkNetwork = net->cdkNetwork;
		return np;
	}

	JRNN::NetworkPtr CSLLFFMLPNetwork::Clone()
	{
		CSLLFFMLPNetPtr oldP = CSLLFFMLPSharedFromThis::shared_from_this();
		return CSLLFFMLPNetwork::Clone(oldP);
	}

	void CSLLFFMLPNetwork::Activate( vecDouble inputs )
	{
		cdkNetwork->Activate(inputs);
		Network::Activate(inputs);
	}

	JRNN::CSLLFFMLPNetPtr CSLLFFMLPNetwork::Create()
	{
		CSLLFFMLPNetPtr np(new CSLLFFMLPNetwork());
		return np;
	}

	CSLLFFMLPNetwork::CSLLFFMLPNetwork(){}

	CSLLFFMLPNetwork::~CSLLFFMLPNetwork(){}

	ConPtr CSLLFFMLPNetwork::ConnectExt( NodePtr n, NodePtr n2 )
	{
		ConPtr cp = Connection::Connect(n,n2);
		cp->SetScaleAndOffset(conScale, conOffset);
		cp->Reset();
		string baseName = cp->GetName();
		string newName = "ext_" + baseName;
		cp->SetName(newName);
		return cp;
	}



}