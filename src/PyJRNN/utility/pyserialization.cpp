/********************************************************************
	created:	2012/08/13
	filename: 	pyserialization.cpp
	author:		jreeder
	
	purpose:	python wrapper for my serialization routines. 
*********************************************************************/

#include <boost/python.hpp>
#include <utility/serialization.h>

using namespace JRNN;
using namespace boost::python;

void exportSerialization(){

	class_<JSONArchiver>("JSONArchiver").
		def("SaveToFile", &JSONArchiver::SaveToFile).
		def("LoadFromFile", &JSONArchiver::LoadFromFile)
		;
}