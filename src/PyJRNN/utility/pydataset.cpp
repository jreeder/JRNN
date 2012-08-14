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

void exportDatasets(){

	class_<DSWrap, DatasetPtr>("Dataset").
		def("LoadFromFile", &Dataset::LoadFromFile).
		//def("LoadFromMatDoubles", &Dataset::LoadFromMatDoubles).
		def("DistData", &Dataset::DistData, &DSWrap::default_DistData).
		def("RedistData", &Dataset::RedistData, &DSWrap::default_RedistData).
		def("GetSize", &Dataset::GetSize).
		def("GetStdDev", &Dataset::GetStdDev).
		def("Clear", &Dataset::Clear)
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