from CMGTools.HToZZTo4Leptons.macros.plotters.TreePlotter import *
from CMGTools.HToZZTo4Leptons.macros.stat.PDFFactory import *
from CMGTools.HToZZTo4Leptons.macros.param.HiggsTheory import *
from CMGTools.HToZZTo4Leptons.tools.fullPath import getFullPath

import ROOT
import array
import os
ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")


class HiggsShapes(object):
    def __init__(self,finalstate,cuts,masses,prod,period):
        #make pdfs
        if os.path.exists(finalstate+'_'+period+'.json'):
            self.fpjson=open(finalstate+'_'+period+'.json')
            self.data = json.load(self.fpjson)
        else:
            self.data=dict()

        if 'shape' not in self.data:    
            self.data['shape']=dict()

        self.theory = HiggsTheory(getFullPath('data/YR_'+period+'.json'))

        self.graph_m0    = ROOT.TGraphErrors()
        self.graph_sigma = ROOT.TGraphErrors()
        self.graph_alphaL = ROOT.TGraphErrors()
        self.graph_alphaR = ROOT.TGraphErrors()
        self.graph_nL = ROOT.TGraphErrors()
        self.graph_nR = ROOT.TGraphErrors()

        self.finalstate=finalstate
        self.period=period

        for i,mass in enumerate(masses):
            self.w = ROOT.RooWorkspace('w')
            self.pdfFactory = PDFFactory(self.w)
#            self.w.factory('H_Mass[125,70,800]')
            plotter = TreePlotter("All_"+period+"/"+prod+str(mass)+".root","FourLeptonTreeProducer/tree",'1')

            #data MC corrections
            plotter.addCorrectionFactor('eventWeight','eventWeight',0.0,'lnN')
            plotter.addCorrectionFactor('vertexWeight','vertexWeight',0.0,'lnN')
            plotter.addCorrectionFactor('eff','H_eff',0.0,'lnN')
            #define final state separation here
            finalStateCut=cuts
            if finalstate=='MuMu':
                finalStateCut+='&&abs(H_Z1_leg1_PdgId)==13&&abs(H_Z2_leg1_PdgId)==13'
            if finalstate=='EleEle':
                finalStateCut+='&&abs(H_Z1_leg1_PdgId)==11&&abs(H_Z2_leg1_PdgId)==11'
            if finalstate=='MuEle':
                finalStateCut+='&&abs(H_Z1_leg1_PdgId)!=abs(H_Z2_leg1_PdgId)'

            #make weighted dataset    

                
            theory = self.theory.getInterp(mass)
            width=float(theory['width'])


            data,obs = plotter.makeDataSet('H_Mass',finalStateCut,[mass-50-3*width],[mass+50+5*width],"data_obs",500000)
            getattr(self.w,'import')(data)

            #make pdf
            if mass<=160.:
                self.pdfFactory.makeDCB('pdf'+finalstate+str(mass),'H_Mass',mass,finalstate)
                self.w.var('CMS_scale_m').setConstant(1)
                self.w.var('CMS_scale_e').setConstant(1)
                self.w.var('CMS_res_m').setConstant(1)
                self.w.var('CMS_res_e').setConstant(1)
                self.w.var('m0').setVal(mass+(26.5702) + (-0.692027*mass) + (0.00680888*mass*mass) + (-3.24132e-05*mass*mass*mass) + (7.37704e-08*mass*mass*mass*mass) + (-6.37514e-11*mass*mass*mass*mass*mass))

            else:    
                if mass<399:
                    self.pdfFactory.makeBW('pdfBW'+finalstate+str(mass),'H_Mass')
                    self.w.var('width').setConstant(1)
                else:    
                    self.pdfFactory.makeBWHighMass('pdfBW'+finalstate+str(mass),'H_Mass')
                self.w.var('MH').setVal(float(mass))
                self.w.var('MH').setConstant(1)
                    
                self.pdfFactory.makeDCB('pdfRes'+finalstate+str(mass),'H_Mass',0.0,finalstate)
                self.w.var('CMS_scale_m').setConstant(1)
                self.w.var('CMS_scale_e').setConstant(1)
                self.w.var('CMS_res_m').setConstant(1)
                self.w.var('CMS_res_e').setConstant(1)

                self.pdfFactory.makeConvolution('pdf'+finalstate+str(mass),'H_Mass','pdfBW'+finalstate+str(mass),'pdfRes'+finalstate+str(mass))
                self.w.var('m0').setVal((26.5702) + (-0.692027*mass) + (0.00680888*mass*mass) + (-3.24132e-05*mass*mass*mass) + (7.37704e-08*mass*mass*mass*mass) + (-6.37514e-11*mass*mass*mass*mass*mass))

            #set defaults
            self.w.var('alphaL').setVal((-21.6016) + (0.602213*mass) + (-0.00620193*mass*mass) + (3.03257e-05*mass*mass*mass) + (-7.05553e-08*mass*mass*mass*mass) + (6.28615e-11*mass*mass*mass*mass*mass))
            self.w.var('alphaR').setVal((110.088) + (-2.74006*mass) + (0.0263436*mass*mass) + (-0.000120005*mass*mass*mass) + (2.59845e-07*mass*mass*mass*mass) + (-2.1514e-10*mass*mass*mass*mass*mass))
            self.w.var('nL').setVal((-261.056) + (5.8594*mass) + (-0.047926*mass*mass) + (0.000186032*mass*mass*mass) + (-3.48259e-07*mass*mass*mass*mass) + (2.53758e-10*mass*mass*mass*mass*mass))
            self.w.var('sigma').setVal((-42.9661) + (1.16387*mass) + (-0.0116153*mass*mass) + (5.54285e-05*mass*mass*mass) + (-1.25301e-07*mass*mass*mass*mass) + (1.07428e-10*mass*mass*mass*mass*mass))


            result = self.w.pdf('pdf'+finalstate+str(mass)).fitTo(self.w.data('data_obs'),ROOT.RooFit.SumW2Error(1),ROOT.RooFit.Save(1),ROOT.RooFit.Minimizer("Minuit2"))

            c=ROOT.TCanvas('c')
            c.cd()
            
            frame=self.w.var('H_Mass').frame()
            self.w.data('data_obs').plotOn(frame)
            self.w.pdf('pdf'+finalstate+str(mass)).plotOn(frame)
            frame.Draw()
            c.SaveAs('validation_pdf'+finalstate+str(mass)+'_'+period+'.png')
            


            self.data['shape'][mass]=dict()
            for param in self.pdfFactory.params:
                self.data['shape'][mass][param]=dict()
                self.data['shape'][mass][param]['value']=self.w.var(param).getVal()
                self.data['shape'][mass][param]['error']=self.w.var(param).getError()
                getattr(self,'graph_'+param).SetPoint(i,float(mass),self.w.var(param).getVal())
                getattr(self,'graph_'+param).SetPointError(i,0.0,self.w.var(param).getError())

        self.fit()
        self.save()
                

    def save(self):
        f=open(self.finalstate+'_'+self.period+'.json','w')
        json.dump(self.data,f)
        f.close()
            



    def fit(self,mini=0.,maxi=1000):

        fil=open('shape_'+self.finalstate+'_'+self.period+'.txt','w')
        
	
        f=ROOT.TF1("f","pol5", mini, maxi)
        self.data['shapeFormula'] =dict()

        for param in ['m0','sigma','alphaL','nL','alphaR','nR']:
            getattr(self,'graph_'+param).Fit(f)
            getattr(self,'graph_'+param).SetMarkerStyle(20)

            c=ROOT.TCanvas('c')
            c.cd()
            getattr(self,'graph_'+param).Draw("AP")    
            f.Draw("LSAME")
            c.SaveAs('shape_'+self.finalstate+'_'+self.period+'_'+param+'.png')

            formula = '('+str(f.GetParameter(0))+'+('+str(f.GetParameter(1))+'*@0)+'+'+('+str(f.GetParameter(2))+'*@0*@0)+('+str(f.GetParameter(3))+'*@0*@0*@0)+('+str(f.GetParameter(4))+'*@0*@0*@0*@0)+('+str(f.GetParameter(5))+"*@0*@0*@0*@0*@0))"
            self.data['shapeFormula'][param] = formula
            fil.write(param +'='+str(f.GetParameter(0))+'+('+str(f.GetParameter(1))+'*@0)+'+'+('+str(f.GetParameter(2))+'*@0*@0)+('+str(f.GetParameter(3))+'*@0*@0*@0)+('+str(f.GetParameter(4))+'*@0*@0*@0*@0)+('+str(f.GetParameter(5))+"*@0*@0*@0*@0*@0)")
        fil.close()
