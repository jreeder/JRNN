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
        
    extractdata("Linear CCMTL", "linear/CCMTLTest")
    extractdata("Band CCMTL", "band/CCMTLTest")
    extractdata("CirInSq CCMTL", "CirInSq/CCMTLTest")
    extractdata("Derm CCMTL", "derm/CCMTLTest")
    extractdata("Glass CCMTL", "glass/CCMTLTest")
    extractdata("CoverType CCMTL", "smallcovtype/CCMTLTest")
    extractdata("Heart CCMTL", "heart/CCMTLTest")

    extractdata("Linear GRADUR", "linear/GRADURTest")
    extractdata("Band GRADUR", "band/GRADURTest")
    extractdata("CirInSq GRADUR", "CirInSq/GRADURTest")
    extractdata("Derm GRADUR", "derm/GRADURTest")
    extractdata("Glass GRADUR", "glass/GRADURTest")
    extractdata("CoverType GRADUR", "smallcovtype/GRADURTest")
    extractdata("Heart GRADUR", "heart/GRADURTest")

    extractdata("Linear LargeSizeNV", "linear/LargeSizeNV")
    extractdata("Band LargeSizeNV", "band/LargeSizeNV")
    extractdata("CirInSq LargeSizeNV", "CirInSq/LargeSizeNV")
    extractdata("Derm LargeSizeNV", "derm/LargeSizeNV")
    extractdata("Glass LargeSizeNV", "glass/LargeSizeNV")
    extractdata("CoverType LargeSizeNV", "smallcovtype/LargeSizeNV")
    extractdata("Heart LargeSizeNV", "heart/LargeSizeNV")

    extractdata("Linear SmallSizeNV", "linear/SmallSizeNV")
    extractdata("Band SmallSizeNV", "band/SmallSizeNV")
    extractdata("CirInSq SmallSizeNV", "CirInSq/SmallSizeNV")
    extractdata("Derm SmallSizeNV", "derm/SmallSizeNV")
    extractdata("Glass SmallSizeNV", "glass/SmallSizeNV")
    extractdata("CoverType SmallSizeNV", "smallcovtype/SmallSizeNV")
    extractdata("Heart SmallSizeNV", "heart/SmallSizeNV")

    extractdata("Linear NormSizeNV", "linear/NormSizeNV")
    extractdata("Band NormSizeNV", "band/NormSizeNV")
    extractdata("CirInSq NormSizeNV", "CirInSq/NormSizeNV")
    extractdata("Derm NormSizeNV", "derm/NormSizeNV")
    extractdata("Glass NormSizeNV", "glass/NormSizeNV")
    extractdata("CoverType NormSizeNV", "smallcovtype/NormSizeNV")
    extractdata("Heart NormSizeNV", "heart/NormSizeNV")

    extractdata("Linear NormSize", "linear/NormSize")
    extractdata("Band NormSize", "band/NormSize")
    extractdata("CirInSq NormSize", "CirInSq/NormSize")
    extractdata("Derm NormSize", "derm/NormSize")
    extractdata("Glass NormSize", "glass/NormSize")
    extractdata("CoverType NormSize", "smallcovtype/NormSize")
    extractdata("Heart NormSize", "heart/NormSize")
    
    extractdata("Linear CSMTL", "linear/CSMTLTest")
    extractdata("Band CSMTL", "band/CSMTLTest")
    extractdata("CirInSq CSMTL", "CirInSq/CSMTLTest")
    extractdata("Derm CSMTL", "derm/CSMTLTest")
    extractdata("Glass CSMTL", "glass/CSMTLTest")
    extractdata("CoverType CSMTL", "smallcovtype/CSMTLTest")
    extractdata("Heart CSMTL", "heart/CSMTLTest")

    extractdata("Linear STL", "linear/stltests")
    extractdata("Band STL", "band/stltests")
    extractdata("CirInSq STL", "CirInSq/stltests")
    extractdata("Derm STL", "derm/stltests")
    extractdata("Glass STL", "glass/stltests")
    extractdata("CoverType STL", "smallcovtype/stltests") 
    extractdata("Heart STL", "heart/stltests")
    
    extractdata("Linear ETAMTL", "linear/ETAMTLTest")
    extractdata("Band ETAMTL", "band/ETAMTLTest")
    extractdata("CirInSq ETAMTL", "CirInSq/ETAMTLTest")
    extractdata("Derm ETAMTL", "derm/ETAMTLTest")
    extractdata("Glass ETAMTL", "glass/ETAMTLTest")
    extractdata("CoverType ETAMTL", "smallcovtype/ETAMTLTest")
    extractdata("Heart ETAMTL", "heart/ETAMTLTest")
    
    print "Finished" 