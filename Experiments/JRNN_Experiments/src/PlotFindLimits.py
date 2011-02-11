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

expfigpath = os.path.join(figpath, "Exp 1-1/Round 3/")
fignum = 0
usesave = True

def MakeGraph(datasetname, datasetfiles):
    global fignum
    hidunitdata = []
    trnumdata = []
    for filename in datasetfiles: 
        if "hid8" in filename:
            trnumdata.append(pe.ParseFile(filename))
        if "tr200" in filename:
            hidunitdata.append(pe.ParseFile(filename))
    
    hidunitdata.sort(key = lambda x : int(x.nHid))
    trnumdata.sort(key = lambda x : int(x.nTrain), reverse=True)
    
    #print [x.nHid for x in hidunitdata]
    #print [x.nTrain for x in trnumdata]

    hiduniterrors = []
    hiduniterrorbars = []
    hidunitticklabels = []
    
    for data in hidunitdata:
        tmperrors = np.array([v['task-0'] for v in data.errors])
        tmpavgerr, tmpavgerrbar = CalcMeanAndError(tmperrors)
        hiduniterrors.append(tmpavgerr)
        hiduniterrorbars.append(tmpavgerrbar)
        hidunitticklabels.append(data.nHid)
    
    trnumerrors = []
    trnumerrorbars = []
    trnumticklabels = []
    
    for data in trnumdata:
        tmperrors = np.array([v['task-0'] for v in data.errors])
        tmpavgerr, tmpavgerrbar = CalcMeanAndError(tmperrors)
        trnumerrors.append(tmpavgerr)
        trnumerrorbars.append(tmpavgerrbar)
        trnumticklabels.append(data.nTrain)

    fignum += 1
    plt.figure(fignum)
    locs = np.arange(1, len(hiduniterrors)+1)
    width = 0.2
    plt.bar(locs,hiduniterrors,yerr=hiduniterrorbars, color="red", width=width, \
            antialiased=True)
    plt.xticks(locs + width/2.0,hidunitticklabels)
    plt.title(datasetname + " Error vs Hidden Unit #")
    plt.ylabel("Error Rate")

    if usesave: Save(datasetname + " ErrVsHidNum")
    
    fignum += 1
    
    plt.figure(fignum)
    locs2 = np.arange(1, len(trnumerrors)+1)
    plt.bar(locs2, trnumerrors, yerr=trnumerrorbars, color="red", width=width, \
            antialiased=True)
    plt.xticks(locs2 + width/2.0, trnumticklabels)
    plt.title(datasetname + " Error vs # Training Points")
    plt.ylabel("Error Rate")
    if usesave: Save(datasetname + " ErrVsTrainPtNum")
    
    
def Save(name):
    plt.savefig(expfigpath + name + ".pdf")
    plt.savefig(expfigpath + name + ".eps")
    plt.savefig(pp, format="pdf")

if __name__=='__main__':
    
    filepath = os.path.join(outpath, "BP Limits 1")
    os.chdir(filepath)

    if not os.path.exists(expfigpath):
        os.makedirs(expfigpath)
    
    filelist = os.listdir(filepath)
    
    datasetfiles = {}
    
    for f in filelist: 
        dsname = f.split("BP")[0][0:-1]
        if dsname in datasetfiles:
            datasetfiles[dsname].append(f)
        else:
            datasetfiles[dsname] = []
            datasetfiles[dsname].append(f)
            
    
    if usesave: pp = PdfPages(expfigpath + "Collected Figures.pdf")
    
    for k,v in datasetfiles.items():
        MakeGraph(k,v)
    
    #MakeGraph("band-task1", datasetfiles["band-task1"])
    if not usesave: plt.show()
    if usesave: pp.close()