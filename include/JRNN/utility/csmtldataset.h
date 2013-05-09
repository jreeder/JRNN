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
	class CSMTLDataset;
	typedef boost::shared_ptr<CSMTLDataset> CSMTLDatasetPtr;

	class DataSetArchiver;

	class CSMTLDataset : public Dataset {

	public:

		friend class DataSetArchiver;

		CSMTLDataset();
		CSMTLDataset(const CSMTLDataset& orig);
		void SetView(strings view);
		void AddTaskFromFile(string fileName, string taskName, int numIn, int numOut);
		void AddTaskFromNet(NetworkPtr net, string taskName);
		void AddMatDoublesToTask(matDouble& inMat, matDouble& outMat, string taskName);
		void AddVecDoublesToTask(vecDouble inVec, vecDouble outVec, string taskName);
		CSMTLDatasetPtr SpawnDS();
		virtual void DistData(int numTrain, int numVal, int numTest, bool impoverish = false, int numImpTrain = 0, int primaryTask = -1);
		void DistSubview(strings view);
		void DistSubview(strings newView, int numTrain, int numVal, int numTest);
		void ResetView();
		void SetConceptData(bool isConceptData);
		bool GetConceptData();
		vecDouble GetRandContext();
		int GetViewSize();
		strings GetView();
		strings GetTaskNames();

		class Task {
		public:
			friend class CSMTLDataset;
			friend class DataSetArchiver;

		private:
			string name;
			ints indexes;
			int numOuts;
			NetworkPtr net;
			bool hasNet;
			hashedIntsMap outClassIndexes;
			hashedDoubleMap outClassPercentages;
			strings outClassNames;
			Task(){
				name = "";
				numOuts = 0;
				hasNet = false;
			}
			Task(const Task& orig){
				name = orig.name;
				indexes = orig.indexes;
				numOuts = orig.numOuts;
				net = orig.net;
				hasNet = orig.hasNet;
				outClassIndexes = orig.outClassIndexes;
				outClassPercentages = orig.outClassPercentages;
				outClassNames = orig.outClassNames;
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
		bool conceptData;
		bool dsGenerated;
		int primaryTask; 
		int numImpTrain;
		CSMTLDataStore dataStore;
		Tasks taskList;
		strings view;
		strings subView;
		StringSet inputStrings;
		matDouble realInputs;
		int numRealInputs;
		void GenerateDS();
		virtual void Distribute();
		virtual void AnalyzeDS();
		virtual void FillSubset( matDouble& ins, matDouble& outs, int numExamples, hashedIntsMap& indexQueues, TaskPtr inTask, int numRepeats = 0);
		virtual void Reshuffle();
		vecDouble CreateContextIn(int taskNum);
		//vecDouble ConcatVec(vecDouble first, vecDouble second);
		vecDouble VecDoubleFromDoubles( const doubles& inDoubles );
		template<class outType, class inType> 
		outType ConvertVector(const inType& inVector);
		void ClearTaskValues();
		Tasks CloneTaskList( Tasks taskList );
	};
}

#endif