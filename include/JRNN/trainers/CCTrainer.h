/* 
 * File:   CCTrainer.h
 * Author: jreeder
 * 
 * Created on June 9, 2010, 3:00 PM
 */

#ifndef _CCTRAINER_H
#define _CCTRAINER_H

namespace JRNN {
	class CCTrainer {
	public:
		CCTrainer(NetworkPtr network, DatasetPtr data);
		~CCTrainer();
		
	private:
		NetworkPtr network;
		DatasetPtr data;
		int numCandidates;
		int patienceEpochs;
		double patienceErrDelta;
		LayerPtr candidateLayer;
		ConList candidateCons;
		matDouble errorBuffer;
		NodeBuffer nodeBuffer;
	};
}

#endif