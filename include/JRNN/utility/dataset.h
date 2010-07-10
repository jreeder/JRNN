/* 
 * File:   dataset.h
 * Author: jreeder
 *
 * Created on May 27, 2010, 4:47 PM
 */

#ifndef _DATASET_H
#define	_DATASET_H

#include "JRNN.h"
#include <boost/tokenizer.hpp>
#include <fstream>

namespace JRNN {

    class Dataset {

    public:

        enum datatype {
            TRAIN,
            VAL,
            TEST,
            ALL
        };

        Dataset();
        Dataset(const Dataset& orig);
        virtual ~Dataset();
        const matDouble& GetInputs(datatype type);
        const matDouble& GetOutputs(datatype type);
        void LoadFromFile(std::string filepath, int numInputs, int numOutputs);
        const int GetSize();
        void SetNumInputs(int numInputs);
        void SetNumOutputs(int numOutputs);
        void DistData(int numTrain, int numVal, int numTest);
        void RedistData();

    private:
        int size;
        int numInputs;
        int numOutputs;
        int numTrain;
        int numVal;
        int numTest;
        int randSeed;
        std::vector<int> randomRange;
        matDouble inputs;
        matDouble outputs;
        matDouble trainIns;
        matDouble trainOuts;
        matDouble valIns;
        matDouble valOuts;
        matDouble testIns;
        matDouble testOuts;

        void GenRandRange();
        void Distribute();

    };

}
#endif	/* _DATASET_H */

