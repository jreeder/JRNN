/********************************************************************
	created:	2013/05/29
	created:	29:5:2013   2:14
	filename: 	RevKBCCTrainer.h
	author:		jreeder
	
	purpose:	Trainer with KBCC and Reverberation
*********************************************************************/

#ifndef _REVKBCCTRAINER_H
#define _REVKBCCTRAINER_H

#include "JRNN.h"
#include "networks/RevKBCCNetwork.h"
#include "trainers/KBCCTrainer.h"

namespace JRNN {
	class RevKBCCTrainer : public KBCCTrainer {
	public:
		RevKBCCTrainer(int numIn, int numOut, int numCandidates, string outNodeType = ASigmoid::_type, string autoAssocType = ASigmoid::_type);
		~RevKBCCTrainer();

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

		DatasetPtr ReverbMainNet( int numTrain, int numVal = 0, strings subView = strings(0) );

		//virtual void AddNewInputs( ints inputIndexes, DatasetPtr newData, bool connectToHidden = false );

		//these functions are used to get and set the networks. So i can save them away. 
		void AddPrevTrainedNets( RevKBCCNetworkPtr inNet1, RevKBCCNetworkPtr inNet2);
		RevKBCCNetworkPtr GetNet1();
		RevKBCCNetworkPtr GetNet2();

		void Reset();

	protected:

		bool firstTrained;
		bool ScopedOut; //Do I want to monitor the performance of a previous dataset during training
		bool realOuts;

		RevKBCCNetworkPtr net1;
		RevKBCCNetworkPtr net2;
		RevKBCCNetworkPtr revNet;

		NetPtrList net1list;
		NetPtrList net2list;

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

		reverbdpoint ReverberateNetwork(strings subView = strings(0));

		virtual vecDouble ActivateNet(vecDouble inPoint, vecDouble outPoint);

		//void AddInputsToNet( ints inputIndexes, RevKBCCNetworkPtr inNet, NodeList &addedNodes );

		vecDouble ConcatVec( vecDouble first, vecDouble second );
		void FinishSetup();
		void FillBufferDS( int numPoints, bool validate = false );

		void SaveNetParameters(netparameters& netparms);

		//Overloaded so that I can insert tests after each epoch. 
		KBCCTrainer::status TrainOuts();

		static RandomGenerator01 revRand;
		//conVars assoc;


	};
}

#endif