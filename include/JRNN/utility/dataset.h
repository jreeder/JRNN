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

	class Dataset;
	typedef boost::shared_ptr<Dataset> DatasetPtr;

	class DataSetArchiver;

    class Dataset {

    public:
		typedef boost::unordered_map<std::string, std::vector<int> > hashedIntsMap;

		friend class DataSetArchiver;

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
		void LoadFromMatDoubles(matDouble& newInputs, matDouble& newOutputs);
		const int GetSize(datatype type);
		double GetStdDev(datatype type);
        void SetNumInputs(int numInputs);
        void SetNumOutputs(int numOutputs);
		void SetOutputPerCategory(bool outPerClass);
		void SetNormalizeReals(bool normReals);
		void MergeSubsets(DatasetPtr dsPtr, bool resetSubsetSize = false);
        virtual void DistData(int numTrain, int numVal, int numTest);
        virtual void RedistData();
		void Clear();
		bool AllowShuffle() const;
		void AllowShuffle(bool val);

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
		bool allowShuffle;
		std::vector<int> randomRange;
		static RandomGeneratorInt dRand;
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
        virtual void Distribute();
		virtual void FillSubset( matDouble& ins, matDouble& outs, int numExamples, hashedIntsMap& indexQueues );
		void CalcStdDevs();
		double CalcStdDev(datatype type);
		void Shuffle(ints &indexes);
		void ShuffleSubsets();
		void ClearSubsets();
		virtual void Reshuffle();
		virtual void AnalyzeDS();
		ints ARange(int start, int end);

    };

}

#endif	/* _DATASET_H */

