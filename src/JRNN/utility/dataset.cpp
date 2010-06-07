/* 
 * File:   dataset.cpp
 * Author: jreeder
 * 
 * Created on May 27, 2010, 4:47 PM
 */

#include "JRNN.h"

using namespace JRNN;
using namespace std;

dataset::dataset() {
}

dataset::dataset(const dataset& orig) {
}

dataset::~dataset() {
}

const matDouble& dataset::getInputs(datatype type){
    switch(type){
        case dataset::TRAIN:
            return this->trainIns;
        case dataset::VAL:
            return this->valIns;
        case dataset::TEST:
            return this->testIns;
        default:
            return this->inputs;
    }

}

const matDouble& dataset::getOutputs(datatype type){
    switch(type){
        case dataset::TRAIN:
            return this->trainOuts;
        case dataset::VAL:
            return this->valOuts;
        case dataset::TEST:
            return this->testOuts;
        default:
            return this->outputs;
    }
}

const int dataset::getSize(){
    return this->size;
}

void dataset::setNumInputs(int numInputs){
    this->numInputs = numInputs;
}

void dataset::setNumOutputs(int numOutputs){
    this->numOutputs = numOutputs;
}

void dataset::loadFromFile(std::string filepath, int numInputs, int numOutputs){
    ifstream dataFile(filepath.c_str());
    this->numInputs = numInputs;
    this->numOutputs = numOutputs;
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("\t");
    std::string line;
    if(dataFile.is_open()){
        while(getline(dataFile,line)){
            tokenizer tok(line,sep);
            vecDouble in(this->numInputs);
            vecDouble out(this->numOutputs);
            int newIns = 0;
            int newOuts = 0;
            BOOST_FOREACH(std::string token, tok){
                if (newIns < this->numInputs){
                    in[newIns] = lexical_cast<double>(token);
                    newIns++;
                }
                else if (newOuts < this->numOutputs){
                    out[newOuts] = lexical_cast<double>(token);
                    newOuts++;
                }
            }
            this->inputs.push_back(in);
            this->outputs.push_back(out);
        }
        this->size = inputs.size();
        dataFile.close();
    }
    else {
        this->size = 0;
    }

}

void dataset::distData(int numTrain, int numVal, int numTest){
    this->numTrain = numTrain;
    this->numVal = numVal;
    this->numTest = numTest;
    this->randSeed = 1983;
    genRandRange();
    distribute();
}

void dataset::redistData(){
    this->randSeed++;
    genRandRange();
    distribute();
}

void dataset::distribute(){
    //TODO: need to place some error checks here ... this is very unsafe.
    int i = 0;
    for (;i < numTrain; i++)
    {
        this->trainIns.push_back(inputs[randomRange[i]]);
        this->trainOuts.push_back(outputs[randomRange[i]]);
    }
    for (; i < (numTrain + numVal); i++){
        this->valIns.push_back(inputs[randomRange[i]]);
        this->valOuts.push_back(outputs[randomRange[i]]);
    }
    for(; i < (numTrain + numVal + numTest); i++){
        this->testIns.push_back(inputs[randomRange[i]]);
        this->testOuts.push_back(outputs[randomRange[i]]);
    }
}

void dataset::genRandRange(){
    srand(this->randSeed);
    vector<int> source;
    source.reserve(this->size);
    this->randomRange.resize(this->size);
    for (int i = 0; i < this->size; i++){
        source.push_back(i);
    }
    this->randomRange[0] = source[0];
    for (int i = 1; i < this->size; i++){
        int j = rand() % (i + 1);
        this->randomRange[i] = this->randomRange[j];
        this->randomRange[j] = source[i];
    }
}