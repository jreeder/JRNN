/********************************************************************
	created:	2011/10/20
	created:	20:10:2011   18:15
	filename: 	EtaMTLRPropTrainer.h
	author:		jreeder
	
	purpose:	Eta MTL adapted to RProp
*********************************************************************/

#ifndef _ETAMTLRPROPTRAINER_H
#define _ETAMTLRPROPTRAINER_H

#include "JRNN.h"
#include "trainers/RPropTrainer.h"
#include "utility/etamtlhelper.h"

namespace JRNN {

	class EtaMTLRPropTrainer : public RPropTrainer, public EtaMTLHelper  {

	public:
		EtaMTLRPropTrainer(FFMLPNetPtr network, DatasetPtr inDataSet, double etaPlus, double etaMinus, ints primaryIndexes = ints(0));
		virtual ~EtaMTLRPropTrainer();
		virtual double TrainEpoch();
		virtual void Reset();

	protected:
		virtual void CalcWeightUpdates(LayerPtr layer, vecDouble desiredOut = vecDouble());
		double globalLearningRate;
	};
}
#endif // _ETAMTLRPROPTRAINER_H