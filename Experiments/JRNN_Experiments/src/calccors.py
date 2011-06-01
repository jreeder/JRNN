#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<>
# Purpose: calculate correlation of datasets tasks
# Created: 5/31/2011

import sys
import os
from scipy.stats import spearmanr as cor
import numpy as np
from config import *

tofile = True
dsnames = ['band', 'CirInSq', 'linear', 'derm', 'glass', 'heart', 'smallcovtype']
if __name__=='__main__':
   #dsname = sys.argv[1]
   #numTasks = sys.argv[2]
   filelist = [os.path.join(datapath, f) for f in os.listdir(datapath)] + [os.path.join(datapath2, f) for f in os.listdir(datapath2)]
   outfile = open("dscorrelations.txt", "w")
   for dsname in dsnames:
      
      dsfilelist = [f for f in filelist if dsname in f]      
      if tofile: print >>outfile, dsname
      else: print dsname
      
      data = {}
      
      for filename in dsfilelist:
         infile = open(filename, "r")
         data[filename] = []
         for line in infile:
            tmparray = line.split("\t")
            data[filename].append(int(tmparray[-1]))
         
            
      height = len(data[dsfilelist[0]])
      width = len(data)
   
      tmpmat = np.array([data[filename] for filename in dsfilelist])
      
      rho, pval = cor(tmpmat, axis=1)
      
      tasknames = [filename.split("-")[1][0:-4] for filename in dsfilelist]
      
      #header = "%10s" % "" + " ".join(["%10s" % task for task in tasknames])
      header = "{:10s}".format("") + " | ".join(["{:_^11s}".format(task) for task in tasknames]) + " |"
      
      if tofile: print >>outfile, header
      else: print header
      
      for i in range(width):
         toprint = "{:10s}".format(tasknames[i]) + " | ".join(["{:>11f}".format(num) for num in rho[i]]) + " |"
         if tofile: print >>outfile, toprint
         else: print toprint
      
      if tofile: print >>outfile, ""
      else: print ""
      #if tofile: print >>outfile, rho      
      #else: print rho
   
   outfile.close()