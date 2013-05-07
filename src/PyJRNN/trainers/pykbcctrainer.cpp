/********************************************************************
	created:	2013/05/07
	created:	7:5:2013   1:48
	filename: 	pykbcctrainer.cpp
	author:		jreeder
	
	purpose:	python wrapper for kbcctrainer		
*********************************************************************/

#include <boost/python.hpp>
#include <trainers/CCTrainer.h>
#include <trainers/KBCCTrainer.h>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace JRNN;
using namespace boost::python;

void exportKBCCTrainer() {

	typedef boost::shared_ptr<KBCCTrainer> KBCCTrainerPtr;

	class_<NetPtrList>("NetPtrList").
		def(vector_indexing_suite<NetPtrList>());

	class_<KBCCTrainer, KBCCTrainerPtr, bases<CCTrainer> >("KBCCTrainer", init<CCNetworkPtr, DatasetPtr, int, optional<NetPtrList, int, ints> >()).
		def("TestOnData", &KBCCTrainer::TestOnData).
		def("TestWiClass", &KBCCTrainer::TestWiClass).
		def("Reset", &KBCCTrainer::Reset).
		def("AddSubNet", &KBCCTrainer::AddSubNet).
		def("ClearSubNetList", &KBCCTrainer::ClearSubNetList).
		add_property("numCopies", &KBCCTrainer::GetNumCopies, &KBCCTrainer::SetNumCopies)
		;
}