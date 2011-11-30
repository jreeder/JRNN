/********************************************************************
	created:	2011/10/19
	created:	19:10:2011   18:31
	filename: 	etamtlhelper.h
	author:		jreeder
	
	purpose:	Utility class to help with eta mtl stuff. Meant to be 
				inherited.
*********************************************************************/

#ifndef _ETAMTLHELPER_H
#define _ETAMTLHELPER_H

#include "JRNN.h"
#include "structure/node.h"
#include "structure/layer.h"
#include "structure/connection.h"

namespace JRNN {
	
	class EtaMTLHelper {
	public:
		//TODO for now this will be specific to only one primary index. Will need to expand this later for generality sake. 

		EtaMTLHelper(int outSize);
		~EtaMTLHelper();
		void SetEtaVars(int primindex, double psi, double relmin);
		void SetRELMIN(double relmin);
		void SetPrimeIndex(int ind);
		void SetPsi(double psi);
	
	protected:
		int primIndex;
		int outSize;
		double Psi;
		double RELMIN;
		vecDouble ak;
		vecDouble dk;
		vecDouble Rks;
		vecDouble outSSEs;

		vecDouble GetInWeights(NodePtr node);			
		double GetRk(int secIndex);
		void CalcAks();
		void CalcDks(NodeList& outNodes);
		void CalcRks(NodeList& outNodes);
	};
	
}
#endif // _ETAMTLHELPER_H