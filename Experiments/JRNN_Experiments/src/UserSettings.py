#!/usr/bin/env python
#coding:utf-8
# Author:  jreeder --<jreeder@gmail.com>
# Purpose: Just defines the UserSettings Class
# Created: 2/25/2013

class UserSettings:
    def __init__(self, turnSensitivity=30, deadzone=15, speedup=100):
        self.turnSensitivity = turnSensitivity
        self.deadzone = deadzone
        self.speedup = speedup