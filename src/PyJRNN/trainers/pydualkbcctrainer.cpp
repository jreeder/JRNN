/********************************************************************
	created:	2013/05/29
	created:	29:5:2013   23:11
	filename: 	pydualkbcctrainer.cpp
	author:		jreeder
	
	purpose:	python wrapper for dualkbcctrainer
*********************************************************************/

#include <boost/python.hpp>
#include <trainers/DualKBCCTrainer.h>
#include <trainers/KBCCTrainer.h>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace JRNN;
using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(DualKBCCTrainer_traintaskoverloads, TrainTask, 3, 6);

void exportDualKBCCTrainer(){

	class_<DualKBCCTrainer::revparameters>("dkrevparameters").
		def_readwrite("numRev", &DualKBCCTrainer::revparameters::numRev).
		def_readwrite("numRevTrainRatio", &DualKBCCTrainer::revparameters::numRevTrainRatio).
		def_readwrite("bufferSize", &DualKBCCTrainer::revparameters::bufferSize).
		def_readwrite("cleanReverb", &DualKBCCTrainer::revparameters::cleanReverb).
		def_readwrite("numContexts", &DualKBCCTrainer::revparameters::numContexts)
		;

	class_<DualKBCCTrainer::TestResult>("dkTestResult").
		def_readwrite("epoch", &DualKBCCTrainer::TestResult::epoch).
		def_readwrite("result", &DualKBCCTrainer::TestResult::result)
		;

	class_<DualKBCCTrainer::TestResults>("dkTestResults").
		def(vector_indexing_suite<DualKBCCTrainer::TestResults>());

	class_<DualKBCCTrainer::netparameters>("dknetparameters").
		def_readwrite("epochs", &DualKBCCTrainer::netparameters::epochs).
		def_readwrite("numResets", &DualKBCCTrainer::netparameters::numResets).
		def_readwrite("numHidLayers", &DualKBCCTrainer::netparameters::numHidLayers).
		def_readwrite("MSERec", &DualKBCCTrainer::netparameters::MSERec).
		def_readwrite("VMSERec", &DualKBCCTrainer::netparameters::VMSERec)
		;

	typedef boost::shared_ptr<DualKBCCTrainer> DualKBCCTrainerPtr;

	class_<DualKBCCTrainer, DualKBCCTrainerPtr, bases<KBCCTrainer> >("DualKBCCTrainer", init<int, int, int, optional<string> >()).
		def("TrainTask", &DualKBCCTrainer::TrainTask, DualKBCCTrainer_traintaskoverloads()).
		def("TestOnData", &DualKBCCTrainer::TestOnData).
		def("TestWiClass", &DualKBCCTrainer::TestWiClass).
		def("Reset", &DualKBCCTrainer::Reset).
		def("Activate", &DualKBCCTrainer::Activate, "Activates the primary network and returns the output").
		def("SetScaleAndOffset", &DualKBCCTrainer::SetScaleAndOffset).
		def("SetSDCCandVaryActFunc", &DualKBCCTrainer::SetSDCCandVaryActFunc).
		def("SetUseRecurrency", &DualKBCCTrainer::SetUseRecurrence).
		def("getTestWhileTrainResults", &DualKBCCTrainer::getTestWhileTrainResults, return_value_policy<copy_const_reference>()).
		def("AddNewInputs", &DualKBCCTrainer::AddNewInputs).
		def("AddPrevTrainedNets", &DualKBCCTrainer::AddPrevTrainedNets).
		def_readwrite("revparams", &DualKBCCTrainer::revparams).
		def_readonly("net1vals", &DualKBCCTrainer::net1vals).
		def_readonly("net2vals", &DualKBCCTrainer::net2vals).
		add_property("net1", &DualKBCCTrainer::GetNet1).
		add_property("net2", &DualKBCCTrainer::GetNet2)
		;
}