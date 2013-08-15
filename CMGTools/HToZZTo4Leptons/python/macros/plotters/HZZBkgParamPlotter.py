from PlotterBase import *
from CMGTools.HToZZTo4Leptons.macros.param.HiggsTheory import *
from CMGTools.HToZZTo4Leptons.macros.stat.PDFFactory import *
from CMGTools.HToZZTo4Leptons.tools.fullPath import getFullPath

import ROOT
import sys


class HZZBkgParamPlotter(PlotterBase):
    def __init__(self,finalstate,prod,period,category = 'inc'):
        self.channel=finalstate
        self.period=period
        self.prod=prod
        self.category=category
        self.w=ROOT.RooWorkspace('w')
#        self.massVar='H_Mass'+'_'+finalstate+'_'+period+'_'+category
        self.massVar='H_Mass'
        self.w.factory(self.massVar+'[120,100,1000]')
        self.corrFactors=[]

        #make PDF
        self.pdfFactory = PDFFactory(self.w)
        if self.prod =='QQZZ':
            self.pdfFactory.makeQQZZ('pdf',self.massVar)
        if self.prod =='GGZZ':
            self.pdfFactory.makeGGZZ('pdf',self.massVar)
        if self.prod =='FAKES':
            self.pdfFactory.makeLandau('pdf',self.massVar)

        self.params=self.pdfFactory.params    
    
    def setRate(self,rate):
        self.eff = rate

    def setParameters(self,params):
        if self.prod in ['QQZZ','GGZZ']:
            for i,param in enumerate(params):
                self.w.var('a'+i).setVal(param)
        if self.prod == ['FAKES']:
                self.w.var('mean').setVal(param[0])
                self.w.var('sigma').setVal(param[0])



    def setupFromFile(self):
        f = open(self.channel+'_'+self.period+'.json')
        data = json.load(f)
        for param in (data['bkgshape'][self.prod]):
            self.w.var(param).setVal(data['bkgshape'][self.prod][param]['value'])
            self.w.var(param).setConstant(1)
        self.setRate(data[self.prod][self.category])    
        

    def drawTH1(self,var,cuts,lumi,bins,mini=100,maxi=800,titlex = "",units = "",drawStyle = "HIST"):
        #First find fraction inside cuts
        self.w.var(self.massVar).setRange('fullRange',100.,1000.)
        self.w.var(self.massVar).setRange('fitRange',mini,maxi)

        fullIntegral = self.w.pdf('pdf').createIntegral(ROOT.RooArgSet(self.w.var(self.massVar)),ROOT.RooFit.Range('fullRange')).getVal()
        fitIntegral  = self.w.pdf('pdf').createIntegral(ROOT.RooArgSet(self.w.var(self.massVar)),ROOT.RooFit.Range('fitRange')).getVal()


        self.w.var(self.massVar).setMin(mini)
        self.w.var(self.massVar).setMax(maxi)
        h = self.w.pdf('pdf').createHistogram(self.massVar,bins)
        scaleFactor = self.eff*fitIntegral/fullIntegral
        h.Scale(scaleFactor)

        h.Scale(float(lumi))
        for corr in self.corrFactors:
            h.Scale(corr['value']) 
        h.GetXaxis().SetTitle(titlex)
        return h

    def make1DPdf(self,w,name,mini=100,maxi=600):
        
        suffix = self.prod+'_'+self.period+'_'+self.category+'_'+self.channel

        if self.prod=='QQZZ': 
            getattr(w,'importClassCode')(ROOT.RooqqZZPdf_v2.Class(),1)
        if self.prod=='GGZZ':
            getattr(w,'importClassCode')(ROOT.RooggZZPdf_v2.Class(),1)

        getattr(w,'import')(self.w.pdf('pdf'),ROOT.RooFit.RecycleConflictNodes(1),ROOT.RooFit.RenameAllVariablesExcept(suffix,self.massVar))
        w.pdf('pdf').SetName(name)    


    def makeDataSet(self,var,cuts,mini = 0,maxi = 1.0e+10,miny=0,maxy=1e+10,name='dataTmp',entries=-1):
        self.mass.setMin(mini)
        self.mass.setMax(maxi)
        #add a generate
        pass






#plotter=HZZBkgParamPlotter('MuMu','FAKES','7TeV','inc')
#plotter.setupFromFile()
#w=ROOT.RooWorkspace('w')
#plotter.make1DPdf(w,'FAKES',100,160)

#w.Print()

#print 'integral',plotter.drawTH1(self.massVar,'',1,100,0,800).Integral()
