// JRNN_test2.cpp : Defines the entry point for the console application.
//

//#include "JRNN.h"
#include "utility/mtldataset.h"
#include "trainers/RPropTrainer.h"
#include "trainers/CCTrainer.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <boost/thread.hpp>

using namespace JRNN;
using namespace std;

void printDoubles(doubles toPrint, iostream& stream);
void CCWorker(CCTrainer& trainer, strings& results, int numRuns, bool useValidation = true);
void BPWorker(RPropTrainer& trainer, int numHid, strings& results, int numRuns, bool useValidation = true);


int main(int argc, char* argv[])
{
	MTLDataset* mds = new MTLDataset();
	mds->AddTaskFromFile("band-task1.txt", "task-1", 2, 1);
	mds->AddTaskFromFile("band-task2.txt", "task-2", 2, 1);
	mds->AddTaskFromFile("band-task3.txt", "task-3", 2, 1);
	mds->AddTaskFromFile("band-task4.txt", "task-4", 2, 1);
	mds->AddTaskFromFile("band-task5.txt", "task-5", 2, 1);
	mds->AddTaskFromFile("band-task6.txt", "task-6", 2, 1);
	mds->AddTaskFromFile("band-task7.txt", "task-7", 2, 1);
	mds->AddTaskFromFile("band-task8.txt", "task-8", 2, 1);

	strings view;
	view.push_back("task-1");
	view.push_back("task-2");
	view.push_back("task-3");
	view.push_back("task-4");

	mds->SetView(view);
	mds->DistData(50, 100, 200);
	mds->ImpoverishPrimaryTaskTraining(0.7);
	ints mtlPrimaryIndexes = mds->GetIndexes("task-1");
	DatasetPtr mtl_ds = mds->SpawnDS();
	view.clear();
	view.push_back("task-1");
	mds->SetView(view);
	mds->DistData(10, 100, 200);
	DatasetPtr stl_ds = mds->SpawnDS();
	delete mds;
	mds = NULL;

	FFMLPNetPtr ffnet_stl = FFMLPNetwork::Create();
	ffnet_stl->Build(2, 4, 1);
	FFMLPNetPtr ffnet_mtl = FFMLPNetwork::Create();
	ffnet_mtl->Build(2, 16, 4);

	CCNetworkPtr ccnet_stl = CCNetwork::Create();
	CCNetworkPtr ccnet_mtl = CCNetwork::Create();
	ccnet_stl->Build(2,1);
	ccnet_mtl->Build(2,4);
	RPropTrainer bp_stl(ffnet_stl,stl_ds,1.2,0.5);
	RPropTrainer bp_mtl(ffnet_mtl,mtl_ds, 1.2, 0.5, mtlPrimaryIndexes);
	CCTrainer cc_stl(ccnet_stl, stl_ds, 8);
	CCTrainer cc_mtl(ccnet_mtl, mtl_ds, 8, mtlPrimaryIndexes);
	
	strings bpstlresults;
	strings bpmtlresults;
	strings ccstlresults;
	strings ccmtlresults;

	boost::thread bpstlthread(BPWorker, bp_stl, 4, bpstlresults, 2, true);
	boost::thread bpmtlthread(BPWorker, bp_mtl, 16, bpmtlresults, 2, true);
	boost::thread ccstlthread(CCWorker, cc_stl, ccstlresults, 2, true);
	boost::thread ccmtlthread(CCWorker, cc_mtl, ccmtlresults, 2, true);

	bpstlthread.join();
	bpmtlthread.join();
	ccstlthread.join();
	ccmtlthread.join();

	return 0;
}

void BPWorker(RPropTrainer& trainer, int numHid, strings& results, int numRuns, bool useValidation){
	stringstream output(stringstream::out);
	for (int i = 0; i < numRuns; i++){
		double time = 0.0;
		clock_t startTime = clock();
		if (useValidation){
			trainer.TrainToValConv(3000);
		}
		else {
			trainer.TrainToConvergence(0.04, 3000);
		}
		time = (clock() - startTime)/(double)CLOCKS_PER_SEC;
		int epochs = trainer.GetEpochs();
		output << epochs << "\t";
		output << time << "\t";
		//cout << epochs << "\t";
		output << numHid << "\t";
		//cout << numHid << "\t";
		hashedDoubleMap testresults = trainer.TestWiClass(Dataset::TEST);
		std::pair<string,double> p;
		BOOST_FOREACH(p, testresults){
			output << p.first << ":" << p.second << "\t";
			//cout << p.first << ":" << p.second << "\t";
		}
		output << "|\t";
		printDoubles(trainer.GetMSERec(),output);
		output << endl;
		//cout << endl;
		results.push_back(output.str());
		output.str("");
		output.clear();
		trainer.Reset();
	}
}

void CCWorker(CCTrainer& trainer, strings& results, int numRuns, bool useValidation){
	stringstream output(stringstream::out);
	for (int i = 0; i < numRuns; i++)
	{
		double time = 0.0;
		clock_t startTime = clock();
		if(useValidation){
			trainer.TrainToValConv(3000);
		}
		else {
			trainer.TrainToConvergence(3000);
		}
		time = (clock() - startTime)/(double)CLOCKS_PER_SEC;

		int epochs = trainer.GetEpochs();
		int hiddenLayers = trainer.GetNumHidLayers();
		output << epochs << "\t";
		output << time << "\t";
		output << hiddenLayers << "\t";
#ifdef _DEBUG
		cout << epochs << "\t";
		cout << hiddenLayers << "\t";
		cout << time << "\t";
#endif // _DEBUG


		hashedDoubleMap testresults = trainer.TestWiClass(Dataset::TEST);
		std::pair<string,double> p;
		BOOST_FOREACH(p, testresults){
			output << p.first << ":" << p.second << "\t";
#ifdef _DEBUG
			cout << p.first << ":" << p.second << "\t";
#endif // _DEBUG
		}
		output << "|\t";
		printDoubles(trainer.GetMSERec(),output);
		output << endl;
#ifdef _DEBUG
		cout << endl;
#endif // _DEBUG
		results.push_back(output.str());
		output.str("");
		output.clear();
		trainer.Reset();
	}
}

void printDoubles(doubles toPrint, iostream& stream){
	BOOST_FOREACH(double db, toPrint){
		stream << db << "\t";
	}
}