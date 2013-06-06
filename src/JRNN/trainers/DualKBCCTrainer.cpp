/********************************************************************
	created:	2013/05/29
	created:	29:5:2013   22:17
	filename: 	DualKBCCTrainer.cpp
	author:		jreeder
	
	purpose:	RevKBCC without the Rev. Just two networks and buffer.
*********************************************************************/

#include "JRNN.h"
#include "trainers/DualKBCCTrainer.h"
#include "utility/dataset.h"
#include "utility/csmtldataset.h"

namespace JRNN {
	RandomGenerator01 DualKBCCTrainer::revRand = RandomGenerator01();

	DualKBCCTrainer::DualKBCCTrainer( int numIn, int numOut, int numCandidates, string outNodeType /*= ASigmoid::_type*/ )
	{
		net1 = KBCCNetwork::Create();
		net2 = KBCCNetwork::Create();
		net1->Build(numIn, numOut, false, false, false, outNodeType);
		net2->Build(numIn, numOut, false, false, false, outNodeType);
		network = net1;
		firstTrained = false;
		ScopedOut = false;
		realOuts = false;
		outTestDStype = Dataset::TEST;
		bufferDS.reset(new Dataset());
		bufferDS->SetNumInputs(numIn);
		bufferDS->SetNumOutputs(numOut);
		revRand.gen.seed(static_cast<unsigned int>(time(NULL)));
		parms.nCand = numCandidates;
		revparams.bufferSize = 200;
		revparams.numRev = 5;
		revparams.numRevTrainRatio = 1;
		revparams.cleanReverb = false;
		revparams.numContexts = 0;
		FinishSetup();
	}

	void DualKBCCTrainer::Reset(){
		if(data.get() != 0){
			ResetVars();
		}
		net1->Reset();
		net2->Reset();
		network = net1;
		firstTrained = false;
		ScopedOut = false;
		TestWhileTrainResults.clear();
		outTestDS.reset();
		bufferDS->Clear();
		SaveNetParameters(net1vals);
		SaveNetParameters(net2vals);
		SubNetlist = resSubNetList;
	}

	void DualKBCCTrainer::FinishSetup(){
		int numOut = network->GetNumOut();
		err.errors = vecDouble(numOut);
		err.sumErrs = vecDouble(numOut);
		err.measure = (ErrorType) parms.errorMeasure;
		valErr.errors = vecDouble(numOut);
		valErr.sumErrs = vecDouble(numOut);
		valErr.measure = (ErrorType) parms.errorMeasure;
		candCorr = hashedVecDoubleMap(numOut);
		candPCorr = hashedVecDoubleMap(numOut);
		candBCorr = hashedVecDoubleMap(numOut);
		networkCache = hashedVecDoubleMap(numOut);
		//ResetVars(); Not needed This is the first thing that traintoconvergence does.
	}

	DualKBCCTrainer::~DualKBCCTrainer(){}

	DualKBCCTrainer::reverbdpoint DualKBCCTrainer::ReverberateNetwork()
	{
		//changing the clean reverb to only threshold the outputs. 
		//if the numcontexts is set above 0 then we'll get them from the dataset.
		int inSize = revNet->GetNumIn();
		if (revparams.numContexts > 0){
			//assert(revparams.numContexts > 0);
			inSize -= revparams.numContexts;
		}

		vecDouble tmpIn(inSize);
		for (uint i = 0; i < tmpIn.size(); i++)
		{
			tmpIn[i] = revRand();
		}
		if (revparams.numContexts > 0){
			vecDouble randContext = dynamic_pointer_cast<CSMTLDataset>(data)->GetRandContext();
			tmpIn = ConcatVec(tmpIn,randContext);
		}
		revNet->Activate(tmpIn);
		/*for (uint i = 1; i < revparams.numRev; i++)
		{
			tmpIn = revNet->GetAutoAssocLayer()->GetOutput();
			revNet->Activate(tmpIn);
		}*/
		reverbdpoint retVal;
		retVal.inPoint = tmpIn;
		retVal.outPoint = revNet->GetOutputs();

		if(revparams.cleanReverb){ //This shouldn't be used when the outputs are real. 
			retVal.outPoint = ApplyThreshold(retVal.outPoint);
		}

#ifdef _DEBUG
		cout << "in: " << retVal.inPoint << "out" << retVal.outPoint << endl;
#endif
		return retVal;
	}

	vecDouble DualKBCCTrainer::ActivateNet( vecDouble inPoint, vecDouble outPoint )
	{
		//used to make the subclassing easier. 
		//This just activates the network and passes the correct desired out
		//back to be used 
		vecDouble retVal;
		/*if (dynamic_pointer_cast<RevKBCCNetwork>(network)->getTrueOuts == true){
			retVal = outPoint;
		}
		else {
			retVal = ConcatVec(outPoint, inPoint);
		}*/
		retVal = outPoint;

		if(useNetCache) {
			network->SetOutputs(GetCachedOuts(inPoint)); 
		}
		else {
			network->Activate(inPoint);
		}
		return retVal;
	}

	vecDouble DualKBCCTrainer::Activate( vecDouble inPoint )
	{
		//This is for convenience to activate the primary network and return it's true outs. 
		vecDouble retVal;
		net1->Activate(inPoint);
		retVal = net1->GetOutputs();
		return retVal;
	}

	vecDouble DualKBCCTrainer::ConcatVec( vecDouble first, vecDouble second )
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

	double DualKBCCTrainer::TestOnData(DatasetPtr testData, Dataset::datatype type)
	{
		double retVal;
		//net1->getTrueOuts = true;
		network = net1;
		DatasetPtr oldData = this->data;
		SetDataSet(testData);
		retVal = CCTrainer::TestOnData(type);
		//net1->getTrueOuts = false;
		SetDataSet(oldData);
		return retVal;
	}

	//double DualKBCCTrainer::TestOnData( Dataset::datatype type, DatasetPtr testData /*= DatasetPtr()*/, bool useTrueOuts /*= true*/ )
	//{
	//	double retVal;
	//	network = net1;
	//	DatasetPtr oldData = this->data;
	//	if (testData.get() != 0){
	//		testData = this->data;
	//	}
	//	if (useTrueOuts)
	//		net1->getTrueOuts = true;
	//	
	//	SetDataSet(testData);
	//	retVal = CCTrainer::TestOnData(type);
	//	if (useTrueOuts)
	//		net1->getTrueOuts = false;
	//	
	//	SetDataSet(oldData);
	//	return retVal;
	//}

	JRNN::hashedDoubleMap DualKBCCTrainer::TestWiClass(DatasetPtr testData, Dataset::datatype type)
	{
		hashedDoubleMap retVal;
		//net1->getTrueOuts = true;
		network = net1;
		DatasetPtr oldData = this->data;
		SetDataSet(testData);
		retVal = CCTrainer::TestWiClass(type);
		//net1->getTrueOuts = false;
		SetDataSet(oldData);
		return retVal;
	}

	void DualKBCCTrainer::TrainTask( DatasetPtr taskData,int maxEpochs, bool validate, bool testWhileTrain /*= false*/, DatasetPtr testData /*= DatasetPtr()*/, Dataset::datatype testDataType /*= Dataset::TRAIN*/ )
	{
		if (revparams.bufferSize > 0) //only do dual network if there is a buffer size, simple way to turn on and off dual network.
		{
			if (!firstTrained)
			{
				//Initial Training
				net1list = SubNetlist;
				net2list = SubNetlist;
				SetDataSet(taskData);
				network = net1;
				TrainToConvergence(maxEpochs, validate);
				net1list = SubNetlist;
				SaveNetParameters(net1vals);
				//First Stage I
				revNet = net1;
				FillBufferDS(revparams.bufferSize);
				network = net2; //This has to happen before SetDataset
				SetDataSet(bufferDS);
				SubNetlist = net2list;
				TrainToConvergence(maxEpochs);
				net2list = SubNetlist;
				SaveNetParameters(net2vals);
				bufferDS->Clear();
				firstTrained = true;
			}
			else {
				//These only make since after the first round of training.
				this->ScopedOut = testWhileTrain;
				this->outTestDS = testData;
				if(testData.get() != 0){
					this->realOuts = testData->getRealOuts();
				}
				this->outTestDStype = testDataType;
				//Stage II
				revNet = net2;
				//right here we add validation items to the buffer ds so if we validate the new task we also validate the buffer
				FillBufferDS(static_cast<int>(floor(taskData->GetSize(Dataset::TRAIN) * revparams.numRevTrainRatio)), validate);
				taskData->MergeSubsets(bufferDS);
				bufferDS->Clear();
				SetDataSet(taskData);
				network = net1;
				SubNetlist = net1list;
				TrainToConvergence(maxEpochs, validate);
				net1list = SubNetlist;
				SaveNetParameters(net1vals);
	
				//Turn this off for Stage I
				this->ScopedOut = false;
				//Stage I
				revNet = net1;
				FillBufferDS(revparams.bufferSize);
				network = net2;
				SetDataSet(bufferDS);
				SubNetlist = net2list;
				TrainToConvergence(maxEpochs); //Not validating right now might need to come back and do that later. would require buffer dataset to have a validation set. 
				net2list = SubNetlist;
				SaveNetParameters(net2vals);
				bufferDS->Clear();
			}
			
			//clean up by putting net1 back as the main network
			network = net1;
			SetDataSet(taskData);
		} 
		else
		{
			this->ScopedOut = testWhileTrain;
			this->outTestDS = testData;
			if(testData.get() != 0){
				this->realOuts = testData->getRealOuts();
			}
			this->outTestDStype = testDataType;
			SetDataSet(taskData);
			network = net1;
			TrainToConvergence(maxEpochs, validate);
			SaveNetParameters(net1vals);
			this->ScopedOut = false;
		}
		
	}

	void DualKBCCTrainer::FillBufferDS( int numPoints, bool validate /*= false */ )
	{
		matDouble newInputs;
		matDouble newOutputs;
		//changed this to add validation data to buffer if requested. 
		if (validate) {
			numPoints *= 2;
		}

		for (int i = 0; i < numPoints; i++)
		{
           	reverbdpoint dpoint = ReverberateNetwork();
			newInputs.push_back(dpoint.inPoint);
			newOutputs.push_back(dpoint.outPoint);
		}
		bufferDS->LoadFromMatDoubles(newInputs, newOutputs);
		if (validate){
			bufferDS->DistData(numPoints/2, numPoints/2, 0);
		}
		else {
			bufferDS->DistData(numPoints,0,0); 
		}
		
	}


	JRNN::DatasetPtr DualKBCCTrainer::ReverbMainNet( int numTrain, int numVal /*= 0*/ )
	{
		matDouble newInputs;
		matDouble newOutputs;
		int totalPoints = numTrain + numVal;
		DatasetPtr newDS(new Dataset());
		revNet = net1;
		
		for (int i = 0; i < totalPoints; i++){
			reverbdpoint dpoint = ReverberateNetwork();
			newInputs.push_back(dpoint.inPoint);
			newOutputs.push_back(dpoint.outPoint);
		}
		newDS->LoadFromMatDoubles(newInputs, newOutputs);
		newDS->DistData(numTrain, numVal, 0);
		return newDS;
	}

	CCTrainer::status DualKBCCTrainer::TrainOuts()
	{
		int quitEpoch = 0;
		double lastError = 0.0;
		int startEpoch = epoch;
		double bestVERR = 0;
		hashedDoubleMap bestWeights;
		if (useTrainOutVal && useValidation)
			bestVERR = CCTrainer::TestOnData(Dataset::VAL);
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
				if (realOuts)
				{
					hashedDoubleMap tmp;
					tmp["task-0"] = TestOnData(outTestDS, outTestDStype);
					tmpResult.result = tmp;
				} 
				else
				{
					tmpResult.result = TestWiClass(outTestDS, outTestDStype);
				}
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

			if (useTrainOutVal && useValidation){ //this changes the condition for stagnation to performance on validation set. 
				double curVERR = CCTrainer::TestOnData(Dataset::VAL);
				if (i == 0 || curVERR < bestVERR){
					bestVERR = curVERR;
					bestWeights = network->GetWeights();
					quitEpoch = epoch + parms.out.patience;
				}
				else if (epoch == quitEpoch){
					network->SetWeights(bestWeights);
					return CCTrainer::STAGNANT;
				}
			}
			else {
				/*if (i == 0){ This was redundant. 
					lastError = err.trueErr;
					quitEpoch = epoch + parms.out.patience;
				}*/
				if (i == 0 || fabs(err.trueErr - lastError) > lastError * parms.out.changeThreshold){
					lastError = err.trueErr;
					quitEpoch = epoch + parms.out.patience;
				}
				else if (epoch == quitEpoch){
					return CCTrainer::STAGNANT;
				}
			}
		}
		if (useTrainOutVal && useValidation)
			network->SetWeights(bestWeights);
		return CCTrainer::TIMEOUT;
	}

	const DualKBCCTrainer::TestResults& DualKBCCTrainer::getTestWhileTrainResults()
	{
		return TestWhileTrainResults;
	}

	void DualKBCCTrainer::SaveNetParameters( netparameters& netparms )
	{
		netparms.epochs = this->GetEpochs();
		netparms.numHidLayers = this->GetNumHidLayers();
		netparms.numResets = this->GetNumResets();
		netparms.MSERec = this->GetMSERec();
		netparms.VMSERec = this->GetVMSERec();
	}

	void DualKBCCTrainer::SetScaleAndOffset( double scale, double offset )
	{
		net1->SetScaleAndOffset(scale, offset);
		net2->SetScaleAndOffset(scale, offset);
	}

	void DualKBCCTrainer::SetSDCCandVaryActFunc( bool useSDCC, bool varyActFunc )
	{
		this->net1->SetUseSDCC(useSDCC);
		this->net2->SetUseSDCC(useSDCC);
		this->net1->SetVaryActFunc(varyActFunc);
		this->net2->SetVaryActFunc(varyActFunc);
	}

	//void DualKBCCTrainer::AddNewInputs( ints inputIndexes, DatasetPtr newData, bool connectToHidden /*= false */ )
	//{
	//	network = net1;
	//	NodeList addedNodes;
	//	AddInputsToNet(inputIndexes, net1, addedNodes);
	//	NodeList addedNodes2;
	//	AddInputsToNet(inputIndexes, net2, addedNodes2);
	//	SetDataSet(newData); //this is moved here so that the new outputs are used in calculating the neccessary values
	//	FinishSetup(); //also used to update based on new AA outputs. 
	//	if (connectToHidden){
	//		network->ConnectToHiddenNodes(addedNodes);
	//		network->LockConnections(true, addedNodes);
	//		ResetVars();
	//		UpdateNet();
	//		network->LockConnections(false);

	//		//Now deal with the backup network
	//		bufferDS->Clear();
	//		revNet = net1;
	//		FillBufferDS(revparams.bufferSize);
	//		network = net2; //This has to happen before SetDataset
	//		SetDataSet(bufferDS);
	//		network->ConnectToHiddenNodes(addedNodes2);
	//		network->LockConnections(true, addedNodes2);
	//		ResetVars();
	//		UpdateNet();
	//		network->LockConnections(false);
	//	}
	//	addedNodes.clear();
	//	addedNodes2.clear();
	//}

	//void DualKBCCTrainer::AddInputsToNet( ints inputIndexes, RevKBCCNetworkPtr inNet, NodeList &addedNodes )
	//{
	//	BOOST_FOREACH(int index, inputIndexes){
	//		//index of -1 means append to the end. 
	//		NodePtr newNode;
	//		if (index == -1){
	//			newNode = inNet->AppendNewInputNode();
	//		}
	//		else if (index < inNet->GetNumIn()){
	//			newNode = inNet->InsertNewInputNode(index);
	//		}
	//		else {
	//			assert(0);
	//		}
	//		addedNodes.push_back(newNode);
	//	}
	//}

	void DualKBCCTrainer::AddPrevTrainedNets( KBCCNetworkPtr inNet1, KBCCNetworkPtr inNet2 )
	{
		net1 = inNet1;
		net2 = inNet2;
		firstTrained = true;
	}

	JRNN::KBCCNetworkPtr DualKBCCTrainer::GetNet1()
	{
		return net1;
	}

	JRNN::KBCCNetworkPtr DualKBCCTrainer::GetNet2()
	{
		return net2;
	}

	void DualKBCCTrainer::SetUseRecurrence( bool useRecurrence )
	{
		net1->SetUseRecurrency(useRecurrence);
		net2->SetUseRecurrency(useRecurrence);
	}

}