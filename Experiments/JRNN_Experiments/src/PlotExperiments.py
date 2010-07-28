#! /usr/bin/python

# Used to parse the experiment files and plot them. 

__author__="jreeder"
__date__ ="$Jul 23, 2010 3:18:41 PM$"

import os
from config import *
import ParseExperimentFile as pe
import scipy.stats as scistats
import numpy as np
import matplotlib.pyplot as plt


def CalcMeanAndError(array, confidence = 0.95):
    n = len(array)
    m, se = np.mean(array), scistats.sem(array)
    h = se * scistats.t._ppf((1+confidence)/2.0, n-1)
    return m, h

if __name__ == "__main__":
    os.chdir(outpath + "BP Test 5/")
    filelist = os.listdir(os.curdir)
    bpdata = {}
    for file in filelist:
        tmpBundle = pe.ParseFile(file)
        bpdata[tmpBundle.dsname] = tmpBundle

    os.chdir(outpath + "CC Test 1/")
    filelist = os.listdir(os.curdir)
    ccdata = {}
    for file in filelist:
        tmpBundle = pe.ParseFile(file)
        ccdata[tmpBundle.dsname] = tmpBundle

    #print [v["task-0"] for v in bpdata["band-task1"].errors]
    task1bperr = np.array([v['task-0'] for v in bpdata["band-task1"].errors])
    task1bpepochs = np.array(bpdata["band-task1"].epochs)
    task1bptimes = np.array(bpdata["band-task1"].times)
    task1erravg, task1errerr = CalcMeanAndError(task1bperr)

    task2bperr = np.array([v['task-0'] for v in bpdata["band-task2"].errors])
    task2bpepochs = np.array(bpdata["band-task2"].epochs)
    task2bptimes = np.array(bpdata["band-task2"].times)
    task2erravg, task2errerr = CalcMeanAndError(task2bperr)

    task3bperr = np.array([v['task-0'] for v in bpdata["band-task3"].errors])
    task3bpepochs = np.array(bpdata["band-task3"].epochs)
    task3bptimes = np.array(bpdata["band-task3"].times)
    task3erravg, task3errerr = CalcMeanAndError(task3bperr)

    task4bperr = np.array([v['task-0'] for v in bpdata["band-task4"].errors])
    task4bpepochs = np.array(bpdata["band-task4"].epochs)
    task4bptimes = np.array(bpdata["band-task4"].times)
    task4erravg, task4errerr = CalcMeanAndError(task4bperr)

    taskavgerrs = [task1erravg, task2erravg, task3erravg, task4erravg]
    taskerrbars = [task1errerr, task2errerr, task3errerr, task4errerr]
    xticks = ["Task 1", "Task 2", "Task 3", "Task 4"]

    plt.figure(1)

    locs = np.arange(1, len(taskavgerrs)+1)
    width = 0.2
    plt.bar(locs, taskavgerrs, yerr=taskerrbars, color="red", width=width, \
    antialiased=True, label="BP STL")
    
    

    mtltask1bperr = np.array([v['task-0'] for v in bpdata["band-task1-4"].errors])
    mtlt1avge, mtlt1eb = CalcMeanAndError(mtltask1bperr)
    mtltask2bperr = np.array([v['task-1'] for v in bpdata["band-task1-4"].errors])
    mtlt2avge, mtlt2eb = CalcMeanAndError(mtltask2bperr)
    mtltask3bperr = np.array([v['task-2'] for v in bpdata["band-task1-4"].errors])
    mtlt3avge, mtlt3eb = CalcMeanAndError(mtltask3bperr)
    mtltask4bperr = np.array([v['task-3'] for v in bpdata["band-task1-4"].errors])
    mtlt4avge, mtlt4eb = CalcMeanAndError(mtltask4bperr)
    mtltask14epochs = np.array(bpdata["band-task1-4"].epochs)
    mtltask14times = np.array(bpdata["band-task1-4"].times)

    mtlavgerr = [mtlt1avge, mtlt2avge, mtlt3avge, mtlt4avge]
    mtleb = [mtlt1eb, mtlt2eb, mtlt3eb, mtlt4eb]

    plt.bar(locs+width, mtlavgerr, yerr=mtleb, color="cyan", width=width, \
    antialiased=True, label="BP MTL")

#    plt.figure(2)

    cctask1bperr = np.array([v['task-0'] for v in ccdata["band-task1"].errors])
    cctask1bpepochs = np.array(ccdata["band-task1"].epochs)
    cctask1bptimes = np.array(ccdata["band-task1"].times)
    cctask1erravg, cctask1errerr = CalcMeanAndError(cctask1bperr)

    cctask2bperr = np.array([v['task-0'] for v in ccdata["band-task2"].errors])
    cctask2bpepochs = np.array(ccdata["band-task2"].epochs)
    cctask2bptimes = np.array(ccdata["band-task2"].times)
    cctask2erravg, cctask2errerr = CalcMeanAndError(cctask2bperr)

    cctask3bperr = np.array([v['task-0'] for v in ccdata["band-task3"].errors])
    cctask3bpepochs = np.array(ccdata["band-task3"].epochs)
    cctask3bptimes = np.array(ccdata["band-task3"].times)
    cctask3erravg, cctask3errerr = CalcMeanAndError(cctask3bperr)

    cctask4bperr = np.array([v['task-0'] for v in ccdata["band-task4"].errors])
    cctask4bpepochs = np.array(ccdata["band-task4"].epochs)
    cctask4bptimes = np.array(ccdata["band-task4"].times)
    cctask4erravg, cctask4errerr = CalcMeanAndError(cctask4bperr)

    cctaskavgerrs = [cctask1erravg, cctask2erravg, cctask3erravg, cctask4erravg]
    cctaskerrbars = [cctask1errerr, cctask2errerr, cctask3errerr, cctask4errerr]

    plt.bar(locs+2*width, cctaskavgerrs, yerr=cctaskerrbars, color="red", width=width, \
    antialiased=True, hatch="/", label="CC STL")

    ccmtltask1bperr = np.array([v['task-0'] for v in ccdata["band-task1-4"].errors])
    ccmtlt1avge, ccmtlt1eb = CalcMeanAndError(ccmtltask1bperr)
    ccmtltask2bperr = np.array([v['task-1'] for v in ccdata["band-task1-4"].errors])
    ccmtlt2avge, ccmtlt2eb = CalcMeanAndError(ccmtltask2bperr)
    ccmtltask3bperr = np.array([v['task-2'] for v in ccdata["band-task1-4"].errors])
    ccmtlt3avge, ccmtlt3eb = CalcMeanAndError(ccmtltask3bperr)
    ccmtltask4bperr = np.array([v['task-3'] for v in ccdata["band-task1-4"].errors])
    ccmtlt4avge, ccmtlt4eb = CalcMeanAndError(ccmtltask4bperr)
    ccmtltask14epochs = np.array(ccdata["band-task1-4"].epochs)
    ccmtltask14times = np.array(ccdata["band-task1-4"].times)

    ccmtlavgerr = [ccmtlt1avge, ccmtlt2avge, ccmtlt3avge, ccmtlt4avge]
    ccmtleb = [ccmtlt1eb, ccmtlt2eb, ccmtlt3eb, ccmtlt4eb]

    plt.bar(locs+3*width, ccmtlavgerr, yerr=ccmtleb, color="cyan", width=width, \
    antialiased=True, hatch="/", label="CC MTL")
    plt.xticks(locs + 2*width, xticks)
    plt.legend()
    plt.show()
    #values = pe.ParseFile("band-task3-BP-tr50-v100-t500-hid4-uvT-r60.txt")
#    print values.type
#    print values.epochs
