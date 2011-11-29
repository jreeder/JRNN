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

experiments += testdicts

# b.bandnorm + b.bandnormnv + b.bandlargenv + b.bandsmallnv 
#experiments = c.cirinsqnorm + c.cirinsqnormnv + c.cirinsqlargenv + c.cirinsqsmallnv \
            #+ l.linearnorm + l.linearnormnv + l.linearlargenv + l.linearsmallnv



#experiments += g.normimp + g.smallimpnv + g.largeimpnv + g.normimpnv \
#experiments += d.normimp + d.smallimpnv + d.largeimpnv + d.normimpnv \
            #+ l.normimp + l.smallimpnv + l.largeimpnv + l.normimpnv \
            #+ h.normimp + h.smallimpnv + h.largeimpnv + h.normimpnv
#+ s.normimp + s.smallimpnv + s.largeimpnv + s.normimpnv \
            #+ b.normimp + b.smallimpnv + b.largeimpnv + b.normimpnv \
            #+ c.normimp + c.smallimpnv + c.largeimpnv + c.normimpnv \

#experiments = b.ccmtl + b.csmtl + l.ccmtl + c.ccmtl

#experiments += b.ccmtl + b.csmtl + b.etamtl \
    #+ c.ccmtl + c.csmtl + c.etamtl \
#experiments += l.ccmtl + l.csmtl + l.etamtl \
    #+ d.ccmtl + d.csmtl + d.etamtl \
    #+ h.ccmtl + h.csmtl + h.etamtl
     #+ g.ccmtl + g.csmtl + g.etamtl \
     #+ s.ccmtl + s.csmtl + s.etamtl \

#experiments += b.csmtlos + c.csmtlos + l.csmtlos + g.csmtlos \
 #   + d.csmtlos + s.csmtlos + h.csmtlos

#experiments += b.csmtlgur + b.mtlgur \
    #+ c.csmtlgur + c.mtlgur \
    #+ l.csmtlgur + l.mtlgur \
    #+ g.csmtlgur + g.mtlgur \
    #+ d.csmtlgur + d.mtlgur \
    #+ s.csmtlgur + s.mtlgur \
    #+ h.csmtlgur + h.mtlgur 

#experiments += b.csmtlur + c.csmtlur + l.csmtlur + g.csmtlur + d.csmtlur + s.csmtlur + h.csmtlur 

#experiments = b.stltests + c.stltests + l.stltests + g.stltests + d.stltests + s.stltests #+ h.stltests

#experiments = d.largeimpnv + d.normimpnv

