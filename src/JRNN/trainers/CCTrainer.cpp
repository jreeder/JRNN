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
		ResetVars();
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

	void CCTrainer::Reset()
	{
		ResetVars();
		network->Reset();
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

			if (i == 0){
				lastError = err.trueErr;
				quitEpoch = epoch + parms.out.patience;
			}
			else if (fabs(err.trueErr - lastError) > lastError * parms.out.changeThreshold){
				lastError = err.trueErr;
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
		
		while(itIns != ins.end()){
			vecDouble input = (*itIns);
			vecDouble desiredOut = (*itOuts);
			network->Activate(input);
			ComputeError(desiredOut, err, outNodes, true, true);


			itIns++;
			itOuts++;
		}
		double MSE = err.trueErr / (double)nTrainOutVals;
		MSE_Rec.push_back(MSE);
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
		double lastScore = 0.0;
		int quitEpoch = 0;
		int startEpoch = epoch;
		err.sumErrs /= (double)data->GetSize(Dataset::TRAIN); //TODO Might be causing some issues here. Needs to be cast to double. 
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
				return STAGNANT;
			}
		}
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
			string name = candidate->GetName();
			value = candidate->GetOut();
			actPrime = candidate->GetPrime();
			change = 0.0;
			cCorr = &candCorr[name];
			cPCorr = &candPCorr[name];

			candSumVals[name] += value;
			actPrime /= err.sumSqErr;

			//TODO need to look into making changes here for eta MTL style focusing. 

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
			vecDouble input = (*itIns);
			vecDouble desiredOut = (*itOuts);
			network->Activate(input);
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

}