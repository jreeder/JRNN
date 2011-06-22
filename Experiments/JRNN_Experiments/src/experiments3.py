#!/usr/bin/env python
# Author:  jreeder --<>
# Purpose: Holds experiments defs for jrnn_exprun 
# Created: 5/27/2011

import bandexp as b
import cirinsqexp as c
import linearexp as l
import glassexp as g
import dermexp as d
import smallcovtypeexp as s


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

experiments = b.ccmtl + b.csmtl + l.ccmtl + c.ccmtl

#experiments = d.largeimpnv + d.normimpnv

