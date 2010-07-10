/* 
 * File:   CCTrainer.h
 * Author: jreeder
 * 
 * Created on June 9, 2010, 3:00 PM
 */

#ifndef _CCTRAINER_H
#define _CCTRAINER_H

#include "JRNN.h"
#include "utility/nodebuffer.h"
#include "structure/network.h"
#include "utility/dataset.h"

namespace JRNN {
	class CCTrainer {
	public:
		CCTrainer(NetworkPtr network, DatasetPtr data, int numCandidates);
		~CCTrainer();
		
	private:
		//Members
		NetworkPtr network;
		DatasetPtr data;
		int numCandidates;
		int patienceEpochs;
		double patienceErrDelta;
		LayerPtr candidateLayer;
		ConList candidateCons;
		matDouble errorBuffer;
		NodeBuffer nodeBuffer;

		//Methods
		void QuickProp(LayerPtr layer, vecDouble desiredOut);
		void TrainOuts();
		void TrainCandidates();
		double CalcCorrelation();
	};
}

#endif