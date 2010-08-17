# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__="jreeder"
__date__ ="$Aug 15, 2010 2:12:34 AM$"

import os
import ParseExperimentFile2 as pe
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from config import *

expfigpath = figpath + "Exp 2/INDEX/"
usesave=True

def Save(name):
    plt.savefig(expfigpath + name + ".pdf")
    plt.savefig(expfigpath + name + ".eps")
    plt.savefig(pp, format="pdf")

def MakeGraphs(title, fignum, folder):
    print "StartFile"
    data = {}
    os.chdir(outpath2 + folder)
    filelist = os.listdir(os.curdir)
    for file in filelist:
        tmpBundle = pe.ParseFile(file)
        data[tmpBundle.name] = tmpBundle

    bpstlerr = np.array([v['task-0'] for v in data['BP_STL'].errors])
    bpstlepochs = np.array(data['BP_STL'].epochs)
    bpstltimes = np.array(data['BP_STL'].times)
    
    bpstlerravg, bpstlerrerr = CalcMeanAndError(bpstlerr)
    bpstlepochavg, bpstlepocherr = CalcMeanAndError(bpstlepochs)
    bpstltimesavg, bpstltimeserr = CalcMeanAndError(bpstltimes)
    
    bpmtlerr = np.array([v['task-0'] for v in data['BP_MTL'].errors])
    bpmtlepochs = np.array(data['BP_MTL'].epochs)
    bpmtltimes = np.array(data['BP_MTL'].times)
    
    bpmtlerravg, bpmtlerrerr = CalcMeanAndError(bpmtlerr)
    bpmtlepochavg, bpmtlepocherr = CalcMeanAndError(bpmtlepochs)
    bpmtltimesavg, bpmtltimeserr = CalcMeanAndError(bpmtltimes)

    bpmtlurerr = np.array([v['task-0'] for v in data['BP_MTL_UR'].errors])
    bpmtlurepochs = np.array(data['BP_MTL_UR'].epochs)
    bpmtlurtimes = np.array(data['BP_MTL_UR'].times)

    bpmtlurerravg, bpmtlurerrerr = CalcMeanAndError(bpmtlurerr)
    bpmtlurepochavg, bpmtlurepocherr = CalcMeanAndError(bpmtlurepochs)
    bpmtlurtimesavg, bpmtlurtimeserr = CalcMeanAndError(bpmtlurtimes)
    
    ccstlerr = np.array([v['task-0'] for v in data['CC_STL'].errors])
    ccstlepochs = np.array(data['CC_STL'].epochs)
    ccstltimes = np.array(data['CC_STL'].times)

    ccstlerravg, ccstlerrerr = CalcMeanAndError(ccstlerr)
    ccstlepochavg, ccstlepochserr = CalcMeanAndError(ccstlepochs)
    ccstltimesavg, ccstltimeserr = CalcMeanAndError(ccstltimes)

    ccmtlerr = np.array([v['task-0'] for v in data['CC_MTL'].errors])
    ccmtlepochs = np.array(data['CC_MTL'].epochs)
    ccmtltimes = np.array(data['CC_MTL'].times)

    ccmtlerravg, ccmtlerrerr = CalcMeanAndError(ccmtlerr)
    ccmtlepochsavg, ccmtlepochserr = CalcMeanAndError(ccmtlepochs)
    ccmtltimesavg, ccmtltimeserr = CalcMeanAndError(ccmtltimes)

    ccmtlurerr = np.array([v['task-0'] for v in data['CC_MTL_UR'].errors])
    ccmtlurepochs = np.array(data['CC_MTL_UR'].epochs)
    ccmtlurtimes = np.array(data['CC_MTL_UR'].times)

    ccmtlurerravg, ccmtlurerrerr = CalcMeanAndError(ccmtlurerr)
    ccmtlurepochsavg, ccmtlurepochserr = CalcMeanAndError(ccmtlurepochs)
    ccmtlurtimesavg, ccmtlurtimeserr = CalcMeanAndError(ccmtlurtimes)

    xticks = ["BP", "CC"]

    plt.figure(fignum)

    stlerravgs = [bpstlerravg, ccstlerravg]
    stlerrerr = [bpstlerrerr, ccstlerrerr]

    locs = np.arange(1, len(xticks) + 1)
    width = 0.2
    plt.bar(locs, stlerravgs, yerr=stlerrerr, color="red", width=width, \
    antialiased=True, label="STL")

    mtlerravgs = [bpmtlerravg, ccmtlerravg]
    mtlerrerr = [bpmtlerrerr, ccmtlerrerr]

    plt.bar(locs + width, mtlerravgs, yerr=mtlerrerr, color="blue", width=width,\
    antialiased=True, label="MTL")

    mtlurerravgs = [bpmtlurerravg, ccmtlurerravg]
    mtlurerrerrs = [bpmtlurerrerr, ccmtlurerrerr]

    plt.bar(locs + 2 * width, mtlurerravgs, yerr=mtlurerrerrs, color="green",\
    width=width, antialiased=True, label="MTL UR")

    plt.legend()
    plt.title(title + " Error")
    plt.xticks((locs + 3*(width/2)), xticks)
    plt.ylabel("Error Rate")

    if usesave: Save(title + " Error")

    plt.figure(fignum+1)

    stlepochsavgs = [bpstlepochavg, ccstlepochavg]
    stlepochserr = [bpstlepocherr, ccstlepochserr]

    plt.bar(locs, stlepochsavgs, yerr=stlepochserr, color="red", width=width,\
    antialiased=True, label="STL")

    mtlepochsavgs = [bpmtlepochavg, ccmtlepochsavg]
    mtlepochserr = [bpmtlepocherr, ccmtlepochserr]

    plt.bar(locs + width, mtlepochsavgs, yerr=mtlepochserr, color="blue", \
    width=width, antialiased=True, label="MTL")

    mtlurepochavgs = [bpmtlurepochavg, ccmtlurepochsavg]
    mtlurepocherrs = [bpmtlurepocherr, ccmtlurepochserr]
    
    plt.bar(locs + 2 * width, mtlurepochavgs, yerr=mtlurepocherrs, color="green",\
    width=width, antialiased=True, label="MTL UR")

    plt.legend()
    plt.title(title + " Epochs")
    plt.xticks((locs + 3*(width/2)), xticks)
    plt.ylabel("# Epochs")
    
    if usesave: Save(title + " Epochs")

    plt.figure(fignum+2)

    stltimesavgs = [bpstltimesavg, ccstltimesavg]
    stltimeserr = [bpstltimeserr, ccstltimeserr]

    plt.bar(locs, stltimesavgs, yerr=stltimeserr, color="red", width=width, \
    antialiased=True, label="STL")

    mtltimesavg = [bpmtltimesavg, ccmtltimesavg]
    mtltimeserr = [bpmtltimeserr, ccmtltimeserr]

    plt.bar(locs + width, mtltimesavg, yerr=mtltimeserr, color="blue", width=width, \
    antialiased=True, label="MTL")

    mtlurtimesavgs = [bpmtlurtimesavg, ccmtlurtimesavg]
    mtlurtimeserr = [bpmtlurtimeserr, ccmtlurtimeserr]
    
    plt.bar(locs + 2 * width, mtlurtimesavgs, yerr=mtlurtimeserr, color="green",\
    width=width, antialiased=True, label="MTL UR")

    plt.legend()
    plt.title(title + " Times")
    plt.xticks((locs + 3*(width/2)), xticks)
    plt.ylabel("Time (s)")

    if usesave: Save(title + " Times")


    

if __name__ == "__main__":
    if not os.path.exists(expfigpath):
        os.makedirs(expfigpath)

    if usesave: pp = PdfPages(expfigpath + "Collected Figures.pdf")
    MakeGraphs("Impoverished Primary Task (NS)", 1, "NormSize")
    MakeGraphs("Impoverished Primary Task (SS)", 4, "SmallSize")
    MakeGraphs("Impoverished Primary Task (LS)", 7, "LargeSize")
    MakeGraphs("Impoverished Primary Task (NS NV)", 10, "NormSizeNoVal")
    MakeGraphs("Impoverished Primary Task (SS NV)", 13, "SmallSizeNoVal")
    MakeGraphs("Impoverished Primary Task (LS NV)", 16, "LargeSizeNoVal")
    if usesave: pp.close()
    if not usesave: plt.show()
