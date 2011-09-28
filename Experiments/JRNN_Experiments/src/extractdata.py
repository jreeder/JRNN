#!/usr/bin/env python
#coding:utf-8
# Author:   jreeder--<jreeder@gmail.com>
# Purpose: This script will parse the experimental data and put it into a format that excel
#          or some other data processing application can read and create drafts from. 
# Created: 9/27/2011

import sys
import os
import ParseExperimentFile2 as pe
from config import *

#headers = ["bpstlerravg", "bpstlerrerr", "bpstlepochavg", "bpstlepocherr", "bpstltimesavg", \
           #"bpstltimeserr", "bpmtlerravg", "bpmtlerrerr", "bpmtlepochavg", "bpmtlepocherr",\
           #"bpmtltimesavg", "bpmtltimeserr", "bpmtlurerravg", "bpmtlurerrerr", \
           #"bpmtlurepochavg", "bpmtlurepocherr", "bpmtlurtimesavg", "bpmtlurtimeserr",\
           #"ccstlerravg", "ccstlerrerr", "ccstlepochavg", "ccstlepochserr", \
           #"ccstltimesavg", "ccstltimeserr", "ccmtlerravg", "ccmtlerrerr", \
           #"ccmtlepochsavg", "ccmtlepochserr", "ccmtltimesavg", "ccmtltimeserr", \
           #"ccmtlurerravg", "ccmtlurerrerr", "ccmtlurepochsavg", "ccmtlurepochserr", \
           #"ccmtlurtimesavg", "ccmtlurtimeserr"]

headerserr = ["bpstlerravg", "bpstlerrerr", "bpmtlerravg", "bpmtlerrerr",\
              "bpmtlurerravg", "bpmtlurerrerr","ccstlerravg", "ccstlerrerr",\
              "ccmtlerravg", "ccmtlerrerr","ccmtlurerravg", "ccmtlurerrerr"]

headersepoch = ["bpstlepochavg", "bpstlepocherr","bpmtlepochavg", "bpmtlepocherr",\
                "bpmtlurepochavg", "bpmtlurepocherr","ccstlepochavg", "ccstlepochserr",\
                "ccmtlepochsavg", "ccmtlepochserr","ccmtlurepochsavg", "ccmtlurepochserr"]

headerstimes = ["bpstltimesavg","bpstltimeserr","bpmtltimesavg", "bpmtltimeserr",\
                "bpmtlurtimesavg", "bpmtlurtimeserr","ccstltimesavg", "ccstltimeserr",\
                "ccmtltimesavg", "ccmtltimeserr", "ccmtlurtimesavg", "ccmtlurtimeserr"]

Test = False

def extractdata(title, folder):
    global headers
    print "Start " + title
    data = {}
    os.chdir(os.path.join(outpath3,folder))
    filelist = os.listdir(os.curdir)
    for filen in filelist:
        tmpBundle = pe.ParseFile(filen)
        data[tmpBundle.name] = tmpBundle
    
    #Pull All the data out and calculate the means and averages. 
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
        
    #row = [bpstlerravg, bpstlerrerr, bpstlepochavg, bpstlepocherr, bpstltimesavg, \
           #bpstltimeserr, bpmtlerravg, bpmtlerrerr, bpmtlepochavg, bpmtlepocherr,\
           #bpmtltimesavg, bpmtltimeserr, bpmtlurerravg, bpmtlurerrerr, \
           #bpmtlurepochavg, bpmtlurepocherr, bpmtlurtimesavg, bpmtlurtimeserr,\
           #ccstlerravg, ccstlerrerr, ccstlepochavg, ccstlepochserr, \
           #ccstltimesavg, ccstltimeserr, ccmtlerravg, ccmtlerrerr, \
           #ccmtlepochsavg, ccmtlepochserr, ccmtltimesavg, ccmtltimeserr, \
           #ccmtlurerravg, ccmtlurerrerr, ccmtlurepochsavg, ccmtlurepochserr, \
           #ccmtlurtimesavg, ccmtlurtimeserr]
    
    rowerr = [bpstlerravg, bpstlerrerr, bpmtlerravg, bpmtlerrerr,\
              bpmtlurerravg, bpmtlurerrerr,ccstlerravg, ccstlerrerr,\
              ccmtlerravg, ccmtlerrerr,ccmtlurerravg, ccmtlurerrerr]

    rowepoch = [bpstlepochavg, bpstlepocherr,bpmtlepochavg, bpmtlepocherr,\
                    bpmtlurepochavg, bpmtlurepocherr,ccstlepochavg, ccstlepochserr,\
                    ccmtlepochsavg, ccmtlepochserr,ccmtlurepochsavg, ccmtlurepochserr]
    
    rowtimes = [bpstltimesavg,bpstltimeserr,bpmtltimesavg, bpmtltimeserr,\
                    bpmtlurtimesavg, bpmtlurtimeserr,ccstltimesavg, ccstltimeserr,\
                    ccmtltimesavg, ccmtltimeserr, ccmtlurtimesavg, ccmtlurtimeserr]

    
    if not Test:
        outfilename = title + " Error.csv"
        outfilepath = os.path.join(csvpath, outfilename)
        with open(outfilepath, 'w') as outfile:
            print >>outfile, ",".join(headerserr)
            print >>outfile, ",".join([str(v) for v in rowerr])
        
        outfilename = title + " Epoch.csv"
        outfilepath = os.path.join(csvpath, outfilename)
        with open(outfilepath, 'w') as outfile:
            print >>outfile, ",".join(headersepoch)
            print >>outfile, ",".join([str(v) for v in rowepoch])
        
        outfilename = title + " Time.csv"
        outfilepath = os.path.join(csvpath, outfilename)
        with open(outfilepath, 'w') as outfile:
            print >>outfile, ",".join(headerstimes)
            print >>outfile, ",".join([str(v) for v in rowtimes])
    
    print "done"



if __name__=='__main__':
    if not os.path.exists(csvpath):
        os.makedirs(csvpath)
        
    extractdata("Linear (NS)", "linear/NormSize")
    extractdata("Linear (SS NV)", "linear/SmallSizeNV")
    extractdata("Linear (LS NV)", "linear/LargeSizeNV")
    extractdata("Band (NS)", "band/NormSize")
    extractdata("Band (SS NV)", "band/SmallSizeNV")
    extractdata("Band (LS NV)", "band/LargeSizeNV")
    extractdata("CirInSq (NS)", "CirInSq/NormSize")
    extractdata("CirInSq (SS NV)", "CirInSq/SmallSizeNV")
    extractdata("CirInSq (LS NV)", "CirInSq/LargeSizeNV")
    extractdata("Glass (NS)", "glass/NormSize")
    extractdata("Glass (SS NV)", "glass/SmallSizeNV")
    extractdata("Glass (LS NV)", "glass/LargeSizeNV")
    extractdata("Dermatology (NS)", "derm/NormSize")
    extractdata("Dermatology (SS NV)", "derm/SmallSizeNV")
    extractdata("Dermatology (LS NV)", "derm/LargeSizeNV")
    extractdata("Cover Type (NS)", "smallcovtype/NormSize")
    extractdata("Cover Type (SS NV)", "smallcovtype/SmallSizeNV")
    extractdata("Cover Type (LS NV)", "smallcovtype/LargeSizeNV")
    
    print "Finished" 