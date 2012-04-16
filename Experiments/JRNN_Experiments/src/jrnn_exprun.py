#!/usr/bin/env python
# Author:  jreeder --<jreeder@gmail.com>
# Purpose: used to run general experiments with the JRNN nueral network library.
# Created: 5/27/2011

import os

from subprocess import *
from multiprocessing import *
from config import *

verbose = Truetest = True
real = False
dsinpath1 = ['linear', 'CirInSq', 'band']
dsinpath2 = ['smallcovtype', 'glass', 'derm', 'heart']


def ProcessExp(expparams):
    global verbose
    global real
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
    outfile = expparams['outfile']
    primTask = expparams['primTask']
    netType = expparams['netType']
    expFold = expparams['expFold']
    useCSMTL = expparams.get('useCSMTLDS', False)
    useEtaMTL = expparams.get('useEtaMTL', False)
    useCandSlope = expparams.get('useCandSlope', False)
    useRelmin = expparams.get('RELMIN', False)
    #reverb parameters
    #useReverb = expparams.get('REVERB', False)
    numReverbs = expparams.get('numReverbs', 5)
    revRatio = expparams.get('revRatio', 1)
    bufferSize = expparams.get('bufferSize', 200)
    subView1 = expparams.get('subView1', '')
    subView2 = expparams.get('subView2', '')
    testRecall = expparams.get('testRecall', True)
    
    path = datapath if dsname in dsinpath1 else datapath2
    outfilepath = os.path.join(outpath3, expFold)
    useValStr = "T" if useValidation else "F"
    exe = jrnn_exprun
    #Need to create outfile name here based on parameters. 
    
    if outfile == "":
        outfile = "%(dsname)s-%(viewString)s-%(netType)s-tr%(numTrain)d-v%(numVal)d-" % locals()\
        + "t%(numTest)d-hid%(numHidPerTask)d-uv%(useValStr)s-r%(numRuns)d.txt" % locals()
    
    if verbose: print outfile
        
    if not os.path.exists(outfilepath):
        os.makedirs(outfilepath)
        
    os.chdir(outfilepath)
    
    #outfilepath = os.path.join(outfilepath, outfile)
    
    cmd = "%(exe)s --basepath \"%(path)s\" --dsname %(dsname)s --numinputs %(numInputs)d " % locals()\
        + "--numoutputs %(numOutputs)d --numtasks %(numTasks)d --numtrain %(numTrain)d " % locals()\
        + "--numval %(numVal)d --numtest %(numTest)d --numhid %(numHidPerTask)d " % locals()\
        + "--numruns %(numRuns)d --view \"%(viewString)s\" --outfile \"%(outfile)s\" " % locals()\
        + "--primtask %(primTask)d --%(netType)s" % locals()

    if impNumTrain > 0:
        cmd += " --impnumtrain %d" % impNumTrain
        
    if useValidation:
        cmd += " -V"
        
    if useCSMTL:
        cmd += " --CSMTLDS"
        
    if useEtaMTL:
        cmd += " --ETAMTL"
        
    if useRelmin:
        cmd += " --relmin %d" % useRelmin
    
    if useCandSlope:
        cmd += " --CandSlope" #only used if CCMTL is the nettype. 
        
    if netType == 'REVERB':
        cmd += " --numReverbs %d" % numReverbs
        cmd += " --revRatio %d" % revRatio
        cmd += " --bufferSize %d" % bufferSize
        cmd += " --subView1 %s" % subView1
        cmd += " --subView2 %s" % subView2
        if testRecall:
            cmd += " --testRecall"
    
    cmd += " --params \"%s\"" % paramspath
    
    if verbose: print cmd
    
    if real:
        (stdout, stderr) = Popen(cmd, stdout = PIPE).communicate()
        if len(stdout) > 0: print stdout 
        
    print outfile + " Done"
 
if __name__=='__main__':
    from experiments3 import *
    
    if not os.path.exists(outpath3):
        os.makedirs(outpath3)
        
    os.chdir(outpath3)
    if test:
        ProcessExp(experiments[0])
    else:
        pool = Pool(numProcesses)
        pool.map(ProcessExp, experiments)
        
    print "Finished"
    
    