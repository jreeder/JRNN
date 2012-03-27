/********************************************************************
	created:	2012/03/21
	created:	21:3:2012   18:10
	filename: 	RevCCTrainer.h
	author:		jreeder
	
	purpose:	Trainer for reverberating CC. This will have two networks
				that work together to alleviate catastrophic forgetting. 
*********************************************************************/

#ifndef _REVCCTRAINER_H
#define _REVCCTRAINER_H

#include "JRNN.h"
#include "trainers/CCTrainer.h"
#include "networks/RevCCNetwork.h"

namespace JRNN {
	
	class RevCCTrainer : public CCTrainer
	{
	public:
		RevCCTrainer(int numIn, int numOut, int numCandidates);
		~RevCCTrainer();

		struct revparameters {
			uint numRev;
			uint numRevTrainPts;
		} revparams;

		struct reverbdpoint {
			vecDouble inPoint;
			vecDouble outPoint;
		};

		virtual double TestOnData(Dataset::datatype type);
		virtual hashedDoubleMap TestWiClass(Dataset::datatype type);

	protected:
		
		RevCCNetworkPtr net1;
		RevCCNetworkPtr net2;
		RevCCNetworkPtr revNet;

		enum Stage {
			INIT,
			STAGEI,
			STAGEII
		} trainStage;

		enum Phase {
			TRAIN,
			TEST
		} algPhase;

		reverbdpoint ReverberateNetwork();

		virtual vecDouble ActivateNet(vecDouble inPoint, vecDouble outPoint);
		
		vecDouble ConcatVec( vecDouble first, vecDouble second );
		static RandomGenerator01 revRand;
		//conVars assoc;


	};
}

#endif