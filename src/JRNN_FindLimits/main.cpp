/*
 *File:		main.cpp
 *Author: 	jreeder
 *
 *Created on January 18, 2011
 * 
 *Experiment to find the limits of a data set in terms of how many nodes and data points are needed. 
 *Actually necessary this can be done from python. Now just a test bed file. 
 */

#include <stdlib.h>
//#include "structure/network.h"
//#include "utility/dataset.h"
//#include "trainers/RPropTrainer.h"
//#include "networks/FFMLPNetwork.h"
#include <ctime>

#include "trainers/RPropTrainer.h"
#include "trainers/RevCCTrainer.h"
#include "networks/RevCCNetwork.h"
#include "networks/CSLLLFFMLPNetwork.h"
#include "networks/FFMLPNetwork.h"
#include "utility/csmtldataset.h"
#include "utility/mtldataset.h"
#include <boost/pointer_cast.hpp>

using namespace JRNN;
using namespace std;

//string basepath = "C:/Users/John/Documents/Source/JRNN/Experiments/Data/Tabbed Data/New Binary Tasks/";
string basepath = "D:/Users/John Reeder/Code/JRNN/Experiments/Data/Tabbed Data/New Binary Tasks/";
string dsname = "band";
string view = "task1,task2,task3,task4";
int numInputs = 2;
int numOutputs = 1;
int numTasks = 4;
string outFileName = "outfilereverb.txt";
string outFileName2 = "outfilenoreverb.txt";
int numTrain = 100;
int numVal = 100;
int numTest = 200;
int numImpTrain = 15;
int numCandidates = 8;
int numHidPerTask = 2;
int numRuns = 2;
ints primaryIndexes = ints(0);
double scale = 0.5;
double offset = 0.25;
bool verbose = true;
bool gCleanReverb = true;

DatasetPtr LoadData( string viewString, string basepath, string dsname, int numInputs, int numOutputs, int primarytask, int impNumTrain, int numTrain, int numVal, int numTest, ints& primaryIndexes, bool useCSMTLDS );

int FFCSLLLExperiment();

int RevCCExperiment();



int main(int argc, char** argv) {
	
	int retValue = EXIT_FAILURE;
	
	//retValue = FFCSLLLExperiment();
	//retValue = RevCCExperiment();

	
	return retValue;
	
}

JRNN::DatasetPtr LoadData( string viewString, string basepath, string dsname, int numInputs, int numOutputs, int primarytask, int impNumTrain, int numTrain, int numVal, int numTest, ints& primaryIndexes, bool useCSMTLDS)
{
	DatasetPtr retDS;

	if (!useCSMTLDS) {
		MTLDatasetPtr mds(new MTLDataset());
		/*for (int i = 0; i < numTasks; i++)
		{
		string tasknum = lexical_cast<string>(i+1);
		string filename = basepath + dsname + "-task" + tasknum + ".txt";
		string taskname = "task-" + tasknum;
		mds->AddTaskFromFile(filename,taskname,numInputs,numOutputs);
		}*/
	
		strings view = splitString(viewString, ",");
	
		BOOST_FOREACH(string taskname, view){
			string filename = basepath + dsname + "-" + taskname + ".txt";
			mds->AddTaskFromFile(filename, taskname, numInputs, numOutputs);
		}
	
	
		mds->SetView(view);
		//mds->DistData(numTrain,numVal,numTest);
	
		primaryIndexes = ints(0);
		if (primarytask > 0){
			string taskname = view[primarytask-1];
			primaryIndexes = mds->GetIndexes(taskname);
		}
		if (impNumTrain > 0 && primarytask > 0) {
			/*int numImpoverished = numTrain - impNumTrain;
			mds->ImpoverishPrimaryTaskTraining(numImpoverished,(primarytask - 1));*/
			mds->DistData(numTrain,numVal,numTest, true, impNumTrain, (primarytask - 1));
		}
		else {
			mds->DistData(numTrain, numVal, numTest);
		}
	
		retDS = mds->SpawnDS();	
	}
	else {
		CSMTLDatasetPtr cds(new CSMTLDataset());
		strings view = splitString(viewString, ",");

		BOOST_FOREACH(string taskname, view){
			string filename = basepath + dsname + "-" + taskname + ".txt";
			cds->AddTaskFromFile(filename, taskname, numInputs, numOutputs);
		}

		cds->SetView(view);

		if (impNumTrain > 0 && primarytask > 0){
			cds->DistData(numTrain, numVal, numTest, true, impNumTrain, (primarytask - 1));
		}
		else {
			cds->DistData(numTrain, numVal, numTest, false, 0, (primarytask - 1));
		}

		retDS = cds->SpawnDS();
	}
	
	return retDS;
}

int FFCSLLLExperiment()
{
	if (verbose)
		cout << "Starting ..." << endl;

	DatasetPtr ds = LoadData(view, basepath, dsname, numInputs, numOutputs, 0, 0, numTrain, numVal, numTrain, primaryIndexes, true);
	CSMTLDatasetPtr cds = boost::dynamic_pointer_cast<CSMTLDataset>(ds); 

	if (cds.get() == NULL){
		return EXIT_FAILURE;
	}

	int numNetInputs, numNetOutputs;

	numNetOutputs = numOutputs;
	numNetInputs = numInputs + numTasks;

	double rPropEtaPlus = 1.2;
	double rPropEtaMinus = 0.5;
	int rPropMaxEpochs = 3000;
	double rPropMinError = 0.04;

	strings outputstring;

	FFMLPNetPtr ffnet = FFMLPNetwork::Create();
	int numHid = numHidPerTask * numTasks;
	ffnet->SetScaleAndOffset(scale, offset);
	ffnet->Build(numNetInputs, numHid, numNetOutputs);

	strings subview;
	subview.push_back("task2");
	subview.push_back("task3");
	subview.push_back("task4");
	cds->DistSubview(subview);
	CSMTLDatasetPtr firstDS = cds->SpawnDS();
	subview.clear();
	subview.push_back("task1");
	cds->DistSubview(subview, numImpTrain, numVal, numTest);
	CSMTLDatasetPtr secondDS = cds->SpawnDS();
	ofstream ofile;

	RPropTrainer ffbp = RPropTrainer(ffnet, firstDS, rPropEtaPlus, rPropEtaMinus, primaryIndexes);

	ffbp.maxWeight = 1000;
	ffbp.useMaxWeight = true;

	ffbp.TrainToValConv(rPropMaxEpochs);

	CSLLFFMLPNetPtr csnet = CSLLFFMLPNetwork::Create();
	csnet->SetScaleAndOffset(scale, offset);
	csnet->Build(numNetInputs, numHidPerTask, numNetOutputs, ffnet);

	RPropTrainer csbp = RPropTrainer(csnet, secondDS, rPropEtaPlus, rPropEtaMinus, primaryIndexes);

	csbp.maxWeight = 1000;
	csbp.useMaxWeight = true;


	csbp.TrainToValConv(rPropMaxEpochs);

	ofile.open(outFileName);
	stringstream output(stringstream::out);

	ofile.close();

	if(verbose)
		cout << "The End" << endl;

	return EXIT_SUCCESS;
}

int RevCCExperiment()
{
	if (verbose)
		cout << "Starting ..." << endl;

	DatasetPtr ds = LoadData(view, basepath, dsname, numInputs, numOutputs, 0, 0, numTrain, numVal, numTrain, primaryIndexes, true);
	CSMTLDatasetPtr cds = boost::dynamic_pointer_cast<CSMTLDataset>(ds); 

	if (cds.get() == NULL){
		return EXIT_FAILURE;
	}

	int numNetInputs, numNetOutputs;

	numNetOutputs = numOutputs;
	numNetInputs = numInputs + numTasks;

	strings subview;
	subview.push_back("task2");
	subview.push_back("task3");
	subview.push_back("task4");
	cds->DistSubview(subview);
	CSMTLDatasetPtr firstDS = cds->SpawnDS();
	subview.clear();
	subview.push_back("task1");
	cds->DistSubview(subview, numImpTrain, numVal, numTest);
	CSMTLDatasetPtr secondDS = cds->SpawnDS();
	ofstream ofile;

	RevCCTrainer* trainer = new RevCCTrainer(numNetInputs, numNetOutputs, numCandidates);

	trainer->revparams.numContexts = cds->GetViewSize();
	trainer->revparams.cleanReverb = gCleanReverb;

	for (int i = 0; i < numRuns; i++)
	{
		if (verbose)
			cout << "Re-verb Run " << i+1 << endl;
		//Train First Task
		trainer->TrainTask(firstDS,3000,true);
		//Train Second Task
		trainer->TrainTask(secondDS,3000,true,true,firstDS,Dataset::TEST);

		//Second Task test results
		hashedDoubleMap secondResults = trainer->TestWiClass(secondDS, Dataset::TEST);
		int hiddenLayers = trainer->net1vals.numHidLayers;
		int epochs = trainer->net1vals.epochs;
		int numResets = trainer->net1vals.numResets;
		int time = 0;


		output << epochs << "\t";
		output << time << "\t";
		output << hiddenLayers << "\t";
		output << numResets << "\t";

		printHashedDoubleMap(secondResults,output);
		output << "|\t";
		printDoubles(trainer->net1vals.MSERec,output);
		output << "|\t";

		//Previous Task test results. 
		hashedDoubleMap firstTaskResults = trainer->TestWiClass(firstDS, Dataset::TEST);
		printHashedDoubleMap(firstTaskResults, output);
		output << "|\t";
		RevCCTrainer::TestResults results = trainer->getTestWhileTrainResults();
		BOOST_FOREACH(RevCCTrainer::TestResult result, results){
			output << "*" << result.epoch << "!";
			printHashedDoubleMap(result.result,output);
		}
		output << endl;
		ofile << output.str();
		output.str("");
		output.clear();

		trainer->Reset();
		firstDS->RedistData();
		secondDS->RedistData();
	}
	ofile.close();

	ofile.open(outFileName2);
	for (int i = 0; i < numRuns; i++){

		if (verbose)
			cout << "No Re-verb Run " << i+1 << endl;

		//Set the reverb down to 1 ... this is straight pseudo rehearsal
		trainer->revparams.numRev = 1;
		//Train First Task
		trainer->TrainTask(firstDS,3000,true);
		//Train Second Task
		trainer->TrainTask(secondDS,3000,true,true,firstDS,Dataset::TEST);

		//Second Task test results
		hashedDoubleMap secondResults = trainer->TestWiClass(secondDS, Dataset::TEST);
		int hiddenLayers = trainer->net1vals.numHidLayers;
		int epochs = trainer->net1vals.epochs;
		int numResets = trainer->net1vals.numResets;
		int time = 0;


		output << epochs << "\t";
		output << time << "\t";
		output << hiddenLayers << "\t";
		output << numResets << "\t";

		printHashedDoubleMap(secondResults,output);
		output << "|\t";
		printDoubles(trainer->net1vals.MSERec,output);
		output << "|\t";

		//Previous Task test results. 
		hashedDoubleMap firstTaskResults = trainer->TestWiClass(firstDS, Dataset::TEST);
		printHashedDoubleMap(firstTaskResults, output);
		output << "|\t";
		RevCCTrainer::TestResults results = trainer->getTestWhileTrainResults();
		BOOST_FOREACH(RevCCTrainer::TestResult result, results){
			output << "*" << result.epoch << "!";
			printHashedDoubleMap(result.result,output);
		}

		output << endl;
		ofile << output.str();
		output.str(""); //needed to set the internal string back to empty
		output.clear(); //Clears any error flags if any ...probably doesn't do anything for me

		trainer->Reset();
		firstDS->RedistData();
		secondDS->RedistData();
	}

	ofile.close();

	if(verbose)
		cout << "The End" << endl;

	return EXIT_SUCCESS;
}
