#!/usr/bin/env python
# Author:  jreeder --<>
# Purpose: Holds experiments defs for jrnn_exprun 
# Created: 5/27/2011





experiments = [\
    {'dsname':'band','numTasks':1,'numRuns':30, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':100, 'numTest':200, 'numHidPerTask':10,\
     'useValidation':True, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'Round 2', 'outfile':''},\
    {'dsname':'band','numTasks':4,'numRuns':30, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':60, 'numVal':0, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task4', 'primTask':0,\
     'netType':'CC', 'expFold':'Round 2', 'outfile':'testout.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':30, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':60, 'numVal':0, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task4', 'primTask':0,\
     'netType':'CC', 'expFold':'Round 3', 'outfile':'testout.txt'}\
    ]