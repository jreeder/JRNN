/* 
 * File:   BackPropTrainer.h
 * Author: jreeder
 *
 * Created on May 20, 2010, 3:32 PM
 */

#ifndef _BACKPROPTRAINER_H
#define	_BACKPROPTRAINER_H

#include "JRNN.h"

namespace JRNN {

class BackPropTrainer {
public:
    BackPropTrainer(NetworkPtr network, DatasetPtr inDataSet, double learningRate);
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
    NetworkPtr mNetwork;
    int epochCount;
    static const int continueEpochs = 50; //Number of epochs to continue looking for improvement on validation set.
    int trainingIndex;
    double learningRate;
    doubles MSE_Rec;
    doubles vMSE_Rec;
    DatasetPtr data;
//    matDouble trainingIns;
//    matDouble trainingOuts;
//    matDouble valIns;
//    matDouble valOuts;
//    matDouble testIns;
//    matDouble testOuts;
    hashedDoubleMap localGradients;
    hashedDoubleMap weightUpdates;
    hashedDoubleMap bestWeights;
    hashedIntMap taskErrors;
    hashedDoubleMap taskErrorRate;

    vecDouble SquareVec(vecDouble vector);
    vecDouble ApplyThreshold(vecDouble vector);
    void CalcWeightUpdates(LayerPtr layer, vecDouble desiredOut = vecDouble());
};

}
#endif	/* _BACKPROPTRAINER_H */

