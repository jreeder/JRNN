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

//#define _CC_USE_CACHE_

namespace JRNN {
	//typedef boost::unordered_map<string, vecDouble> hashedVecDoubleMap;
	class CCTrainer {
	public:
		CCTrainer(CCNetworkPtr network, DatasetPtr data, int numCandidates, ints primaryIndexes = ints(0));
		virtual ~CCTrainer();

		enum ErrorType {
			BITS,
			INDEX
		};

		void ResetVars();
		virtual void Reset();
		void RedistData();

		void SetDataSet(DatasetPtr dataset);

		virtual void AddNewInputs(ints inputIndexes, DatasetPtr newData, bool connectToHidden = false);

		//Training Methods
		void TrainToValConv(int maxEpochs);
		void TrainToConvergence(int maxEpochs, bool validate = false);

		//Testing Methods
		virtual double TestOnData(Dataset::datatype type);
		virtual hashedDoubleMap TestWiClass(Dataset::datatype type);

		//Network information methods. 
		int GetEpochs();
		int GetNumResets();
		int GetNumHidLayers();
		doubles& GetMSERec();
		doubles& GetVMSERec();

		bool GetUseTrainOutVal() const;
		void SetUseTrainOutVal(bool val);
		
		struct parameters
		{
			int nTrials;
			int maxNewUnits;
			int valPatience;
			int impPatience;
			double weightMult;
			double maxWeight;
			bool useMaxWeight;
			int maxResets;
			double primeOffset;
			bool useSDCC;
			double SDCCRatio;

			struct outParms 
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

			struct candParms
			{
				int epochs;
				int patience;

				double epsilon;
				double shrinkFactor;
				double decay;
				double mu;
				double changeThreshold;
			} cand;

			double indexThreshold;
			double scoreThreshold;
			int errorMeasure;

			int nCand;
		} parms;
		
	protected:
		//Special constructor of my subclass. This is hacky at the moment. 
		CCTrainer();
		//Members
		CCNetworkPtr network;
		DatasetPtr data;
		doubles MSE_Rec;
		doubles vMSE_Rec;
		ints primaryIndexes;
		bool resetFlag;
		int numResets;
		bool useTrainOutVal;
		bool useValidation;
		
		//LayerPtr candidateLayer;
		//ConList candidateCons;
		//TODO Implement Network caching
		//matDouble errorBuffer; 
		//NodeBuffer nodeBuffer; //buffer of each nodes output
		//double sumSqErr; //Sum Squared Error Primes
		//double trueErr;
		double candBestScore;
		int nTrainOutVals; //Training points times num outs
		NodePtr bestCand;
		//vecDouble errors;
		//vecDouble sumErrs;
		hashedDoubleMap taskErrorRate;
		hashedIntMap taskErrors;
		struct valVars{
			hashedDoubleMap bestWeights;
			double bestErr;
			int bestPass;
		} val, imp;
		int epoch;

		//hashedDoubleMap outSumErrs; //sum of errors for each output node
		struct conVars
		{
			hashedDoubleMap conDeltas; //deltas for each weight
			hashedDoubleMap conPSlopes; //current slopes for each weight
			hashedDoubleMap conSlopes; //previous slopes for each weight
			hashedDoubleMap localGradients; //used only for whole net quickprop
		} out, cand, net;

		struct errVars 
		{
			int bits;
			double trueErr;
			double sumSqErr;
			vecDouble errors;
			vecDouble sumErrs;
			ErrorType measure;
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

		bool useNetCache;
		hashedVecDoubleMap networkCache; //This is used to cache network response 
		hashedHashedDoubleMap nodeCache; //First hash is stringed inputs, second is the node name
		string currentInString; //used to pass the current input through the Candidate training

		//Methods
		void QuickProp(ConPtr con, conVars& vars, double epsilon, double decay, double mu, double shrinkFactor);
		double ErrorIndex(double trueError, double stdDev, int nTrainOutVals);
		void PrintStatus(status runStatus);
		int CalcErrorBits(vecDouble error);

		//Output training methods
		void resetError(errVars& errorVars);
		void resetOutValues();
		virtual status TrainOuts();
		void resetTrainOuts();
		void OutputEpoch(); 

		//Retraining methods. Used when adding a new input or to allow all weights to move. 
		status UpdateNet();
		void resetUpdateValues();
		void UpdateNetEpoch();
		void ComputeNetWeightUpdates(LayerPtr layer, vecDouble desiredOut = vecDouble(0));
		void UpdateNetWeights();

		void ComputeError(vecDouble desiredOut, errVars& errs, NodeList& outNodes, bool alterStats, bool updateSlopes);

		//void ComputeOutError();
		void UpdateOutWeights(); //Adjust output weights.

		//Candidate training methods
		virtual void CreateCandidates();
		status TrainCandidates();
		void resetTrainCandidates();
		void resetCandValues();
		virtual void CorrelationEpoch();
		void CandEpoch();
		virtual void UpdateCandWeights();
		virtual void UpdateCorrelations();
		virtual void ComputeCorrelations();
		virtual void ComputeCandSlopes();
		virtual void InsertCandidate();

		//Validation Methods

		status ValidationEpoch();
		status TestImprovement();
		
		//Utility Methods - Make it easier for my subclass. This is hacky should probably change. 
		virtual vecDouble ActivateNet(vecDouble inPoint, vecDouble outPoint);

		vecDouble GetCachedOuts(vecDouble inVec);
		void ClearCache();
		void StoreNodeCache( string inString );
		void CacheActivateCands( const LayerPtr candL );
		//Used for adding new inputs. Done in the ccnetwork class
		//void ConnectToHiddenNodes( NodeList addedNodes );
	};
}

#endif