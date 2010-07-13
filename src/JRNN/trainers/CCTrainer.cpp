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
		parms.nTrials = 1;
		parms.maxNewUnites = 25;
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
	}

	CCTrainer::~CCTrainer(){}

	void CCTrainer::QuickProp( ConPtr con, double epsilon, double decay, 
								double mu, double shrinkFactor )
	{
		std::string conName = con->GetName();
		double w, //weight value of con
			d, //Delta value for this con
			s, //Slope for this con
			p, //Previous slope for this con
			dw = 0.0; //The change in weight

		w = con->GetWeight();
		s = conSlopes[conName];
		d = conDeltas[conName];
		p = conPSlopes[conName];

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

		conDeltas[conName] = dw;
		(*con.get()) += dw; //Update the connection weight
		conPSlopes[conName] = s;
		conSlopes[conName] = 0.0;
	}

	void CCTrainer::TrainOuts()
	{

	}

	void CCTrainer::TrainCandidates()
	{

	}

}