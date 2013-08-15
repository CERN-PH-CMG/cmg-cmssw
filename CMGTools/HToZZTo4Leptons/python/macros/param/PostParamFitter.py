#CHANGED
import ROOT
import json


class PostParamFitter(object):
    def __init__(self,finalstate,period):
        f=open(finalstate+'_'+period+'.json')
        self.data = json.load(f)

        self.filename=finalstate+'_'+period+'.json'

    def save(self):
        f=open(self.filename,'w')
        json.dump(self.data,f)

    def refitParam(self,param,fit,mini=0,maxi=1000,exclude =[]):
        graph =ROOT.TGraphErrors()
        for i,mass in enumerate(self.data['shape']):
            if param in self.data['shape'][mass] and float(mass)>mini and float(mass)<maxi and mass not in exclude:   
                print 'mass=',mass
                
                paramVal = self.data['shape'][mass][param]['value']
                paramError = self.data['shape'][mass][param]['error']
                graph.SetPoint(i,float(mass),float(paramVal))
                graph.SetPointError(i,0.0,float(paramError))
        graph.SetMarkerStyle(20)    
        f=ROOT.TF1('func','pol'+str(fit),mini,maxi)
        graph.Fit(f)

        c=ROOT.TCanvas('c')
        c.cd()
        graph.Draw("AP")    
        f.Draw("LSAME")
        formula=''
        for i in range(0,fit+1):
            formula+='('+str(f.GetParameter(i))
            for j in range(0,i):
                formula+='*@0'
            formula+=')'
            if i<fit:
                formula+='+'

        print formula
        self.data['shapeFormula'][param] = formula

        return c,graph




    def refitEff(self,prod,suffix,fit,mini=0,maxi=1000,exclude =[]):
        graph =ROOT.TGraph()
        for i,mass in enumerate(self.data['eff'][prod][suffix]):
            if float(mass)>mini and float(mass)<maxi and mass not in exclude:   
                print 'mass=',mass

                paramVal = self.data['eff'][prod][suffix][mass]

                graph.SetPoint(i,float(mass),float(paramVal))

        graph.SetMarkerStyle(20)    
        f=ROOT.TF1('func','pol'+str(fit),mini,maxi)
        graph.Fit(f)

        c=ROOT.TCanvas('c')
        c.cd()
        graph.Draw("AP")    
        f.Draw("LSAME")
        formula=''
        for i in range(0,fit+1):
            formula+='('+str(f.GetParameter(i))
            for j in range(0,i):
                formula+='*@0'
            formula+=')'
            if i<fit:
                formula+='+'

        print formula
        self.data['effFormula'][prod][suffix] = formula

        return c,graph
