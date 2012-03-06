/********************************************************************
	created:	2012/03/05
	created:	5:3:2012   18:19
	filename: 	RevCCNetwork.h
	author:		jreeder
	
	purpose:	RevCCNetwork class. Meant to add the reverberating 
				process to the CC network. This will be the building block
				of the life long learning system. 
*********************************************************************/

#include "JRNN.h"
#include "networks/CCNetwork.h"

namespace JRNN {
	class RevCCNetwork;
	typedef NetworkPtrHelper<RevCCNetwork>::Ptr RevCCNetworkPtr;
	typedef boost::enable_shared_from_this<RevCCNetwork> RevCCSharedFromThis;
}