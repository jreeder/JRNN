#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<jreeder@gmail.com>
# Purpose: Quick Plot of the reverberation stuff. 
# Created: 4/10/2012

import sys
import unittest

import os
import ParseReverbFiles as pe
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from config import *



def MakeGraph(title, filepath):
    title = title + "First Task Performance"

    if isinstance(filepath, str):
        data = pe.ParseFile(filepath)
    else:
        data = pe.ParseArray(filepath)
    
    ftperf = data.ftwtrain
    minlen = float("inf")
    for d in ftperf:
        nlen = len(d)
        if nlen < minlen: minlen = nlen
    tmpArr = [[] for i in range(minlen)]
    
    for d in ftperf:
        sorteddict = [x for x in d.iteritems()]
        sorteddict.sort(key=lambda x: x[0])
        count = 0
        for x in sorteddict:
            if count == minlen: break
            tmpArr[count].append(float(x[1]))
            count += 1
        
    tmpArr2 = [float(sum(x))/len(x) for x in tmpArr]
    
    pArray = np.array(tmpArr2)
    #plot(pArray)
    return pArray
