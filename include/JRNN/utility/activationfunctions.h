/* 
 * File:   activationfunctions.h
 * Author: jreeder
 * 
 * Created on July 9, 2010, 1:00 PM
 */
#pragma once
#ifndef _ACTIVATION_FUNCTIONS_H
#define _ACTIVATION_FUNCTIONS_H

#include "JRNN.h"

namespace JRNN { namespace ActivationFunctions {
	class BaseActivationFunction {
	public:
		static std::string getType() {
			return _type;
		}
	private:
		static const std::string _type;
	};
	

	template <class U>
	class ActivationFunction {
	public:
		ActivationFunction(){};
		~ActivationFunction(){};

		double activate(double sum){
			return U::activate(sum);
		}

		double prime(double value, double sum){
			return U::prime(value, sum);
		}

		std::string getType() {
			return U::getType();
		}
	};

	class Sigmoid : BaseActivationFunction {
	public:
		static double activate(double sum){
			if  ( sum < -15.0 )
				return -0.5;
			if  ( sum > 15.0 )
				return 0.5;
			return  ( 1.0 / (1.0 + exp( -sum )) - 0.5 );
		}
		
		static double prime(double value, double sum){
			return  ( 0.25 - value * value );
		}
	private:
		  static const std::string _type;
	};
	

	class ASigmoid : BaseActivationFunction
	{
	public:
		static double activate(double sum){
			if  ( sum < -15.0 )
				return 0.000001;
			if  ( sum > 15.0 )
				return 0.999999;
			return  ( 1.0 / (1.0 + exp( -sum  )) );
		}
		static double prime(double value, double sum){
			return  ( value * ( 1.0 - value ) );
		}
	private:
		static const std::string _type;
	};
	

	class Gaussian : BaseActivationFunction
	{
	public:
		static double activate(double sum){
			double temp = -0.5 * sum * sum;
			if  ( temp < -75.0 )
				return 0.0;
			else
				return ( exp( temp ) );
		}
		static double prime(double value, double sum){
			return  ( sum * (-value) );
		}
	private:
		static const std::string _type;
	};
	

	class Bias : BaseActivationFunction {
	public:
		static double activate(double sum){
			return 1;
		}
		static double prime(double value, double sum){
			return 1;
		}
	private:
		static const std::string _type;
	};
	

	class Linear : BaseActivationFunction {
	public:
		static double activate(double sum){
			return( sum );
		}
		static double prime(double value, double sum){
			return 1.0;
		}
	private:
		static const std::string _type;
	};
	

}}

#endif