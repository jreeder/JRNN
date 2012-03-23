/********************************************************************
	created:	2011/05/03
	created:	3:5:2011   19:35
	filename:	csmtldataset.cpp
	author:		jreeder
	
	purpose:	csmtl dataset class. Used to train csmtl networks
*********************************************************************/

#include "JRNN.h"
#include "utility/csmtldataset.h"

namespace JRNN {
	CSMTLDataset::CSMTLDataset()
	{
		newData = true;
		impoverish = false;
		primaryTask = -1;
		numImpTrain = 0;
	}

	CSMTLDataset::CSMTLDataset(const CSMTLDataset& orig ) : Dataset(orig)
	{
		this->dataStore = orig.dataStore;
		this->impoverish = orig.impoverish;
		this->inputStrings = orig.inputStrings;
		this->newData = orig.newData;
		this->numImpTrain = orig.numImpTrain;
		this->primaryTask = orig.primaryTask;
		this->realInputs = orig.realInputs;
		this->taskList = orig.taskList;
		this->view = orig.view;
		this->numRealInputs = orig.numRealInputs;
	}

	void CSMTLDataset::SetView(strings view)
	{
		this->view = view;
		this->dsAnalyzed = false;
		GenerateDS();
		//This probably will change
	}

	void CSMTLDataset::AddTaskFromFile( string fileName, string taskName, int numIn, int numOut )
	{
		TaskPtr tp(new Task());
		tp->name = taskName;
		tp->hasNet = false;
		tp->numOuts = numOut;
		taskList[taskName] = tp;
		newData = true;

		ifstream dataFile(fileName.c_str());
		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		boost::char_separator<char> sep("\t");
		string line;
		if(dataFile.is_open()){
			matDouble tmpInputs;
			while(getline(dataFile,line)){
				tokenizer tok(line, sep);
				vecDouble in(numIn);
				doubles out;
				int newIns = 0;
				int newOuts = 0;
				BOOST_FOREACH(string token, tok){
					if (newIns < numIn){
						in[newIns] = lexical_cast<double>(token);
						newIns++;
					}
					else if(newOuts < numOut){
						out.push_back(lexical_cast<double>(token));
						newOuts++;
					}
				}
				string instring = StringFromVector(in);
				StringSetRet ret = inputStrings.insert(instring);
				dataStore[instring][taskName] = out;

				if (ret.second){
					realInputs.push_back(in);
				}
			}
			dataFile.close();
			numOutputs = numOut;
			numRealInputs = numIn;
		}
		else {
			//Data file didn't load
			assert(0);
		}
	}

	void CSMTLDataset::AddTaskFromNet( NetworkPtr net, string taskName )
	{
		TaskPtr tp(new Task());
		tp->net = net;
		tp->name = taskName;
		tp->numOuts = net->GetNumOut();
		tp->hasNet = true;
		taskList[taskName] = tp;
	}


	JRNN::DatasetPtr CSMTLDataset::SpawnDS()
	{
		DatasetPtr ds(new CSMTLDataset(*this));
		return ds;
	}

	JRNN::vecDouble CSMTLDataset::CreateContextIn( int taskNum )
	{
		vecDouble retVec(view.size());
		FillVec(retVec, 0);
		retVec[taskNum] = 1;
		return retVec;
	}

	vecDouble CSMTLDataset::ConcatVec( vecDouble first, vecDouble second )
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

	void CSMTLDataset::GenerateDS()
	{
		numInputs = numRealInputs + view.size();
		inputs.clear();
		outputs.clear();
		int indexCounter = 0;
		BOOST_FOREACH(vecDouble realIn, realInputs){
			int viewCount = 0;
			BOOST_FOREACH(string task, view){
				vecDouble tmpIn(numInputs);
				vecDouble tmpOut(numOutputs);
				bool found = false;

				tmpIn = ConcatVec(realIn, CreateContextIn(viewCount));
				if (taskList[task]->hasNet){
					tmpOut = taskList[task]->getNetOuts(realIn);
					found = true;
				}
				TaskOuts tmpTasks = dataStore[StringFromVector(realIn)];
				TaskOuts::iterator it = tmpTasks.find(task);
				if (it != tmpTasks.end()){
					found = true;
					tmpOut = VecDoubleFromDoubles(it->second);
				}
				if(found) {
					inputs.push_back(tmpIn);
					outputs.push_back(tmpOut);
					taskList[task]->indexes.push_back(indexCounter);
					indexCounter++;
				}
				viewCount++;
			}
		}
	}

	vecDouble CSMTLDataset::VecDoubleFromDoubles( const doubles& inDoubles )
	{
		vecDouble retVec(inDoubles.size());
		for (unsigned int i = 0; i < inDoubles.size(); i++){
			retVec[i] = inDoubles[i];
		}
		return retVec;
	}

	void CSMTLDataset::DistData( int numTrain, int numVal, int numTest, bool impoverish /*= false*/, int primaryTask /*= 0*/, int numImpTrain /*= 0*/ )
	{
		this->numImpTrain = numImpTrain;
		this->numTrain = numTrain;
		this->numVal = numVal;
		this->numTest = numTest;
		this->primaryTask = primaryTask;
		this->impoverish = impoverish;
		ClearSubsets();
		if (!dsAnalyzed){
			AnalyzeDS();
		}
		Distribute();
	}

	void CSMTLDataset::AnalyzeDS()
	{
		outClassIndexes.clear();
		outClassPercentage.clear();
		outClassNames.clear();
		BOOST_FOREACH(string taskName, view){
			TaskPtr task = taskList[taskName];
			task->outClassIndexes.clear();
			task->outClassPercentages.clear();
			task->outClassNames.clear();
			int taskSize = task->indexes.size();
			for (int i = 0; i < taskSize; i++){
				string outname = StringFromVector(outputs[task->indexes[i]]);
				task->outClassIndexes[outname].push_back(task->indexes[i]);
				outClassIndexes[outname].push_back(task->indexes[i]);
			}
			hashedIntsMap::iterator it = task->outClassIndexes.begin();

			while(it != task->outClassIndexes.end()){
				double tmpPerc = it->second.size() / (double)taskSize;
				task->outClassPercentages[it->first] = tmpPerc;
				task->outClassNames.push_back(it->first);
				it++;
			}
		}
		hashedIntsMap::iterator it = outClassIndexes.begin();
		//calculate the percentage of each outclass to the whole.
		while(it != outClassIndexes.end()){
			double tmpPerc = it->second.size() / (double)size;
			outClassPercentage[it->first] = tmpPerc;
			outClassNames.push_back(it->first);
			//Shuffle(it->second); Removed so that the datasets are always the same for the first load
			it++;
		}
	}

	void CSMTLDataset::FillSubset( matDouble& ins, matDouble& outs, int numExamples, hashedIntsMap& indexQueues, TaskPtr inTask, int numRepeats /*= 0*/ )
	{
		int i = 0, total = 0;
		int numClasses = inTask->outClassNames.size();
		ints usedIndexes;
		while (total < numExamples){
			int classIndex = i++ %numClasses;
			string className = inTask->outClassNames[classIndex];
			double tmpPerc = inTask->outClassPercentages[className];
			int tmpCount = 0;
			tmpCount = (int)floor((numExamples * tmpPerc) + 0.5);
			if (tmpCount > (numExamples - total)){
				tmpCount = numExamples - total;
			}
			while (!indexQueues[className].empty() && tmpCount-- > 0){
				int index = indexQueues[className].back();
				ins.push_back(inputs[index]);
				outs.push_back(outputs[index]);
				total++;
				usedIndexes.push_back(index);
				indexQueues[className].pop_back();
			}
		}
		if (numRepeats > 0){
			for (int i = 0; i < numRepeats; i++){
				BOOST_FOREACH(int index, usedIndexes){
					ins.push_back(inputs[index]);
					outs.push_back(outputs[index]);
				}
			}
		}
	}

	void CSMTLDataset::Distribute()
	{
		int viewSize = view.size();
		int numToDist = viewSize*numTrain + viewSize * numVal + viewSize * numTest;
		assert(size > numToDist && numImpTrain < numTrain && viewSize > 0 && primaryTask <viewSize);
		for(int i = 0; i < viewSize; i++){
			hashedIntsMap indexQueues = taskList[view[i]]->outClassIndexes;
			TaskPtr task = taskList[view[i]];
			if (impoverish && primaryTask == i){
				int numRepeats = numTrain / numImpTrain;
				FillSubset(trainIns, trainOuts, numImpTrain, indexQueues, task, numRepeats);
			}
			else{
				FillSubset(trainIns, trainOuts, numTrain, indexQueues, task);
			}
			if (primaryTask == -1 || primaryTask == i){
				FillSubset(valIns, valOuts, numVal, indexQueues, task);
				FillSubset(testIns, testOuts, numTest, indexQueues, task);
			}
		}
		ShuffleSubsets();
		CalcStdDevs();
	}

	//Only really useful to the revcc that I'm writing right now. Might not make it into final.
	void CSMTLDataset::DistSubview( strings newView )
	{
		ClearSubsets();
		int viewSize = newView.size();
		int numToDist = viewSize*numTrain + viewSize * numVal + viewSize * numTest;
		assert(size > numToDist && numImpTrain < numTrain && viewSize > 0 && primaryTask <viewSize);
		for(int i = 0; i < viewSize; i++){
			hashedIntsMap indexQueues = taskList[newView[i]]->outClassIndexes;
			TaskPtr task = taskList[newView[i]];
			if (impoverish && primaryTask == i){
				int numRepeats = numTrain / numImpTrain;
				FillSubset(trainIns, trainOuts, numImpTrain, indexQueues, task, numRepeats);
			}
			else{
				FillSubset(trainIns, trainOuts, numTrain, indexQueues, task);
			}
			if (primaryTask == -1 || primaryTask == i){
				FillSubset(valIns, valOuts, numVal, indexQueues, task);
				FillSubset(testIns, testOuts, numTest, indexQueues, task);
			}
		}
		ShuffleSubsets();
		CalcStdDevs();
	}


	void CSMTLDataset::Reshuffle()
	{
		BOOST_FOREACH(string className, outClassNames){
			Shuffle(outClassIndexes[className]);
			BOOST_FOREACH(string taskName, view){
				TaskPtr task = taskList[taskName];
				Shuffle(task->outClassIndexes[className]);
			}
		}
	}

	vecDouble CSMTLDataset::Task::getNetOuts( vecDouble inputs )
	{
		vecDouble outputs;
		net->Activate(inputs);
		outputs = net->GetOutputs();
		return outputs;
	}

}