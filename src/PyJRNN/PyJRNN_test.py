#This is a test file for PyJRNN

import numpy

from PyJRNN import networks as net
cc = net.CCNetwork.Create()
cc.Build(2,1)
import pyublas

a = numpy.array((0.5,0.5))
cc.Activate(a)

raw_input("Press any key")