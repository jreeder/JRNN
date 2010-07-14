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
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <algorithm>


namespace ublas = boost::numeric::ublas;


namespace JRNN {
	class Node;
    class Network;
    class Connection;
    class Layer;
    class Dataset;
	class ActivationFunction;
    typedef boost::unordered_map<std::string, double> hashedDoubleMap;
    typedef boost::unordered_map<std::string, int> hashedIntMap;
    typedef boost::shared_ptr<Node> NodePtr;
	typedef boost::scoped_ptr<ActivationFunction> ActFuncPtr;
    typedef std::vector<NodePtr> NodeList;
    typedef ublas::vector<double> vecDouble;
	template<typename T = Network>
	struct NetworkPtrHelper {
		    typedef boost::shared_ptr<T> Ptr;
	};
    typedef boost::shared_ptr<Connection> ConPtr;
    typedef boost::shared_ptr<Layer> LayerPtr;
    typedef std::vector<ConPtr> ConList;
    typedef boost::shared_ptr<Layer> LayerPtr;
    typedef std::map<std::string,LayerPtr> LayerMap;
    typedef std::vector<ublas::vector<double> > matDouble;
    typedef std::vector<double> doubles;
    typedef boost::shared_ptr<Dataset> DatasetPtr;
}

using boost::lexical_cast;
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

