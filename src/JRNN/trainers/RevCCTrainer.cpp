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
		bufferDS.reset(new Dataset());
		bufferDS->SetNumInputs(numIn);
		bufferDS->SetNumOutputs(numOut);
		revRand.gen.seed(static_cast<unsigned int>(time(NULL)));
		parms.nCand = numCandidates;
		FinishSetup();
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
		ResetVars();
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

	double RevCCTrainer::TestOnData(DatasetPtr data)
	{
		double retVal;
		net1->getTrueOuts = true;
		network = net1;
		SetDataSet(data);
		retVal = CCTrainer::TestOnData(Dataset::TEST);
		net1->getTrueOuts = false;
		return retVal;
	}

	JRNN::hashedDoubleMap RevCCTrainer::TestWiClass(DatasetPtr data)
	{
		hashedDoubleMap retVal;
		net1->getTrueOuts = true;
		network = net1;
		SetDataSet(data);
		retVal = CCTrainer::TestWiClass(Dataset::TEST);
		net1->getTrueOuts = false;
		return retVal;
	}

	void RevCCTrainer::TrainTask( DatasetPtr taskData, bool validate )
	{
		if (!firstTrained)
		{
			//Initial Training
			SetDataSet(taskData);
			network = net1;
			TrainToConvergence(3000, validate);;//TODO: Need to parameterize some of these values. 
			
			//First Stage I
			revNet = net1;
			FillBufferDS(300);
			network = net2;
			SetDataSet(bufferDS);
			TrainToConvergence(3000);
			bufferDS->Clear();
			firstTrained = true;
		}
		else {
			//Stage II
			revNet = net2;
			FillBufferDS(taskData->GetSize(Dataset::TRAIN));
			taskData->MergeSubsets(bufferDS);
			bufferDS->Clear();
			SetDataSet(taskData);
			network = net1;
			TrainToConvergence(3000, validate);

			//Stage I
			revNet = net1;
			FillBufferDS(300);
			network = net2;
			SetDataSet(bufferDS);
			TrainToConvergence(3000);
			bufferDS->Clear();
		}
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

}