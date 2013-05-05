/********************************************************************
	created:	2013/05/05
	created:	5:5:2013   3:08
	filename: 	KBCCTrainer.cpp
	author:		jreeder
	
	purpose:	Trainer for the KBCC Network. 
*********************************************************************/

#include "JRNN.h"
#include "trainers/KBCCTrainer.h"

using namespace JRNN {
	void KBCCTrainer::AddNewInputs( ints inputIndexes, DatasetPtr newData, bool connectToHidden /*= false */ )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	double KBCCTrainer::TestOnData( Dataset::datatype type )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	hashedDoubleMap KBCCTrainer::TestWiClass( Dataset::datatype type )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	status KBCCTrainer::TrainOuts()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCTrainer::CreateCandidates()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCTrainer::CorrelationEpoch()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCTrainer::UpdateCorrelations()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCTrainer::ComputeCandSlopes()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void KBCCTrainer::InsertCandidate()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	vecDouble KBCCTrainer::ActivateNet( vecDouble inPoint, vecDouble outPoint )
	{
		throw std::exception("The method or operation is not implemented.");
	}
}


