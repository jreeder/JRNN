#! /usr/bin/python

#Meant to be given a folder of data files and to find the limits in terms of hidden layer nodes and training points. 

import os
from config import *
from subprocess import *
from multiprocessing import *


numTest = 200
numVal = 200
numIn = 2
numOut = 1
type = 'BP'
useVal = 'F'
numRuns = 30
expFold = 'BP Limits No Val'

expvals = {}
expvals["smallcovtype"] = {"numTest":200, "numVal":0, "numIn":10}
expvals["derm"] = {"numTest":200, "numVal":0, "numIn":33}
expvals["heart"] = {"numTest":200, "numVal":0, "numIn":13}
expvals["glass"] = {"numTest":100, "numVal":0, "numIn":9}

def ProcessFile(file):
    
    (filepath, filename) = os.path.split(file)
    (dataset,trash) = os.path.splitext(filename)
    outfilepath = os.path.join(outpath, expFold)
    os.chdir(filepath)
    datasetname = dataset.split("-")[0]
    if datasetname in expvals.keys():
        numTest = expvals[datasetname]["numTest"]
        numVal = expvals[datasetname]["numVal"]
        numIn = expvals[datasetname]["numIn"]
        
    for numHid in [1,2,4,6,8,10,12,14,16,18,20]:
        for numTrain in [5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100]:
    #for numHid in [1]:
        #for numTrain in [5]:
            outfilename = "%s-%s-tr%s-v%s-t%s-hid%s-uv%s-r%s.txt" % (dataset, type, \
            numTrain, numVal, numTest, numHid, useVal, numRuns)
        
            outfile = os.path.join(outfilepath, outfilename)
            
            cmd = "%s %s %s %s %s %s %s %s %s %s \"%s\" %s \"%s\"" % \
            (jrnn_exe, filename, numTrain, numVal, numTest, numIn, numHid, numOut, \
            type, useVal, outfile, numRuns, paramspath)
    
            (stdout, stderr) = Popen(cmd, stdout = PIPE).communicate()
            if len(stdout) > 0: print stdout
            print outfilename + " Done"

    #numTrain = 200
    
    #for numHid in [1,2,4,8,16,32]:
        
        #outfilename = "%s-%s-tr%s-v%s-t%s-hid%s-uv%s-r%s.txt" % (dataset, type, \
            #numTrain, numVal, numTest, numHid, useVal, numRuns)
        
        #outfile = os.path.join(outfilepath, outfilename)
        
        #cmd = "%s %s %s %s %s %s %s %s %s %s \"%s\" %s" % \
        #(jrnn_exe, filename, numTrain, numVal, numTest, numIn, numHid, numOut, \
        #type, useVal, outfile, numRuns)

        #(stdout, stderr) = Popen(cmd, stdout = PIPE).communicate()
        #if len(stdout) > 0: print stdout
        #print outfilename + " Done"
        
    #numHid = 8
    
    #for numTrain in [100,50,40,30,20,10,5]:
        #outfilename = "%s-%s-tr%s-v%s-t%s-hid%s-uv%s-r%s.txt" % (dataset, type, \
            #numTrain, numVal, numTest, numHid, useVal, numRuns)
        
        #outfile = os.path.join(outfilepath, outfilename)
        
        #cmd = "%s %s %s %s %s %s %s %s %s %s \"%s\" %s" % \
        #(jrnn_exe, filename, numTrain, numVal, numTest, numIn, numHid, numOut, \
        #type, useVal, outfile, numRuns)
        
        #(stdout, stderr) = Popen(cmd, stdout = PIPE).communicate()
        #if len(stdout) > 0: print stdout
        #print outfilename + " Done"
    
    print dataset + " Done"

    
if __name__ == "__main__":
    
    if not os.path.exists(outpath + expFold):
        os.makedirs(outpath + expFold)
    
    #bandpath = datapath + "Band Tasks/Single Tasks"
    #strucpath = datapath + "New Binary Tasks"
    
    #filelist =  [os.path.join(strucpath, f) for f in os.listdir(strucpath)]+[os.path.join(bandpath, f) for f in os.listdir(bandpath)]
    
    filelist = [os.path.join(datapath, f) for f in os.listdir(datapath)]
    
    #ProcessFile(filelist[0])
    
    pool = Pool(7)
    pool.map(ProcessFile,filelist)
    print "Finished"
    
    