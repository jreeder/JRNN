#This is a test file for PyJRNN

#import os

#os.chdir("D:\\Users\\John Reeder\\Code\\JRNN\\Builds\\VS2010\\JRNN\\Debug")
#print os.path.abspath(os.curdir)

import numpy

raw_input("Test CCNetwork")

from PyJRNN_d import networks as net
cc = net.CCNetwork.Create()
cc.Build(2,1)
import pyublas

a = numpy.array((0.5,0.5))
cc.Activate(a)

raw_input("Test Loading Data in to DS")
from PyJRNN_d import utility as utils
from PyJRNN_d.types import strings
from PyJRNN_d.types import matDouble

cds = utils.CSMTLDataset()
cds.isConceptData = False
inVec = numpy.random.random(5,)
outVec = numpy.random.random(2,)

inMatDouble = matDouble()
outMatDouble = matDouble()

for i in range(10):
        invalue = numpy.random.random(5,)
        outvalue = numpy.random.random(2,)
        inMatDouble.append(invalue)
        outMatDouble.append(outvalue)

for val in inMatDouble:
    print val

cds.AddMatDoublesToTask(inMatDouble, outMatDouble, "Task1")

cds.GetSize(utils.DSDatatype.ALL)

view = strings()
view.append("Task1")
cds.View = view

cds.DistData(2,2,2)
inputs = cds.GetInputs(utils.DSDatatype.TRAIN)

print len(inputs)

raw_input("Test Add New Input")

import PyJRNN_d as pyj

from config import *

ucidatapath = basepath + r"/Experiments/Data/UCI Data/"
import scipy.io

import os
iris = scipy.io.loadmat(os.path.join(ucidatapath, "IRIS/iris.mat"))

# <codecell>

irisdata = iris["irisdata"]

# <codecell>

def matDoubleFromArray(inMat):
    newMat = pyj.types.matDouble()
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

ds1 = pyj.utility.Dataset()

# <codecell>

ds1.LoadFromMatDoubles(inVec1, outVec)
ds1.DistData(70, 10, 20)

# <codecell>

ds2 = pyj.utility.Dataset()
ds2.LoadFromMatDoubles(inVec2, outVec)
ds2.DistData(70, 10, 20)

# <codecell>

from PyJRNN_d.types import hashedDoubleMap
from PyJRNN_d.utility import DSDatatype
from PyJRNN_d.types import ints

from PyJRNN_d.trainers import CCTrainer, ErrorType
from PyJRNN_d.networks import CCNetwork

import time
def CCWorker(trainer, results, numRuns, useValidation, maxEpochs):
    for i in range(numRuns):
        starttime = time.clock()
        if useValidation:
            trainer.TrainToValConv(maxEpochs)
        else:
            trainer.TrainToConvergence(maxEpochs)
        tottime = starttime - time.clock()
        
        epochs = trainer.Epochs
        hiddenLayers = trainer.NumHidLayers
        numResets = trainer.NumResets
        resultline = "\t".join([epochs, time, hiddenLayers, numResets])
        resultline += "\t"
        
        testresult = trainer.TestWiClass(DSDatatype.TEST)
        
        for key in testresult.iterkeys():
            resultline += "{0}:{1}\t".format(key, testresult[key])
            
        resultline += "|\t"
        
        mserec = trainer.GetMSERec()
        resultline += "\t".join(mserec)
        resultline += "\n"
        results.append(resultline)
        resultline = ""
        trainer.Reset()
        trainer.RedistData()

# <codecell>

def TestAddNewInput(ds1, ds2, results, numRuns, useValidation, maxEpochs, newInputIndexes, connectToHidden):
    for i in range(numRuns):
        
        ccnet = CCNetwork.Create()
        ccnet.SetScaleAndOffset(1.0, 0.5)
        ccnet.Build(2,1,False, True, False)
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
        tottime = starttime - time.clock()
        
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
        tottime = starttime - time.clock()
        
        epochs = cctrainer.Epochs
        hiddenLayers = cctrainer.NumHidLayers
        numResets = cctrainer.NumResets
        
        testresult = cctrainer.TestWiClass(DSDatatype.TEST)
        mserec = cctrainer.GetMSERec()
        
        resultdict = dict(zip(("epochs", "tottime", "hiddenLayers", "numResets", "testresult", "mserec"), \
                              (epochs, tottime, hiddenLayers, numResets, testresult, mserec)))
        #resultdict = dict(((k, eval(k)) for k in ("epochs", "tottime", "hiddenLyaers", "numResets", "testresult", "mserec")))
        results["ds2"].append(resultdict)
        

# <codecell>

results = {}
results["ds1"] = []
results["ds2"] = []
newIndexes = ints()
newIndexes.append(-1)
newIndexes.append(-1)
#TestAddNewInput(ds1, ds2, results, 2, True, 2000, newIndexes, False)

results2 = {}
results2["ds1"] = []
results2["ds2"] = []
TestAddNewInput(ds1, ds2, results2, 2, True, 2000, newIndexes, True)

raw_input("Press any key")