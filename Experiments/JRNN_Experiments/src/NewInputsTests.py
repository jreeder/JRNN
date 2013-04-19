#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<jreeder@gmail.com>
# Purpose: debugging file for add new inputs. 
# Created: 4/18/2013

import sys
import unittest

import PyJRNN_d as PyJRNN
import pyublas
from config import *

ucidatapath = basepath + r"/Experiments/Data/UCI Data/"

# <codecell>
import numpy
import scipy.io
import time

# <codecell>

import os
iris = scipy.io.loadmat(os.path.join(ucidatapath, "IRIS/iris.mat"))

# <codecell>

irisdata = iris["irisdata"]

# <codecell>

def matDoubleFromArray(inMat):
    newMat = PyJRNN.types.matDouble()
    for vec in inMat:
        if isinstance(vec, numpy.float64):
            newMat.append(np.array([vec]))
        else:
            newMat.append(vec)
    return newMat

# <codecell>

inputs1 = irisdata[:, [0,1]]
inputs2 = irisdata[:, [0,1,2,3]]
outs = irisdata[:, 4]

# <codecell>

inVec1 = matDoubleFromArray(inputs1)
inVec2 = matDoubleFromArray(inputs2)
outVec = matDoubleFromArray(outs)

# <codecell>

ds1 = PyJRNN.utility.Dataset()

# <codecell>

ds1.LoadFromMatDoubles(inVec1, outVec)
ds1.DistData(70, 10, 20)

# <codecell>

ds2 = PyJRNN.utility.Dataset()
ds2.LoadFromMatDoubles(inVec2, outVec)
ds2.DistData(70, 10, 20)

# <codecell>

from PyJRNN_d.types import hashedDoubleMap
from PyJRNN_d.utility import DSDatatype
from PyJRNN_d.types import ints

from PyJRNN_d.trainers import CCTrainer, ErrorType
from PyJRNN_d.networks import CCNetwork

def TestAddNewInput(ds1, ds2, numRuns, useValidation, maxEpochs, newInputIndexes, connectToHidden):
    results = {}
    results['ds1'] = []
    results['ds2'] = []
    for i in range(numRuns):
        
        ccnet = CCNetwork.Create()
        ccnet.SetScaleAndOffset(1.0, 0.5)
        numIn = ds1.numInputs
        numOut = ds1.numOutputs
        ccnet.Build(numIn, numOut, False, True, False)
        cctrainer = CCTrainer(ccnet, ds1, 8)
        cctrainer.parms.errorMeasure = ErrorType.BITS
        cctrainer.parms.out.epochs = 1000
        cctrainer.parms.SDCCRatio = 0.9
        cctrainer.parms.useSDCC = True
        
        starttime = time.clock()
        if useValidation:
            cctrainer.TrainToValConv(maxEpochs)
        else:
            cctrainer.TrainToConvergence(maxEpochs)
        tottime = time.clock() - starttime
        
        epochs = cctrainer.Epochs
        hiddenLayers = cctrainer.NumHidLayers
        numResets = cctrainer.NumResets
        
        testresult = cctrainer.TestWiClass(DSDatatype.TEST)
        mserec = cctrainer.GetMSERec()
        
        resultdict = dict(zip(("epochs", "tottime", "hiddenLayers", "numResets", "testresult", "mserec"), \
                              (epochs, tottime, hiddenLayers, numResets, testresult, mserec)))
        #resultdict = dict(((k, eval(k)) for k in ("epochs", "tottime", "hiddenLyaers", "numResets", "testresult", "mserec")))
        results["ds1"].append(resultdict)
        
        #Start the new input stuff
        cctrainer.AddNewInputs(newInputIndexes, ds2, connectToHidden)
        
        starttime = time.clock()
        if useValidation:
            cctrainer.TrainToValConv(maxEpochs)
        else:
            cctrainer.TrainToConvergence(maxEpochs)
        tottime = time.clock() - starttime
        
        epochs = cctrainer.Epochs
        hiddenLayers = cctrainer.NumHidLayers
        numResets = cctrainer.NumResets
        
        testresult = cctrainer.TestWiClass(DSDatatype.TEST)
        mserec = cctrainer.GetMSERec()
        
        resultdict = dict(zip(("epochs", "tottime", "hiddenLayers", "numResets", "testresult", "mserec"), \
                              (epochs, tottime, hiddenLayers, numResets, testresult, mserec)))
        #resultdict = dict(((k, eval(k)) for k in ("epochs", "tottime", "hiddenLyaers", "numResets", "testresult", "mserec")))
        results["ds2"].append(resultdict)
    
    return results

newIndexes = ints()
newIndexes.append(-1)
newIndexes.append(-1)

import wingdbstub
resultsIris = TestAddNewInput(ds1, ds2, 30, True, 2000, newIndexes, False)
