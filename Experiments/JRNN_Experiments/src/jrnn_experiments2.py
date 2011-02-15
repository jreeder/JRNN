#! /usr/bin/python

# Experiment Using JRNN_test2

__author__="jreeder"
__date__ ="$Aug 9, 2010 4:30:03 PM$"

import os
from config import *
from subprocess import *


def ProcessExperiment(argDict):
    dsname = argDict['dsname']
    numTasks = argDict['numTasks']
    numTrain = argDict['numTrain']
    numVal = argDict['numVal']
    numTest = argDict['numTest']
    numHidPerOut = argDict['numHidPerOut']
    numRuns = argDict['numRuns']
    expFold = argDict['expFold']
    impPerc = argDict['impPerc']
    useVal = argDict['useVal']

    outfilepath = os.path.join(outpath2, dsname, expFold)
    if not os.path.exists(outfilepath):
        os.makedirs(outfilepath)

    os.chdir(outfilepath)

    cmd = "%s \"%s\" %s %s %s %s %s %s %s %s %s" % (jrnn_exe2, datapath, dsname, numTasks, numTrain, numVal,\
        numTest, numHidPerOut, numRuns, impPerc, useVal)

    (stdout, stderr) = Popen(cmd, stdout = PIPE).communicate()
    if len(stdout) > 0: print stdout
    print dsname + " " + expFold + " Done"

if __name__ == "__main__":
    from experiments2 import *
    print "Starting"
    
    for argDict in experiments:
        ProcessExperiment(argDict)

    print "Finished"