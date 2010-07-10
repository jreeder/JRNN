/* 
 * File:   activationfunctions.cpp
 * Author: jreeder
 * 
 * Created on July 9, 2010, 1:00 PM
 */

#include "JRNN.h"
#include "utility/activationfunctions.h"

using namespace JRNN;
using namespace ActivationFunctions;

const std::string BaseActivationFunction::_type = "N/A";
const std::string Sigmoid::_type = "SIGMOID";
const std::string ASigmoid::_type = "ASIGMOID";
const std::string Gaussian::_type = "GAUSSIAN";
const std::string Bias::_type = "BIAS";
const std::string Linear::_type = "LINEAR";