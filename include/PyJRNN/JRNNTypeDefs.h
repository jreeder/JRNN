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

void IndexError() {PyErr_SetString(PyExc_IndexError, "Index out of range");}

template<class T>
struct std_item {
	typedef typename T::value_type V;

	static V& get(T& x, unsigned int i){
		if (i < 0) i += x.size();
		if (i >= 0 && i < x.size()) return x[i];
		IndexError();
	}

	static void set(T& x, unsigned int i, V const& v){
		if(i < 0) i += x.size();
		if(i >= 0 && i < x.size()) x[i] = v;
		else IndexError();
	}

	static void del(T& x, unsigned int i){
		if(i < 0) i += x.size();
		if(i >= 0 && i < x.size()) x.erase(x.begin() + i);
		else IndexError();
	}

	static void add(T& x, V& v){
		x.push_back(v);
	}
};

void exportTypeDefs(){

	class_<doubles>("doubles").
		def(vector_indexing_suite<doubles>());

	class_<strings>("strings").
		def(vector_indexing_suite<strings>());

	class_<ints>("ints").
		def(vector_indexing_suite<ints>());


	class_<matDouble>("matDouble").
		def("__len__", &matDouble::size).
		def("clear", &matDouble::clear).
		def("append", &std_item<matDouble>::add,
			with_custodian_and_ward<1,2>()). // To let container keep value
		def("__getitem__", &std_item<matDouble>::get,
			return_value_policy<copy_non_const_reference>()).
		def("__setitem__", &std_item<matDouble>::set,
			with_custodian_and_ward<1,2>()). // To let container keep value
		def("__delitem__", &std_item<matDouble>::del)
		;
}

#endif
