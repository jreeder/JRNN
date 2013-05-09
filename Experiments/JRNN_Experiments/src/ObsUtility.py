# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

debug = True

if debug:
    import PyJRNN_d as PyJRNN # for debugging
    from PyJRNN_d.utility import DSDatatype, CSMTLDataset
    from PyJRNN_d.types import strings
    from PyJRNN_d.trainers import TestResults, TestResult    
else:
    import PyJRNN
    from PyJRNN.utility import DSDatatype, CSMTLDataset
    from PyJRNN.types import strings
    from PyJRNN.trainers import TestResults, TestResult    

import pyublas
import numpy
import os
import re
import json
import pickle
from localvars import sourcebase, luserspath

# <codecell>
#userspath = "D:\Users\John Reeder\Code\opennero\Build\dist\Debug\jrnnexp1\users" # For Titan
userspath = luserspath
# <codecell>


class UserSettings:
    def __init__(self, turnSensitivity=30, deadzone=15):
        self.turnSensitivity = turnSensitivity
        self.deadzone = deadzone


def ConvHashedDM(hashedDM):
    returnDict = {}
    for elm in hashedDM:
        returnDict[elm.key()] = elm.data()
        
    return returnDict


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


def loadCSMTLDSfromData(outDS, data, indexes, numFrames, taskName, normalize=True, splitOuts=True, useFannedTurn=True, settings=UserSettings()):
    subData = [data[x] for x in indexes]
    minSize = min([len(x) for x in subData])
    strideSize = minSize / numFrames
    for d in subData:
        inputs = numpy.array([item['sensors'] for item in d[-minSize::strideSize]])
        outputs = numpy.array([item['actions'] for item in d[-minSize::strideSize]])
        if splitOuts:
            outputs = SplitAndAdjustObsOutputs(outputs, settings, useFannedTurn)
        if normalize:
            inputs = Norm01Array(inputs)
            outputs = Norm01Array(outputs)
            
        outDS.AddMatDoublesToTask(matDoubleFromArray(inputs), matDoubleFromArray(outputs), taskName)


def ConsolidatedTrainingTest(dstupple, numRuns, rCC, maxEpochs, reshuffle=False, WinnerGroups = [], realOuts = True):
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
        if realOuts:
            resDict['tr1.testError'] = rCC.TestOnData(te1, DSDatatype.TEST)
        else:
            resDict['tr1.testError'] = ConvHashedDM(rCC.TestWiClass(te1, DSDatatype.TEST))
        if len(WinnerGroups) > 0:
            resDict['tr1.winError'] = TestWiWinners(te1, DSDatatype.TEST, rCC, WinnerGroups)
        print "Finished Train and Test 1"
        
        rCC.TrainTask(tr2, maxEpochs, True)
        resDict['tr2.epochs'] = rCC.net1vals.epochs
        resDict['tr2.hiddenLayers'] = rCC.net1vals.numHidLayers
        if realOuts:
            resDict['tr2.testError'] = rCC.TestOnData(te1, DSDatatype.TEST)
        else:
            resDict['tr2.testError'] = ConvHashedDM(rCC.TestWiClass(te1, DSDatatype.TEST))
        if len(WinnerGroups) > 0:
            resDict['tr2.winError'] = TestWiWinners(te1, DSDatatype.TEST, rCC, WinnerGroups)        
        print "Finished Train and Test 2"
        
        rCC.TrainTask(tr3, maxEpochs, True)
        resDict['tr3.epochs'] = rCC.net1vals.epochs
        resDict['tr3.hiddenLayers'] = rCC.net1vals.numHidLayers
        if realOuts:
            resDict['tr3.testError'] = rCC.TestOnData(te1, DSDatatype.TEST)
        else:
            resDict['tr3.testError'] = ConvHashedDM(rCC.TestWiClass(te1, DSDatatype.TEST))
        if len(WinnerGroups) > 0:
            resDict['tr3.winError'] = TestWiWinners(te1, DSDatatype.TEST, rCC, WinnerGroups)        
        print "Finished Train and Test 3"
        
        #print resDict
        
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


def SumSquareError(desired, actual, indList = []):
    indexes = indList if len(indList) > 0 else range(len(desired))
    SSE = 0
    for i in indexes:
        SSE += (desired[i] - actual[i])**2
        
    return SSE


def TestWiWinners(ds, dstype, rCC, winnerGroups):
    inputs = ds.GetInputs(dstype)
    outputs = ds.GetOutputs(dstype)
    size = len(inputs)
    #totalOpps = size * len(winnerGroups)
    totalDist = size * sum([x[-1] - x[0] for x in winnerGroups])
    grpDists = [size * x[-1] - x[0] for x in winnerGroups]
    winnerOutsSize = sum([len(x) for x in winnerGroups])
    grpOutsSize = [len(x) for x in winnerGroups]
    assert winnerOutsSize == len(outputs[0])
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
                groupInc[i] += abs(netWinInd - desWinInd)
            
            groupRealErr[i] += SumSquareError(example[1], netOut, grp)
            #realError += (desWinVal - netWinVal)**2
            #groupRealErr[i] += (desWinVal - netWinVal)**2
        realError += SumSquareError(example[1], netOut)
            
    totalErrorRate = numInCorrect / (totalDist * 1.0) # Total Distance from correct as a percentage of max possible distance incorrect
    groupErrorRate = [x / (grpDists[i] * 1.0) for i,x in enumerate(groupInc)] # Group Distance from correct as a percentage of max possible distance incorrect
    totalMSE = realError / (size * winnerOutsSize * 1.0) # MSE of all outputs
    groupMSE = [x / (size * grpOutsSize[i] * 1.0) for i, x in enumerate(groupRealErr)] # MSE of grouped outputs
    returnDict = {'totalErrorRate': totalErrorRate, 'groupErrorRate': groupErrorRate, \
                  'totalMSE': totalMSE, 'groupMSE': groupMSE}
    
    return returnDict


def RevCCWorker(RevCCTrainer, results, numRuns, subView1, subView2, cds, testRecall, useValidation=True, maxEpochs=3000):
    print 'Worker Starting'
    dt = DSDatatype.TEST
    if(testRecall):
        dt = DSDatatype.TRAIN
        
    
    useRealOuts = cds.realOuts
    
    def printHashedDoubleMap(DM):
        returnStr = ""
        for elm in DM:
            returnStr += "{0}:{1}\t".format(elm.key(), elm.data())
            
        return returnStr
    
    def ConvTestResults(testResults):
        returnArray = []
        for result in testResults:
            returnArray.append((result.epoch, ConvHashedDM(result.result)))
            
        return returnArray
    
    def ConvHashedDM(hashedDM):
        returnDict = {}
        for elm in hashedDM:
            returnDict[elm.key()] = elm.data()
            
        return returnDict
    
    
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
            print 'Starting Run {0}'.format(i)
            print 'useRealOuts: {0}'.format(cds.realOuts)
            resultDict = {subView1:{}, subView2:{}}
            #Run First Task
            RevCCTrainer.TrainTask(firstDS, maxEpochs, useValidation)
            print 'Finished First Task'
            #First Task Results
            resultDict[subView1]['epochs'] = RevCCTrainer.net1vals.epochs
            resultDict[subView1]['hiddenLayers'] = RevCCTrainer.net1vals.numHidLayers
            resultDict[subView1]['numResets'] = RevCCTrainer.net1vals.numResets
            resultDict[subView1]['MSERec'] = [x for x in RevCCTrainer.net1vals.MSERec]
            #Test on second test before training on it. 
            if useRealOuts:
                firstResults = RevCCTrainer.TestOnData(firstDS, DSDatatype.TEST)
                resultDict[subView1]['firstResults'] = firstResults
            else:
                firstResults = RevCCTrainer.TestWiClass(firstDS, DSDatatype.TEST)
                resultDict[subView1]['firstResults'] = ConvHashedDM(firstResults)
                
            
            
            #Run Second Task
            RevCCTrainer.TrainTask(secondDS, maxEpochs, useValidation, True, firstDS, dt)
            print 'Finished Second Task'
            #SecondTask Results
            if useRealOuts:
                secondResults = RevCCTrainer.TestOnData(secondDS, DSDatatype.TEST)
                resultDict[subView2]['secondResults'] = secondResults
            else:
                secondResults = RevCCTrainer.TestWiClass(secondDS, DSDatatype.TEST)
                resultDict[subView2]['secondResults'] = ConvHashedDM(secondResults)
                
            hiddenLayers = RevCCTrainer.net1vals.numHidLayers
            epochs = RevCCTrainer.net1vals.epochs
            numResets = RevCCTrainer.net1vals.numResets
            
            resultDict[subView2]['epochs'] = epochs
            resultDict[subView2]['hiddenLayers'] = hiddenLayers
            resultDict[subView2]['numResets'] = numResets
            
            
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
        
            resultDict[subView2]['MSERec'] = [x for x in RevCCTrainer.net1vals.MSERec]
        
            if useRealOuts:
                firstTaskResults = RevCCTrainer.TestOnData(firstDS, dt)
                result += "{0}:{1}\t|\t".format('task-0', firstTaskResults)
                resultDict[subView2]['firstTaskResults'] = firstTaskResults
            else:
                firstTaskResults = RevCCTrainer.TestWiClass(firstDS, dt)
                result += printHashedDoubleMap(firstTaskResults) + "|\t"
                resultDict[subView2]['firstTaskResults'] = ConvHashedDM(firstTaskResults)
            
            
            tmpResults = RevCCTrainer.getTestWhileTrainResults()
            
            resultDict[subView2]['testWhileTrainResults'] = ConvTestResults(tmpResults)
            
            for trainResult in tmpResults:
                result += "*{0}!{1}".format(trainResult.epoch, printHashedDoubleMap(trainResult.result))
            
            result += "$\n"
            results.append(result)
            resultArray.append(resultDict)
            result = ""
            RevCCTrainer.Reset()
            firstDS.RedistData()
            secondDS.RedistData()
            print 'Run {0} complete'.format(i)
            
        print 'Worker Done'
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




def Create3DBars(datalist, firstKey, secondKey="", adjVal = 1.0):

    def Plot3dBar(xvals, yvals, xinds, yinds, zvals):
        fig = plt.figure()
        ax = fig.gca(projection='3d')
        sxvals = sorted(xvals)
        syvals = sorted(yvals)
        xvalli = list(sxvals)
        yvalli = list(syvals)
        XCoord = np.array([xvalli.index(x) for x in xinds])
        YCoord = np.array([yvalli.index(x) for x in yinds])
        ZCoord1 = np.array(zvals)
        zinds = np.zeros_like(XCoord)
        dx = np.ones_like(XCoord)
        dy = np.ones_like(YCoord)
        ax.bar3d(XCoord - 0.25, YCoord - 0.25, zinds, 0.5, 0.5, ZCoord1)
        plt.xticks(range(len(sxvals)),[str(x) for x in sxvals])
        plt.yticks(range(len(syvals)),[str(x) for x in syvals])
        plt.show()

    # This will index over numTrain and numRev x will be nT y will be numRev
    global findNtr, findNrev
    xvals = set()
    yvals = set()
    xinds = []
    yinds = []
    tr1z = []
    tr2z = []
    tr3z = []
    for fileN in datalist:
        xval = int(findNrev.search(fileN).group(0))
        yval = int(findNtr.search(fileN).group(0))
        xvals.add(xval)
        yvals.add(yval)
        xinds.append(xval)
        yinds.append(yval)
        tmpVal = None
        if secondKey == "":
            tmpVal = ParseObsResult(fileN, firstKey)
        else:
            tmpVal = ParseObsResult(fileN, firstKey, secondKey, adjVal)
        
        tr1z.append(tmpVal['Average']['tr1'])
        tr2z.append(tmpVal['Average']['tr2'])
        tr3z.append(tmpVal['Average']['tr3'])
        
    Plot3dBar(xvals, yvals, xinds, yinds, tr1z)


def AnnotatedLegend(ax, lb, ub, epoch=-1):
    lines = ax.lines
    box = ax.get_position()
    ax.set_position([box.x0, box.y0, box.width * 0.9, box.height])
    annposs = []
    for line in lines:
        data = line.get_data()
        label = line.get_label()
        endpoint = epoch if len(data[1]) > epoch else len(data[1]) - 1
        avgy = numpy.mean(data[1][endpoint-10:endpoint])
        percy = (avgy - lb) / (1.0 * (ub - lb))
        annposs.append(avgy)
        ax.annotate(label, xy=(1.00, percy), xycoords='axes fraction', xytext=(1.03, percy), textcoords='axes fraction',
                    arrowprops=dict(arrowstyle="-", relpos=(0.0, 0.5)), verticalalignment='center')
    
    return annposs


def adjust_boxes(initpos, widths, lb, ub, max_iter=1000, adjust_factor=0.35, factor_decrement=2.0, fd_p=0.75):
    niter = 0
    changed=True
    npos = len(initpos)
    
    adjpos = [[x,y] for x,y in enumerate(initpos)]
    adjpos.sort(key=lambda x: x[1])
    while changed:
        changed = False
        for i in range(npos):
            if i > 0:
                diff1 = adjpos[i][1] - adjpos[i-1][1]
                separation1 = (widths[i] + widths[i-1]) / 1.8
            else:
                diff1 = adjpos[i][1] - lb + widths[i] * 1.01
                separation1 = widths[i]
            
            if i < npos - 2:
                diff2 = adjpos[i+1][1] - adjpos[i][1]
                separation2 = (widths[i] + widths[i+1]) / 1.8
            else:
                diff2 = ub + widths[i] * 1.01 - adjpos[i][1]
                separation2 = widths[i]
            
            if diff1 < separation1 or diff2 < separation2:
                if adjpos[i][1] == lb: diff1 = 0
                if adjpos[i][1] == ub: diff2 = 0
                if diff2 > diff1:
                    adjpos[i][1] = adjpos[i][1] + separation2 * adjust_factor
                    adjpos[i][1] = adjpos[i][1] if adjpos[i][1] < ub else ub
                else:
                    adjpos[i][1] = adjpos[i][1] - separation1 * adjust_factor
                    adjpos[i][1] = adjpos[i][1] if adjpos[i][1] > lb else lb
                changed = True
            niter += 1
        if niter == max_iter * fd_p:
            adjust_factor /= factor_decrement
        if niter >= max_iter:
            break
    
    adjpos.sort(key=lambda x: x[0])
    
    return [(x[1] - lb) / (1.0 * (ub - lb)) for x in adjpos], changed, niter