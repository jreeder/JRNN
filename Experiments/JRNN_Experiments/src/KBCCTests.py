# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

#%pylab
debug = False
import PyJRNN #_d as PyJRNN
import pyublas
import scipy.io
import ObsUtility
import numpy
import os

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

def LoadBandCross(shift = False):
    ds = scipy.io.loadmat("Notebook Data/datasets/bandcross.mat")
    cds = PyJRNN.utility.CSMTLDataset()
    cds.isConceptData = False
    inputs = numpy.ascontiguousarray(ds["inputs"])
    if shift:
        inputs = inputs - 0.5
    outNames = ['outBox1', 'outBox2', 'outBox3', 'outBox12', 'outBox13', 'outBox123']
    for outName in outNames:
        outputs = numpy.ascontiguousarray(ds[outName])
        if shift:
            outputs = outputs - 0.5
        cds.AddMatDoublesToTask(ObsUtility.matDoubleFromArray(inputs), ObsUtility.matDoubleFromArray(outputs), outName)
    
    cds.DistData(200,200,200)
    
    return cds

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

import wingdbstub

#TestKBCC(cds, "outBox1", "outBox2", "outBox12")

networkpath = 'Notebook Data/networks/'
dsname = 'outBox1'

#archiver = PyJRNN.utility.JSONArchiver()
#kbccnet = archiver.LoadFromFile(os.path.join(networkpath, 'kbcc-outBox1.net'))

SingleDataTest(dsname)
#SameDataTest(dsname)