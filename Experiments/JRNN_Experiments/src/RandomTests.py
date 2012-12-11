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

import PyJRNN_d as pyj
import pyublas
import numpy
from PyJRNN_d.utility import DSDatatype
import os
import re
import json
import ObsUtility
from ObsUtility import userspath, UserSettings

def parallelTest(user, scenario, maxEpochs=2000, numFrames=900, numCand=4, numRev=5, numRevTrainRatio=1, indexes = [1,2,3,4], distNums = (), reshuffle=False):
    global sortField
    scenarios = ObsUtility.LoadUserData(user)
    settings = ObsUtility.LoadUserSettings(user)
    training = scenarios[scenario]['training']
    training.sort(key=lambda x: int(ObsUtility.sortField.search(x).group(1)))
    data = ObsUtility.LoadTrainingData(training)
    dstuple = ObsUtility.CreateUserDatasets(data, numFrames, indexes, distNums, False, True, settings) # this is going to do splitouts by default and not normalize with this config
    del scenarios
    del training
    del data
    numInputs = dstuple[0].numInputs
    numOutputs = dstuple[0].numOutputs
    revCC = pyj.trainers.RevCCTrainer(numInputs,numOutputs,numCand)
    revCC.revparams.numRevTrainRatio = numRevTrainRatio
    revCC.revparams.numRev = numRev
    
    returnVal = ObsUtility.ConsolidatedTrainingTest(dstuple, 1, revCC, maxEpochs, reshuffle)
    for ds in dstuple:
        ds.Clear()

    return returnVal
    #return "hello world"
    

maxEpochs=300
numFrames=900
trialNum=1
numRuns=5
indexes=[1,2,3,4]
distNums=(200,200,900)

parallelTest('Cong', 'Chase - 16Nov2012171621', maxEpochs=maxEpochs, numFrames=numFrames, indexes=indexes, distNums=distNums)