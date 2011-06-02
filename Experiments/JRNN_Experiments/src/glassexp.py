#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<>
# Purpose: file for glass experiments
# Created: 6/1/2011

def copyAndUpdate(source, upd):
    dictcopy = source.copy()
    dictcopy.update(upd)
    return dictcopy

standardvars = {}