#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<jreeder@gmail.com>
# Purpose: More general extract data script to deal with general files instead of 
# specially named files. 
# Created: 10/10/2011

import sys
import os
import ParseExperimentFile3 as pe
from config import *

edpath = outpath3

#headers = ["bpstlerravg", "bpstlerrerr", "bpstlepochavg", "bpstlepocherr", "bpstltimesavg", \
           #"bpstltimeserr", "bpmtlerravg", "bpmtlerrerr", "bpmtlepochavg", "bpmtlepocherr",\
           #"bpmtltimesavg", "bpmtltimeserr", "bpmtlurerravg", "bpmtlurerrerr", \
           #"bpmtlurepochavg", "bpmtlurepocherr", "bpmtlurtimesavg", "bpmtlurtimeserr",\
           #"ccstlerravg", "ccstlerrerr", "ccstlepochavg", "ccstlepochserr", \
           #"ccstltimesavg", "ccstltimeserr", "ccmtlerravg", "ccmtlerrerr", \
           #"ccmtlepochsavg", "ccmtlepochserr", "ccmtltimesavg", "ccmtltimeserr", \
           #"ccmtlurerravg", "ccmtlurerrerr", "ccmtlurepochsavg", "ccmtlurepochserr", \
           #"ccmtlurtimesavg", "ccmtlurtimeserr"]

#headerserr = ["bpstlerravg", "bpstlerrerr", "bpmtlerravg", "bpmtlerrerr",\
              #"bpmtlurerravg", "bpmtlurerrerr","ccstlerravg", "ccstlerrerr",\
              #"ccmtlerravg", "ccmtlerrerr","ccmtlurerravg", "ccmtlurerrerr"]

#headersepoch = ["bpstlepochavg", "bpstlepocherr","bpmtlepochavg", "bpmtlepocherr",\
                #"bpmtlurepochavg", "bpmtlurepocherr","ccstlepochavg", "ccstlepochserr",\
                #"ccmtlepochsavg", "ccmtlepochserr","ccmtlurepochsavg", "ccmtlurepochserr"]

#headerstimes = ["bpstltimesavg","bpstltimeserr","bpmtltimesavg", "bpmtltimeserr",\
                #"bpmtlurtimesavg", "bpmtlurtimeserr","ccstltimesavg", "ccstltimeserr",\
                #"ccmtltimesavg", "ccmtltimeserr", "ccmtlurtimesavg", "ccmtlurtimeserr"]

Test = False

def extractdata(title, folder):
    #global headers
    global edpath
    print "Start " + title
    data = {}
    os.chdir(os.path.join(edpath,folder))
    filelist = os.listdir(os.curdir)
    for filen in filelist:
        tmpBundle = pe.ParseFile(filen)
        data[tmpBundle.name] = tmpBundle
    
    #headerserr = ["Error Avg", "Error Std Error"]
    #headersepoch = ["Epoch Avg", "Epoch Std Error"]
    #headerstimes = ["Time Avg", "Time Std Error"]
    headerserr = []
    headersepoch = []
    headerstimes = []
    rowerr = []
    rowepoch = []
    rowtimes = []
    
    for key in data.iterkeys():
        #Pull All the data out and calculate the means and averages. 
        err = np.array([v['task-0'] for v in data[key].errors])
        epochs = np.array(data[key].epochs)
        times = np.array(data[key].times)
        
        erravg, errerr = CalcMeanAndError(err)
        epochavg, epocherr = CalcMeanAndError(epochs)
        timesavg, timeserr = CalcMeanAndError(times)
        
        headerserr = headerserr + [key + " Error Avg", key + " Std Error"]
        headersepoch = headersepoch + [key + " Epoch Avg", key + " Epoch Std Error"]
        headerstimes = headerstimes + [key + " Time Avg", key + " Time Std Error"]
    
        rowerr = rowerr + [erravg, errerr]
        rowepoch = rowepoch + [epochavg, epocherr]
        rowtimes = rowtimes + [timesavg, timeserr]
    
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
        
    #extractdata("Linear CCMTL", "linear/CCMTLTest")
    extractdata("Band CSMTL", "band/CSMTLTest")
    #extractdata("CirInSq CCMTL", "CirInSq/CCMTLTest")
    
    print "Finished" 