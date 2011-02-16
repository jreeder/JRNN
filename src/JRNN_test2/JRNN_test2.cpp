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

//void printDoubles(doubles toPrint, iostream& stream);
void CCWorker(CCTrainer& trainer, strings* results, int numRuns, bool useValidation = true);
void BPWorker(RPropTrainer& trainer, int numHid, strings* results, int numRuns, bool useValidation = true);
boost::mutex io_mutex;

int main(int argc, char* argv[])
{
	string basepath = "";
	string dsname = "";
	int numTasks = 0;
	int numRuns = 5;
	double impPerc = 0.8;
	int numTrain = 50;
	int numVal = 100;
	int numTest = 500;
	int numHidPerOut = 4;
	bool useValidation = true;
	if (argc > 1){
		basepath = argv[1];
		dsname = argv[2];
		numTasks = lexical_cast<int>(argv[3]);
		if (argc == 11){
			numTrain = lexical_cast<int>(argv[4]);
			numVal = lexical_cast<int>(argv[5]);
			numTest = lexical_cast<int>(argv[6]);
			numHidPerOut = lexical_cast<int>(argv[7]);
			numRuns = lexical_cast<int>(argv[8]);
			impPerc = lexical_cast<double>(argv[9]);
			useValidation = (string(argv[10]) == "T") ? true : false;
		}
		else if (argc > 4){
			cout << "Incorrect Number of arguments ... Please give basepath, dataset name, number of tasks, and 8 experiment values" << endl;
			return -1;
		}
	}
	else {
		cout << "Incorrect usage ... Please give basepath, dataset name, and number of tasks." << endl;
		return -1;
	}
	
	MTLDataset* mds = new MTLDataset();
	/*mds->AddTaskFromFile(basepath + "band-task1.txt", "task-1", 2, 1);
	mds->AddTaskFromFile(basepath + "band-task2.txt", "task-2", 2, 1);
	mds->AddTaskFromFile(basepath + "band-task3.txt", "task-3", 2, 1);
	mds->AddTaskFromFile(basepath + "band-task4.txt", "task-4", 2, 1);
	mds->AddTaskFromFile(basepath + "band-task5.txt", "task-5", 2, 1);
	mds->AddTaskFromFile(basepath + "band-task6.txt", "task-6", 2, 1);
	mds->AddTaskFromFile(basepath + "band-task7.txt", "task-7", 2, 1);
	mds->AddTaskFromFile(basepath + "band-task8.txt", "task-8", 2, 1);*/

	for (int i = 0; i < numTasks; i++)
	{
		string tasknum = lexical_cast<string>(i+1);
		string filename = basepath + dsname + "-task" + tasknum + ".txt";
		string taskname = "task-" + tasknum;
		mds->AddTaskFromFile(filename,taskname,2,1);
	}

	strings view;
	view.push_back("task-1");
	view.push_back("task-2");
	view.push_back("task-3");
	view.push_back("task-4");

	mds->SetView(view);
	mds->DistData(numTrain, numVal, numTest);
	DatasetPtr full_mtl_ds = mds->SpawnDS();
	mds->ImpoverishPrimaryTaskTraining(impPerc);
	ints mtlPrimaryIndexes = mds->GetIndexes("task-1");
	ints mtlPrimaryIndexes2 = mds->GetIndexes("task-2");
	ints mtlPrimaryIndexes3 = mds->GetIndexes("task-3");
	ints mtlPrimaryIndexes4 = mds->GetIndexes("task-4");
	DatasetPtr mtl_ds = mds->SpawnDS();
	
	
	view.clear();
	view.push_back("task-1");
	view.push_back("task-2");
	view.push_back("task-5");
	view.push_back("task-6");
	mds->SetView(view);
	mds->DistData(numTrain, numVal, numTest);
	mds->ImpoverishPrimaryTaskTraining(impPerc);
	DatasetPtr mtl_ds_ur = mds->SpawnDS();
	
	view.clear();
	view.push_back("task-1");
	mds->SetView(view);
	mds->DistData((int)floor(numTrain*(1-impPerc)), numVal, numTest);
	DatasetPtr stl_ds = mds->SpawnDS();
	mds->DistData(numTrain, numVal, numTest);
	DatasetPtr full_stl_ds = mds->SpawnDS();

	view.clear();
	view.push_back("task-2");
	mds->SetView(view);
	mds->DistData(numTrain,numVal,numTest);
	DatasetPtr stl_ds_t2 = mds->SpawnDS();

	view.clear();
	view.push_back("task-3");
	mds->SetView(view);
	mds->DistData(numTrain,numVal,numTest);
	DatasetPtr stl_ds_t3 = mds->SpawnDS();

	view.clear();
	view.push_back("task-4");
	mds->SetView(view);
	mds->DistData(numTrain,numVal,numTest);
	DatasetPtr stl_ds_t4 = mds->SpawnDS();

	delete mds;
	mds = NULL;

	FFMLPNetPtr ffnet_stl = FFMLPNetwork::Create();
	ffnet_stl->Build(2, numHidPerOut, 1);
	FFMLPNetPtr ffnet_stl2 = FFMLPNetwork::Create();
	ffnet_stl->Build(2, numHidPerOut, 1);
	FFMLPNetPtr ffnet_stl3 = FFMLPNetwork::Create();
	ffnet_stl3->Build(2, numHidPerOut, 1);
	FFMLPNetPtr ffnet_stl4 = FFMLPNetwork::Create();
	ffnet_stl4->Build(2, numHidPerOut, 1);

	FFMLPNetPtr ffnet_mtl = FFMLPNetwork::Create();
	ffnet_mtl->Build(2, numHidPerOut * 4, 4);
	FFMLPNetPtr ffnet_mtl_full1 = FFMLPNetwork::Create();
	ffnet_mtl_full1->Build(2, numHidPerOut * 4, 4);
	FFMLPNetPtr ffnet_mtl_full2 = FFMLPNetwork::Create();
	ffnet_mtl_full2->Build(2, numHidPerOut * 4, 4);
	FFMLPNetPtr ffnet_mtl_full3 = FFMLPNetwork::Create();
	ffnet_mtl_full3->Build(2, numHidPerOut * 4, 4);
	FFMLPNetPtr ffnet_mtl_full4 = FFMLPNetwork::Create();
	ffnet_mtl_full4->Build(2, numHidPerOut * 4, 4);

	FFMLPNetPtr ffnet_mtl_ur = FFMLPNetwork::Create();
	ffnet_mtl_ur->Build(2, numHidPerOut * 4, 4);

	CCNetworkPtr ccnet_stl = CCNetwork::Create();
	CCNetworkPtr ccnet_stl2 = CCNetwork::Create();
	CCNetworkPtr ccnet_stl3 = CCNetwork::Create();
	CCNetworkPtr ccnet_stl4 = CCNetwork::Create();

	CCNetworkPtr ccnet_mtl = CCNetwork::Create();
	CCNetworkPtr ccnet_mtl_full1 = CCNetwork::Create();
	CCNetworkPtr ccnet_mtl_full2 = CCNetwork::Create();
	CCNetworkPtr ccnet_mtl_full3 = CCNetwork::Create();
	CCNetworkPtr ccnet_mtl_full4 = CCNetwork::Create();

	CCNetworkPtr ccnet_mtl_ur = CCNetwork::Create();
	
	
	ccnet_stl->Build(2,1);
	ccnet_stl2->Build(2,1);
	ccnet_stl3->Build(2,1);
	ccnet_stl4->Build(2,1);

	ccnet_mtl->Build(2,4);
	ccnet_mtl_full1->Build(2,4);
	ccnet_mtl_full2->Build(2,4);
	ccnet_mtl_full3->Build(2,4);
	ccnet_mtl_full4->Build(2,4);

	ccnet_mtl_ur->Build(2,4);

	RPropTrainer bp_stl(ffnet_stl,stl_ds,1.2,0.5);
	RPropTrainer bp_stl2(ffnet_stl2,stl_ds_t2,1.2,0.5);
	RPropTrainer bp_stl3(ffnet_stl3,stl_ds_t3,1.2,0.5);
	RPropTrainer bp_stl4(ffnet_stl4,stl_ds_t4,1.2,0.5);

	RPropTrainer bp_mtl(ffnet_mtl,mtl_ds, 1.2, 0.5, mtlPrimaryIndexes);
	RPropTrainer bp_mtl_full1(ffnet_mtl_full1,full_mtl_ds, 1.2, 0.5, mtlPrimaryIndexes);
	RPropTrainer bp_mtl_full2(ffnet_mtl_full2,full_mtl_ds, 1.2, 0.5, mtlPrimaryIndexes2);
	RPropTrainer bp_mtl_full3(ffnet_mtl_full3,full_mtl_ds, 1.2, 0.5, mtlPrimaryIndexes3);
	RPropTrainer bp_mtl_full4(ffnet_mtl_full4,full_mtl_ds, 1.2, 0.5, mtlPrimaryIndexes4);

	RPropTrainer bp_mtl_ur(ffnet_mtl_ur,mtl_ds_ur, 1.2, 0.5, mtlPrimaryIndexes);

	CCTrainer cc_stl(ccnet_stl, stl_ds, 8);
	CCTrainer cc_stl2(ccnet_stl2, stl_ds_t2, 8);
	CCTrainer cc_stl3(ccnet_stl3, stl_ds_t3, 8);
	CCTrainer cc_stl4(ccnet_stl4, stl_ds_t4, 8);

	CCTrainer cc_mtl(ccnet_mtl, mtl_ds, 8, mtlPrimaryIndexes);
	CCTrainer cc_mtl_full1(ccnet_mtl_full1, full_mtl_ds, 8, mtlPrimaryIndexes);
	CCTrainer cc_mtl_full2(ccnet_mtl_full2, full_mtl_ds, 8, mtlPrimaryIndexes2);
	CCTrainer cc_mtl_full3(ccnet_mtl_full3, full_mtl_ds, 8, mtlPrimaryIndexes3);
	CCTrainer cc_mtl_full4(ccnet_mtl_full4, full_mtl_ds, 8, mtlPrimaryIndexes4);

	CCTrainer cc_mtl_ur(ccnet_mtl_ur, mtl_ds_ur, 8, mtlPrimaryIndexes);

	strings bpstlresults;
	strings bpstl2results;
	strings bpstl3results;
	strings bpstl4results;

	strings bpmtlresults;
	strings bpmtlfull1results;
	strings bpmtlfull2results;
	strings bpmtlfull3results;
	strings bpmtlfull4results;

	strings bpmtlurresults;

	strings ccstlresults;
	strings ccstl2results;
	strings ccstl3results;
	strings ccstl4results;

	strings ccmtlresults;
	strings ccmtlfull1results;
	strings ccmtlfull2results;
	strings ccmtlfull3results;
	strings ccmtlfull4results;

	strings ccmtlurresults;

	boost::thread bpstlthread(BPWorker, bp_stl, numHidPerOut, &bpstlresults, numRuns, useValidation);
	boost::thread bpstl2thread(BPWorker, bp_stl2, numHidPerOut, &bpstl2results, numRuns, useValidation);
	boost::thread bpstl3thread(BPWorker, bp_stl3, numHidPerOut, &bpstl3results, numRuns, useValidation);
	boost::thread bpstl4thread(BPWorker, bp_stl4, numHidPerOut, &bpstl4results, numRuns, useValidation);

	boost::thread bpmtlthread(BPWorker, bp_mtl, numHidPerOut * 4, &bpmtlresults, numRuns, useValidation);
	boost::thread bpmtlfull1thread(BPWorker, bp_mtl_full1, numHidPerOut * 4, &bpmtlfull1results, numRuns, useValidation);
	boost::thread bpmtlfull2thread(BPWorker, bp_mtl_full2, numHidPerOut * 4, &bpmtlfull2results, numRuns, useValidation);
	boost::thread bpmtlfull3thread(BPWorker, bp_mtl_full3, numHidPerOut * 4, &bpmtlfull3results, numRuns, useValidation);
	boost::thread bpmtlfull4thread(BPWorker, bp_mtl_full4, numHidPerOut * 4, &bpmtlfull4results, numRuns, useValidation);
	
	boost::thread ccstlthread(CCWorker, cc_stl, &ccstlresults, numRuns, useValidation);
	boost::thread ccstl2thread(CCWorker, cc_stl2, &ccstl2results, numRuns, useValidation);
	boost::thread ccstl3thread(CCWorker, cc_stl3, &ccstl3results, numRuns, useValidation);
	boost::thread ccstl4thread(CCWorker, cc_stl4, &ccstl4results, numRuns, useValidation);

	boost::thread ccmtlthread(CCWorker, cc_mtl, &ccmtlresults, numRuns, useValidation);
	boost::thread ccmtlfull1thread(CCWorker, cc_mtl_full1, &ccmtlfull1results, numRuns, useValidation);
	boost::thread ccmtlfull2thread(CCWorker, cc_mtl_full2, &ccmtlfull2results, numRuns, useValidation);
	boost::thread ccmtlfull3thread(CCWorker, cc_mtl_full3, &ccmtlfull3results, numRuns, useValidation);
	boost::thread ccmtlfull4thread(CCWorker, cc_mtl_full4, &ccmtlfull4results, numRuns, useValidation);

	bpstlthread.join();
	bpstl2thread.join();
	bpstl3thread.join();
	bpstl4thread.join();
	
	bpmtlthread.join();
	bpmtlfull1thread.join();
	bpmtlfull2thread.join();
	bpmtlfull3thread.join();
	bpmtlfull4thread.join();

	ccstlthread.join();
	ccstl2thread.join();
	ccstl3thread.join();
	ccstl4thread.join();

	ccmtlthread.join();
	ccmtlfull1thread.join();
	ccmtlfull2thread.join();
	ccmtlfull3thread.join();
	ccmtlfull4thread.join();

	boost::thread ccmtlurthread(CCWorker, cc_mtl_ur, &ccmtlurresults, numRuns, useValidation);
	boost::thread bpmtlurthread(BPWorker, bp_mtl_ur, numHidPerOut * 4,  &bpmtlurresults, numRuns, useValidation);

	ccmtlurthread.join();
	bpmtlurthread.join();

	ofstream bpstlfile;
	ofstream bpstl2file;
	ofstream bpstl3file;
	ofstream bpstl4file;

	ofstream bpmtlfile;
	ofstream bpmtlfull1file;
	ofstream bpmtlfull2file;
	ofstream bpmtlfull3file;
	ofstream bpmtlfull4file;

	ofstream bpmtlurfile;

	ofstream ccstlfile;
	ofstream ccstl2file;
	ofstream ccstl3file;
	ofstream ccstl4file;

	ofstream ccmtlfile;
	ofstream ccmtlfull1file;
	ofstream ccmtlfull2file;
	ofstream ccmtlfull3file;
	ofstream ccmtlfull4file;

	ofstream ccmtlurfile;

	bpstlfile.open("bpstlresults.txt");
	bpstl2file.open("bpstl2results.txt");
	bpstl3file.open("bpstl3results.txt");
	bpstl4file.open("bpstl4results.txt");

	bpmtlfile.open("bpmtlresults.txt");
	bpmtlfull1file.open("bpmtlfull1results.txt");
	bpmtlfull2file.open("bpmtlfull2results.txt");
	bpmtlfull3file.open("bpmtlfull3results.txt");
	bpmtlfull4file.open("bpmtlfull4results.txt");

	bpmtlurfile.open("bpmtlurresults.txt");

	ccstlfile.open("ccstlresults.txt");
	ccstl2file.open("bpstl2results.txt");
	ccstl3file.open("bpstl3results.txt");
	ccstl4file.open("bpstl4results.txt");

	ccmtlfile.open("ccmtlresutls.txt");
	ccmtlfull1file.open("bpmtlfull1results.txt");
	ccmtlfull2file.open("bpmtlfull2results.txt");
	ccmtlfull3file.open("bpmtlfull3results.txt");
	ccmtlfull4file.open("bpmtlfull4results.txt");

	ccmtlurfile.open("ccmtlurresults.txt");
	
	//bpstl results
	BOOST_FOREACH(string line, bpstlresults){
		bpstlfile << line << endl;
	}
	BOOST_FOREACH(string line, bpstl2results){
		bpstl2file << line << endl;
	}
	BOOST_FOREACH(string line, bpstl3results){
		bpstl3file << line << endl;
	}
	BOOST_FOREACH(string line, bpstl4results){
		bpstl4file << line << endl;
	}

	//bpmtl results
	BOOST_FOREACH(string line, bpmtlresults){
		bpmtlfile << line << endl;
	}
	BOOST_FOREACH(string line, bpmtlfull1results){
		bpmtlfull1file << line << endl;
	}
	BOOST_FOREACH(string line, bpmtlfull2results){
		bpmtlfull2file << line << endl;
	}
	BOOST_FOREACH(string line, bpmtlfull3results){
		bpmtlfull3file << line << endl;
	}
	BOOST_FOREACH(string line, bpmtlfull4results){
		bpmtlfull4file << line << endl;
	}

	//bpmtlur results
	BOOST_FOREACH(string line, bpmtlurresults){
		bpmtlurfile << line << endl;
	}

	//ccstl results
	BOOST_FOREACH(string line, ccstlresults){
		ccstlfile << line << endl;
	}
	BOOST_FOREACH(string line, ccstl2results){
		ccstl2file << line << endl;
	}
	BOOST_FOREACH(string line, ccstl3results){
		ccstl3file << line << endl;
	}
	BOOST_FOREACH(string line, ccstl4results){
		ccstl4file << line << endl;
	}

	//ccmtl results
	BOOST_FOREACH(string line, ccmtlresults){
		ccmtlfile << line << endl;
	}
	BOOST_FOREACH(string line, ccmtlfull1results){
		ccmtlfull1file << line << endl;
	}
	BOOST_FOREACH(string line, ccmtlfull2results){
		ccmtlfull2file << line << endl;
	}
	BOOST_FOREACH(string line, ccmtlfull3results){
		ccmtlfull3file << line << endl;
	}
	BOOST_FOREACH(string line, ccmtlfull4results){
		ccmtlfull4file << line << endl;
	}

	//ccmtlur results
	BOOST_FOREACH(string line, ccmtlurresults){
		ccmtlurfile << line << endl;
	}

	bpstlfile.close();
	bpstl2file.close();
	bpstl3file.close();
	bpstl4file.close();

	bpmtlfile.close();
	bpmtlfull1file.close();
	bpmtlfull2file.close();
	bpmtlfull3file.close();
	bpmtlfull4file.close();

	bpmtlurfile.close();

	ccstlfile.close();
	ccstl2file.close();
	ccstl3file.close();
	ccstl4file.close();

	ccmtlfile.close();
	ccmtlfull1file.close();
	ccmtlfull2file.close();
	ccmtlfull3file.close();
	ccmtlfull4file.close();

	ccmtlurfile.close();
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

//void printDoubles(doubles toPrint, iostream& stream){
//	BOOST_FOREACH(double db, toPrint){
//		stream << db << "\t";
//	}
//}