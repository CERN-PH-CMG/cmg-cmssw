#!/usr/bin/env python

import ROOT
from array import array
from CMGTools.VVResonances.plotting.TreePlotter import TreePlotter
from CMGTools.VVResonances.plotting.MergedPlotter import MergedPlotter
from CMGTools.VVResonances.plotting.StackPlotter import StackPlotter
from CMGTools.VVResonances.statistics.Fitter import Fitter
from math import log
import os, sys, re, optparse,pickle,shutil,json

def returnString(func):
    st='0'
    for i in range(0,func.GetNpar()):
        st=st+"+("+str(func.GetParameter(i))+")"+("*MH"*i)
    return st    


parser = optparse.OptionParser()
parser.add_option("-s","--sample",dest="sample",default='',help="Type of sample")
parser.add_option("-c","--cutShape",dest="cutShape",help="Cut to apply for shape",default='')
parser.add_option("-C","--cutYield",dest="cutYield",help="Cut to apply for yield",default='')
parser.add_option("-o","--output",dest="output",help="Output JSON",default='')
parser.add_option("-v","--MJJ",dest="mjj",help="mjj variable",default='')
parser.add_option("-V","--MVV",dest="mvv",help="mVV variable",default='')
parser.add_option("-p","--particle",dest="boson",help="particle",default='W')


parser.add_option("-m","--minMJJ",dest="minMJJ",type=float,help="minimum MJJ",default=20.0)
parser.add_option("-M","--maxMJJ",dest="maxMJJ",type=float,help="maximum MJJ",default=1700.0)
parser.add_option("-b","--BR",dest="BR",type=float,help="branching ratio",default=1)

#    parser.add_option("-x","--minMVV",dest="minMVV",type=float,help="minimum MVV",default=1000.0)
#    parser.add_option("-X","--maxMVV",dest="maxMVV",type=float,help="maximum MVV",default=13000.0)






(options,args) = parser.parse_args()
#define output dictionary

samples={}
graphs={'yield':ROOT.TGraphErrors(),'scale':ROOT.TGraphErrors(),'sigma':ROOT.TGraphErrors(),'alpha1':ROOT.TGraphErrors(),'n1':ROOT.TGraphErrors(),'alpha2':ROOT.TGraphErrors(),'n2':ROOT.TGraphErrors(),\
            'SCALE':ROOT.TGraphErrors(),'SIGMA':ROOT.TGraphErrors(),'ALPHA1':ROOT.TGraphErrors(),'N1':ROOT.TGraphErrors(),'ALPHA2':ROOT.TGraphErrors(),'N2':ROOT.TGraphErrors(),'slope':ROOT.TGraphErrors(),'f':ROOT.TGraphErrors()}


for filename in os.listdir(args[0]):
    if not (filename.find(options.sample)!=-1):
        continue

#found sample. get the mass
    fnameParts=filename.split('.')
    fname=fnameParts[0]
    ext=fnameParts[1]
    if ext.find("root") ==-1:
        continue
        

    mass = float(fname.split('_')[-1])

        

    samples[mass] = fname

    print 'found',filename,'mass',str(mass) 


#Now we have the samples: Sort the masses and run the fits
N=0
for mass in sorted(samples.keys()):

    print 'fitting',str(mass) 
    plotter=TreePlotter(args[0]+'/'+samples[mass]+'.root','tree')
    plotter.setupFromFile(args[0]+'/'+samples[mass]+'.pck')
    plotter.addCorrectionFactor('genWeight','tree')
    plotter.addCorrectionFactor('xsec','tree')
    plotter.addCorrectionFactor('puWeight','tree')
        
        
    fitter=Fitter(['m','M'])
    fitter.signal2D('model',options.boson)
    fitter.w.var("MH").setVal(mass)


    histo = plotter.drawTH2(options.mjj+":"+options.mvv,options.cutShape,"1",500,0,13000,120,25,165)
    histoYield = plotter.drawTH2(options.mjj+":"+options.mvv,options.cutYield,"1",130,0,13000,100,25,165)
    fitter.importBinnedData(histo,['M','m'],'data')
    fitter.fit('model','data',[ROOT.RooFit.SumW2Error(0)])

    #create the yield
#    fitter.w.var('m').setMax(options.maxMJJ)
#    fitter.w.var('m').setMin(options.minMJJ)
#    integral = fitter.w.pdf("model").createIntegral(ROOT.RooArgSet(fitter.w.var("m"),fitter.w.var("M")))

#    analysisIntegral=integral.getVal()
#    fitter.w.var('m').setMin(25)
#    fitter.w.var('m').setMax(165)
#    integral = fitter.w.pdf("model").createIntegral(ROOT.RooArgSet(fitter.w.var("m"),fitter.w.var("M")))
#    fitRangeIntegral=integral.getVal()


    events=histoYield.Integral()*options.BR
#*analysisIntegral/fitRangeIntegral

    
    graphs['yield'].SetPoint(N,mass,events)

    fitter.projection("model","data","m","fitjj_"+str(mass)+".root")
    fitter.projection("model","data","M","fitVV_"+str(mass)+".root")

    

    for var,graph in graphs.iteritems():
        if var=='yield':
            continue
        value,error=fitter.fetch(var)
        graph.SetPoint(N,mass,value)
        graph.SetPointError(N,0.0,error)
                
    N=N+1


            
#now the fits

pol7 = ROOT.TF1("pol7","pol7",0,13000)
pol5 = ROOT.TF1("pol5","pol5",0,13000)
pol3 = ROOT.TF1("pol3","pol3",0,13000)
pol0 = ROOT.TF1("pol0","pol0",0,13000)
pol1 = ROOT.TF1("pol1","pol1",0,13000)




parameterization={}


c=ROOT.TCanvas("param","param")


graphs['yield'].Fit(pol7)
parameterization['yield']=returnString(pol7)
c.cd()


graphs['scale'].Fit(pol3)
parameterization['scale']=returnString(pol3)
c.cd()

graphs['sigma'].Fit(pol3)
parameterization['sigma']=returnString(pol3)
c.cd()


graphs['n1'].Fit(pol0)
parameterization['n1']=returnString(pol0)
c.cd()


graphs['n2'].Fit(pol0)
parameterization['n2']=returnString(pol0)
c.cd()


graphs['alpha1'].Fit(pol0)
parameterization['alpha1']=returnString(pol0)
c.cd()

graphs['alpha2'].Fit(pol0)
parameterization['alpha2']=returnString(pol0)
c.cd()

graphs['slope'].Fit(pol3)
parameterization['slope']=returnString(pol3)
c.cd()

graphs['f'].Fit(pol3)
parameterization['f']=returnString(pol3)
c.cd()


graphs['SCALE'].Fit(pol3)
parameterization['SCALE']=returnString(pol3)
c.cd()

graphs['SIGMA'].Fit(pol3)
parameterization['SIGMA']=returnString(pol3)
c.cd()

graphs['N1'].Fit(pol0)
parameterization['N1']=returnString(pol0)
c.cd()

graphs['N2'].Fit(pol0)
parameterization['N2']=returnString(pol0)
c.cd()


graphs['ALPHA1'].Fit(pol0)
parameterization['ALPHA1']=returnString(pol0)
c.cd()
    

graphs['ALPHA2'].Fit(pol0)
parameterization['ALPHA2']=returnString(pol0)
c.cd()


f=open(options.output,"w")
json.dump(parameterization,f)
f.close()

F=ROOT.TFile(options.output+'_graphs.root',"RECREATE")
F.cd()
for name,graph in graphs.iteritems():
    graph.Write(name)
F.Close()
            
