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
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
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
		return x[0]; //Should never get here, just to get rid of warning
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

	static void add(T& x, V const& v){
		x.push_back(v);
	}
};

//template<class T>
//struct map_item
//{
//	typedef typename T::key_type K;
//	typedef typename T::mapped_type V;
//	static V& get(T const& x, K const& i)
//	{
//		if( x.find(i) != x.end() ) return x[i];
//		KeyError();
//	}
//	static void set(T const& x, K const& i, V const& v)
//	{
//		x[i]=v; // use map autocreation feature
//	}
//	static void del(T const& x, K const& i)
//	{
//		if( x.find(i) != x.end() ) x.erase(i);
//		else KeyError();
//	}
//	static bool in(T const& x, K const& i)
//	{
//		return x.find(i) != x.end();
//	}
//	static list keys(T const& x)
//	{
//		list t;
//		for(T::const_iterator it=x.begin; it!=x.end(); ++it)
//			t.append(it->first);
//		return t;
//	}
//	static list values(T const& x)
//	{
//		list t;
//		for(T::const_iterator it=x.begin; it!=x.end(); ++it)
//			t.append(it->second);
//		return t;
//	}
//	static list items(T const& x)
//	{
//		list t;
//		for(T::const_iterator it=x.begin; it!=x.end(); ++it)
//			t.append(make_tuple(it->first,it->second));
//		return t;
//	}
//};
//void KeyError() { PyErr_SetString(PyExc_KeyError, "Key not found"); }

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
		def("__delitem__", &std_item<matDouble>::del).
		def("__iter__", iterator<matDouble>())
		;

	class_<hashedDoubleMap>("hashedDoubleMap").
		def(map_indexing_suite<hashedDoubleMap>());

}

#endif
