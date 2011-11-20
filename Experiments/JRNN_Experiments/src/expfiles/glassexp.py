#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<>
# Purpose: file for glass experiments
# Created: 6/1/2011

def cau(source, upd):
    dictcopy = source.copy()
    dictcopy.update(upd)
    return dictcopy

standardvars = {'dsname':'glass','numRuns':60, 'numInputs':9,'numOutputs':1,\
                'numVal':50, 'numTest':100, 'numTrain':50, \
                'numHidPerTask':5,'useValidation':True, 'primTask':0, \
                'numTasks':1, 'viewString':'task1', 'expFold':'glass/NormSize', \
                'outfile':'results.txt', 'netType':'BP', 'impNumTrain':0}

impNumTrain = 15
normnvpath = 'glass/NormSizeNV'
largenvpath = 'glass/LargeSizeNV'
smallnvpath = 'glass/SmallSizeNV'

stlpath = 'glass/stltests'
ccmtlpath = 'glass/CCMTLTest'
etamtlpath = 'glass/ETAMTLTest'
csmtlpath = 'glass/CSMTLTest'
gradurpath = 'glass/GRADURTest'

gradurview1 = 'task1,task1,task1,task1,task1,task1'
gradurview2 = 'task1,task1,task1,task1,task1,task2'
gradurview3 = 'task1,task1,task1,task1,task2,task3'
gradurview4 = 'task1,task1,task1,task2,task3,task6'

largenumhid = 10
regnumhid = 5
smallnumhid = 2
relatedView = 'task1,task2,task3,task6'
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