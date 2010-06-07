/* 
 * File:   main.cpp
 * Author: jreeder
 *
 * Created on May 27, 2010, 6:15 PM
 */

#include <stdlib.h>
#include "JRNN.h"
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
        cout << "Wrong Number of imputs" << endl;
        return -1;
    }
    else {
        filename = argv[1];
        numIn = lexical_cast<int>(argv[2]);
        numHid = lexical_cast<int>(argv[3]);
        numOut = lexical_cast<int>(argv[4]);
        outfile += filename;
        outfile += "results.txt";
    }
    ofstream myfile;
    myfile.open(outfile.c_str());
    datasetPtr ds(new dataset());
    ds->loadFromFile(filename, numIn,numOut);
    ds->distData(100,100,500);
    network net(numIn,numHid,numOut);
    //net.printConnections();
    BackPropTrainer bp(net, ds, 0.01);
//    cout << bp.trainEpoch() << endl;
    int skips = 0;
    for (int i = 0; i < 30;){
        //bp.trainToConvergence(0.1, 50000);
        bp.trainToValConv(50000);
        int epochs = bp.getEpochs();
        if (epochs > 10000){
            myfile << epochs << "\t";
            hashedDoubleMap testresults = bp.testWiClass(dataset::TEST);
            std::pair<std::string,double> p;
            BOOST_FOREACH(p, testresults){
                myfile << p.first << " " << p.second << "\t";
            }
            myfile << endl;
            i++;
        }
        else{
            ds->redistData();
        }
        bp.reset();
    }
    myfile.close();
    return (EXIT_SUCCESS);
}

