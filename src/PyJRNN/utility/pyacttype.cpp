/********************************************************************
	created:	2013/05/16
	created:	16:5:2013   8:28
	filename: 	pyacttype.cpp
	author:		jreeder
	
	purpose:	Simple container for activation function strings. 
*********************************************************************/

#include <boost/python.hpp>
#include <string>

using namespace boost::python;
using namespace std;

struct ActType {
	static const string GAUSSIAN;
	static const string ASIGMOID;
	static const string SIGMOID;
	static const string BIAS;
	static const string LINEAR;
};

const string ActType::GAUSSIAN = "GAUSSIAN";
const string ActType::ASIGMOID = "ASIGMOID";
const string ActType::SIGMOID = "SIGMOID";
const string ActType::BIAS = "BIAS";
const string ActType::LINEAR = "LINEAR";

void exportActType() {

	class_<ActType>("ActType").
		add_static_property("GAUSSIAN", make_getter(ActType::GAUSSIAN)).
		add_static_property("ASIGMOID", make_getter(ActType::ASIGMOID)).
		add_static_property("SIGMOID", make_getter(ActType::SIGMOID)).
		add_static_property("BIAS", make_getter(ActType::BIAS)).
		add_static_property("LINEAR", make_getter(ActType::LINEAR));

}