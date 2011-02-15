/* 
 * File:   mtldataset.cpp
 * Author: jreeder
 * 
 * Created on August 3, 2010
 */

#include "JRNN.h"
#include "utility/mtldataset.h"

namespace JRNN {

	//TODO: need to test this dataset to see if it builds correctly. 

	MTLDataset::MTLDataset()
	{
		firstLoad = true;
	}

	void MTLDataset::AddTaskFromNet( NetworkPtr net, string taskName, ints primaryOuts )
	{
		TaskPtr tp(new Task());
		tp->net = net;
		tp->name = taskName;
		tp->primaryOuts = primaryOuts;
		tp->numOuts = primaryOuts.size();
		tp->hasNet = true;
		taskList[taskName] = tp;
	}

	void MTLDataset::SetView( strings view )
	{
		this->view = view;
		this->dsAnalyzed = false;
		GenerateDS();
	}

	strings MTLDataset::GetView()
	{
		return view;
	}

	ints MTLDataset::GetIndexes(string taskName){
		return taskList[taskName]->indexes;
	}

	void MTLDataset::AddTaskFromFile( string fileName, string taskName, int numIn, int numOut )
	{
		TaskPtr tp(new Task());
		tp->name = taskName;
		tp->hasNet = false;
		tp->numOuts = numOut;
		taskList[taskName] = tp;
		ifstream dataFile(fileName.c_str());
		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		boost::char_separator<char> sep("\t");
		string line;
		if(dataFile.is_open()){
			matDouble tmpInputs;
			while(getline(dataFile,line)){
				tokenizer tok(line,sep);
				vecDouble in(numIn);
				doubles out;
				int newIns = 0;
				int newOuts = 0;
				BOOST_FOREACH(string token, tok){
					if (newIns < numIn){
						in[newIns] = lexical_cast<double>(token);
						newIns++;
					}
					else if (newOuts < numOut){
						out.push_back(lexical_cast<double>(token));
						newOuts++;
					}
				}
				string instring = StringFromVector(in);
				if (firstLoad){
					inputStrings.push_back(instring);
				}
				dataStore[instring][taskName] = out;
				tmpInputs.push_back(in);
				//outputs.push_back(out);
			}
			if (firstLoad){
				size = tmpInputs.size();
				inputs = tmpInputs;
			}
			else{
				//this throws if the new dataset size doesn't match the existing size. 
				assert(size == inputs.size());
			}
			
			dataFile.close();
		}
		else {
			//This means the file didn't open, We should stop. 
			assert(0);
		}
		if(firstLoad){
			for (int i = 0; i < size; i ++){
				randomRange.push_back(i);
			}
			firstLoad = false;
		}
	}

	void MTLDataset::GenerateDS()
	{
		matDouble::iterator inIt = inputs.begin();
		int outSize = 0;
		assert(inputs.size() > 0);
		BOOST_FOREACH(string taskName, view){
			int startIndex = outSize;
			outSize += taskList[taskName]->numOuts;
			taskList[taskName]->indexes.clear();
			for(int i = startIndex; i < outSize; i++){
				taskList[taskName]->indexes.push_back(i);
			}
		}
		numOutputs = outSize;
		numInputs = inputs[0].size();
		outputs.clear();
		while (inIt != inputs.end()){
			vecDouble out(numOutputs);
			int outIndex = 0;
			BOOST_FOREACH(string taskName, view){
				int nOut = taskList[taskName]->numOuts;
				assert(outIndex + nOut <= outSize);
				if (taskList[taskName]->hasNet){
					vecDouble tmpOut = taskList[taskName]->getNetOuts((*inIt));
					for (unsigned int i = 0; i < tmpOut.size(); i++){
						out[outIndex] = tmpOut[i];
						outIndex++;
					}
				}
				else {
					doubles tmpOut = dataStore[StringFromVector((*inIt))][taskName];
					for (unsigned int i = 0; i < tmpOut.size(); i++){
						out[outIndex] = tmpOut[i];
						outIndex++;
					}
				}
			}
			outputs.push_back(out);
			inIt++;
		}
	}

	void MTLDataset::ImpoverishPrimaryTaskTraining(double percentMissing, unsigned int primaryTask)
	{
		assert(trainOuts.size() > 0 && view.size() > 0 && primaryTask < view.size());
		string primTask = view[primaryTask];
		ints primIndexes = taskList[primTask]->indexes;
		srand(randSeed);
		BOOST_FOREACH(vecDouble& outVec, trainOuts){
			double randNum = (rand() % 100) / (double)100;
			if (randNum < percentMissing){
				BOOST_FOREACH(int ind, primIndexes){
					outVec[ind] = UNKNOWN;
				}
			}
		}
	}

	DatasetPtr MTLDataset::SpawnDS()
	{
		DatasetPtr ds(new Dataset(*this));
		return ds;
	}

	vecDouble MTLDataset::Task::getNetOuts( vecDouble inputs )
	{
		vecDouble outputs;
		net->Activate(inputs);
		outputs = net->GetOutputs();
		vecDouble retVec(primaryOuts.size());
		if (primaryOuts.size() < outputs.size()){
			for(unsigned int i = 0; i < primaryOuts.size(); i++){
				retVec[i] = outputs[primaryOuts[i]];
			}
		}
		else {
			retVec = outputs;
		}
		return retVec;
	}

}