/* 
 * File:   mtldataset.h
 * Author: jreeder
 * 
 * Created on August 3, 2010
 */

#pragma once
#ifndef _MTLDATASET_H
#define _MTLDATASET_H

#include "JRNN.h"
#include "utility/dataset.h"
#include "structure/network.h"

namespace JRNN {
	
	class MTLDataset : public Dataset {
		
	public:
		MTLDataset();
		void SetView(strings view);
		strings GetView();
		ints GetIndexes(string taskName);
		void AddTaskFromFile(string fileName, string taskName, int numIn, int numOut);
		void AddTaskFromNet(NetworkPtr net, string taskName, ints primaryOuts);
		void ImpoverishPrimaryTaskTraining(double percentMissing, unsigned int primaryTask = 0);
		DatasetPtr SpawnDS();
		
		class Task {
		public:
			friend class MTLDataset;
		private:
			string name;
			ints indexes;
			int numOuts;
			NetworkPtr net;
			ints primaryOuts;
			bool hasNet;
			Task(){
				name = "";
				numOuts = 0;
				hasNet = false;
			}
			vecDouble getNetOuts(vecDouble inputs);
		};
		typedef boost::unordered_map<string, doubles> TaskOuts;
		typedef boost::unordered_map<string, TaskOuts> MTLDataStore;
		typedef boost::shared_ptr<Task> TaskPtr;
		typedef boost::unordered_map<string, TaskPtr> Tasks;
		typedef std::pair<string, TaskPtr> TaskPair;

	private:
		bool firstLoad;
		MTLDataStore dataStore;
		Tasks taskList;
		strings view;
		strings inputStrings;
		void GenerateDS();
	};
 	
}

#endif