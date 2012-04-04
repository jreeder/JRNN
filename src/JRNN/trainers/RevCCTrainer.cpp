/********************************************************************
	created:	2012/03/23
	created:	23:3:2012   0:46
	filename: 	RevCCTrainer.cpp
	author:		jreeder
	
	purpose:	Trainer for reverberating CC. This will have two networks
	that work together to alleviate catastrophic forgetting. 
*********************************************************************/
#include "JRNN.h"
#include "trainers/RevCCTrainer.h"
#include "utility/dataset.h"

namespace JRNN {

	RandomGenerator01 RevCCTrainer::revRand = RandomGenerator01();

	RevCCTrainer::RevCCTrainer(int numIn, int numOut, int numCandidates){
		net1 = RevCCNetwork::Create();
		net2 = RevCCNetwork::Create();
		net1->Build(numIn, numOut);
		net2->Build(numIn, numOut);
		network = net1;
		firstTrained = false;
		ScopedOut = false;
		outTestDStype = Dataset::TEST;
		bufferDS.reset(new Dataset());
		bufferDS->SetNumInputs(numIn);
		bufferDS->SetNumOutputs(numOut);
		revRand.gen.seed(static_cast<unsigned int>(time(NULL)));
		parms.nCand = numCandidates;
		revparams.bufferSize = 200;
		revparams.numRev = 5;
		revparams.numRevTrainRatio = 1;
		FinishSetup();
	}

	void RevCCTrainer::Reset(){
		ResetVars();
		net1->Reset();
		net2->Reset();
		network = net1;
		firstTrained = false;
		ScopedOut = false;
		TestWhileTrainResults.clear();
		outTestDS.reset();
		bufferDS->Clear();
	}

	void RevCCTrainer::FinishSetup(){
		int numOut = network->GetNumOut();
		err.errors = vecDouble(numOut);
		err.sumErrs = vecDouble(numOut);
		err.measure = (ErrorType) parms.errorMeasure;
		valErr.errors = vecDouble(numOut);
		valErr.sumErrs = vecDouble(numOut);
		valErr.measure = (ErrorType) parms.errorMeasure;
		candCorr = hashedVecDoubleMap(numOut);
		candPCorr = hashedVecDoubleMap(numOut);
		//ResetVars(); Not needed This is the first thing that traintoconvergence does.
	}

	RevCCTrainer::~RevCCTrainer(){}

	RevCCTrainer::reverbdpoint RevCCTrainer::ReverberateNetwork()
	{
		//Right now I'm letting this randomly generate all of these. I need to at some point
		//set this up to randomly choose a context. This might be something I could test. 
		//to see if this makes a difference. 

		vecDouble tmpIn(revNet->GetNumIn());
		for (uint i = 0; i < tmpIn.size(); i++)
		{
			tmpIn[i] = revRand();
		}
		revNet->Activate(tmpIn);
		for (uint i = 1; i < revparams.numRev; i++)
		{
			tmpIn = revNet->GetAutoAssocLayer()->GetOutput();
			revNet->Activate(tmpIn);
		}
		reverbdpoint retVal;
		retVal.inPoint = tmpIn;
		retVal.outPoint = revNet->GetNormOutLayer()->GetOutput();

		return retVal;
	}

	vecDouble RevCCTrainer::ActivateNet( vecDouble inPoint, vecDouble outPoint )
	{
		vecDouble retVal;
		retVal = ConcatVec(outPoint, inPoint);
		network->Activate(inPoint);
		return retVal;
	}

	vecDouble RevCCTrainer::ConcatVec( vecDouble first, vecDouble second )
	{
		vecDouble retVec(first.size() + second.size());
		int newSize = first.size() + second.size();
		int i = 0;
		BOOST_FOREACH(double val, first){
			retVec[i] = val;
			i++;
		}
		BOOST_FOREACH(double val, second){
			retVec[i] = val;
			i++;
		}
		return retVec;
	}

	double RevCCTrainer::TestOnData(DatasetPtr testData, Dataset::datatype type)
	{
		double retVal;
		net1->getTrueOuts = true;
		network = net1;
		DatasetPtr oldData = this->data;
		SetDataSet(testData);
		retVal = CCTrainer::TestOnData(type);
		SetDataSet(oldData);
		net1->getTrueOuts = false;
		return retVal;
	}

	JRNN::hashedDoubleMap RevCCTrainer::TestWiClass(DatasetPtr testData, Dataset::datatype type)
	{
		hashedDoubleMap retVal;
		net1->getTrueOuts = true;
		network = net1;
		DatasetPtr oldData = this->data;
		SetDataSet(testData);
		retVal = CCTrainer::TestWiClass(type);
		SetDataSet(oldData);
		net1->getTrueOuts = false;
		return retVal;
	}

	void RevCCTrainer::TrainTask( DatasetPtr taskData,int maxEpochs, bool validate, bool testWhileTrain /*= false*/, DatasetPtr testData /*= DatasetPtr()*/, Dataset::datatype testDataType /*= Dataset::TRAIN*/ )
	{
		if (!firstTrained)
		{
			//Initial Training
			SetDataSet(taskData);
			network = net1;
			TrainToConvergence(maxEpochs, validate);;//TODO: Need to parameterize some of these values. 
			SaveNetParameters(net1vals);
			//First Stage I
			revNet = net1;
			FillBufferDS(revparams.bufferSize);
			network = net2; //This has to happen before SetDataset
			SetDataSet(bufferDS);
			TrainToConvergence(maxEpochs);
			SaveNetParameters(net2vals);
			bufferDS->Clear();
			firstTrained = true;
		}
		else {
			//These only make since after the first round of training.
			this->ScopedOut = testWhileTrain;
			this->outTestDS = testData;
			this->outTestDStype = testDataType;
			//Stage II
			revNet = net2;
			FillBufferDS(taskData->GetSize(Dataset::TRAIN) * revparams.numRevTrainRatio);
			taskData->MergeSubsets(bufferDS);
			bufferDS->Clear();
			SetDataSet(taskData);
			network = net1;
			TrainToConvergence(maxEpochs, validate);
			SaveNetParameters(net1vals);

			//Turn this off for Stage I
			this->ScopedOut = false;
			//Stage I
			revNet = net1;
			FillBufferDS(revparams.bufferSize);
			network = net2;
			SetDataSet(bufferDS);
			TrainToConvergence(maxEpochs); //Not validating right now might need to come back and do that later. would require buffer dataset to have a validation set. 
			SaveNetParameters(net2vals);
			bufferDS->Clear();
		}
		//clean up by putting net1 back as the main network
		network = net1;
		SetDataSet(taskData);
	}

	void RevCCTrainer::FillBufferDS( int numPoints )
	{
		matDouble newInputs;
		matDouble newOutputs;
		for (int i = 0; i < numPoints; i++)
		{
			reverbdpoint dpoint = ReverberateNetwork();
			newInputs.push_back(dpoint.inPoint);
			newOutputs.push_back(dpoint.outPoint);
		}
		bufferDS->LoadFromMatDoubles(newInputs, newOutputs);
		bufferDS->DistData(numPoints,0,0); //TOOD: need to change these to parameters.
	}


	CCTrainer::status RevCCTrainer::TrainOuts()
	{
		int quitEpoch = 0;
		double lastError = 0.0;
		int startEpoch = epoch;
		//resetOutValues();
		for (int i = 0; i < parms.out.epochs; i++){
			resetError(err);

			OutputEpoch();

			if ((err.measure == BITS) && (err.bits == 0)) {
				return CCTrainer::WIN;
			}
			else if (err.measure == INDEX) {
				double index = ErrorIndex(err.trueErr,1,nTrainOutVals);
				if (index < parms.indexThreshold){
					return CCTrainer::WIN;
				}
			}

			UpdateOutWeights();
			epoch++;

			if (ScopedOut){
				TestResult tmpResult;
				tmpResult.epoch = epoch;
				tmpResult.result = TestWiClass(outTestDS, outTestDStype);
				TestWhileTrainResults.push_back(tmpResult);
#ifdef _DEBUG
				cout << epoch << ":";
				std::pair<string, double> p;
				BOOST_FOREACH(p, tmpResult.result){
					cout << p.first << "-" << p.second;
				}
				cout << endl;
#endif
			}

			if (resetFlag == true){ //this is not used currently. 
				epoch = startEpoch;
				i = 0;
				quitEpoch = 0;
				lastError = 0.0;
				resetTrainOuts();
				resetFlag = false;
#ifdef _DEBUG
				cout << "Reset Train Outs" << endl;
#endif // _DEBUG
				continue;
			}

			if (i == 0){
				lastError = err.trueErr;
				quitEpoch = epoch + parms.out.patience;
			}
			else if (fabs(err.trueErr - lastError) > lastError * parms.out.changeThreshold){
				lastError = err.trueErr;
				quitEpoch = epoch + parms.out.patience;
			}
			else if (epoch == quitEpoch){
				return CCTrainer::STAGNANT;
			}
		}
		return CCTrainer::TIMEOUT;
	}

	const RevCCTrainer::TestResults& RevCCTrainer::getTestWhileTrainResults()
	{
		return TestWhileTrainResults;
	}

	void RevCCTrainer::SaveNetParameters( netparameters& netparms )
	{
		netparms.epochs = this->GetEpochs();
		netparms.numHidLayers = this->GetNumHidLayers();
		netparms.numResets = this->GetNumResets();
		netparms.MSERec = this->GetMSERec();
		netparms.VMSERec = this->GetVMSERec();
	}

}