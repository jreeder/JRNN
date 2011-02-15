/* 
 * File:   dataset.h
 * Author: jreeder
 *
 * Created on May 27, 2010, 4:47 PM
 */

#ifndef _DATASET_H
#define	_DATASET_H

#include "JRNN.h"
#include <fstream>

namespace JRNN {

    class Dataset {

    public:
		typedef boost::unordered_map<std::string, std::vector<int> > hashedIntsMap;

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
        void LoadFromFile(string filepath, int numInputs, int numOutputs);
		const int GetSize(datatype type);
		double GetStdDev(datatype type);
        void SetNumInputs(int numInputs);
        void SetNumOutputs(int numOutputs);
		void SetOutputPerCategory(bool outPerClass);
		void SetNormalizeReals(bool normReals);
        void DistData(int numTrain, int numVal, int numTest);
        void RedistData();

    protected:
        int size;
        int numInputs;
        int numOutputs;
        int numTrain;
        int numVal;
        int numTest;
        int randSeed;
		bool outputPerCategory;
		bool normalizeReals;
		bool dsAnalyzed;
        std::vector<int> randomRange;
		double trainStdDev;
		double testStdDev;
		double valStdDev;
		double stdDev;
        matDouble inputs;
        matDouble outputs;
        matDouble trainIns;
        matDouble trainOuts;
        matDouble valIns;
        matDouble valOuts;
        matDouble testIns;
        matDouble testOuts;
		hashedIntsMap outClassIndexes;
		hashedDoubleMap outClassPercentage;
		strings outClassNames;
        void GenRandRange();
        void Distribute();
		void CalcStdDevs();
		double CalcStdDev(datatype type);
		void Shuffle(ints &indexes);
		void ShuffleSubsets();
		void Reshuffle();
		void AnalyzeDS();

    };

}

#endif	/* _DATASET_H */

