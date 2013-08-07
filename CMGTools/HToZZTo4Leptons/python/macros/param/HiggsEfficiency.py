from CMGTools.HToZZTo4Leptons.macros.hzzSetup import HZZSetup
from CMGTools.HToZZTo4Leptons.macros.plotters.TreePlotter import TreePlotter

import ROOT
import array
import json
import math
import os
ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")


class HiggsEfficiency(object):
    def __init__(self,finalstate,cuts,masses,prod,period,suffix =''):

        if os.path.exists(finalstate+'_'+period+'.json'):
            self.fpjson=open(finalstate+'_'+period+'.json')
            self.data = json.load(self.fpjson)
        else:
            self.data=dict()

        if 'eff' not in self.data:
            self.data['eff']=dict()

        if prod not in self.data['eff']:
            self.data['eff'][prod]=dict()

        if suffix not in self.data['eff'][prod]:
            self.data['eff'][prod][suffix]=dict()
            
            

        self.finalstate=finalstate
        self.prod=prod
        self.period=period
        self.suffix=suffix
        self.x=array.array('f')
        self.y=array.array('f')



        self.graph    = ROOT.TGraphErrors()
        i=0
        for mass in masses:
            fname={'ggH':'ggH','qqH':'qqH','WH':'VH','ZH':'VH','ttH':'VH'}
            plotter = TreePlotter("All_"+period+"/"+fname[prod]+str(mass)+".root","FourLeptonTreeProducer/tree",'1')
            plotter.setupFromFile("All_"+period+"/"+fname[prod]+str(mass)+".pck",finalstate,prod)

            #data MC corrections
            plotter.addCorrectionFactor('eventWeight','eventWeight',0.0,'lnN')
            plotter.addCorrectionFactor('vertexWeight','vertexWeight',0.0,'lnN')
            plotter.addCorrectionFactor('eff','H_eff',0.0,'lnN')
            if mass not in [124,125,126] and period =='7TeV':
                plotter.addCorrectionFactor('kin',0.5+0.5*ROOT.TMath.Erf((mass - 80.85)/50.42),0.0,'lnN')

            #define final state separation here
            finalStateCut=cuts

            if prod =='ZH':
                finalStateCut = finalStateCut+'&&genHProcess==24'
            elif  prod =='WH':
                finalStateCut = finalStateCut+'&&genHProcess==26'
            elif  prod =='ttH':
                finalStateCut = finalStateCut+'&&genHProcess>120'
                
            if finalstate=='MuMu':
                finalStateCut+='&&abs(H_Z1_leg1_PdgId)==13&&abs(H_Z2_leg1_PdgId)==13&&genHDecay==2'
            if finalstate=='EleEle':
                finalStateCut+='&&abs(H_Z1_leg1_PdgId)==11&&abs(H_Z2_leg1_PdgId)==11&&genHDecay==1'
            if finalstate=='MuEle':
                finalStateCut+='&&abs(H_Z1_leg1_PdgId)!=abs(H_Z2_leg1_PdgId)&&genHDecay==3'



            h = plotter.drawTH1('H_Mass',finalStateCut,'1',100,0,100000)
            self.data['eff'][self.prod][self.suffix][mass]=h.Integral()
            if h.Integral()>0 and mass>100 :
                self.x.append(mass)
                self.y.append(h.Integral())
                self.graph.SetPoint(i,mass,h.Integral())
                if h.GetEffectiveEntries()>0:
                    self.graph.SetPointError(i,0,h.Integral()/math.sqrt(h.GetEffectiveEntries()))
                else:    
                    self.graph.SetPointError(i,0,0)
                i=i+1

    def fit(self,mini=0.,maxi=1000):
        self.graph.SetMarkerStyle(20);
        fname='eff_'+self.finalstate+'_'+self.prod+'_'+self.period+'_'+self.suffix


        f=ROOT.TF1("f","pol5", mini, maxi)
        self.graph.Fit(f,'','',mini,maxi)
        c=ROOT.TCanvas('c')
        c.cd()
        self.graph.Draw("AP")    
        f.Draw("LSAME")
        c.SaveAs(fname+'_fit.png')

        self.formula = '('+str(f.GetParameter(0))+'+('+str(f.GetParameter(1))+'*@0)'+'+('+str(f.GetParameter(2))+'*@0*@0)+('+str(f.GetParameter(3))+'*@0*@0*@0)+('+str(f.GetParameter(4))+'*@0*@0*@0*@0)+('+str(f.GetParameter(5))+"*@0*@0*@0*@0*@0))"

        if 'effFormula' not in self.data:
            self.data['effFormula']=dict()

        if self.prod not in self.data['effFormula']:
            self.data['effFormula'][self.prod]=dict()
        self.data['effFormula'][self.prod][self.suffix] = self.formula

        print 'FORMULA==='+self.prod+'_'+self.finalstate+'_'+self.period+':::'+self.formula

#        fil=open(fname+'.txt','w')
#        fil.write('a1 = '+str(f.GetParameter(0))+'\n')
##        fil.write('a2 = '+str(f.GetParameter(1))+'\n')
#        fil.write('a3 = '+str(f.GetParameter(2))+'\n')
#        fil.write('a4 = '+str(f.GetParameter(3))+'\n')
#        fil.write('b1 = '+str(f.GetParameter(4))+'\n')
#        fil.write('b2 = '+str(f.GetParameter(5))+'\n')
##        fil.write('b3 = '+str(f.GetParameter(6))+'\n')
#        fil.close()


    def getSpline(self,name='spline'):
        self.MH = ROOT.RooRealVar("MH","",125,min(self.x),max(self.x))
        spline=ROOT.RooSpline1D(name, name, self.MH, len(self.x), self.x, self.y)
        return spline,self.MH

    def save(self,suffix):
        fname=self.finalstate+'_'+self.period
        f=open(fname+'.json','w')
        json.dump(self.data,f)
        f.close()


