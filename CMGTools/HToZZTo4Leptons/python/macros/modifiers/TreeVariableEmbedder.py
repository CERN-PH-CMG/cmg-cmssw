from CMGTools.HToZZTo4Leptons.macros.modifiers.TreeModifierBase import *
import numpy
import math

class TreeVariableEmbedder(TreeModifierBase):
    def __init__(self,variable,method):
        self.var=variable
        self.method = method
        self.n = numpy.zeros(1,float)
        self.branch= None
    def modifyTree(self,tree):
        self.branch = tree.Branch(self.var,self.n,self.var+'/D')
        

    def processTreeEvent(self,event,tree):
        self.n[0] =self.method(event)
        self.branch.Fill()

 
