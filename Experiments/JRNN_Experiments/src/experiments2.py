# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__="jreeder"
__date__ ="$Aug 9, 2010 4:42:52 PM$"

experiments = [\
    {'numTrain':50,'numVal':100, 'numTest':500, 'numHidPerOut':4, 'numRuns':60,\
    'impPerc':0.8,'expFold':'NormSize'},\
    {'numTrain':50,'numVal':100, 'numTest':500, 'numHidPerOut':1, 'numRuns':60,\
    'impPerc':0.8,'expFold':'SmallSize'},\
    {'numTrain':50,'numVal':100, 'numTest':500, 'numHidPerOut':8, 'numRuns':60,\
    'impPerc':0.8,'expFold':'LargeSize'}
]