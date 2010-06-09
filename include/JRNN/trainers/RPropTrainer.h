/* 
 * File:   RPropTrainer.h
 * Author: jreeder
 *
 * Created on June 7, 2010, 11:10 PM
 */

#ifndef _RPROPTRAINER_H
#define	_RPROPTRAINER_H

namespace JRNN {

	class RPropTrainer {
		//TODO: need to abstract this class and backproptrainer and use inheritance
	public:
		RPropTrainer(networkPtr network, datasetPtr inDataSet, double etaPlus, double etaMinus);
		RPropTrainer(const RPropTrainer& orig);
		virtual ~RPropTrainer();
		double trainEpoch();
		double testOnData(dataset::datatype type);
		hashedDoubleMap testWiClass(dataset::datatype type);
		doubles& getMSERec();
		doubles& getVMSERec();
		int getEpochs();
		void trainToConvergence(double maxSSE, int maxEpoch);
		void trainToValConv(int maxEpoch);
		void reset();

	private:
		networkPtr mNetwork;
		int epochCount;
		double etaPlus;
		double etaMinus;
		static const int continueEpochs = 50;
		static const double deltaMax;
		static const double deltaMin;
		int trainingIndex;
		double learningRate;
		doubles MSE_Rec;
		doubles vMSE_Rec;
		datasetPtr data;
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

		void RPropUpdate(conPtr con);
		int sign(double num);
		vecDouble squareVec(vecDouble vector);
		vecDouble applyThreshold(vecDouble vector);
		void calcWeightUpdates(layerPtr layer, vecDouble desiredOut = vecDouble());
	};

}
#endif	/* _RPROPTRAINER_H */

