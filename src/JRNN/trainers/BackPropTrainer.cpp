/* 
 * File:   BackPropTrainer.cpp
 * Author: jreeder
 * 
 * Created on May 20, 2010, 3:32 PM
 */

#include "JRNN.h"

using namespace JRNN;

BackPropTrainer::BackPropTrainer(network& inNetwork, datasetPtr inDataSet, double learningRate) {
    this->mNetwork = &inNetwork;
    this->data = inDataSet;
    //TODO: right now I'm assuming the dataset is ready to go. need to add in a flah
    //that says it is ready or not and do something here accordingly. 
//    this->trainingIns = trainingData;
//    this->trainingOuts = desiredOuts;
    this->learningRate = learningRate;
    this->epochCount = 0;
}

BackPropTrainer::BackPropTrainer(const BackPropTrainer& orig) {
    this->mNetwork = orig.mNetwork;
    this->data = orig.data;
//    this->trainingIns = orig.trainingIns;
//    this->trainingOuts = orig.trainingOuts;
    this->learningRate = orig.learningRate;
    this->epochCount = orig.epochCount;
}

BackPropTrainer::~BackPropTrainer() {
}

void BackPropTrainer::reset(){
    this->MSE_Rec.clear();
    this->bestWeights.clear();
    this->epochCount = 0;
    this->localGradients.clear();
    this->mNetwork->reset();
    this->taskErrorRate.clear();
    this->taskErrors.clear();
    this->weightUpdates.clear();
    this->vMSE_Rec.clear();
}

double BackPropTrainer::trainEpoch(){
    matDouble trainingIns = data->getInputs(dataset::TRAIN);
    matDouble trainingOuts = data->getOutputs(dataset::TRAIN);
    matDouble::iterator itData;
    matDouble::iterator itOut;
//    itData = this->trainingIns.begin();
//    itOut = this->trainingOuts.begin();
    itData = trainingIns.begin();
    itOut = trainingOuts.begin();
    double SSE = 0;
    while(itData != trainingIns.end()){
        vecDouble input = (*itData);
        vecDouble desiredOut = (*itOut);
        this->mNetwork->activate(input);
        vecDouble output = this->mNetwork->getOutputs();
       // cout << "network output: " << output << " Desired Out: " << desiredOut << endl;
        vecDouble error = desiredOut - output;
        vecDouble sqError = squareVec(error);
        SSE += ublas::sum(sqError);
        this->weightUpdates.clear();
        this->localGradients.clear();
        calcWeightUpdates(this->mNetwork->getLayer("out"), desiredOut);
        conList consToUpdate = this->mNetwork->getConnections();
        BOOST_FOREACH(conPtr con, consToUpdate){
            double tmp = this->weightUpdates[con->getName()];
            //cout << "weight update: " << tmp << endl;
            (*con.get()) += this->weightUpdates[con->getName()];
        }
        itData++;
        itOut++;
    }
    this->epochCount++;
    SSE /= (double)trainingIns.size();
    this->MSE_Rec.push_back(SSE);
    return SSE;
}

double BackPropTrainer::testOnData(dataset::datatype type){
    matDouble ins = data->getInputs(type);
    matDouble outs = data->getOutputs(type);
    matDouble::iterator itIns = ins.begin();
    matDouble::iterator itOuts = outs.begin();

    double SSE = 0;
    while(itIns != ins.end()){
        vecDouble input = (*itIns);
        vecDouble desiredOut = (*itOuts);
        this->mNetwork->activate(input);
        vecDouble output = this->mNetwork->getOutputs();
        vecDouble error = desiredOut - output;
        vecDouble sqError = squareVec(error);
        SSE += ublas::sum(sqError);
        itIns++;
        itOuts++;
    }
    SSE /= (double)ins.size();
    return SSE;
}

hashedDoubleMap BackPropTrainer::testWiClass(dataset::datatype type){
    //TODO: need to make this more robust so we can have more than one output per task;
    matDouble ins = data->getInputs(type);
    matDouble outs = data->getOutputs(type);
    matDouble::iterator itIns = ins.begin();
    matDouble::iterator itOuts = outs.begin();

    int numInCorrect = 0;
    int totalItems = ins.size();
    int numTasks = (*itOuts).size();
    while(itIns != ins.end()){
        vecDouble input = (*itIns);
        vecDouble desiredOut = (*itOuts);
        this->mNetwork->activate(input);
        vecDouble output = this->mNetwork->getOutputs();
        vecDouble thresOut = applyThreshold(output);
        vecDouble errors = desiredOut - thresOut;

        for(int i = 0; i < numTasks; i++){
            std::string name = "task-";
            name += lexical_cast<std::string>(i);
            int tmp = (int)errors[i];
            //cout << tmp << " " << output << " " << desiredOut << endl;
            if (errors[i] != 0){
                this->taskErrors[name]++;
            }
        }
        itIns++;
        itOuts++;
    }
    for (int i = 0; i < numTasks; i++){
        std::string name = "task-";
        name += lexical_cast<std::string>(i);
        this->taskErrorRate[name] = this->taskErrors[name] / (double)totalItems;
    }
    return this->taskErrorRate;
}

void BackPropTrainer::calcWeightUpdates(layerPtr layer, vecDouble desiredOut){
    if(layer->getType() != layer::input){
        nodeList& nodes = layer->getNodes();
        switch(layer->getType()){
            case layer::out:
                for(int i = 0; i < layer->getSize(); i++){
                    std::string name = nodes[i]->getName();
                    double act = nodes[i]->getOut();
                    double sigSteep = nodes[i]->getSigSteepness();
                    double error = desiredOut[i] - act;
                    double delta = sigSteep * error * act * (1-act);
                    this->localGradients[name] = delta;
                    conList cons = nodes[i]->getConnections(node::IN);
                    BOOST_FOREACH(conPtr con, cons){
                        double dw = this->learningRate * con->getValue() * delta;
                        this->weightUpdates[con->getName()] = dw;
                    }
                }
                break;
            case layer::hidden:
                for(int i = 0; i < layer->getSize(); i++){
                    std::string name = nodes[i]->getName();
                    double act = nodes[i]->getOut();
                    double sigSteep = nodes[i]->getSigSteepness();
                    conList outCons = nodes[i]->getConnections(node::OUT);
                    double sumOfChildError = 0;
                    BOOST_FOREACH(conPtr con, outCons){
                        sumOfChildError += this->localGradients[con->getOutNodeName()] * con->getWeight();
                    }
                    double delta = sigSteep * act * (1-act) * sumOfChildError;
                    this->localGradients[name] = delta;
                    conList inCons = nodes[i]->getConnections(node::IN);
                    BOOST_FOREACH(conPtr con, inCons){
                        double dw = this->learningRate * con->getValue() * delta;
                        this->weightUpdates[con->getName()] = dw;
                    }
                }
                break;
            default:
                break;
        }
        calcWeightUpdates(layer->getPrevLayer());
    }
}

doubles& BackPropTrainer::getMSERec(){
    return this->MSE_Rec;
}

void BackPropTrainer::trainToConvergence(double maxSSE, int maxEpoch){
    double curSSE = 10;
    while(curSSE > maxSSE && this->epochCount < maxEpoch){
        curSSE = this->trainEpoch();
    }
}

void BackPropTrainer::trainToValConv(int maxEpoch){
    double curSSE = 10;
    double bestVERR = this->testOnData(dataset::VAL);
    bool proceed = true;
    int epochsSinceImpr = 0;
    while(proceed){
        curSSE = this->trainEpoch();
        double curVERR = this->testOnData(dataset::VAL);
        this->vMSE_Rec.push_back(curVERR);
        epochsSinceImpr++;
        if (this->epochCount == 1 || curVERR < bestVERR){
            bestVERR = curVERR;
            this->bestWeights = this->mNetwork->getWeights();
            epochsSinceImpr = 0;
        }
        if (this->epochCount >= maxEpoch){
            this->mNetwork->setWeights(this->bestWeights);
            proceed = false;
            continue;
        }
        if (epochsSinceImpr >= this->continueEpochs){
            this->mNetwork->setWeights(this->bestWeights);
            proceed = false;
            continue;
        }

    }
}

vecDouble BackPropTrainer::squareVec(vecDouble vector){
    vecDouble::iterator it = vector.begin();
    for(;it < vector.end(); it++){
        (*it) = pow((*it),2);
    }
    return vector;
}
vecDouble BackPropTrainer::applyThreshold(vecDouble vector){
    vecDouble::iterator it = vector.begin();
    for(;it != vector.end(); it++){
        if ((*it) < 0.5){
            (*it) = 0;
        }
        else{
            (*it) = 1;
        }
    }
    return vector;
}

doubles& BackPropTrainer::getVMSERec(){
    return this->vMSE_Rec;
}


int BackPropTrainer::getEpochs(){
    return this->epochCount;
}