/* 
 * File:   RPropTrainer.cpp
 * Author: jreeder
 * 
 * Created on June 7, 2010, 11:10 PM
 */
#include "JRNN.h"
#include "trainers/RPropTrainer.h"

namespace JRNN {

	const double RPropTrainer::deltaMax = 50;
	const double RPropTrainer::deltaMin = 0.000001;

	RPropTrainer::RPropTrainer(NetworkPtr network, DatasetPtr inDataSet, double etaPlus, double etaMinus) {
		mNetwork = network;
		data = inDataSet;
		etaPlus = etaPlus;
		etaMinus = etaMinus;
		epochCount = 0;
	}

	RPropTrainer::RPropTrainer(const RPropTrainer& orig) {
		mNetwork = orig.mNetwork;
		data = orig.data;
		etaMinus = orig.etaMinus;
		etaPlus = orig.etaPlus;
		epochCount = orig.epochCount;
	}

	RPropTrainer::~RPropTrainer() {
	}

	void RPropTrainer::Reset(){
		MSE_Rec.clear();
		bestWeights.clear();
		epochCount = 0;
		localGradients.clear();
		mNetwork->Reset();
		taskErrorRate.clear();
		taskErrors.clear();
		weightUpdates.clear();
		vMSE_Rec.clear();
		dw.clear();
		dw_last.clear();
		delta.clear();
		delta_last.clear();
	}

	double RPropTrainer::TrainEpoch(){
		matDouble trainingIns = data->GetInputs(Dataset::TRAIN);
		matDouble trainingOuts = data->GetOutputs(Dataset::TRAIN);
		matDouble::iterator itData;
		matDouble::iterator itOut;
	//    itData = trainingIns.begin();
	//    itOut = trainingOuts.begin();
		itData = trainingIns.begin();
		itOut = trainingOuts.begin();
		double SSE = 0;
		while(itData != trainingIns.end()){
			vecDouble input = (*itData);
			vecDouble desiredOut = (*itOut);
			mNetwork->Activate(input);
			vecDouble output = mNetwork->GetOutputs();
		   // cout << "network output: " << output << " Desired Out: " << desiredOut << endl;
			vecDouble error = desiredOut - output;
			vecDouble sqError = SquareVec(error);
			SSE += ublas::sum(sqError);
			//weightUpdates.clear();
			localGradients.clear();
			CalcWeightUpdates(mNetwork->GetLayer("out"), desiredOut);
			itData++;
			itOut++;
		}
		ConList consToUpdate = mNetwork->GetConnections();
		BOOST_FOREACH(ConPtr con, consToUpdate){
			RPropUpdate(con);
			double tmp = weightUpdates[con->GetName()];
			//cout << "weight update: " << tmp << endl;
			(*con.get()) += weightUpdates[con->GetName()];
		}
		dw.clear();
		epochCount++;
		SSE /= (double)trainingIns.size();
		MSE_Rec.push_back(SSE);
		return SSE;
	}

	double RPropTrainer::TestOnData(Dataset::datatype type){
		matDouble ins = data->GetInputs(type);
		matDouble outs = data->GetOutputs(type);
		matDouble::iterator itIns = ins.begin();
		matDouble::iterator itOuts = outs.begin();

		double SSE = 0;
		while(itIns != ins.end()){
			vecDouble input = (*itIns);
			vecDouble desiredOut = (*itOuts);
			mNetwork->Activate(input);
			vecDouble output = mNetwork->GetOutputs();
			vecDouble error = desiredOut - output;
			vecDouble sqError = SquareVec(error);
			SSE += ublas::sum(sqError);
			itIns++;
			itOuts++;
		}
		SSE /= (double)ins.size();
		return SSE;
	}

	hashedDoubleMap RPropTrainer::TestWiClass(Dataset::datatype type){
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
				std::string name = "task-";
				name += lexical_cast<std::string>(i);
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
			std::string name = "task-";
			name += lexical_cast<std::string>(i);
			taskErrorRate[name] = taskErrors[name] / (double)totalItems;
		}
		return taskErrorRate;
	}

	void RPropTrainer::CalcWeightUpdates(LayerPtr layer, vecDouble desiredOut){
		if(layer->GetType() != Layer::input){
			NodeList& nodes = layer->GetNodes();
			switch(layer->GetType()){
				case Layer::out:
					for(int i = 0; i < layer->GetSize(); i++){
						std::string name = nodes[i]->GetName();
						double act = nodes[i]->GetOut();
						double sigSteep = nodes[i]->GetSigSteepness();
						double error = desiredOut[i] - act;
						double delta = sigSteep * error * act * (1-act);
						localGradients[name] = delta;
						ConList cons = nodes[i]->GetConnections(Node::IN);
						BOOST_FOREACH(ConPtr con, cons){
							double dwtmp = con->GetValue() * delta;
							dw[con->GetName()] += dwtmp;
						}
					}
					break;
				case Layer::hidden:
					for(int i = 0; i < layer->GetSize(); i++){
						std::string name = nodes[i]->GetName();
						double act = nodes[i]->GetOut();
						double sigSteep = nodes[i]->GetSigSteepness();
						ConList outCons = nodes[i]->GetConnections(Node::OUT);
						double sumOfChildError = 0;
						BOOST_FOREACH(ConPtr con, outCons){
							sumOfChildError += localGradients[con->GetOutNodeName()] * con->GetWeight();
						}
						double delta = sigSteep * act * (1-act) * sumOfChildError;
						localGradients[name] = delta;
						ConList inCons = nodes[i]->GetConnections(Node::IN);
						BOOST_FOREACH(ConPtr con, inCons){
							double dwtmp = con->GetValue() * delta;
							dw[con->GetName()] += dwtmp;
						}
					}
					break;
				default:
					break;
			}
			CalcWeightUpdates(layer->GetPrevLayer());
		}
	}

	void RPropTrainer::RPropUpdate(ConPtr con){
		std::string conName = con->GetName();
		double tmpMax = deltaMax;
		double tmpMin = deltaMin;
		if (dw_last[conName] * dw[conName] > 0){
			delta[conName] = std::min(delta_last[conName] * etaPlus, tmpMax);
			weightUpdates[conName] = Sign(dw[conName]) * delta[conName];
			delta_last[conName] = delta[conName];
			dw_last[conName] = dw[conName];
		}
		else if (dw_last[conName] * dw[conName] < 0){
			delta[conName] = std::max(delta_last[conName] * etaMinus, tmpMin);
			//weightUpdates[conName] = -sign(dw_last[conName]) * delta_last[conName];
			weightUpdates[conName] = 0;
			dw_last[conName] = 0;
			delta_last[conName] = delta[conName];
		}
		else {
			delta[conName] = delta[conName] == 0 ? 1 : delta[conName];
			weightUpdates[conName] = Sign(dw[conName]) * delta[conName];
			dw_last[conName] = dw[conName];
			delta_last[conName] = delta[conName];
		}
	}

	doubles& RPropTrainer::GetMSERec(){
		return MSE_Rec;
	}

	void RPropTrainer::TrainToConvergence(double maxSSE, int maxEpoch){
		double curSSE = 10;
		while(curSSE > maxSSE && epochCount < maxEpoch){
			curSSE = TrainEpoch();
			//cout << curSSE << endl;
		}
	}

	void RPropTrainer::TrainToValConv(int maxEpoch){
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

	doubles& RPropTrainer::GetVMSERec(){
		return vMSE_Rec;
	}


	int RPropTrainer::GetEpochs(){
		return epochCount;
	}
}