import os.path
#! /usr/bin/python

# Used to parse the experiment files and plot them. 

__author__="jreeder"
__date__ ="$Jul 23, 2010 3:18:41 PM$"

import os
from config import *
from itertools import izip
import ParseExperimentFile as pe
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

expfigpath = figpath + "Exp 1/INDEX05/"
usesave=True

def MakeGraphs(title, fignum, folder1, folder1name, folder2, folder2name):
    os.chdir(outpath + folder1)
    filelist = os.listdir(os.curdir)
    bpdata = {}
    for file in filelist:
        tmpBundle = pe.ParseFile(file)
        bpdata[tmpBundle.dsname] = tmpBundle

    os.chdir(outpath + folder2)
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
    task1epochavg, task1epocherr = CalcMeanAndError(task1bpepochs)
    task1timeavg, task1timeerr = CalcMeanAndError(task1bptimes)

    task2bperr = np.array([v['task-0'] for v in bpdata["band-task2"].errors])
    task2bpepochs = np.array(bpdata["band-task2"].epochs)
    task2bptimes = np.array(bpdata["band-task2"].times)
    task2erravg, task2errerr = CalcMeanAndError(task2bperr)
    task2epochavg, task2epocherr = CalcMeanAndError(task2bpepochs)
    task2timeavg, task2timeerr = CalcMeanAndError(task2bptimes)

    task3bperr = np.array([v['task-0'] for v in bpdata["band-task3"].errors])
    task3bpepochs = np.array(bpdata["band-task3"].epochs)
    task3bptimes = np.array(bpdata["band-task3"].times)
    task3erravg, task3errerr = CalcMeanAndError(task3bperr)
    task3epochavg, task3epocherr = CalcMeanAndError(task3bpepochs)
    task3timeavg, task3timeerr = CalcMeanAndError(task3bptimes)

    task4bperr = np.array([v['task-0'] for v in bpdata["band-task4"].errors])
    task4bpepochs = np.array(bpdata["band-task4"].epochs)
    task4bptimes = np.array(bpdata["band-task4"].times)
    task4erravg, task4errerr = CalcMeanAndError(task4bperr)
    task4epochavg, task4epocherr = CalcMeanAndError(task4bpepochs)
    task4timeavg, task4timeerr = CalcMeanAndError(task4bptimes)

    taskavgerrs = [task1erravg, task2erravg, task3erravg, task4erravg]
    taskerrbars = [task1errerr, task2errerr, task3errerr, task4errerr]
    bpstlavgepochs = [task1epochavg, task2epochavg, task3epochavg, task4epochavg]
    bpstlavgtime = [task1timeavg, task2timeavg, task3timeavg, task4timeavg]
    bpstlerrepochs = [task1epocherr, task2epocherr, task3epocherr, task4epocherr]
    bpstlerrtime = [task1timeerr, task2timeerr, task3timeerr, task4timeerr]
    
    xticks = ["Task 1", "Task 2", "Task 3", "Task 4"]

    plt.figure(fignum)

    locs = np.arange(1, len(taskavgerrs)+1)
    width = 0.2
    plt.bar(locs, taskavgerrs, yerr=taskerrbars, color="red", width=width, \
    antialiased=True, label=folder1name + " STL")



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
    bpmtlepochavg, bpmtlepocherr = CalcMeanAndError(mtltask14epochs)
    bpmtltimeavg, bpmtltimeerr = CalcMeanAndError(mtltask14times)

    mtlavgerr = [mtlt1avge, mtlt2avge, mtlt3avge, mtlt4avge]
    mtleb = [mtlt1eb, mtlt2eb, mtlt3eb, mtlt4eb]

    plt.bar(locs+width, mtlavgerr, yerr=mtleb, color="cyan", width=width, \
    antialiased=True, label=folder1name + " MTL")

#    plt.figure(2)

    cctask1bperr = np.array([v['task-0'] for v in ccdata["band-task1"].errors])
    cctask1bpepochs = np.array(ccdata["band-task1"].epochs)
    cctask1bptimes = np.array(ccdata["band-task1"].times)
    cctask1erravg, cctask1errerr = CalcMeanAndError(cctask1bperr)
    cctask1epochavg, cctask1epocherr = CalcMeanAndError(cctask1bpepochs)
    cctask1timeavg, cctask1timeerr = CalcMeanAndError(cctask1bptimes)

    cctask2bperr = np.array([v['task-0'] for v in ccdata["band-task2"].errors])
    cctask2bpepochs = np.array(ccdata["band-task2"].epochs)
    cctask2bptimes = np.array(ccdata["band-task2"].times)
    cctask2erravg, cctask2errerr = CalcMeanAndError(cctask2bperr)
    cctask2epochavg, cctask2epocherr = CalcMeanAndError(cctask2bpepochs)
    cctask2timeavg, cctask2timeerr = CalcMeanAndError(cctask2bptimes)

    cctask3bperr = np.array([v['task-0'] for v in ccdata["band-task3"].errors])
    cctask3bpepochs = np.array(ccdata["band-task3"].epochs)
    cctask3bptimes = np.array(ccdata["band-task3"].times)
    cctask3erravg, cctask3errerr = CalcMeanAndError(cctask3bperr)
    cctask3epochavg, cctask3epocherr = CalcMeanAndError(cctask3bpepochs)
    cctask3timeavg, cctask3timeerr = CalcMeanAndError(cctask3bptimes)

    cctask4bperr = np.array([v['task-0'] for v in ccdata["band-task4"].errors])
    cctask4bpepochs = np.array(ccdata["band-task4"].epochs)
    cctask4bptimes = np.array(ccdata["band-task4"].times)
    cctask4erravg, cctask4errerr = CalcMeanAndError(cctask4bperr)
    cctask4epochavg, cctask4epocherr = CalcMeanAndError(cctask4bpepochs)
    cctask4timeavg, cctask4timeerr = CalcMeanAndError(cctask4bptimes)

    cctaskavgerrs = [cctask1erravg, cctask2erravg, cctask3erravg, cctask4erravg]
    cctaskerrbars = [cctask1errerr, cctask2errerr, cctask3errerr, cctask4errerr]
    cctaskavgepochs = [cctask1epochavg, cctask2epochavg, cctask3epochavg, cctask4epochavg]
    cctaskerrepochs = [cctask1epocherr, cctask3epocherr, cctask3epocherr, cctask4epocherr]
    cctaskavgtime = [cctask1timeavg, cctask2timeavg, cctask3timeavg, cctask4timeavg]
    cctaskerrtime = [cctask1timeerr, cctask2timeerr, cctask4timeerr, cctask4timeerr]

    plt.bar(locs+2*width, cctaskavgerrs, yerr=cctaskerrbars, color="red", width=width, \
    antialiased=True, hatch="/", label=folder2name + " STL")

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
    ccmtltaskepochavg, ccmtltaskepocherr = CalcMeanAndError(ccmtltask14epochs)
    ccmtltasktimeavg, ccmtltasktimeerr = CalcMeanAndError(ccmtltask14times)

    ccmtlavgerr = [ccmtlt1avge, ccmtlt2avge, ccmtlt3avge, ccmtlt4avge]
    ccmtleb = [ccmtlt1eb, ccmtlt2eb, ccmtlt3eb, ccmtlt4eb]

    plt.bar(locs+3*width, ccmtlavgerr, yerr=ccmtleb, color="cyan", width=width, \
    antialiased=True, hatch="/", label=folder2name + " MTL")
    plt.xticks(locs + 2*width, xticks)
    plt.title(title + " Error")
    plt.ylabel("Error Rate")
    plt.legend()
    if usesave: Save(title + " Error")

    plt.figure(fignum + 1)
    locs2 = np.arange(1,3)
    plt.title(title + " Epochs")
    xticks2 = ["BP", "CC",""]
    plt.ylabel("# Epochs")
#    plt.bar(locs[0], bpstlavgepochs, yerr=bpstlerrepochs, width=width, \
#        antialiased=True)
    StackBar(locs2[0], bpstlavgepochs, bpstlerrepochs, width, True)
    plt.bar(locs2[0] + width, bpmtlepochavg, yerr=bpmtlepocherr, width=width, \
        antialiased=True, color = '#FF3333', label = 'MTL All Tasks')

    StackBar(locs2[1], cctaskavgepochs, cctaskerrepochs, width, False)
    plt.bar(locs2[1] + width, ccmtltaskepochavg, yerr=ccmtltaskepocherr, width=width, \
        antialiased=True, color = '#FF3333')

    plt.legend()
    plt.xticks(locs + width, xticks2)
    if usesave: Save(title + " Epochs")

    plt.figure(fignum + 2)
    plt.title(title + " Times")
    plt.ylabel("Time (s)")

    StackBar(locs2[0], bpstlavgtime, bpstlerrtime, width, True)
    plt.bar(locs2[0] + width, bpmtltimeavg, yerr=bpmtltimeerr, width=width,\
    antialiased=True, color="#FF3333", label='MTL All Tasks')

    StackBar(locs2[1], cctaskavgtime, cctaskerrtime, width, False)
    plt.bar(locs2[1] + width, ccmtltasktimeavg, yerr=ccmtltasktimeerr, width=width, \
    antialiased=True, color="#FF3333")

    plt.legend()
    plt.xticks(locs+width, xticks2)
    if usesave: Save(title + " Times")

def StackBar(loc, avg, errs, width, label):
    bottom = 0
    count = 1
    colors = ['#008FB2','#00B8E6', '#33D6FF', '#80E6FF']
    for a, e, c in izip(avg, errs, colors):
        if label: thislabel = "%s %s" % ("STL Task",count)
        else: thislabel = ""
        plt.bar(loc, a, yerr=e, width=width, bottom=bottom, antialiased=True, \
            label = thislabel, color=c)
        bottom = bottom + a
        count = count + 1

def Save(name):
    plt.savefig(expfigpath + name + ".pdf")
    plt.savefig(expfigpath + name + ".eps")
    plt.savefig(pp, format="pdf")

if __name__ == "__main__":

    if not os.path.exists(expfigpath):
        os.makedirs(expfigpath)

    if usesave: pp = PdfPages(expfigpath + "Collected Figures.pdf")
    MakeGraphs("Full Training", 1, "BP Test 1/", "BP", "CC Test 1/", "CC")
    MakeGraphs("Half Training", 4, "BP Test 2/", "BP", "CC Test 2/", "CC")
    MakeGraphs("Minimal Training", 7, "BP Test 3/", "BP", "CC Test 3/", "CC")
    MakeGraphs("Full Training (NV)", 10, "BP Test 7/", "BP", "CC Test 4/", "CC")
    MakeGraphs("Half Training (NV)", 13, "BP Test 8/", "BP", "CC Test 5/", "CC")
    MakeGraphs("Minimal Training (NV)", 16, "BP Test 9/", "BP", "CC Test 6/", "CC")

    MakeGraphs("Full Training (1 NPT)", 19, "BP Test 4/", "BP", "CC Test 1/", "CC")
    MakeGraphs("Full Training (8 NPT)", 22, "BP Test 5/", "BP", "CC Test 1/", "CC")
    MakeGraphs("Full Training (8 NPT NV)", 25, "BP Test 6/", "BP", "CC Test 4/", "CC")
    if not usesave: plt.show()
    if usesave: pp.close()
    #values = pe.ParseFile("band-task3-BP-tr50-v100-t500-hid4-uvT-r60.txt")
#    print values.type
#    print values.epochs
