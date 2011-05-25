# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__="jreeder"
__date__ ="$Aug 9, 2010 4:42:52 PM$"

experiments = [\
    {'dsname':'band','numTasks':8,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':4, 'numRuns':60,'impPerc':0.7,'expFold':'NormSize', 'useVal':'T'},\
    {'dsname':'band','numTasks':8,'numHidPerOut':6,'numVal':100, 'numTest':500,\
     'numHidPerOut':1, 'numRuns':60,'impPerc':0.7,'expFold':'SmallSize', 'useVal':'T'},\
    {'dsname':'band','numTasks':8,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':8, 'numRuns':60,'impPerc':0.7,'expFold':'LargeSize', 'useVal':'T'},\
    {'dsname':'band','numTasks':8,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':4, 'numRuns':60,'impPerc':0.7,'expFold':'NormSizeNoVal', 'useVal':'F'},\
    {'dsname':'band','numTasks':8,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':1, 'numRuns':60,'impPerc':0.7,'expFold':'SmallSizeNoVal', 'useVal':'F'},\
    {'dsname':'band','numTasks':8,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':8, 'numRuns':60,'impPerc':0.7,'expFold':'LargeSizeNoVal', 'useVal':'F'},\
    {'dsname':'CirInSq','numTasks':8,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':6, 'numRuns':60,'impPerc':0.7,'expFold':'NormSize', 'useVal':'T'},\
    {'dsname':'CirInSq','numTasks':8,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':1, 'numRuns':60,'impPerc':0.7,'expFold':'SmallSize', 'useVal':'T'},\
    {'dsname':'CirInSq','numTasks':8,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':8, 'numRuns':60,'impPerc':0.7,'expFold':'LargeSize', 'useVal':'T'},\
    {'dsname':'CirInSq','numTasks':8,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':6, 'numRuns':60,'impPerc':0.7,'expFold':'NormSizeNoVal', 'useVal':'F'},\
    {'dsname':'CirInSq','numTasks':8,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':1, 'numRuns':60,'impPerc':0.7,'expFold':'SmallSizeNoVal', 'useVal':'F'},\
    {'dsname':'CirInSq','numTasks':8,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':8, 'numRuns':60,'impPerc':0.7,'expFold':'LargeSizeNoVal', 'useVal':'F'},\
    {'dsname':'linear','numTasks':6,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':4, 'numRuns':60,'impPerc':0.83,'expFold':'NormSize', 'useVal':'T'},\
    {'dsname':'linear','numTasks':6,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':1, 'numRuns':60,'impPerc':0.83,'expFold':'SmallSize', 'useVal':'T'},\
    {'dsname':'linear','numTasks':6,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':8, 'numRuns':60,'impPerc':0.83,'expFold':'LargeSize', 'useVal':'T'},\
    {'dsname':'linear','numTasks':6,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':4, 'numRuns':60,'impPerc':0.83,'expFold':'NormSizeNoVal', 'useVal':'F'},\
    {'dsname':'linear','numTasks':6,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':1, 'numRuns':60,'impPerc':0.83,'expFold':'SmallSizeNoVal', 'useVal':'F'},\
    {'dsname':'linear','numTasks':6,'numTrain':60,'numVal':100, 'numTest':500,\
     'numHidPerOut':8, 'numRuns':60,'impPerc':0.83,'expFold':'LargeSizeNoVal', 'useVal':'F'}
]