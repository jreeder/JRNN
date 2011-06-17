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
	}

	CSMTLDataset::CSMTLDataset(const CSMTLDataset& orig ) : Dataset(orig)
	{
		this->dataStore = orig.dataStore;
		this->impoverish = orig.impoverish;
		this->inputStrings = orig.inputStrings;
		this->newData = orig.newData;
		this->numImpoverish = orig.numImpoverish;
		this->primaryTask = orig.primaryTask;
		this->realInputs = orig.realInputs;
		this->taskList = orig.taskList;
		this->view = orig.view;
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
		for (int i = 0; i < inDoubles.size(); i++){
			retVec[i] = inDoubles[i];
		}
		return retVec;
	}

	vecDouble CSMTLDataset::Task::getNetOuts( vecDouble inputs )
	{
		vecDouble outputs;
		net->Activate(inputs);
		outputs = net->GetOutputs();
		return outputs;
	}

}