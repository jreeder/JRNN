/* 
 * File:   CCTrainer.h
 * Author: jreeder
 * 
 * Created on June 9, 2010, 3:00 PM
 */
#include "JRNN.h"
#include "trainers/CCTrainer.h"

namespace JRNN {
	
	CCTrainer::CCTrainer(NetworkPtr network, DatasetPtr data, int numCandidates){

	}
	CCTrainer::~CCTrainer(){}

	void CCTrainer::QuickProp( LayerPtr layer, vecDouble desiredOut )
	{

	}

	void CCTrainer::TrainOuts()
	{

	}

	void CCTrainer::TrainCandidates()
	{

	}

	double CCTrainer::CalcCorrelation()
	{
		double result = 0;
		return result;
	}

}