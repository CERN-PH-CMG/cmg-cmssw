#kCHANGED
from CMGTools.HToZZTo4Leptons.macros.plotters.TreePlotter import *
from CMGTools.HToZZTo4Leptons.macros.stat.PDFFactory import *
from CMGTools.HToZZTo4Leptons.macros.param.HiggsTheory import *
from CMGTools.HToZZTo4Leptons.tools.fullPath import getFullPath
from CMGTools.HToZZTo4Leptons.macros.hzzSetup import *

import ROOT
import array
import os
ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")


class BackgroundShapes(object):
    def __init__(self,finalstate,cuts,prod,period):
        #make pdfs
        if os.path.exists(finalstate+'_'+period+'.json'):
            self.fpjson=open(finalstate+'_'+period+'.json')
            self.data = json.load(self.fpjson)
        else:
            self.data=dict()

        if 'bkgshape' not in self.data:    
            self.data['bkgshape']=dict()

        self.finalstate=finalstate
        self.period=period

        self.w = ROOT.RooWorkspace('w')
        self.pdfFactory = PDFFactory(self.w)
        self.setup =HZZSetup()

        if prod in['GGZZ','QQZZ']:
            plotter,zzplotters=self.setup.getZZPlotters(finalstate,prod,period)
        if prod in['FAKES']:
            plotter=self.setup.getFakePlotter(finalstate,period,'HLoose_Z2_Charge!=0&&abs(HLoose_Z2_leg1_SIP3D)<4&&abs(HLoose_Z2_leg2_SIP3D)<4')

        #define final state separation here
        finalStateCut=cuts
        if finalstate=='MuMu':
            finalStateCut+='&&abs(H_Z1_leg1_PdgId)==13&&abs(H_Z2_leg1_PdgId)==13'
        if finalstate=='EleEle':
            finalStateCut+='&&abs(H_Z1_leg1_PdgId)==11&&abs(H_Z2_leg1_PdgId)==11'
        if finalstate=='MuEle':
            finalStateCut+='&&abs(H_Z1_leg1_PdgId)!=abs(H_Z2_leg1_PdgId)'

            #make weighted dataset    
            


        data,obs = plotter.makeDataSet('H_Mass',finalStateCut,[100],[1000],"data_obs",-1)
        getattr(self.w,'import')(data)

            #make pdf
        if prod =='GGZZ':
            self.pdfFactory.makeGGZZ('pdf'+prod,'H_Mass')
        elif prod=='QQZZ':
            self.pdfFactory.makeQQZZ('pdf'+prod,'H_Mass')
        else: 
            self.pdfFactory.makeLandau('pdf'+prod,'HLoose_Mass')


        result = self.w.pdf('pdf'+prod).fitTo(self.w.data('data_obs'),ROOT.RooFit.SumW2Error(1),ROOT.RooFit.Save(1),ROOT.RooFit.Minimizer("Minuit2"))

        c=ROOT.TCanvas('c')
        c.cd()
        if prod in ['QQZZ','GGZZ']:    
            frame=self.w.var('H_Mass').frame()
        else:    
            frame=self.w.var('HLoose_Mass').frame()

        self.w.data('data_obs').plotOn(frame)
        self.w.pdf('pdf'+prod).plotOn(frame)
        frame.Draw()
        c.SaveAs('validation_pdf'+finalstate+prod+'_'+period+'.png')

        self.data['bkgshape'][prod]=dict()
        for param in self.pdfFactory.params:
            self.data['bkgshape'][prod][param]=dict()
            self.data['bkgshape'][prod][param]['value']=self.w.var(param).getVal()
            self.data['bkgshape'][prod][param]['error']=self.w.var(param).getError()
        self.save()
                

    def save(self):
        f=open(self.finalstate+'_'+self.period+'.json','w')
        json.dump(self.data,f)
        f.close()
            


