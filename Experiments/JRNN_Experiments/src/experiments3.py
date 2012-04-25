#!/usr/bin/env python
# Author:  jreeder --<>
# Purpose: Holds experiments defs for jrnn_exprun 
# Created: 5/27/2011

from expfiles import bandexp as b
from expfiles import cirinsqexp as c
from expfiles import linearexp as l
from expfiles import glassexp as g
from expfiles import dermexp as d
from expfiles import smallcovtypeexp as s
from expfiles import heartexp as h


testdicts = [{'dsname':'band','numRuns':60, 'numInputs':2,'numOutputs':1,\
        'numVal':100, 'numTest':200, 'numTrain':100, \
        'numHidPerTask':4,'useValidation':True, 'primTask':1, \
        'numTasks':4, 'viewString':'task1,task2,task5,task6', 'expFold':'band/NormSize', \
        'outfile':'results.txt', 'netType':'BP', 'impNumTrain':20,'useEtaMTL':True}\
        ]
experiments = []

#experiments += d.csmtl    Just a testing line

#experiments += [b.csmtl[1]]
#
#experiments += testdicts

#New Experiments

#experiments += b.stltests + c.stltests + l.stltests + g.stltests + d.stltests + s.stltests + h.stltests

#experiments += g.normimp + g.smallimpnv + g.largeimpnv + g.normimpnv \
            #+ d.normimp + d.smallimpnv + d.largeimpnv + d.normimpnv \
            #+ l.normimp + l.smallimpnv + l.largeimpnv + l.normimpnv \
            #+ h.normimp + h.smallimpnv + h.largeimpnv + h.normimpnv \
            #+ s.normimp + s.smallimpnv + s.largeimpnv + s.normimpnv \
            #+ b.normimp + b.smallimpnv + b.largeimpnv + b.normimpnv \
            #+ c.normimp + c.smallimpnv + c.largeimpnv + c.normimpnv \



#experiments += b.ccmtl + b.etamtl \
    #+ c.ccmtl + c.etamtl \
    #+ l.ccmtl + l.etamtl \
    #+ d.ccmtl + d.etamtl \
    #+ h.ccmtl + h.etamtl \
    #+ g.ccmtl + g.etamtl \
    #+ s.ccmtl + s.etamtl \

#experiments += b.csmtl \
    #+ c.csmtl \
    #+ l.csmtl \
    #+ d.csmtl \
    #+ h.csmtl \
    #+ g.csmtl \
    #+ s.csmtl 



#experiments += b.csmtlos + c.csmtlos + l.csmtlos + g.csmtlos \
    #+ d.csmtlos + s.csmtlos + h.csmtlos

#experiments += b.csmtlgur + b.mtlgur \
    #+ c.csmtlgur + c.mtlgur \
    #+ l.csmtlgur + l.mtlgur \
    #+ g.csmtlgur + g.mtlgur \
    #+ d.csmtlgur + d.mtlgur \
    #+ s.csmtlgur + s.mtlgur \
    #+ h.csmtlgur + h.mtlgur 

#experiments += b.csmtlur + c.csmtlur + l.csmtlur + g.csmtlur + d.csmtlur + s.csmtlur + h.csmtlur 



experiments += b.reverbNoValNumReverbsRecall + b.reverbValNumReverbsRecall

experiments += b.reverbValNumReverbsRecall

#experiments = [b.reverbValNumReverbsRecall[0], b.reverbNoValNumReverbsRecall[0]]

#old experiments no longer used 
# b.bandnorm + b.bandnormnv + b.bandlargenv + b.bandsmallnv 
#experiments = c.cirinsqnorm + c.cirinsqnormnv + c.cirinsqlargenv + c.cirinsqsmallnv \
            #+ l.linearnorm + l.linearnormnv + l.linearlargenv + l.linearsmallnv
#experiments = b.ccmtl + b.csmtl + l.ccmtl + c.ccmtl
#experiments = d.largeimpnv + d.normimpnv
