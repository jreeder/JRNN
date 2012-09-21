#This is a test file for PyJRNN

import os

os.chdir("D:\\Users\\John Reeder\\Code\\JRNN\\Builds\\VS2010\\JRNN\\Debug")
print os.path.abspath(os.curdir)

import numpy

from PyJRNN import networks as net
cc = net.CCNetwork.Create()
cc.Build(2,1)
import pyublas

a = numpy.array((0.5,0.5))
cc.Activate(a)


from PyJRNN import utility as utils
from PyJRNN.types import strings
from PyJRNN.types import matDouble

cds = utils.CSMTLDataset()
cds.isConceptData = False
inVec = numpy.random.random(5,)
outVec = numpy.random.random(2,)

inMatDouble = matDouble()
outMatDouble = matDouble()

for i in range(10):
        invalue = numpy.random.random(5,)
        outvalue = numpy.random.random(2,)
        inMatDouble.append(invalue)
        outMatDouble.append(outvalue)

for val in inMatDouble:
    print val

cds.AddMatDoublesToTask(inMatDouble, outMatDouble, "Task1")

cds.GetSize(utils.DSDatatype.ALL)

view = strings()
view.append("Task1")
cds.View = view

cds.DistData(2,2,2)
inputs = cds.GetInputs(utils.DSDatatype.TRAIN)

print len(inputs)

raw_input("Press any key")