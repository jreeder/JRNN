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

RandomGeneratorInt Dataset::dRand = RandomGeneratorInt(1,RAND_MAX);

Dataset::Dataset() {
	outputPerCategory = false;
	normalizeReals = false;
	dsAnalyzed = false;
	outClassIndexes.clear();
	outClassPercentage.clear();
	outClassNames.clear();
}

Dataset::Dataset(const Dataset& orig) {
	inputs = orig.inputs;
	numInputs = orig.numInputs;
	numOutputs = orig.numOutputs;
	numTest	= orig.numTest;
	numTrain = orig.numTrain;
	numVal = orig.numVal;
	outputPerCategory = orig.outputPerCategory;
	normalizeReals = orig.normalizeReals;
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
	outClassIndexes = orig.outClassIndexes;
	outClassPercentage = orig.outClassPercentage;
	outClassNames = orig.outClassNames;
	dsAnalyzed = orig.dsAnalyzed;
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

void Dataset::SetOutputPerCategory( bool outPerCat )
{
	outputPerCategory = outPerCat;
}

void Dataset::SetNormalizeReals( bool normReals )
{
	normalizeReals = normReals;
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
	int count = 0;
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
			string outname = StringFromVector(out);
			outClassIndexes[outname].push_back(count);
			//if (outClassIndexes.find(outname) != outClassIndexes.end()){
			//	outClassIndexes[outname].push_back(count);
			//}
			//else {
			//	//temp
			//}
			count++;
		}
		size = inputs.size();
		hashedIntsMap::iterator it = outClassIndexes.begin();
		//calculate the percentage of each outclass to the whole.
		while(it != outClassIndexes.end()){
			double tmpPerc = it->second.size() / (double)size;
			outClassPercentage[it->first] = tmpPerc;
			outClassNames.push_back(it->first);
			Shuffle(it->second);
			it++;
		}
		dsAnalyzed = true;
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
	if (!dsAnalyzed){
		AnalyzeDS();
	}
    Distribute();
}

void Dataset::RedistData(){
    randSeed++;
    //GenRandRange();
	Reshuffle();
    Distribute();
}

//Analyze the dataset and shuffle according to out class distribution
void Dataset::AnalyzeDS(){
	outClassIndexes.clear();
	outClassPercentage.clear();
	outClassNames.clear();
	for (int i = 0; i < size; i++){
		string outname = StringFromVector(outputs[i]);
		outClassIndexes[outname].push_back(i);
	}
	hashedIntsMap::iterator it = outClassIndexes.begin();
	//calculate the percentage of each outclass to the whole.
	while(it != outClassIndexes.end()){
		double tmpPerc = it->second.size() / (double)size;
		outClassPercentage[it->first] = tmpPerc;
		outClassNames.push_back(it->first);
		Shuffle(it->second);
		it++;
	}
}

void Dataset::Distribute(){
    //TODO: need to place some error checks here ... this is very unsafe.
    assert(size > (numTrain + numVal + numTest));
	
	//Find the number of points from each class that goes into the subsets
	//ints trainClsCounts, valClsCounts, testClsCounts, clsPositions;
	//int totalTr = 0, totalVal = 0, totalTest = 0;
	//int numClasses = outClassNames.size();
	hashedIntsMap indexQueues = outClassIndexes;


	FillSubset(trainIns, trainOuts, numTrain, indexQueues);
	FillSubset(valIns, valOuts, numVal, indexQueues);
	FillSubset(testIns, testOuts, numTest, indexQueues);
	//changed to attempt to overcome small numbers of specific outputs. 
	//int i = 0;
	//while (totalTr < numTrain)
	//{
	//	int classIndex = i++ % numClasses; 
	//	string className = outClassNames[classIndex];
	//	double tmpPerc = outClassPercentage[className];
	//	int tmpCount = 0;
	//	tmpCount = (int)floor((numTrain * tmpPerc) + 0.5);
	//	if (tmpCount > (numTrain - totalTr)){
	//		tmpCount = numTrain - totalTr;
	//	}
	//	while (!indexQueues[className].empty() && tmpCount-- > 0)
	//	{
	//		int index = indexQueues[className].back();
	//		trainIns.push_back(inputs[index]);
	//		trainOuts.push_back(outputs[index]);
	//		totalTr++;
	//		indexQueues[className].pop_back();
	//	}
	//	int tmpSize1 = indexQueues[className].size();
	//	int tmpSize2 = outClassIndexes[className].size();
	//}
	//i = 0;
	//while (totalVal < numVal)
	//{
	//	int classIndex = i++ % numClasses; 
	//	string className = outClassNames[classIndex];
	//	double tmpPerc = outClassPercentage[className];
	//	int tmpCount = 0;
	//	tmpCount = (int)floor((numVal * tmpPerc) + 0.5);
	//	if (tmpCount > (numVal - totalVal)){
	//		tmpCount = numVal - totalVal;
	//	}
	//	while (!indexQueues[className].empty() && tmpCount-- > 0)
	//	{
	//		int index = indexQueues[className].back();
	//		valIns.push_back(inputs[index]);
	//		valOuts.push_back(outputs[index]);
	//		totalVal++;
	//		indexQueues[className].pop_back();
	//	}
	//	int tmpSize1 = indexQueues[className].size();
	//	int tmpSize2 = outClassIndexes[className].size();
	//}
	//i = 0;
	//while (totalTest < numTest)
	//{
	//	int classIndex = i++ % numClasses; 
	//	string className = outClassNames[classIndex];
	//	double tmpPerc = outClassPercentage[className];
	//	int tmpCount = 0;
	//	tmpCount = (int)floor((numTest * tmpPerc) + 0.5);
	//	if (tmpCount > (numTest - totalTest)){
	//		tmpCount = numTest - totalTest;
	//	}
	//	while (!indexQueues[className].empty() && tmpCount-- > 0)
	//	{
	//		int index = indexQueues[className].back();
	//		testIns.push_back(inputs[index]);
	//		testOuts.push_back(outputs[index]);
	//		totalTest++;
	//		indexQueues[className].pop_back();
	//	}
	//	int tmpSize1 = indexQueues[className].size();
	//	int tmpSize2 = outClassIndexes[className].size();
	//}

	//for(int i = 0; i < numClasses; i++){
	//	string className = outClassNames[i];
	//	double tmpPerc = outClassPercentage[className];
	//	int trTmpCount = 0, valTmpCount = 0, testTmpCount = 0;
	//	if (outClassIndexes[className].size() < 3){
	//		continue;
	//	}
	//	if (i < numClasses - 1){
	//		trTmpCount = (int)floor(numTrain * tmpPerc);
	//		valTmpCount = (int)floor(numVal * tmpPerc);
	//		testTmpCount = (int)floor(numTest * tmpPerc);
	//		totalTr += trTmpCount;
	//		totalVal += valTmpCount;
	//		totalTest += testTmpCount;
	//	} 
	//	else
	//	{
	//		trTmpCount = numTrain - totalTr;
	//		valTmpCount = numVal - totalVal;
	//		testTmpCount = numTest - totalTest;
	//	}
	//	/*trainClsCounts.push_back(trTmpCount);
	//	valClsCounts.push_back(valTmpCount);
	//	testClsCounts.push_back(testTmpCount);
	//	clsPositions.push_back(0);*/
	//	assert (outClassIndexes[className].size() > (unsigned int)(trTmpCount + valTmpCount + testTmpCount));
	//	int j = 0;
	//	for (; j < trTmpCount; j++){
	//		trainIns.push_back(inputs[outClassIndexes[className][j]]);
	//		trainOuts.push_back(outputs[outClassIndexes[className][j]]);
	//	}
	//	for (; j < trTmpCount + valTmpCount; j++){
	//		valIns.push_back(inputs[outClassIndexes[className][j]]);
	//		valOuts.push_back(outputs[outClassIndexes[className][j]]);
	//	}
	//	for(; j < trTmpCount + valTmpCount + testTmpCount; j++){
	//		testIns.push_back(inputs[outClassIndexes[className][j]]);
	//		testOuts.push_back(outputs[outClassIndexes[className][j]]);
	//	}
	//}


	ShuffleSubsets();


	//Old way of distributing
	//int i = 0;
	//
 //   for (;i < numTrain; i++)
 //   {
 //       trainIns.push_back(inputs[randomRange[i]]);
 //       trainOuts.push_back(outputs[randomRange[i]]);
 //   }
 //   for (; i < (numTrain + numVal); i++){
 //       valIns.push_back(inputs[randomRange[i]]);
 //       valOuts.push_back(outputs[randomRange[i]]);
 //   }
 //   for(; i < (numTrain + numVal + numTest); i++){
 //       testIns.push_back(inputs[randomRange[i]]);
 //       testOuts.push_back(outputs[randomRange[i]]);
 //   }
	CalcStdDevs();
}

void Dataset::FillSubset( matDouble& ins, matDouble& outs, int numExamples, hashedIntsMap& indexQueues )
{
	int i = 0, total = 0;
	int numClasses = outClassNames.size();
	while (total < numExamples){
		int classIndex = i++ % numClasses;
		string className = outClassNames[classIndex];
		double tmpPerc = outClassPercentage[className];
		int tmpCount = 0;
		tmpCount = (int)floor((numExamples * tmpPerc) + 0.5);
		if (tmpCount > (numExamples - total)){
			tmpCount = numExamples - total;
		}
		while(!indexQueues[className].empty() && tmpCount-- > 0){
			int index = indexQueues[className].back();
			ins.push_back(inputs[index]);
			outs.push_back(outputs[index]);
			total++;
			indexQueues[className].pop_back();
		}
	}
}


//Not really used anymore. 
void Dataset::GenRandRange(){
    dRand.gen.seed(randSeed);
    vector<int> source;
    source.reserve(size);
    randomRange.resize(size);
    for (int i = 0; i < size; i++){
        source.push_back(i);
    }
    randomRange[0] = source[0];
    for (int i = 1; i < size; i++){
        int j = dRand() % (i + 1);
        randomRange[i] = randomRange[j];
        randomRange[j] = source[i];
    }
}

void Dataset::Shuffle(ints &indexes){
	dRand.gen.seed((unsigned int)time(NULL));
	int tmpSize = indexes.size();
	int tmpIndex = 0;
	for (int i = tmpSize - 1; i > 0; i--){
		int j = dRand() % (i+1);
		tmpIndex = indexes[i];
		indexes[i] = indexes[j];
		indexes[j] = tmpIndex;
	}
}

ints Dataset::ARange(int start, int end){
	ints retInts;
	retInts.reserve((end - start) + 1);
	for (int i = start; i <= end; i++){
		retInts.push_back(i);
	}
	return retInts;
}

void Dataset::Reshuffle(){
	BOOST_FOREACH(string className, outClassNames){
		Shuffle(outClassIndexes[className]);
	}
}

//Shuffles the subsets so that they are in random order. 
void Dataset::ShuffleSubsets(){
	dRand.gen.seed((unsigned int)time(NULL));
	int tmpSize = trainIns.size();
	vecDouble tmpInVec, tmpOutVec;
	for (int i = tmpSize - 1; i > 0; i--){
		int j = dRand() % (i + 1);
		tmpInVec = trainIns[i];
		tmpOutVec = trainOuts[i];
		trainIns[i] = trainIns[j];
		trainIns[j] = tmpInVec;
		trainOuts[i] = trainOuts[j];
		trainOuts[j] = tmpOutVec;
	}

	tmpSize = valIns.size();
	for (int i = tmpSize - 1; i > 0; i--){
		int j = dRand() % (i + 1);
		tmpInVec = valIns[i];
		tmpOutVec = valOuts[i];
		valIns[i] = valIns[j];
		valIns[j] = tmpInVec;
		valOuts[i] = valOuts[j];
		valOuts[j] = tmpOutVec;
	}

	tmpSize = testIns.size();
	for (int i = tmpSize - 1; i > 0; i--){
		int j = dRand() % (i + 1);
		tmpInVec = testIns[i];
		tmpOutVec = testOuts[i];
		testIns[i] = testIns[j];
		testIns[j] = tmpInVec;
		testOuts[i] = testOuts[j];
		testOuts[j] = tmpOutVec;
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
	if(nVals > 0){
		return sqrt((nVals * sumSq - sum * sum) / (double)(nVals * (nVals - 1.0)));
	}
	else {
		return 0;
	}
}
