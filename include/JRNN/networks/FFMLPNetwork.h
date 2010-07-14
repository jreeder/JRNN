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

	class FFMLPNetwork : public Network {
		
	public: 
		FFMLPNetwork();
		~FFMLPNetwork(){};
		static FFMLPNetPtr Create();
		void Build(int numIn, int numHid, int numOut);

	private:
		int numHid;

		void FullyConnect();
	};

}

#endif