#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<>
# Purpose: Hold linear experiments
# Created: 5/31/2011

linearnorm = [\
    #BP Tests
    #STL Full Results
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/NormSize', 'outfile':'bpstl1results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task2', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/NormSize', 'outfile':'bpstl2results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task3', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/NormSize', 'outfile':'bpstl3results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task4', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/NormSize', 'outfile':'bpstl4results.txt'},\
    #MTL Full Results
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'BP', 'expFold':'linear/NormSize', 'outfile':'bpmtl1results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task5', 'primTask':2,\
     'netType':'BP', 'expFold':'linear/NormSize', 'outfile':'bpmtl2results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task5', 'primTask':3,\
     'netType':'BP', 'expFold':'linear/NormSize', 'outfile':'bpmtl3results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task5', 'primTask':4,\
     'netType':'BP', 'expFold':'linear/NormSize', 'outfile':'bpmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'BP', 'expFold':'linear/NormSize', 'outfile':'bpmtlresults.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':10, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/NormSize', 'outfile':'bpstlresults.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task4,task6', 'primTask':1,\
     'netType':'BP', 'expFold':'linear/NormSize', 'outfile':'bpmtlurresults.txt'},\
    #CC Results
    #STL Full Results
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/NormSize', 'outfile':'ccstl1results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task2', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/NormSize', 'outfile':'ccstl2results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task3', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/NormSize', 'outfile':'ccstl3results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task4', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/NormSize', 'outfile':'ccstl4results.txt'},\
    #MTL Full Results
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'CC', 'expFold':'linear/NormSize', 'outfile':'ccmtl1results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task5', 'primTask':2,\
     'netType':'CC', 'expFold':'linear/NormSize', 'outfile':'ccmtl2results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task5', 'primTask':3,\
     'netType':'CC', 'expFold':'linear/NormSize', 'outfile':'ccmtl3results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task5', 'primTask':4,\
     'netType':'CC', 'expFold':'linear/NormSize', 'outfile':'ccmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'CC', 'expFold':'linear/NormSize', 'outfile':'ccmtlresults.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':10, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/NormSize', 'outfile':'ccstlresults.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':True, 'viewString':'task1,task2,task4,task6', 'primTask':1,\
     'netType':'CC', 'expFold':'linear/NormSize', 'outfile':'ccmtlurresults.txt'}\
    ]

linearnormnv = [\
    #BP Tests
    #STL Full Results
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/NormSizeNV', 'outfile':'bpstl1results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/NormSizeNV', 'outfile':'bpstl2results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/NormSizeNV', 'outfile':'bpstl3results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/NormSizeNV', 'outfile':'bpstl4results.txt'},\
    #MTL Full Results
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'BP', 'expFold':'linear/NormSizeNV', 'outfile':'bpmtl1results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':2,\
     'netType':'BP', 'expFold':'linear/NormSizeNV', 'outfile':'bpmtl2results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':3,\
     'netType':'BP', 'expFold':'linear/NormSizeNV', 'outfile':'bpmtl3results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':4,\
     'netType':'BP', 'expFold':'linear/NormSizeNV', 'outfile':'bpmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'BP', 'expFold':'linear/NormSizeNV', 'outfile':'bpmtlresults.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':10, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/NormSizeNV', 'outfile':'bpstlresults.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task4,task6', 'primTask':1,\
     'netType':'BP', 'expFold':'linear/NormSizeNV', 'outfile':'bpmtlurresults.txt'},\
    #CC Results
    #STL Full Results
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/NormSizeNV', 'outfile':'ccstl1results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/NormSizeNV', 'outfile':'ccstl2results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/NormSizeNV', 'outfile':'ccstl3results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/NormSizeNV', 'outfile':'ccstl4results.txt'},\
    #MTL Full Results
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'CC', 'expFold':'linear/NormSizeNV', 'outfile':'ccmtl1results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':2,\
     'netType':'CC', 'expFold':'linear/NormSizeNV', 'outfile':'ccmtl2results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':3,\
     'netType':'CC', 'expFold':'linear/NormSizeNV', 'outfile':'ccmtl3results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':4,\
     'netType':'CC', 'expFold':'linear/NormSizeNV', 'outfile':'ccmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'CC', 'expFold':'linear/NormSizeNV', 'outfile':'ccmtlresults.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':10, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/NormSizeNV', 'outfile':'ccstlresults.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':4,\
     'useValidation':False, 'viewString':'task1,task2,task4,task6', 'primTask':1,\
     'netType':'CC', 'expFold':'linear/NormSizeNV', 'outfile':'ccmtlurresults.txt'}\
    ]

linearsmallnv = [\
    #BP Tests
    #STL Full Results
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/SmallSizeNV', 'outfile':'bpstl1results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/SmallSizeNV', 'outfile':'bpstl2results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/SmallSizeNV', 'outfile':'bpstl3results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/SmallSizeNV', 'outfile':'bpstl4results.txt'},\
    #MTL Full Results
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'BP', 'expFold':'linear/SmallSizeNV', 'outfile':'bpmtl1results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':2,\
     'netType':'BP', 'expFold':'linear/SmallSizeNV', 'outfile':'bpmtl2results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':3,\
     'netType':'BP', 'expFold':'linear/SmallSizeNV', 'outfile':'bpmtl3results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':4,\
     'netType':'BP', 'expFold':'linear/SmallSizeNV', 'outfile':'bpmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'BP', 'expFold':'linear/SmallSizeNV', 'outfile':'bpmtlresults.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':10, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/SmallSizeNV', 'outfile':'bpstlresults.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task4,task6', 'primTask':1,\
     'netType':'BP', 'expFold':'linear/SmallSizeNV', 'outfile':'bpmtlurresults.txt'},\
    #CC Results
    #STL Full Results
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/SmallSizeNV', 'outfile':'ccstl1results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/SmallSizeNV', 'outfile':'ccstl2results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/SmallSizeNV', 'outfile':'ccstl3results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/SmallSizeNV', 'outfile':'ccstl4results.txt'},\
    #MTL Full Results
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'CC', 'expFold':'linear/SmallSizeNV', 'outfile':'ccmtl1results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':2,\
     'netType':'CC', 'expFold':'linear/SmallSizeNV', 'outfile':'ccmtl2results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':3,\
     'netType':'CC', 'expFold':'linear/SmallSizeNV', 'outfile':'ccmtl3results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':4,\
     'netType':'CC', 'expFold':'linear/SmallSizeNV', 'outfile':'ccmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'CC', 'expFold':'linear/SmallSizeNV', 'outfile':'ccmtlresults.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':10, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/SmallSizeNV', 'outfile':'ccstlresults.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':1,\
     'useValidation':False, 'viewString':'task1,task2,task4,task6', 'primTask':1,\
     'netType':'CC', 'expFold':'linear/SmallSizeNV', 'outfile':'ccmtlurresults.txt'}\
    ]    



linearlargenv = [\
    #BP Tests
    #STL Full Results
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/LargeSizeNV', 'outfile':'bpstl1results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/LargeSizeNV', 'outfile':'bpstl2results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/LargeSizeNV', 'outfile':'bpstl3results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/LargeSizeNV', 'outfile':'bpstl4results.txt'},\
    #MTL Full Results
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'BP', 'expFold':'linear/LargeSizeNV', 'outfile':'bpmtl1results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':2,\
     'netType':'BP', 'expFold':'linear/LargeSizeNV', 'outfile':'bpmtl2results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':3,\
     'netType':'BP', 'expFold':'linear/LargeSizeNV', 'outfile':'bpmtl3results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':4,\
     'netType':'BP', 'expFold':'linear/LargeSizeNV', 'outfile':'bpmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'BP', 'expFold':'linear/LargeSizeNV', 'outfile':'bpmtlresults.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':10, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'linear/LargeSizeNV', 'outfile':'bpstlresults.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task4,task6', 'primTask':1,\
     'netType':'BP', 'expFold':'linear/LargeSizeNV', 'outfile':'bpmtlurresults.txt'},\
    #CC Results
    #STL Full Results
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/LargeSizeNV', 'outfile':'ccstl1results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/LargeSizeNV', 'outfile':'ccstl2results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/LargeSizeNV', 'outfile':'ccstl3results.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/LargeSizeNV', 'outfile':'ccstl4results.txt'},\
    #MTL Full Results
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'CC', 'expFold':'linear/LargeSizeNV', 'outfile':'ccmtl1results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':2,\
     'netType':'CC', 'expFold':'linear/LargeSizeNV', 'outfile':'ccmtl2results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':3,\
     'netType':'CC', 'expFold':'linear/LargeSizeNV', 'outfile':'ccmtl3results.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':4,\
     'netType':'CC', 'expFold':'linear/LargeSizeNV', 'outfile':'ccmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task3,task5', 'primTask':1,\
     'netType':'CC', 'expFold':'linear/LargeSizeNV', 'outfile':'ccmtlresults.txt'},\
    {'dsname':'linear','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':10, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'linear/LargeSizeNV', 'outfile':'ccstlresults.txt'},\
    {'dsname':'linear','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':4,\
     'impNumTrain':10,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':8,\
     'useValidation':False, 'viewString':'task1,task2,task4,task6', 'primTask':1,\
     'netType':'CC', 'expFold':'linear/LargeSizeNV', 'outfile':'ccmtlurresults.txt'}\
    ]    