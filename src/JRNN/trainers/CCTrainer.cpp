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

		if(primaryIndexes.size() > 0){
			nTrainOutVals = data->GetSize(Dataset::TRAIN) * primaryIndexes.size();
		}
		else {
			nTrainOutVals = data->GetSize(Dataset::TRAIN) * network->GetNumOut();
		}
		
		parms.nTrials = 1;//Not used
		parms.maxNewUnits = 25;//Not used
		parms.valPatience = 2;
		parms.weightMult = 1.0;

		parms.out.epochs = 300; //200
		parms.out.patience = 12;//12
		parms.out.epsilon = 10.0;//1.0
		parms.out.decay = 0.001;
		parms.out.mu = 2.0;
		parms.out.changeThreshold = 0.02; //0.01

		parms.cand.epochs = 200; //200
		parms.cand.patience = 12; //12
		parms.cand.epsilon = 100;
		parms.cand.decay = 0.001;
		parms.cand.mu = 2.0;
		parms.cand.changeThreshold = 0.03;
		
		parms.nCand = numCandidates;
		parms.indexThreshold = 0.1;
		parms.scoreThreshold = 0.4;
		out.conDeltas.clear();
		out.conPSlopes.clear();
		out.conSlopes.clear();
		cand.conDeltas.clear();
		cand.conPSlopes.clear();
		cand.conSlopes.clear();

		int numOut = network->GetNumOut();
		err.errors = vecDouble(numOut);
		err.sumErrs = vecDouble(numOut);
		err.measure = BITS;
		valErr.errors = vecDouble(numOut);
		valErr.sumErrs = vecDouble(numOut);
		valErr.measure = BITS;
		candCorr = hashedVecDoubleMap(numOut);
		candPCorr = hashedVecDoubleMap(numOut);
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
		resetError(err);
		resetError(valErr);
		taskErrors.clear();
		taskErrorRate.clear();
		val.bestErr = -1.0;
		val.bestWeights.clear();
		val.bestPass = network->GetNumUnits();
		vMSE_Rec.clear();
		MSE_Rec.clear();
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

	void CCTrainer::CreateCandidates()
	{
		network->CreateCandLayer(parms.nCand);
		resetCandValues();
	}

	CCTrainer::status CCTrainer::TrainOuts()
	{
		int quitEpoch = 0;
		double lastError = 0.0;
		//resetOutValues();
		for (int i = 0; i < parms.out.epochs; i++){
			resetError(err);
			
			OutputEpoch();

			if ((err.measure == BITS) && (err.bits == 0)) {
				return CCTrainer::WIN;
			}
			else {
				double index = ErrorIndex(err.trueErr,1,nTrainOutVals);
				if (index < parms.indexThreshold){
					return CCTrainer::WIN;
				}
			}

			UpdateOutWeights();
			epoch++;

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
		vecDouble outPrimes = network->GetPrimes(string("out"));
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

		candBestScore = -1.0;
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
				cor = ((*curCorr)[j] - avgValue * err.sumErrs[j]) / err.sumSqErr;
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
			string name = candidate->GetName();
			value = candidate->GetOut();
			actPrime = candidate->GetPrime();
			change = 0.0;
			cCorr = &candCorr[name];
			cPCorr = &candPCorr[name];

			candSumVals[name] += value;
			actPrime /= err.sumSqErr;

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

	void CCTrainer::Reset()
	{
		ResetVars();
		network->Reset();
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
		while(epoch < maxEpochs){
			outStatus = TrainOuts();

#ifdef _DEBUG
			cout << "Out Status: ";
			PrintStatus(outStatus);
			cout << "Epochs: " << epoch << endl;
#endif // _DEBUG

			if (outStatus == WIN)
				break;

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
		if(outStatus != WIN && valStatus == TRAINING)
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

}