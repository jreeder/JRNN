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
	string filename;
	int numIn,numOut;

	if (argc != 4)
	{	
		cout << "Incorrect Arguments" << endl;
		cout << "Proper Syntax: JRNN_FindLimits <numIn> <numOut>" << endl;
		return -1;
	} 
	else
	{
		filename = argv[1];
		numIn = lexical_cast<int>(argv[2]);
		numOut = lexical_cast<int>(argv[3]);
	}
	//TODO: need to finish this experiment. All it needs to do is read in a data file and run multiple
	//sets of experiments with differing numbers of training points and differing number of hidden layers.
}