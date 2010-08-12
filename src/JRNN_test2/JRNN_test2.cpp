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
#include <cmath>

using namespace JRNN;
using namespace std;

void printDoubles(doubles toPrint, iostream& stream);
void CCWorker(CCTrainer& trainer, strings* results, int numRuns, bool useValidation = true);
void BPWorker(RPropTrainer& trainer, int numHid, strings* results, int numRuns, bool useValidation = true);
boost::mutex io_mutex;

int main(int argc, char* argv[])
{
	string basepath = "";
	int numRuns = 5;
	double impPerc = 0.8;
	int numTrain = 50;
	int numVal = 100;
	int numTest = 500;
	int numHidPerOut = 4;
	if (argc > 1){
		basepath = argv[1];
		if (argc == 8){
			numTrain = lexical_cast<int>(argv[2]);
			numVal = lexical_cast<int>(argv[3]);
			numTest = lexical_cast<int>(argv[4]);
			numHidPerOut = lexical_cast<int>(argv[5]);
			numRuns = lexical_cast<int>(argv[6]);
			impPerc = lexical_cast<double>(argv[7]);
		}
		else if (argc > 2){
			cout << "Incorrect Number of arguments ... Please give basepath and 7 experiment values" << endl;
			return -1;
		}
	}
	else {
		cout << "Incorrect usage ... Please give basepath." << endl;
		return -1;
	}
	
	MTLDataset* mds = new MTLDataset();
	mds->AddTaskFromFile(basepath + "band-task1.txt", "task-1", 2, 1);
	mds->AddTaskFromFile(basepath + "band-task2.txt", "task-2", 2, 1);
	mds->AddTaskFromFile(basepath + "band-task3.txt", "task-3", 2, 1);
	mds->AddTaskFromFile(basepath + "band-task4.txt", "task-4", 2, 1);
	mds->AddTaskFromFile(basepath + "band-task5.txt", "task-5", 2, 1);
	mds->AddTaskFromFile(basepath + "band-task6.txt", "task-6", 2, 1);
	mds->AddTaskFromFile(basepath + "band-task7.txt", "task-7", 2, 1);
	mds->AddTaskFromFile(basepath + "band-task8.txt", "task-8", 2, 1);

	strings view;
	view.push_back("task-1");
	view.push_back("task-2");
	view.push_back("task-5");
	view.push_back("task-6");

	mds->SetView(view);
	mds->DistData(numTrain, numVal, numTest);
	mds->ImpoverishPrimaryTaskTraining(impPerc);
	ints mtlPrimaryIndexes = mds->GetIndexes("task-1");
	DatasetPtr mtl_ds = mds->SpawnDS();
	view.clear();
	view.push_back("task-1");
	mds->SetView(view);
	mds->DistData((int)floor(numTrain*(1-impPerc)), numVal, numTest);
	DatasetPtr stl_ds = mds->SpawnDS();
	delete mds;
	mds = NULL;

	FFMLPNetPtr ffnet_stl = FFMLPNetwork::Create();
	ffnet_stl->Build(2, numHidPerOut, 1);
	FFMLPNetPtr ffnet_mtl = FFMLPNetwork::Create();
	ffnet_mtl->Build(2, numHidPerOut * 4, 4);

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

	boost::thread bpstlthread(BPWorker, bp_stl, numHidPerOut, &bpstlresults, numRuns, true);
	boost::thread bpmtlthread(BPWorker, bp_mtl, numHidPerOut * 4, &bpmtlresults, numRuns, true);
	boost::thread ccstlthread(CCWorker, cc_stl, &ccstlresults, numRuns, true);
	boost::thread ccmtlthread(CCWorker, cc_mtl, &ccmtlresults, numRuns, true);

	bpstlthread.join();
	bpmtlthread.join();
	ccstlthread.join();
	ccmtlthread.join();

	ofstream bpstlfile;
	ofstream bpmtlfile;
	ofstream ccstlfile;
	ofstream ccmtlfile;

	bpstlfile.open("bpstlresults.txt");
	bpmtlfile.open("bpmtlresults.txt");
	ccstlfile.open("ccstlresults.txt");
	ccmtlfile.open("ccmtlresutls.txt");

	BOOST_FOREACH(string line, bpstlresults){
		bpstlfile << line << endl;
	}
	BOOST_FOREACH(string line, bpmtlresults){
		bpmtlfile << line << endl;
	}
	BOOST_FOREACH(string line, ccstlresults){
		ccstlfile << line << endl;
	}
	BOOST_FOREACH(string line, ccmtlresults){
		ccmtlfile << line << endl;
	}

	bpstlfile.close();
	bpmtlfile.close();
	ccstlfile.close();
	ccmtlfile.close();
	return 0;
}

void BPWorker(RPropTrainer& trainer, int numHid, strings* results, int numRuns, bool useValidation){
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
		output << 0 << "\t";
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
		results->push_back(output.str());
		output.str("");
		output.clear();
		trainer.Reset();
	}
}

void CCWorker(CCTrainer& trainer, strings* results, int numRuns, bool useValidation){
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
		output << trainer.GetNumResets() << "\t";
#ifdef _DEBUG
		io_mutex.lock();
		cout << epochs << "\t";
		cout << hiddenLayers << "\t";
		cout << time << "\t";
		cout << trainer.GetNumResets() << "\t";
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
		io_mutex.unlock();
#endif // _DEBUG
		results->push_back(output.str());
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