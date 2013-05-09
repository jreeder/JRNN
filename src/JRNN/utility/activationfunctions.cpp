/* 
 * File:   activationfunctions.cpp
 * Author: jreeder
 * 
 * Created on July 9, 2010, 1:00 PM
 */
#include "JRNN.h"
#include "utility/activationfunctions.h"

using namespace JRNN;

const string ActivationFunction::_type = "N/A";
const string Sigmoid::_type = "SIGMOID";
const string ASigmoid::_type = "ASIGMOID";
const string Gaussian::_type = "GAUSSIAN";
const string Bias::_type = "BIAS";
const string Linear::_type = "LINEAR";

ActivationFunction* ActivationFunction::Create( string ActType )
{
	ActivationFunction* tmpAct;

	string oldActName = ActType;
	if(oldActName == "SIGMOID"){
		tmpAct = new Sigmoid();
	}
	else if(oldActName == "ASIGMOID"){
		tmpAct = new ASigmoid();
	}
	else if(oldActName == "GAUSSIAN"){
		tmpAct = new Gaussian();
	}
	else if(oldActName == "BIAS"){
		tmpAct = new Bias();
	}
	else if(oldActName == "LINEAR"){
		tmpAct = new Linear();
	}
	else {
		tmpAct = NULL;
	}

	return tmpAct;
}
