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
}