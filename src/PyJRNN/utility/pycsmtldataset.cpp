/********************************************************************
	created:	2012/08/13
	filename: 	pycsmtldataset.cpp
	author:		jreeder
	
	purpose:	Wrapper for the csmtldataset class
*********************************************************************/

#include <boost/python.hpp>
#include <utility/csmtldataset.h>

using namespace JRNN;
using namespace boost::python;

void (CSMTLDataset::*dist1)(strings) = &CSMTLDataset::DistSubview;
void (CSMTLDataset::*dist2)(strings, int, int, int) = &CSMTLDataset::DistSubview;

vecDouble (CSMTLDataset::*getRandC)() = &CSMTLDataset::GetRandContext;
vecDouble (CSMTLDataset::*getRandC2)(strings) = &CSMTLDataset::GetRandContext;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CSMTLDataset_DDOverloads, DistData, 3,6);

void exportCSMTLDataset(){

	class_<CSMTLDataset, CSMTLDatasetPtr, bases<Dataset> >("CSMTLDataset").
		add_property("View", &CSMTLDataset::GetView, &CSMTLDataset::SetView).
		def("AddTaskFromFile", &CSMTLDataset::AddTaskFromFile).
		def("AddTaskFromNet", &CSMTLDataset::AddTaskFromNet).
		def("AddMatDoublesToTask", &CSMTLDataset::AddMatDoublesToTask).
		def("AddVecDoublesToTask", &CSMTLDataset::AddVecDoublesToTask).
		def("SpawnDS", &CSMTLDataset::SpawnDS).
		def("DistSubview", dist1).
		def("DistSubview", dist2).
		add_property("ViewSize", &CSMTLDataset::GetViewSize).
		add_property("isConceptData", &CSMTLDataset::GetConceptData, &CSMTLDataset::SetConceptData).
		def("DistData", &CSMTLDataset::DistData, CSMTLDataset_DDOverloads()).
		def("ResetView", &CSMTLDataset::ResetView).
		def("GetRandContext", getRandC).
		def("GetRandContext", getRandC2).
		def("GetTaskNames", &CSMTLDataset::GetTaskNames)
		;
}