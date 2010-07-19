/* 
 * File:   CCTrainer.cpp
 * Author: jreeder
 * 
 * Created on June 9, 2010, 3:00 PM
 */
#include "JRNN.h"
#include "trainers/CCTrainer.h"

namespace JRNN {
	
	CCTrainer::CCTrainer(CCNetworkPtr network, DatasetPtr data, int numCandidates){
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
		parms.sseThreshold = 0.2;
		parms.candScoreThreshold = 0.4;
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
		epoch = 0;
		parms.out.shrinkFactor = parms.out.mu / (1.0 + parms.out.mu);
		parms.out.scaledEpsilon = parms.out.epsilon / data->GetSize(Dataset::TRAIN);
		parms.cand.shrinkFactor = parms.cand.mu / (1.0 + parms.out.mu);
		resetCandValues();
		resetOutValues();
		resetError();
		taskErrors.clear();
		taskErrorRate.clear();
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
		trueErr = 0.0;
		sumSqErr = 0.0;
		err.sumErrs.clear();
		err.errors.clear();
		//outSumErrs.clear();
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
		candSumVals.clear();
		candCorr.clear();
		candPCorr.clear();
		candBestScore = 0.0;
	}

	void CCTrainer::CreateCandidates()
	{
		network->CreateCandLayer(parms.nCand);
		resetCandValues();
	}

	CCTrainer::status CCTrainer::TrainOuts()
	{
		int quitEpoch = 0;
		double lastError = 0.0;
		for (int i = 0; i < parms.out.epochs; i++){
			resetError();
			OutputEpoch();
			if (trueErr < parms.sseThreshold){
				return CCTrainer::WIN;
			}
			UpdateOutWeights();
			epoch++;

			if (i == 0){
				lastError = trueErr;
			}
			else if (fabs(trueErr - lastError) > lastError * parms.out.changeThreshold){
				lastError = trueErr;
				quitEpoch = epoch + parms.out.patience;
			}
			else if (epoch == quitEpoch){
				return CCTrainer::STAGNANT;
			}
		}
		return CCTrainer::TIMEOUT;
	}

	void CCTrainer::OutputEpoch()
	{
		matDouble ins = data->GetInputs(Dataset::TRAIN);
		matDouble outs = data->GetOutputs(Dataset::TRAIN);
		matDouble::iterator itIns = ins.begin();
		matDouble::iterator itOuts = outs.begin();
		NodeList outNodes = network->GetLayer("out")->GetNodes();
		//double SSE = 0;
		while(itIns != ins.end()){
			vecDouble input = (*itIns);
			vecDouble desiredOut = (*itOuts);
			network->Activate(input);
			ComputeError(desiredOut, err, outNodes, true, true);


			itIns++;
			itOuts++;
		}
		//trueErr /= (double)ins.size(); //Might need this later
		//return SSE;
	}

	void CCTrainer::UpdateOutWeights()
	{
		NodeList outNodes = network->GetLayer("out")->GetNodes();
		BOOST_FOREACH(NodePtr node, outNodes){
			ConList cons = node->GetConnections(IN);
			BOOST_FOREACH(ConPtr con, cons){
				QuickProp(con,out,parms.out.scaledEpsilon,parms.out.decay,
							parms.out.mu,parms.out.shrinkFactor);
			}
		}
	}

	CCTrainer::status CCTrainer::TrainCandidates()
	{
		double lastScore = 0.0;
		int quitEpoch = 0;
		err.sumErrs /= data->GetSize(Dataset::TRAIN);

		CorrelationEpoch();

		for (int i = 0; i < parms.cand.epochs; i ++){
			CandEpoch();

			UpdateCandWeights();

			UpdateCorrelations();

			epoch++;

			if ( i == 0 ){
				lastScore = candBestScore;
			}
			else if (fabs(candBestScore - lastScore) > (lastScore * parms.cand.changeThreshold)){
				quitEpoch = epoch + parms.cand.patience;
				lastScore = candBestScore;
			}
			else if (epoch == quitEpoch){
				return STAGNANT;
			}
		}
		return TIMEOUT;
	}

	void CCTrainer::CorrelationEpoch()
	{
		matDouble ins = data->GetInputs(Dataset::TRAIN);
		matDouble outs = data->GetOutputs(Dataset::TRAIN);
		matDouble::iterator itIns = ins.begin();
		matDouble::iterator itOuts = outs.begin();
		NodeList outNodes = network->GetLayer("out")->GetNodes();
		//double SSE = 0;
		while(itIns != ins.end()){
			vecDouble input = (*itIns);
			vecDouble desiredOut = (*itOuts);
			network->Activate(input);
			ComputeError(desiredOut, err, outNodes, false, false);

			ComputeCorrelations();

			itIns++;
			itOuts++;
		}

		UpdateCorrelations();
		epoch++;
	}

	void CCTrainer::ComputeError( vecDouble desiredOut, errVars& errs, NodeList &outNodes, bool alterStats, bool updateSlopes)
	{
		//Compute Errors
		vecDouble output = network->GetOutputs();
		vecDouble error = desiredOut - output;
		vecDouble outPrimes = network->GetPrimes(std::string("out"));
		vecDouble errPrimes = VecMultiply(error, outPrimes);
		vecDouble sqError = SquareVec(error);
		errs.errors = errPrimes;

		//Alter Stats
		if (alterStats){
			errs.sumErrs += errPrimes;
			sumSqErr += ublas::sum(SquareVec(errPrimes));
			trueErr += ublas::sum(sqError);
		}
		//Update Slopes
		if (updateSlopes){
			assert(outNodes.size() == errPrimes.size());
			for (unsigned int i = 0; i < outNodes.size(); i++){
				ConList cons = outNodes[i]->GetConnections(IN);
				BOOST_FOREACH(ConPtr con, cons){
					out.conSlopes[con->GetName()] += errPrimes[i] * con->GetValue();
				}
			}
		}
	}

	void CCTrainer::ComputeCorrelations()
	{
		double sum,val;
		vecDouble* cCorr;
		NodeList candNodes = network->GetCandLayer()->GetNodes();
		network->GetCandLayer()->Activate();
		BOOST_FOREACH(NodePtr node, candNodes){
			sum = 0.0;
			std::string name = node->GetName();
			cCorr = &candCorr[name];
			val = node->GetOut();
			candSumVals[name] += val;

			//computer correlation for this unit
			for (unsigned int j = 0; j < err.errors.size(); j++ ){
				(*cCorr)[j] += val * err.errors[j];
			}
		}
	}

	void CCTrainer::UpdateCorrelations()
	{
		double avgValue;
		double score;
		double cor;
		vecDouble *curCorr;
		vecDouble *prevCorr;

		candBestScore = 0.0;
		bestCand.reset();
		int nTrainPts = data->GetSize(Dataset::TRAIN);
		int nOuts = network->GetNumOut();
		NodeList candNodes = network->GetCandLayer()->GetNodes();
		BOOST_FOREACH(NodePtr node, candNodes){
			std::string name = node->GetName();
			avgValue = candSumVals[name] / nTrainPts;
			score = 0.0;
			curCorr = &candCorr[name];
			prevCorr = &candPCorr[name];

			assert((*curCorr).size() == nOuts);
			for (int j = 0; j < nOuts; j++){
				cor = ((*curCorr)[j] - avgValue * err.sumErrs[j]) / sumSqErr;
				(*prevCorr)[j] = cor;
				(*curCorr)[j] = 0.0;
				score += fabs (cor);
			}

			candSumVals[name] = 0.0;

			//check for best candidate

			if ( score > candBestScore ){
				candBestScore = score;
				bestCand = node;
			}
		}
	}

	void CCTrainer::CandEpoch()
	{
		matDouble ins = data->GetInputs(Dataset::TRAIN);
		matDouble outs = data->GetOutputs(Dataset::TRAIN);
		matDouble::iterator itIns = ins.begin();
		matDouble::iterator itOuts = outs.begin();
		NodeList outNodes = network->GetLayer("out")->GetNodes();
		//double SSE = 0;
		while(itIns != ins.end()){
			vecDouble input = (*itIns);
			vecDouble desiredOut = (*itOuts);
			network->Activate(input);
			ComputeError(desiredOut, err, outNodes, false, false);

			ComputeCandSlopes();

			itIns++;
			itOuts++;
		}
	}

	void CCTrainer::ComputeCandSlopes()
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
			std::string name = candidate->GetName();
			value = candidate->GetOut();
			actPrime = candidate->GetPrime();
			change = 0.0;
			cCorr = &candCorr[name];
			cPCorr = &candPCorr[name];

			candSumVals[name] += value;
			actPrime /= sumSqErr;

			//compute correlations to each output
			for (int j = 0; j < nOuts; j++){
				error = err.errors[j];
				direction = ((*cPCorr)[j] < 0.0) ? -1.0 : 1.0;
				change -= direction * actPrime * (error - err.sumErrs[j]);
				(*cCorr)[j] += error * value;
			}

			//use change to compute new slopes

			ConList cons = candidate->GetConnections(IN);
			BOOST_FOREACH(ConPtr con, cons){
				cand.conSlopes[con->GetName()] += change * con->GetValue();
			}
		}

	}

	void CCTrainer::UpdateCandWeights()
	{
		double scaledEpsilon;
		
		scaledEpsilon = parms.cand.epsilon / (double) (data->GetSize(Dataset::TRAIN) * network->GetNumUnits());
		NodeList candNodes = network->GetCandLayer()->GetNodes();
		BOOST_FOREACH(NodePtr node, candNodes){
			ConList cons = node->GetConnections(IN);
			BOOST_FOREACH(ConPtr con, cons){
				QuickProp(con,cand,scaledEpsilon,parms.cand.decay,
							parms.cand.mu,parms.cand.shrinkFactor);
			}
		}
	}

	//void CCTrainer::CreateCandidates(){
	//	candidateLayer = Layer::CreateLayer(Layer::hidden, parms.nCand, )
	//}

	hashedDoubleMap CCTrainer::TestWiClass(Dataset::datatype type){
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
			network->Activate(input);
			vecDouble output = network->GetOutputs();
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

	double CCTrainer::TestOnData( Dataset::datatype type )
	{
		matDouble ins = data->GetInputs(type);
		matDouble outs = data->GetOutputs(type);
		matDouble::iterator itIns = ins.begin();
		matDouble::iterator itOuts = outs.begin();

		double SSE = 0;
		while(itIns != ins.end()){
			vecDouble input = (*itIns);
			vecDouble desiredOut = (*itOuts);
			network->Activate(input);
			vecDouble output = network->GetOutputs();
			vecDouble error = desiredOut - output;
			vecDouble sqError = SquareVec(error);
			SSE += ublas::sum(sqError);
			itIns++;
			itOuts++;
		}
		SSE /= (double)ins.size();
		return SSE;
	}

}