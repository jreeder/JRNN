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

expfigpath = figpath + "Exp 2-1/INDEX 5/"
usesave=True

def Save(name):
    plt.savefig(expfigpath + name + ".pdf")
    plt.savefig(expfigpath + name + ".eps")
    plt.savefig(pp, format="pdf")

def MakeGraphs(title, fignum, folder):
    print "StartFile"
    data = {}
    os.chdir(outpath3 + folder)
    filelist = os.listdir(os.curdir)
    for file in filelist:
        tmpBundle = pe.ParseFile(file)
        data[tmpBundle.name] = tmpBundle
    
    MakeGraph1(title, fignum, data)
    MakeGraph2(title, fignum + 3, data)

    
def MakeGraph1(title, fignum, data):
    title = title + " Impoverished Primary Task"
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
    
    
def MakeGraph2(title, fignum, data):
    title = title + " All Tasks"
    plt.figure(fignum)

    xticks = ["Task 1", "Task 2", "Task 3", "Task 4"]
    locs = np.arange(1, len(xticks) + 1)
    width = 0.2
    
    #Part of Experiment 1
    bpstl1err = np.array([v['task-0'] for v in data['BP_STL_1'].errors])
    bpstl2err = np.array([v['task-0'] for v in data['BP_STL_2'].errors])
    bpstl3err = np.array([v['task-0'] for v in data['BP_STL_3'].errors])
    bpstl4err = np.array([v['task-0'] for v in data['BP_STL_4'].errors])

    bpstl1erravg, bpstl1errerr = CalcMeanAndError(bpstl1err)
    bpstl2erravg, bpstl2errerr = CalcMeanAndError(bpstl2err)
    bpstl3erravg, bpstl3errerr = CalcMeanAndError(bpstl3err)
    bpstl4erravg, bpstl4errerr = CalcMeanAndError(bpstl4err)
    
    bpmtl1err = np.array([v['task-0'] for v in data['BP_MTL_1'].errors])
    bpmtl2err = np.array([v['task-1'] for v in data['BP_MTL_2'].errors])
    bpmtl3err = np.array([v['task-2'] for v in data['BP_MTL_3'].errors])
    bpmtl4err = np.array([v['task-3'] for v in data['BP_MTL_4'].errors])
    
    bpmtl1erravg, bpmtl1errerr = CalcMeanAndError(bpmtl1err)
    bpmtl2erravg, bpmtl2errerr = CalcMeanAndError(bpmtl2err)
    bpmtl3erravg, bpmtl3errerr = CalcMeanAndError(bpmtl3err)
    bpmtl4erravg, bpmtl4errerr = CalcMeanAndError(bpmtl4err)
    
    ccstl1err = np.array([v['task-0'] for v in data['CC_STL_1'].errors])
    ccstl2err = np.array([v['task-0'] for v in data['CC_STL_2'].errors])
    ccstl3err = np.array([v['task-0'] for v in data['CC_STL_3'].errors])
    ccstl4err = np.array([v['task-0'] for v in data['CC_STL_4'].errors])

    ccstl1erravg, ccstl1errerr = CalcMeanAndError(ccstl1err)
    ccstl2erravg, ccstl2errerr = CalcMeanAndError(ccstl2err)
    ccstl3erravg, ccstl3errerr = CalcMeanAndError(ccstl3err)
    ccstl4erravg, ccstl4errerr = CalcMeanAndError(ccstl4err)
    
    ccmtl1err = np.array([v['task-0'] for v in data['CC_MTL_1'].errors])
    ccmtl2err = np.array([v['task-1'] for v in data['CC_MTL_2'].errors])
    ccmtl3err = np.array([v['task-2'] for v in data['CC_MTL_3'].errors])
    ccmtl4err = np.array([v['task-3'] for v in data['CC_MTL_4'].errors])
    
    ccmtl1erravg, ccmtl1errerr = CalcMeanAndError(ccmtl1err)
    ccmtl2erravg, ccmtl2errerr = CalcMeanAndError(ccmtl2err)
    ccmtl3erravg, ccmtl3errerr = CalcMeanAndError(ccmtl3err)
    ccmtl4erravg, ccmtl4errerr = CalcMeanAndError(ccmtl4err)
    #end of Experiment 1 Data
    
    bpfullstlerravg = [bpstl1erravg, bpstl2erravg, bpstl3erravg, bpstl4erravg]
    bpfullstlerrerr = [bpstl1errerr, bpstl2errerr, bpstl3errerr, bpstl4errerr]
    
    bpfullmtlerravg = [bpmtl1erravg, bpmtl2erravg, bpstl3erravg, bpstl4erravg]
    bpfullmtlerrerr = [bpmtl1errerr, bpmtl2errerr, bpstl3errerr, bpstl4errerr]
    
    ccfullmtlerravg = [ccmtl1erravg, ccmtl2erravg, ccmtl3erravg, ccmtl4erravg]
    ccfullmtlerrerr = [ccmtl1errerr, ccmtl2errerr, ccmtl3errerr, ccmtl4errerr]
    
    ccfullstlerravg = [ccstl1erravg, ccstl2erravg, ccstl3erravg, ccstl4erravg]
    ccfullstlerrerr = [ccstl1errerr, ccstl2errerr, ccstl3errerr, ccstl4errerr]
                       
    plt.bar(locs, bpfullstlerravg, yerr=bpfullstlerrerr, color="red", width=width, \
    antialiased=True, label="BP STL")
    
    plt.bar(locs + width, bpfullmtlerravg, yerr=bpfullmtlerrerr, color="red", width=width, \
    antialiased=True, hatch="/", label="BP MTL")
    
    plt.bar(locs + 2*width, ccfullstlerravg, yerr=ccfullstlerrerr, color="blue", width=width, \
    antialiased=True, label="CC STL")
    
    plt.bar(locs + 3*width, ccfullmtlerravg, yerr=ccfullmtlerrerr, color="blue", width=width, \
    antialiased=True, hatch="/", label="CC MTL")
    
    plt.legend()
    plt.title(title + " Error")
    plt.xticks(locs + 2*width, xticks)
    plt.ylabel("Error")
    
    if usesave: Save(title + " Error")
    
    
if __name__ == "__main__":
    if not os.path.exists(expfigpath):
        os.makedirs(expfigpath)

    if usesave: pp = PdfPages(expfigpath + "Collected Figures.pdf")
    MakeGraphs("Linear (NS)", 1, "linear/NormSize")
    MakeGraphs("Linear (SS NV)", 5, "linear/SmallSizeNV")
    MakeGraphs("Linear (LS NV)", 9, "linear/LargeSizeNV")
    MakeGraphs("Band (NS)", 13, "band/NormSize")
    MakeGraphs("Band (SS NV)", 17, "band/SmallSizeNV")
    MakeGraphs("Band (LS NV)", 21, "band/LargeSizeNV")
    MakeGraphs("CirInSq (NS)", 25, "CirInSq/NormSize")
    MakeGraphs("CirInSq (SS NV)", 29, "CirInSq/SmallSizeNV")
    MakeGraphs("CirInSq (LS NV)", 33, "CirInSq/LargeSizeNV")
    #MakeGraphs(" (NS NV)", 10, "NormSizeNoVal")
    #MakeGraphs(" (SS NV)", 13, "SmallSizeNoVal")
    #MakeGraphs(" (LS NV)", 16, "LargeSizeNoVal")
    if usesave: pp.close()
    if not usesave: plt.show()
