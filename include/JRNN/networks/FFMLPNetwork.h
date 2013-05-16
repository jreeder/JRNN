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
	typedef boost::shared_ptr<FFMLPNetwork> FFMLPNetPtr;
	typedef derived_shared_from_this<FFMLPNetwork> FFMLPSharedFromThis;

	class FFMLPNetwork : public Network, public FFMLPSharedFromThis {
		
	public: 
		static const string Type;
		virtual string GetType() {
			return Type;
		}
		FFMLPNetwork();
		~FFMLPNetwork(){};
		static FFMLPNetPtr Create();
		void Build(int numIn, int numHid, int numOut, bool cloneouts = false, string outNodeType = ASigmoid::_type, string hidNodeType = ASigmoid::_type );
		static FFMLPNetPtr Clone( FFMLPNetPtr net);
		virtual NetworkPtr Clone();

	protected:

		void FullyConnect(bool cloneouts);
		
	};

}

#endif