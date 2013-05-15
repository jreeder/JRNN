/********************************************************************
	created:	2012/04/26
	created:	26:4:2012   23:03
	filename: 	CSLLLFFMLPNetwork.h
	author:		jreeder
	
	purpose:	Meant to be an analog to Silvers CSLLL system. This 
				network should do most of the work of the mimicry. Should 
				only be used with the CSMTL paradigm. 
*********************************************************************/

#pragma once
#ifndef _CSLLFFMLPNETWORK_H
#define _CSLLFFMLPNETWORK_H

#include "JRNN.h"
#include "networks/FFMLPNetwork.h"

namespace JRNN {
	class CSLLFFMLPNetwork;
	typedef boost::shared_ptr<CSLLFFMLPNetwork> CSLLFFMLPNetPtr;
	typedef derived_shared_from_this<CSLLFFMLPNetwork> CSLLFFMLPSharedFromThis;

	class CSLLFFMLPNetwork : public FFMLPNetwork, public CSLLFFMLPSharedFromThis {
	public:
		static const string Type;
		virtual string GetType() {
			return Type;
		}
		CSLLFFMLPNetwork();
		~CSLLFFMLPNetwork();
		static CSLLFFMLPNetPtr Create();
		void Build(int numIn, int numHid, int numOut, NetworkPtr cdkNetwork);
		static CSLLFFMLPNetPtr Clone ( CSLLFFMLPNetPtr net);
		virtual NetworkPtr Clone();

		virtual void Activate(vecDouble inputs);

	protected:

		NetworkPtr cdkNetwork;
		void ConnectCDK();
		ConPtr ConnectExt( NodePtr n, NodePtr n2 );
	};
}

#endif