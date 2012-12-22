#from ObsUtility import *

#scenarios = LoadUserData('Cong')

#training = scenarios['Chase - 16Nov2012171621']['training']
#training.sort(key=lambda x: int(sortField.search(x).group(1)))
#data = LoadTrainingData(training)

#import numpy

#size = len(data[1])
#stride = size / 900
#inputs = numpy.array([x['sensors'] for x in data[1][::stride]])
#outputs = numpy.array([x['actions'] for x in data[1][::stride]])

#import os
#import pickle

#settings = LoadUserSettings('Cong')


#adjustTurnVal = 100.0 / settings.turnSensitivity

#adjustArray = numpy.array([1, adjustTurnVal])
#newOutputs = outputs * adjustArray

#testranges2 = CreateRanges(-1, 2, 2)

#tmpArray = map(lambda x: DiscritizeIntoRanges(x, testranges2, False), newOutputs[:, 1])

#tmpArray2 = map(lambda x: DiscritizeIntoRanges(x, testranges2, False), newOutputs[:, 0])

import PyJRNN_d as PyJRNN
import pyublas
import numpy
from PyJRNN_d.utility import DSDatatype
import os
import re
import json
import ObsUtility
from ObsUtility import userspath, UserSettings
from config import datapath, LoadTabbedDataFiles

#def parallelTest(user, scenario, maxEpochs=2000, numFrames=900, numCand=4, numRev=5, numRevTrainRatio=1, indexes = [1,2,3,4], distNums = (), reshuffle=False):
    #global sortField
    #scenarios = ObsUtility.LoadUserData(user)
    #settings = ObsUtility.LoadUserSettings(user)
    #training = scenarios[scenario]['training']
    #training.sort(key=lambda x: int(ObsUtility.sortField.search(x).group(1)))
    #data = ObsUtility.LoadTrainingData(training)
    #dstuple = ObsUtility.CreateUserDatasets(data, numFrames, indexes, distNums, False, True, settings) # this is going to do splitouts by default and not normalize with this config
    #del scenarios
    #del training
    #del data
    #numInputs = dstuple[0].numInputs
    #numOutputs = dstuple[0].numOutputs
    #revCC = PyJRNN.trainers.RevCCTrainer(numInputs,numOutputs,numCand)
    #revCC.revparams.numRevTrainRatio = numRevTrainRatio
    #revCC.revparams.numRev = numRev
    
    #returnVal = ObsUtility.ConsolidatedTrainingTest(dstuple, 1, revCC, maxEpochs, reshuffle)
    #for ds in dstuple:
        #ds.Clear()

    #return returnVal
    ##return "hello world"

#def parallelTest(user, scenario, maxEpochs=2000, numFrames=900, numCand=4, numRev=5, numRevTrainRatio=1, indexes = [1,2,3,4], distNums = (), reshuffle=False):
    #global sortField
    #scenarios = ObsUtility.LoadUserData(user)
    #settings = ObsUtility.LoadUserSettings(user)
    #training = scenarios[scenario]['training']
    #training.sort(key=lambda x: int(ObsUtility.sortField.search(x).group(1)))
    #data = ObsUtility.LoadTrainingData(training)
    #dstuple = ObsUtility.CreateUserDatasets(data, numFrames, indexes, distNums, True, True, settings) # this is going to do splitouts by default and not normalize with this config
    #del scenarios
    #del training
    #del data
    #numInputs = dstuple[0].numInputs
    #numOutputs = dstuple[0].numOutputs
    #revCC = PyJRNN.trainers.RevCCTrainer(numInputs,numOutputs,numCand)
    #revCC.revparams.numRevTrainRatio = numRevTrainRatio
    #revCC.revparams.numRev = numRev
    
    #returnVal = ObsUtility.ConsolidatedTrainingTest(dstuple, 1, revCC, maxEpochs, reshuffle, [[0,1],[2,3]])
    #for ds in dstuple[:]:
        #ds.Clear()
        #del ds

    #return returnVal
    ##return "hello world"

def parallelTest(user, scenario, maxEpochs=2000, numFrames=900, numCand=4, numRuns=1, numRev=5, numRevTrainRatio=1, indexes = [1,2,3,4], distNums = (), reshuffle=False, useFannedTurn = False):
    global sortField
    scenarios = ObsUtility.LoadUserData(user)
    settings = ObsUtility.LoadUserSettings(user)
    training = scenarios[scenario]['training']
    training.sort(key=lambda x: int(ObsUtility.sortField.search(x).group(1)))
    data = ObsUtility.LoadTrainingData(training)
    dstuple = ObsUtility.CreateUserDatasets(data, numFrames, indexes, distNums, True, True, settings, useFannedTurn) # this is going to do splitouts by default and normalize with this config
    del scenarios
    del training
    del data
    numInputs = dstuple[0].numInputs
    numOutputs = dstuple[0].numOutputs
    revCC = PyJRNN.trainers.RevCCTrainer(numInputs,numOutputs,numCand)
    revCC.revparams.numRevTrainRatio = numRevTrainRatio
    revCC.revparams.numRev = numRev
    
    winnerGroups = []
    if useFannedTurn:
        winnerGroups = [[0,1],[2,3,4,5,6,7,8]]
    else:
        winnerGroups = [[0,1],[2,3]]
        
    returnVal = ObsUtility.ConsolidatedTrainingTest(dstuple, numRuns, revCC, maxEpochs, reshuffle, winnerGroups)
    for ds in dstuple[:]:
        ds.Clear()
        del ds

    return returnVal
    #return "hello world"

def ParallelTabbedConsTest(dsname, numIns, numOuts, numTrain, numVal, numTest, numCand=8, numRuns=1, numRev=5, maxEpochs=2000, numRevTrainRatio=1, reshuffle=True, fileName=""):
    def CreateTabbedConsDS(dsname, numIns, numOuts, numTrain, numVal, numTest):
        global datapath
        dspath = os.path.join(datapath, dsname)
        ins, outs = LoadTabbedDataFiles(numIns, numOuts, dspath)
        trainDSLength = numTrain + numVal
        print trainDSLength
    
        ds1 = PyJRNN.utility.Dataset()
        ds2 = PyJRNN.utility.Dataset()
        ds3 = PyJRNN.utility.Dataset()
        tr1 = PyJRNN.utility.Dataset()
    
        ds1.LoadFromMatDoubles(ObsUtility.matDoubleFromArray(ins[0:trainDSLength]), ObsUtility.matDoubleFromArray(outs[0:trainDSLength]))
        ds2.LoadFromMatDoubles(ObsUtility.matDoubleFromArray(ins[trainDSLength:2*trainDSLength]), ObsUtility.matDoubleFromArray(outs[trainDSLength:2*trainDSLength]))
        ds3.LoadFromMatDoubles(ObsUtility.matDoubleFromArray(ins[trainDSLength*2:trainDSLength*3]), ObsUtility.matDoubleFromArray(outs[trainDSLength*2:trainDSLength*3]))
        tr1.LoadFromMatDoubles(ObsUtility.matDoubleFromArray(ins[trainDSLength*3:]), ObsUtility.matDoubleFromArray(outs[trainDSLength*3:]))
    
        del ins, outs
        ds1.DistData(numTrain, numVal, 0)
        ds2.DistData(numTrain, numVal, 0)
        ds3.DistData(numTrain, numVal, 0)
        tr1.DistData(0,0,numTest)
    
        return (ds1, ds2, ds3, tr1)
    
    dstuple = CreateTabbedConsDS(dsname, numIns, numOuts, numTrain, numVal, numTest)
    revCC = PyJRNN.trainers.RevCCTrainer(numIns, numOuts, numCand)
    revCC.revparams.numRevTrainRatio = numRevTrainRatio
    if numRev == -1:
        revCC.revparams.bufferSize = 0
        numRev = 0
    revCC.revparams.numRev = numRev
    returnVal = ObsUtility.ConsolidatedTrainingTest(dstuple, numRuns, revCC, maxEpochs, reshuffle, realOuts=False)
    for ds in dstuple[:]:
        ds.Clear()
        del ds
    
    if fileName != "":
        with open(fileName, 'w') as fileh:
            json.dump(returnVal, fileh)
            
    return returnVal

import wingdbstub

maxEpochs=1000
numFrames=900
trialNum=5 # 1 was with 2 outputs - 2 is with 4 outputs - 3 is 4 outputs and Normalization turned on. - 4 has test with winner - 5 has test with winner and many varied parameters. 
numRuns=1
indexes=[1,2,3,4]
distNums=(50,200,900)
numRev=5
reshuffle=True
useFannedTurn=True

#parallelTest('Cong', 'Chase - 16Nov2012171621', maxEpochs=maxEpochs, numFrames=numFrames, indexes=indexes, distNums=distNums, numRev=numRev, reshuffle=reshuffle, useFannedTurn=useFannedTurn)

result = ParallelTabbedConsTest('band-task2.txt', 2, 1, 50, 100, 400, numRev=1)