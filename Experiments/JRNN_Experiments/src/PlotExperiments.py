#! /usr/bin/python

# Used to parse the experiment files and plot them. 

__author__="jreeder"
__date__ ="$Jul 23, 2010 3:18:41 PM$"

import os
from config import *
import ParseExperimentFile as pe
import scipy as sci
import numpy as np
import matplotlib.pyplot as plt

if __name__ == "__main__":
    os.chdir(outpath + "BP Test 1/")
    values = pe.ParseFile("band-task3-BP-tr50-v100-t500-hid4-uvT-r60.txt")
    print values.type
    print values.epochs
