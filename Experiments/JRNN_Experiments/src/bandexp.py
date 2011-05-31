#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<>
# Purpose: Holds band experiments
# Created: 5/31/2011

bandnorm = [\
    #BP Tests
    #STL Full Results
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'band/NormSize', 'outfile':'bpstl1results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task2', 'primTask':0,\
     'netType':'BP', 'expFold':'band/NormSize', 'outfile':'bpstl2results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task3', 'primTask':0,\
     'netType':'BP', 'expFold':'band/NormSize', 'outfile':'bpstl3results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task4', 'primTask':0,\
     'netType':'BP', 'expFold':'band/NormSize', 'outfile':'bpstl4results.txt'},\
    #MTL Full Results
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'BP', 'expFold':'band/NormSize', 'outfile':'bpmtl1results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task4', 'primTask':2,\
     'netType':'BP', 'expFold':'band/NormSize', 'outfile':'bpmtl2results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task4', 'primTask':3,\
     'netType':'BP', 'expFold':'band/NormSize', 'outfile':'bpmtl3results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task4', 'primTask':4,\
     'netType':'BP', 'expFold':'band/NormSize', 'outfile':'bpmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'BP', 'expFold':'band/NormSize', 'outfile':'bpmtlresults.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'band/NormSize', 'outfile':'bpstlresults.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'BP', 'expFold':'band/NormSize', 'outfile':'bpmtlurresults.txt'},\
    #CC Results
    #STL Full Results
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'band/NormSize', 'outfile':'ccstl1results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task2', 'primTask':0,\
     'netType':'CC', 'expFold':'band/NormSize', 'outfile':'ccstl2results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task3', 'primTask':0,\
     'netType':'CC', 'expFold':'band/NormSize', 'outfile':'ccstl3results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task4', 'primTask':0,\
     'netType':'CC', 'expFold':'band/NormSize', 'outfile':'ccstl4results.txt'},\
    #MTL Full Results
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'CC', 'expFold':'band/NormSize', 'outfile':'ccmtl1results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task4', 'primTask':2,\
     'netType':'CC', 'expFold':'band/NormSize', 'outfile':'ccmtl2results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task4', 'primTask':3,\
     'netType':'CC', 'expFold':'band/NormSize', 'outfile':'ccmtl3results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task4', 'primTask':4,\
     'netType':'CC', 'expFold':'band/NormSize', 'outfile':'ccmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'CC', 'expFold':'band/NormSize', 'outfile':'ccmtlresults.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'band/NormSize', 'outfile':'ccstlresults.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'CC', 'expFold':'band/NormSize', 'outfile':'ccmtlurresults.txt'}\
    ]

bandnormnv = [\
    #BP Tests
    #STL Full Results
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'band/NormSizeNV', 'outfile':'bpstl1results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'BP', 'expFold':'band/NormSizeNV', 'outfile':'bpstl2results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'BP', 'expFold':'band/NormSizeNV', 'outfile':'bpstl3results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'BP', 'expFold':'band/NormSizeNV', 'outfile':'bpstl4results.txt'},\
    #MTL Full Results
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'BP', 'expFold':'band/NormSizeNV', 'outfile':'bpmtl1results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':2,\
     'netType':'BP', 'expFold':'band/NormSizeNV', 'outfile':'bpmtl2results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':3,\
     'netType':'BP', 'expFold':'band/NormSizeNV', 'outfile':'bpmtl3results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':4,\
     'netType':'BP', 'expFold':'band/NormSizeNV', 'outfile':'bpmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'BP', 'expFold':'band/NormSizeNV', 'outfile':'bpmtlresults.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'band/NormSizeNV', 'outfile':'bpstlresults.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'BP', 'expFold':'band/NormSizeNV', 'outfile':'bpmtlurresults.txt'},\
    #CC Results
    #STL Full Results
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'band/NormSizeNV', 'outfile':'ccstl1results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'CC', 'expFold':'band/NormSizeNV', 'outfile':'ccstl2results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'CC', 'expFold':'band/NormSizeNV', 'outfile':'ccstl3results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'CC', 'expFold':'band/NormSizeNV', 'outfile':'ccstl4results.txt'},\
    #MTL Full Results
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'CC', 'expFold':'band/NormSizeNV', 'outfile':'ccmtl1results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':2,\
     'netType':'CC', 'expFold':'band/NormSizeNV', 'outfile':'ccmtl2results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':3,\
     'netType':'CC', 'expFold':'band/NormSizeNV', 'outfile':'ccmtl3results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':4,\
     'netType':'CC', 'expFold':'band/NormSizeNV', 'outfile':'ccmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'CC', 'expFold':'band/NormSizeNV', 'outfile':'ccmtlresults.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'band/NormSizeNV', 'outfile':'ccstlresults.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'CC', 'expFold':'band/NormSizeNV', 'outfile':'ccmtlurresults.txt'}\
    ]

bandsmallnv = [\
    #BP Tests
    #STL Full Results
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'band/SmallSizeNV', 'outfile':'bpstl1results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'BP', 'expFold':'band/SmallSizeNV', 'outfile':'bpstl2results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'BP', 'expFold':'band/SmallSizeNV', 'outfile':'bpstl3results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'BP', 'expFold':'band/SmallSizeNV', 'outfile':'bpstl4results.txt'},\
    #MTL Full Results
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'BP', 'expFold':'band/SmallSizeNV', 'outfile':'bpmtl1results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':2,\
     'netType':'BP', 'expFold':'band/SmallSizeNV', 'outfile':'bpmtl2results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':3,\
     'netType':'BP', 'expFold':'band/SmallSizeNV', 'outfile':'bpmtl3results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':4,\
     'netType':'BP', 'expFold':'band/SmallSizeNV', 'outfile':'bpmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'BP', 'expFold':'band/SmallSizeNV', 'outfile':'bpmtlresults.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'band/SmallSizeNV', 'outfile':'bpstlresults.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'BP', 'expFold':'band/SmallSizeNV', 'outfile':'bpmtlurresults.txt'},\
    #CC Results
    #STL Full Results
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'band/SmallSizeNV', 'outfile':'ccstl1results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'CC', 'expFold':'band/SmallSizeNV', 'outfile':'ccstl2results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'CC', 'expFold':'band/SmallSizeNV', 'outfile':'ccstl3results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'CC', 'expFold':'band/SmallSizeNV', 'outfile':'ccstl4results.txt'},\
    #MTL Full Results
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'CC', 'expFold':'band/SmallSizeNV', 'outfile':'ccmtl1results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':2,\
     'netType':'CC', 'expFold':'band/SmallSizeNV', 'outfile':'ccmtl2results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':3,\
     'netType':'CC', 'expFold':'band/SmallSizeNV', 'outfile':'ccmtl3results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':4,\
     'netType':'CC', 'expFold':'band/SmallSizeNV', 'outfile':'ccmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'CC', 'expFold':'band/SmallSizeNV', 'outfile':'ccmtlresults.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'band/SmallSizeNV', 'outfile':'ccstlresults.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'CC', 'expFold':'band/SmallSizeNV', 'outfile':'ccmtlurresults.txt'}\
    ]    



bandlargenv = [\
    #BP Tests
    #STL Full Results
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'band/LargeSizeNV', 'outfile':'bpstl1results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'BP', 'expFold':'band/LargeSizeNV', 'outfile':'bpstl2results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'BP', 'expFold':'band/LargeSizeNV', 'outfile':'bpstl3results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'BP', 'expFold':'band/LargeSizeNV', 'outfile':'bpstl4results.txt'},\
    #MTL Full Results
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'BP', 'expFold':'band/LargeSizeNV', 'outfile':'bpmtl1results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':2,\
     'netType':'BP', 'expFold':'band/LargeSizeNV', 'outfile':'bpmtl2results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':3,\
     'netType':'BP', 'expFold':'band/LargeSizeNV', 'outfile':'bpmtl3results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':4,\
     'netType':'BP', 'expFold':'band/LargeSizeNV', 'outfile':'bpmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'BP', 'expFold':'band/LargeSizeNV', 'outfile':'bpmtlresults.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'band/LargeSizeNV', 'outfile':'bpstlresults.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'BP', 'expFold':'band/LargeSizeNV', 'outfile':'bpmtlurresults.txt'},\
    #CC Results
    #STL Full Results
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'band/LargeSizeNV', 'outfile':'ccstl1results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'CC', 'expFold':'band/LargeSizeNV', 'outfile':'ccstl2results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'CC', 'expFold':'band/LargeSizeNV', 'outfile':'ccstl3results.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'CC', 'expFold':'band/LargeSizeNV', 'outfile':'ccstl4results.txt'},\
    #MTL Full Results
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'CC', 'expFold':'band/LargeSizeNV', 'outfile':'ccmtl1results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':2,\
     'netType':'CC', 'expFold':'band/LargeSizeNV', 'outfile':'ccmtl2results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':3,\
     'netType':'CC', 'expFold':'band/LargeSizeNV', 'outfile':'ccmtl3results.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':4,\
     'netType':'CC', 'expFold':'band/LargeSizeNV', 'outfile':'ccmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task4', 'primTask':1,\
     'netType':'CC', 'expFold':'band/LargeSizeNV', 'outfile':'ccmtlresults.txt'},\
    {'dsname':'band','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'band/LargeSizeNV', 'outfile':'ccstlresults.txt'},\
    {'dsname':'band','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'CC', 'expFold':'band/LargeSizeNV', 'outfile':'ccmtlurresults.txt'}\
    ]    