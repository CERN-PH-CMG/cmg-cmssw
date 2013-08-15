#CHANGED
from CMGTools.HToZZTo4Leptons.macros.modifiers.TreeModifierBase import *
from CMGTools.HToZZTo4Leptons.macros.hzzSetup import * 

import numpy
import math

ROOT.gSystem.Load("libZZMatrixElementSuperMELA")

class SuperMELAEmbedder(TreeModifierBase):
    def __init__(self,MH,energy,period,prefix='H'):
        self.prefix=prefix
        self.var=prefix+'_SuperMELA'+str(int(MH))
        self.n = numpy.zeros(1,float)
        self.n1 = numpy.zeros(1,float)
        self.n2 = numpy.zeros(1,float)
        self.n3 = numpy.zeros(1,float)
        
        self.MH=MH

        self.SMMuMu = ROOT.SuperMELA(MH,"4mu",energy)
        self.SMEleEle = ROOT.SuperMELA(MH,"4e",energy)
        self.SMMuEle = ROOT.SuperMELA(MH,"2e2mu",energy)


    def modifyTree(self,tree):

        self.branch = tree.Branch(self.var,self.n,self.var+'/D')


    def getValue(self,finalstate,mass,melaS,melaB):
        getattr(self,'SM'+finalstate).computeKD(mass,melaS,melaB,n,n1,n2,n4)

    def processTreeEvent(self,event,tree):
        mass =  getattr(event,self.prefix+'_Mass')
        melaS=  getattr(event,self.prefix+'_MELAS')
        melaB=  getattr(event,self.prefix+'_MELAB')
        pdg1 =  getattr(event,self.prefix+'_Z1_leg1_PdgId')
        pdg2 =  getattr(event,self.prefix+'_Z2_leg1_PdgId')

        if abs(pdg1)==11 and abs(pdg2)==11:
            fs = 'EleEle'
        elif    abs(pdg1)==13 and abs(pdg2)==13:
            fs = 'MuMu'
        else:     
            fs ='MuEle'
        
        self.getValue(fs,mass,melaS,melaB)
        self.branch.Fill()
        



 
