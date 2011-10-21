/********************************************************************
	created:	2011/10/20
	created:	20:10:2011   18:15
	filename: 	EtaMTLRPropTrainer.cpp
	author:		jreeder
	
	purpose:	Eta MTL adapted to RProp
*********************************************************************/

#include "JRNN.h"
#include "trainers/EtaMTLRPropTrainer.h"

namespace JRNN {

	double EtaMTLRPropTrainer::TrainEpoch(){
		matDouble trainingIns = data->GetInputs(Dataset::TRAIN);
		matDouble trainingOuts = data->GetOutputs(Dataset::TRAIN);
		matDouble::iterator itData;
		matDouble::iterator itOut;
		//    itData = trainingIns.begin();
		//    itOut = trainingOuts.begin();
		itData = trainingIns.begin();
		itOut = trainingOuts.begin();
		double MSE = 0;
		FillVec(outSSEs, 0.0);
		while(itData != trainingIns.end()){
			vecDouble input = (*itData);
			vecDouble desiredOut = (*itOut);
			mNetwork->Activate(input);
			vecDouble output = mNetwork->GetOutputs();
			// cout << "network output: " << output << " Desired Out: " << desiredOut << endl;
			//vecDouble error = desiredOut - output;
			vecDouble error = Error(desiredOut, output);
			vecDouble sqError = SquareVec(error);
			outSSEs += sqError;
			MSE += ublas::sum(FilterError(sqError, primaryIndexes));
			//weightUpdates.clear();
			localGradients.clear();
			CalcWeightUpdates(mNetwork->GetLayer("out"), desiredOut);
			itData++;
			itOut++;
		}
		ConMap consToUpdate = mNetwork->GetConnections();
		BOOST_FOREACH(ConPair conp, consToUpdate){
			ConPtr con = conp.second;
			RPropUpdate(con);
			double tmp = weightUpdates[con->GetName()];
			//cout << "weight update: " << tmp << endl;
			(*con.get()) += weightUpdates[con->GetName()];
			if (useMaxWeight && con->GetWeight() > maxWeight){
				con->Reset();
				delta[con->GetName()] = 1;
				dw_last[con->GetName()] = 0;
				numResets++;
			}
		}
		dw.clear();
		epochCount++;
		CalcRks(mNetwork->GetLayer("out")->GetNodes());
		MSE /= (double)nTrainOutVals;
		//MSE /= (double)mNetwork->GetNumOut();
		MSE_Rec.push_back(MSE);
		return MSE;
	}

	void EtaMTLRPropTrainer::CalcWeightUpdates(LayerPtr layer, vecDouble desiredOut){
		if(layer->GetType() != Layer::input){
			NodeList& nodes = layer->GetNodes();
			switch(layer->GetType()){
			case Layer::out:
				for(int i = 0; i < layer->GetSize(); i++){
					string name = nodes[i]->GetName();
					double act = nodes[i]->GetOut();
					double nPrime = nodes[i]->GetPrime() + primeOffset;
					double etaK = Rks[i] * globalLearningRate; //Eta MTL Stuff.
					//double sigSteep = nodes[i]->GetSigSteepness();
					//double error = desiredOut[i] - act;
					double error = Error(desiredOut[i], act);
					//double delta = sigSteep * error * act * (1-act);
					double delta = error * nPrime * etaK; // Eta MTL stuff. I can't think of any other way for this to work in RProp. 
					localGradients[name] = delta;
					ConList cons = nodes[i]->GetConnections(IN);
					BOOST_FOREACH(ConPtr con, cons){
						double dwtmp = con->GetValue() * delta;
						dw[con->GetName()] += dwtmp;
					}
				}
				break;
			case Layer::hidden:
				for(int i = 0; i < layer->GetSize(); i++){
					string name = nodes[i]->GetName();
					double act = nodes[i]->GetOut();
					double nPrime = nodes[i]->GetPrime() + primeOffset;
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

	EtaMTLRPropTrainer::EtaMTLRPropTrainer( FFMLPNetPtr network, DatasetPtr inDataSet, double etaPlus, double etaMinus, ints primaryIndexes /*= ints(0)*/ )
		: RPropTrainer(network, inDataSet, etaPlus, etaMinus, primaryIndexes)
	{
		globalLearningRate = 1.0;
		FillVec(outSSEs, 0.0);
	}

	void EtaMTLRPropTrainer::Reset()
	{
		FillVec(outSSEs, 0.0);
		FillVec(Rks, 1.0);
		RPropTrainer::Reset();
	}

}