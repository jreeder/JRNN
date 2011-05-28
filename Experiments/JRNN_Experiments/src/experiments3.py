#!/usr/bin/env python
# Author:  jreeder --<>
# Purpose: Holds experiments defs for jrnn_exprun 
# Created: 5/27/2011


expFold = "Exp 1"


experiments = [\
    {'dsname':'band','numTasks':4,'numRuns':30, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':60, 'numVal':0, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':0, 'netType':'CC'}\
    ]