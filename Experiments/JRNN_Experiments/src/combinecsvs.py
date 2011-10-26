#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<jreeder@gmail.com>
# Purpose: Combine csv files based on filters to create tables in excel
# Created: 10/6/2011

import sys
import os

folder = r"C:\Users\John\Documents\Source\JRNN\Experiments\Results\csvresults\EXP 2-1 Round 10"
filter1 = "ETAMTL"
filter2 = "Error"
outfilename = "combined/" + filter1 + " - " + filter2 + ".csv"
test = True
if __name__=='__main__':
    if not test:
        folder = sys.argv[1]
        filter1 = sys.argv[2]
        filter2 = sys.argv[3]
        outfilename = sys.argv[4]
    
    os.chdir(folder)
    filelist = os.listdir(os.curdir)
    filterlist = [path for path in filelist if (filter1 in path) and (filter2 in path)]
    
    headerwritten = False
    with open(outfilename, "w") as outfile:
        for filename in filterlist:
            dsname = filename.split(" ")[0]
            with open(filename, 'r') as infile:
                lines = infile.readlines()
                if not headerwritten:
                    headerline = " ," + lines[0]
                    outfile.write(headerline)
                    headerwritten = True
                newline = dsname + ", " + lines[1]
                outfile.write(newline)
                
                
                