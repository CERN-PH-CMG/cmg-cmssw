import imp
import os
import math
from ROOT import gDirectory
from CMGTools.RootTools.RootInit import *
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.plotter.H2TauTauDataMC_diTau import *

def saveForLimit(TightIsoOS, prefixLabel, mass, massType, fine_binning, category, susy):

    TightIsoOS.Hist('WW').Add(TightIsoOS.Hist('ZZ'))
    TightIsoOS.Hist('WW').Add(TightIsoOS.Hist('WZ'))

    noscomp = {
        'DYJets'            :'ZTT',
        'DYJets_Electron'   :'ZL',
        'DYJets_Fakes'      :'ZJ',
        'WJets'             :'W',
        'TTJets'            :'TT',
        'WW'                :'VV',
        'QCDdata'           :'QCD',
        'Data'              :'data_obs'        
        }        

    if susy :
      TightIsoOS.Hist('HiggsGGH125').Add(TightIsoOS.Hist('HiggsVBF125')) ## adding SM Higgs as a bkg
      TightIsoOS.Hist('HiggsGGH125').Add(TightIsoOS.Hist('HiggsVH125'))  ## adding SM Higgs as a bkg
      noscomp.update({'HiggsGGH125':'ggH_SM125+qqH_SM125+VH_SM125'})
      sigcomp = {
          'HiggsSUSYBB'    +str(mass):'bbH'+str(mass),
          'HiggsSUSYGluGlu'+str(mass):'ggH'+str(mass),
          }
    else :
      sigcomp = {
          'HiggsGGH'+str(mass):'ggH'+str(mass),
          'HiggsVBF'+str(mass):'qqH'+str(mass),
          'HiggsVH' +str(mass):'VH' +str(mass),        
          }

    allcomp = {}

    if fine_binning :
      fbnoscomp = {}
      fbsigcomp = {}
      for k in noscomp.keys() :
        fbnoscomp.update({k:noscomp[k]+'_fine_binning'})   
      for k in sigcomp.keys() :
        fbsigcomp.update({k:sigcomp[k]+'_fine_binning'})   
      allcomp.update(fbnoscomp)
      allcomp.update(fbsigcomp)
    else :
      allcomp.update(noscomp)
      allcomp.update(sigcomp)
              
    fileName = '/'.join([os.getcwd(),prefixLabel,prefixLabel+'_tauTau_'+category+'_'+TightIsoOS.varName+'.root'])

    if TightIsoOS.varName == massType :
     
      if not os.path.isfile(fileName) :
        rootfile = TFile(fileName,'recreate')
        channel  = rootfile.mkdir('tauTau_'+category)
        for comp in allcomp.keys() :
          TightIsoOS.Hist(comp).weighted.SetName(allcomp[comp])
          channel.Add(TightIsoOS.Hist(comp).weighted)
        channel.Write()
       
      else : 
        rootfile = TFile(fileName,'update')
        rootfile.cd('tauTau_'+category)
        
        alreadyIn = []
        dirList = gDirectory.GetListOfKeys()
        for k2 in dirList:
          h2 = k2.ReadObj()
          alreadyIn.append(h2.GetName())

        for comp in allcomp.keys() :
          if comp in alreadyIn : pass
          TightIsoOS.Hist(comp).weighted.SetName(allcomp[comp])
          TightIsoOS.Hist(comp).weighted.Write()
        gDirectory.cd('..')

    rootfile.Close()

def saveForPlotting(TightIsoOS, prefixLabel, mass, susy):

    TightIsoOS.Hist('WW').Add(TightIsoOS.Hist('ZZ'))
    TightIsoOS.Hist('WW').Add(TightIsoOS.Hist('WZ'))

    noscomp = {
        'DYJets'            :'ZTT',
        'DYJets_Electron'   :'ZL',
        'DYJets_Fakes'      :'ZJ',
        'WJets'             :'W',
        'TTJets'            :'TT',
        'WW'                :'VV',
        'QCDdata'           :'QCD',
        'Data'              :'data_obs'        
        }        

    if susy :
      TightIsoOS.Hist('HiggsGGH125').Add(TightIsoOS.Hist('HiggsVBF125')) ## adding SM Higgs as a bkg
      TightIsoOS.Hist('HiggsGGH125').Add(TightIsoOS.Hist('HiggsVH125'))  ## adding SM Higgs as a bkg
      noscomp.update({'HiggsGGH125':'ggH_SM125+qqH_SM125+VH_SM125'})
      sigcomp = {
          'HiggsSUSYBB'    +str(mass):'bbH'+str(mass),
          'HiggsSUSYGluGlu'+str(mass):'ggH'+str(mass),
          }
    else :
      sigcomp = {
          'HiggsGGH'+str(mass):'ggH'+str(mass),
          'HiggsVBF'+str(mass):'qqH'+str(mass),
          'HiggsVH' +str(mass):'VH' +str(mass),        
          }

    allcomp = {}
    allcomp.update(noscomp)
    allcomp.update(sigcomp)






    fileName = '/'.join([os.getcwd(),prefixLabel,prefixLabel+'_tauTau_mH'+str(mass)+'_PLOT.root'])

    rootfile = TFile(fileName,'UPDATE')
    channel  = rootfile.mkdir(str(TightIsoOS.varName))
    
    for comp in allcomp.keys() :
      TightIsoOS.Hist(comp).weighted.SetName(allcomp[comp])
      channel.Add(TightIsoOS.Hist(comp).weighted)
    channel.Write()
    rootfile.Close()

def saveQCD(QCDlooseOS, QCDlooseSS, QCDtightSS, var, sys, prefixLabel, mass, corr=False):

    if corr :
      rootfile = TFile(os.getcwd()+'/'+prefixLabel+'/'+prefixLabel+'_CORRELATIONS_QCD_TauTau_mH'+str(mass)+'_PLOT.root','UPDATE')      
    else :
      print 'working on ', os.getcwd()+'/'+prefixLabel+'/'+prefixLabel+sys+'_QCD_TauTau_mH'+str(mass)+'_PLOT.root'
      rootfile = TFile(os.getcwd()+'/'+prefixLabel+'/'+prefixLabel+sys+'_QCD_TauTau_mH'+str(mass)+'_PLOT.root','UPDATE')
    
    channel = rootfile.mkdir(str(var))
    rootfile.cd(str(var))
    
    names = {QCDlooseOS:'QCDlooseOS', QCDlooseSS:'QCDlooseSS', QCDtightSS:'QCDtightSS'}
          
    QCDlooseOS.weighted.SetName('QCDlooseOS')
    QCDlooseSS.weighted.SetName('QCDlooseSS')
    QCDtightSS.weighted.SetName('QCDtightSS')

    QCDlooseOS.weighted.Write()
    QCDlooseSS.weighted.Write()
    QCDtightSS.weighted.Write()
    
    ratio = QCDtightSS.weighted.Clone()
    
    num   = QCDtightSS.weighted.Clone()
    den   = QCDlooseSS.weighted.Clone()
    
    if den.Integral()>0:
      scale = num.Integral() / den.Integral()
    else:
      scale = 1
    
    ratio.Divide(num,den,1,scale)
    ratio.SetName('QCDtightSS_QCDlooseSS_ratio')

    ratio.Write()      
    
    rootfile.cd('..')
    
    rootfile.Close()
