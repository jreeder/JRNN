/********************************************************************
	created:	2012/08/13
	filename: 	cctrainer.cpp
	author:		jreeder
	
	purpose:	Python wrapper for the cctrainer class. 
*********************************************************************/

#include <boost/python.hpp>
#include <trainers/CCTrainer.h>

using namespace JRNN;
using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CCTrainer_ttcoverloads, TrainToConvergence, 1,2);

struct CCTrainerWrap : CCTrainer, wrapper<CCTrainer> {

	double TestOnData(Dataset::datatype type) {
		if (override TestOnData = this->get_override("TestOnData"))
			return TestOnData(type);
		return CCTrainer::TestOnData(type);
	}

	double default_TestOnData(Dataset::datatype type) {return this->CCTrainer::TestOnData(type);}

	hashedDoubleMap TestWiClass(Dataset::datatype type) {
		if(override TestWiClass = this->get_override("TestWiClass"))
			return TestWiClass(type);
		return CCTrainer::TestWiClass(type);
	}

	hashedDoubleMap default_TestWiClass(Dataset::datatype type) {return this->CCTrainer::TestWiClass(type);}

	void Reset() {
		if (override Reset = this->get_override("Reset")){
			Reset();
		}
		else {
			CCTrainer::Reset();
		}
	}

	void default_Reset() {this->CCTrainer::Reset();}
};

void exportCCTrainer(){

	enum_<CCTrainer::ErrorType>("ErrorType").
		value("BITS", CCTrainer::BITS).
		value("INDEX", CCTrainer::INDEX).
		export_values()
		;

	class_<CCTrainer::parameters::outParms>("outParms").
		def_readwrite("epochs", &CCTrainer::parameters::outParms::epochs).
		def_readwrite("patience", &CCTrainer::parameters::outParms::patience).
		def_readwrite("epsilon", &CCTrainer::parameters::outParms::epsilon).
		def_readwrite("scaledEpsilon", &CCTrainer::parameters::outParms::scaledEpsilon).
		def_readwrite("shrinkFactor", &CCTrainer::parameters::outParms::shrinkFactor).
		def_readwrite("decay", &CCTrainer::parameters::outParms::decay).
		def_readwrite("mu", &CCTrainer::parameters::outParms::mu).
		def_readwrite("changeThreshold", &CCTrainer::parameters::outParms::changeThreshold)
		;

	class_<CCTrainer::parameters::candParms>("candParms").
		def_readwrite("epochs", &CCTrainer::parameters::candParms::epochs).
		def_readwrite("patience", &CCTrainer::parameters::candParms::patience).
		def_readwrite("epsilon", &CCTrainer::parameters::candParms::epsilon).
		def_readwrite("shrinkFactor", &CCTrainer::parameters::candParms::shrinkFactor).
		def_readwrite("decay", &CCTrainer::parameters::candParms::decay).
		def_readwrite("mu", &CCTrainer::parameters::candParms::mu).
		def_readwrite("changeThreshold", &CCTrainer::parameters::candParms::changeThreshold)
		;

	class_<CCTrainer::parameters>("parameters").
		def_readwrite("nTrials", &CCTrainer::parameters::nTrials).
		def_readwrite("maxNewUnits", &CCTrainer::parameters::maxNewUnits).
		def_readwrite("valPatience", &CCTrainer::parameters::valPatience).
		def_readwrite("impPatience", &CCTrainer::parameters::impPatience).
		def_readwrite("weightMult", &CCTrainer::parameters::weightMult).
		def_readwrite("maxWeight", &CCTrainer::parameters::maxWeight).
		def_readwrite("useMaxWeight", &CCTrainer::parameters::useMaxWeight).
		def_readwrite("maxResets", &CCTrainer::parameters::maxResets).
		def_readwrite("primeOffset", &CCTrainer::parameters::primeOffset).
		def_readwrite("useSDCC", &CCTrainer::parameters::useSDCC).
		def_readwrite("SDCCRatio", &CCTrainer::parameters::SDCCRatio).
		def_readwrite("out", &CCTrainer::parameters::out).
		def_readwrite("cand", &CCTrainer::parameters::cand).
		def_readwrite("indexThreshold", &CCTrainer::parameters::indexThreshold).
		def_readwrite("scoreThreshold", &CCTrainer::parameters::scoreThreshold).
		def_readwrite("errorMeasure", &CCTrainer::parameters::errorMeasure).
		def_readwrite("nCand", &CCTrainer::parameters::nCand)
		;

	typedef boost::shared_ptr<CCTrainer> CCTrainerPtr;

	class_<CCTrainerWrap, CCTrainerPtr, boost::noncopyable>("CCTrainer",init<CCNetworkPtr,DatasetPtr,int,optional<ints> >()).
		def("ResetVars", &CCTrainer::ResetVars).
		def("Reset", &CCTrainer::Reset, &CCTrainerWrap::default_Reset).
		def("RedistData", &CCTrainer::RedistData).
		def("SetDataSet", &CCTrainer::SetDataSet).
		def("AddNewInputs", &CCTrainer::AddNewInputs).
		def("TrainToValConv", &CCTrainer::TrainToValConv).
		def("TrainToConvergence", &CCTrainer::TrainToConvergence, CCTrainer_ttcoverloads()).
		def("TestOnData", &CCTrainer::TestOnData, &CCTrainerWrap::default_TestOnData).
		def("TestWiClass", &CCTrainer::TestWiClass, &CCTrainerWrap::default_TestWiClass).
		add_property("Epochs", &CCTrainer::GetEpochs).
		add_property("NumResets", &CCTrainer::GetNumResets).
		add_property("NumHidLayers", &CCTrainer::GetNumHidLayers).
		add_property("useTrainOutVal", &CCTrainer::GetUseTrainOutVal, &CCTrainer::SetUseTrainOutVal).
		add_property("useHoldBestCand", &CCTrainer::GetUseHoldBestCand, &CCTrainer::SetUseHoldBestCand).
		def("GetMSERec", &CCTrainer::GetMSERec,return_value_policy<copy_non_const_reference>()).
		def("GetVMSERec", &CCTrainer::GetVMSERec, return_value_policy<copy_non_const_reference>()).
		def_readwrite("parms", &CCTrainer::parms)
		;

}