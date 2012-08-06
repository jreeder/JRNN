/********************************************************************
	created:	30:7:2012   14:37
	filename: 	JRNNTypeDefs.h
	author:		jreeder
	
	purpose:	Provide wrappers for all the standard types and boost 
				types used in JRNN

*********************************************************************/
#pragma once;

#ifndef _JRNNTYPEDEFS_H
#define _JRNNTYPEDEFS_H

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#define JRNN_DO_IMPORT
#include <JRNN.h>

using namespace JRNN;
using namespace boost::python;

void exportTypeDefs(){

	class_<doubles>("doubles").
		def(vector_indexing_suite<doubles>());

	class_<strings>("strings").
		def(vector_indexing_suite<strings>());

	class_<ints>("ints").
		def(vector_indexing_suite<ints>());

	//Need to figure out how to convert matDoubles from lists of numpy arrays this doesn't 
	//work
	//class_<matDouble>("matDouble").
	//	def(vector_indexing_suite<matDouble>());
}

#endif
