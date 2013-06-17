#CHANGED
from PlotterBase import *
from CMGTools.HToZZTo4Leptons.macros.param.HiggsTheory import *
from CMGTools.HToZZTo4Leptons.macros.stat.PDFFactory import *
from CMGTools.HToZZTo4Leptons.tools.fullPath import getFullPath

import ROOT
import sys


class HZZSignalParamPlotter(PlotterBase):
    def __init__(self,finalstate,prod,period,category = 'inc'):
        self.channel=finalstate
        self.period=period
        self.prod=prod
        self.category=category
        self.MH = ROOT.RooRealVar('MH','HiggsMass',125)
        self.width = ROOT.RooRealVar('width','HiggsWidthScale',1.0)
#        self.massVar='H_Mass'+'_'+finalstate+'_'+period+'_'+category
        self.massVar='H_Mass'
        self.mass = ROOT.RooRealVar(self.massVar,'HiggsMass',100,1000)
        self.lowMass=160
        #systematics
        self.scale_mu      = ROOT.RooRealVar("CMS_scale_m","",1.0,0,2.)
        self.scale_ele     = ROOT.RooRealVar("CMS_scale_e","",1.0,0,2.)
        self.res_mu        = ROOT.RooRealVar("CMS_res_m","",0.0,-1,1.)
        self.res_ele       = ROOT.RooRealVar("CMS_res_e","",0.0,-1,1.)
        self.lowmass=160
        self.corrFactors=[]
        #initialize theory
        self.theory = HiggsTheory(getFullPath('data/YR_'+period+'.json'))

        

        if finalstate in ['MuMu','EleEle']:
            self.brStr ='eeee' 
        else:    
            self.brStr ='eemumu' 




        #override for now
#        self.brStr = 'llllt'


    def setMean(self,paramFormula):
        self.m0Formula = paramFormula

            


    def setSigma(self,paramFormula):
        if self.channel =='MuMu':
            self.sigma= ROOT.RooFormulaVar('sigma',paramFormula+'*(1+@1)',ROOT.RooArgList(self.MH,self.res_mu))
        if self.channel =='EleEle':
            self.sigma= ROOT.RooFormulaVar('sigma',paramFormula+'*(1+@1)',ROOT.RooArgList(self.MH,self.res_ele))
        if self.channel =='MuEle':
            self.sigma= ROOT.RooFormulaVar('sigma',paramFormula+'*(1+@1+@2)',ROOT.RooArgList(self.MH,self.res_ele,self.res_mu))

    def setAlphaL(self,paramFormula):
        self.alphaL= ROOT.RooFormulaVar('alphaL',paramFormula,ROOT.RooArgList(self.MH))

    def setNL(self,paramFormula):
        self.nL= ROOT.RooFormulaVar('nL',paramFormula,ROOT.RooArgList(self.MH))

    def setAlphaR(self,paramFormula):
        self.alphaR= ROOT.RooFormulaVar('alphaR',paramFormula,ROOT.RooArgList(self.MH))

    def setNR(self,paramFormula):
        self.nR= ROOT.RooFormulaVar('nR',paramFormula,ROOT.RooArgList(self.MH))

    def setMH(self,MH):
        self.MH.setVal(MH)

    def setEfficiency(self,paramFormula):
        self.eff = ROOT.RooFormulaVar('eff',paramFormula,ROOT.RooArgList(self.MH))

    def setupFromFile(self):
        f = open(self.channel+'_'+self.period+'.json')
        data = json.load(f)
        self.setMean(data['shapeFormula']['m0'])
        self.setSigma(data['shapeFormula']['sigma'])
        self.setAlphaL(data['shapeFormula']['alphaL'])
        self.setAlphaR(data['shapeFormula']['alphaR'])
        self.setNL(data['shapeFormula']['nL'])
        self.setNR(data['shapeFormula']['nR'])
        self.setEfficiency(data['effFormula'][self.prod][self.category])

        
    def updatePdf(self):
        offset='+0'
        if self.MH.getVal()<self.lowMass:
            offset='+@0*@1'

        if self.channel == 'MuMu':
            self.m0= ROOT.RooFormulaVar('m0',self.m0Formula+'*@1'+offset,ROOT.RooArgList(self.MH,self.scale_mu))
        if self.channel == 'MuEle':
            self.m0= ROOT.RooFormulaVar('m0',self.m0Formula+'*@1*@2/4.'+offset,ROOT.RooArgList(self.MH,self.scale_mu,self.scale_ele))
        if self.channel == 'EleEle':
            self.m0= ROOT.RooFormulaVar('m0',self.m0Formula+'*@1'+offset,ROOT.RooArgList(self.MH,self.scale_ele))

        
        if self.MH.getVal()>self.lowMass:

             #Define the energy scale variables and the formula vars
            self.mass.setBins(1000,'fft')
            self.bw    = ROOT.RooRelBWUFParam("HiggsBW", "signalBW",self.mass,self.MH,self.width)
            self.cball    = ROOT.RooDoubleCB('resolution','resolution',self.mass,self.m0,self.sigma,self.alphaL,self.nL,self.nR,self.alphaR)
            self.pdf = ROOT.RooFFTConvPdf('pdf','',self.mass,self.bw,self.cball,2)
            self.pdf.setBufferFraction(0.2)
        else:
            self.pdf    = ROOT.RooDoubleCB('pdf','pdf',self.mass,self.m0,self.sigma,self.alphaL,self.nL,self.nR,self.alphaR)
            
        self.params=[self.m0,self.sigma,self.alphaL,self.nL,self.nR,self.alphaR]

    def drawTH1(self,var,cuts,lumi,bins,mini,maxi,titlex = "",units = "",drawStyle = "HIST"):
        self.mass.setMin(mini)
        self.mass.setMax(maxi)
        self.updatePdf()
        h = self.pdf.createHistogram(self.mass.GetName(),bins)
        scaleFactor = self.eff.getVal()
        h.Scale(self.eff.getVal())
        

        h.Scale(float(lumi))
        for corr in self.corrFactors:
            h.Scale(corr['value'])
        

        theory = self.theory.getInterp(self.MH.getVal())
        fname = {'ggH':'GGH','qqH':'VBF','ZH':'ZH','WH':'WH','ttH':'TTH'}
        if self.prod in ['WH','ZH','ttH']:
            h.Scale(theory[fname[self.prod]]['sigma']*theory['H2B']['ZZ'])
        else:    
            h.Scale(theory[fname[self.prod]]['sigma']*theory['H4F'][self.brStr])
        h.GetXaxis().SetTitle(titlex)
        return h

    def make1DPdf(self,w,name,mini=100,maxi=600):

        self.mass.setRange('fullRange',100.,1000.)
        self.mass.setRange('fitRange',mini,maxi)
        self.updatePdf()
#        self.pdf.SetName('pdf_mass'+'_'+self.prod)
        self.pdf.SetName(name)
        

        fullIntegral = self.pdf.createIntegral(ROOT.RooArgSet(self.mass),ROOT.RooFit.Range('fullRange')).getVal()
        fitIntegral  = self.pdf.createIntegral(ROOT.RooArgSet(self.mass),ROOT.RooFit.Range('fitRange')).getVal()
        
        suffix = self.prod+'_'+self.period+'_'+self.category+'_'+self.channel


        self.mass.setRange(mini,maxi)

        getattr(w,'importClassCode')(ROOT.RooDoubleCB.Class(),1)
        getattr(w,'import')(self.pdf,ROOT.RooFit.RecycleConflictNodes(1),
                            ROOT.RooFit.RenameAllVariablesExcept(suffix,'CMS_res_e,CMS_scale_e,CMS_res_m,CMS_scale_m,'+self.massVar+',MH,width'))

        
        getattr(w,'import')(self.eff,ROOT.RooFit.RecycleConflictNodes(1))
        fname = {'ggH':'GGH','qqH':'VBF','ZH':'ZH','WH':'WH','ttH':'TTH'}

        
        sigma = self.theory.param[fname[self.prod]]['sigma']
        sigma.SetName('sigma_'+self.prod+'_'+self.period)
        getattr(w,'import')(sigma,ROOT.RooFit.RecycleConflictNodes(1))


        if self.prod in ['WH','ZH','ttH']:
            br = self.theory.param['H2B']['ZZ']
        else:    
            br = self.theory.param['H4F'][self.brStr]

        br.SetName('br_'+self.channel)
        getattr(w,'import')(br,ROOT.RooFit.RecycleConflictNodes(1))


        scaleFactor=fitIntegral/fullIntegral
        for corr in self.corrFactors:
            scaleFactor=scaleFactor*corr['value']
        w.factory('scaleFactor_'+suffix+'['+str(scaleFactor)+']')


        for p in ['m0','sigma','alphaL','nL','alphaR','nR','eff']:
            w.function(p).SetName(p+'_'+suffix)

        w.factory("prod::"+self.prod+"_norm("+'br_'+self.channel+','+'sigma_'+self.prod+'_'+self.period+',eff_'+suffix+',scaleFactor_'+suffix+")")
        w.Print()



    def makeDataSet(self,var,cuts,mini = 0,maxi = 1.0e+10,miny=0,maxy=1e+10,name='dataTmp',entries=-1):
        self.mass.setMin(mini)
        self.mass.setMax(maxi)
        #add a generate
        pass






#plotter=HZZSignalParamPlotter('MuMu','GGH','7TeV','inc')

#plotter.setupFromFile()
#plotter.addCorrectionFactor('lumi',17000,0.0,'lnN')
#plotter.setMH(125.)
#w=ROOT.RooWorkspace('w')
#plotter.make1DPdf(w,'GGH',100,160)
#w.var('MH').setVal(125)
#print w.var("scaleFactor_GGH_7TeV_inc_MuMu").getVal()
#print w.function('eff_GGH_7TeV_inc_MuMu').getVal()
#print w.function('sigma_GGH').getVal()
#print w.function('br_MuMu').getVal()
#print w.function('GGH_norm').getVal()

#w.Print()
