/********************************************************************
	created:	2013/05/29
	created:	29:5:2013   21:49
	filename: 	pyrevkbcctrainer.cpp
	author:		jreeder
	
	purpose:	python wrapper for revkbcctrainer
*********************************************************************/

#include <boost/python.hpp>
#include <trainers/RevKBCCTrainer.h>
#include <trainers/KBCCTrainer.h>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace JRNN;
using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(RevKBCCTrainer_traintaskoverloads, TrainTask, 3, 6);

void exportRevKBCCTrainer(){

	class_<RevKBCCTrainer::revparameters>("rkrevparameters").
		def_readwrite("numRev", &RevKBCCTrainer::revparameters::numRev).
		def_readwrite("numRevTrainRatio", &RevKBCCTrainer::revparameters::numRevTrainRatio).
		def_readwrite("bufferSize", &RevKBCCTrainer::revparameters::bufferSize).
		def_readwrite("cleanReverb", &RevKBCCTrainer::revparameters::cleanReverb).
		def_readwrite("numContexts", &RevKBCCTrainer::revparameters::numContexts)
		;

	class_<RevKBCCTrainer::TestResult>("rkTestResult").
		def_readwrite("epoch", &RevKBCCTrainer::TestResult::epoch).
		def_readwrite("result", &RevKBCCTrainer::TestResult::result)
		;

	class_<RevKBCCTrainer::TestResults>("rkTestResults").
		def(vector_indexing_suite<RevKBCCTrainer::TestResults>());

	class_<RevKBCCTrainer::netparameters>("rknetparameters").
		def_readwrite("epochs", &RevKBCCTrainer::netparameters::epochs).
		def_readwrite("numResets", &RevKBCCTrainer::netparameters::numResets).
		def_readwrite("numHidLayers", &RevKBCCTrainer::netparameters::numHidLayers).
		def_readwrite("MSERec", &RevKBCCTrainer::netparameters::MSERec).
		def_readwrite("VMSERec", &RevKBCCTrainer::netparameters::VMSERec)
		;

	typedef boost::shared_ptr<RevKBCCTrainer> RevKBCCTrainerPtr;

	class_<RevKBCCTrainer, RevKBCCTrainerPtr, bases<KBCCTrainer> >("RevKBCCTrainer", init<int, int, int, optional<string, string> >()).
		def("TrainTask", &RevKBCCTrainer::TrainTask, RevKBCCTrainer_traintaskoverloads()).
		def("TestOnData", &RevKBCCTrainer::TestOnData).
		def("TestWiClass", &RevKBCCTrainer::TestWiClass).
		def("Reset", &RevKBCCTrainer::Reset).
		def("Activate", &RevKBCCTrainer::Activate, "Activates the primary network and returns the output").
		def("SetScaleAndOffset", &RevKBCCTrainer::SetScaleAndOffset).
		def("SetSDCCandVaryActFunc", &RevKBCCTrainer::SetSDCCandVaryActFunc).
		def("SetUseRecurrency", &RevKBCCTrainer::SetUseRecurrence).
		def("getTestWhileTrainResults", &RevKBCCTrainer::getTestWhileTrainResults, return_value_policy<copy_const_reference>()).
		def("AddNewInputs", &RevKBCCTrainer::AddNewInputs).
		def("AddPrevTrainedNets", &RevKBCCTrainer::AddPrevTrainedNets).
		def_readwrite("revparams", &RevKBCCTrainer::revparams).
		def_readonly("net1vals", &RevKBCCTrainer::net1vals).
		def_readonly("net2vals", &RevKBCCTrainer::net2vals).
		add_property("net1", &RevKBCCTrainer::GetNet1).
		add_property("net2", &RevKBCCTrainer::GetNet2)
		;
}