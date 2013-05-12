# -*- coding: utf-8 -*-
"""
Created on Fri May 10 21:24:28 2013

@author: John
"""
import re
from collections import defaultdict

variables = re.compile('(\S+?: \S+)\b')

name = re.compile("^(.+) ValSums")

def readIn(fileh):
    data = defaultdict(list)
    for line in fileh:
        vName = name.search(line).groups()[0]
        values = variables.findall(line)
        data[vName].append(values)
    return data
    
    