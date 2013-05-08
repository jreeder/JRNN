# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

%pylab
import PyJRNN
import pyublas
import scipy.io
import ObsUtility

# <codecell>

ds = scipy.io.loadmat("Notebook Data/datasets/bandcross.mat")

# <codecell>

cds = PyJRNN.utility.CSMTLDataset()

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
    cds.view = tmpsview
    cds.distdata(200,200,200)
    ds1 = cds.SpawnDS()
    del tmpsview[:]
    tmpsview.append(secondtask)
    cds.view = tmpsview
    cds.RedistData()
    ds2 = cds.SpawnDS()
    del tmpsview[:]
    tmpsview.append(thirdtask)
    cds.view = tmpsview
    cds.RedistData()
    ds3 = cds.SpawnDS()
    
    ccnet1 = PyJRNN.networks.CCNetwork.Create()
    ccnet2 = PyJRNN.networks.CCNetwork.Create()
    kbccnet1 = PyJRNN.networks.KBCCNetwork.Create()
    
    cct1 = PyJRNN.trainers.CCTrainer(ccnet1, ds1, 8)
    cct2 = PyJRNN.trainers.CCTrainer(ccnet2, ds2, 8)
    
    kbcct1 = PyJRNN.trainers.KBCCTrainer(kbccnet1, ds3, 4)
    
    kbcct1.AddSubNet(ccnet1)
    kbcct1.AddSubNet(ccnet2)
    kbcct1.numCopies = 3
    
    cct1.TrainToValConv()
    cct2.TrainToValConv()
    
    kbcct1.TrainToValConv()

# <codecell>


