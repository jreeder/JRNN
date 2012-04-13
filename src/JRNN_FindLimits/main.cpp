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

#include "trainers/RevCCTrainer.h"
#include "utility/csmtldataset.h"
#include "utility/mtldataset.h"
#include <boost/pointer_cast.hpp>

using namespace JRNN;
using namespace std;

//string basepath = "C:/Users/John/Documents/Source/JRNN/Experiments/Data/Tabbed Data/New Binary Tasks/";
string basepath = "D:/Users/John Reeder/Code/JRNN/Experiments/Data/Tabbed Data/New Binary Tasks/";
string dsname = "band";
string view = "task1,task2,task3,task5";
int numInputs = 2;
int numOutputs = 1;
int numTasks = 4;
string outFileName = "outfilereverb.txt";
string outFileName2 = "outfilenoreverb.txt";
int numTrain = 100;
int numVal = 100;
int numTest = 200;
int numCandidates = 8;
int numRuns = 2;
bool verbose = true;
bool gCleanReverb = true;

DatasetPtr LoadData( string viewString, string basepath, string dsname, int numInputs, int numOutputs, int primarytask, int impNumTrain, int numTrain, int numVal, int numTest, ints& primaryIndexes, bool useCSMTLDS );


int main(int argc, char** argv) {
	if (verbose)
		cout << "Starting ..." << endl;
	
	DatasetPtr ds = LoadData(view, basepath, dsname, numInputs, numOutputs, 0, 0, numTrain, numVal, numTrain, ints(0), true);
	CSMTLDatasetPtr cds = boost::dynamic_pointer_cast<CSMTLDataset>(ds); 

	if (cds.get() == NULL){
		return EXIT_FAILURE;
	}
	
	int numNetInputs, numNetOutputs;

	numNetOutputs = numOutputs;
	numNetInputs = numInputs + numTasks;

	strings outputstring;

	RevCCTrainer* trainer = new RevCCTrainer(numNetInputs, numNetOutputs, numCandidates);

	trainer->revparams.numContexts = cds->GetViewSize();
	trainer->revparams.cleanReverb = gCleanReverb;

	strings subview;
	subview.push_back("task1");
	cds->DistSubview(subview);
	CSMTLDatasetPtr firstDS = cds->SpawnDS();
	subview.clear();
	subview.push_back("task5");
	cds->DistSubview(subview);
	CSMTLDatasetPtr secondDS = cds->SpawnDS();
	ofstream ofile;
	
	ofile.open(outFileName);
	stringstream output(stringstream::out);
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
	//string filename,outfile;
	//int numIn,numOut;

	//if (argc != 5)
	//{	
	//	cout << "Incorrect Arguments" << endl;
	//	cout << "Proper Syntax: JRNN_FindLimits <filename> <outfile> <numIn> <numOut>" << endl;
	//	return -1;
	//} 
	//else
	//{
	//	filename = argv[1];
	//	outfile = argv[2];
	//	numIn = lexical_cast<int>(argv[3]);
	//	numOut = lexical_cast<int>(argv[4]);
	//}
	////TODO: need to finish this experiment. All it needs to do is read in a data file and run multiple
	////sets of experiments with differing numbers of training points and differing number of hidden layers.

	//fstream myfile;
	//myfile.open(outfile.c_str());
	//if (myfile.is_open() == false){
	//	cout << "Output file not open: " << outfile.c_str() << endl;
	//	return -1;
	//}

	//DatasetPtr ds(new Dataset());

	//ds->LoadFromFile(filename,numIn,numOut);
	//int numTest = 200;
	//int numVal = 200;
	//int numRuns = 60;
	//ints numTrains;
	//int temparray[] = {200, 100, 50, 40, 30, 20, 10, 5};
	//BOOST_FOREACH(int x , temparray){
	//	numTrains.push_back(x);
	//}
	//ints numHids;
	//int temparray2[] = {1,2,4,8,16,32};
	//BOOST_FOREACH(int x, temparray2){
	//	numHids.push_back(x);
	//}

	//hashedDoubleMap test1;
	//hashedIntMap test2;

	//cout << test1["hello"] << " " << test2["hello"] << endl;

	//return 0;
	
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