/********************************************************************
	created:	2012/08/20
	created:	20:8:2012   18:07
	filename: 	pyrproptrainer.cpp
	author:		jreeder
	
	purpose:	Wrapper for the RPropTrainer
*********************************************************************/

#include <boost/python.hpp>
#include <trainers/RPropTrainer.h>

using namespace JRNN;
using namespace boost::python;

void exportRPropTrainer() {

	typedef boost::shared_ptr<RPropTrainer> RPropTrainerPtr;

	class_<RPropTrainer, RPropTrainerPtr>("RPropTrainer", init<FFMLPNetPtr, DatasetPtr, double, double , optional<ints> >()).
		def("TestOnData", &RPropTrainer::TestOnData).
		def("TestWiClass", &RPropTrainer::TestWiClass).
		def("GetMSERec", &RPropTrainer::GetMSERec, return_value_policy<copy_non_const_reference>()).
		def("GetVMSERec", &RPropTrainer::GetVMSERec, return_value_policy<copy_non_const_reference>()).
		def("GetEpochs", &RPropTrainer::GetEpochs).
		def("TrainToConvergence", &RPropTrainer::TrainToConvergence).
		def("TrainToValConv", &RPropTrainer::TrainToValConv).
		def("Reset", &RPropTrainer::Reset).
		def("RedistData", &RPropTrainer::RedistData).
		def("GetNumResets", &RPropTrainer::GetNumResets).
		def_readwrite("useMaxWeight", &RPropTrainer::useMaxWeight).
		def_readwrite("maxWeight", &RPropTrainer::maxWeight)
		;
}