#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<jreeder@gmail.com>
# Purpose: calculate the user agent results
# Created: 6/13/2013

import sys
import os
from config import *
from localvars import *
from subprocess import *
from multiprocessing import *
import numpy
import json

def CalcResult(user, netfilename):
    resultPath = os.path.join(luserspath, user, 'results')
    filename = [x for x in os.listdir(resultPath) if 'SimResult' in x and netfilename in x][0]
    results = None
    with open(os.path.join(resultPath, filename), 'r') as fileh:
        results = json.load(fileh)
        
    resultMat = numpy.array(results)
    return numpy.mean(resultMat, axis=1)
    
test = True

if __name__=='__main__':
    if len(sys.argv) != 2:
        user = 'jcginn'
    else:    
        user = sys.argv[1]
        
    userP = os.path.join(luserspath, user, 'agents')
    
    networkfiles = os.listdir(userP)
    
    if test:
        CalcResult(user, networkfiles[0])    