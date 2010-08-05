// JRNN_test2.cpp : Defines the entry point for the console application.
//

//#include "JRNN.h"
#include "utility/mtldataset.h"
#include "trainers/RPropTrainer.h"
#include "trainers/CCTrainer.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace JRNN;
using namespace std;

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
	DatasetPtr ds = mds->SpawnDS();
	view.clear();
	view.push_back("task-1");
	mds->SetView(view);
	mds->DistData(10, 100, 200);
	DatasetPtr ds2 = mds->SpawnDS();
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
	RPropTrainer bp_stl(ffnet_stl,ds2,1.2,0.5);
	RPropTrainer bp_mtl(ffnet_mtl,ds, 1.2, 0.5, mtlPrimaryIndexes);
	CCTrainer cc_stl(ccnet_stl, ds2, 8);
	CCTrainer cc_mtl(ccnet_mtl, ds, 8, mtlPrimaryIndexes);
	return 0;
}

