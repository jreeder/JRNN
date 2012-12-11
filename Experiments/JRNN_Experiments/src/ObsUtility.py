# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

import PyJRNN #_d as PyJRNN # for debugging
import pyublas
import numpy
#from PyJRNN_d.utility import DSDatatype
from PyJRNN.utility import DSDatatype
import os
import re
import json
import pickle
from localvars import sourcebase

# <codecell>
#userspath = "D:\Users\John Reeder\Code\opennero\Build\dist\Debug\jrnnexp1\users" # For Titan
userspath = sourcebase + "/opennero/Build/dist/Debug/jrnnexp1/Users" # For Lab Comp --- Should work for both now.
# <codecell>


class UserSettings:
    def __init__(self, turnSensitivity=30, deadzone=15):
        self.turnSensitivity = turnSensitivity
        self.deadzone = deadzone


def LoadUserSettings(user):
    filepath = os.path.join(userspath, user, "settings.pickle")
    uSettings = pickle.load(open(filepath))
    return uSettings


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


sortField = re.compile('eps_(\d+)')


def LoadTrainingData(listOfTrainingFiles):
    dataArray = []
    for fileName in listOfTrainingFiles:
        with open(fileName, 'r') as fileh:
            dataArray.append(json.load(fileh))
    
    return dataArray


def Norm01Array(inArray):
    minval = numpy.min(inArray, axis=0)
    maxval = numpy.max(inArray, axis=0)
    inArray -= minval
    inArray /= (maxval - minval + 0.000001)
    return inArray


def matDoubleFromArray(inMat):
    newMat = PyJRNN.types.matDouble()
    for vec in inMat:
        if isinstance(vec, numpy.float64):
            newMat.append(numpy.array([vec]))
        else:
            newMat.append(vec)
    return newMat

def SplitAndAdjustObsOutputs(array, settings=UserSettings()):
    #This is purpose built for the outputs of the game
    splitRange = CreateRanges(-1.0, 2, 2)
    adjustArray = numpy.array([1, (100.0 / settings.turnSensitivity)])
    tmpArray = array * adjustArray
    tmpArray1 = map(lambda x: DiscritizeIntoRanges(x, splitRange, False), tmpArray[:,0])
    tmpArray2 = map(lambda x: DiscritizeIntoRanges(x, splitRange, False), tmpArray[:,1])
    #retArray = numpy.concatenate((tmpArray1, tmpArray2), axis=1) this does weird things when checking the size
    retArray = numpy.array([x[0] + x[1] for x in zip(tmpArray1, tmpArray2)])
    return retArray


def CreateUserDatasets(userData, numFrames, indexes = [1,2,3,4], distNums = (), normalize=True, splitOuts=True, settings=UserSettings()):
    train1 = PyJRNN.utility.Dataset()
    train2 = PyJRNN.utility.Dataset()
    train3 = PyJRNN.utility.Dataset()
    test1 = PyJRNN.utility.Dataset()

    numTrain = 0
    numVal = 0
    numTest = 0
    
    if len(distNums) > 0:
        numTrain = distNums[0]
        numVal = distNums[1]
        numTest = distNums[2]
    else:
        numTrain = 0.8 * numFrames
        numVal = 0.2 * numFrames
        numTest = numFrames
    
    minSize = min([len(x) for x in userData])
    strideSize = minSize / numFrames

    def loadDataset(ds, userDataList, settings, normalize=True, splitOuts=True):
        inputs = numpy.array([item['sensors'] for item in userDataList[-minSize::strideSize]])
        outputs = numpy.array([item['actions'] for item in userDataList[-minSize::strideSize]])
        if splitOuts:
            outputs = SplitAndAdjustObsOutputs(outputs, settings)
        if normalize:
            inputs = Norm01Array(inputs)
            outputs = Norm01Array(outputs)
        ds.LoadFromMatDoubles(matDoubleFromArray(inputs), matDoubleFromArray(outputs), True)
    
    loadDataset(train1, userData[indexes[0]], settings, normalize, splitOuts)
    loadDataset(train2, userData[indexes[1]], settings, normalize, splitOuts)
    loadDataset(train3, userData[indexes[2]], settings, normalize, splitOuts)
    loadDataset(test1, userData[indexes[3]], settings, normalize, splitOuts)
    train1.DistData(int(numTrain), int(numVal), 0)
    train2.DistData(int(numTrain), int(numVal), 0)
    train3.DistData(int(numTrain), int(numVal), 0)
    test1.DistData(0,0,numTest)
    
    return (train1, train2, train3, test1)


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

def CreateRanges(minVal, dist, numRanges=5, percentages=[], symetric=False):
    ranges = []
    if len(percentages) == 0:
        rangeWidth = dist / float(numRanges)
        startp = minVal
        for i in range(numRanges):
            r = (startp, startp + rangeWidth)
            ranges.append(r)
            startp = startp + rangeWidth
    else:
        if symetric == False:
            startp = minVal
            for i, v in enumerate(percentages):
                tmpWidth = dist * v
                r = (startp, startp + tmpWidth)
                ranges.append(r)
                startp = startp + tmpWidth
        else:
            startp = minVal
            for i, v in enumerate(percentages):
                tmpWidth = dist * v
                r = (startp, startp + tmpWidth)
                ranges.append(r)
                startp = startp + tmpWidth
            
            for i, v in enumerate(reversed(percentages)):
                if i == 0:
                    continue
                tmpWidth = dist * v
                r = (startp, startp + tmpWidth)
                ranges.append(r)
                startp = startp + tmpWidth
                
    return ranges


def GetIndexFromRange(value, ranges, absolute=True):
    outIndex = -1
    outValue = -1\
    
    #check to make sure that the value doesn't fall outside of the ranges
    if value < ranges[0][0]:
        value = ranges[0][0]
    
    if value > ranges[-1][1]:
        value = ranges[-1][1]
        
    for i, v in enumerate(ranges):
        if value >= v[0] and value <= v[1]:
            if absolute:
                return i, 1
            else:
                if value <= 0:
                    outVal = (value - v[1]) / (v[0] - v[1])
                else:
                    outVal = (value - v[0]) / (v[1] - v[0])
                return i, outVal


def DiscritizeIntoRanges(Value, ranges, absolute=True):
    outArray = [0.0] * len(ranges)
    i, val = GetIndexFromRange(Value, ranges, absolute)
    outArray[i] = val
    return outArray