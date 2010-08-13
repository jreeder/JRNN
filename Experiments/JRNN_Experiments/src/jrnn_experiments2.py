#! /usr/bin/python

# Experiment Using JRNN_test2

__author__="jreeder"
__date__ ="$Aug 9, 2010 4:30:03 PM$"

import os
from config import *
from subprocess import *


def ProcessExperiment(argDict):
    numTrain = argDict['numTrain']
    numVal = argDict['numVal']
    numTest = argDict['numTest']
    numHidPerOut = argDict['numHidPerOut']
    numRuns = argDict['numRuns']
    expFold = argDict['expFold']
    impPerc = argDict['impPerc']
    useVal = argDict['useVal']

    if not os.path.exists(outpath2 + expFold):
        os.makedirs(outpath2 + expFold)

    os.chdir(outpath2 + expFold)

    cmd = "%s \"%s\" %s %s %s %s %s %s %s" % (jrnn_exe2, datapath, numTrain, numVal,\
        numTest, numHidPerOut, numRuns, impPerc, useVal)

    (stdout, stderr) = Popen(cmd, stdout = PIPE).communicate()
    if len(stdout) > 0: print stdout
    print expFold + " Done"

if __name__ == "__main__":
    from experiments2 import *
    print "Starting"
    
    for argDict in experiments:
        ProcessExperiment(argDict)

    print "Finished"