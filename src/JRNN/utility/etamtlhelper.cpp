/********************************************************************
	created:	2011/10/19
	created:	19:10:2011   18:48
	filename: 	etamtlhelper.cpp
	author:		jreeder
	
	purpose:	Utility class to help with eta mtl stuff. Meant to be 
				inherited. 
*********************************************************************/

#include "JRNN.h"
#include "utility/etamtlhelper.h"

namespace JRNN {

	typedef unsigned int uint;

	double EtaMTLHelper::getRk(int secIndex){
		double retVal = 0;
		retVal = std::tanh((ak[secIndex]/(std::pow(dk[secIndex],2) + Psi)) * (1/RELMIN));
		return retVal;
	}

	void EtaMTLHelper::calcAks(vecDouble outsSSE){
		FillVec(ak, 1.0);
		ak = VecDivide(ak, outsSSE);
	}

	void EtaMTLHelper::calcDks(NodeList outNodes){
		vecDouble primConWeights = GetInWeights(outNodes[primIndex]);
		for (int i = 0; i < outNodes.size(); i++){
			if (i != primIndex){
				vecDouble secConWeights = GetInWeights(outNodes[i]);
				dk[i] = VecDistance(primConWeights, secConWeights);
			}
			else {
				dk[i] = 0;
			}
		}

	}

	vecDouble EtaMTLHelper::GetInWeights(NodePtr node){
		ConList cons = node->GetConnections(IN);
		vecDouble retVec(cons.size());
		for (uint i = 0; i < cons.size(); i++){
			retVec[i] = cons[i]->GetWeight();
		}
		return retVec;
	}
}