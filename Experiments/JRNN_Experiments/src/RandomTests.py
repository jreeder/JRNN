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

parallelTest('Cong', 'Chase - 16Nov2012171621', maxEpochs=maxEpochs, numFrames=numFrames, indexes=indexes, distNums=distNums, numRev=numRev, reshuffle=reshuffle, useFannedTurn=useFannedTurn)