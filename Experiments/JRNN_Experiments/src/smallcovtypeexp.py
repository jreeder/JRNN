#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<>
# Purpose: smallcovtype exp 
# Created: 6/5/2011

def cau(source, upd):
    dictcopy = source.copy()
    dictcopy.update(upd)
    return dictcopy

standardvars = {'dsname':'smallcovtype','numRuns':60, 'numInputs':10,'numOutputs':1,\
                'numVal':100, 'numTest':200, 'numTrain':100, \
                'numHidPerTask':6,'useValidation':True, 'primTask':0, \
                'numTasks':1, 'viewString':'task1', 'expFold':'smallcovtype/NormSize', \
                'outfile':'results.txt', 'netType':'BP', 'impNumTrain':0}

impNumTrain = 25
normnvpath = 'smallcovtype/NormSizeNV'
largenvpath = 'smallcovtype/LargeSizeNV'
smallnvpath = 'smallcovtype/SmallSizeNV'
largenumhid = 15
smallnumhid = 3
relatedView = 'task1,task2,task3,task7'
urView = 'task1,task2,task4,task5'
primTask = 1


normimp = [cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':relatedView, 'primTask':primTask, 'outfile':'bpmtlresults.txt'}),\
           cau(standardvars, {'numTasks':1, 'numTrain':impNumTrain, 'viewString':'task1', 'outfile':'bpstlresults.txt'}),\
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':urView, 'primTask':primTask, 'outfile':'bpmtlurresults.txt'}),\
           #CC 
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':relatedView, 'primTask':primTask, 'outfile':'ccmtlresults.txt','netType':'CC'}),\
           cau(standardvars, {'numTasks':1, 'numTrain':impNumTrain, 'viewString':'task1', 'outfile':'ccstlresults.txt','netType':'CC'}),\
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':urView, 'primTask':primTask, 'outfile':'ccmtlurresults.txt','netType':'CC'}),\
           ]


standardvars.update({'useValidation':False, 'expFold':normnvpath})

normimpnv = [cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':relatedView, 'primTask':primTask, 'outfile':'bpmtlresults.txt'}),\
           cau(standardvars, {'numTasks':1, 'numTrain':impNumTrain, 'viewString':'task1', 'outfile':'bpstlresults.txt'}),\
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':urView, 'primTask':primTask, 'outfile':'bpmtlurresults.txt'}),\
           #CC 
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':relatedView, 'primTask':primTask, 'outfile':'ccmtlresults.txt','netType':'CC'}),\
           cau(standardvars, {'numTasks':1, 'numTrain':impNumTrain, 'viewString':'task1', 'outfile':'ccstlresults.txt','netType':'CC'}),\
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':urView, 'primTask':primTask, 'outfile':'ccmtlurresults.txt','netType':'CC'}),\
           ]

standardvars.update({'expFold':largenvpath, 'numHidPerTask':largenumhid})

largeimpnv = [cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':relatedView, 'primTask':primTask, 'outfile':'bpmtlresults.txt'}),\
           cau(standardvars, {'numTasks':1, 'numTrain':impNumTrain, 'viewString':'task1', 'outfile':'bpstlresults.txt'}),\
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':urView, 'primTask':primTask, 'outfile':'bpmtlurresults.txt'}),\
           #CC 
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':relatedView, 'primTask':primTask, 'outfile':'ccmtlresults.txt','netType':'CC'}),\
           cau(standardvars, {'numTasks':1, 'numTrain':impNumTrain, 'viewString':'task1', 'outfile':'ccstlresults.txt','netType':'CC'}),\
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':urView, 'primTask':primTask, 'outfile':'ccmtlurresults.txt','netType':'CC'}),\
           ]           


standardvars.update({'expFold':smallnvpath, 'numHidPerTask':smallnumhid})

smallimpnv = [cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':relatedView, 'primTask':primTask, 'outfile':'bpmtlresults.txt'}),\
           cau(standardvars, {'numTasks':1, 'numTrain':impNumTrain, 'viewString':'task1', 'outfile':'bpstlresults.txt'}),\
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':urView, 'primTask':primTask, 'outfile':'bpmtlurresults.txt'}),\
           #CC 
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':relatedView, 'primTask':primTask, 'outfile':'ccmtlresults.txt','netType':'CC'}),\
           cau(standardvars, {'numTasks':1, 'numTrain':impNumTrain, 'viewString':'task1', 'outfile':'ccstlresults.txt','netType':'CC'}),\
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':urView, 'primTask':primTask, 'outfile':'ccmtlurresults.txt','netType':'CC'}),\
           ]           

#Extra Tests
standardvars.update({'viewString':urView,'primTask':primTask,'numTasks':4, 'useValidation':True,\
                    'numHidPerTask':regnumhid, 'expFold':'smallcovtype/CCMTLTest'})

#Test of whether the focusing the correlation on the primary task worked or not. 
ccmtl = [\
    cau(standardvars, {'netType':'CC', 'outfile':'ccurresult.txt'}),\
    cau(standardvars, {'netType':'CCMTL', 'outfile':'ccmtlurresult.txt'})\
]

standardvars.update({'expFold':'smallcovtype/ETAMTLTest', 'useEtaMTL':True})

#Eta MTL tests. Need to compare these against other unrelated bp and cc tests.
etamtl = [\
    cau(standardvars, {'netType':'BP', 'outfile':'bpetamtlurresult.txt'}),\
    cau(standardvars, {'netType':'CCMTL', 'outfile':'ccetamtlscoreurresult.txt'}),\
    cau(standardvars, {'netType':'CCMTL', 'useCandSlope':True, 'outfile':'ccetamtlslopeurresult.txt'})\
]

standardvars.update({'viewString':relatedView, 'expFold':'smallcovtype/CSMTLTest', 'useCSMTLDS':True})

#Test of the csmtl paradymn
csmtl = [\
    cau(standardvars, {'netType':'BP', 'outfile':'bpcsmtlresult.txt'}),\
    cau(standardvars, {'netType':'CC', 'outfile':'cccsmtlresult.txt'})\
    ]