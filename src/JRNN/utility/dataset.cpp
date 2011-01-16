/* 
 * File:   dataset.cpp
 * Author: jreeder
 * 
 * Created on May 27, 2010, 4:47 PM
 */
#include "JRNN.h"
#include "utility/dataset.h"

using namespace JRNN;
using namespace std;

Dataset::Dataset() {
}

Dataset::Dataset(const Dataset& orig) {
	inputs = orig.inputs;
	numInputs = orig.numInputs;
	numOutputs = orig.numOutputs;
	numTest	= orig.numTest;
	numTrain = orig.numTrain;
	numVal = orig.numVal;
	outputs = orig.outputs;
	randomRange = orig.randomRange;
	randSeed = orig.randSeed;
	stdDev = orig.stdDev;
	testIns = orig.testIns;
	testOuts = orig.testOuts;
	testStdDev = orig.testStdDev;
	trainIns = orig.trainIns;
	trainOuts = orig.trainOuts;
	trainStdDev = orig.trainStdDev;
	valIns = orig.valIns;
	valOuts = orig.valOuts;
	valStdDev = orig.valStdDev;
	size = orig.size;
}

Dataset::~Dataset() {
}

const matDouble& Dataset::GetInputs(datatype type){
    switch(type){
        case Dataset::TRAIN:
            return trainIns;
        case Dataset::VAL:
            return valIns;
        case Dataset::TEST:
            return testIns;
        default:
            return inputs;
    }

}

const matDouble& Dataset::GetOutputs(datatype type){
    switch(type){
        case Dataset::TRAIN:
            return trainOuts;
        case Dataset::VAL:
            return valOuts;
        case Dataset::TEST:
            return testOuts;
        default:
            return outputs;
    }
}

const int Dataset::GetSize(datatype type){
	switch (type)
	{
	case Dataset::TRAIN:
		return numTrain;
	case Dataset::VAL:
		return numVal;
	case Dataset::TEST:
		return numTest;
	default:
		return size;
	}
}

void Dataset::SetNumInputs(int numInputs){
    this->numInputs = numInputs;
}

void Dataset::SetNumOutputs(int numOutputs){
    this->numOutputs = numOutputs;
}

void Dataset::LoadFromFile(string filepath, int numInputs, int numOutputs){
	//TODO: Need to read class when it loads here and store that information for later use.
	//TODO: Need to analyze class distribution of data coming in. 
	ifstream dataFile(filepath.c_str());
	this->numInputs = numInputs;
	this->numOutputs = numOutputs;
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep("\t");
	string line;
	if(dataFile.is_open()){
		while(getline(dataFile,line)){
			tokenizer tok(line,sep);
			vecDouble in(numInputs);
			vecDouble out(numOutputs);
			int newIns = 0;
			int newOuts = 0;
			BOOST_FOREACH(string token, tok){
				if (newIns < numInputs){
					in[newIns] = lexical_cast<double>(token);
					newIns++;
				}
				else if (newOuts < numOutputs){
					out[newOuts] = lexical_cast<double>(token);
					newOuts++;
				}
			}
			inputs.push_back(in);
			outputs.push_back(out);
		}
		size = inputs.size();
		dataFile.close();
	}
	else {
		size = 0;
	}
	for (int i = 0; i < size; i ++){
		randomRange.push_back(i);
	}

}

void Dataset::DistData(int numTrain, int numVal, int numTest){
    this->numTrain = numTrain;
    this->numVal = numVal;
    this->numTest = numTest;
	trainIns.clear();
	trainOuts.clear();
	valIns.clear();
	valOuts.clear();
	testIns.clear();
	testOuts.clear();
    randSeed = 314159;
    //genRandRange();
    Distribute();
}

void Dataset::RedistData(){
    randSeed++;
    GenRandRange();
    Distribute();
}

void Dataset::Distribute(){
    //TODO: need to place some error checks here ... this is very unsafe.
	//TODO: need to load numbers of points based on class distribution.
    int i = 0;
	assert(size);
    for (;i < numTrain; i++)
    {
        trainIns.push_back(inputs[randomRange[i]]);
        trainOuts.push_back(outputs[randomRange[i]]);
    }
    for (; i < (numTrain + numVal); i++){
        valIns.push_back(inputs[randomRange[i]]);
        valOuts.push_back(outputs[randomRange[i]]);
    }
    for(; i < (numTrain + numVal + numTest); i++){
        testIns.push_back(inputs[randomRange[i]]);
        testOuts.push_back(outputs[randomRange[i]]);
    }
	CalcStdDevs();
}

void Dataset::GenRandRange(){
    srand(randSeed);
    vector<int> source;
    source.reserve(size);
    randomRange.resize(size);
    for (int i = 0; i < size; i++){
        source.push_back(i);
    }
    randomRange[0] = source[0];
    for (int i = 1; i < size; i++){
        int j = rand() % (i + 1);
        randomRange[i] = randomRange[j];
        randomRange[j] = source[i];
    }
}

double Dataset::GetStdDev( datatype type )
{
	switch(type){
	case Dataset::TRAIN:
		return trainStdDev;
	case Dataset::TEST:
		return testStdDev;
	case Dataset::VAL:
		return valStdDev;
	default:
		return stdDev;
	}
}

void Dataset::CalcStdDevs()
{
	trainStdDev = CalcStdDev(Dataset::TRAIN);
	testStdDev = CalcStdDev(Dataset::TEST);
	valStdDev = CalcStdDev(Dataset::VAL);
	stdDev = CalcStdDev(Dataset::ALL);
}

double Dataset::CalcStdDev( datatype type )
{
	double sum = 0.0;
	double sumSq = 0.0;
	matDouble outs;
	int nPoints;
	int nVals;
	switch (type){
	case Dataset::TRAIN:
		outs = trainOuts;
		nPoints = numTrain;
		break;
	case Dataset::TEST:
		outs = testOuts;
		nPoints = numTest;
		break;
	case Dataset::VAL:
		outs = valOuts;
		nPoints = numVal;
		break;
	default:
		outs = outputs;
		nPoints = size;
	}
	nVals = nPoints * numOutputs;

	BOOST_FOREACH(vecDouble curOut, outs){
		sum += ublas::sum(curOut);
		sumSq += ublas::sum(SquareVec(curOut));
	}

	return sqrt((nVals * sumSq - sum * sum) / (double)(nVals * (nVals - 1.0)));
}
