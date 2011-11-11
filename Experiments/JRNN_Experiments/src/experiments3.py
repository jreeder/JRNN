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
        'numTasks':4, 'viewString':'task1,task2,task3,task4', 'expFold':'band/NormSize', \
        'outfile':'results.txt', 'netType':'BP', 'impNumTrain':20}\
        ]

# b.bandnorm + b.bandnormnv + b.bandlargenv + b.bandsmallnv 
#experiments = c.cirinsqnorm + c.cirinsqnormnv + c.cirinsqlargenv + c.cirinsqsmallnv \
            #+ l.linearnorm + l.linearnormnv + l.linearlargenv + l.linearsmallnv



#experiments = g.normimp + g.smallimpnv + g.largeimpnv + g.normimpnv \
            #+ d.normimp + d.smallimpnv + d.largeimpnv + d.normimpnv \
            #+ s.normimp + s.smallimpnv + s.largeimpnv + s.normimpnv

#experiments = b.ccmtl + b.csmtl + l.ccmtl + c.ccmtl

#experiments = b.ccmtl + b.csmtl + b.etamtl \
#    + c.ccmtl + c.csmtl + c.etamtl \
#    + l.ccmtl + l.csmtl + l.etamtl \
#    + g.ccmtl + g.csmtl + g.etamtl \
#    + d.ccmtl + d.csmtl + d.etamtl \
#    + s.ccmtl + s.csmtl + s.etamtl

#experiments = b.csmtlos + c.csmtlos + l.csmtlos + g.csmtlos + d.csmtlos + s.csmtlos

experiments = b.stltests + c.stltests + l.stltests + g.stltests + d.stltests + s.stltests + h.stltests

#experiments = d.largeimpnv + d.normimpnv

