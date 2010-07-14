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
#include "networks/CCNetwork.h"
#include "utility/dataset.h"

namespace JRNN {
	class CCTrainer {
	public:
		CCTrainer(CCNetworkPtr network, DatasetPtr data, int numCandidates);
		~CCTrainer();

		void ResetVars();
		
		struct  
		{
			int nTrials;
			int maxNewUnits;
			int valPatience;

			struct  
			{
				int epochs;
				int patience;
				double epsilon;
				double scaledEpsilon;
				double shrinkFactor;
				double decay;
				double mu;
				double changeThreshold;
			} out;

			struct  
			{
				int epochs;
				int patience;

				double epsilon;
				double shrinkFactor;
				double decay;
				double mu;
				double changeThreshold;
			} cand;

			int nCand;
		} parms;
		
	private:
		//Members
		CCNetworkPtr network;
		DatasetPtr data;
		LayerPtr candidateLayer;
		ConList candidateCons;
		//TODO Implement Network caching
		//matDouble errorBuffer; 
		//NodeBuffer nodeBuffer; //buffer of each nodes output
		double sumSqErr; //Sum Squared Error Primes
		hashedDoubleMap outSumErrs; //sum of errors for each output node
		struct conVars
		{
			hashedDoubleMap conDeltas; //deltas for each weight
			hashedDoubleMap conPSlopes; //current slopes for each weight
			hashedDoubleMap conSlopes; //previous slopes for each weight
		} out, cand;
		hashedDoubleMap candSumVals; //Sum of candidate activations over training. 
		hashedDoubleMap candCorr; //Correlation of each candidate node
		hashedDoubleMap candPCore; //Previous correlation of each candidate node

		//Methods
		void QuickProp(ConPtr con, conVars& vars, double epsilon, double decay, double mu, double shrinkFactor);
		//Output training methods
		void resetError();
		void resetOutValues();
		void TrainOuts();
		void OutputEpoch();
		void ComputerOutError();
		void UpdateOutWeights(); //Adjust output weights.

		//Candidate training methods
		void CreateCandidates();
		void TrainCandidates();
		void resetCandValues();
		void CorrelationEpoch();
		void CandEpoch();
		void UpdateCandWeights();
		void UpdateCorrelations();
		void ComputeCorrelations();
		void ComputeCandSlopes();

	};
}

#endif