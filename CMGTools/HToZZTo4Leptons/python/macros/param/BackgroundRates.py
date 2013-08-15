#CHANGED
from CMGTools.HToZZTo4Leptons.macros.plotters.TreePlotter import *
from CMGTools.HToZZTo4Leptons.macros.hzzSetup import *

import ROOT
import array
import json
import math
import os
ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")


class BackgroundRates(object):
    def __init__(self,finalstate,cuts,prod,period,suffix =''):

        if os.path.exists(finalstate+'_'+period+'.json'):
            self.fpjson=open(finalstate+'_'+period+'.json')
            self.data = json.load(self.fpjson)
        else:
            self.data=dict()

        if prod not in self.data:
            self.data[prod]=dict()

        self.finalstate=finalstate
        self.prod=prod
        self.period=period
        self.suffix=suffix
        self.setup = HZZSetup()

        

        if self.prod in['GGZZ','QQZZ']:
            plotter,plotters=self.setup.getZZPlotters(finalstate,self.prod,period)
        elif self.prod=='FAKES':
            if suffix =='vbf':
                plotter=self.setup.getFakePlotter(finalstate,period,'HLoose_Z2_Charge!=0&&abs(HLoose_Z2_leg1_SIP3D)<4&&abs(HLoose_Z2_leg2_SIP3D)<4','&&H_NJets>=2',0.3)
            elif suffix =='lt':
                plotter=self.setup.getFakePlotter(finalstate,period,'HLoose_Z2_Charge!=0&&abs(HLoose_Z2_leg1_SIP3D)<4&&abs(HLoose_Z2_leg2_SIP3D)<4','&&H_lepton_Pt>10',4.48e-3)

            else:    
                plotter=self.setup.getFakePlotter(finalstate,period,'HLoose_Z2_Charge!=0&&abs(HLoose_Z2_leg1_SIP3D)<4&&abs(HLoose_Z2_leg2_SIP3D)<4')

        #define final state separation here
        finalStateCut=cuts
        if finalstate=='MuMu':
            finalStateCut+='&&abs(H_Z1_leg1_PdgId)==13&&abs(H_Z2_leg1_PdgId)==13'
        if finalstate=='EleEle':
            finalStateCut+='&&abs(H_Z1_leg1_PdgId)==11&&abs(H_Z2_leg1_PdgId)==11'
        if finalstate=='MuEle':
            finalStateCut+='&&abs(H_Z1_leg1_PdgId)!=abs(H_Z2_leg1_PdgId)'

        h = plotter.drawTH1('H_Mass',finalStateCut,'1',100,100,800)
        self.data[self.prod][self.suffix]=h.Integral()
        

    def save(self,suffix):
        fname=self.finalstate+'_'+self.period
        f=open(fname+'.json','w')
        json.dump(self.data,f)
        f.close()


