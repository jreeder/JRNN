/********************************************************************
	created:	2012/07/26
	created:	26:7:2012   10:45
	filename: 	PyJRNN.cpp
	author:		jreeder
	
	purpose:	Main file for the Python binding of the JRNN library
*********************************************************************/

#include <boost/python.hpp>
//#define PY_ARRAY_UNIQUE_SYMBOL PyJRNN_unique
#include "JRNNTypeDefs.h"

using namespace boost::python;

char const* greet(unsigned x)
{
	static char const* const msgs[] = { "hello", "Boost.Python", "world!" };
		
	if (x > 2) 
		throw std::range_error("greet: index out of range");

	return msgs[x];
}

void exportTemp(){
	object module(handle<>(borrowed(PyImport_AddModule("PyJRNN.networks"))));
	scope().attr("networks") = module;
	scope module_scope = module;

	def("greet", greet, "return one of 3 parts of a greeting");
}

void exportNetworks();

BOOST_PYTHON_MODULE(PyJRNN)
{
	object package = scope();
	package.attr("__path__") = "PyJRNN";
	
	exportTypeDefs();

	object module(handle<>(borrowed(PyImport_AddModule("PyJRNN.networks"))));
	scope().attr("networks") = module;
	scope module_scope = module;
	exportNetworks();

}