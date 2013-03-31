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

namespace JRNN {

	class KBCCTrainer;
	typedef boost::shared_ptr<KBCCTrainer> KBCCTrainerPtr;

	class KBCCTrainer: public CCTrainer {

	};
}

#endif /* KBCCTRAINER_H_ */
