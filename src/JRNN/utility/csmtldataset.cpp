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
		firstLoad = true;
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
				//Don't need this yet. Inputs will change based on tasks. 
				if (ret.second){
					realInputs.push_back(in);
				}
			}
			dataFile.close();
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

	void CSMTLDataset::ImpoverishPrimaryTaskTraining( double percentMissing, unsigned int primaryTask /*= 0*/ )
	{

	}

	JRNN::DatasetPtr CSMTLDataset::SpawnDS()
	{

	}



}