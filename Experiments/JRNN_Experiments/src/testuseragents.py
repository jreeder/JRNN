#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<jreeder@gmail.com>
# Purpose: This file will run through and evaluate all networks in a users directory
# Created: 6/13/2013

import os
import sys
from config import *
from localvars import *
from subprocess import *
from multiprocessing import *
import numpy
import json


test = False

def GetUsers():
    return [d for d in os.listdir(luserspath) if os.path.isdir(os.path.join(luserspath, d))]

def testNetwork(netfilename, numRepeats=5, numTicks=2000):
    print "{0} Starting".format(netfilename)
    openneroexe = sourcebase + r"/opennero/Build/dist/Release/OpenNERO.exe"
    opennerodir = sourcebase + r"/opennero/Build/dist/Release/"
    os.chdir(opennerodir)
    netname = os.path.splitext(netfilename)[0]
    outfile = "{netfilename}SimResults.json".format(**locals())
    
    cmd = "{openneroexe} --mod jrnnexp1 --modpath jrnnexp1:common:_jrnn --headless ".format(**locals())\
        + "--command \"TestNetwork('{netfilename}', '{outfile}', {numRepeats}, {numTicks})\"".format(**locals())
    
    
    (stdout, stderr) = Popen(cmd, stdout = PIPE).communicate()

    #if len(stdout) > 0:
        #print stdout
    
    print "{0} Done".format(netname)

def ShowNetsWOResults(firstlist, existlist):
    retList = []
    for filename in firstlist:
        exists = len([x for x in existlist if filename in x])
        if exists == 0:
            retList.append(filename)
    
    return retList

test = False

if __name__=='__main__':
    
    if len(sys.argv) != 2:
        inuser = ''
    else:    
        inuser = sys.argv[1]

    if inuser == '':
        users = GetUsers()
    else:
        users = [inuser]
        
    for user in users:
    
        userP = os.path.join(luserspath, user, 'agents')
        
        networkfiles = os.listdir(userP)
        
        resultP = os.path.join(luserspath, user, 'results')
        
        resultFiles = [x for x in os.listdir(resultP) if 'SimResults' in x]
        
        lefttodo = ShowNetsWOResults(networkfiles, resultFiles)
        
        print len(resultFiles)
        print len(networkfiles)
        print len(lefttodo)
        
        if test:
            pass
            #testNetwork(networkfiles[0], 2, 1000)
        else:
            pool = Pool(numProcesses)
            for netfile in networkfiles:
                #print "Pooling {}".format(netfile)
                pool.apply_async(testNetwork, (netfile,))
                
            pool.close()
            pool.join()
        
        print user + " Complete"
        
    print "Finished"