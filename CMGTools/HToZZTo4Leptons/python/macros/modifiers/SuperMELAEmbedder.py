#CHANGED
from CMGTools.HToZZTo4Leptons.macros.modifiers.TreeModifierBase import *
from CMGTools.HToZZTo4Leptons.macros.hzzSetup import * 

import numpy
import math

class SuperMELAEmbedder(TreeModifierBase):
    def __init__(self,MH,period,prefix='H'):
        self.prefix=prefix
        self.var=prefix+'_SuperMELA'+str(int(MH))
        self.scaleUp=prefix+'_SuperMELA'+str(int(MH))+'_scaleUp'
        self.scaleDown=prefix+'_SuperMELA'+str(int(MH))+'_scaleDown'
        self.resUp=prefix+'_SuperMELA'+str(int(MH))+'_resUp'
        self.resDown=prefix+'_SuperMELA'+str(int(MH))+'_resDown'
        self.n = numpy.zeros(1,float)
        self.nSUp = numpy.zeros(1,float)
        self.nSDwn = numpy.zeros(1,float)
        self.nRUp = numpy.zeros(1,float)
        self.nRDwn = numpy.zeros(1,float)

        self.setup  = HZZSetup()
        self.signalPlotterMuMu = self.setup.getHiggsParamPlotter('MuMu','GGH',period,MH)
        self.signalPlotterMuEle = self.setup.getHiggsParamPlotter('MuEle','GGH',period,MH)
        self.signalPlotterEleEle = self.setup.getHiggsParamPlotter('EleEle','GGH',period,MH)

        self.zzPlotterMuMu = self.setup.getZZParamPlotter('MuMu','QQZZ',period)
        self.zzPlotterMuEle = self.setup.getZZParamPlotter('MuEle','QQZZ',period)
        self.zzPlotterEleEle = self.setup.getZZParamPlotter('EleEle','QQZZ',period)
        
        self.w = ROOT.RooWorkspace('w')
        self.signalPlotterMuMu.make1DPdf(self.w,'signalMuMu',100,1000)
        self.signalPlotterMuEle.make1DPdf(self.w,'signalMuEle',100,1000)
        self.signalPlotterEleEle.make1DPdf(self.w,'signalEleEle',100,1000)
        self.MH=MH

        #precalculate integrals



        self.zzPlotterMuMu.make1DPdf(self.w,'bkgMuMu',100,1000)
        self.zzPlotterMuEle.make1DPdf(self.w,'bkgMuEle',100,1000)
        self.zzPlotterEleEle.make1DPdf(self.w,'bkgEleEle',100,1000)
        self.w.var('H_Mass').setRange('fitRange',100,1000)


        for finalstate in ['MuMu','MuEle','EleEle']:
            setattr(self,'integralS'+finalstate,self.w.pdf('signal'+finalstate).createIntegral(ROOT.RooArgSet(self.w.var('H_Mass')),ROOT.RooFit.Range('fitRange')).getVal())
        
            setattr(self,'integralB'+finalstate,self.w.pdf('bkg'+finalstate).createIntegral(ROOT.RooArgSet(self.w.var('H_Mass')),ROOT.RooFit.Range('fitRange')).getVal())


        

    def modifyTree(self,tree):

        self.branch1 = tree.Branch(self.var,self.n,self.var+'/D')
        self.branch2 = tree.Branch(self.scaleUp,self.nSUp,self.scaleUp+'/D')
        self.branch3 = tree.Branch(self.scaleDown,self.nSDwn,self.scaleDown+'/D')
        self.branch4 = tree.Branch(self.resUp,self.nRUp,self.resUp+'/D')
        self.branch5 = tree.Branch(self.var,self.nRDwn,self.resDown+'/D')

    def getValue(self,finalstate,mass,melaS,melaB):
        self.w.var('H_Mass').setVal(mass)
        ps = self.w.pdf('signal'+finalstate).getVal()/getattr(self,'integralS'+finalstate)

        pb = self.w.pdf('bkg'+finalstate).getVal()/getattr(self,'integralB'+finalstate)
        return melaS*ps/(melaS*ps+melaB*pb)

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
        
        self.n[0] =self.getValue(fs,mass,melaS,melaB)
        self.w.var('MH').setVal(self.MH+1)
        self.nSUp[0] =self.getValue(fs,mass,melaS,melaB)
        self.w.var('MH').setVal(self.MH-1)
        self.nSDwn[0] =self.getValue(fs,mass,melaS,melaB)
        self.w.var('MH').setVal(self.MH)
        self.w.var('CMS_res_m').setVal(1.2)
        self.w.var('CMS_res_e').setVal(1.2)
        self.nRUp[0] =self.getValue(fs,mass,melaS,melaB)
        self.w.var('CMS_res_m').setVal(0.8)
        self.w.var('CMS_res_e').setVal(0.8)
        self.nRDwn[0] =self.getValue(fs,mass,melaS,melaB)
        self.w.var('CMS_res_m').setVal(1)
        self.w.var('CMS_res_e').setVal(1)




        self.branch1.Fill()
        self.branch2.Fill()
        self.branch3.Fill()
        self.branch4.Fill()
        self.branch5.Fill()
        



 
