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
		
		struct  
		{
			int nTrials;
			int maxNewUnites;
			int valPatience;

			struct  
			{
				int epochs;
				int patience;
				double epsilon;
				double decay;
				double mu;
				double changeThreshold;
			} out;

			struct  
			{
				int epochs;
				int patience;
				double epsilon;
				double decay;
				double mu;
				double changeThreshold;
			} cand;

			int nCand;
		} parms;
		
	private:
		//Members
		NetworkPtr network;
		DatasetPtr data;
		LayerPtr candidateLayer;
		ConList candidateCons;
		//TODO Implement Network caching
		//matDouble errorBuffer; 
		//NodeBuffer nodeBuffer; //buffer of each nodes output
		double sumSqErr; //Sum Squared Error Primes
		hashedDoubleMap outSumErrs; //sum of errors for each output node
		hashedDoubleMap conDeltas; //deltas for each weight
		hashedDoubleMap conSlopes; //current slopes for each weight
		hashedDoubleMap conPSlopes; //previous slopes for each weight
		hashedDoubleMap candSumVals; //Sum of candidate activations over training. 
		hashedDoubleMap candCorr; //Correlation of each candidate node
		hashedDoubleMap candPCore; //Previous correlation of each candidate node

		//Methods
		void QuickProp(ConPtr con, double epsilon, double decay, double mu, double shrinkFactor);
		//Output training methods
		void resetError();
		void TrainOuts();
		void OutputEpoch();
		void ComputerOutError();
		void UpdateOutWeights(); //Adjust output weights.

		//Candidate training methods
		void TrainCandidates();
		void CorrelationEpoch();
		void CandEpoch();
		void UpdateCandWeights();
		void UpdateCorrelations();
		void ComputeCorrelations();
		void ComputeCandSlopes();

	};
}

#endif