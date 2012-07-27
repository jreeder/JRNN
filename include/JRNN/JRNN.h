// -*- C++ -*-
/* 
 * File:   JRNN.h
 * Author: jreeder
 *
 * Created on May 20, 2010, 5:21 PM
 */

#ifndef _JRNN_H
#define	_JRNN_H

#include <vector>
#include <map>
#include <set>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/pointer_cast.hpp>
#include <iostream>
#include <limits>
#include <cmath>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <assert.h>


namespace ublas = boost::numeric::ublas;


namespace JRNN {
    
	//These two forward declarations have to be here because they have a circular dependency
	class Node;
	typedef boost::shared_ptr<Node> NodePtr;
	typedef std::vector<NodePtr> NodeList;

	class Connection;
	typedef boost::shared_ptr<Connection> ConPtr;
	typedef std::vector<ConPtr> ConList;
	typedef std::map<std::string, ConPtr> ConMap;
	typedef std::pair<std::string, ConPtr> ConPair;

	typedef boost::unordered_map<std::string, double> hashedDoubleMap;
    typedef boost::unordered_map<std::string, int> hashedIntMap;
	typedef ublas::vector<double> vecDouble;
    typedef std::vector<ublas::vector<double> > matDouble;
    typedef std::vector<double> doubles;
	typedef std::vector<std::string> strings;
	typedef std::set<std::string> StringSet;
	typedef std::pair<StringSet::iterator, bool> StringSetRet;
	typedef std::vector<int> ints;
	const double UNKNOWN = std::numeric_limits<double>::max();
	typedef boost::mt19937 rangeType;
	typedef boost::uniform_01<double> UniformDist01;
	typedef boost::uniform_int<long> UniformDistInt;
	typedef boost::variate_generator<rangeType&, UniformDist01> RandGenerator01;
	typedef boost::variate_generator<rangeType&, UniformDistInt> RandGeneratorInt;
	typedef unsigned int uint;

	class StaticRange {
	public:
		static rangeType gen;
	};
}

using boost::lexical_cast;
using boost::dynamic_pointer_cast;
using std::string;

#include "utility/utility.h"
//#include "structure/network.h"
//#include "structure/node.h"
//#include "structure/layer.h"
//#include "structure/connection.h"
//#include "utility/dataset.h"
//#include "utility/nodebuffer.h"
//#include "trainers/BackPropTrainer.h"
//#include "trainers/RPropTrainer.h"
//#include "trainers/CCTrainer.h"

#endif	/* _JRNN_H */

