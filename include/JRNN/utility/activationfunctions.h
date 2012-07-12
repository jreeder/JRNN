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

namespace JRNN {
	class ActivationFunction {
	public:
		virtual double activate(double sum) = 0;

		virtual double prime(double value, double sum) = 0;
		
		string getType() {
			return _type; //Need to figure out how to make this work for all of my classes. 
		}
	private:
		static const string _type;
	};
	

	/*template <class U>
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

		string getType() {
			return U::getType();
		}
	};*/

	class Sigmoid : public ActivationFunction {
	public:

		double activate(double sum){
			return _activate(sum);
		}

		double prime(double value, double sum){
			return _prime(value, sum);
		}
		
	private:
		  static const string _type;

		  static double _activate(double sum){
			  if  ( sum < -15.0 )
				  return -0.5;
			  if  ( sum > 15.0 )
				  return 0.5;
			  return  ( 1.0 / (1.0 + exp( -sum )) - 0.5 );
		  }

		  static double _prime(double value, double sum){
			  return  ( 0.25 - value * value );
		  }
	};
	

	class ASigmoid : public ActivationFunction
	{
	public:
		double activate(double sum){
			return _activate(sum);
		}

		double prime(double value, double sum){
			return _prime(value, sum);
		}
	private:
		static const string _type;
		static double _activate(double sum){ 
			if  ( sum < -15.0 )
				return 0.0;
			if  ( sum > 15.0 )
				return 1.0;
			double retVal = ( 1.0 / (1.0 + exp( -sum  )) );
			//if (retVal == 0) { //Confuses correlation stuff add prime in trainers that need it. 
			//	retVal = 0.01;
			//}
			//else if (retVal == 1){
			//	retVal = 0.99;
			//}
			return  retVal;
		}
		static double _prime(double value, double sum){
			return  ( value * ( 1.0 - value ) );
		}
	};
	

	class Gaussian : public ActivationFunction
	{
	public:
		double activate(double sum){
			return _activate(sum);
		}

		double prime(double value, double sum){
			return _prime(value, sum);
		}
	private:
		static const string _type;
		static double _activate(double sum){
			double temp = -0.5 * sum * sum;
			if  ( temp < -75.0 )
				return 0.0;
			else
				return ( exp( temp ) );
		}
		static double _prime(double value, double sum){
			return  ( sum * (-value) );
		}
	};
	

	class Bias : public ActivationFunction {
	public:
		double activate(double sum){
			return _activate(sum);
		}

		double prime(double value, double sum){
			return _prime(value, sum);
		}
	private:
		static const string _type;
		static double _activate(double sum){
			return 1;
		}
		static double _prime(double value, double sum){
			return 1;
		}
	};
	

	class Linear : public ActivationFunction {
	public:
		double activate(double sum){
			return _activate(sum);
		}

		double prime(double value, double sum){
			return _prime(value, sum);
		}
	private:
		static const string _type;
		static double _activate(double sum){
			return( sum );
		}
		static double _prime(double value, double sum){
			return 1.0;
		}
	};
	

}

#endif