/********************************************************************
	created:	2011/06/07
	created:	7:6:2011   17:32
	filename:	MTLCCTrainer.h
	author:		jreeder
	
	purpose:	MTL specific CCTrainer. This will handle any changes
				to the CCTrainer. 
*********************************************************************/

#include "JRNN.h"
#include "trainers/CCTrainer.h"


namespace JRNN {
	class MTLCCTrainer : public CCTrainer
	{
	public:
		MTLCCTrainer(CCNetworkPtr network, DatasetPtr data, int numCandidates, ints primaryIndexes = ints(0));
		~MTLCCTrainer();

		struct mtlparameters {
			double primaryWeight;
			double secondaryWeight;
			bool weightCandScore;
		} mtlparams;
	protected:
		
	private:
		virtual void UpdateCorrelations();
		virtual void ComputeCandSlopes();
		virtual void CorrelationEpoch();
	};
}