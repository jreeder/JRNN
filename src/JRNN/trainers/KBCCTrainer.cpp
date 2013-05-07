/********************************************************************
	created:	2013/05/05
	created:	5:5:2013   3:08
	filename: 	KBCCTrainer.cpp
	author:		jreeder
	
	purpose:	Trainer for the KBCC Network. 
*********************************************************************/

#include "JRNN.h"
#include "trainers/KBCCTrainer.h"

namespace JRNN {

	KBCCTrainer::KBCCTrainer( CCNetworkPtr network, DatasetPtr data, int numCandidates, NetPtrList SubNetList, int numCopies, ints primaryIndexes /*= ints(0)*/ ) : CCTrainer(network, data, numCandidates, primaryIndexes)
	{
		this->SubNetlist = SubNetList;
		this->numCopies = numCopies;
	}
	
	KBCCTrainer::~KBCCTrainer(){}

	double KBCCTrainer::TestOnData( Dataset::datatype type )
	{
		return CCTrainer::TestOnData(type);
	}

	hashedDoubleMap KBCCTrainer::TestWiClass( Dataset::datatype type )
	{
		return CCTrainer::TestWiClass(type);
	}

	CCTrainer::status KBCCTrainer::TrainOuts()
	{
		return CCTrainer::TrainOuts();
	}

	void KBCCTrainer::CreateCandidates()
	{
		if (SubNetlist.size() == 0){
			network->CreateCandLayer(parms.nCand);
		}
		else {
			dynamic_pointer_cast<KBCCNetwork>(network)->CreateCandLayer(parms.nCand, SubNetlist, numCopies);
		}
		
		resetCandValues();
	}

	void KBCCTrainer::UpdateCorrelations()
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
			score = 0.0;
			if (node->GetActFuncType() != NetworkNode::ActType){
				string name = node->GetName();
				avgValue = candSumVals[name] / nTrainPts;
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

				candSumVals[name] = 0.0;
			}
			else {
				NodeList candOutNodes = dynamic_pointer_cast<NetworkNode>(node)->GetOutputNodes();
				BOOST_FOREACH(NodePtr coNode, candOutNodes){
					string name = coNode->GetName();
					avgValue = candSumVals[name] / nTrainPts;
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

					candSumVals[name] = 0.0;
				}
			}
			if (parms.useSDCC)
			{
				int tmpHeight = network->GetCandLayer()->GetHeight();
				if (node->GetHeight() == tmpHeight){
					score *= parms.SDCCRatio;
				}
			}
			
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

	void KBCCTrainer::ComputeCandSlopes()
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
			if (candidate->GetActFuncType() != NetworkNode::ActType)
			{
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
			else
			{
				NodeList candInNodes = dynamic_pointer_cast<NetworkNode>(candidate)->GetInputNodes();
				NodeList candOutNodes = dynamic_pointer_cast<NetworkNode>(candidate)->GetOutputNodes();
				hashedVecDoubleMap candActPrimes = dynamic_pointer_cast<NetworkNode>(candidate)->GetPrimes();
				for (uint i = 0; i < candInNodes.size(); i++)
				{
					change = 0.0;
					BOOST_FOREACH(NodePtr coNodes, candOutNodes){
						string name = coNodes->GetName();
						value = coNodes->GetOut();
						cCorr = &candCorr[name];
						cPCorr = &candPCorr[name];

						actPrime = candActPrimes[name][i];

						candSumVals[name] += value;
						actPrime /= err.sumSqErr;

						//compute correlations to each output
						for (int j = 0; j < nOuts; j++){
							error = err.errors[j];
							direction = ((*cPCorr)[j] < 0.0) ? -1.0 : 1.0;
							change -= direction * actPrime * (error - err.sumErrs[j]); 
							(*cCorr)[j] += error * value;
						}

					}

					ConList cons = candInNodes[i]->GetConnections(IN);
					BOOST_FOREACH(ConPtr con, cons){
						cand.conSlopes[con->GetName()] += change * con->GetValue();
					}
				}
			}
		}
	}

	void KBCCTrainer::InsertCandidate()
	{
		int numOuts = network->GetNumOut();
		vecDouble outWeights(numOuts);
		double weightMod;
		weightMod = parms.weightMult / network->GetNumUnits();
		assert(bestCand.get() != 0);
		if (bestCand->GetActFuncType() != NetworkNode::ActType)
		{
			for (int i = 0; i < numOuts; i++){
				outWeights[i] = -candPCorr[bestCand->GetName()][i] * weightMod;
			}
			network->InstallCandidate(bestCand,outWeights);
		} 
		else
		{
			NodeList candOutNodes = dynamic_pointer_cast<NetworkNode>(bestCand)->GetOutputNodes();
			hashedVecDoubleMap hashedOutWeights;
			BOOST_FOREACH(NodePtr node, candOutNodes){
				string name = node->GetName();
				for (int i = 0; i < numOuts; i++){
					hashedOutWeights[name][i] = -candPCorr[name][i] * weightMod;
				}
			}
			dynamic_pointer_cast<KBCCNetwork>(network)->InstallCandidate(bestCand, hashedOutWeights);
		}
	}

	vecDouble KBCCTrainer::ActivateNet( vecDouble inPoint, vecDouble outPoint )
	{
		return CCTrainer::ActivateNet(inPoint, outPoint);
	}

	void KBCCTrainer::ComputeCorrelations()
	{
		double sum,val;
		vecDouble* cCorr;
		NodeList candNodes = network->GetCandLayer()->GetNodes();
		network->GetCandLayer()->Activate();
		BOOST_FOREACH(NodePtr node, candNodes){
			sum = 0.0;
			if (node->GetActFuncType() != NetworkNode::ActType)
			{
				string name = node->GetName();
				cCorr = &candCorr[name];
				val = node->GetOut();
				candSumVals[name] += val;

				//compute correlation for this unit
				for (unsigned int j = 0; j < err.errors.size(); j++ ){
					(*cCorr)[j] += val * err.errors[j];
				}
			} 
			else
			{
				NodeList candOutNodes = dynamic_pointer_cast<NetworkNode>(node)->GetOutputNodes();
				BOOST_FOREACH(NodePtr coNode, candOutNodes){
					string name = coNode->GetName();
					val = coNode->GetOut();
					cCorr = &candCorr[name];
					candSumVals[name] += val;

					//compute correlation for this unit
					for (unsigned int j = 0; j < err.errors.size(); j++ ){
						(*cCorr)[j] += val * err.errors[j];
					}
				}
			}
		}
	}

	void KBCCTrainer::AddNewInputs( ints inputIndexes, DatasetPtr newData, bool connectToHidden /*= false */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	int KBCCTrainer::GetNumCopies() const
	{
		return numCopies;
	}

	void KBCCTrainer::SetNumCopies( int val )
	{
		numCopies = val;
	}

	void KBCCTrainer::AddSubNet( NetworkPtr newNet )
	{
		SubNetlist.push_back(newNet);
	}

	void KBCCTrainer::ClearSubNetList()
	{
		SubNetlist.clear();
	}

	void KBCCTrainer::Reset()
	{
		CCTrainer::Reset();
	}

}


