/********************************************************************
	created:	2012/08/20
	created:	20:8:2012   17:44
	filename: 	pybackproptrainer.cpp
	author:		jreeder
	
	purpose:	Wrapper for back prop trainer
*********************************************************************/


#include <boost/python.hpp>
#include <trainers/BackPropTrainer.h>

using namespace JRNN;
using namespace boost::python;

void exportBackPropTrainer() {

	typedef boost::shared_ptr<BackPropTrainer> BackPropTrainerPtr;

	class_<BackPropTrainer, BackPropTrainerPtr>("BackPropTrainer", init<FFMLPNetPtr, DatasetPtr, double>()).
		def("TestOnData", &BackPropTrainer::TestOnData).
		def("TestWiClass", &BackPropTrainer::TestWiClass).
		def("GetMSERec", &BackPropTrainer::GetMSERec, return_value_policy<copy_non_const_reference>()).
		def("GetVMSERec", &BackPropTrainer::GetVMSERec, return_value_policy<copy_non_const_reference>()).
		def("GetEpochs", &BackPropTrainer::GetEpochs).
		def("TrainToConvergence", &BackPropTrainer::TrainToConvergence).
		def("TrainToValConv", &BackPropTrainer::TrainToValConv).
		def("Reset", &BackPropTrainer::Reset)
		;

}