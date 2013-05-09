# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

#%pylab
import PyJRNN_d as PyJRNN
import pyublas
import scipy.io
import ObsUtility
import numpy as np

# <codecell>

ds = scipy.io.loadmat("Notebook Data/datasets/bandcross.mat")

# <codecell>

cds = PyJRNN.utility.CSMTLDataset()
cds.isConceptData = False

# <codecell>

inputs = np.ascontiguousarray(ds["inputs"])

# <codecell>

outNames = ['outBox1', 'outBox2', 'outBox3', 'outBox12', 'outBox13', 'outBox123']

# <codecell>

for outName in outNames:
    cds.AddMatDoublesToTask(ObsUtility.matDoubleFromArray(inputs), ObsUtility.matDoubleFromArray(np.ascontiguousarray(ds[outName])), outName)

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

    print "Done"
# <codecell>


import wingdbstub

TestKBCC(cds, "outBox1", "outBox2", "outBox12")