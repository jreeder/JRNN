/*
 * KBCCTrainer.h
 *
 *  Created on: Mar 31, 2013
 *      Author: jreeder
 */

#ifndef KBCCTRAINER_H_
#define KBCCTRAINER_H_

#include "JRNN.h"
#include "trainers/CCTrainer.h"
#include "networks/KBCCNetwork.h"
#include "structure/networknode.h"

namespace JRNN {

	//class KBCCTrainer;
	//typedef boost::shared_ptr<KBCCTrainer> KBCCTrainerPtr;

	class KBCCTrainer : public CCTrainer {
	public:
		KBCCTrainer(CCNetworkPtr network, DatasetPtr data, int numCandidates, NetPtrList SubNetList = NetPtrList(), int numCopies = 0, ints primaryIndexes = ints(0));
		~KBCCTrainer();

		virtual void AddNewInputs( ints inputIndexes, DatasetPtr newData, bool connectToHidden = false );

		virtual double TestOnData( Dataset::datatype type );

		virtual hashedDoubleMap TestWiClass( Dataset::datatype type );

		void AddSubNet(NetworkPtr newNet, string netName = "");
		void ClearSubNetList();
		int GetNumCopies() const;
		void SetNumCopies(int val);
		virtual void Reset();
		
	protected:
		NetPtrList SubNetlist;
		int numCopies;
		
		virtual status TrainOuts();

		virtual void CreateCandidates();

		virtual void UpdateCorrelations();

		virtual void ComputeCandSlopes();

		virtual void InsertCandidate();

		virtual vecDouble ActivateNet( vecDouble inPoint, vecDouble outPoint );

		virtual void ComputeCorrelations();

		void RemoveSubNet( string netName );

		virtual void SetBCorr( NodePtr inBestCand );

		virtual void SwapBPCorr( NodePtr inBestCand );

	};
}

#endif /* KBCCTRAINER_H_ */
