# To change this template, choose Tools | Templates
# and open the template in the editor.

import scipy.stats as scistats
import numpy as np
from math import modf, floor
from localvars import *

#moved base path to local non repository file called localvars.py
#Lab Computer
#basepath = r"C:/Users/John/Documents/Source/JRNN"
#Home Computer
#basepath = r"D:/Users/John Reeder/Code/JRNN"

#Windows Values
jrnn_exe = basepath + r"/Builds/VS2010/JRNN/Release/JRNN_test.exe"
datapath = basepath + r"/Experiments/Data/Tabbed Data/New Binary Tasks/"
datapath2 = basepath + r"/Experiments/Data/Tabbed Data/UCI Data/"
outpath = basepath + r"/Experiments/Results/Exp 1-1/Round-4/"
outpath2 = basepath + r"/Experiments/Results/Exp 2-1/Round 5/"
outpath3 = basepath + r"/Experiments/Results/Exp 2-1/Round 12/" #8 is best run 9 is new csmtl stuff 10 is all ds with csmtl ccmtl and etamtl and stltests
#Round 11 has the new experiments with more impoverished sets 12 has some reruns the gradur fix and the csmtlur exp. 
genericout = basepath + r"/Experiments/Results/"
jrnn_exe2 = basepath + r"/Builds/VS2010/JRNN/Release/JRNN_test2.exe"
jrnn_exprun = basepath + r"/Builds/VS2010/JRNN/Release/JRNN_exprun.exe"
figpath = basepath + r"/Experiments/Results/Figures/"
csvpath = basepath + r"/Experiments/Results/csvresults/"
paramspath = basepath + r"/Experiments/config/Parameters.xml"

#Mac Values
#jrnn_exe = r"/Users/jreeder/Source/JRNN/Builds/NetBeans/JRNN_test/dist/Release/GNU-MacOSX/jrnn_test"
#datapath = r"/Users/jreeder/Source/JRNN/src/JRNN_test/data/"
#outpath = r"/Users/jreeder/Source/JRNN/Experiments/Exp 1/Round-5/"
#outpath2 = r"/Users/jreeder/Source/JRNN/Experiments/Exp 2/Round 7/"
#jrnn_exe2 = r"/Users/jreeder/Source/JRNN/Builds/NetBeans/JRNN_test2/dist/Release/GNU-MacOSX/jrnn_test2"

def CalcMeanAndError(array, confidence = 0.95, rmoutliers = True):
    if rmoutliers:
        array = RemoveOutliers(array)
    n = len(array)
    m, se = np.mean(array), scistats.sem(array)
    h = se * scistats.t._ppf((1+confidence)/2.0, n-1)
    return m, h

from math import modf, floor
 
 
def RemoveOutliers(array, verbose=False):
    """
    This function uses a quick method of removing outliers using the 
    Interquartile Range. If a value lies 1.5 times outside the interquartile range 
    it will be removed
    """
    y = sorted(array)
    q1 = quantile(y, .25, issorted=True)
    q3 = quantile(y, .75, issorted=True)
    iqr = q3 - q1
    outlow = q1 - 1.5*iqr
    outhigh = q3 + 1.5*iqr
    newarray = [v for v in array if (v <= outhigh and v >= outlow)]
    if verbose: print "Orginal Size: "+ str(len(array)) + " New Size: "+ str(len(newarray))
    return newarray
    
def quantile(x, q,  qtype = 7, issorted = False):
    """
    Args:
       x - input data
       q - quantile
       qtype - algorithm
       issorted- True if x already sorted.
 
    Compute quantiles from input array x given q.For median,
    specify q=0.5.
 
    References:
       http://reference.wolfram.com/mathematica/ref/Quantile.html
       http://wiki.r-project.org/rwiki/doku.php?id=rdoc:stats:quantile
 
    Author:
	Ernesto P.Adorio Ph.D.
	UP Extension Program in Pampanga, Clark Field.
    """
    if not issorted:
        y = sorted(x)
    else:
        y = x
    if not (1 <= qtype <= 9): 
        return None  # error!
 
    # Parameters for the Hyndman and Fan algorithm
    abcd = [(0,   0, 1, 0), # inverse empirical distrib.function., R type 1
            (0.5, 0, 1, 0), # similar to type 1, averaged, R type 2
            (0.5, 0, 0, 0), # nearest order statistic,(SAS) R type 3
 
            (0,   0, 0, 1), # California linear interpolation, R type 4
            (0.5, 0, 0, 1), # hydrologists method, R type 5
            (0,   1, 0, 1), # mean-based estimate(Weibull method), (SPSS,Minitab), type 6 
            (1,  -1, 0, 1), # mode-based method,(S, S-Plus), R type 7
            (1.0/3, 1.0/3, 0, 1), # median-unbiased ,  R type 8
            (3/8.0, 0.25, 0, 1)   # normal-unbiased, R type 9.
           ]
 
    a, b, c, d = abcd[qtype-1]
    n = len(x)
    g, j = modf( a + (n+b) * q -1)
    if j < 0:
        return y[0]
    elif j >= n:           
        return y[n-1]   # oct. 8, 2010 y[n]???!! uncaught  off by 1 error!!!
 
    j = int(floor(j))
    if g ==  0:
        return y[j]
    else:
        return y[j] + (y[j+1]- y[j])* (c + d * g)   