/* 
 * File:   BackPropTrainer.cpp
 * Author: jreeder
 * 
 * Created on May 20, 2010, 3:32 PM
 */
#include "JRNN.h"
#include "trainers/BackPropTrainer.h"

using namespace JRNN;

BackPropTrainer::BackPropTrainer(FFMLPNetPtr inNetwork, DatasetPtr inDataSet, double learningRate) {
    mNetwork = inNetwork;
    data = inDataSet;
    //TODO: right now I'm assuming the dataset is ready to go. need to add in a flag
    //that says it is ready or not and do something here accordingly. 
//    trainingIns = trainingData;
//    trainingOuts = desiredOuts;
    this->learningRate = learningRate;
    epochCount = 0;
}

BackPropTrainer::BackPropTrainer(const BackPropTrainer& orig) {
    mNetwork = orig.mNetwork;
    data = orig.data;
//    trainingIns = orig.trainingIns;
//    trainingOuts = orig.trainingOuts;
    learningRate = orig.learningRate;
    epochCount = orig.epochCount;
}

BackPropTrainer::~BackPropTrainer() {
}

void BackPropTrainer::Reset(){
    MSE_Rec.clear();
    bestWeights.clear();
    epochCount = 0;
    localGradients.clear();
    mNetwork->Reset();
    taskErrorRate.clear();
    taskErrors.clear();
    weightUpdates.clear();
    vMSE_Rec.clear();
}

double BackPropTrainer::TrainEpoch(){
    matDouble trainingIns = data->GetInputs(Dataset::TRAIN);
    matDouble trainingOuts = data->GetOutputs(Dataset::TRAIN);
    matDouble::iterator itData;
    matDouble::iterator itOut;
//    itData = trainingIns.begin();
//    itOut = trainingOuts.begin();
    itData = trainingIns.begin();
    itOut = trainingOuts.begin();
    double MSE = 0;
    while(itData != trainingIns.end()){
        vecDouble input = (*itData);
        vecDouble desiredOut = (*itOut);
        mNetwork->Activate(input);
        vecDouble output = mNetwork->GetOutputs();
       // cout << "network output: " << output << " Desired Out: " << desiredOut << endl;
        //vecDouble error = desiredOut - output;
		vecDouble error = Error(desiredOut, output);
        vecDouble sqError = SquareVec(error);
        MSE += ublas::sum(sqError);
        weightUpdates.clear();
        localGradients.clear();
        CalcWeightUpdates(mNetwork->GetLayer("out"), desiredOut);
        ConMap consToUpdate = mNetwork->GetConnections();
        BOOST_FOREACH(ConPair conp, consToUpdate){
			ConPtr con = conp.second;
            double tmp = weightUpdates[con->GetName()];
            //cout << "weight update: " << tmp << endl;
            (*con.get()) += weightUpdates[con->GetName()];
        }
        itData++;
        itOut++;
    }
    epochCount++;
    MSE /= (double)trainingIns.size();
	MSE /= (double)mNetwork->GetNumOut();
    MSE_Rec.push_back(MSE);
    return MSE;
}

double BackPropTrainer::TestOnData(Dataset::datatype type){
    matDouble ins = data->GetInputs(type);
    matDouble outs = data->GetOutputs(type);
    matDouble::iterator itIns = ins.begin();
    matDouble::iterator itOuts = outs.begin();

    double MSE = 0;
    while(itIns != ins.end()){
        vecDouble input = (*itIns);
        vecDouble desiredOut = (*itOuts);
        mNetwork->Activate(input);
        vecDouble output = mNetwork->GetOutputs();
        vecDouble error = desiredOut - output;
        vecDouble sqError = SquareVec(error);
        MSE += ublas::sum(sqError);
        itIns++;
        itOuts++;
    }
    MSE /= (double)ins.size();
	MSE /= (double)mNetwork->GetNumOut();
    return MSE;
}

hashedDoubleMap BackPropTrainer::TestWiClass(Dataset::datatype type){
    //TODO: need to make this more robust so we can have more than one output per task;
    matDouble ins = data->GetInputs(type);
    matDouble outs = data->GetOutputs(type);
    matDouble::iterator itIns = ins.begin();
    matDouble::iterator itOuts = outs.begin();

    int numInCorrect = 0;
    int totalItems = ins.size();
    int numTasks = (*itOuts).size();
    while(itIns != ins.end()){
        vecDouble input = (*itIns);
        vecDouble desiredOut = (*itOuts);
        mNetwork->Activate(input);
        vecDouble output = mNetwork->GetOutputs();
        vecDouble thresOut = ApplyThreshold(output);
        vecDouble errors = desiredOut - thresOut;

        for(int i = 0; i < numTasks; i++){
            string name = "task-";
            name += lexical_cast<string>(i);
            int tmp = (int)errors[i];
            //cout << tmp << " " << output << " " << desiredOut << endl;
            if (errors[i] != 0){
                taskErrors[name]++;
            }
        }
        itIns++;
        itOuts++;
    }
    for (int i = 0; i < numTasks; i++){
        string name = "task-";
        name += lexical_cast<string>(i);
        taskErrorRate[name] = taskErrors[name] / (double)totalItems;
    }
    return taskErrorRate;
}

void BackPropTrainer::CalcWeightUpdates(LayerPtr layer, vecDouble desiredOut){
    if(layer->GetType() != Layer::input){
        NodeList& nodes = layer->GetNodes();
        switch(layer->GetType()){
            case Layer::out:
                for(int i = 0; i < layer->GetSize(); i++){
                    string name = nodes[i]->GetName();
                    double act = nodes[i]->GetOut();
					double nPrime = nodes[i]->GetPrime();
                    //double sigSteep = nodes[i]->GetSigSteepness();
                    //double error = desiredOut[i] - act;
					double error = Error(desiredOut[i], act);
                    //double delta = sigSteep * error * act * (1-act);
					double delta =  error * nPrime;
                    localGradients[name] = delta;
                    ConList cons = nodes[i]->GetConnections(IN);
                    BOOST_FOREACH(ConPtr con, cons){
                        double dw = learningRate * con->GetValue() * delta;
                        weightUpdates[con->GetName()] = dw;
                    }
                }
                break;
            case Layer::hidden:
                for(int i = 0; i < layer->GetSize(); i++){
                    string name = nodes[i]->GetName();
                    double act = nodes[i]->GetOut();
					double nPrime = nodes[i]->GetPrime();
                    //double sigSteep = nodes[i]->GetSigSteepness();
                    ConList outCons = nodes[i]->GetConnections(OUT);
                    double sumOfChildError = 0;
                    BOOST_FOREACH(ConPtr con, outCons){
                        sumOfChildError += localGradients[con->GetOutNodeName()] * con->GetWeight();
                    }
                    //double delta = sigSteep * act * (1-act) * sumOfChildError;
					double delta = nPrime * sumOfChildError;
                    localGradients[name] = delta;
                    ConList inCons = nodes[i]->GetConnections(IN);
                    BOOST_FOREACH(ConPtr con, inCons){
                        double dw = learningRate * con->GetValue() * delta;
                        weightUpdates[con->GetName()] = dw;
                    }
                }
                break;
            default:
                break;
        }
        CalcWeightUpdates(layer->GetPrevLayer());
    }
}

doubles& BackPropTrainer::GetMSERec(){
	return MSE_Rec;
}

doubles& BackPropTrainer::GetVMSERec(){
	return vMSE_Rec;
}


void BackPropTrainer::TrainToConvergence(double maxSSE, int maxEpoch){
    double curSSE = 10;
    while(curSSE > maxSSE && epochCount < maxEpoch){
        curSSE = TrainEpoch();
    }
}

void BackPropTrainer::TrainToValConv(int maxEpoch){
    double curSSE = 10;
    double bestVERR = TestOnData(Dataset::VAL);
    bool proceed = true;
    int epochsSinceImpr = 0;
    while(proceed){
        curSSE = TrainEpoch();
        double curVERR = TestOnData(Dataset::VAL);
        vMSE_Rec.push_back(curVERR);
        epochsSinceImpr++;
        if (epochCount == 1 || curVERR < bestVERR){
            bestVERR = curVERR;
            bestWeights = mNetwork->GetWeights();
            epochsSinceImpr = 0;
        }
        if (epochCount >= maxEpoch){
            mNetwork->SetWeights(bestWeights);
            proceed = false;
            continue;
        }
        if (epochsSinceImpr >= continueEpochs){
            mNetwork->SetWeights(bestWeights);
            proceed = false;
            continue;
        }

    }
}

int BackPropTrainer::GetEpochs(){
    return epochCount;
}