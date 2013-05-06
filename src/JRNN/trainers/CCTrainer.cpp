/* 
 * File:   CCTrainer.cpp
 * Author: jreeder
 * 
 * Created on June 9, 2010, 3:00 PM
 */
#include "JRNN.h"
#include "trainers/CCTrainer.h"

namespace JRNN {
	
	CCTrainer::CCTrainer(CCNetworkPtr network, DatasetPtr data, int numCandidates, ints primaryIndexes){
		this->network = network;
		this->data = data;
		this->primaryIndexes = primaryIndexes;
		resetFlag = false;
		numResets = 0;
		this->useTrainOutVal = true;
		this->useValidation = false;
		if(primaryIndexes.size() > 0){
			nTrainOutVals = data->GetSize(Dataset::TRAIN) * primaryIndexes.size();
		}
		else {
			nTrainOutVals = data->GetSize(Dataset::TRAIN) * network->GetNumOut();
		}
		
		parms.nTrials = 1;//Not used
		parms.maxNewUnits = 25;//Not used
		parms.valPatience = 2;
		parms.impPatience = 2;
		parms.weightMult = 1.0;
		parms.maxWeight = 10000;
		parms.useMaxWeight = true;
		parms.maxResets = 10; //Not used right now
		parms.primeOffset = 0.1;
		parms.useSDCC = false;
		parms.SDCCRatio = 0.8;

		parms.out.epochs = 300; //200
		parms.out.patience = 12;//12
		parms.out.epsilon = 10.0;//1.0
		parms.out.decay = 0.0;//0.001
		parms.out.mu = 2.0;
		parms.out.changeThreshold = 0.02; //0.01

		parms.cand.epochs = 200; //200
		parms.cand.patience = 12; //12
		parms.cand.epsilon = 100;
		parms.cand.decay = 0.0;//0.001
		parms.cand.mu = 2.0;
		parms.cand.changeThreshold = 0.03;
		
		parms.nCand = numCandidates;
		parms.indexThreshold = 0.05; //0.1 Index is the Sqrt of MSE so Mean Err
		parms.scoreThreshold = 0.4;
		parms.errorMeasure = INDEX;
		out.conDeltas.clear();
		out.conPSlopes.clear();
		out.conSlopes.clear();
		cand.conDeltas.clear();
		cand.conPSlopes.clear();
		cand.conSlopes.clear();

		int numOut = network->GetNumOut();
		err.errors = vecDouble(numOut);
		err.sumErrs = vecDouble(numOut);
		err.measure = (ErrorType) parms.errorMeasure;
		valErr.errors = vecDouble(numOut);
		valErr.sumErrs = vecDouble(numOut);
		valErr.measure = (ErrorType) parms.errorMeasure;
		candCorr = hashedVecDoubleMap(numOut);
		candPCorr = hashedVecDoubleMap(numOut);

		networkCache = hashedVecDoubleMap(numOut);
		useNetCache = false;
		ResetVars();
	}

	//Only meant to be used by subclasses. 
	CCTrainer::CCTrainer(){
		this->primaryIndexes = ints(0);
		resetFlag = false;
		numResets = 0;
		this->useTrainOutVal = true;
		this->useValidation = false;
		//Need to set this value in the subclass also. 
		//if(primaryIndexes.size() > 0){
		//	nTrainOutVals = data->GetSize(Dataset::TRAIN) * primaryIndexes.size();
		//}
		//else {
		//	nTrainOutVals = data->GetSize(Dataset::TRAIN) * network->GetNumOut();
		//}

		parms.nTrials = 1;//Not used
		parms.maxNewUnits = 25;//Not used
		parms.valPatience = 2;
		parms.impPatience = 2;
		parms.weightMult = 1.0;
		parms.maxWeight = 10000;
		parms.useMaxWeight = true;
		parms.maxResets = 10; //Not used right now
		parms.primeOffset = 0.1;
		parms.useSDCC = false;
		parms.SDCCRatio = 0.8;

		parms.out.epochs = 300; //200
		parms.out.patience = 12;//12
		parms.out.epsilon = 10.0;//1.0
		parms.out.decay = 0.0;//0.001
		parms.out.mu = 2.0;
		parms.out.changeThreshold = 0.02; //0.01

		parms.cand.epochs = 200; //200
		parms.cand.patience = 12; //12
		parms.cand.epsilon = 100;
		parms.cand.decay = 0.0;//0.001
		parms.cand.mu = 2.0;
		parms.cand.changeThreshold = 0.03;

		parms.nCand = 8; //This is normally set by the constructor. Need to change this if I subclass.
		parms.indexThreshold = 0.05; //0.1 Index is the Sqrt of MSE so Mean Err
		parms.scoreThreshold = 0.4;
		parms.errorMeasure = INDEX;
		out.conDeltas.clear();
		out.conPSlopes.clear();
		out.conSlopes.clear();
		cand.conDeltas.clear();
		cand.conPSlopes.clear();
		cand.conSlopes.clear();

		useNetCache = false;
		//Need to do this in the subclass when I know how many outs there will be. 
		//and what the data will be. 
		/*int numOut = network->GetNumOut();
		err.errors = vecDouble(numOut);
		err.sumErrs = vecDouble(numOut);
		err.measure = (ErrorType) parms.errorMeasure;
		valErr.errors = vecDouble(numOut);
		valErr.sumErrs = vecDouble(numOut);
		valErr.measure = (ErrorType) parms.errorMeasure;
		candCorr = hashedVecDoubleMap(numOut);
		candPCorr = hashedVecDoubleMap(numOut);
		ResetVars();*/
	}

	CCTrainer::~CCTrainer(){}

	void CCTrainer::QuickProp( ConPtr con, conVars& vars, double epsilon, double decay, 
								double mu, double shrinkFactor )
	{
		string conName = con->GetName();
		double w, //weight value of con
			d, //Delta value for this con
			s, //Slope for this con
			p, //Previous slope for this con
			dw = 0.0; //The change in weight

		w = con->GetWeight();
		s = vars.conSlopes[conName] + decay * w;
		d = vars.conDeltas[conName];
		p = vars.conPSlopes[conName];

		 /* The step must always be in direction opposite to the slope. */

		if (d < 0.0){ /* If last step was negative...  */  
			if (s > 0.0) 
				dw -= epsilon * s; /*  Add in linear term if current slope is still positive.*/

			if (s >= (shrinkFactor * p))/*If current slope is close to or larger than prev slope...  */
				dw += mu * d; /* Take maximum size negative step. */

			else
				dw += d * s / (p - s); /* Else, use quadratic estimate. */
		}
		else if ( d > 0.0){ /* If last step was positive...  */
			if ( s < 0.0 ) 
				dw -= epsilon * s; /*  Add in linear term if current slope is still negative.*/
			if ( s <= (shrinkFactor * p))  /* If current slope is close to or more neg than prev slope... */
				dw += mu * d; /* Take maximum size positive step. */
			else
				dw += d * s / (p - s); /* Else, use quadratic estimate. */
		}
		else
			dw -= epsilon * s; //Last step was zero

		vars.conDeltas[conName] = dw;
		(*con.get()) += dw; //Update the connection weight
		vars.conPSlopes[conName] = s;
		vars.conSlopes[conName] = 0.0;
		if (parms.useMaxWeight){
			if (con->GetWeight() > parms.maxWeight){
				//resetFlag = true;
				numResets++;
				con->Reset();
				vars.conDeltas[conName] = 0.0;
				vars.conPSlopes[conName] = 0.0;
			}
		}
	}

	void CCTrainer::resetError(errVars& errorVars){
		errorVars.bits = 0;
		errorVars.trueErr = 0.0;
		errorVars.sumSqErr = 0.0;
		FillVec(errorVars.sumErrs,0);
		FillVec(errorVars.errors,0);
		//outSumErrs.clear();
	}

	void CCTrainer::resetOutValues(){
		out.conDeltas.clear();
		out.conPSlopes.clear();
		out.conSlopes.clear();
		out.localGradients.clear();
	}

	void CCTrainer::resetCandValues(){
		cand.conDeltas.clear();
		cand.conPSlopes.clear();
		cand.conSlopes.clear();
		cand.localGradients.clear();
		candSumVals.clear();
		candCorr.clear();
		candPCorr.clear();
		candBestScore = 0.0;
	}

	void CCTrainer::resetUpdateValues()
	{
		net.conDeltas.clear();
		net.conPSlopes.clear();
		net.conSlopes.clear();
		net.localGradients.clear();
	}

	void CCTrainer::Reset()
	{
		ResetVars();
		network->Reset();
	}

	void CCTrainer::RedistData(){
		data->RedistData();
	}

	void CCTrainer::ResetVars()
	{
		epoch = 0;
		numResets = 0;
		parms.out.shrinkFactor = parms.out.mu / (1.0 + parms.out.mu);
		parms.out.scaledEpsilon = parms.out.epsilon / data->GetSize(Dataset::TRAIN);
		parms.cand.shrinkFactor = parms.cand.mu / (1.0 + parms.out.mu);
		resetCandValues();
		resetOutValues();
		resetError(err);
		resetError(valErr);
		taskErrors.clear();
		taskErrorRate.clear();
		val.bestErr = -1.0;
		val.bestWeights.clear();
		val.bestPass = network->GetNumUnits();
		imp.bestErr = -1.0;
		imp.bestWeights.clear();
		imp.bestPass = network->GetNumUnits();
		vMSE_Rec.clear();
		MSE_Rec.clear();
		ClearCache();
		useNetCache = false;
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
		int startEpoch = epoch;
		double bestVERR = 0;
		hashedDoubleMap bestWeights;
		if (useTrainOutVal && useValidation)
			bestVERR = TestOnData(Dataset::VAL);
		//resetOutValues();
		for (int i = 0; i < parms.out.epochs; i++){
			resetError(err);
			
			OutputEpoch();

			if ((err.measure == BITS) && (err.bits == 0)) {
				return CCTrainer::WIN;
			}
			else if (err.measure == INDEX) {
				double index = ErrorIndex(err.trueErr,1,nTrainOutVals);
				if (index < parms.indexThreshold){
					return CCTrainer::WIN;
				}
			}

			UpdateOutWeights();
			epoch++;

			if (resetFlag == true){ //this is not used currently. 
				epoch = startEpoch;
				i = 0;
				quitEpoch = 0;
				lastError = 0.0;
				resetTrainOuts();
				resetFlag = false;
#ifdef _DEBUG
				cout << "Reset Train Outs" << endl;
#endif // _DEBUG
				continue;
			}

			if (useTrainOutVal && useValidation){ //this changes the condition for stagnation to performance on validation set. 
				double curVERR = TestOnData(Dataset::VAL);
				if (i == 0 || curVERR < bestVERR){
					bestVERR = curVERR;
					bestWeights = network->GetWeights();
					quitEpoch = epoch + parms.out.patience;
				}
				else if (epoch == quitEpoch){
					network->SetWeights(bestWeights);
					return CCTrainer::STAGNANT;
				}
			}
			else {
				/*if (i == 0){ This was redundant. 
					lastError = err.trueErr;
					quitEpoch = epoch + parms.out.patience;
				}*/
				if (i == 0 || fabs(err.trueErr - lastError) > lastError * parms.out.changeThreshold){
					lastError = err.trueErr;
					quitEpoch = epoch + parms.out.patience;
				}
				else if (epoch == quitEpoch){
					return CCTrainer::STAGNANT;
				}
			}
		}
		if (useTrainOutVal && useValidation)
			network->SetWeights(bestWeights);
		return CCTrainer::TIMEOUT;
	}

	CCTrainer::status CCTrainer::UpdateNet()
	{
		int quitEpoch = 0;
		double lastError = 0.0;
		int startEpoch = epoch;
		bool validate = false;
		//automatically validate if the dataset has a validation set. 
		//might make this explicit later but it serves the purpose right now. 
		if (data->GetSize(Dataset::VAL) > 0)
			validate = true;

		double bestVERR = 0;
		hashedDoubleMap bestWeights;
		if (validate)
			bestVERR = TestOnData(Dataset::VAL);

		resetUpdateValues();
		for (int i = 0; i < parms.out.epochs; i++){
			resetError(err);

			UpdateNetEpoch();

			if ((err.measure == BITS) && (err.bits == 0)) {
				return CCTrainer::WIN;
			}
			else if (err.measure == INDEX) {
				double index = ErrorIndex(err.trueErr,1,nTrainOutVals);
				if (index < parms.indexThreshold){
					return CCTrainer::WIN;
				}
			}

			UpdateNetWeights();
			epoch++;

			if (validate){ //this changes the condition for stagnation to performance on validation set. 
				double curVERR = TestOnData(Dataset::VAL);
				if (i == 0 || curVERR < bestVERR){
					bestVERR = curVERR;
					bestWeights = network->GetWeights();
					quitEpoch = epoch + parms.out.patience;
				}
				else if (epoch == quitEpoch){
					network->SetWeights(bestWeights);
					return CCTrainer::STAGNANT;
				}
			}
			else {
				/*if (i == 0){ This was redundant. 
					lastError = err.trueErr;
					quitEpoch = epoch + parms.out.patience;
				}*/
				if (i == 0 || fabs(err.trueErr - lastError) > lastError * parms.out.changeThreshold){
					lastError = err.trueErr;
					quitEpoch = epoch + parms.out.patience;
				}
				else if (epoch == quitEpoch){
					return CCTrainer::STAGNANT;
				}
			}
		}
		if (validate)
			network->SetWeights(bestWeights);
		return CCTrainer::TIMEOUT;
	}

	void CCTrainer::OutputEpoch()
	{
		matDouble ins = data->GetInputs(Dataset::TRAIN);
		matDouble outs = data->GetOutputs(Dataset::TRAIN);
		matDouble::iterator itIns = ins.begin();
		matDouble::iterator itOuts = outs.begin();
		NodeList outNodes = network->GetLayer("out")->GetNodes();
		
		while(itIns != ins.end()){
			/*vecDouble input = (*itIns);
			vecDouble desiredOut = (*itOuts);
			network->Activate(input);*/
			//This looks backwards but the true network output is pulled in the compute error function
			vecDouble desiredOut = ActivateNet((*itIns), (*itOuts));
			ComputeError(desiredOut, err, outNodes, true, true);


			itIns++;
			itOuts++;
		}
		double MSE = err.trueErr / (double)nTrainOutVals;
		MSE_Rec.push_back(MSE);
	}

	void CCTrainer::UpdateNetEpoch()
	{
		matDouble trainingIns = data->GetInputs(Dataset::TRAIN);
		matDouble trainingOuts = data->GetOutputs(Dataset::TRAIN);
		matDouble::iterator itIns;
		matDouble::iterator itOuts;
		itIns = trainingIns.begin();
		itOuts = trainingOuts.begin();
		LayerPtr outLayer = network->GetLayer("out");

		while(itIns != trainingIns.end()){
			vecDouble desiredOut = ActivateNet((*itIns), (*itOuts));
			ComputeNetWeightUpdates(outLayer, desiredOut);
			itIns++;
			itOuts++;
			
		}
		double MSE = err.trueErr / (double)nTrainOutVals;
		MSE_Rec.push_back(MSE);
	}

	vecDouble CCTrainer::ActivateNet( vecDouble inPoint, vecDouble outPoint )
	{
		//this basically doesn't do anything but it allows me to subclass alot less
		//for the rev cc algorithm. 
		//Now it lets me put the caching in relatively easily. 
		//vecDouble retVal;
		if(useNetCache) {
			network->SetOutputs(GetCachedOuts(inPoint)); 
		}
		else {
			network->Activate(inPoint);
		}
		return outPoint;
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

	void CCTrainer::UpdateNetWeights()
	{
		BOOST_FOREACH(ConPair conP, network->GetConnections()){
			ConPtr con = conP.second;
			if (!con->GetLocked()){
				QuickProp(con, net, parms.out.scaledEpsilon, parms.out.decay,
							parms.out.mu, parms.out.shrinkFactor);
			}
		}
	}

	void CCTrainer::resetTrainOuts()
	{
		NodeList outNodes = network->GetLayer("out")->GetNodes();
		BOOST_FOREACH(NodePtr node, outNodes){
			ConList cons = node->GetConnections(IN);
			BOOST_FOREACH(ConPtr con, cons){
				con->Reset();
			}
		}
		resetOutValues();
	}

	CCTrainer::status CCTrainer::TrainCandidates()
	{
#ifdef _CC_USE_CACHE_
		useNetCache = true;
		ClearCache();
#endif
		double lastScore = 0.0;
		int quitEpoch = 0;
		int startEpoch = epoch;
		err.sumErrs /= (double)data->GetSize(Dataset::TRAIN); 
		CorrelationEpoch();
		for (int i = 0; i < parms.cand.epochs; i ++){
			
			CandEpoch();

			UpdateCandWeights();

			UpdateCorrelations();

			epoch++;

			if(resetFlag){ //Not used right now
				epoch = startEpoch;
				i = 0;
				quitEpoch = 0;
				lastScore = 0.0;
				resetTrainCandidates();
				CorrelationEpoch();
				resetFlag = false;
#ifdef _DEBUG
				cout << "Reset Candidate Training" << endl;
#endif // _DEBUG
				continue;
			}

			if ( i == 0 ){
				lastScore = candBestScore;
				quitEpoch = epoch + parms.cand.patience;
			}
			else if (fabs(candBestScore - lastScore) > (lastScore * parms.cand.changeThreshold)){
				quitEpoch = epoch + parms.cand.patience;
				lastScore = candBestScore;
			}
			else if (epoch == quitEpoch){
#ifdef _CC_USE_CACHE_
				useNetCache = false;
#endif
				return STAGNANT;
			}
		}
#ifdef _CC_USE_CACHE_
		useNetCache = false;
#endif
		return TIMEOUT;
	}

	void CCTrainer::resetTrainCandidates()
	{
		NodeList candNodes = network->GetCandLayer()->GetNodes();
		BOOST_FOREACH(NodePtr node, candNodes){
			ConList cons = node->GetConnections(IN);
			BOOST_FOREACH(ConPtr con, cons){
				con->Reset();
			}
		}
		resetCandValues();
	}

	void CCTrainer::CorrelationEpoch() //this function I will bootstrap to calculate the eta mtl parameters. 
	{
		matDouble ins = data->GetInputs(Dataset::TRAIN);
		matDouble outs = data->GetOutputs(Dataset::TRAIN);
		matDouble::iterator itIns = ins.begin();
		matDouble::iterator itOuts = outs.begin();
		NodeList outNodes = network->GetLayer("out")->GetNodes();
		//double SSE = 0;
		while(itIns != ins.end()){
			/*vecDouble input = (*itIns);
			vecDouble desiredOut = (*itOuts);
			network->Activate(input);*/
			//This looks backwards but the true network output is pulled in the compute error function
			vecDouble desiredOut = ActivateNet((*itIns), (*itOuts));
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
		//vecDouble error = output - desiredOut;
		vecDouble error = Error(output, desiredOut);
		vecDouble tmpPrimes = network->GetPrimes(string("out"));
		vecDouble outPrimes = VecAddScalar(tmpPrimes, parms.primeOffset);
		vecDouble errPrimes = VecMultiply(error, outPrimes);
		vecDouble sqError = SquareVec(error);
		errs.errors = errPrimes;

		//Alter Stats
		if (alterStats){
			errs.bits += CalcErrorBits(FilterError(error,primaryIndexes));
			errs.sumErrs += errPrimes;
			errs.sumSqErr += ublas::sum(SquareVec(errPrimes));
			errs.trueErr += ublas::sum(FilterError(sqError,primaryIndexes));
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

	void CCTrainer::ComputeNetWeightUpdates( LayerPtr layer, vecDouble desiredOut /*= vecDouble(0)*/ )
	{
		if (layer->GetType() != Layer::input){
			NodeList& nodes = layer->GetNodes();
			vecDouble output, error, tmpPrimes, outPrimes, errPrimes, sqError, nPrimes;
			switch(layer->GetType()){
			case Layer::out:
				output = network->GetOutputs();
				error = Error(output, desiredOut);
				tmpPrimes = network->GetPrimes(string("out"));
				outPrimes = VecAddScalar(tmpPrimes, parms.primeOffset);
				errPrimes = VecMultiply(error, outPrimes);
				sqError = SquareVec(error);
				err.errors = errPrimes;

				//Alter Stats
				err.bits += CalcErrorBits(FilterError(error, primaryIndexes));
				err.sumErrs += errPrimes;
				err.sumSqErr += ublas::sum(SquareVec(errPrimes));
				err.trueErr += ublas::sum(FilterError(sqError, primaryIndexes));

				//Update Slopes
				for (int i = 0; i < layer->GetSize(); i++){
					string name = nodes[i]->GetName();
					net.localGradients[name] = errPrimes[i];
					ConList cons = nodes[i]->GetConnections(IN);
					BOOST_FOREACH(ConPtr con, cons){
						net.conSlopes[con->GetName()] += errPrimes[i] * con->GetValue();
					}
				}
				break;

			case Layer::hidden:
				tmpPrimes = network->GetPrimes(layer->GetName());
				nPrimes = VecAddScalar(tmpPrimes, parms.primeOffset);
				//vecDouble errPrimes = VecMultiply(error, outPrimes);
				//vecDouble sqError = SquareVec(error);
				for (int i = 0; i < layer->GetSize(); i++){
					string name = nodes[i]->GetName();
					ConList outCons = nodes[i]->GetConnections(OUT);
					double sumOfChildError = 0;
					BOOST_FOREACH(ConPtr con, outCons){
						sumOfChildError += net.localGradients[con->GetOutNodeName()] * con->GetWeight();
					}
					double delta = nPrimes[i] * sumOfChildError;
					net.localGradients[name] = delta;
					ConList inCons = nodes[i]->GetConnections(IN);
					BOOST_FOREACH(ConPtr con, inCons){
						net.conSlopes[con->GetName()] += delta * con->GetValue(); //might be able to save some time here if I check for locked cons. Only if it seems to be a problem. 
					}
				}

				break;

			default:
				break;
			}
			ComputeNetWeightUpdates(layer->GetPrevLayer());
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
			string name = node->GetName();
			cCorr = &candCorr[name];
			val = node->GetOut();
			candSumVals[name] += val;

			//compute correlation for this unit
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
				score += fabs (cor);
			}

			if (parms.useSDCC)
			{
				int tmpHeight = network->GetCandLayer()->GetHeight();
				if (node->GetHeight() == tmpHeight){
					score *= parms.SDCCRatio;
				}
			}

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

	void CCTrainer::CandEpoch()
	{
		matDouble ins = data->GetInputs(Dataset::TRAIN);
		matDouble outs = data->GetOutputs(Dataset::TRAIN);
		matDouble::iterator itIns = ins.begin();
		matDouble::iterator itOuts = outs.begin();
		NodeList outNodes = network->GetLayer("out")->GetNodes();
		//double SSE = 0;
		while(itIns != ins.end()){
			/*vecDouble input = (*itIns);
			vecDouble desiredOut = (*itOuts);
			network->Activate(input);*/
			//This looks backwards but the true network output is pulled in the compute error function
			vecDouble desiredOut = ActivateNet((*itIns), (*itOuts));
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
		if (useNetCache)
		{
			CacheActivateCands(network->GetCandLayer());
		} 
		else
		{
			network->GetCandLayer()->Activate(); 
		}
		int nOuts = network->GetNumOut();
		BOOST_FOREACH(NodePtr candidate, candNodes){
			string name = candidate->GetName();
			value = candidate->GetOut();
			actPrime = candidate->GetPrime();
			change = 0.0;
			cCorr = &candCorr[name];
			cPCorr = &candPCorr[name];

			candSumVals[name] += value;
			actPrime /= err.sumSqErr; //This comes from fahlmans cc code. 

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
		taskErrors.clear();

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
				string name = "task-";
				name += lexical_cast<string>(i);
				int tmp = (int)errors[i];
#ifdef _DEBUG
				cout << tmp << " " << output << " " << desiredOut << endl;
#endif
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

	double CCTrainer::TestOnData( Dataset::datatype type )
	{
		matDouble ins = data->GetInputs(type);
		matDouble outs = data->GetOutputs(type);
		matDouble::iterator itIns = ins.begin();
		matDouble::iterator itOuts = outs.begin();

		double SSE = 0;
		while(itIns != ins.end()){
			vecDouble input = (*itIns);
			/*vecDouble desiredOut = (*itOuts);
			network->Activate(input);*/
			vecDouble desiredOut = ActivateNet(input, (*itOuts)); //this makes using this for subclasses easier
			vecDouble output = network->GetOutputs();
			vecDouble error = desiredOut - output;
			vecDouble sqError = SquareVec(error);
			SSE += ublas::sum(FilterError(sqError,primaryIndexes));
			itIns++;
			itOuts++;
		}
		SSE /= (double)ins.size();
		if (primaryIndexes.size() > 0){
			SSE /= (double)primaryIndexes.size();
		}
		else {
			SSE /= (double)network->GetNumOut();
		}
		return SSE; //This is now MSE 
	}

	void CCTrainer::InsertCandidate()
	{
		int numOuts = network->GetNumOut();
		vecDouble outWeights(numOuts);
		double weightMod;
		weightMod = parms.weightMult / network->GetNumUnits();
		assert(bestCand.get() != 0);
		for (int i = 0; i < numOuts; i++){
			outWeights[i] = -candPCorr[bestCand->GetName()][i] * weightMod;
		}
		network->InstallCandidate(bestCand,outWeights);
	}

	CCTrainer::status CCTrainer::ValidationEpoch()
	{
		matDouble ins = data->GetInputs(Dataset::VAL);
		matDouble outs = data->GetOutputs(Dataset::VAL);
		matDouble::iterator itIns = ins.begin();
		matDouble::iterator itOuts = outs.begin();
		NodeList outNodes = network->GetLayer("out")->GetNodes();
		status returnStatus = CCTrainer::TRAINING;
		//double SSE = 0;
		resetError(valErr);
		while(itIns != ins.end()){
			/*vecDouble input = (*itIns);
			vecDouble desiredOut = (*itOuts);
			network->Activate(input);*/
			//This looks backwards but the true network output is pulled in the compute error function
			vecDouble desiredOut = ActivateNet((*itIns), (*itOuts));
			ComputeError(desiredOut, valErr, outNodes, true, false);


			itIns++;
			itOuts++;
		}
		valErr.trueErr /= (double)nTrainOutVals; //Might need this later
		vMSE_Rec.push_back(valErr.trueErr);

		if(val.bestErr == -1.0){
			val.bestErr = valErr.trueErr;
			val.bestPass = network->GetNumUnits();
			val.bestWeights = network->GetWeights();
		}
		else if(valErr.trueErr < val.bestErr){
			val.bestErr = valErr.trueErr;
			val.bestPass = network->GetNumUnits();
			val.bestWeights = network->GetWeights();
		}
		else if ((network->GetNumUnits() - val.bestPass) > parms.valPatience){
			network->SetWeights(val.bestWeights);
			returnStatus = CCTrainer::STAGNANT;
			val.bestErr = -1.0;
		}

		return returnStatus;
	}

	CCTrainer::status CCTrainer::TestImprovement()
	{
		status returnStatus = CCTrainer::TRAINING;

		double trueErr = err.trueErr / (double)nTrainOutVals;
		if(imp.bestErr == -1.0){
			imp.bestErr = trueErr;
			imp.bestPass = network->GetNumUnits();
			imp.bestWeights = network->GetWeights();
		}
		else if(trueErr < imp.bestErr){
			imp.bestErr = trueErr;
			imp.bestPass = network->GetNumUnits();
			imp.bestWeights = network->GetWeights();
		}
		else if ((network->GetNumUnits() - imp.bestPass) > parms.impPatience){
			network->SetWeights(imp.bestWeights);
			returnStatus = CCTrainer::STAGNANT;
			imp.bestErr = -1.0;
		}

		return returnStatus;
	}

	void CCTrainer::TrainToValConv( int maxEpochs )
	{
		TrainToConvergence(maxEpochs, true);
	}

	void CCTrainer::TrainToConvergence( int maxEpochs, bool validate )
	{
#ifdef _DEBUG
		cout << "Start Training" << endl;
#endif // _DEBUG
		ResetVars();
		status valStatus = TRAINING;
		status outStatus;
		status candStatus;
		status impStatus;
		this->useValidation = validate;
		while(epoch < maxEpochs){
			outStatus = TrainOuts();

#ifdef _DEBUG
			cout << "Out Status: ";
			PrintStatus(outStatus);
			cout << "Epochs: " << epoch << endl;
#endif // _DEBUG

			if (outStatus == WIN)
				break;

			impStatus = TestImprovement();
			if (impStatus != TRAINING){
				break;
			}
			//do validation? 
			if (validate) {
				assert(data->GetSize(Dataset::VAL) > 0);
				valStatus = ValidationEpoch();
				if (valStatus != TRAINING)
					break;
			}

#ifdef _DEBUG
			cout << "Val Status: ";
			PrintStatus(valStatus);
			cout << endl;
#endif // _DEBUG

			//train candidates

			CreateCandidates();
			candStatus = TrainCandidates();
			InsertCandidate();

#ifdef _DEBUG
			cout << "Cand Status: ";
			PrintStatus(candStatus);
			cout << "Epochs: " << epoch	<< endl;
#endif // _DEBUG
		}
		if(outStatus != WIN && valStatus == TRAINING && impStatus == TRAINING)
			TrainOuts();
	}

	int CCTrainer::GetEpochs()
	{
		return epoch;
	}

	int CCTrainer::GetNumHidLayers()
	{
		return network->GetNumHidLayers();
	}

	double CCTrainer::ErrorIndex( double trueError, double stdDev, int nTrainOutVals )
	{
		return (sqrt( trueError/ (double)nTrainOutVals) / stdDev);
	}

	doubles& CCTrainer::GetMSERec(){
		return MSE_Rec;
	}

	doubles& CCTrainer::GetVMSERec(){
		return vMSE_Rec;
	}

	void CCTrainer::PrintStatus( status runStatus )
	{
		switch (runStatus)
		{
		case STAGNANT:
			cout << "Stagnant ";
			break;
		case WIN:
			cout << "Win ";
			break;
		case TRAINING:
			cout << "Training ";
			break;
		case TIMEOUT:
			cout << "Timeout ";
			break;
		default:
			cout << "N/A ";
		}
	}

	int CCTrainer::CalcErrorBits( vecDouble error )
	{
		int errBits = 0;
		BOOST_FOREACH(double err, error){
			if (fabs(err) > parms.scoreThreshold)
				errBits++;
		}
		return errBits;
	}

	int CCTrainer::GetNumResets()
	{
		return numResets;
	}

	void CCTrainer::SetDataSet( DatasetPtr dataset )
	{
		this->data = dataset;
		if(primaryIndexes.size() > 0){
			nTrainOutVals = data->GetSize(Dataset::TRAIN) * primaryIndexes.size();
		}
		else {
			nTrainOutVals = data->GetSize(Dataset::TRAIN) * network->GetNumOut();
		}
	}

	/*
	 *	The indexes should be -1 for an append. The number of new inputs will match the size of the indexes vector
	 */
	void CCTrainer::AddNewInputs( ints inputIndexes, DatasetPtr newData, bool connectToHidden /*= False*/ )
	{
		SetDataSet(newData);
		NodeList addedNodes;
		BOOST_FOREACH(int index, inputIndexes){
			//index of -1 means append to the end. 
			NodePtr newNode;
			if (index == -1){
				newNode = this->network->AppendNewInputNode();
			}
			else if (index < network->GetNumIn()){
				newNode = this->network->InsertNewInputNode(index);
			}
			else {
				assert(0);
			}
			addedNodes.push_back(newNode);
		}
		if (connectToHidden){
			network->ConnectToHiddenNodes(addedNodes);
			network->LockConnections(true, addedNodes);
			ResetVars();
			UpdateNet();
			network->LockConnections(false);
		}
	}

	JRNN::vecDouble CCTrainer::GetCachedOuts( vecDouble inVec )
	{
		string inString = StringFromVector(inVec);
		currentInString = inString;
		vecDouble retOut;
		if (networkCache.find(inString)) {
			retOut = networkCache[inString];
		} 
		else {
			network->Activate(inVec);
			retOut = network->GetOutputs();
			networkCache[inString] = retOut;
			StoreNodeCache(inString);
		}
		return retOut;
	}

	void CCTrainer::ClearCache()
	{
		networkCache.clear();
		BOOST_FOREACH(hashedHashedDoubleMap::value_type nodeC, nodeCache){
			nodeC.second.clear();
		}
		nodeCache.clear();
	}

	void CCTrainer::StoreNodeCache( string inString )
	{
		hashedDoubleMap tmpDM;
		BOOST_FOREACH(LayerPair LayerP, network->GetLayers()){
			BOOST_FOREACH(NodePtr nodeP, LayerP.second->GetNodes()){
				tmpDM[nodeP->GetName()] = nodeP->GetOut();
			}
		}
		nodeCache[inString] = tmpDM;
	}

	void CCTrainer::CacheActivateCands( const LayerPtr candL )
	{
		BOOST_FOREACH(NodePtr candN, candL->GetNodes()){
			double tmpSum = 0;
			BOOST_FOREACH(ConPtr con, candN->GetConnections(IN)){
				tmpSum += con->GetWeight() * nodeCache[currentInString][con->GetInNodeName()];
			}
			candN->Activate(tmpSum);
		}
	}

	bool CCTrainer::GetUseTrainOutVal() const
	{
		return useTrainOutVal;
	}

	void CCTrainer::SetUseTrainOutVal( bool val )
	{
		useTrainOutVal = val;
	}

}