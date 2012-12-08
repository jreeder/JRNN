/********************************************************************
	created:	2012/08/05
	filename: 	pydataset.cpp
	author:		jreeder
	
	purpose:	Wrapper for the dataset class.
*********************************************************************/


#include <boost/python.hpp>
#include <utility/dataset.h>
#include "utility/pydatasethelper.h"

using namespace boost::python;
using namespace JRNN;

void exportMTLDataset();
void exportCSMTLDataset();

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(Dataset_lff_overloads, LoadFromFile, 3, 4);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(Dataset_lfmd_overloads, LoadFromMatDoubles, 2,3);

void exportDatasets(){

	class_<DSWrap, DatasetPtr>("Dataset").
		def("LoadFromFile", &Dataset::LoadFromFile, Dataset_lff_overloads()).
		def("LoadFromMatDoubles", &Dataset::LoadFromMatDoubles, Dataset_lfmd_overloads()).
		def("DistData", &Dataset::DistData, &DSWrap::default_DistData).
		def("RedistData", &Dataset::RedistData, &DSWrap::default_RedistData).
		def("GetSize", &Dataset::GetSize).
		def("GetStdDev", &Dataset::GetStdDev).
		def("Clear", &Dataset::Clear).
		def("GetInputs", &Dataset::GetInputs, return_value_policy<copy_const_reference>()).
		def("GetOutputs", &Dataset::GetOutputs, return_value_policy<copy_const_reference>()).
		add_property("numInputs", &Dataset::GetNumInputs).
		add_property("numOutputs", &Dataset::GetNumOutputs).
		add_property("realOuts", &Dataset::getRealOuts, &Dataset::setRealOuts)
		;

	enum_<Dataset::datatype>("DSDatatype").
		value("TRAIN", Dataset::TRAIN).
		value("TEST", Dataset::TEST).
		value("VAL", Dataset::VAL).
		value("ALL", Dataset::ALL).
		export_values()
		;

	exportMTLDataset();
	exportCSMTLDataset();
}