import imp
import os
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.H2TauTauDataMC_diTau import *
from CMGTools.RootTools.RootInit import *
import math


def saveForLimit(plotVarDataOS, prefixLabel, mass, massType, category):
    rootfile=TFile(os.getcwd()+"/"+prefixLabel+"/"+prefixLabel+"_tauTau_"+category+"_mH"+str(mass)+"_"+plotVarDataOS.varName+".root","RECREATE")
    channel=rootfile.mkdir("tauTau_"+category)
    
      
    
    if plotVarDataOS.varName == massType+"" :
		plotVarDataOS.Hist(str("Higgsgg" +str(mass))).weighted.SetName(str("SM" +str(mass)))
		plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted.SetName(str("VBF"+str(mass)))
		plotVarDataOS.Hist(str("HiggsVH" +str(mass))).weighted.SetName(str("VH" +str(mass)))
		#plotVarDataOS.Hist("DYJets").Add(plotVarDataOS.Hist("DYJets_Photon"))
		plotVarDataOS.Hist("DYJets").weighted.SetName("ZTT")
		plotVarDataOS.Hist("DYJets_Electron").weighted.SetName("ZL")
		plotVarDataOS.Hist("DYJets_Fakes").weighted.SetName("ZJ")
		if prefixLabel.find('BOOSTED') > 0 :
		  plotVarDataOS.Hist("WJets").Add(plotVarDataOS.Hist("WJets_Fakes"))
		  plotVarDataOS.Hist("WJets").weighted.SetName("W")
		if prefixLabel.find('VBF') > 0 :
		  plotVarDataOS.Hist("W3Jets").weighted.SetName("W")
		plotVarDataOS.Hist("TTJets").weighted.SetName("TT")
		plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("ZZ"))
		plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("WZ"))
		plotVarDataOS.Hist("WW").weighted.SetName("VV")
		plotVarDataOS.Hist("QCDdata").weighted.SetName("QCD")
		plotVarDataOS.Hist("Data").weighted.SetName("data_obs")
		channel.Add(plotVarDataOS.Hist(str("Higgsgg"+str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist(str("HiggsVH" +str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist("DYJets").weighted)
		channel.Add(plotVarDataOS.Hist("DYJets_Electron").weighted)
		channel.Add(plotVarDataOS.Hist("DYJets_Fakes").weighted)
		if prefixLabel.find('BOOSTED') > 0 :
		  channel.Add(plotVarDataOS.Hist("WJets").weighted)
		if prefixLabel.find('VBF') > 0 :
		  channel.Add(plotVarDataOS.Hist("W3Jets").weighted)
		channel.Add(plotVarDataOS.Hist("TTJets").weighted)
		channel.Add(plotVarDataOS.Hist("WW").weighted)
		channel.Add(plotVarDataOS.Hist("QCDdata").weighted)
		channel.Add(plotVarDataOS.Hist("Data").weighted)

    if plotVarDataOS.varName == massType+"*1.03" :
		plotVarDataOS.Hist(str("Higgsgg" +str(mass))).weighted.SetName(str("SM" +str(mass)+"_CMS_scale_tUp"))
		plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted.SetName(str("VBF"+str(mass)+"_CMS_scale_tUp"))
		plotVarDataOS.Hist(str("HiggsVH" +str(mass))).weighted.SetName(str("VH" +str(mass)+"_CMS_scale_tUp"))
		#plotVarDataOS.Hist("DYJets").Add(plotVarDataOS.Hist("DYJets_Photon"))
		plotVarDataOS.Hist("DYJets").weighted.SetName("ZTT_CMS_scale_tUp")
		plotVarDataOS.Hist("DYJets_Electron").weighted.SetName("ZL_CMS_scale_tUp")
		plotVarDataOS.Hist("DYJets_Fakes").weighted.SetName("ZJ_CMS_scale_tUp")
		if prefixLabel.find('BOOSTED') > 0 :
		  plotVarDataOS.Hist("WJets").Add(plotVarDataOS.Hist("WJets_Fakes"))
		  plotVarDataOS.Hist("WJets").weighted.SetName("W_CMS_scale_tUp")
		if prefixLabel.find('VBF') > 0 :
		  plotVarDataOS.Hist("W3Jets").weighted.SetName("W_CMS_scale_tUp")
		plotVarDataOS.Hist("TTJets").weighted.SetName("TT_CMS_scale_tUp")
		plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("ZZ"))
		plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("WZ"))
		plotVarDataOS.Hist("WW").weighted.SetName("VV_CMS_scale_tUp")
		channel.Add(plotVarDataOS.Hist(str("Higgsgg" +str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist(str("HiggsVH" +str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist("DYJets").weighted)
		channel.Add(plotVarDataOS.Hist("DYJets_Electron").weighted)
		channel.Add(plotVarDataOS.Hist("DYJets_Fakes").weighted)
		if prefixLabel.find('BOOSTED') > 0 :
		  channel.Add(plotVarDataOS.Hist("WJets").weighted)
		if prefixLabel.find('VBF') > 0 :
		  channel.Add(plotVarDataOS.Hist("W3Jets").weighted)
		channel.Add(plotVarDataOS.Hist("TTJets").weighted)
		channel.Add(plotVarDataOS.Hist("WW").weighted)

    if plotVarDataOS.varName == massType+"*0.97" :
		plotVarDataOS.Hist(str("Higgsgg" +str(mass))).weighted.SetName(str("SM" +str(mass)+"_CMS_scale_tDown"))
		plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted.SetName(str("VBF"+str(mass)+"_CMS_scale_tDown"))
		plotVarDataOS.Hist(str("HiggsVH" +str(mass))).weighted.SetName(str("VH" +str(mass)+"_CMS_scale_tDown"))
		#plotVarDataOS.Hist("DYJets").Add(plotVarDataOS.Hist("DYJets_Photon"))
		plotVarDataOS.Hist("DYJets").weighted.SetName("ZTT_CMS_scale_tDown")
		plotVarDataOS.Hist("DYJets_Electron").weighted.SetName("ZL_CMS_scale_tDown")
		plotVarDataOS.Hist("DYJets_Fakes").weighted.SetName("ZJ_CMS_scale_tDown")
		if prefixLabel.find('BOOSTED') > 0 :
		  plotVarDataOS.Hist("WJets").Add(plotVarDataOS.Hist("WJets_Fakes"))
		  plotVarDataOS.Hist("WJets").weighted.SetName("W_CMS_scale_tDown")
		if prefixLabel.find('VBF') > 0 :
		  plotVarDataOS.Hist("W3Jets").weighted.SetName("W_CMS_scale_tDown")
		plotVarDataOS.Hist("TTJets").weighted.SetName("TT_CMS_scale_tDown")
		plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("ZZ"))
		plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("WZ"))
		plotVarDataOS.Hist("WW").weighted.SetName("VV_CMS_scale_tDown")
		channel.Add(plotVarDataOS.Hist(str("Higgsgg" +str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist(str("HiggsVH" +str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist("DYJets").weighted)
		channel.Add(plotVarDataOS.Hist("DYJets_Electron").weighted)
		channel.Add(plotVarDataOS.Hist("DYJets_Fakes").weighted)
		if prefixLabel.find('BOOSTED') > 0 :
		  channel.Add(plotVarDataOS.Hist("WJets").weighted)
		if prefixLabel.find('VBF') > 0 :
		  channel.Add(plotVarDataOS.Hist("W3Jets").weighted)
		channel.Add(plotVarDataOS.Hist("TTJets").weighted)
		channel.Add(plotVarDataOS.Hist("WW").weighted)

    channel.Write()
    rootfile.Close()

def saveForPlotting(plotVarDataOS, prefixLabel, mass):
    rootfile=TFile(os.getcwd()+"/"+prefixLabel+"/"+prefixLabel+"_tauTau_mH"+str(mass)+"_PLOT.root","UPDATE")
    channel=rootfile.mkdir(str(plotVarDataOS.varName))
    rootfile.cd(str(plotVarDataOS.varName))

    plotVarDataOS.Hist(str("Higgsgg" +str(mass))).weighted.SetName(str("SM" +str(mass)))
    plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted.SetName(str("VBF"+str(mass)))
    plotVarDataOS.Hist(str("HiggsVH" +str(mass))).weighted.SetName(str("VH" +str(mass)))
    #plotVarDataOS.Hist("DYJets").Add(plotVarDataOS.Hist("DYJets_Photon"))
    plotVarDataOS.Hist("DYJets").weighted.SetName("ZTT")
    plotVarDataOS.Hist("DYJets_Electron").weighted.SetName("ZL")
    plotVarDataOS.Hist("DYJets_Fakes").weighted.SetName("ZJ")
    plotVarDataOS.Hist("WJets").Add(plotVarDataOS.Hist("WJets_Fakes"))
    plotVarDataOS.Hist("WJets").weighted.SetName("W")
    plotVarDataOS.Hist("W3Jets").weighted.SetName("W3")
    plotVarDataOS.Hist("TTJets").weighted.SetName("TT")
    plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("ZZ"))
    plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("WZ"))
    plotVarDataOS.Hist("WW").weighted.SetName("VV")
    plotVarDataOS.Hist("QCDdata").weighted.SetName("QCD")
    plotVarDataOS.Hist("Data").weighted.SetName("data_obs")
    channel.Add(plotVarDataOS.Hist(str("Higgsgg" +str(mass))).weighted)
    channel.Add(plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted)
    channel.Add(plotVarDataOS.Hist(str("HiggsVH" +str(mass))).weighted)
    channel.Add(plotVarDataOS.Hist("DYJets").weighted)
    channel.Add(plotVarDataOS.Hist("DYJets_Electron").weighted)
    channel.Add(plotVarDataOS.Hist("DYJets_Fakes").weighted)
    channel.Add(plotVarDataOS.Hist("WJets").weighted)
    channel.Add(plotVarDataOS.Hist("W3Jets").weighted)
    channel.Add(plotVarDataOS.Hist("TTJets").weighted)
    channel.Add(plotVarDataOS.Hist("WW").weighted)
    channel.Add(plotVarDataOS.Hist("QCDdata").weighted)
    channel.Add(plotVarDataOS.Hist("Data").weighted)

    channel.Write()
    rootfile.Close()

def saveQCD(QCDlooseOS, QCDlooseSS, QCDtightSS, var, prefixLabel, mass, corr=False):

    if corr :
      rootfile = TFile(os.getcwd()+"/"+prefixLabel+"/"+prefixLabel+"_CORRELATIONS_QCD_TauTau_mH"+str(mass)+"_PLOT.root","UPDATE")      
    else :
      rootfile = TFile(os.getcwd()+"/"+prefixLabel+"/"+prefixLabel+"_QCD_TauTau_mH"+str(mass)+"_PLOT.root","UPDATE")
    
    channel = rootfile.mkdir(str(var))
    rootfile.cd(str(var))
    
    names = {QCDlooseOS:'QCDlooseOS', QCDlooseSS:'QCDlooseSS', QCDtightSS:'QCDtightSS'}
    
#     for sample in [QCDlooseOS, QCDlooseSS, QCDtightSS]:
#       sample.weighted.SetName(names[sample])
#       sample.weighted.Write()      
#       rootfile.cd('..')
      
    QCDlooseOS.weighted.SetName('QCDlooseOS')
    QCDlooseSS.weighted.SetName('QCDlooseSS')
    QCDtightSS.weighted.SetName('QCDtightSS')

    QCDlooseOS.weighted.Write()
    QCDlooseSS.weighted.Write()
    QCDtightSS.weighted.Write()
    
    ratio = QCDtightSS.weighted.Clone()
    
    num   = QCDtightSS.weighted.Clone()
    den   = QCDlooseSS.weighted.Clone()
    
    scale = num.Integral() / den.Integral()
    
    ratio.Divide(num,den,1,scale)
    ratio.SetName('QCDtightSS_QCDlooseSS_ratio')

    ratio.Write()      
    
    rootfile.cd('..')
    
    rootfile.Close()


def saveForWeighting(plotVarDataOS, plotVarDataSS, plotVarDataLooseIsoSS, plotVarDataLooseIsoOS, prefixLabel, mass):

    rootfile    = TFile(os.getcwd()+"/"+prefixLabel+"/"+prefixLabel+"_tauTau_mH"+str(mass)+"_WEIGHTING_PLOT.root","UPDATE")
    channelOST  = rootfile.mkdir(str(plotVarDataOS.varName)+'_tightOS')
    rootfile.cd(str(plotVarDataOS.varName)+'_tightOS')

    plotVarDataOS.Hist(str("Higgsgg" +str(mass))).weighted.SetName(str("SM" +str(mass)))
    plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted.SetName(str("VBF"+str(mass)))
    plotVarDataOS.Hist(str("HiggsVH" +str(mass))).weighted.SetName(str("VH" +str(mass)))
    plotVarDataOS.Hist("DYJets").weighted.SetName("ZTT")
    plotVarDataOS.Hist("DYJets_Electron").weighted.SetName("ZL")
    plotVarDataOS.Hist("DYJets_Fakes").weighted.SetName("ZJ")
    plotVarDataOS.Hist("WJets").Add(plotVarDataOS.Hist("WJets_Fakes"))
    plotVarDataOS.Hist("WJets").weighted.SetName("W")
    plotVarDataOS.Hist("W3Jets").weighted.SetName("W3")
    plotVarDataOS.Hist("TTJets").weighted.SetName("TT")
    plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("ZZ"))
    plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("WZ"))
    plotVarDataOS.Hist("WW").weighted.SetName("VV")
    plotVarDataOS.Hist("QCDdata").weighted.SetName("QCD")
    plotVarDataOS.Hist("Data").weighted.SetName("data_obs")
    channelOST.Add(plotVarDataOS.Hist(str("Higgsgg" +str(mass))).weighted)
    channelOST.Add(plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted)
    channelOST.Add(plotVarDataOS.Hist(str("HiggsVH" +str(mass))).weighted)
    channelOST.Add(plotVarDataOS.Hist("DYJets").weighted)
    channelOST.Add(plotVarDataOS.Hist("DYJets_Electron").weighted)
    channelOST.Add(plotVarDataOS.Hist("DYJets_Fakes").weighted)
    channelOST.Add(plotVarDataOS.Hist("WJets").weighted)
    channelOST.Add(plotVarDataOS.Hist("W3Jets").weighted)
    channelOST.Add(plotVarDataOS.Hist("TTJets").weighted)
    channelOST.Add(plotVarDataOS.Hist("WW").weighted)
    channelOST.Add(plotVarDataOS.Hist("QCDdata").weighted)
    channelOST.Add(plotVarDataOS.Hist("Data").weighted)

    channelOST.Write()
    
    rootfile.cd('..')
    channelSST  = rootfile.mkdir(str(plotVarDataSS.varName)+'_tightSS')
    rootfile.cd(str(plotVarDataSS.varName)+'_tightSS')

    #plotVarDataSS.Hist(str("Higgsgg" +str(mass))).weighted.SetName(str("SM" +str(mass)))
    #plotVarDataSS.Hist(str("HiggsVBF"+str(mass))).weighted.SetName(str("VBF"+str(mass)))
    #plotVarDataSS.Hist(str("HiggsVH" +str(mass))).weighted.SetName(str("VH" +str(mass)))
    plotVarDataSS.Hist("DYJets").weighted.SetName("ZTT")
    plotVarDataSS.Hist("DYJets_Electron").weighted.SetName("ZL")
    plotVarDataSS.Hist("DYJets_Fakes").weighted.SetName("ZJ")
    plotVarDataSS.Hist("WJets").Add(plotVarDataSS.Hist("WJets_Fakes"))
    plotVarDataSS.Hist("WJets").weighted.SetName("W")
    plotVarDataSS.Hist("W3Jets").weighted.SetName("W3")
    plotVarDataSS.Hist("TTJets").weighted.SetName("TT")
    plotVarDataSS.Hist("WW").Add(plotVarDataSS.Hist("ZZ"))
    plotVarDataSS.Hist("WW").Add(plotVarDataSS.Hist("WZ"))
    plotVarDataSS.Hist("WW").weighted.SetName("VV")
    #plotVarDataSS.Hist("QCDdata").weighted.SetName("QCD")
    plotVarDataSS.Hist("Data").weighted.SetName("data_obs")
    #channelSST.Add(plotVarDataSS.Hist(str("Higgsgg" +str(mass))).weighted)
    #channelSST.Add(plotVarDataSS.Hist(str("HiggsVBF"+str(mass))).weighted)
    #channelSST.Add(plotVarDataSS.Hist(str("HiggsVH" +str(mass))).weighted)
    channelSST.Add(plotVarDataSS.Hist("DYJets").weighted)
    channelSST.Add(plotVarDataSS.Hist("DYJets_Electron").weighted)
    channelSST.Add(plotVarDataSS.Hist("DYJets_Fakes").weighted)
    channelSST.Add(plotVarDataSS.Hist("WJets").weighted)
    channelSST.Add(plotVarDataSS.Hist("W3Jets").weighted)
    channelSST.Add(plotVarDataSS.Hist("TTJets").weighted)
    channelSST.Add(plotVarDataSS.Hist("WW").weighted)
    #channelSST.Add(plotVarDataSS.Hist("QCDdata").weighted)
    channelSST.Add(plotVarDataSS.Hist("Data").weighted)

    channelSST.Write()


    rootfile.cd('..')
    channelSSL  = rootfile.mkdir(str(plotVarDataLooseIsoSS.varName)+'_looseSS')
    rootfile.cd(str(plotVarDataLooseIsoSS.varName)+'_looseSS')

    #plotVarDataLooseIsoSS.Hist(str("Higgsgg" +str(mass))).weighted.SetName(str("SM" +str(mass)))
    #plotVarDataLooseIsoSS.Hist(str("HiggsVBF"+str(mass))).weighted.SetName(str("VBF"+str(mass)))
    #plotVarDataLooseIsoSS.Hist(str("HiggsVH" +str(mass))).weighted.SetName(str("VH" +str(mass)))
    plotVarDataLooseIsoSS.Hist("DYJets").weighted.SetName("ZTT")
    plotVarDataLooseIsoSS.Hist("DYJets_Electron").weighted.SetName("ZL")
    plotVarDataLooseIsoSS.Hist("DYJets_Fakes").weighted.SetName("ZJ")
    plotVarDataLooseIsoSS.Hist("WJets").Add(plotVarDataLooseIsoSS.Hist("WJets_Fakes"))
    plotVarDataLooseIsoSS.Hist("WJets").weighted.SetName("W")
    plotVarDataLooseIsoSS.Hist("W3Jets").weighted.SetName("W3")
    plotVarDataLooseIsoSS.Hist("TTJets").weighted.SetName("TT")
    plotVarDataLooseIsoSS.Hist("WW").Add(plotVarDataLooseIsoSS.Hist("ZZ"))
    plotVarDataLooseIsoSS.Hist("WW").Add(plotVarDataLooseIsoSS.Hist("WZ"))
    plotVarDataLooseIsoSS.Hist("WW").weighted.SetName("VV")
    #plotVarDataLooseIsoSS.Hist("QCDdata").weighted.SetName("QCD")
    plotVarDataLooseIsoSS.Hist("Data").weighted.SetName("data_obs")
    #channelSSL.Add(plotVarDataLooseIsoSS.Hist(str("Higgsgg" +str(mass))).weighted)
    #channelSSL.Add(plotVarDataLooseIsoSS.Hist(str("HiggsVBF"+str(mass))).weighted)
    #channelSSL.Add(plotVarDataLooseIsoSS.Hist(str("HiggsVH" +str(mass))).weighted)
    channelSSL.Add(plotVarDataLooseIsoSS.Hist("DYJets").weighted)
    channelSSL.Add(plotVarDataLooseIsoSS.Hist("DYJets_Electron").weighted)
    channelSSL.Add(plotVarDataLooseIsoSS.Hist("DYJets_Fakes").weighted)
    channelSSL.Add(plotVarDataLooseIsoSS.Hist("WJets").weighted)
    channelSSL.Add(plotVarDataLooseIsoSS.Hist("W3Jets").weighted)
    channelSSL.Add(plotVarDataLooseIsoSS.Hist("TTJets").weighted)
    channelSSL.Add(plotVarDataLooseIsoSS.Hist("WW").weighted)
    #channelSSL.Add(plotVarDataLooseIsoSS.Hist("QCDdata").weighted)
    channelSSL.Add(plotVarDataLooseIsoSS.Hist("Data").weighted)

    channelSSL.Write()


    rootfile.cd('..')
    channelOSL  = rootfile.mkdir(str(plotVarDataLooseIsoOS.varName)+'_looseOS')
    rootfile.cd(str(plotVarDataLooseIsoOS.varName)+'_looseOS')

    #plotVarDataLooseIsoOS.Hist(str("Higgsgg" +str(mass))).weighted.SetName(str("SM" +str(mass)))
    #plotVarDataLooseIsoOS.Hist(str("HiggsVBF"+str(mass))).weighted.SetName(str("VBF"+str(mass)))
    #plotVarDataLooseIsoOS.Hist(str("HiggsVH" +str(mass))).weighted.SetName(str("VH" +str(mass)))
    plotVarDataLooseIsoOS.Hist("DYJets").weighted.SetName("ZTT")
    plotVarDataLooseIsoOS.Hist("DYJets_Electron").weighted.SetName("ZL")
    plotVarDataLooseIsoOS.Hist("DYJets_Fakes").weighted.SetName("ZJ")
    plotVarDataLooseIsoOS.Hist("WJets").Add(plotVarDataLooseIsoOS.Hist("WJets_Fakes"))
    plotVarDataLooseIsoOS.Hist("WJets").weighted.SetName("W")
    plotVarDataLooseIsoOS.Hist("W3Jets").weighted.SetName("W3")
    plotVarDataLooseIsoOS.Hist("TTJets").weighted.SetName("TT")
    plotVarDataLooseIsoOS.Hist("WW").Add(plotVarDataLooseIsoOS.Hist("ZZ"))
    plotVarDataLooseIsoOS.Hist("WW").Add(plotVarDataLooseIsoOS.Hist("WZ"))
    plotVarDataLooseIsoOS.Hist("WW").weighted.SetName("VV")
    #plotVarDataLooseIsoOS.Hist("QCDdata").weighted.SetName("QCD")
    plotVarDataLooseIsoOS.Hist("Data").weighted.SetName("data_obs")
    #channelOSL.Add(plotVarDataLooseIsoOS.Hist(str("Higgsgg" +str(mass))).weighted)
    #channelOSL.Add(plotVarDataLooseIsoOS.Hist(str("HiggsVBF"+str(mass))).weighted)
    #channelOSL.Add(plotVarDataLooseIsoOS.Hist(str("HiggsVH" +str(mass))).weighted)
    channelOSL.Add(plotVarDataLooseIsoOS.Hist("DYJets").weighted)
    channelOSL.Add(plotVarDataLooseIsoOS.Hist("DYJets_Electron").weighted)
    channelOSL.Add(plotVarDataLooseIsoOS.Hist("DYJets_Fakes").weighted)
    channelOSL.Add(plotVarDataLooseIsoOS.Hist("WJets").weighted)
    channelOSL.Add(plotVarDataLooseIsoOS.Hist("W3Jets").weighted)
    channelOSL.Add(plotVarDataLooseIsoOS.Hist("TTJets").weighted)
    channelOSL.Add(plotVarDataLooseIsoOS.Hist("WW").weighted)
    #channelOSL.Add(plotVarDataLooseIsoOS.Hist("QCDdata").weighted)
    channelOSL.Add(plotVarDataLooseIsoOS.Hist("Data").weighted)

    channelOSL.Write()

    rootfile.Close()

