/* 
 * File:   FFMLPBuilder.h
 * Author: jreeder
 * 
 * Created on July 13, 2010
 */
#pragma once
#ifndef _FFMLPNETWORK_H
#define _FFMLPNETWORK_H

#include "JRNN.h"
#include "structure/network.h"

namespace JRNN {
	class FFMLPNetwork;
	typedef NetworkPtrHelper<FFMLPNetwork>::Ptr FFMLPNetPtr;
	typedef boost::enable_shared_from_this<FFMLPNetwork> FFMLPSharedFromThis;

	class FFMLPNetwork : public Network, public FFMLPSharedFromThis {
		
	public: 
		FFMLPNetwork();
		~FFMLPNetwork(){};
		static FFMLPNetPtr Create();
		void Build(int numIn, int numHid, int numOut);
		static FFMLPNetPtr Clone( FFMLPNetPtr net);
		virtual NetworkPtr Clone();

	private:
		int numHid;

		void FullyConnect();
		
	};

}

#endif