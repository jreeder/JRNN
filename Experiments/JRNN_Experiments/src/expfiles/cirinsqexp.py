#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<>
# Purpose: Hold cirinsq experiments
# Created: 5/31/2011

def cau(source, upd):
    dictcopy = source.copy()
    dictcopy.update(upd)
    return dictcopy

standardvars = {'dsname':'CirInSq','numRuns':60, 'numInputs':2,'numOutputs':1,\
                'numVal':100, 'numTest':200, 'numTrain':100, \
                'numHidPerTask':6,'useValidation':True, 'primTask':0, \
                'numTasks':1, 'viewString':'task1', 'expFold':'CirInSq/NormSize', \
                'outfile':'results.txt', 'netType':'BP', 'impNumTrain':0}

impNumTrain = 15
normnvpath = 'CirInSq/NormSizeNV'
largenvpath = 'CirInSq/LargeSizeNV'
smallnvpath = 'CirInSq/SmallSizeNV'

stlpath = 'CirInSq/stltests'
ccmtlpath = 'CirInSq/CCMTLTest'
etamtlpath = 'CirInSq/ETAMTLTest'
csmtlpath = 'CirInSq/CSMTLTest'
gradurpath = 'CirInSq/GRADURTest'

gradurview1 = 'task1,task1,task1,task1,task1,task1'
gradurview2 = 'task1,task1,task1,task1,task1,task2'
gradurview3 = 'task1,task1,task1,task1,task2,task7'
gradurview4 = 'task1,task1,task1,task2,task7,task8'

largenumhid = 12
regnumhid = 6
smallnumhid = 2
relatedView = 'task1,task2,task7,task8'
urView = 'task1,task2,task5,task6'
primTask = 1

normimp = [cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':relatedView, 'primTask':primTask, 'outfile':'bpmtlresults.txt'}),\
           cau(standardvars, {'numTasks':1, 'numTrain':impNumTrain, 'viewString':'task1', 'outfile':'bpstlresults.txt'}),\
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':urView, 'primTask':primTask, 'outfile':'bpmtlurresults.txt'}),\
           #CC 
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':relatedView, 'primTask':primTask, 'outfile':'ccmtlresults.txt','netType':'CC'}),\
           cau(standardvars, {'numTasks':1, 'numTrain':impNumTrain, 'viewString':'task1', 'outfile':'ccstlresults.txt','netType':'CC'}),\
           cau(standardvars, {'numTasks':4, 'impNumTrain':impNumTrain, 'viewString':urView, 'primTask':primTask, 'outfile':'ccmtlurresults.txt','netType':'CC'}),\
           ]

standardvars.update({'expFold':stlpath})

stltests = [cau(standardvars, {'viewString':'task1', 'outfile':'bpstltask1.txt'}),\
            cau(standardvars, {'viewString':'task2', 'outfile':'bpstltask2.txt'}),\
            cau(standardvars, {'viewString':'task3', 'outfile':'bpstltask3.txt'}),\
            cau(standardvars, {'viewString':'task4', 'outfile':'bpstltask4.txt'}),\
            cau(standardvars, {'viewString':'task5', 'outfile':'bpstltask5.txt'}),\
            cau(standardvars, {'viewString':'task6', 'outfile':'bpstltask6.txt'}),\
            #CC
            cau(standardvars, {'viewString':'task1', 'outfile':'ccstltask1.txt', 'netType':'CC'}),\
            cau(standardvars, {'viewString':'task2', 'outfile':'ccstltask2.txt', 'netType':'CC'}),\
            cau(standardvars, {'viewString':'task3', 'outfile':'ccstltask3.txt', 'netType':'CC'}),\
            cau(standardvars, {'viewString':'task4', 'outfile':'ccstltask4.txt', 'netType':'CC'}),\
            cau(standardvars, {'viewString':'task5', 'outfile':'ccstltask5.txt', 'netType':'CC'}),\
            cau(standardvars, {'viewString':'task6', 'outfile':'ccstltask6.txt', 'netType':'CC'}),\
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



standardvars.update({'viewString':urView,'numTasks':4,'primTask':primTask, 'useValidation':True, \
                     'impNumTrain':impNumTrain, 'numHidPerTask':regnumhid, 'expFold':ccmtlpath})

ccmtl = [\
    cau(standardvars, {'netType':'CC', 'outfile':'ccurresult.txt'}),\
    cau(standardvars, {'netType':'CCMTL', 'outfile':'ccmtlurresult.txt'})\
    ]

standardvars.update({'expFold':etamtlpath, 'useEtaMTL':True})

#Eta MTL tests. Need to compare these against other unrelated bp and cc tests. 
etamtl = [\
    cau(standardvars, {'netType':'BP', 'outfile':'bpetamtlurresult.txt'}),\
    cau(standardvars, {'netType':'CCMTL', 'outfile':'ccetamtlscoreurresult.txt'}),\
    cau(standardvars, {'netType':'CCMTL', 'useCandSlope':True, 'outfile':'ccetamtlslopeurresult.txt'})\
]

standardvars.update({'useEtaMTL':False})

standardvars.update({'viewString':relatedView, 'expFold':csmtlpath, 'useCSMTLDS':True})

#Test of the csmtl paradymn
csmtl = [\
    cau(standardvars, {'netType':'BP', 'outfile':'bpcsmtlresult.txt'}),\
    cau(standardvars, {'netType':'CC', 'outfile':'cccsmtlresult.txt'})\
    ]

#Test of csmtl with odd fixed sizes. 

standardvars.update({'useValidation':False})

csmtlos = [\
    cau(standardvars, {'netType':'BP', 'numHidPerTask':largenumhid, 'outfile':'bpcsmtllsresult.txt'}),\
    cau(standardvars, {'netType':'CC', 'numHidPerTask':largenumhid, 'outfile':'cccsmtlnvresult.txt'}),\
    cau(standardvars, {'netType':'BP', 'numHidPerTask':smallnumhid, 'outfile':'bpcsmtlssresult.txt'})\
    ]


#Test Showing What happens in MTL and csMTL when the number of tasks become more and more related. 
standardvars.update({'useValidation':True, 'numTasks':6, 'numTrain':impNumTrain, 'expFold':gradurpath,\
                     'impNumTrain':0})

csmtlgur = [\
    cau(standardvars, {'netType':'BP', 'viewString':gradurview1, 'outfile':'bpcsmtlgurv1result.txt'}),\
    cau(standardvars, {'netType':'BP', 'viewString':gradurview2, 'outfile':'bpcsmtlgurv2result.txt'}),\
    cau(standardvars, {'netType':'BP', 'viewString':gradurview3, 'outfile':'bpcsmtlgurv3result.txt'}),\
    cau(standardvars, {'netType':'BP', 'viewString':gradurview4, 'outfile':'bpcsmtlgurv4result.txt'}),\
    #CC
    cau(standardvars, {'netType':'CC', 'viewString':gradurview1, 'outfile':'cccsmtlgurv1result.txt'}),\
    cau(standardvars, {'netType':'CC', 'viewString':gradurview2, 'outfile':'cccsmtlgurv2result.txt'}),\
    cau(standardvars, {'netType':'CC', 'viewString':gradurview3, 'outfile':'cccsmtlgurv3result.txt'}),\
    cau(standardvars, {'netType':'CC', 'viewString':gradurview4, 'outfile':'cccsmtlgurv4result.txt'}),\
    ]

standardvars.update({'useCSMTLDS':False})

mtlgur = [\
    cau(standardvars, {'netType':'BP', 'viewString':gradurview1, 'outfile':'bpmtlgurv1result.txt'}),\
    cau(standardvars, {'netType':'BP', 'viewString':gradurview2, 'outfile':'bpmtlgurv2result.txt'}),\
    cau(standardvars, {'netType':'BP', 'viewString':gradurview3, 'outfile':'bpmtlgurv3result.txt'}),\
    cau(standardvars, {'netType':'BP', 'viewString':gradurview4, 'outfile':'bpmtlgurv4result.txt'}),\
    #CC
    cau(standardvars, {'netType':'CC', 'viewString':gradurview1, 'outfile':'ccmtlgurv1result.txt'}),\
    cau(standardvars, {'netType':'CC', 'viewString':gradurview2, 'outfile':'ccmtlgurv2result.txt'}),\
    cau(standardvars, {'netType':'CC', 'viewString':gradurview3, 'outfile':'ccmtlgurv3result.txt'}),\
    cau(standardvars, {'netType':'CC', 'viewString':gradurview4, 'outfile':'ccmtlgurv4result.txt'}),\
    ]





#Old Tests Here

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