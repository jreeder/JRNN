/*
 *File:		main.cpp
 *Author: 	jreeder
 *
 *Created on January 18, 2011
 * 
 *Experiment to find the limits of a data set in terms of how many nodes and data points are needed. 
 */

#include <stdlib.h>
#include "structure/network.h"
#include "utility/dataset.h"
#include "trainers/RPropTrainer.h"
#include "networks/FFMLPNetwork.h"
#include <ctime>

using namespace JRNN;
using namespace std;

int main(int argc, char** argv) {
	string filename,outfile;
	int numIn,numOut;

	if (argc != 5)
	{	
		cout << "Incorrect Arguments" << endl;
		cout << "Proper Syntax: JRNN_FindLimits <filename> <outfile> <numIn> <numOut>" << endl;
		return -1;
	} 
	else
	{
		filename = argv[1];
		outfile = argv[2];
		numIn = lexical_cast<int>(argv[3]);
		numOut = lexical_cast<int>(argv[4]);
	}
	//TODO: need to finish this experiment. All it needs to do is read in a data file and run multiple
	//sets of experiments with differing numbers of training points and differing number of hidden layers.

	fstream myfile;
	myfile.open(outfile.c_str());
	if (myfile.is_open() == false){
		cout << "Output file not open: " << outfile.c_str() << endl;
		return -1;
	}

	DatasetPtr ds(new Dataset());

	ds->LoadFromFile(filename,numIn,numOut);
	int numTest = 200;
	int numVal = 200;
	int numRuns = 60;
	ints numTrains;
	int temparray[] = {200, 100, 50, 40, 30, 20, 10, 5};
	BOOST_FOREACH(int x , temparray){
		numTrains.push_back(x);
	}
	ints numHids;
	int temparray2[] = {1,2,4,8,16,32};
	BOOST_FOREACH(int x, temparray2){
		numHids.push_back(x);
	}
}