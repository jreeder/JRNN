#!/usr/bin/env python
#coding:utf-8
# Author:  John Reeder
# Purpose: Plot Find Limit Experiments
# Created: 2/1/2011

import os
from config import *
from itertools import izip
import ParseExperimentFile as pe
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

expfigpath = os.path.join(figpath, "Exp 1-1")

def MakeGraph(datasetname, datasetfiles):
    hidunitdata = []
    trnumdata = []
    for filename in datasetfiles: 
        if "hid8" in filename:
            trnumdata.append(pe.ParseFile(filename))
        if "tr200" in filename:
            hidunitdata.append(pe.ParseFile(filename))
    
    hidunitdata.sort(key = lambda x : int(x.nHid))
    trnumdata.sort(key = lambda x : int(x.nTrain), reverse=True)
    
    print [x.nHid for x in hidunitdata]
    print [x.nTrain for x in trnumdata]
    
    
    

if __name__=='__main__':
    filepath = os.path.join(outpath, "BP Limits 1")
    os.chdir(filepath)
    
    filelist = os.listdir(filepath)
    
    datasetfiles = {}
    
    for f in filelist: 
        dsname = f.split("BP")[0][0:-1]
        if dsname in datasetfiles:
            datasetfiles[dsname].append(f)
        else:
            datasetfiles[dsname] = []
            datasetfiles[dsname].append(f)
            
    for k,v in datasetfiles.items():
        MakeGraph(k,v)