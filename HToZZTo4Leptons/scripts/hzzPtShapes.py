#!/usr/bin/env python
import optparse
import os
import copy
import ROOT
from CMGTools.HToZZTo4Leptons.macros.stat.PDFFactory  import *         

def normalize(h):
    for binx in range(1,h.GetNbinsX()+1):
        sumy=h.ProjectionY('px',binx,binx).Integral()
        if sumy>0:
            for biny in range(1,h.GetNbinsY()+1):
                h.SetBinContent(binx,biny,h.GetBinContent(binx,biny)/sumy)
                h.SetBinError(binx,biny,h.GetBinError(binx,biny)/sumy)


if __name__ == '__main__':

    f = ROOT.TFile("template_Pt.root","RECREATE")
    f.cd()
    for period in ['7TeV','8TeV']:
        for prod in ['ggH','qqH','ZH','WH','ttH','QQZZ','GGZZ','FAKES']:
            w = ROOT.RooWorkspace('w')
            pdfFactory = PDFFactory(w)
            formula=''
        
            if prod == 'ggH':
                formula='1.16763-0.987732*@0+1.61603*@0*@0-0.806264*@0*@0*@0'
            elif prod in ['qqH','ZH','WH','ttH']:
                formula='1.1615-0.943562*@0+1.50617*@0*@0-0.731642*@0*@0*@0'
            elif prod in ['QQZZ','GGZZ']:
                formula='0.90454+0.842558*@0-2.54485*@0*@0+2.24854*@0*@0*@0'
            elif prod =='FAKES':
                formula='1.15298-0.880692*@0+0.896615*@0*@0-0.0458547*@0*@0*@0'
            else:
                print "FUCKED UP"

            w.factory('MELA[0,1]')
            w.factory('PtOM[0,5]')
            w.factory("expr:alpha('"+"@1/("+formula+")',MELA,PtOM)")
            pdfFactory.makeTsallis(prod,'alpha',True)
            pdfFactory.loadPtPreset(prod+'_'+period)

            h = w.pdf(prod).createHistogram('MELA,PtOM',100,100)
            for finalstate in ['MuMu','MuEle','EleEle']:
                h.SetName(finalstate+'_'+prod+'_'+period)
                normalize(h)
                h.Write()

            for syst in  ['Resummation','TopMass','QQPDF','QQZZ7MELA','QQZZ8MELA','QQZZ7Statistics','QQZZ8Statistics','FAKE7Statistics','FAKE8Statistics']:
                s={'Up':'Up','Dwn':'Down'}
                for shift in ['Up','Dwn']:
                    if pdfFactory.loadPtPreset(prod+'_'+period+'_'+syst+'_'+shift):
                        h = w.pdf(prod).createHistogram('MELA,PtOM',100,100)
                        for finalstate in ['MuMu','MuEle','EleEle']:
                            h.SetName(finalstate+'_'+prod+'_'+period+'_'+syst+'_'+s[shift])
                            normalize(h)
                            h.Write()
                        
        
    f.Close()


    
