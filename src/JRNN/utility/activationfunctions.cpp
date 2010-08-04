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