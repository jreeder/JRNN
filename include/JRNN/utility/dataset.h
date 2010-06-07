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
#include <iostream>
#include <fstream>

namespace JRNN {

    class dataset {

    public:

        enum datatype {
            TRAIN,
            VAL,
            TEST,
            ALL
        };

        dataset();
        dataset(const dataset& orig);
        virtual ~dataset();
        const matDouble& getInputs(datatype type);
        const matDouble& getOutputs(datatype type);
        void loadFromFile(std::string filepath, int numInputs, int numOutputs);
        const int getSize();
        void setNumInputs(int numInputs);
        void setNumOutputs(int numOutputs);
        void distData(int numTrain, int numVal, int numTest);
        void redistData();

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

        void genRandRange();
        void distribute();

    };

}
#endif	/* _DATASET_H */

