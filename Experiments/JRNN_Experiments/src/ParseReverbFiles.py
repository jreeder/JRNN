#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<jreeder@gmail.com>
# Purpose: This is meant to parse the cc csmtl reverberation files. 
# Created: 4/10/2012

import os

class Bunch(object):
    def __init__(self, **kwds):
        self.__dict__.update(kwds)

def ParseFile(filename):
    filepath, filen = os.path.split(filename)
    thisName, ext = os.path.splitext(filen)  
    
    infile = open(filename, 'r')
    epochsArr = []
    timesArr = []
    errorsArr = []
    hidLayersArr = []
    epochMSEsArr = []
    resetsArr = []
    firstDSErrArr = []
    firstDSErrsWhileTrain = []
    #TODO Change this later to handle the $ token. 
    for line in infile:
        tasksplit = line.split("$")
        if len(tasksplit) == 2:
            firstsplit = tasksplit[0].split("|")
            if len(firstsplit) < 4: continue
            #results of the second task training
            secondTaskMetrics = firstsplit[0].split("\t")
            secondTaskrunMSEs = [float(x) for x in firstsplit[1].split("\t")[1:-1]]
            epochsArr.append(int(secondTaskMetrics[0]))
            timesArr.append(float(secondTaskMetrics[1]))
            hidLayersArr.append(int(secondTaskMetrics[2]))
            resetsArr.append(int(secondTaskMetrics[3]))
            tmpDict = {}
            for item in secondTaskMetrics[4:]:
                itemsplit = item.split(":")
                if len(itemsplit) > 1:
                    tmpDict[itemsplit[0]] = float(itemsplit[1])
            errorsArr.append(tmpDict)
            epochMSEsArr.append(secondTaskrunMSEs)
            
            #results from the first task trained while the second is training
            firsttaskerrs = firstsplit[2].split("\t")
            tmpDict.clear()
            for item in firsttaskerrs:
                itemsplit = item.split(":")
                if len(itemsplit) > 1:
                    tmpDict[itemsplit[0]] = float(itemsplit[1])
            firstDSErrArr.append(tmpDict)
            testWhileTrain = [x.strip() for x in firstsplit[3].split("*")]
            tmpDict.clear()
            for item in testWhileTrain:
                itemsplit = item.split("!")
                if len(itemsplit) > 1:
                    secsplit = itemsplit[1].split(":")
                    tmpDict[int(itemsplit[0])] = secsplit[1]
            firstDSErrsWhileTrain.append(tmpDict)
    
    infile.close()
    retVal = Bunch(name = thisName, resets=resetsArr,\
        epochs=epochsArr, times=timesArr, errors=errorsArr, hidLayers=hidLayersArr,\
        epochMSEs = epochMSEsArr, ferrors = firstDSErrArr, ftwtrain = firstDSErrsWhileTrain)
    
    return retVal