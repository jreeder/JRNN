#pragma once

#include "JRNN.h"
#include "structure/network.h"

namespace JRNN {
	
	class NetworkBuilder
	{
	public:
		NetworkBuilder(void){};
		~NetworkBuilder(void){};

		virtual NetworkPtr Create() = 0;
	};

}


