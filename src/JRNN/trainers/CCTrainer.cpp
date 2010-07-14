/* 
 * File:   CCTrainer.cpp
 * Author: jreeder
 * 
 * Created on June 9, 2010, 3:00 PM
 */
#include "JRNN.h"
#include "trainers/CCTrainer.h"

namespace JRNN {
	
	CCTrainer::CCTrainer(NetworkPtr network, DatasetPtr data, int numCandidates){
		parms.nTrials = 1;
		parms.maxNewUnits = 25;
		parms.valPatience = 12;

		parms.out.epochs = 200;
		parms.out.patience = 12;
		parms.out.epsilon = 1.0;
		parms.out.decay = 0.0;
		parms.out.mu = 2.0;
		parms.out.changeThreshold = 0.01;

		parms.cand.epochs = 200;
		parms.cand.patience = 12;
		parms.cand.epsilon = 100;
		parms.cand.decay = 0.0;
		parms.cand.mu = 2.0;
		parms.cand.changeThreshold = 0.03;
		parms.nCand = 8;
		out.conDeltas.clear();
		out.conPSlopes.clear();
		out.conSlopes.clear();
		cand.conDeltas.clear();
		cand.conPSlopes.clear();
		cand.conSlopes.clear();
		ResetVars();
	}

	void CCTrainer::ResetVars()
	{
		parms.out.shrinkFactor = parms.out.mu / (1.0 + parms.out.mu);
		parms.out.scaledEpsilon = parms.out.epsilon / data->GetSize(Dataset::TRAIN);
		parms.cand.shrinkFactor = parms.cand.mu / (1.0 + parms.out.mu);
	}

	CCTrainer::~CCTrainer(){}

	void CCTrainer::QuickProp( ConPtr con, conVars& vars, double epsilon, double decay, 
								double mu, double shrinkFactor )
	{
		std::string conName = con->GetName();
		double w, //weight value of con
			d, //Delta value for this con
			s, //Slope for this con
			p, //Previous slope for this con
			dw = 0.0; //The change in weight

		w = con->GetWeight();
		s = vars.conSlopes[conName];
		d = vars.conDeltas[conName];
		p = vars.conPSlopes[conName];

		if (d < 0.0){
			if (s > 0.0)
				dw -= epsilon * s;

			if (s >= (shrinkFactor * p))
				dw += mu * d;

			else
				dw += d * s / (p - s);
		}
		else if ( d > 0.0){
			if ( s < 0.0 )
				dw -= epsilon * s;
			if ( s <= (shrinkFactor * p))
				dw += mu * d;
			else
				dw += d * s / (p - s);
		}
		else
			dw -= epsilon * s; //Last step was zero

		vars.conDeltas[conName] = dw;
		(*con.get()) += dw; //Update the connection weight
		vars.conPSlopes[conName] = s;
		vars.conSlopes[conName] = 0.0;
	}

	void CCTrainer::resetError(){
		sumSqErr = 0.0;
		outSumErrs.clear();
	}

	void CCTrainer::resetOutValues(){
		out.conDeltas.clear();
		out.conPSlopes.clear();
		out.conSlopes.clear();
	}

	void CCTrainer::resetCandValues(){
		cand.conDeltas.clear();
		cand.conPSlopes.clear();
		cand.conSlopes.clear();
	}

	//void CCTrainer::CreateCandidates(){
	//	candidateLayer = Layer::CreateLayer(Layer::hidden, parms.nCand, )
	//}

}