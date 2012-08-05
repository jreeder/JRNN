/********************************************************************
	created:	2012/08/05
	filename:	pydatasethelper.h
	author:		jreeder
	
	purpose:	Helper for the dataset class. 
*********************************************************************/

#ifndef _PYDATASETHELPER_H
#define _PYDATASETHELPER_H

#include <boost/python.hpp>
#include <utility/dataset.h>

using namespace JRNN;
using namespace boost::python;

struct DSWrap : Dataset, wrapper<Dataset>{
	void DistData (int numTrain, int numVal, int numTest){
		if (override DistData = this->get_override("DistData")){
			DistData(numTrain, numVal, numTest);
		}
		else {
			Dataset::DistData(numTrain, numVal, numTest);
		}
	}

	void default_DistData(int numTrain, int numVal, int numTest){
		return this->Dataset::DistData(numTrain, numVal, numTest);
	}

	void RedistData() {
		if (override RedistData = this->get_override("RedistData")){
			RedistData();
		}
		else {
			Dataset::RedistData();
		}
	}

	void default_RedistData() {
		return this->Dataset::RedistData();
	}
};

#endif