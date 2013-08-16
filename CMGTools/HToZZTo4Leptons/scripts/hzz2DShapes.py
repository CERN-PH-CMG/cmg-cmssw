#!/usr/bin/env python
import optparse
import os

from CMGTools.HToZZTo4Leptons.macros.hzzSetup import *


            
def normalize(h):
    for binx in range(1,h.GetNbinsX()+1):
        sumy=h.ProjectionY('px',binx,binx).Integral()
        if sumy>0:
            for biny in range(1,h.GetNbinsY()+1):
                h.SetBinContent(binx,biny,h.GetBinContent(binx,biny)/sumy)
                h.SetBinError(binx,biny,h.GetBinError(binx,biny)/sumy)

          

if __name__ == '__main__':

    parser = optparse.OptionParser()
    parser.add_option("-c","--cut",dest="cut",default='H_Mass>100&&H_NJets>=2',help="cut to apply")
    parser.add_option("-v","--var",dest="var",default='H_Fisher:H_Mass',help="cut to apply")
    parser.add_option("-m","--max",dest="maxMass",type='float',default='160',help="max mass")
    parser.add_option("-r","--runOnly",dest="runOnly",default='',help="runonly")
    parser.add_option("-n","--name",dest="name",default='Fisher',help="runonly")
    parser.add_option("-b","--binningX",dest="binningX",default='100,110,120,130,140,150,160,180,200,250,300,350,400,500,600,700,800,1000,1500',help="binningX")
    parser.add_option("-B","--binningY",dest="binningY",default='0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,1.0,2.0',help="binningX")
    (options,args) = parser.parse_args()

    setup =HZZSetup()

    f=ROOT.TFile('template_'+options.name+'.root','RECREATE')
    f.cd()


    #define the binning
    binningx=[]
    for comp in options.binningX.split(','):
        binningx.append(float(comp))

    binningy=[]
    for comp in options.binningY.split(','):
        binningy.append(float(comp))

        
    masses7TeV =[]
    masses8TeV=[]

    pname={'ggH':'ggH','qqH':'qqH','ZH':'VH','WH':'WH','ttH':'ttH'}
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
        
        for finalstate in ['MuMu','MuEle','EleEle']:
            plotters=[]
            for m in masses7TeV:
                plotter=TreePlotter("All_7TeV/"+pname[prod]+str(m)+".root","FourLeptonTreeProducer/tree",'1')
                plotter.addCorrectionFactor('eff','H_eff',0.0,'lnN')
                plotters.append(plotter)
            for m in masses8TeV:
                plotter=TreePlotter("All_8TeV/"+pname[prod]+str(m)+".root","FourLeptonTreeProducer/tree",'1')
                plotter.addCorrectionFactor('eff','H_eff',0.0,'lnN')

            print 'Plotters',plotters
            p=MergedPlotter(plotters)
            h=p.makeTemplateHistBinned(options.var,options.cut,binningx,binningy,0)
            normalize(h)
            h.SetName(finalstate+'_'+prod)
            f.cd()
            h.Write()


    for prod in ['QQZZ','GGZZ']:
        for finalstate in ['MuMu','MuEle','EleEle']:
            plotter,plotters7TeV=setup.getZZPlotters(finalstate,prod,'7TeV')
            plotter,plotters8TeV=setup.getZZPlotters(finalstate,prod,'8TeV')
            plotters=plotters7TeV+plotters8TeV
            p=MergedPlotter(plotters)
            h=p.makeTemplateHistBinned(options.var,options.cut,binningx,binningy,0)
            h.SetName(finalstate+'_'+prod)
            #######MAKE A TRICK TO USE THE ABSENSE OF CORRELATIONS 
            h1D = p.makeTemplateHistBinned((options.var).split(':')[0],options.cut,binningy,binningy,0)
            for bx in range(1,len(binningx)):
                for by in range(1,len(binningy)):
                    h.SetBinContent(bx,by,h1D.GetBinContent(by))
                
            normalize(h)
            f.cd()
            h.Write()
            h.SetName(finalstate+'_FAKES')
            f.cd()
            h.Write()


#    for prod in ['FAKES']:
#        for finalstate in ['MuMu','MuEle','EleEle']:
#            plotter=setup.getFakePlotter(finalstate,prod,'7TeV')
#            plotters=plotters7TeV+plotters8TeV
#            p=MergedPlotter(plotters)
#            h=p.makeTemplateHistBinned(options.var,options.cut,binningx,binningy,0)
#            normalize(h)
#2#B            h.SetName(finalstate+'_'+prod)
#2#B            f.cd()
#            h.Write()

                
    f.Close()        


    
