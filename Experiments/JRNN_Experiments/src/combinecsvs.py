#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<jreeder@gmail.com>
# Purpose: Combine csv files based on filters to create tables in excel
# Created: 10/6/2011

import sys
import os

folder = r"C:\Users\John\Documents\Source\JRNN\Experiments\Results\csvresults\EXP 2-1 Round 10-1"
filter1 = "STL"
filter2 = "Error"
outfilename = "combined/" + filter1 + " - " + filter2 + ".csv"
test = True

table = {}
dsnames = []


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
            dsnames.append(dsname)
            with open(filename, 'r') as infile:
                lines = infile.readlines()
                headings = lines[0][:-1].split(',')
                values = lines[1][:-1].split(',')
                for heading, value in zip(headings,values):
                    if heading not in table:
                        table[heading] = {}
                    table[heading][dsname] = value
        
        headers = table.keys();
        headers.sort()
        headerline = ' ,' + ','.join(headers)
        print >>outfile, headerline
        
        for dsname in dsnames:
            line = dsname + ', '
            values = []
            for header in headers:
                values.append(table[header].get(dsname, ' '))
            line += ','.join(values)
            print >>outfile, line
                
                
                