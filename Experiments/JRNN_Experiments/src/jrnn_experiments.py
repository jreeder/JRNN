# This file expects a congfig.py file in the same folder
# defining the strings jrnn_exe datapath and outpath

__author__="jreeder"
__date__ ="$Jul 22, 2010 2:45:31 PM$"

#import string
import os
from subprocess import *
from multiprocessing import *

from config import *

def ProcessExperiment(argDict):
    dataset = argDict['dsfile']
    numTrain = argDict['numTrain']
    numVal = argDict['numVal']
    numTest = argDict['numTest']
    numIn = argDict['numIn']
    numHid = argDict['numHid']
    numOut = argDict['numOut']
    type = argDict['type']
    useVal = argDict['useVal']
    #outfile = argDict['outfile']
    numRuns = argDict['numRuns']
    expFold = argDict['expFold']

    if not os.path.exists(outpath + expFold):
        os.makedirs(outpath + expFold)

    outfile = "%s-%s-tr%s-v%s-t%s-hid%s-uv%s-r%s.txt" % (dataset[:-4], type, \
        numTrain, numVal, numTest, numHid, useVal, numRuns)

    outfile = "\"./" + expFold + "/" + outfile + "\""

    dataset = "\"%s%s\"" % (datapath, dataset)

    cmd = "%s %s %s %s %s %s %s %s %s %s %s %s" % \
        (jrnn_exe, dataset, numTrain, numVal, numTest, numIn, numHid, numOut, \
        type, useVal, outfile, numRuns)

    (stdout, stderr) = Popen(cmd, stdout = PIPE).communicate()
    if len(stdout) > 0: print stdout
    print outfile + " Done"

if __name__ == "__main__":

    from experiments import *

    if not os.path.exists(outpath):
        os.makedirs(outpath)

    os.chdir(outpath)
    pool = Pool()
    pool.map(ProcessExperiment,experiments)
    print "Finished"
