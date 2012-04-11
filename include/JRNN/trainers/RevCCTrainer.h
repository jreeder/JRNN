/********************************************************************
	created:	2012/03/21
	created:	21:3:2012   18:10
	filename: 	RevCCTrainer.h
	author:		jreeder
	
	purpose:	Trainer for reverberating CC. This will have two networks
				that work together to alleviate catastrophic forgetting. 
*********************************************************************/

#ifndef _REVCCTRAINER_H
#define _REVCCTRAINER_H

#include "JRNN.h"
#include "trainers/CCTrainer.h"
#include "networks/RevCCNetwork.h"

namespace JRNN {
	
	class RevCCTrainer : public CCTrainer
	{
	public:
		RevCCTrainer(int numIn, int numOut, int numCandidates);
		~RevCCTrainer();
		
		struct TestResult {
			int epoch;
			hashedDoubleMap result;
		};
		
		typedef std::vector<TestResult> TestResults;

		struct revparameters {
			uint numRev;
			uint numRevTrainRatio; //multiplier for the number of rev training points to inject into net1
			int bufferSize;
			bool cleanReverb;
			int numContexts;
		} revparams;

		struct reverbdpoint {
			vecDouble inPoint;
			vecDouble outPoint;
		};

		//Values that are changed after each training. Keep them here so they can be recalled if need be.
		struct netparameters {
			int epochs;
			int numResets;
			int numHidLayers;
			doubles MSERec;
			doubles VMSERec;
		} net1vals, net2vals;

		void TrainTask(DatasetPtr taskData,int maxEpochs, bool validate, bool testWhileTrain = false, DatasetPtr testData = DatasetPtr(), Dataset::datatype testDataType = Dataset::TRAIN);
		
		double TestOnData(DatasetPtr testData, Dataset::datatype type);
		hashedDoubleMap TestWiClass(DatasetPtr testData, Dataset::datatype type);

		const TestResults& getTestWhileTrainResults();

		void Reset();

	protected:
		
		bool firstTrained;
		bool ScopedOut; //Do I want to monitor the performance of a previous dataset during training

		RevCCNetworkPtr net1;
		RevCCNetworkPtr net2;
		RevCCNetworkPtr revNet;

		DatasetPtr bufferDS;
		DatasetPtr outTestDS; //The dataset that will be monitored during training. 
		Dataset::datatype outTestDStype;

		TestResults TestWhileTrainResults;
		
		enum Stage {
			INIT,
			STAGEI,
			STAGEII
		} trainStage;

		enum Phase {
			TRAIN,
			TEST
		} algPhase;

		reverbdpoint ReverberateNetwork();

		virtual vecDouble ActivateNet(vecDouble inPoint, vecDouble outPoint);
		
		vecDouble ConcatVec( vecDouble first, vecDouble second );
		void FinishSetup();
		void FillBufferDS( int numPoints );

		void SaveNetParameters(netparameters& netparms);
		
		//Overloaded so that I can insert tests after each epoch. 
		CCTrainer::status TrainOuts();
		
		static RandomGenerator01 revRand;
		//conVars assoc;


	};
}

#endif