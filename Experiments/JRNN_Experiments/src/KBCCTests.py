# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

#%pylab
debug = True
import PyJRNN_d as PyJRNN
import pyublas
import scipy.io
import ObsUtility
import numpy
import os
import re

# <codecell>

ds = scipy.io.loadmat("Notebook Data/datasets/bandcross.mat")

# <codecell>

cds = PyJRNN.utility.CSMTLDataset()
cds.isConceptData = False

# <codecell>

inputs = numpy.ascontiguousarray(ds["inputs"])

# <codecell>

outNames = ['outBox1', 'outBox2', 'outBox3', 'outBox12', 'outBox13', 'outBox123']

# <codecell>

for outName in outNames:
    cds.AddMatDoublesToTask(ObsUtility.matDoubleFromArray(inputs), ObsUtility.matDoubleFromArray(numpy.ascontiguousarray(ds[outName])), outName)

# <codecell>

cds.GetTaskNames()

# <codecell>

for name in cds.GetTaskNames():
    print name

# <codecell>

def TestKBCC(cds, firsttask, secondtask, thirdtask):
    tmpsview = PyJRNN.types.strings()
    tmpsview.append(firsttask)
    cds.View = tmpsview
    cds.DistData(200,200,200)
    ds1 = cds.SpawnDS()
    del tmpsview[:]
    tmpsview.append(secondtask)
    cds.View = tmpsview
    cds.RedistData()
    ds2 = cds.SpawnDS()
    del tmpsview[:]
    tmpsview.append(thirdtask)
    cds.View = tmpsview
    cds.RedistData()
    ds3 = cds.SpawnDS()
    
    #ccnet1 = PyJRNN.networks.CCNetwork.Create()
    #ccnet2 = PyJRNN.networks.CCNetwork.Create()
    kbccnet1 = PyJRNN.networks.KBCCNetwork.Create()
    
    #ccnet1.Build(ds1.numInputs, ds1.numOutputs, False, True, False)
    #ccnet2.Build(ds2.numInputs, ds2.numOutputs, False, True, False)
    #cct1 = PyJRNN.trainers.CCTrainer(ccnet1, ds1, 8)
    #cct2 = PyJRNN.trainers.CCTrainer(ccnet2, ds2, 8)
    
    kbccnet1.Build(ds3.numInputs, ds3.numOutputs, False, True, False)
    kbcct1 = PyJRNN.trainers.KBCCTrainer(kbccnet1, ds3, 4)

    archiver = PyJRNN.utility.JSONArchiver()
    
    ccnet1 = archiver.LoadFromFile("ccnet1.net")
    ccnet2 = archiver.LoadFromFile("ccnet2.net")
    
    kbcct1.AddSubNet(ccnet1)
    kbcct1.AddSubNet(ccnet2)
    kbcct1.numCopies = 1
    
    #cct1.TrainToValConv(2000)
    #cct2.TrainToValConv(2000)
    
    kbcct1.TrainToValConv(2000)

    res = kbcct1.TestWiClass(PyJRNN.utility.TEST)
    print ObsUtility.ConvHashedDM(res)
    print "Done"
# <codecell>

def LoadBandCross(shift = False, view=[]):
    ds = scipy.io.loadmat("Notebook Data/datasets/bandcross.mat")
    cds = PyJRNN.utility.CSMTLDataset()
    cds.isConceptData = False
    inputs = numpy.ascontiguousarray(ds["inputs"])
    outNames = None
    if shift:
        inputs = inputs - 0.5
    if len(view) == 0:
        outNames = ['outBox1', 'outBox2', 'outBox3', 'outBox12', 'outBox13', 'outBox123']
    else:
        outNames = view
        
    for outName in outNames:
        outputs = numpy.ascontiguousarray(ds[outName])
        if shift:
            outputs = outputs - 0.5
        cds.AddMatDoublesToTask(ObsUtility.matDoubleFromArray(inputs), ObsUtility.matDoubleFromArray(numpy.ascontiguousarray(ds[outName])), outName)
    
    inView = PyJRNN.types.strings()
    for name in outNames:
        inView.append(name)
    
    cds.View = inView
    
    cds.DistData(200,200,200)
    
    return cds

def sortkey(string):
    errRe = re.compile("-(\d+\.\d+)-")
    return float(errRe.findall(string)[0])

def TestWiClass(net, dataset, dstype):
    inputs = dataset.GetInputs(dstype)
    outputs = dataset.GetOutputs(dstype)
    numInCorrect = 0
    totalItems = len(inputs)
    numTasks = len(outputs[0])
    taskErrs = defaultdict(int)
    taskErrorRate = {}
    for (inputIt, outputIt) in zip(inputs, outputs):
        net.Activate(inputIt)
        netout = net.GetOutputs()
        error = outputIt - netout
        for i in range(numTasks):
            name = "task-{0}".format(i)
            err = error[i]
            if abs(err) > 0.5:
                taskErrs[name] += 1
    
    for i in range(numTasks):
        name = "task-{0}".format(i)
        taskErrorRate[name] = taskErrs[name] / (totalItems * 1.0)
    return taskErrorRate

def SingleDataTest(dsname, archive=True, count = 0, useVal = True, shift = True):
    cds = LoadBandCross(shift)
    tmpsview = PyJRNN.types.strings()
    tmpsview.append(dsname)
    cds.DistSubview(tmpsview)
    ds = cds.SpawnDS()
    cc = PyJRNN.networks.CCNetwork.Create()
    useSDCC = True
    useVaryAct = True
    outNodeType = PyJRNN.utility.ActType.ASIGMOID
    if shift:
        outNodeType = PyJRNN.utility.ActType.SIGMOID
    cc.Build(ds.numInputs, ds.numOutputs, False, useSDCC, useVaryAct, outNodeType)
    cct = PyJRNN.trainers.CCTrainer(cc, ds, 8)
    cct.parms.errorMeasure = PyJRNN.trainers.BITS
    cct.TrainToConvergence(3000, useVal)
    result = ObsUtility.ConvHashedDM(cct.TestWiClass(PyJRNN.utility.TEST))
    epochs = cct.Epochs
    numHid = cct.NumHidLayers
    mseRec = cct.GetMSERec()
    retVal = (result, epochs, numHid, [x for x in mseRec])
    if archive:
        archiver = PyJRNN.utility.JSONArchiver()
        err = result['task-0']
        archiver.SaveToFile(cc, "{0}-{1}-uSD{2}-uVA{3}-uVal{5}-{4}.net".format(dsname, err, useSDCC, useVaryAct, count, useVal))
    return retVal

def SameDataTest(dsname):
    cds = LoadBandCross()
    tmpsview = PyJRNN.types.strings()
    tmpsview.append(dsname)
    cds.DistSubview(tmpsview)
    ds = cds.SpawnDS()
    kbcc = PyJRNN.networks.KBCCNetwork.Create()
    kbcc.Build(ds.numInputs, ds.numOutputs, False, True, True)
    kbcct = PyJRNN.trainers.KBCCTrainer(kbcc, ds, 8)
    kbcct.parms.errorMeasure = PyJRNN.trainers.BITS
    kbcct.useHoldBestCand = True
    filename = [x for x in os.listdir(networkpath) if dsname in x][0]
    archiver = PyJRNN.utility.JSONArchiver()
    ccnet = archiver.LoadFromFile(os.path.join(networkpath, filename))
    kbcct.AddSubNet(ccnet)
    kbcct.numCopies = 2
    kbcct.TrainToValConv(200)
    result = ObsUtility.ConvHashedDM(kbcct.TestWiClass(PyJRNN.utility.TEST))
    epochs = kbcct.Epochs
    numHid = kbcct.NumHidLayers
    mseRec = kbcct.GetMSERec()
    archiver.SaveToFile(kbcc, os.path.join(networkpath, "kbcc-{0}.net".format(dsname)))
    retVal = (result, epochs, numHid, [x for x in mseRec])
    return retVal

def MergeNetTest(dsname1, dsname2, dsname12, count = 0, useVal = True, shift=False, useRev=True, numRev=2, bufferSize=200):
    cds = LoadBandCross(shift)
    tmpsview = PyJRNN.types.strings()
    tmpsview.append(dsname12)
    cds.DistSubview(tmpsview)
    ds = cds.SpawnDS()
    del tmpsview[:]
    tmpsview.append(dsname1)
    tmpsview.append(dsname2)
    cds.DistSubview(tmpsview)
    ds2 = cds.SpawnDS()
    
    numContext = cds.ViewSize
    filename1 = ""
    filename2 = ""
    varString = "uVal{0}-uSh{1}".format(useVal, shift)
    netType = 'revcc' if useRev else 'kbcc'
    filename1 = sorted([x for x in os.listdir(networkpath) if dsname1+'-' in x and varString in x and netType in x], key=sortkey)[0]
    filename2 = sorted([x for x in os.listdir(networkpath) if dsname2+'-' in x and varString in x and netType in x], key=sortkey)[0]
    archiver = PyJRNN.utility.JSONArchiver()
    print "Loading Existing Nets"
    net1 = archiver.LoadFromFile(os.path.join(networkpath, filename1))
    net2 = archiver.LoadFromFile(os.path.join(networkpath, filename2))
    
    revGenT = None
    if useRev:
        revGenT = PyJRNN.trainers.RevCCTrainer(net1.numIn, net1.numOut, 8)
    else:
        revGenT = PyJRNN.trainers.DualKBCCTrainer(net1.numIn, net1.numOut, 8)
        
    print "Generating Pseudo Data"
    revGenT.AddPrevTrainedNets(net1, net1.Clone())
    revGenT.revparams.numRev = numRev
    revGenT.revparams.bufferSize = bufferSize
    revGenT.revparams.numContexts = numContext
    revGenT.revparams.cleanReverb = True
    revGenT.SetDataSet(cds)
    del tmpsview[:]
    tmpsview.append(dsname1)
    newDS = revGenT.ReverbMainNet(100, 100, tmpsview)
    
    del tmpsview[:]
    tmpsview.append(dsname2)
    revGenT.AddPrevTrainedNets(net2, net2.Clone())
    secDS = revGenT.ReverbMainNet(100, 100, tmpsview)
    
    
    newDS.MergeSubsets(secDS, True)
    
    kbcct = None
    
    if useRev:
        kbcct = PyJRNN.trainers.RevKBCCTrainer(newDS.numInputs, newDS.numOutputs, 4) #use Varied means this gets trippled
    else:
        kbcct = PyJRNN.trainers.DualKBCCTrainer(newDS.numInputs, newDS.numOutputs, 4)
        
    print "Creating Trainer"
    kbcct.SetSDCCandVaryActFunc(True, True)
    kbcct.revparams.numRev = numRev if useRev else 0
    kbcct.revparams.bufferSize = 0 # Only going to be training task once. 
    
    subnet1 = net1 if not useRev else net1.CloneToCC()
    subnet2 = net2 if not useRev else net2.CloneToCC()
    
    kbcct.AddSubNet(subnet1)
    kbcct.AddSubNet(subnet2)
    kbcct.numCopies = 0 # only add direct connections
    
    print "Training Pseudo Set"
    kbcct.TrainTask(newDS, 3000, useVal)
    
    print "Getting Results"
    resultDict = {}
    resultDict['FinTask'] = dsname12
    resultDict['SubTasks'] = (filename1, filename2)
    resultDict['epochs'] = kbcct.net1vals.epochs
    resultDict['hiddenLayers'] = kbcct.net1vals.numHidLayers
    resultDict['MSERec'] = [x for x in kbcct.net1vals.MSERec]
    resultDict['testMSEComb'] = kbcct.TestOnData(ds, PyJRNN.utility.DSDatatype.TEST)
    resultDict['errorRateComb'] = TestWiClass(kbcct.net1, ds, PyJRNN.utility.DSDatatype.TEST)
    resultDict['testMSESep'] = kbcct.TestOnData(ds2, PyJRNN.utility.DSDatatype.TEST)
    resultDict['errorRateSep'] = TestWiClass(kbcct.net1, ds2, PyJRNN.utility.DSDatatype.TEST)
    
    err = resultDict['errorRate']
    newNetType = 'revkbcc' if useRev else 'kbcc'
    archiver.SaveToFile(kbcct.net1, os.path.join(networkpath, "merged-{dsname12}-{err}-uSDTrue-uVATrue-uVal{useVal}-uSh{shift}-{count}.{newNetType}".format(**locals())))
    
    return resultDict

import wingdbstub

#TestKBCC(cds, "outBox1", "outBox2", "outBox12")

networkpath = 'Notebook Data/networks/'
dsname = 'outBox1'
dsname1 = 'outBox1'
dsname2 = 'outBox2'
dsname12 = 'outBox12'

useVal = True
shift = False
useRev=True
varString = "uVal{0}-uSh{1}".format(useVal, shift)
netType = 'revcc' if useRev else 'kbcc'

results = MergeNetTest(dsname1, dsname2, dsname12, useVal = True, shift=False, useRev=False, numRev=2, bufferSize=200)

#archiver = PyJRNN.utility.JSONArchiver()
#kbccnet = archiver.LoadFromFile(os.path.join(networkpath, 'kbcc-outBox1.net'))

#SingleDataTest(dsname)
#SameDataTest(dsname)