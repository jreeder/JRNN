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
	//typedef boost::unordered_map<std::string, vecDouble> hashedVecDoubleMap;
	class CCTrainer {
	public:
		CCTrainer(CCNetworkPtr network, DatasetPtr data, int numCandidates);
		~CCTrainer();

		void ResetVars();
		void Reset();
		void TrainToValConv(int maxEpochs);
		void TrainToConvergence(int maxEpochs, bool validate = false);
		double TestOnData(Dataset::datatype type);
		hashedDoubleMap TestWiClass(Dataset::datatype type);
		int GetEpochs();
		int GetNumHidLayers();
		
		struct parameters
		{
			int nTrials;
			int maxNewUnits;
			int valPatience;
			double weightMult;

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

			double sseThreshold;
			double candScoreThreshold;

			int nCand;
		} parms;
		
	private:
		//Members
		CCNetworkPtr network;
		DatasetPtr data;
		//LayerPtr candidateLayer;
		//ConList candidateCons;
		//TODO Implement Network caching
		//matDouble errorBuffer; 
		//NodeBuffer nodeBuffer; //buffer of each nodes output
		//double sumSqErr; //Sum Squared Error Primes
		//double trueErr;
		double candBestScore;
		NodePtr bestCand;
		//vecDouble errors;
		//vecDouble sumErrs;
		hashedDoubleMap taskErrorRate;
		hashedIntMap taskErrors;
		struct valVars{
			hashedDoubleMap bestWeights;
			double bestErr;
			int bestPass;
		} val;
		int epoch;

		//hashedDoubleMap outSumErrs; //sum of errors for each output node
		struct conVars
		{
			hashedDoubleMap conDeltas; //deltas for each weight
			hashedDoubleMap conPSlopes; //current slopes for each weight
			hashedDoubleMap conSlopes; //previous slopes for each weight
		} out, cand;

		struct errVars 
		{
			int bits;
			double trueErr;
			double sumSqErr;
			vecDouble errors;
			vecDouble sumErrs;
		}valErr, err;

		hashedDoubleMap candSumVals; //Sum of candidate activations over training. 
		hashedVecDoubleMap candCorr; //Correlation of each candidate node
		hashedVecDoubleMap candPCorr; //Previous correlation of each candidate node
		
		enum status {
			TRAINING,
			TIMEOUT,
			STAGNANT,
			WIN
		};

		//Methods
		void QuickProp(ConPtr con, conVars& vars, double epsilon, double decay, double mu, double shrinkFactor);
		
		//Output training methods
		void resetError(errVars& errorVars);
		void resetOutValues();
		status TrainOuts();
		void OutputEpoch(); 

		void ComputeError( vecDouble desiredOut, errVars& errs, NodeList &outNodes, bool alterStats, bool updateSlopes);

		//void ComputeOutError();
		void UpdateOutWeights(); //Adjust output weights.

		//Candidate training methods
		void CreateCandidates();
		status TrainCandidates();
		void resetCandValues();
		void CorrelationEpoch();
		void CandEpoch();
		void UpdateCandWeights();
		void UpdateCorrelations();
		void ComputeCorrelations();
		void ComputeCandSlopes();
		void InsertCandidate();

		//Validation Methods

		status ValidationEpoch();



	};
}

#endif