from CMGTools.HToZZTo4Leptons.macros.modifiers.TreeModifierBase import *
import numpy
import math

class MergedModifier(TreeModifierBase):
    def __init__(self,modifiers):
        self.modifiers=modifiers
    def modifyTree(self,tree):
        for modifier in self.modifiers:
            modifier.modifyTree(tree)
    def processTreeEvent(self,event,tree):
        for modifier in self.modifiers:
            modifier.processTreeEvent(event,tree)


 
