#!/usr/bin/env python
# Author:  jreeder --<>
# Purpose: used to run general experiments with the JRNN nueral network library.
# Created: 5/27/2011

import os

from subprocess import *
from multiprocessing import *
from config import *

def ProcessExp(expparams):
    dsname = expparams['dsname']
    numTasks = expparams['numTasks']
    numRuns = expparams['numRuns']
    numInputs = expparams['numInputs']
    numOutputs = expparams['numOutputs'] 
    impNumTrain = expparams['impNumTrain']
    numTrain = expparams['numTrain']
    numVal = expparams['numVal']
    numTest = expparams['numTest']
    numHidPerTask = expparams['numHidPerTask']
    useValidation = expparams['useValidation']
    viewString = expparams['viewString']
    #outfile = expparams['outfile']
    primTask = expparams['primTask']
    netType = expparams['netType']
    path = datapath
    outfilepath = os.path.join(outpath3, expFold, dsname)
    
    #Need to create outfile name here based on parameters. 
    
    outfile = os.path.join(outfilepath, outfile)
    
    cmd = "%s --basepath \"%(path)\" --dsname %(dsname) --numinputs %(numInputs)" \
        + " --numoutputs %(numOutputs) --numtasks %(numTasks) --numtrain %(numTrain)s" \
        + " --numval %(numVal)s --numtest %(numTest)s --numhid %(numHidPerTask)s" \
        + " --numruns %(numRuns)s --view \"%(viewString)\" --outfile \"%(outfile)\"" \
        + " --primtask %(primTask)s --%(netType)" % locals()
    
    if useValidation:
        cmd = cmd + " -V"
        
    cmd = cmd + " --params \"%s\"" % paramspath
    
    (stdout, stderr) = Popen(cmd, stdout = PIPE).communicate()
    if len(stdout) > 0: print stdout 
    print outfile + " Done"
 
if __name__=='__main__':
    from experiments3 import *
    
    if not os.path.exists(outpath3):
        os.makedirs(outpath3)
        
    os.chdir(outpath)
    pool = Pool(4)
    pool.map(ProcessExp, experiments)
    print "Finished"
    