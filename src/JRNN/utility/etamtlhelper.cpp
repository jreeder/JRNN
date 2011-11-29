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

	EtaMTLHelper::EtaMTLHelper(int outSize){
		//default values. 
		this->Psi = 0.000001;
		this->RELMIN = 0.05;
		this->primIndex = 0;
		this->outSize = outSize;
		Rks = vecDouble(outSize);
		outSSEs = vecDouble(outSize);
		dk = vecDouble(outSize);
		ak = vecDouble(outSize);
		FillVec(Rks, 1.0);
		FillVec(outSSEs, 0.0);
	}

	EtaMTLHelper::~EtaMTLHelper(){
		//nothing to destruct. 
	}

	void EtaMTLHelper::SetEtaVars(int primindex, double psi, double relmin){
		this->primIndex = primindex;
		this->Psi = psi;
		this->RELMIN = relmin;
	}

	double EtaMTLHelper::GetRk(int secIndex){
		double retVal = 0;
		retVal = std::tanh((ak[secIndex]/(std::pow(dk[secIndex],2) + Psi)) * (1/RELMIN));
		double ak_ = ak[secIndex];
		double dk_2 = (std::pow(dk[secIndex],2) + Psi);
		return retVal;
	}

	void EtaMTLHelper::CalcAks(){
		FillVec(ak, 1.0);
		ak = VecDivide(ak, outSSEs);
	}

	void EtaMTLHelper::CalcDks(NodeList& outNodes){
		vecDouble primConWeights = GetInWeights(outNodes[primIndex]);
		for (uint i = 0; i < outNodes.size(); i++){
			if (i != primIndex){
				vecDouble secConWeights = GetInWeights(outNodes[i]);
				dk[i] = VecDistance(primConWeights, secConWeights);
			}
			else {
				dk[i] = 0;
			}
		}

	}

	void EtaMTLHelper::CalcRks(NodeList& outNodes){
		CalcAks();
		CalcDks(outNodes);
		for (uint i = 0; i < Rks.size(); i++){
			Rks[i] = GetRk(i);
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