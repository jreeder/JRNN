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


def ScoreFromInputs(inputVec):
    #This is based off of the second to last value which is the distance to the target
    numClose = sum([1 for x in inputVec[:, -2] if x < 0.15]) # this translates to true if closer than 45 units
    return numClose / (len(inputVec) * 1.0) * 30 # percentage of run close * 30 second run. 


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

def SplitAndAdjustObsOutputs(array, settings=UserSettings(), useFannedTurn=False):
    #This is purpose built for the outputs of the game
    splitRange = CreateRanges(-1.0, 2, 2)
    fannedRange = CreateRanges(-1, 2, percentages=[0.25, 0.15, 0.075, 0.05], symetric=True)
    adjustArray = numpy.array([1, (100.0 / settings.turnSensitivity)])
    tmpArray = array * adjustArray
    tmpArray1 = map(lambda x: DiscritizeIntoRanges(x, splitRange, False), tmpArray[:,0])
    tmpArray2 = []
    if useFannedTurn:
        tmpArray2 = map(lambda x: DiscritizeIntoRanges(x, fannedRange, True), tmpArray[:,1])
    else:    
        tmpArray2 = map(lambda x: DiscritizeIntoRanges(x, splitRange, False), tmpArray[:,1])
    #retArray = numpy.concatenate((tmpArray1, tmpArray2), axis=1) this does weird things when checking the size
    retArray = numpy.array([x[0] + x[1] for x in zip(tmpArray1, tmpArray2)])
    return retArray


def CreateUserDatasets(userData, numFrames, indexes = [1,2,3,4], distNums = (), normalize=True, splitOuts=True, settings=UserSettings(), useFannedTurn=False):
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

    def loadDataset(ds, userDataList, settings, normalize=True, splitOuts=True, useFannedTurn=False):
        inputs = numpy.array([item['sensors'] for item in userDataList[-minSize::strideSize]])
        outputs = numpy.array([item['actions'] for item in userDataList[-minSize::strideSize]])
        if splitOuts:
            outputs = SplitAndAdjustObsOutputs(outputs, settings, useFannedTurn=useFannedTurn)
        if normalize:
            inputs = Norm01Array(inputs)
            outputs = Norm01Array(outputs)
        ds.LoadFromMatDoubles(matDoubleFromArray(inputs), matDoubleFromArray(outputs), True)
    
    loadDataset(train1, userData[indexes[0]], settings, normalize, splitOuts, useFannedTurn)
    loadDataset(train2, userData[indexes[1]], settings, normalize, splitOuts, useFannedTurn)
    loadDataset(train3, userData[indexes[2]], settings, normalize, splitOuts, useFannedTurn)
    loadDataset(test1, userData[indexes[3]], settings, normalize, splitOuts, useFannedTurn)
    train1.DistData(int(numTrain), int(numVal), 0)
    train2.DistData(int(numTrain), int(numVal), 0)
    train3.DistData(int(numTrain), int(numVal), 0)
    test1.DistData(0,0,numTest)
    
    return (train1, train2, train3, test1)


def loadCSMTLDSfromData(outDS, data, indexes, numFrames, taskName, normalize=True, splitOuts=True, settings=UserSettings()):
    subData = [data[x] for x in indexes]
    minSize = min([len(x) for x in subData])
    strideSize = minSize / numFrames
    for d in subData:
        inputs = numpy.array([item['sensors'] for item in d[-minSize::strideSize]])
        outputs = numpy.array([item['actions'] for item in d[-minSize::strideSize]])
        if splitOuts:
            outputs = SplitAndAdjustObsOutputs(outputs, settings)
        if normalize:
            inputs = Norm01Array(inputs)
            outputs = Norm01Array(outputs)
            
        outDS.AddMatDoublesToTask(matDoubleFromArray(inputs), matDoubleFromArray(outputs), taskName)


def ConsolidatedTrainingTest(dstupple, numRuns, rCC, maxEpochs, reshuffle=False, WinnerGroups = []):
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
        if len(WinnerGroups) > 0:
            resDict['tr1.winError'] = TestWiWinners(te1, DSDatatype.TEST, rCC, WinnerGroups)
        print "Finished Train and Test 1"
        
        rCC.TrainTask(tr2, maxEpochs, True)
        resDict['tr2.epochs'] = rCC.net1vals.epochs
        resDict['tr2.hiddenLayers'] = rCC.net1vals.numHidLayers
        resDict['tr2.testError'] = rCC.TestOnData(te1, DSDatatype.TEST)
        if len(WinnerGroups) > 0:
            resDict['tr2.winError'] = TestWiWinners(te1, DSDatatype.TEST, rCC, WinnerGroups)        
        print "Finished Train and Test 2"
        
        rCC.TrainTask(tr3, maxEpochs, True)
        resDict['tr3.epochs'] = rCC.net1vals.epochs
        resDict['tr3.hiddenLayers'] = rCC.net1vals.numHidLayers
        resDict['tr3.testError'] = rCC.TestOnData(te1, DSDatatype.TEST)
        if len(WinnerGroups) > 0:
            resDict['tr3.winError'] = TestWiWinners(te1, DSDatatype.TEST, rCC, WinnerGroups)        
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


def GetWinnerIndex(vecD, indList):
    highVal = -1  # This is only for outs with 0-1 right now
    highInd = -1
    for index in indList:
        if vecD[index] > highVal:
            highVal = vecD[index]
            highInd = index
    if highVal < 0.1:
        highInd = -1  # This is meant to tell that both were very small and neigther should activate
    return highInd, highVal


def TestWiWinners(ds, dstype, rCC, winnerGroups):
    inputs = ds.GetInputs(dstype)
    outputs = ds.GetOutputs(dstype)
    size = len(inputs)
    totalOpps = size * len(winnerGroups)
    winnerOuts = sum([len(x) for x in winnerGroups])
    assert winnerOuts == len(outputs[0])
    numInCorrect = 0
    realError = 0
    groupInc = [0] * len(winnerGroups)
    groupRealErr = [0.0] * len(winnerGroups)
    
    for example in zip(inputs, outputs):
        netOut = rCC.Activate(example[0])
        for i, grp in enumerate(winnerGroups):
            netWinInd, netWinVal = GetWinnerIndex(netOut, grp)
            desWinInd, desWinVal = GetWinnerIndex(example[1], grp)
            if netWinInd != desWinInd:
                numInCorrect += abs(netWinInd - desWinInd)  # This will give a more useful value when there are more than two in a group. It will give the distance away from the desired. 
                groupInc[i] += 1
            
            realError += (desWinVal - netWinVal)**2
            groupRealErr[i] += (desWinVal - netWinVal)**2
            
    totalErrorRate = numInCorrect / (totalOpps * 1.0)
    groupErrorRate = [x / (size *1.0) for x in groupInc]
    totalMSE = realError / (totalOpps * 1.0)
    groupMSE = [x / (size * 1.0) for x in groupRealErr]
    returnDict = {'totalErrorRate': totalErrorRate, 'groupErrorRate': groupErrorRate, \
                  'totalMSE': totalMSE, 'groupMSE': groupMSE}
    
    return returnDict


def RevCCWorker(RevCCTrainer, results, numRuns, subView1, subView2, cds, testRecall, useValidation=True, maxEpochs=3000):
    dt = DSDatatype.TEST
    if(testRecall):
        dt = DSDatatype.TRAIN
        
    
    useRealOuts = cds.realOuts
    
    def printHashedDoubleMap(DM):
        returnStr = ""
        for elm in DM:
            returnStr += "{0}:{1}\t".format(elm.key(), elm.data())
            
        return returnStr
    
    
    if (subView1 != '' and subView2 != ''):
        subview = strings()
        subview.append(subView1)
        cds.DistSubview(subview)
        firstDS = cds.SpawnDS()
        del subview[:]
        subview.append(subView2)
        cds.DistSubview(subview)
        secondDS = cds.SpawnDS()
        
        resultArray = []
        
        for i in range(numRuns):
            resultDict = {'Chase':{}, 'ChaseObstacles':{}}
            #Run First Task
            RevCCTrainer.TrainTask(firstDS, maxEpochs, useValidation)
            #First Task Results
            resultDict['Chase']['epochs'] = RevCCTrainer.net1vals.epochs
            resultDict['Chase']['hiddenLayers'] = RevCCTrainer.net1vals.hiddenLayers
            resultDict['Chase']['numResets'] = RevCCTrainer.net1vals.hiddenLayers
            resultDict['Chase']['MSERec'] = RevCCTrainer.net1vals.MSERec
            #Test on second test before training on it. 
            if useRealOuts:
                firstResults = RevCCTrainer.TestOnData(secondDS, DSDatatype.TEST)
            else:
                firstResults = RevCCTrainer.TestWiClass(secondDS, DSDatatype.TEST)
                
            resultDict['Chase']['firstResults'] = firstResults
            
            #Run Second Task
            RevCCTrainer.TrainTask(secondDS, maxEpochs, useValidation, True, firstDS, dt)
            
            #SecondTask Results
            if useRealOuts:
                secondResults = RevCCTrainer.TestOnData(secondDS, DSDatatype.TEST)
            else:
                secondResults = RevCCTrainer.TestWiClass(secondDS, DSDatatype.TEST)
            hiddenLayers = RevCCTrainer.net1vals.numHidLayers
            epochs = RevCCTrainer.net1vals.epochs
            numResets = RevCCTrainer.net1vals.numResets
            
            resultDict['ChaseObstacles']['epochs'] = epochs
            resultDict['ChaseObstacles']['hiddenLayers'] = hiddenLayers
            resultDict['ChaseObstacles']['numResets'] = numResets
            resultDict['ChaseObstacles']['secondResults'] = secondResults
            
            time = 0
            result = ""
            result += "{0}\t".format(epochs)
            result += "{0}\t".format(time)
            result += "{0}\t".format(hiddenLayers)
            result += "{0}\t".format(numResets)
            if useRealOuts:
                result += "{0}:{1}\t|\t".format('task-0', secondResults)
            else:
                result += printHashedDoubleMap(secondResults) + "|\t"
                
            result += "\t".join([str(x) for x in RevCCTrainer.net1vals.MSERec]) + "\t|\t"
        
            resultDict['ChaseObstacles']['MSERec'] = RevCCTrainer.net1vals.MSERec
        
            if useRealOuts:
                firstTaskResults = RevCCTrainer.TestOnData(firstDS, dt)
                result += "{0}:{1}\t|\t".format('task-0', firstTaskResults)
            else:
                firstTaskResults = RevCCTrainer.TestWiClass(firstDS, dt)
                result += printHashedDoubleMap(firstTaskResults) + "|\t"
            
            resultDict['ChaseObstacles']['firstTaskResults'] = firstTaskResults
            
            tmpResults = RevCCTrainer.getTestWhileTrainResults()
            
            resultDict['ChaseObstacles']['testWhileTrainResults'] = tmpResults
            
            for trainResult in tmpResults:
                result += "*{0}!{1}".format(trainResult.epoch, printHashedDoubleMap(trainResult.result))
            
            result += "$\n"
            results.append(result)
            resultArray.append(resultDict)
            result = ""
            RevCCTrainer.Reset()
            firstDS.RedistData()
            secondDS.RedistData()
            
        return resultArray


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