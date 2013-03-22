import imp
import os
import math
from ROOT import gDirectory
from CMGTools.RootTools.RootInit import *
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.plotter.H2TauTauDataMC_diTau import *

def saveForLimit(TightIsoOS, prefixLabel, mass, massType, category, susy):

    TightIsoOS.Hist('WW').Add(TightIsoOS.Hist('ZZ'))
    TightIsoOS.Hist('WW').Add(TightIsoOS.Hist('WZ'))

    noscomp = {
        'DYJets'            :'ZTT',
        'DYJets_Electron'   :'ZL',
        'DYJets_Fakes'      :'ZJ',
        'WJets'             :'W',
        'TTJets'            :'TT',
        'WW'                :'VV',
        #'ZZ'                :'',
        #'WZ'                :'',
        'QCDdata'           :'QCD',
        'Data'              :'data_obs'        
        }        

    if susy :
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

    fileName = '/'.join([os.getcwd(),prefixLabel,prefixLabel+'_tauTau_'+category+'_'+TightIsoOS.varName+'.root'])

    if TightIsoOS.varName == massType+'' :
    
     if not os.path.isfile(fileName) :
       rootfile = TFile(fileName,'recreate')
       channel  = rootfile.mkdir('tauTau_'+category)
       for comp in allcomp.keys() :
         TightIsoOS.Hist(comp).weighted.SetName(allcomp[comp])
         channel.Add(TightIsoOS.Hist(comp).weighted)
       channel.Write()
       
     else : 
       rootfile = TFile(fileName,'update')
       gDirectory.cd('tauTau_'+category)
       for comp in sigcomp.keys() :
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
        #'ZZ'                :'',
        #'WZ'                :'',
        'QCDdata'           :'QCD',
        'Data'              :'data_obs'        
        }        

    if susy :
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







#     TightIsoOS.Hist(str('HiggsGGH' +str(mass))).weighted.SetName(str('ggH' +str(mass)))
#     TightIsoOS.Hist(str('HiggsVBF'+str(mass))).weighted.SetName(str('qqH' +str(mass)))
#     TightIsoOS.Hist(str('HiggsVH' +str(mass))).weighted.SetName(str('VH' +str(mass)))
#     #TightIsoOS.Hist('DYJets').Add(TightIsoOS.Hist('DYJets_Photon'))
#     TightIsoOS.Hist('DYJets').weighted.SetName('ZTT')
#     TightIsoOS.Hist('DYJets_Electron').weighted.SetName('ZL')
#     TightIsoOS.Hist('DYJets_Fakes').weighted.SetName('ZJ')
#     #TightIsoOS.Hist('WJets').Add(TightIsoOS.Hist('WJets_Fakes'))
#     TightIsoOS.Hist('WJets').weighted.SetName('W')
#     TightIsoOS.Hist('TTJets').weighted.SetName('TT')
#     TightIsoOS.Hist('T_tW').Add(TightIsoOS.Hist('Tbar_tW'))
#     TightIsoOS.Hist('T_tW').weighted.SetName('T')
#     TightIsoOS.Hist('WW').Add(TightIsoOS.Hist('ZZ'))
#     TightIsoOS.Hist('WW').Add(TightIsoOS.Hist('WZ'))
#     TightIsoOS.Hist('WW').weighted.SetName('VV')
#     TightIsoOS.Hist('QCDdata').weighted.SetName('QCD')
#     TightIsoOS.Hist('Data').weighted.SetName('data_obs')
#     channel.Add(TightIsoOS.Hist(str('HiggsGGH' +str(mass))).weighted)
#     channel.Add(TightIsoOS.Hist(str('HiggsVBF'+str(mass))).weighted)
#     channel.Add(TightIsoOS.Hist(str('HiggsVH' +str(mass))).weighted)
#     channel.Add(TightIsoOS.Hist('DYJets').weighted)
#     channel.Add(TightIsoOS.Hist('DYJets_Electron').weighted)
#     channel.Add(TightIsoOS.Hist('DYJets_Fakes').weighted)
#     channel.Add(TightIsoOS.Hist('WJets').weighted)
#     #channel.Add(TightIsoOS.Hist('W3Jets').weighted)
#     channel.Add(TightIsoOS.Hist('TTJets').weighted)
#     channel.Add(TightIsoOS.Hist('T_tW').weighted)
#     channel.Add(TightIsoOS.Hist('WW').weighted)
#     channel.Add(TightIsoOS.Hist('QCDdata').weighted)
#     channel.Add(TightIsoOS.Hist('Data').weighted)
# 
#     channel.Write()
#     rootfile.Close()

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
