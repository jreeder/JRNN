/********************************************************************
	created:	2012/08/20
	filename: 	pyrevcctrainer.cpp
	author:		jreeder
	
	purpose:	wrapper for the revcctrainer class.
*********************************************************************/

#include <boost/python.hpp>
#include <trainers/RevCCTrainer.h>
#include <trainers/CCTrainer.h>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace JRNN;
using namespace boost::python;

void exportRevCCTrainer(){

	class_<RevCCTrainer::revparameters>("revparameters").
		def_readwrite("numRev", &RevCCTrainer::revparameters::numRev).
		def_readwrite("numRevTrainRatio", &RevCCTrainer::revparameters::numRevTrainRatio).
		def_readwrite("bufferSize", &RevCCTrainer::revparameters::bufferSize).
		def_readwrite("cleanReverb", &RevCCTrainer::revparameters::cleanReverb).
		def_readwrite("numContexts", &RevCCTrainer::revparameters::numContexts)
		;

	class_<RevCCTrainer::TestResult>("TestResult").
		def_readwrite("epoch", &RevCCTrainer::TestResult::epoch).
		def_readwrite("result", &RevCCTrainer::TestResult::result)
		;

	class_<RevCCTrainer::TestResults>("TestResults").
		def(vector_indexing_suite<RevCCTrainer::TestResults>());

	class_<RevCCTrainer::netparameters>("netparameters").
		def_readwrite("epochs", &RevCCTrainer::netparameters::epochs).
		def_readwrite("numResets", &RevCCTrainer::netparameters::numResets).
		def_readwrite("numHidLayers", &RevCCTrainer::netparameters::numHidLayers).
		def_readwrite("MSERec", &RevCCTrainer::netparameters::MSERec).
		def_readwrite("VMSERec", &RevCCTrainer::netparameters::VMSERec)
		;

	typedef boost::shared_ptr<RevCCTrainer> RevCCTrainerPtr;

	class_<RevCCTrainer, RevCCTrainerPtr, bases<CCTrainer> >("RevCCTrainer", init<int, int, int>()).
		def("TrainTask", &RevCCTrainer::TrainTask).
		def("TestOnData", &RevCCTrainer::TestOnData).
		def("TestWiClass", &RevCCTrainer::TestWiClass).
		def("Activate", &RevCCTrainer::Activate, "Activates the primary network and returns the output").
		def("SetScaleAndOffset", &RevCCTrainer::SetScaleAndOffset).
		def("SetSDCCandVaryActFunc", &RevCCTrainer::SetSDCCandVaryActFunc).
		def("getTestWhileTrainResults", &RevCCTrainer::getTestWhileTrainResults, return_value_policy<copy_const_reference>()).
		def_readwrite("revparams", &RevCCTrainer::revparams).
		def_readonly("net1vals", &RevCCTrainer::net1vals).
		def_readonly("net2vals", &RevCCTrainer::net2vals)
		;
}