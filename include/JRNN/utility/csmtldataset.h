/********************************************************************
	created:	2011/05/03
	created:	3:5:2011   18:43
	filename:	csmtldataset.h
	author:		jreeder
	
	purpose:	csmtl dataset class. Used to train csmtl networks
*********************************************************************/

#pragma once
#ifndef _CSMTLDATASET_H
#define _CSMTLDATASET_H

#include "JRNN.h"
#include "utility/dataset.h"
#include "structure/network.h"

namespace JRNN {

	class CSMTLDataset : public Dataset {

	public:
		CSMTLDataset();
		CSMTLDataset(const CSMTLDataset& orig);
		void SetView(strings view);
		void AddTaskFromFile(string fileName, string taskName, int numIn, int numOut);
		void AddTaskFromNet(NetworkPtr net, string taskName);
		DatasetPtr SpawnDS();
		virtual void DistData(int numTrain, int numVal, int numTest, bool impoverish = false, int primaryTask = 0, int numImpoverish = 0);
		

		class Task {
		public:
			friend class CSMTLDataset;
		private:
			string name;
			ints indexes;
			int numOuts;
			NetworkPtr net;
			bool hasNet;
			Task(){
				name = "";
				numOuts = 0;
				hasNet = false;
			}
			vecDouble getNetOuts(vecDouble inputs);
		};
		typedef boost::unordered_map<string, doubles> TaskOuts;
		typedef boost::unordered_map<string, TaskOuts> CSMTLDataStore;
		typedef boost::shared_ptr<Task> TaskPtr;
		typedef boost::unordered_map<string, TaskPtr> Tasks;
		typedef std::pair<string, TaskPtr> TaskPair;

	private:
		bool newData;
		bool impoverish; 
		int primaryTask; 
		int numImpoverish;
		CSMTLDataStore dataStore;
		Tasks taskList;
		strings view;
		StringSet inputStrings;
		matDouble realInputs;
		int numRealInputs;
		void GenerateDS();
		virtual void Distribute();
		vecDouble CreateContextIn(int taskNum);
		vecDouble ConcatVec(vecDouble first, vecDouble second);
		vecDouble VecDoubleFromDoubles( const doubles& inDoubles );
	};
}

#endif