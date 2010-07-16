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
    int numIn,numHid,numOut;

    if (argc != 5){
        cout << "Wrong Number of inputs" << endl;
        return -1;
    }
    else {
        filename = argv[1];
        numIn = lexical_cast<int>(argv[2]);
        numHid = lexical_cast<int>(argv[3]);
        numOut = lexical_cast<int>(argv[4]);
        outfile += filename;
        outfile += " results.txt";
    }
    ofstream myfile;
    myfile.open(outfile.c_str());
    DatasetPtr ds(new Dataset());
	//FFMLPNetwork netBuilder(numIn, numHid, numOut);
    ds->LoadFromFile(filename, numIn,numOut);
    ds->DistData(100,100,500);
    //NetworkPtr net = Network::CreateFFMLPNetwork(numIn,numHid,numOut);
	FFMLPNetPtr net = FFMLPNetwork::Create();
	net->Build(numIn,numHid,numOut);
    //net.printConnections();
//    BackPropTrainer bp(net, ds, 0.01);
    RPropTrainer bp(net, ds, 1.2, 0.5);
//    cout << bp.trainEpoch() << endl;
    int skips = 0;
    for (int i = 0; i < 30;){
        //bp.trainToConvergence(0.1, 1000);
        bp.TrainToValConv(1000);
        int epochs = bp.GetEpochs();
        //if (epochs > 10000){
            myfile << epochs << "\t";
            cout << epochs << "\t";
            hashedDoubleMap testresults = bp.TestWiClass(Dataset::TEST);
            std::pair<std::string,double> p;
            BOOST_FOREACH(p, testresults){
                myfile << p.first << " " << p.second << "\t";
                cout << p.first << " " << p.second << "\t";
            }
            myfile << endl;
            cout << endl;
            i++;
//        }
//        else{
//            ds->redistData();
//        }
        bp.Reset();
    }
    myfile.close();
    return (EXIT_SUCCESS);
}

