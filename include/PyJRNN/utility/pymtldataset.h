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
//void (MTLDataset::*ImpPrim11)(double) = &MTLDataset::ImpoverishPrimaryTaskTraining;
//void ImpPrim11(double x){ MTLDataset md; (md.* ImpPrim1)(x,0); }
void (MTLDataset::*ImpPrim2)(int, unsigned int) = &MTLDataset::ImpoverishPrimaryTaskTraining;
//void (MTLDataset::*ImpPrim21)(int) = &MTLDataset::ImpoverishPrimaryTaskTraining;
//void ImpPrim21(int x) { MTLDataset md; (md.* ImpPrim2)(x, 0); }

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(MTLDataset_ImpPrimOverlaods, ImpoverishPrimaryTaskTraining, 1,2);

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(MTLDataset_DistDataOverloads, DistData, 3, 6);

void exportMTLDataset(){

	class_<MTLDataset, MTLDatasetPtr, bases<Dataset> >("MTLDataset").
		add_property("View", &MTLDataset::GetView, &MTLDataset::SetView).
		def("GetIndexes", &MTLDataset::GetIndexes).
		def("AddTaskFromFile", &MTLDataset::AddTaskFromFile).
		def("AddTaskFromNet", &MTLDataset::AddTaskFromNet).
		def("ImpoverishPrimaryTaskTraining", ImpPrim1, MTLDataset_ImpPrimOverlaods()).
		def("ImpoverishPrimaryTaskTraining", ImpPrim2, MTLDataset_ImpPrimOverlaods()).
		def("SpawnDS", &MTLDataset::SpawnDS).
		def("DistData", &MTLDataset::DistData, MTLDataset_DistDataOverloads())
		;
}
#endif