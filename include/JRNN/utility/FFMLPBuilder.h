/* 
 * File:   FFMLPBuilder.h
 * Author: jreeder
 * 
 * Created on July 13, 2010
 */
#pragma once
#ifndef _FFMLPBUILDER_H
#define _FFMLPBUILDER_H

#include "JRNN.h"
#include "NetworkBuilder.h"

namespace JRNN {
	class FFMLPBuilder : public NetworkBuilder {
		
	public: 
		FFMLPBuilder(int numIn, int numHid, int numOut);
		~FFMLPBuilder(){};
		NetworkPtr Create();

	private:
		int numIn;
		int numHid;
		int numOut;

		void FullyConnect(NetworkPtr np);
	};

}

#endif