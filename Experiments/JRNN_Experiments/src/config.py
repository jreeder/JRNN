# To change this template, choose Tools | Templates
# and open the template in the editor.

import scipy.stats as scistats
import numpy as np

#Lab Computer
basepath = r"C:/Users/John/Documents/Source/JRNN"
#Home Computer
#basepath = r"C:/Users/John Reeder/Code/JRNN"

#Windows Values
jrnn_exe = basepath + r"/Builds/VS2010/JRNN/Release/JRNN_test.exe"
datapath = basepath + r"/Experiments/Data/Tabbed Data/New Binary Tasks/"
outpath = basepath + r"/Experiments/Exp 1-1/Round-4/"
outpath2 = basepath + r"/Experiments/Exp 2-1/Round 5/"
jrnn_exe2 = basepath + r"/Builds/VS2010/JRNN/Release/JRNN_test2.exe"
figpath = basepath + r"/Experiments/Figures/"
paramspath = basepath + r"/Experiments/config/Parameters.xml"

#Mac Values
#jrnn_exe = r"/Users/jreeder/Source/JRNN/Builds/NetBeans/JRNN_test/dist/Release/GNU-MacOSX/jrnn_test"
#datapath = r"/Users/jreeder/Source/JRNN/src/JRNN_test/data/"
#outpath = r"/Users/jreeder/Source/JRNN/Experiments/Exp 1/Round-5/"
#outpath2 = r"/Users/jreeder/Source/JRNN/Experiments/Exp 2/Round 7/"
#jrnn_exe2 = r"/Users/jreeder/Source/JRNN/Builds/NetBeans/JRNN_test2/dist/Release/GNU-MacOSX/jrnn_test2"

def CalcMeanAndError(array, confidence = 0.95):
    n = len(array)
    m, se = np.mean(array), scistats.sem(array)
    h = se * scistats.t._ppf((1+confidence)/2.0, n-1)
    return m, h