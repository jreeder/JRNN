/*
 * KBCCTrainer.h
 *
 *  Created on: Mar 31, 2013
 *      Author: jreeder
 */

#ifndef KBCCTRAINER_H_
#define KBCCTRAINER_H_

#include "JRNN.h"
#include "trainers/CCTrainer.h"
#include "networks/KBCCNetwork.h"

namespace JRNN {

	//class KBCCTrainer;
	//typedef boost::shared_ptr<KBCCTrainer> KBCCTrainerPtr;

	class KBCCTrainer : public CCTrainer {
	public:
		KBCCTrainer(int numIn, int numOut, int numCandidates);
		~KBCCTrainer();

		virtual void AddNewInputs( ints inputIndexes, DatasetPtr newData, bool connectToHidden = false );

		virtual double TestOnData( Dataset::datatype type );

		virtual hashedDoubleMap TestWiClass( Dataset::datatype type );

		virtual status TrainOuts();

		virtual void CreateCandidates();

		virtual void CorrelationEpoch();

		virtual void UpdateCorrelations();

		virtual void ComputeCandSlopes();

		virtual void InsertCandidate();

		virtual vecDouble ActivateNet( vecDouble inPoint, vecDouble outPoint );

	protected:

	};
}

#endif /* KBCCTRAINER_H_ */
