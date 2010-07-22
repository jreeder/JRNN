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

using namespace JRNN;
using namespace std;
/*
 * 
 */
int main(int argc, char** argv) {

    std::string filename;
    std::string outfile = "";
	std::string type = "";
    int numIn,numHid,numOut, numTrain, numVal, numTest;

    if (argc != 10){
        cout << "Incorrect Arguments" << endl;
		cout << "Proper Syntax: JRNN_test <filename> <numTrain> <numVal> <numTest> <numIn> <numHid> <numOut> <type = 'BP' or 'CC'> <outfilename>" << endl;
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
		type = std::string(argv[8]);
		if (type != "CC" && type != "BP"){
			cout << "Type must be 'CC' or 'BP'" << endl;
			return -1;
		}
		outfile = std::string(argv[9]);
		
		
        /*outfile = filename;
		outfile.replace(outfile.end()-4,outfile.end()," ");
		outfile += type;
        outfile += " results.txt";*/
    }
    ofstream myfile;
    myfile.open(outfile.c_str());
	if (myfile.is_open() == false){
		cout << "Output file Not open: " << outfile.c_str() << endl;
		return -1;
	}
    DatasetPtr ds(new Dataset());
	//FFMLPNetwork netBuilder(numIn, numHid, numOut);
    ds->LoadFromFile(filename, numIn,numOut);
    ds->DistData(100,100,500);
    //NetworkPtr net = Network::CreateFFMLPNetwork(numIn,numHid,numOut);
	if (type == "BP")
	{
		FFMLPNetPtr net = FFMLPNetwork::Create();
		net->Build(numIn,numHid,numOut);
		//net.printConnections();
	//    BackPropTrainer bp(net, ds, 0.01);
		RPropTrainer bp(net, ds, 1.2, 0.5);
	//    cout << bp.trainEpoch() << endl;
		int skips = 0;
		for (int i = 0; i < 60;){
			//bp.trainToConvergence(0.1, 1000);
			bp.TrainToValConv(3000);
			int epochs = bp.GetEpochs();
			myfile << epochs << "\t";
			cout << epochs << "\t";
			myfile << numHid << "\t";
			cout << numHid << "\t";
			hashedDoubleMap testresults = bp.TestWiClass(Dataset::TEST);
			std::pair<std::string,double> p;
			BOOST_FOREACH(p, testresults){
				myfile << p.first << " " << p.second << "\t";
				cout << p.first << " " << p.second << "\t";
			}
			myfile << endl;
			cout << endl;
			i++;
			bp.Reset();
		}
	} 
	else
	{
		CCNetworkPtr net = CCNetwork::Create();
		net->Build(numIn,numOut);

		CCTrainer cc = CCTrainer(net,ds,8);

		for (int i = 0; i < 60;){
			cc.TrainToValConv(3000);
			int epochs = cc.GetEpochs();
			int hiddenLayers = cc.GetNumHidLayers();
			myfile << epochs << "\t";
			cout << epochs << "\t";
			myfile << hiddenLayers << "\t";
			cout << hiddenLayers << "\t";
			hashedDoubleMap testresults = cc.TestWiClass(Dataset::TEST);
			std::pair<std::string,double> p;
			BOOST_FOREACH(p, testresults){
				myfile << p.first << " " << p.second << "\t";
				cout << p.first << " " << p.second << "\t";
			}
			myfile << endl;
			cout << endl;
			i++;
			cc.Reset();
		}
	}
    myfile.close();
    return (EXIT_SUCCESS);
}

