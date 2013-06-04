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
		RevCCTrainer(int numIn, int numOut, int numCandidates, string outNodeType = ASigmoid::_type, string autoAssocType = ASigmoid::_type);
		~RevCCTrainer();
		
		struct TestResult {
			int epoch;
			hashedDoubleMap result;
			bool operator==(TestResult const& rhs){
				if(this->epoch == rhs.epoch && this->result == rhs.result){
					return true;
				}
				else {
					return false;
				}
			}
		};
		
		typedef std::vector<TestResult> TestResults;

		struct revparameters {
			uint numRev;
			double numRevTrainRatio; //multiplier for the number of rev training points to inject into net1
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
		//double TestOnData(Dataset::datatype type, DatasetPtr testData = DatasetPtr(), bool useTrueOuts = true);
		hashedDoubleMap TestWiClass(DatasetPtr testData, Dataset::datatype type);

		vecDouble Activate(vecDouble inPoint);

		const TestResults& getTestWhileTrainResults();

		void SetScaleAndOffset(double scale, double offset);

		void SetSDCCandVaryActFunc( bool useSDCC, bool varyActFunc );

		void SetUseRecurrence(bool useRecurrence);

		virtual void AddNewInputs( ints inputIndexes, DatasetPtr newData, bool connectToHidden = false );

		//these functions are used to get and set the networks. So i can save them away. 
		void AddPrevTrainedNets( RevCCNetworkPtr inNet1, RevCCNetworkPtr inNet2);
		RevCCNetworkPtr GetNet1();
		RevCCNetworkPtr GetNet2();

		void Reset();

	protected:
		
		bool firstTrained;
		bool ScopedOut; //Do I want to monitor the performance of a previous dataset during training
		bool realOuts;

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
		
		void AddInputsToNet( ints inputIndexes, RevCCNetworkPtr inNet, NodeList &addedNodes );

		vecDouble ConcatVec( vecDouble first, vecDouble second );
		void FinishSetup();
		void FillBufferDS( int numPoints, bool validate = false );

		void SaveNetParameters(netparameters& netparms);
		
		//Overloaded so that I can insert tests after each epoch. 
		CCTrainer::status TrainOuts();

		static RandomGenerator01 revRand;
		//conVars assoc;


	};
}

#endif