/********************************************************************
	created:	2012/03/23
	created:	23:3:2012   0:46
	filename: 	RevCCTrainer.cpp
	author:		jreeder
	
	purpose:	Trainer for reverberating CC. This will have two networks
	that work together to alleviate catastrophic forgetting. 
*********************************************************************/
#include "JRNN.h"
#include "trainers/RevCCTrainer.h"
#include "utility/dataset.h"

namespace JRNN {

	RandomGenerator01 RevCCTrainer::revRand = RandomGenerator01();

	RevCCTrainer::RevCCTrainer(int numIn, int numOut, int numCandidates){
		net1 = RevCCNetwork::Create();
		net2 = RevCCNetwork::Create();
		net1->Build(numIn, numOut);
		net2->Build(numIn, numOut);
		revRand.gen.seed(static_cast<unsigned int>(time(NULL)));
	}
	RevCCTrainer::~RevCCTrainer(){}

	RevCCTrainer::reverbdpoint RevCCTrainer::ReverberateNetwork()
	{
		vecDouble tmpIn(revNet->GetNumIn());
		for (uint i = 0; i < tmpIn.size(); i++)
		{
			tmpIn[i] = revRand();
		}
		revNet->Activate(tmpIn);
		for (uint i = 1; i < revparams.numRev; i++)
		{
			tmpIn = revNet->GetAutoAssocLayer()->GetOutput();
			revNet->Activate(tmpIn);
		}
		reverbdpoint retVal;
		retVal.inPoint = tmpIn;
		retVal.outPoint = revNet->GetNormOutLayer()->GetOutput();

		return retVal;
	}

	vecDouble RevCCTrainer::ActivateNet( vecDouble inPoint, vecDouble outPoint )
	{
		vecDouble retVal;
		retVal = ConcatVec(outPoint, inPoint);
		network->Activate(inPoint);
		return retVal;
	}

	vecDouble RevCCTrainer::ConcatVec( vecDouble first, vecDouble second )
	{
		vecDouble retVec(first.size() + second.size());
		int newSize = first.size() + second.size();
		int i = 0;
		BOOST_FOREACH(double val, first){
			retVec[i] = val;
			i++;
		}
		BOOST_FOREACH(double val, second){
			retVec[i] = val;
			i++;
		}
		return retVec;
	}

	double RevCCTrainer::TestOnData( Dataset::datatype type )
	{
		net1->getTrueOuts = true;
		network = net1;
		return CCTrainer::TestOnData(type);
		net1->getTrueOuts = true;
	}

	JRNN::hashedDoubleMap RevCCTrainer::TestWiClass( Dataset::datatype type )
	{
		net1->getTrueOuts = true;
		network = net1;
		return CCTrainer::TestWiClass(type);
		net1->getTrueOuts = false;
	}

}