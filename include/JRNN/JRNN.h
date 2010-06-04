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
#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <time.h>



namespace ublas = boost::numeric::ublas;

namespace JRNN {
    class node;
    class network;
    class connection;
    class layer;
    class dataset;
    typedef boost::unordered_map<std::string, double> hashedDoubleMap;
    typedef boost::unordered_map<std::string, int> hashedIntMap;
    typedef boost::shared_ptr<node> nodePtr;
    typedef std::vector<nodePtr> nodeList;
    typedef ublas::vector<double> vecDouble;
    typedef boost::shared_ptr<network> networkPtr;
    typedef boost::shared_ptr<connection> conPtr;
    typedef boost::shared_ptr<layer> layerPtr;
    typedef std::vector<conPtr> conList;
    typedef boost::shared_ptr<layer> layerPtr;
    typedef std::map<std::string,layerPtr> layerMap;
    typedef std::vector<ublas::vector<double> > matDouble;
    typedef std::vector<double> doubles;
    typedef boost::shared_ptr<dataset> datasetPtr;
}

using boost::lexical_cast;
#include "structure/network.h"
#include "structure/node.h"
#include "structure/layer.h"
#include "structure/connection.h"
#include "utility/dataset.h"
#include "trainers/BackPropTrainer.h"

#endif	/* _JRNN_H */

