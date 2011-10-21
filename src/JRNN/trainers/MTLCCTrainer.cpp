/********************************************************************
	created:	2011/06/07
	created:	7:6:2011   17:58
	filename:	MTLCCTrainer.cpp
	author:		jreeder
	
	purpose:	MTL specific CC Trainer
*********************************************************************/

#include "JRNN.h"
#include "trainers/MTLCCTrainer.h"

namespace JRNN {
	MTLCCTrainer::MTLCCTrainer(CCNetworkPtr network, DatasetPtr data, int numCandidates, ints primaryIndexes /* = ints */)
		: CCTrainer(network, data, numCandidates, primaryIndexes)
	{
		mtlparams.primaryWeight = 1.0;
		mtlparams.secondaryWeight = 0.5;
		mtlparams.weightCandScore = true;
	}

	MTLCCTrainer::~MTLCCTrainer(){}

	void MTLCCTrainer::UpdateCorrelations(){
		double avgValue;
		double score;
		double cor;
		vecDouble *curCorr;
		vecDouble *prevCorr;
		bool usePrimary = (primaryIndexes.size() > 0) ? true : false;
		candBestScore = 0.0;
		bestCand.reset();
		int nTrainPts = data->GetSize(Dataset::TRAIN);
		int nOuts = network->GetNumOut();
		NodeList candNodes = network->GetCandLayer()->GetNodes();
		BOOST_FOREACH(NodePtr node, candNodes){
			string name = node->GetName();
			avgValue = candSumVals[name] / nTrainPts;
			score = 0.0;
			curCorr = &candCorr[name];
			prevCorr = &candPCorr[name];

			assert((*curCorr).size() == nOuts);
			for (int j = 0; j < nOuts; j++){
				/*double tmp1 = (*curCorr)[j]; //Used for Debugging. 
				double tmp2 = err.sumErrs[j];*/
				cor = ((*curCorr)[j] - avgValue * err.sumErrs[j]) / err.sumSqErr;
				(*prevCorr)[j] = cor;
				(*curCorr)[j] = 0.0;
				if (usePrimary && mtlparams.weightCandScore){
					bool prim = InVector(primaryIndexes, j);
					double weight = prim ? mtlparams.primaryWeight : mtlparams.secondaryWeight;
					score += (fabs (cor) * weight);
				}
				else {
					score += fabs (cor);
				}
			}

			//TODO Need to add code here to weight the scores.
			//Positively for primary task and tending to zero for unrelated tasks.

			candSumVals[name] = 0.0;

			//check for best candidate

			if ( score > candBestScore ){
				candBestScore = score;
				bestCand = node;
			}
		}
		if (bestCand.get() == 0){
			bestCand = candNodes[0];
		}
	}

	void MTLCCTrainer::ComputeCandSlopes()
	{
		double	change,
			actPrime,
			error,
			value,
			direction;

		vecDouble	*cCorr,
			*cPCorr;

		NodeList candNodes = network->GetCandLayer()->GetNodes();
		network->GetCandLayer()->Activate();
		int nOuts = network->GetNumOut();
		BOOST_FOREACH(NodePtr candidate, candNodes){
			string name = candidate->GetName();
			value = candidate->GetOut();
			actPrime = candidate->GetPrime();
			change = 0.0;
			cCorr = &candCorr[name];
			cPCorr = &candPCorr[name];

			candSumVals[name] += value;
			actPrime /= err.sumSqErr; //This is a slight modification to the original. 
			//It's equivalent, but I'm not sure why I did this maybe optimizing. 

			//TODO need to look into making changes here for eta MTL style focusing. 

			//compute correlations to each output
			for (int j = 0; j < nOuts; j++){
				error = err.errors[j];
				direction = ((*cPCorr)[j] < 0.0) ? -1.0 : 1.0;
				change -= direction * actPrime * (error - err.sumErrs[j]); //Probably could add eta mtl change here if need be. 
				(*cCorr)[j] += error * value;
			}

			//use change to compute new slopes

			ConList cons = candidate->GetConnections(IN);
			BOOST_FOREACH(ConPtr con, cons){
				cand.conSlopes[con->GetName()] += change * con->GetValue();
			}
		}

	}
}