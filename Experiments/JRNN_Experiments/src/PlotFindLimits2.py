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
from matplotlib.mlab import griddata
from matplotlib import cm
from mpl_toolkits.mplot3d import axes3d
from matplotlib.backends.backend_pdf import PdfPages

expfigpath = os.path.join(figpath, "Exp 1-1/Round 4/No Val 2/")
fignum = 0
usesave = True
def MakeGraph(datasetname, datasetfiles):
    global fignum
    #hidunitdata = []
    #trnumdata = []
    filedata = []
    for filename in datasetfiles: 
        filedata.append(pe.ParseFile(filename))
        #if "hid8" in filename:
            #trnumdata.append(pe.ParseFile(filename))
        #if "tr200" in filename:
            #hidunitdata.append(pe.ParseFile(filename))
            
    #hidunitdata.sort(key = lambda x : int(x.nHid))
    #trnumdata.sort(key = lambda x : int(x.nTrain), reverse=True)
    
    #print [x.nHid for x in hidunitdata]
    #print [x.nTrain for x in trnumdata]

    #hiduniterrors = []
    #hiduniterrorbars = []
    #hidunitticklabels = []
    
    errors = []
    numhids = []
    numtrs = []
    for data in filedata:
        tmperrors = np.array([v['task-0'] for v in data.errors])
        tmpavgerror, tmpavgerrbar = CalcMeanAndError(tmperrors)
        errors.append(tmpavgerror)
        numhids.append(int(data.nHid))
        numtrs.append(int(data.nTrain))
        
    nperrors = np.array(errors)
    npnumhids = np.array(numhids)
    npnumtrs = np.array(numtrs)
    
    #for data in hidunitdata:
        #tmperrors = np.array([v['task-0'] for v in data.errors])
        #tmpavgerr, tmpavgerrbar = CalcMeanAndError(tmperrors)
        #hiduniterrors.append(tmpavgerr)
        #hiduniterrorbars.append(tmpavgerrbar)
        #hidunitticklabels.append(data.nHid)
    
    #trnumerrors = []
    #trnumerrorbars = []
    #trnumticklabels = []
    
    #for data in trnumdata:
        #tmperrors = np.array([v['task-0'] for v in data.errors])
        #tmpavgerr, tmpavgerrbar = CalcMeanAndError(tmperrors)
        #trnumerrors.append(tmpavgerr)
        #trnumerrorbars.append(tmpavgerrbar)
        #trnumticklabels.append(data.nTrain)

    fignum += 1
    fig = plt.figure(fignum)
    #ax = fig.add_subplot(111, projection='3d')
    #ax = fig.gca(projection = '3d')
    #ax = Axes3D(fig)
    #xi = np.linspace(min(npnumhids), max(npnumhids),num=11)
    #yi = np.linspace(min(npnumtrs), max(npnumtrs),num=11)
    xi = np.array([1,2,4,6,8,10,12,14,16,18,20])
    yi = np.array([5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100])
    zi = griddata(npnumhids, npnumtrs, nperrors, xi, yi)
    xim, yim = np.meshgrid(xi, yi)
    plt.contourf(xim,yim,zi, cmap=cm.jet)
    plt.colorbar()
    plt.xlabel("Number Hidden Nodes")
    plt.ylabel("Number Training Points")
    plt.suptitle(datasetname + " Error vs # of Hidden Nodes & Training Points")
    #ax.contourf(xim, yim, zi, linewidth=0, antialiased=False)
    #ax.plot3D(npnumhids,npnumtrs,zs=nperrors)
    #fig.add_axes(ax)
    
    #locs = np.arange(1, len(hiduniterrors)+1)
    #width = 0.2
    #plt.bar(locs,hiduniterrors,yerr=hiduniterrorbars, color="red", width=width, \
            #antialiased=True)
    #plt.xticks(locs + width/2.0,hidunitticklabels)
    #plt.title(datasetname + " Error vs Hidden Unit #")
    #plt.ylabel("Error Rate")

    if usesave: Save(datasetname + " ErrContour")
    
    #fignum += 1
    
    #plt.figure(fignum)
    #locs2 = np.arange(1, len(trnumerrors)+1)
    #plt.bar(locs2, trnumerrors, yerr=trnumerrorbars, color="red", width=width, \
            #antialiased=True)
    #plt.xticks(locs2 + width/2.0, trnumticklabels)
    #plt.title(datasetname + " Error vs # Training Points")
    #plt.ylabel("Error Rate")
    #if usesave: Save(datasetname + " ErrVsTrainPtNum")
    
    
def Save(name):
    plt.savefig(expfigpath + name + ".pdf")
    plt.savefig(expfigpath + name + ".eps")
    plt.savefig(pp, format="pdf")

if __name__=='__main__':
    
    filepath = os.path.join(outpath, "BP Limits No Val")
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
    
    #fig = plt.figure()
    #ax = fig.add_subplot(111, projection='3d')
    #X, Y, Z = axes3d.get_test_data(0.05)
    #cset = ax.contour(X, Y, Z)
    #ax.clabel(cset, fontsize=9, inline=1)
    
    #plt.show()    
    
    for k,v in datasetfiles.items():
        MakeGraph(k,v)
    
    #MakeGraph("band-task1", datasetfiles["band-task1"])
    if not usesave: plt.show()
    if usesave: pp.close()