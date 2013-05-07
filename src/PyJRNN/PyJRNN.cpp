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
void exportDatasets();
void exportSerialization();
void exportCCTrainer();
void exportRevCCTrainer();
void exportBackPropTrainer();
void exportRPropTrainer();
void exportKBCCTrainer();

#ifdef _DEBUG
	#define MODNAME "PyJRNN_d"
#else
	#define MODNAME "PyJRNN"
#endif

#ifdef _DEBUG
BOOST_PYTHON_MODULE(PyJRNN_d)
#else
BOOST_PYTHON_MODULE(PyJRNN)
#endif
{
	object package = scope();
	package.attr("__path__") = "PyJRNN";
	
	object module0(handle<>(borrowed(PyImport_AddModule(MODNAME ".types"))));
	package.attr("types") = module0;
	scope module0_scope = module0;
	exportTypeDefs();

	object module1(handle<>(borrowed(PyImport_AddModule(MODNAME ".networks"))));
	package.attr("networks") = module1;
	scope module1_scope = module1;
	exportNetworks();

	object module2(handle<>(borrowed(PyImport_AddModule(MODNAME ".utility"))));
	package.attr("utility") = module2;
	scope module2_scope = module2;
	exportDatasets();
	exportSerialization();

	object module3(handle<>(borrowed(PyImport_AddModule(MODNAME ".trainers"))));
	package.attr("trainers") = module3;
	scope module3_scope = module3;
	exportCCTrainer();
	exportRevCCTrainer();
	exportBackPropTrainer();
	exportRPropTrainer();
	exportKBCCTrainer();
}