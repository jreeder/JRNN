/* 
 * File:   RPropTrainer.h
 * Author: jreeder
 *
 * Created on June 7, 2010, 11:10 PM
 */

#ifndef _RPROPTRAINER_H
#define	_RPROPTRAINER_H

#include "JRNN.h"
//#include "structure/network.h"
#include "networks/FFMLPNetwork.h"
#include "utility/dataset.h"

namespace JRNN {

	class RPropTrainer {
		//TODO: need to abstract this class and backproptrainer and use inheritance
		//TODO: need to pull MTL specific stuff out into a parent class so all trainers can share. 
	public:
		RPropTrainer(FFMLPNetPtr network, DatasetPtr inDataSet, double etaPlus, double etaMinus, ints primaryIndexes = ints(0));
		//RPropTrainer(const RPropTrainer& orig);
		virtual ~RPropTrainer();
		virtual double TrainEpoch();
		double TestOnData(Dataset::datatype type);
		hashedDoubleMap TestWiClass(Dataset::datatype type);
		doubles& GetMSERec();
		doubles& GetVMSERec();
		int GetEpochs();
		void TrainToConvergence(double maxSSE, int maxEpoch);
		void TrainToValConv(int maxEpoch);
		void Reset();
		void RedistData();
		int GetNumResets();
		bool useMaxWeight;
		double maxWeight;
		//void SetUseMaxWeight(bool inUseMaxWeight);
		//void SetMaxWeight(double inMaxWeight);

	protected:
		FFMLPNetPtr mNetwork;
		int epochCount;
		int numResets;
		double etaPlus;
		double etaMinus;
		static const int continueEpochs = 50;
		static const double deltaMax;
		static const double deltaMin;
		int trainingIndex;
		double learningRate;
		double primeOffset; //helps stop flat spots
		doubles MSE_Rec;
		doubles vMSE_Rec;
		ints primaryIndexes;
		DatasetPtr data;
		hashedDoubleMap localGradients;
		hashedDoubleMap weightUpdates;
		hashedDoubleMap lastUpdate;
		hashedDoubleMap dw;
		hashedDoubleMap dw_last;
		hashedDoubleMap delta;
		hashedDoubleMap delta_last;
		hashedDoubleMap bestWeights;
		hashedIntMap taskErrors;
		hashedDoubleMap taskErrorRate;
		int nTrainOutVals;
		void RPropUpdate(ConPtr con);
		virtual void CalcWeightUpdates(LayerPtr layer, vecDouble desiredOut = vecDouble());
	};

}
#endif	/* _RPROPTRAINER_H */

