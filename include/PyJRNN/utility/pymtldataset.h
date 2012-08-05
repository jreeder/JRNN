/********************************************************************
	created:	2012/08/05
	filename: 	mtldataset.h
	author:		jreeder
	
	purpose:	wrapper for the mtldataset class. 
*********************************************************************/

#ifndef _PYMTLDATASET_H
#define _PYMTLDATASET_H

#include <boost/python.hpp>
#include <utility/mtldataset.h>
#include "utility/pydatasethelper.h"

using namespace JRNN;
using namespace boost::python;

//Need to fix these to have default arguments
void (MTLDataset::*ImpPrim1)(double, unsigned int) = &MTLDataset::ImpoverishPrimaryTaskTraining;
void (MTLDataset::*ImpPrim11)(double) = &MTLDataset::ImpoverishPrimaryTaskTraining;
void (MTLDataset::*ImpPrim2)(int, unsigned int) = &MTLDataset::ImpoverishPrimaryTaskTraining;
void (MTLDataset::*ImpPrim21)(int) = &MTLDataset::ImpoverishPrimaryTaskTraining;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(MTLDataset_DistDataOverloads, DistData, 3, 6);

void exportMTLDataset(){

	class_<MTLDataset, MTLDatasetPtr, bases<Dataset> >("MTLDataset").
		add_property("View", &MTLDataset::GetView, &MTLDataset::SetView).
		def("GetIndexes", &MTLDataset::GetIndexes).
		def("AddTaskFromFile", &MTLDataset::AddTaskFromFile).
		def("AddTaskFromNet", &MTLDataset::AddTaskFromNet).
		def("ImpoverishPrimaryTaskTraining", ImpPrim1).
		def("ImpoverishPrimaryTaskTraining", ImpPrim11).
		def("ImpoverishPrimaryTaskTraining", ImpPrim2).
		def("ImpoverishPrimaryTaskTraining", ImpPrim21).
		def("SpawnDS", &MTLDataset::SpawnDS).
		def("DistData", &MTLDataset::DistData, MTLDataset_DistDataOverloads())
		;
}
#endif