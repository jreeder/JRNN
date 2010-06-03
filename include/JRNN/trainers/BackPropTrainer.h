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
    BackPropTrainer(network& network, datasetPtr inDataSet, double learningRate);
    BackPropTrainer(const BackPropTrainer& orig);
    virtual ~BackPropTrainer();
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
    network* mNetwork;
    int epochCount;
    static const int continueEpochs = 50; //Number of epochs to continue looking for improvement on validation set.
    int trainingIndex;
    double learningRate;
    doubles MSE_Rec;
    doubles vMSE_Rec;
    datasetPtr data;
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

    vecDouble squareVec(vecDouble vector);
    vecDouble applyThreshold(vecDouble vector);
    void calcWeightUpdates(layerPtr layer, vecDouble desiredOut = vecDouble());
};

}
#endif	/* _BACKPROPTRAINER_H */

