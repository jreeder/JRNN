#This is a test file for PyJRNN

#import os

#os.chdir("D:\\Users\\John Reeder\\Code\\JRNN\\Builds\\VS2010\\JRNN\\Debug")
#print os.path.abspath(os.curdir)

#import numpy

#raw_input("Test CCNetwork")

#from PyJRNN_d import networks as net
#cc = net.CCNetwork.Create()
#cc.Build(2,1)
#import pyublas

#a = numpy.array((0.5,0.5))
#cc.Activate(a)

#raw_input("Test Loading Data in to DS")
#from PyJRNN_d import utility as utils
#from PyJRNN_d.types import strings
#from PyJRNN_d.types import matDouble

#cds = utils.CSMTLDataset()
#cds.isConceptData = False
#inVec = numpy.random.random(5,)
#outVec = numpy.random.random(2,)

#inMatDouble = matDouble()
#outMatDouble = matDouble()

#for i in range(10):
#        invalue = numpy.random.random(5,)
#        outvalue = numpy.random.random(2,)
#        inMatDouble.append(invalue)
#        outMatDouble.append(outvalue)

#for val in inMatDouble:
#    print val

#cds.AddMatDoublesToTask(inMatDouble, outMatDouble, "Task1")

#cds.GetSize(utils.DSDatatype.ALL)

#view = strings()
#view.append("Task1")
#cds.View = view

#cds.DistData(2,2,2)
#inputs = cds.GetInputs(utils.DSDatatype.TRAIN)

#print len(inputs)

#raw_input("Test Add New Input")

#import PyJRNN_d as pyj

#from config import *

#ucidatapath = basepath + r"/Experiments/Data/UCI Data/"
#import scipy.io

#import os
#iris = scipy.io.loadmat(os.path.join(ucidatapath, "IRIS/iris.mat"))

## <codecell>

#irisdata = iris["irisdata"]

## <codecell>

#def matDoubleFromArray(inMat):
#    newMat = pyj.types.matDouble()
#    for vec in inMat:
#        if isinstance(vec, numpy.float64):
#            newMat.append(np.array([vec]))
#        else:
#            newMat.append(vec)
#    return newMat

## <codecell>

#inputs1 = irisdata[:, [0,1]]
#inputs2 = irisdata[:, [0,1,2,3]]
#outs = irisdata[:, 4]

## <codecell>

#inVec1 = matDoubleFromArray(inputs1)
#inVec2 = matDoubleFromArray(inputs2)
#outVec = matDoubleFromArray(outs)

## <codecell>

#ds1 = pyj.utility.Dataset()

## <codecell>

#ds1.LoadFromMatDoubles(inVec1, outVec)
#ds1.DistData(70, 10, 20)

## <codecell>

#ds2 = pyj.utility.Dataset()
#ds2.LoadFromMatDoubles(inVec2, outVec)
#ds2.DistData(70, 10, 20)

## <codecell>

#from PyJRNN_d.types import hashedDoubleMap
#from PyJRNN_d.utility import DSDatatype
#from PyJRNN_d.types import ints

#from PyJRNN_d.trainers import CCTrainer, ErrorType
#from PyJRNN_d.networks import CCNetwork

#import time
#def CCWorker(trainer, results, numRuns, useValidation, maxEpochs):
#    for i in range(numRuns):
#        starttime = time.clock()
#        if useValidation:
#            trainer.TrainToValConv(maxEpochs)
#        else:
#            trainer.TrainToConvergence(maxEpochs)
#        tottime = starttime - time.clock()
        
#        epochs = trainer.Epochs
#        hiddenLayers = trainer.NumHidLayers
#        numResets = trainer.NumResets
#        resultline = "\t".join([epochs, time, hiddenLayers, numResets])
#        resultline += "\t"
        
#        testresult = trainer.TestWiClass(DSDatatype.TEST)
        
#        for key in testresult.iterkeys():
#            resultline += "{0}:{1}\t".format(key, testresult[key])
            
#        resultline += "|\t"
        
#        mserec = trainer.GetMSERec()
#        resultline += "\t".join(mserec)
#        resultline += "\n"
#        results.append(resultline)
#        resultline = ""
#        trainer.Reset()
#        trainer.RedistData()

## <codecell>

#def TestAddNewInput(ds1, ds2, results, numRuns, useValidation, maxEpochs, newInputIndexes, connectToHidden):
#    for i in range(numRuns):
        
#        ccnet = CCNetwork.Create()
#        ccnet.SetScaleAndOffset(1.0, 0.5)
#        ccnet.Build(2,1,False, True, False)
#        cctrainer = CCTrainer(ccnet, ds1, 8)
#        cctrainer.parms.errorMeasure = ErrorType.BITS
#        cctrainer.parms.out.epochs = 1000
#        cctrainer.parms.SDCCRatio = 0.9
#        cctrainer.parms.useSDCC = True
        
#        starttime = time.clock()
#        if useValidation:
#            cctrainer.TrainToValConv(maxEpochs)
#        else:
#            cctrainer.TrainToConvergence(maxEpochs)
#        tottime = starttime - time.clock()
        
#        epochs = cctrainer.Epochs
#        hiddenLayers = cctrainer.NumHidLayers
#        numResets = cctrainer.NumResets
        
#        testresult = cctrainer.TestWiClass(DSDatatype.TEST)
#        mserec = cctrainer.GetMSERec()
        
#        resultdict = dict(zip(("epochs", "tottime", "hiddenLayers", "numResets", "testresult", "mserec"), \
#                              (epochs, tottime, hiddenLayers, numResets, testresult, mserec)))
#        #resultdict = dict(((k, eval(k)) for k in ("epochs", "tottime", "hiddenLyaers", "numResets", "testresult", "mserec")))
#        results["ds1"].append(resultdict)
        
#        #Start the new input stuff
#        cctrainer.AddNewInputs(newInputIndexes, ds2, connectToHidden)
        
#        starttime = time.clock()
#        if useValidation:
#            cctrainer.TrainToValConv(maxEpochs)
#        else:
#            cctrainer.TrainToConvergence(maxEpochs)
#        tottime = starttime - time.clock()
        
#        epochs = cctrainer.Epochs
#        hiddenLayers = cctrainer.NumHidLayers
#        numResets = cctrainer.NumResets
        
#        testresult = cctrainer.TestWiClass(DSDatatype.TEST)
#        mserec = cctrainer.GetMSERec()
        
#        resultdict = dict(zip(("epochs", "tottime", "hiddenLayers", "numResets", "testresult", "mserec"), \
#                              (epochs, tottime, hiddenLayers, numResets, testresult, mserec)))
#        #resultdict = dict(((k, eval(k)) for k in ("epochs", "tottime", "hiddenLyaers", "numResets", "testresult", "mserec")))
#        results["ds2"].append(resultdict)
        

## <codecell>

#results = {}
#results["ds1"] = []
#results["ds2"] = []
#newIndexes = ints()
#newIndexes.append(-1)
#newIndexes.append(-1)
##TestAddNewInput(ds1, ds2, results, 2, True, 2000, newIndexes, False)

#results2 = {}
#results2["ds1"] = []
#results2["ds2"] = []
#TestAddNewInput(ds1, ds2, results2, 2, True, 2000, newIndexes, True)

#raw_input("Press any key")

userspath = "D:\Users\John Reeder\Code\opennero\Build\dist\Debug\jrnnexp1\users"
import os
import re
import json
import PyJRNN_d as pyj
import pyublas
import numpy as np
from PyJRNN_d.utility import DSDatatype


def loadScenarioFiles(scenarioPath):
    scenariofiles = {}
    trainingdir = os.path.join(scenarioPath, "training")
    historydir = os.path.join(scenarioPath, "history")
    scenariofiles["training"] = [os.path.join(trainingdir, x) for x in os.listdir(trainingdir)]
    scenariofiles["history"] = [os.path.join(historydir, x) for x in os.listdir(historydir)]
    return scenariofiles

def LoadUserData(username):
    global userspath
    Scenarios = {}
    userP = os.path.join(userspath, username)
    userDataP = os.path.join(userP, "data")
    scenarioList = os.listdir(userDataP)
    for scene in scenarioList:
        Scenarios[scene] = loadScenarioFiles(os.path.join(userDataP, scene))
    
    return Scenarios

#jamesscenarios = LoadUserData('Cong')

#jamestraining = jamesscenarios['Chase - 16Nov2012171621']['training']


prog = re.compile('eps_(\d+)')

def LoadTrainingData(listOfTrainingFiles):
    dataArray = []
    for fileName in listOfTrainingFiles:
        with open(fileName, 'r') as fileh:
            dataArray.append(json.load(fileh))
    
    return dataArray


#jamesdata = LoadTrainingData(jamestraining)

def matDoubleFromArray(inMat):
    newMat = pyj.types.matDouble()
    for vec in inMat:
        if isinstance(vec, np.float64):
            newMat.append(np.array([vec]))
        else:
            newMat.append(vec)
    return newMat

def CreateUserDatasets(userData, numFrames):
    train1 = pyj.utility.Dataset()
    train2 = pyj.utility.Dataset()
    train3 = pyj.utility.Dataset()
    test1 = pyj.utility.Dataset()
    
    minSize = min([len(x) for x in userData])
    strideSize = minSize / numFrames

    def Norm01Array(inArray):
        minval = np.min(inArray, axis=0)
        maxval = np.max(inArray, axis=0)
        inArray -= minval
        inArray /= (maxval - minval + 0.000001)
        return inArray

    def loadDataset(ds, userDataList):
        inputs = np.array([item['sensors'] for item in userDataList[-minSize::strideSize]])
        outputs = np.array([item['actions'] for item in userDataList[-minSize::strideSize]])
        ds.LoadFromMatDoubles(matDoubleFromArray(Norm01Array(inputs)), matDoubleFromArray(Norm01Array(outputs)), True)
    
    loadDataset(train1, userData[1])
    loadDataset(train2, userData[2])
    loadDataset(train3, userData[3])
    loadDataset(test1, userData[4])
    train1.DistData(int(numFrames * 0.8), int(numFrames * 0.2), 0)
    train2.DistData(int(numFrames * 0.8), int(numFrames * 0.2), 0)
    train3.DistData(int(numFrames * 0.8), int(numFrames * 0.2), 0)
    test1.DistData(0,0,numFrames)
    
    return (train1, train2, train3, test1)

#(jtrain1, jtrain2, jtrain3, jtest1) = CreateUserDatasets(jamesdata, 100)

#revCC = pyj.trainers.RevCCTrainer(21, 2, 8)

#revCC.parms.useSDCC = True
#revCC.parms.SDCCRatio = 0.9
#revCC.revparams.numRevTrainRatio = 0.5

def ConsolidatedTrainingTest(dstupple, numRuns, rCC, maxEpochs):
    print "Loading Data"
    tr1 = dstupple[0]
    tr2 = dstupple[1]
    tr3 = dstupple[2]
    te1 = dstupple[3]
    print "Finished Loading"
    results = []
    for i in range(numRuns):
        print "starting run {0}".format(i)
        resDict = {}
        rCC.TrainTask(tr1, maxEpochs, True)
        resDict['tr1.epochs'] = rCC.net1vals.epochs
        resDict['tr1.hiddenLayers'] = rCC.net1vals.numHidLayers
        resDict['tr1.testError'] = rCC.TestOnData(te1, DSDatatype.TEST)
        print "Finished Train and Test 1"
        
        rCC.TrainTask(tr2, maxEpochs, True)
        resDict['tr2.epochs'] = rCC.net1vals.epochs
        resDict['tr2.hiddenLayers'] = rCC.net1vals.numHidLayers
        resDict['tr2.testError'] = rCC.TestOnData(te1, DSDatatype.TEST)
        print "Finished Train and Test 2"
        
        rCC.TrainTask(tr3, maxEpochs, True)
        resDict['tr3.epochs'] = rCC.net1vals.epochs
        resDict['tr3.hiddenLayers'] = rCC.net1vals.numHidLayers
        resDict['tr3.testError'] = rCC.TestOnData(te1, DSDatatype.TEST)
        print "Finished Train and Test 3"
        
        print resDict
        
        print "Resetting"
        results.append(resDict)
        rCC.Reset()
        tr1.RedistData()
        tr2.RedistData()
        tr3.RedistData()
        te1.RedistData()
    
    return results


#jamesds = (jtrain1, jtrain2, jtrain3, jtest1)
#import time
#starttime = time.clock()
#jamesresults = ConsolidatedTrainingTest(jamesds, 5, revCC, 2000)
#elapsedtime = time.clock() - starttime
#print elapsedtime

#revCC.TrainTask(jtrain1, 3000, True)

#result = revCC.TestOnData(jtest1, DSDatatype.TEST)
#print result

#revCC.TrainTask(jtrain2, 3000, True)

#result2 = revCC.TestOnData(jtest1, DSDatatype.TEST)
#print result2