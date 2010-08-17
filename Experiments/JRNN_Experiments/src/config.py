# To change this template, choose Tools | Templates
# and open the template in the editor.

import scipy.stats as scistats
import numpy as np

#Windows Values
jrnn_exe = r"C:/Documents and Settings/jreeder/My Documents/Code/JRNN/Builds/VS2010/JRNN/Release/JRNN_test.exe"
datapath = r"C:/Documents and Settings/jreeder/My Documents/Code/JRNN/src/JRNN_test/data/"
outpath = r"C:/Documents and Settings/jreeder/My Documents/Code/JRNN/Experiments/Exp 1/Round-6/"
outpath2 = r"C:/Documents and Settings/jreeder/My Documents/Code/JRNN/Experiments/Exp 2/Round 6/"
jrnn_exe2 = r"C:/Documents and Settings/jreeder/My Documents/Code/JRNN/Builds/VS2010/JRNN/Release/JRNN_test2.exe"
figpath = r"C:/Documents and Settings/jreeder/My Documents/Code/JRNN/Experiments/Figures/"

#Mac Values
#jrnn_exe = r"/Users/jreeder/Code/JRNN/Builds/NetBeans/JRNN_test/dist/Release/GNU-MacOSX/jrnn_test"
#datapath = r"/Users/jreeder/Code/JRNN/src/JRNN_test/data/"
#outpath = r"/Users/jreeder/Code/JRNN/Experiments/Exp 1/Round-5/"
#outpath2 = r"/Users/jreeder/Code/JRNN/Experiments/Exp 2/Round 7/"
#jrnn_exe2 = r"/Users/jreeder/Code/JRNN/Builds/NetBeans/JRNN_test2/dist/Release/GNU-MacOSX/jrnn_test2"

def CalcMeanAndError(array, confidence = 0.95):
    n = len(array)
    m, se = np.mean(array), scistats.sem(array)
    h = se * scistats.t._ppf((1+confidence)/2.0, n-1)
    return m, h