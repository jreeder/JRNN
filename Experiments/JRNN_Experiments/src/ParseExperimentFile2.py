# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__="jreeder"
__date__ ="$Aug 15, 2010 12:56:38 AM$"

class Bunch(object):
    def __init__(self, **kwds):
        self.__dict__.update(kwds)

def ParseFile(filename):
    if "bp" in filename:
        typeName = "BP"
    else:
        typeName = "CC"

    if "mtl" in filename:
        taskType = "MTL"
    else:
        taskType = "STL"

    if "ur" in filename:
        unrelated = True
    else:
        unrelated = False

    if not unrelated:
        thisName = typeName + "_" + taskType
    else:
        thisName = typeName + "_" + taskType + "_UR"

#    print thisName
    
#    subfilename = filename.split(typeName)
#    datasetName = subfilename[0][:-1]
#    expparams = subfilename[1][1:-4].split("-")
#
#    for item in expparams:
#        if "tr" in item:
#            numTrain = item[2:]
#        elif "uv" in item:
#            if "T" in item:
#                useVal = True
#            else:
#                useVal = False
#
#        elif "v" in item:
#            numVal = item[1:]
#
#        elif "t" in item:
#            numTest = item[1:]
#
#        elif "hid" in item:
#            numHid = item[3:]
#
#        else:
#            numRuns = item[1:]

    infile = open(filename, 'r')
    epochsArr = []
    timesArr = []
    errorsArr = []
    hidLayersArr = []
    epochMSEsArr = []
    resetsArr = []
    for line in infile:
        firstsplit = line.split("|")
        if len(firstsplit) < 2: continue
        runMetrics = firstsplit[0].split("\t")
        runMSEs = [float(x) for x in firstsplit[1].split("\t")[1:-1]]
        #runMSEs = firstsplit[1].split("\t")
        #print runMSEs
        epochsArr.append(int(runMetrics[0]))
        timesArr.append(float(runMetrics[1]))
        hidLayersArr.append(int(runMetrics[2]))
        resetsArr.append(int(runMetrics[3]))
        tmpDict = {}
        for item in runMetrics[4:]:
            itemsplit = item.split(":")
            if len(itemsplit) > 1:
                tmpDict[itemsplit[0]] = float(itemsplit[1])
        errorsArr.append(tmpDict)
        epochMSEsArr.append(runMSEs)

    infile.close()
   # print epochsArr
    retVal = Bunch(name=thisName, type=typeName, taskType=taskType, unrelated=unrelated, resets=resetsArr,\
        epochs=epochsArr, times=timesArr, errors=errorsArr, hidLayers=hidLayersArr,\
        epochMSEs = epochMSEsArr)

    return retVal
#    print "task: " + dataset
#    print "Train#: " + numTrain
#    print "Val#: " + numVal
#    print "Test#: " + numTest
#    print "Validation: %s" % useVal
#    print "Hidden#: " + numHid
#    print "Runs#: " + numRuns
