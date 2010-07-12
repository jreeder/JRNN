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

const int Dataset::GetSize(){
    return size;
}

void Dataset::SetNumInputs(int numInputs){
    this->numInputs = numInputs;
}

void Dataset::SetNumOutputs(int numOutputs){
    this->numOutputs = numOutputs;
}

void Dataset::LoadFromFile(std::string filepath, int numInputs, int numOutputs){
    ifstream dataFile(filepath.c_str());
    numInputs = numInputs;
    numOutputs = numOutputs;
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("\t");
    std::string line;
    if(dataFile.is_open()){
        while(getline(dataFile,line)){
            tokenizer tok(line,sep);
            vecDouble in(numInputs);
            vecDouble out(numOutputs);
            int newIns = 0;
            int newOuts = 0;
            BOOST_FOREACH(std::string token, tok){
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
    int i = 0;
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