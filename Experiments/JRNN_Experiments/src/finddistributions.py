#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<jreeder@gmail.com>
# Purpose: Meant to calculate the class distribution of the datasets
# Created: 11/15/2011

import os
from config import *

def ProcessFile(file):
    (filepath, filename) = os.path.split(file)
    (dataset,trash) = os.path.splitext(filename)
    outfilepath = genericout
    os.chdir(filepath)
    datasetname = dataset.split("-")[0]
    
    classcounts = {}
    classcounts[0] = 0
    classcounts[1] = 0
    
    with open(file, mode="r") as infile:
        for line in infile:
            values = [float(v) for v in line.split('\t')]
            cls = values[-1]
            classcounts[cls] += 1
            
    total = classcounts[0] + classcounts[1]
    
    perc = classcounts[1] / float(total)
    
    print datasetname + " " + str(perc)
    
if __name__=='__main__':
    
    filelist = [os.path.join(datapath, f) for f in os.listdir(datapath)] +\
        [os.path.join(datapath2, f) for f in os.listdir(datapath2)]
    
    for filename in filelist: 
        ProcessFile(filename)