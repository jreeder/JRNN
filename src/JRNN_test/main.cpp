/* 
 * File:   main.cpp
 * Author: jreeder
 *
 * Created on May 27, 2010, 6:15 PM
 */

#include <stdlib.h>
#include "structure/network.h"
#include "utility/dataset.h"
#include "trainers/RPropTrainer.h"
#include "networks/FFMLPNetwork.h"
#include "networks/CCNetwork.h"
#include "trainers/CCTrainer.h"
//#include "utility/NetworkBuilder.h" Stupid Idea and not necessary
#include <iostream>
#include <fstream>
#include <ctime>
#include <xmlconfig/xmlconfigurator.h>

using namespace JRNN;
using namespace std;
using namespace xmlconfig;

//void printDoubles(doubles toPrint, ofstream& file);

/*
 * 
 */
int main(int argc, char** argv) {

    string filename;
    string outfile = "";
	string type = "";
	string xmlpath = "";
    int numIn,numHid,numOut, numTrain, numVal, numTest, numRuns;
	bool useValidation = true;
	XmlConfigurator params;

    if (argc != 13){
        cout << "Incorrect Arguments" << endl;
		cout << "Proper Syntax: JRNN_test <filename> <numTrain> <numVal> <numTest> <numIn> <numHid> <numOut> <type = 'BP' or 'CC'> <validate = 'T' or 'F'> <outfilename> <numRuns> <xmlconfigpath>" << endl;
		cout << "The number of training, validation, and testing points must be less than the number of points in the test set." << endl;
        return -1;
    }
    else {
        filename = argv[1];
        numTrain = lexical_cast<int>(argv[2]);
        numVal = lexical_cast<int>(argv[3]);
        numTest = lexical_cast<int>(argv[4]);
		numIn = lexical_cast<int>(argv[5]);
		numHid = lexical_cast<int>(argv[6]);
		numOut = lexical_cast<int>(argv[7]);
		type = string(argv[8]);
		if (type != "CC" && type != "BP"){
			cout << "Type must be 'CC' or 'BP'" << endl;
			return -1;
		}

		useValidation = (string(argv[9]) == "T") ? true : false;

		outfile = string(argv[10]);
		
		numRuns = lexical_cast<int>(argv[11]);

		xmlpath	= string(argv[12]);

        /*outfile = filename;
		outfile.replace(outfile.end()-4,outfile.end()," ");
		outfile += type;
        outfile += " results.txt";*/
    }
    fstream myfile;
    myfile.open(outfile.c_str(),fstream::out);
	if (myfile.is_open() == false){
		cout << "Output file Not open: " << outfile.c_str() << endl;
		return -1;
	}
    DatasetPtr ds(new Dataset());
	//FFMLPNetwork netBuilder(numIn, numHid, numOut);
    ds->LoadFromFile(filename, numIn,numOut);
	if (ds->GetSize(Dataset::ALL) == 0){
		cout << "input file not loaded" << endl;
		return -1;
	}
    ds->DistData(numTrain,numVal,numTest);
    //NetworkPtr net = Network::CreateFFMLPNetwork(numIn,numHid,numOut);

	double rPropEtaPlus = 1.2;
	double rPropEtaMinus = 0.5;
	int rPropMaxEpochs = 3000;
	double rPropMinError = 0.04;

	double ccMaxEpochs = 3000;
	double ccNumCands = 8;

	if (params.Load(xmlpath,"params")){
		params.GetVar("rProp.vars@etaPlus",rPropEtaPlus,true);
		params.GetVar("rProp.vars@etaMinus",rPropEtaMinus,true);
		params.GetVar("rProp.vars@maxEpochs",rPropMaxEpochs,true);
		params.GetVar("rProp.vars@minError",rPropMinError,true);

		params.GetVar("CC.vars@maxEpochs", ccMaxEpochs, true);
		params.GetVar("CC.vars@numCands", ccNumCands, true);
		
	}




	if (type == "BP")
	{
		FFMLPNetPtr net = FFMLPNetwork::Create();
		net->Build(numIn,numHid,numOut);
		//net.printConnections();
	//    BackPropTrainer bp(net, ds, 0.01);
		RPropTrainer bp(net, ds, rPropEtaPlus, rPropEtaMinus);
	//    cout << bp.trainEpoch() << endl;
		int skips = 0;
		for (int i = 0; i < numRuns;){
			//bp.trainToConvergence(0.1, 1000);
			double time = 0.0;
			clock_t startTime = clock();
			if (useValidation){
				bp.TrainToValConv(rPropMaxEpochs);
			}
			else {
				bp.TrainToConvergence(rPropMinError, rPropMaxEpochs);
			}
			time = (clock() - startTime)/(double)CLOCKS_PER_SEC;
			int epochs = bp.GetEpochs();
			myfile << epochs << "\t";
			myfile << time << "\t";
			//cout << epochs << "\t";
			myfile << numHid << "\t";
			//cout << numHid << "\t";
			hashedDoubleMap testresults = bp.TestWiClass(Dataset::TEST);
			std::pair<string,double> p;
			BOOST_FOREACH(p, testresults){
				myfile << p.first << ":" << p.second << "\t";
				//cout << p.first << ":" << p.second << "\t";
			}
			myfile << "|\t";
			printDoubles(bp.GetMSERec(),myfile);
			myfile << endl;
			//cout << endl;
			i++;
			bp.Reset();
		}
	} 
	else
	{
		CCNetworkPtr net = CCNetwork::Create();
		net->Build(numIn,numOut);

		CCTrainer cc = CCTrainer(net,ds,ccNumCands);

		for (int i = 0; i < numRuns;){
			double time = 0.0;
			clock_t startTime = clock();
			if(useValidation){
				cc.TrainToValConv(ccMaxEpochs);
			}
			else {
				cc.TrainToConvergence(ccMaxEpochs);
			}
			time = (clock() - startTime)/(double)CLOCKS_PER_SEC;

			int epochs = cc.GetEpochs();
			int hiddenLayers = cc.GetNumHidLayers();
			myfile << epochs << "\t";
			myfile << time << "\t";
			myfile << hiddenLayers << "\t";
#ifdef _DEBUG
			cout << epochs << "\t";
			cout << hiddenLayers << "\t";
			cout << time << "\t";
#endif // _DEBUG
			
			
			hashedDoubleMap testresults = cc.TestWiClass(Dataset::TEST);
			std::pair<string,double> p;
			BOOST_FOREACH(p, testresults){
				myfile << p.first << ":" << p.second << "\t";
#ifdef _DEBUG
				cout << p.first << ":" << p.second << "\t";
#endif // _DEBUG
			}
			myfile << "|\t";
			printDoubles(cc.GetMSERec(),myfile);
			myfile << endl;
#ifdef _DEBUG
			cout << endl;
#endif // _DEBUG
			
			i++;
			cc.Reset();
		}
	}
    myfile.close();
    return (EXIT_SUCCESS);
}

//void printDoubles(doubles toPrint, ofstream& file){
//	BOOST_FOREACH(double db, toPrint){
//		file << db << "\t";
//	}
//}