#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<>
# Purpose: Hold cirinsq experiments
# Created: 5/31/2011


cirinsqnorm = [\
    #BP Tests
    #STL Full Results
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/NormSize', 'outfile':'bpstl1results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task2', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/NormSize', 'outfile':'bpstl2results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task3', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/NormSize', 'outfile':'bpstl3results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task4', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/NormSize', 'outfile':'bpstl4results.txt'},\
    #MTL Full Results
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'BP', 'expFold':'CirInSq/NormSize', 'outfile':'bpmtl1results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1,task2,task7,task8', 'primTask':2,\
     'netType':'BP', 'expFold':'CirInSq/NormSize', 'outfile':'bpmtl2results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1,task2,task7,task8', 'primTask':3,\
     'netType':'BP', 'expFold':'CirInSq/NormSize', 'outfile':'bpmtl3results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1,task2,task7,task8', 'primTask':4,\
     'netType':'BP', 'expFold':'CirInSq/NormSize', 'outfile':'bpmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'BP', 'expFold':'CirInSq/NormSize', 'outfile':'bpmtlresults.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/NormSize', 'outfile':'bpstlresults.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'BP', 'expFold':'CirInSq/NormSize', 'outfile':'bpmtlurresults.txt'},\
    #CC Results
    #STL Full Results
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/NormSize', 'outfile':'ccstl1results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task2', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/NormSize', 'outfile':'ccstl2results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task3', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/NormSize', 'outfile':'ccstl3results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task4', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/NormSize', 'outfile':'ccstl4results.txt'},\
    #MTL Full Results
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'CC', 'expFold':'CirInSq/NormSize', 'outfile':'ccmtl1results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1,task2,task7,task8', 'primTask':2,\
     'netType':'CC', 'expFold':'CirInSq/NormSize', 'outfile':'ccmtl2results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1,task2,task7,task8', 'primTask':3,\
     'netType':'CC', 'expFold':'CirInSq/NormSize', 'outfile':'ccmtl3results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1,task2,task7,task8', 'primTask':4,\
     'netType':'CC', 'expFold':'CirInSq/NormSize', 'outfile':'ccmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'CC', 'expFold':'CirInSq/NormSize', 'outfile':'ccmtlresults.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/NormSize', 'outfile':'ccstlresults.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':True, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'CC', 'expFold':'CirInSq/NormSize', 'outfile':'ccmtlurresults.txt'}\
    ]    

cirinsqnormnv = [\
    #BP Tests
    #STL Full Results
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/NormSizeNV', 'outfile':'bpstl1results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/NormSizeNV', 'outfile':'bpstl2results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/NormSizeNV', 'outfile':'bpstl3results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/NormSizeNV', 'outfile':'bpstl4results.txt'},\
    #MTL Full Results
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'BP', 'expFold':'CirInSq/NormSizeNV', 'outfile':'bpmtl1results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':2,\
     'netType':'BP', 'expFold':'CirInSq/NormSizeNV', 'outfile':'bpmtl2results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':3,\
     'netType':'BP', 'expFold':'CirInSq/NormSizeNV', 'outfile':'bpmtl3results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':4,\
     'netType':'BP', 'expFold':'CirInSq/NormSizeNV', 'outfile':'bpmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'BP', 'expFold':'CirInSq/NormSizeNV', 'outfile':'bpmtlresults.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/NormSizeNV', 'outfile':'bpstlresults.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'BP', 'expFold':'CirInSq/NormSizeNV', 'outfile':'bpmtlurresults.txt'},\
    #CC Results
    #STL Full Results
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/NormSizeNV', 'outfile':'ccstl1results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/NormSizeNV', 'outfile':'ccstl2results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/NormSizeNV', 'outfile':'ccstl3results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/NormSizeNV', 'outfile':'ccstl4results.txt'},\
    #MTL Full Results
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'CC', 'expFold':'CirInSq/NormSizeNV', 'outfile':'ccmtl1results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':2,\
     'netType':'CC', 'expFold':'CirInSq/NormSizeNV', 'outfile':'ccmtl2results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':3,\
     'netType':'CC', 'expFold':'CirInSq/NormSizeNV', 'outfile':'ccmtl3results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':4,\
     'netType':'CC', 'expFold':'CirInSq/NormSizeNV', 'outfile':'ccmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'CC', 'expFold':'CirInSq/NormSizeNV', 'outfile':'ccmtlresults.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/NormSizeNV', 'outfile':'ccstlresults.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':6,\
     'useValidation':False, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'CC', 'expFold':'CirInSq/NormSizeNV', 'outfile':'ccmtlurresults.txt'}\
    ]  

cirinsqsmallnv = [\
    #BP Tests
    #STL Full Results
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'bpstl1results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'bpstl2results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'bpstl3results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'bpstl4results.txt'},\
    #MTL Full Results
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'BP', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'bpmtl1results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':2,\
     'netType':'BP', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'bpmtl2results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':3,\
     'netType':'BP', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'bpmtl3results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':4,\
     'netType':'BP', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'bpmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'BP', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'bpmtlresults.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'bpstlresults.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'BP', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'bpmtlurresults.txt'},\
    #CC Results
    #STL Full Results
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'ccstl1results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'ccstl2results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'ccstl3results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'ccstl4results.txt'},\
    #MTL Full Results
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'CC', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'ccmtl1results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':2,\
     'netType':'CC', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'ccmtl2results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':3,\
     'netType':'CC', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'ccmtl3results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':4,\
     'netType':'CC', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'ccmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'CC', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'ccmtlresults.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'ccstlresults.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':2,\
     'useValidation':False, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'CC', 'expFold':'CirInSq/SmallSizeNV', 'outfile':'ccmtlurresults.txt'}\
    ]    


cirinsqlargenv = [\
    #BP Tests
    #STL Full Results
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'bpstl1results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'bpstl2results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'bpstl3results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'bpstl4results.txt'},\
    #MTL Full Results
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'BP', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'bpmtl1results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':2,\
     'netType':'BP', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'bpmtl2results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':3,\
     'netType':'BP', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'bpmtl3results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':4,\
     'netType':'BP', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'bpmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'BP', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'bpmtlresults.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'BP', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'bpstlresults.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'BP', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'bpmtlurresults.txt'},\
    #CC Results
    #STL Full Results
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'ccstl1results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task2', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'ccstl2results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task3', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'ccstl3results.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task4', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'ccstl4results.txt'},\
    #MTL Full Results
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'CC', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'ccmtl1results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':2,\
     'netType':'CC', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'ccmtl2results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':3,\
     'netType':'CC', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'ccmtl3results.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':4,\
     'netType':'CC', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'ccmtl4results.txt'},\
    #Impoverished Tests
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1,task2,task7,task8', 'primTask':1,\
     'netType':'CC', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'ccmtlresults.txt'},\
    {'dsname':'CirInSq','numTasks':1,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':0,'numTrain':20, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1', 'primTask':0,\
     'netType':'CC', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'ccstlresults.txt'},\
    {'dsname':'CirInSq','numTasks':4,'numRuns':60, 'numInputs':2, 'numOutputs':1,\
     'impNumTrain':20,'numTrain':100, 'numVal':200, 'numTest':200, 'numHidPerTask':12,\
     'useValidation':False, 'viewString':'task1,task2,task5,task6', 'primTask':1,\
     'netType':'CC', 'expFold':'CirInSq/LargeSizeNV', 'outfile':'ccmtlurresults.txt'}\
    ]