/********************************************************************
	created:	2011/05/24
	created:	24:5:2011   21:57
	filename:	main.cpp
	author:		jreeder
	
	purpose:	More extensible MTL test platform. Will allow varied
				experiments using command line arguments. To be called
				from python experiment scripts.
*********************************************************************/


#include "utility/mtldataset.h"
#include "utility/csmtldataset.h"
#include "trainers/RPropTrainer.h"
#include "trainers/CCTrainer.h"
#include "trainers/MTLCCTrainer.h"
#include "trainers/EtaMTLRPropTrainer.h"
#include "trainers/RevCCTrainer.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
//#include <boost/thread.hpp>
#include <cmath>
#include <tclap/CmdLine.h>
#include <xmlconfig/xmlconfigurator.h>

using namespace JRNN;
using namespace std;
using namespace TCLAP;
using namespace xmlconfig;

void CCWorker(CCTrainer& trainer, strings* results, int numRuns, bool useValidation = true, int maxEpochs = 3000);
void BPWorker(RPropTrainer& trainer, int numHid, strings* results, int numRuns, bool useValidation = true, int maxEpochs = 3000, double minError = 0.04);
void RevCCWorker(RevCCTrainer& trainer, strings* results, int numRuns, string subView1, string subView2, CSMTLDatasetPtr cds, bool useValidation = true, int maxEpochs = 3000);

DatasetPtr LoadData( string viewString, string basepath, string dsname, int numInputs, int numOutputs, int primarytask, int impNumTrain, int numTrain, int numVal, int numTest, ints& primaryIndexes, bool useCSMTLDS );


int main(int argc, char* argv[]){
	
	//experiment parameters
	string basepath = "";
	string dsname = "";
	int numTasks = 0;
	int numRuns = 5;
	int numInputs = 2;
	int numOutputs = 1;
	int impNumTrain = 10;
	int numTrain = 50;
	int numVal = 100;
	int numTest = 500;
	int numHidPerTask = 4;
	int numNetOutputs = 1;
	int numNetInputs = 2;
	ints primaryIndexes = ints(0);

	//Reverb Parameters
	int numReverbs = 5;
	int revRatio = 1;
	int bufferSize = 200;
	string subView1 = "";
	string subView2 = "";

	bool useValidation = true;
	string viewString = "";
	string outfile;
	string paramspath;
	int primarytask = 0;
	bool useCC = false;
	bool useBP = false;
	bool useCCMTL = false;
	bool useCSMTLDS = false;
	bool useREVERB = false;
	bool useEtaMTL = false;
	bool useCandSlope = false;
	bool useCandScore = false;
	bool useCleanReverb = false;

	DatasetPtr ds;

	//RProp parameters
	double rPropEtaPlus = 1.2;
	double rPropEtaMinus = 0.5;
	int rPropMaxEpochs = 3000;
	double rPropMinError = 0.04;
	//CC Parameters
	int ccMaxEpochs = 3000;
	int ccNumCands = 8;
	
	//EtaMTL Parameter
	double RELMIN;

	//XML Parameter options
	bool xmlLoaded = false;
	bool parmsOptional = true;
	XmlConfigurator params;

	try {
		CmdLine cmd("JRNN_exprun: Atomic JRNN experiment executable", ' ', "0.89");
		ValueArg<string> inBasepath("", "basepath", "The data basepath", true, "", "string", cmd);
		ValueArg<string> inDsname("", "dsname", "The name of the dataset", true, "", "string", cmd);
		ValueArg<int> inNumInputs("", "numinputs", "The number of inputs", true, 2, "int", cmd);
		ValueArg<int> inNumOutputs("", "numoutputs", "The number of outputs", true, 1, "int", cmd);
		ValueArg<int> inNumTasks("", "numtasks", "The number of tasks", true, 1, "int", cmd);
		ValueArg<int> inNumTrain("", "numtrain", "The number of training points", true, 0, "int", cmd);
		ValueArg<int> inNumVal("", "numval", "Number of validation points", true, 0, "int", cmd);
		ValueArg<int> inNumTest("", "numtest", "Number of testing points", true, 0, "int", cmd);
		ValueArg<int> inNumHidPerTask("", "numhid", "Number of hidden nodes per task", true, 0, "int", cmd);
		ValueArg<int> inNumRuns("", "numruns", "Number runs", true, 2, "int", cmd);
		ValueArg<int> inImpNumTrain("", "impnumtrain", "Number of training points in impoverished primary task", false, 0, "int", cmd);
		SwitchArg inUseVal("V", "usevalidation", "Use Validation", cmd, false);
		ValueArg<string> inViewString("", "view", "A comma separated string of task names ex. 'task1,task2,task3'", true, "", "string", cmd);
		ValueArg<string> inOutFilename("", "outfile", "The name of the output file", true, "", "string", cmd);
		ValueArg<string> inParamsPath("", "params", "The path to the parameters xml file", false, "", "string", cmd);
		ValueArg<int> inPrimaryTask("", "primtask", "The primary task, ex: 1", false, 0, "int", cmd);
		ValueArg<double> inEtaRelmin("", "relmin", "The RELMIN parameter", false, 0.05, "double", cmd);
		
		ValueArg<int> inNumReverbs("", "numReverbs", "The number of reverberations", false, 5, "int", cmd);
		ValueArg<int> inRevRatio("", "revRation", "The ratio of reverbated points to normal points", false, 1, "int", cmd);
		ValueArg<int> inBufferSize("", "bufferSize", "The size of the reberberated buffer", false, 200, "int", cmd);
		ValueArg<string> inSubView1("", "subView1", "The first task to learn with reverberation", false, "", "string", cmd);
		ValueArg<string> inSubView2("", "subView2", "The second task to learn with reverberation", false, "", "string", cmd);

		SwitchArg inCC("","CC", "Use Cascade Correlation", false);
		SwitchArg inBP("","BP", "Use Back Propagation", false);
		SwitchArg inCCMTL("", "CCMTL", "Use MTL Cascade Correlation", false);
		SwitchArg inCSMTLDS("", "CSMTLDS", "Use the CSMTL Dataset instead of MTL", cmd, false);
		SwitchArg inREVERB("", "REVERB", "Use Reverberation CCCSMTL", false);
		SwitchArg inEtaMTL("", "ETAMTL", "Use Eta MTL this will require CCMTL instead of CC", cmd, false);
		SwitchArg inUseCandScore("", "CandScore", "Use the Candidate score adjustment in CCMTL", cmd, false);
		SwitchArg inUseCandSlope("", "CandSlope", "Use the Candidate slope adjustment in CCMTL", cmd, false);
		SwitchArg inUseCleanReverb("", "CleanReverb", "Clean the inputs and outputs of the reverberation process", cmd, false);

		vector<Arg*> xorlist;
		xorlist.push_back(&inCCMTL);
		xorlist.push_back(&inCC);
		xorlist.push_back(&inBP);
		xorlist.push_back(&inREVERB);
		cmd.xorAdd(xorlist);

		cmd.parse(argc,argv);

		basepath = inBasepath.getValue();
		dsname = inDsname.getValue();
		numInputs = inNumInputs.getValue();
		numOutputs = inNumOutputs.getValue();
		numTasks = inNumTasks.getValue();
		numTrain = inNumTrain.getValue();
		numVal = inNumVal.getValue();
		numTest = inNumTest.getValue();
		numHidPerTask = inNumHidPerTask.getValue();
		numRuns = inNumRuns.getValue();
		impNumTrain = inImpNumTrain.getValue();
		useValidation = inUseVal.getValue();
		viewString = inViewString.getValue();
		outfile = inOutFilename.getValue();
		paramspath = inParamsPath.getValue();
		primarytask = inPrimaryTask.getValue();
		RELMIN = inEtaRelmin.getValue();
		
		numReverbs = inNumReverbs.getValue();
		revRatio = inRevRatio.getValue();
		bufferSize = inBufferSize.getValue();
		subView1 = inSubView1.getValue();
		subView2 = inSubView2.getValue();

		useCC = inCC.getValue();
		useBP = inBP.getValue();
		useCCMTL = inCCMTL.getValue();
		useCSMTLDS = inCSMTLDS.getValue();
		useREVERB = inREVERB.getValue();
		useEtaMTL = inEtaMTL.getValue();
		useCandScore = inUseCandScore.getValue();
		useCandSlope = inUseCandSlope.getValue();
		useCleanReverb = inUseCleanReverb.getValue();
	}
	catch (TCLAP::ArgException &e) {
		cout << "error: " << e.error() << " for arg " << e.argId() << endl;
	}

	if (paramspath != "" && params.Load(paramspath,"params")){
#ifdef DEBUG
		cout << "Loading parameters from xml" << endl;
#endif
		params.GetVar("rProp.params@etaPlus",rPropEtaPlus, parmsOptional);
		params.GetVar("rProp.params@etaMinus",rPropEtaMinus, parmsOptional);
		params.GetVar("rProp.params@maxEpochs",rPropMaxEpochs, parmsOptional);
		params.GetVar("rProp.params@minError",rPropMinError, parmsOptional);

		params.GetVar("CC.params@maxEpochs", ccMaxEpochs, parmsOptional);
		params.GetVar("CC.params@numCands", ccNumCands, parmsOptional);
		xmlLoaded = true;
	}

	ds = LoadData(viewString, basepath, dsname, numInputs, numOutputs, primarytask, impNumTrain, numTrain, numVal, numTest, primaryIndexes, useCSMTLDS);

	if(!useCSMTLDS && !useREVERB){
		numNetOutputs = numTasks * numOutputs;
		numNetInputs = numInputs;
	}
	else {
		numNetOutputs = numOutputs;
		numNetInputs = numInputs + numTasks;
	}

	strings results;
	
	if (useBP) {
		FFMLPNetPtr ffnet = FFMLPNetwork::Create();
		int numHid = numHidPerTask * numTasks;
		double scale, offset;
		if (xmlLoaded) {
			bool success = true;
			success &= params.GetVar("rProp.params@conScale", scale, parmsOptional);
			success &= params.GetVar("rProp.params@conOffset", offset, parmsOptional);
			if (success) {
				ffnet->SetScaleAndOffset(scale, offset);
			}
		}
		
		ffnet->Build(numNetInputs, numHid, numNetOutputs,useEtaMTL); //when etamtl is true we want to use cloneouts 
		
		RPropTrainer* bp;
		if (useEtaMTL) {
			bp = new EtaMTLRPropTrainer(ffnet, ds, rPropEtaPlus, rPropEtaMinus,primaryIndexes);
			static_cast<EtaMTLRPropTrainer*>(bp)->SetRELMIN(RELMIN);
		}
		else {
			bp = new RPropTrainer(ffnet, ds, rPropEtaPlus, rPropEtaMinus,primaryIndexes);
		}
		//RPropTrainer bp(ffnet, ds, rPropEtaPlus, rPropEtaMinus,primaryIndexes);
		//RPropTrainer bp = (*bptemp);
		if (xmlLoaded){
			params.GetVar("rProp.params@maxWeight", bp->maxWeight, parmsOptional);
			params.GetVar("rProp.params@useMaxWeight", bp->useMaxWeight, parmsOptional);
		}
		BPWorker((*bp), numHid, &results, numRuns, useValidation,rPropMaxEpochs,rPropMinError);
	}
	else if (useCC || useCCMTL ) {
		CCNetworkPtr ccnet = CCNetwork::Create();
		double scale,offset;
		if (xmlLoaded){
			bool success = true;
			success &= params.GetVar("CC.params@conScale", scale, parmsOptional);
			success &= params.GetVar("CC.params@conOffset", offset, parmsOptional);
			if (success){
				ccnet->SetScaleAndOffset(scale, offset);
			}
		}
		ccnet->Build(numNetInputs, numNetOutputs, useEtaMTL); //when etamtl is true we want to use cloneouts 
		CCTrainer* cc;
		if (useCC){
			cc = new CCTrainer(ccnet,ds,ccNumCands,primaryIndexes);
		}
		else {
			cc = new MTLCCTrainer(ccnet,ds,ccNumCands, primaryIndexes);
			if (useEtaMTL){
				static_cast<MTLCCTrainer*>(cc)->mtlparams.useEtaMTL = true;
				static_cast<MTLCCTrainer*>(cc)->SetRELMIN(RELMIN);
				if (useCandSlope){
					static_cast<MTLCCTrainer*>(cc)->mtlparams.weightCandSlopes = true;
					static_cast<MTLCCTrainer*>(cc)->mtlparams.weightCandScore = false;
				}
			}
		}
		//CCTrainer cc = (*cctemp);
		if (xmlLoaded) {
			params.GetVar("CC.params@valPatience", cc->parms.valPatience, parmsOptional);
			params.GetVar("CC.params@impPatience", cc->parms.impPatience, parmsOptional);
			params.GetVar("CC.params@weightMult", cc->parms.weightMult, parmsOptional);
			params.GetVar("CC.params@maxWeight", cc->parms.maxWeight, parmsOptional);
			params.GetVar("CC.params@useMaxWeight", cc->parms.useMaxWeight, parmsOptional);
			params.GetVar("CC.params@primeOffset", cc->parms.primeOffset, parmsOptional);
			params.GetVar("CC.params@indexThreshold", cc->parms.indexThreshold, parmsOptional);
			params.GetVar("CC.params@scoreThreshold", cc->parms.scoreThreshold, parmsOptional);
			params.GetVar("CC.params@errorMeasure", cc->parms.errorMeasure, parmsOptional);
			params.GetVar("CC.params.out@epochs", cc->parms.out.epochs, parmsOptional);
			params.GetVar("CC.params.out@patience", cc->parms.out.patience, parmsOptional);
			params.GetVar("CC.params.out@epsilon", cc->parms.out.epsilon, parmsOptional);
			params.GetVar("CC.params.out@decay", cc->parms.out.decay, parmsOptional);
			params.GetVar("CC.params.out@mu", cc->parms.out.mu, parmsOptional);
			params.GetVar("CC.params.out@changeThreshold", cc->parms.out.changeThreshold , parmsOptional);
			params.GetVar("CC.params.cand@epochs", cc->parms.cand.epochs , parmsOptional);
			params.GetVar("CC.params.cand@patience", cc->parms.cand.patience, parmsOptional);
			params.GetVar("CC.params.cand@epsilon", cc->parms.cand.epsilon , parmsOptional);
			params.GetVar("CC.params.cand@decay", cc->parms.cand.decay, parmsOptional);
			params.GetVar("CC.params.cand@mu", cc->parms.cand.mu, parmsOptional);
			params.GetVar("CC.params.cand@changeThreshold", cc->parms.cand.changeThreshold, parmsOptional);
		}
		CCWorker((*cc), &results, numRuns, useValidation, ccMaxEpochs);
	} else if (useREVERB) {
		RevCCTrainer* rcc = new RevCCTrainer(numNetInputs,numNetOutputs,ccNumCands);
		double scale,offset;
		if (xmlLoaded){
			bool success = true;
			success &= params.GetVar("CC.params@conScale", scale, parmsOptional);
			success &= params.GetVar("CC.params@conOffset", offset, parmsOptional);
			if (success){
				rcc->SetScaleAndOffset(scale, offset);
			}
			params.GetVar("CC.params@valPatience", rcc->parms.valPatience, parmsOptional);
			params.GetVar("CC.params@impPatience", rcc->parms.impPatience, parmsOptional);
			params.GetVar("CC.params@weightMult", rcc->parms.weightMult, parmsOptional);
			params.GetVar("CC.params@maxWeight", rcc->parms.maxWeight, parmsOptional);
			params.GetVar("CC.params@useMaxWeight", rcc->parms.useMaxWeight, parmsOptional);
			params.GetVar("CC.params@primeOffset", rcc->parms.primeOffset, parmsOptional);
			params.GetVar("CC.params@indexThreshold", rcc->parms.indexThreshold, parmsOptional);
			params.GetVar("CC.params@scoreThreshold", rcc->parms.scoreThreshold, parmsOptional);
			params.GetVar("CC.params@errorMeasure", rcc->parms.errorMeasure, parmsOptional);
			params.GetVar("CC.params.out@epochs", rcc->parms.out.epochs, parmsOptional);
			params.GetVar("CC.params.out@patience", rcc->parms.out.patience, parmsOptional);
			params.GetVar("CC.params.out@epsilon", rcc->parms.out.epsilon, parmsOptional);
			params.GetVar("CC.params.out@decay", rcc->parms.out.decay, parmsOptional);
			params.GetVar("CC.params.out@mu", rcc->parms.out.mu, parmsOptional);
			params.GetVar("CC.params.out@changeThreshold", rcc->parms.out.changeThreshold , parmsOptional);
			params.GetVar("CC.params.cand@epochs", rcc->parms.cand.epochs , parmsOptional);
			params.GetVar("CC.params.cand@patience", rcc->parms.cand.patience, parmsOptional);
			params.GetVar("CC.params.cand@epsilon", rcc->parms.cand.epsilon , parmsOptional);
			params.GetVar("CC.params.cand@decay", rcc->parms.cand.decay, parmsOptional);
			params.GetVar("CC.params.cand@mu", rcc->parms.cand.mu, parmsOptional);
			params.GetVar("CC.params.cand@changeThreshold", rcc->parms.cand.changeThreshold, parmsOptional);
		}
		CSMTLDatasetPtr cds = dynamic_pointer_cast<CSMTLDataset>(ds);
		rcc->revparams.cleanReverb = useREVERB;
		rcc->revparams.bufferSize = bufferSize;
		rcc->revparams.numRev = numReverbs;
		rcc->revparams.numRevTrainRatio = revRatio;
		rcc->revparams.numContexts = cds->GetViewSize();
		RevCCWorker((*rcc), &results, numRuns, subView1, subView2, cds, useValidation, ccMaxEpochs);
	}

	ofstream ofile;
	ofile.open(outfile);
	BOOST_FOREACH(string line, results){
		ofile << line << endl;
	}

	ofile.close();
	return 0;
}

void BPWorker(RPropTrainer& trainer, int numHid, strings* results, int numRuns, bool useValidation, int maxEpochs, double minError){
	stringstream output(stringstream::out);
	for (int i = 0; i < numRuns; i++){
		double time = 0.0;
		clock_t startTime = clock();
		if (useValidation){
			trainer.TrainToValConv(maxEpochs);
		}
		else {
			trainer.TrainToConvergence(minError, maxEpochs);
		}
		time = (clock() - startTime)/(double)CLOCKS_PER_SEC;
		int epochs = trainer.GetEpochs();
		output << epochs << "\t";
		output << time << "\t";
		//cout << epochs << "\t";
		output << numHid << "\t";
		//cout << numHid << "\t";
		output << trainer.GetNumResets() << "\t";
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
		trainer.RedistData();
	}
}

void CCWorker(CCTrainer& trainer, strings* results, int numRuns, bool useValidation, int maxEpochs){
	stringstream output(stringstream::out);
	for (int i = 0; i < numRuns; i++)
	{
		double time = 0.0;
		clock_t startTime = clock();
		if(useValidation){
			trainer.TrainToValConv(maxEpochs);
		}
		else {
			trainer.TrainToConvergence(maxEpochs);
		}
		time = (clock() - startTime)/(double)CLOCKS_PER_SEC;

		int epochs = trainer.GetEpochs();
		int hiddenLayers = trainer.GetNumHidLayers();
		output << epochs << "\t";
		output << time << "\t";
		output << hiddenLayers << "\t";
		output << trainer.GetNumResets() << "\t";
#ifdef _DEBUG
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
#endif // _DEBUG
		results->push_back(output.str());
		output.str("");
		output.clear();
		trainer.Reset();
		trainer.RedistData();
	}
}

void RevCCWorker( RevCCTrainer& trainer, strings* results, int numRuns, string subView1, string subView2, CSMTLDatasetPtr cds, bool useValidation /*= true*/, int maxEpochs /*= 3000*/ )
{
	stringstream output(stringstream::out);
	strings subview;
	subview.push_back(subView1);
	cds->DistSubview(subview);
	CSMTLDatasetPtr firstDS = cds->SpawnDS();
	subview.clear();
	subview.push_back(subView2);
	cds->DistSubview(subview);
	CSMTLDatasetPtr secondDS = cds->SpawnDS();

	for (int i = 0; i < numRuns; i++)
	{
		//Train First Task
		trainer.TrainTask(firstDS,3000,true);
		//Train Second Task
		trainer.TrainTask(secondDS,3000,true,true,firstDS,Dataset::TEST);

		//Second Task test results
		hashedDoubleMap secondResults = trainer.TestWiClass(secondDS, Dataset::TEST);
		int hiddenLayers = trainer.net1vals.numHidLayers;
		int epochs = trainer.net1vals.epochs;
		int numResets = trainer.net1vals.numResets;
		int time = 0;


		output << epochs << "\t";
		output << time << "\t";
		output << hiddenLayers << "\t";
		output << numResets << "\t";

		printHashedDoubleMap(secondResults,output);
		output << "|\t";
		printDoubles(trainer.net1vals.MSERec,output);
		output << "|\t";

		//Previous Task test results. 
		hashedDoubleMap firstTaskResults = trainer.TestWiClass(firstDS, Dataset::TEST);
		printHashedDoubleMap(firstTaskResults, output);
		output << "|\t";
		RevCCTrainer::TestResults ftresults = trainer.getTestWhileTrainResults();
		BOOST_FOREACH(RevCCTrainer::TestResult result, ftresults){
			output << "*" << result.epoch << "!";
			printHashedDoubleMap(result.result,output);
		}
		output << endl;
		results->push_back(output.str());
		output.str("");
		output.clear();

		trainer.Reset();
		firstDS->RedistData();
		secondDS->RedistData();
	}
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