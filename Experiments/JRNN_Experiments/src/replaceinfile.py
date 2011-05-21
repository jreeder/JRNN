#!/usr/bin/python

import sys
import os

filepath = sys.argv[1]
find = sys.argv[2]
replace = sys.argv[3]

(filep,filename) = os.path.split(filepath)

(originfname,ext) = os.path.splitext(filename)

newfname = originfname + ".reprocessed" + ext

newfpath = os.path.join(filep, newfname)

f = open(filepath, 'r')
f2 = open(newfpath, 'w')

for line in f:
    newline = line.replace('?', '-9')
    f2.write(newline)
    
f.close()
f2.close()