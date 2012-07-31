/********************************************************************
	created:	30:7:2012   11:08
	filename: 	pynetworkshelper.h
	author:		jreeder
	
	purpose:	Provide Polymorphic wrappers and helper functions for
				wrapping the networks in python
*********************************************************************/

#pragma once;

#ifndef _PYNETWORKSHELPER_H
#define _PYNETWORKSHELPER_H


#include <structure/network.h>
#include <networks/CCNetwork.h>
#include <boost/python.hpp>

using namespace JRNN;
using namespace boost::python;

struct NetWrap : Network, wrapper<Network>
{
	NetworkPtr Clone(){
		if (override Clone = this->get_override("Clone"))
			return Clone();
		return Network::Clone();
	}

	NetworkPtr default_Clone() {return this->Network::Clone();}

	void Activate(vecDouble inputs) {
		if (override Activate = this->get_override("Activate")){
			Activate(inputs);
		}
		else {
			Network::Activate(inputs);
		}
	}

	void default_Activate(vecDouble inputs) {this->Network::Activate(inputs);} 

	vecDouble GetOutputs() {
		if (override GetOutputs = this->get_override("GetOutputs"))
			return GetOutputs();
		return Network::GetOutputs();
	}

	vecDouble default_GetOutputs() {return this->Network::GetOutputs();}
};

#endif