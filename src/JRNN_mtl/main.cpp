/********************************************************************
	created:	2011/05/24
	created:	24:5:2011   21:57
	filename:	main.cpp
	author:		jreeder
	
	purpose:	More extensible MTL test platform. Will allow varied
				experiments using command line arguments. To be called
				from python experiment scripts.
*********************************************************************/


#include "utility/mtldataset.h"
#include "trainers/RPropTrainer.h"
#include "trainers/CCTrainer.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <boost/thread.hpp>
#include <cmath>
#include <tclap/CmdLine.h>

using namespace JRNN;
using namespace std;
using namespace TCLAP;