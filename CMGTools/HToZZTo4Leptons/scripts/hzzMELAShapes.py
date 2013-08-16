#!/usr/bin/env python
import optparse
import os
import copy

from CMGTools.HToZZTo4Leptons.macros.hzzSetup import *

def reweightForInterference(histo):
    reweightFunc =ROOT.TF1("reweightFunc","gaus",100,1600)    
    reweightFunc.SetParameter(0,0.4053)
    reweightFunc.SetParameter(1,110.6)
    reweightFunc.SetParameter(2,22.57)
    for i in range(1,histo.GetNbinsX()+1):
        slope=reweightFunc.Eval((i-1)*2.0+101)
        for j in range(1,histo.GetNbinsY()+1):
            newval = histo.GetBinContent(i,j)*(1+slope*(j/30.-0.5))
            histo.SetBinContent(i,j,newval)
            
def normalize(h):
    for binx in range(1,h.GetNbinsX()+1):
        sumy=h.ProjectionY('px',binx,binx).Integral()
        if sumy>0:
            for biny in range(1,h.GetNbinsY()+1):
                h.SetBinContent(binx,biny,h.GetBinContent(binx,biny)/sumy)
                h.SetBinError(binx,biny,h.GetBinError(binx,biny)/sumy)

def reweightForCR(histo):
    tempUp  = copy.deepcopy(histo)
    tempUp.SetName("tempUp")
    tempDwn = copy.deepcopy(histo)
    tempDwn.SetName("tempDwn")
    
    low=[100.,120.,140.,160.,180.,220.,260.,300.]
    high=[120.,140.,160.,180.,220.,260.,300.,1602.]
    slope=[0.507154,1.13711,0.771557,-1.87945,-3.35464,-1.54489,-1.17508,0.0]
    yIntr=[0.712435,0.654165,0.791353,1.45909,2.83655,1.42336,1.1215,1.0]
    for i in range(1,histo.GetNbinsX()+1):
        point=-1
        for p in range(0,len(slope)):
            m=histo.GetBinCenter(i+1)
            if m>low[p] and m<high[p]:
                point =p
        if point ==-1:
            print 'Problem in reweighting for control region'

        for j in range(1,histo.GetNbinsY()+1):
            oldTempValue = histo.GetBinContent(i,j)
            newTempValue = oldTempValue*(slope[point]*j/30.+yIntr[point])
            tempUp.SetBinContent(i,j,newTempValue)
            newTempValue = oldTempValue*(-slope[point]*j/30.+2.-yIntr[point])
            tempDwn.SetBinContent(i,j,newTempValue)
        normalize(tempUp)
        normalize(tempDwn)
    return tempUp,tempDwn


          

if __name__ == '__main__':

    parser = optparse.OptionParser()
    parser.add_option("-c","--cut",dest="cut",default='H_Mass>100',help="cut to apply")
    parser.add_option("-m","--max",dest="maxMass",default='160',help="max mass")
    parser.add_option("-r","--runOnly",dest="runOnly",default='',help="runonly")
    (options,args) = parser.parse_args()

    setup =HZZSetup()

    f=ROOT.TFile('template_MELA.root','RECREATE')
    f.cd()


    #define the binning
    binningx=[]
    value=100
    while value<1600:
        binningx.append(value)
        if value<300:
            value=value+2
        elif value<350:
            value=value+5
        elif value<500:
            value=value+15
        elif value<600:
            value=value+20
        elif value<800:
            value=value+30
        elif value<1000:
            value=value+50
        elif value<1200:
            value=value+60
        else:
            value=value+100

    value=0
    binningy=[]
    while value<1:
        binningy.append(value)
        value=value+1./30.
        
    masses7TeV =[]
    masses8TeV=[]

    pname={'ggH':'ggH','qqH':'qqH','ZH':'ggH','WH':'ggH','ttH':'ggH'}

    cut = {'MuMu':'&&H_Z1_leg1_PdgId==13&&H_Z2_leg1_PdgId==13',\
           'MuEle':'&&H_Z1_leg1_PdgId==13&&H_Z2_leg1_PdgId==11',\
           'EleEle':'&&H_Z1_leg1_PdgId==11&&H_Z2_leg1_PdgId==11',\
           'EleMu':'&&H_Z1_leg1_PdgId==11&&H_Z2_leg1_PdgId==13'}
           


    for period in ['7TeV','8TeV']:
        for prod in ['ggH','qqH','ZH','WH','ttH']:
            masses7TeV=[]
            masses8TeV=[]
            for filename in os.listdir('All_7TeV'):
                name,extention=filename.split('.')
                if name.find(pname[prod])>-1 and extention == 'root':
                    s=name.split(pname[prod])
                    if int(s[1])<int(options.maxMass):
                        masses7TeV.append(int(s[1]))
            for filename in os.listdir('All_8TeV'):
                name,extention=filename.split('.')
                if name.find(pname[prod])>-1 and extention == 'root':
                    s=name.split(pname[prod])
                    if int(s[1])<int(options.maxMass):
                        masses8TeV.append(int(s[1]))
            if len(options.runOnly)>1:
                masses7TeV=[]
                masses8TeV=[]
                masses=options.runOnly.split(',')
                for m in masses:
                    masses7TeV.append(int(m))
                    masses8TeV.append(int(m))

            print prod,'--------------------'        
            print masses7TeV,masses8TeV
            print '--------------------'        
        
            for finalstate in ['MuMu','MuEle','EleEle','EleMu']:
                plotters=[]
                for m in masses7TeV:
                    plotter=TreePlotter("All_7TeV/"+pname[prod]+str(m)+".root","FourLeptonTreeProducer/tree",'1')
                    plotter.addCorrectionFactor('eff','H_eff',0.0,'lnN')
                    plotters.append(plotter)
                for m in masses8TeV:
                    plotter=TreePlotter("All_8TeV/"+pname[prod]+str(m)+".root","FourLeptonTreeProducer/tree",'1')
                    plotter.addCorrectionFactor('eff','H_eff',0.0,'lnN')
                    plotters.append(plotter)
                print 'Plotters',plotters
                p=MergedPlotter(plotters)
                h=p.makeTemplateHistBinned('H_MELA:H_Mass',options.cut+cut[finalstate],binningx,binningy,0)
                if finalstate not in  ['MuEle','EleMu']:
                    reweightForInterference(h)
                normalize(h)
                h.SetName(finalstate+'_'+prod+'_'+period)
                f.cd()
                h.Write()


        for prod in ['QQZZ','GGZZ']:
            for finalstate in ['MuMu','MuEle','EleEle','EleMu']:
                plotter,plotters7TeV=setup.getZZPlotters(finalstate,prod,'7TeV')
                plotter,plotters8TeV=setup.getZZPlotters(finalstate,prod,'8TeV')
                plotters=plotters7TeV+plotters8TeV
                p=MergedPlotter(plotters)
                h=p.makeTemplateHistBinned('H_MELA:H_Mass',options.cut+cut[finalstate],binningx,binningy,0)
                hUp,hDwn = reweightForCR(h)
                normalize(h)

                h.SetName(finalstate+'_'+prod+'_'+period)
                hUp.SetName(finalstate+'_'+prod+'_'+period+'_CR_Up')
                hDwn.SetName(finalstate+'_'+prod+'_'+period+'_CR_Down')
                f.cd()
                h.Write()
                hUp.Write()
                hDwn.Write()
                if prod =='QQZZ':
                    h.SetName(finalstate+'_FAKES_'+period)
                    hUp.SetName(finalstate+'_FAKES_'+period+'_CR_Up')
                    hDwn.SetName(finalstate+'_FAKES_'+period+'_CR_Down')

                    f.cd()
                    h.Write()
                    hUp.Write()
                    hDwn.Write()
    f.Close()        


    
