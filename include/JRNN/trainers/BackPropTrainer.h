/* 
 * File:   BackPropTrainer.h
 * Author: jreeder
 *
 * Created on May 20, 2010, 3:32 PM
 */

#ifndef _BACKPROPTRAINER_H
#define	_BACKPROPTRAINER_H

#include "JRNN.h"
//#include "structure/network.h"
#include "utility/FFMLPNetwork.h"
#include "utility/dataset.h"

namespace JRNN {

class BackPropTrainer {
public:
    BackPropTrainer(FFMLPNetPtr network, DatasetPtr inDataSet, double learningRate);
    BackPropTrainer(const BackPropTrainer& orig);
    virtual ~BackPropTrainer();
    double TrainEpoch();
    double TestOnData(Dataset::datatype type);
    hashedDoubleMap TestWiClass(Dataset::datatype type);
    doubles& GetMSERec();
    doubles& GetVMSERec();
    int GetEpochs();
    void TrainToConvergence(double maxSSE, int maxEpoch);
    void TrainToValConv(int maxEpoch);
    void Reset();

private:
    FFMLPNetPtr mNetwork;
    int epochCount;
    static const int continueEpochs = 50; //Number of epochs to continue looking for improvement on validation set.
    int trainingIndex;
    double learningRate;
    doubles MSE_Rec;
    doubles vMSE_Rec;
    DatasetPtr data;
    hashedDoubleMap localGradients;
    hashedDoubleMap weightUpdates;
    hashedDoubleMap bestWeights;
    hashedIntMap taskErrors;
    hashedDoubleMap taskErrorRate;

    void CalcWeightUpdates(LayerPtr layer, vecDouble desiredOut = vecDouble());
};

}
#endif	/* _BACKPROPTRAINER_H */

