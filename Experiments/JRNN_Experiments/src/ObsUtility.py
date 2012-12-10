# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

import PyJRNN
import pyublas
import numpy
from PyJRNN.utility import DSDatatype
import os
import re
import json

# <codecell>
#userspath = "D:\Users\John Reeder\Code\opennero\Build\dist\Debug\jrnnexp1\users" # For Titan
userspath = "C:\Users\John\Documents\Source\opennero\Build\dist\Debug\jrnnexp1\Users" # For Lab Comp
# <codecell>



# <codecell>

def loadScenarioFiles(scenarioPath):
    scenariofiles = {}
    trainingdir = os.path.join(scenarioPath, "training")
    historydir = os.path.join(scenarioPath, "history")
    scenariofiles["training"] = [os.path.join(trainingdir, x) for x in os.listdir(trainingdir)]
    scenariofiles["history"] = [os.path.join(historydir, x) for x in os.listdir(historydir)]
    return scenariofiles

# <codecell>

def LoadUserData(username):
    global userspath
    Scenarios = {}
    userP = os.path.join(userspath, username)
    userDataP = os.path.join(userP, "data")
    scenarioList = os.listdir(userDataP)
    for scene in scenarioList:
        Scenarios[scene] = loadScenarioFiles(os.path.join(userDataP, scene))
    
    return Scenarios

# <codecell>

#jamesscenarios = LoadUserData('jcginn')

# <codecell>

# yinjiescenarios = LoadUserData('Yinjie')
# congscenarios = LoadUserData('Cong')

# <codecell>

# jamestraining = jamesscenarios['Chase - 13Nov2012223245']['training']

# <codecell>


sortField = re.compile('eps_(\d+)')
# print prog.search(jamestraining[0]).group(1)

# <codecell>

# jamestraining.sort(key=lambda x: int(prog.search(x).group(1)))

# <codecell>



def LoadTrainingData(listOfTrainingFiles):
    dataArray = []
    for fileName in listOfTrainingFiles:
        with open(fileName, 'r') as fileh:
            dataArray.append(json.load(fileh))
    
    return dataArray

# <codecell>

# jamesdata = LoadTrainingData(jamestraining)

# <codecell>

# len(jamesdata)

# <codecell>

# lengths = [len(x) for x in jamesdata]

# <codecell>

# sum(lengths[1:]) / float(len(lengths[1:]))

# <codecell>

# min(lengths)

# <codecell>


# <codecell>

# tmpList = jamesdata[1][-14896::16]

# <codecell>

# len(tmpList)

# <codecell>

# 14896 / (30*30)

# <codecell>

def matDoubleFromArray(inMat):
    newMat = PyJRNN.types.matDouble()
    for vec in inMat:
        if isinstance(vec, numpy.float64):
            newMat.append(numpy.array([vec]))
        else:
            newMat.append(vec)
    return newMat

# <codecell>

def CreateUserDatasets(userData, numFrames):
    train1 = PyJRNN.utility.Dataset()
    train2 = PyJRNN.utility.Dataset()
    train3 = PyJRNN.utility.Dataset()
    test1 = PyJRNN.utility.Dataset()
    
    minSize = min([len(x) for x in userData])
    strideSize = minSize / numFrames

    def Norm01Array(inArray):
        minval = numpy.min(inArray, axis=0)
        maxval = numpy.max(inArray, axis=0)
        inArray -= minval
        inArray /= (maxval - minval + 0.000001)
        return inArray

    def loadDataset(ds, userDataList):
        inputs = numpy.array([item['sensors'] for item in userDataList[-minSize::strideSize]])
        outputs = numpy.array([item['actions'] for item in userDataList[-minSize::strideSize]])
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
# <codecell>

# (jtrain1, jtrain2, jtrain3, jtest1) = CreateUserDatasets(jamesdata, 900)

# <codecell>

# print type(jtrain1)

# <codecell>

# print jtrain1.numInputs, jtrain1.numOutputs

# <codecell>

# revCC = PyJRNN.trainers.RevCCTrainer(21, 2, 8)

# <codecell>

# revCC.TrainTask(jtrain1, 3000, True)

# <codecell>

# result = revCC.TestOnData(jtest1, DSDatatype.TEST)
# print result

# <codecell>

# revCC.TrainTask(jtrain2, 3000, True)

# <codecell>

# result2 = revCC.TestOnData(jtest1, DSDatatype.TEST)
# print result2

# <codecell>

# revCC.TrainTask(jtrain3, 3000, True)

# <codecell>

# result3 = revCC.TestOnData(jtest1, DSDatatype.TEST)
# print result3

# <codecell>

def ConsolidatedTrainingTest(dstupple, numRuns, rCC, maxEpochs, reshuffle=False):
    tr1 = dstupple[0]
    tr2 = dstupple[1]
    tr3 = dstupple[2]
    te1 = dstupple[3]
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
        
        results.append(resDict)
        rCC.Reset()
        if reshuffle:
            tr1.RedistData()
            tr2.RedistData()
            tr3.RedistData()
            te1.RedistData()
    
    return results

# <codecell>

# revCC.Reset()

# <codecell>

# jamesds = (jtrain1, jtrain2, jtrain3, jtest1)

# <codecell>

# jamesresults = ConsolidatedTrainingTest(jamesds, 1, revCC, 3000)

# <codecell>

# jtrain1.GetSize(DSDatatype.TRAIN)

