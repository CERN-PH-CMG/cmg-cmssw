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
		plotVarDataOS.Hist(str("Higgsgg"+str(mass))).weighted.SetName(str("SM"+str(mass)))
		plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted.SetName(str("VBF"+str(mass)))
		#plotVarDataOS.Hist("DYJets").Add(plotVarDataOS.Hist("DYJets_Photon"))
		plotVarDataOS.Hist("DYJets").weighted.SetName("ZTT")
		plotVarDataOS.Hist("DYJets_Electron").weighted.SetName("ZL")
		plotVarDataOS.Hist("DYJets_Fakes").weighted.SetName("ZJ")
		plotVarDataOS.Hist("WJets").Add(plotVarDataOS.Hist("WJets_Fakes"))
		plotVarDataOS.Hist("WJets").weighted.SetName("W")
		plotVarDataOS.Hist("TTJets").weighted.SetName("TT")
		plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("ZZ"))
		plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("WZ"))
		plotVarDataOS.Hist("WW").weighted.SetName("diBosons")
		plotVarDataOS.Hist("QCDdata").weighted.SetName("QCD")
		plotVarDataOS.Hist("Data").weighted.SetName("data_obs")
		channel.Add(plotVarDataOS.Hist(str("Higgsgg"+str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist("DYJets").weighted)
		channel.Add(plotVarDataOS.Hist("DYJets_Electron").weighted)
		channel.Add(plotVarDataOS.Hist("DYJets_Fakes").weighted)
		channel.Add(plotVarDataOS.Hist("WJets").weighted)
		channel.Add(plotVarDataOS.Hist("TTJets").weighted)
		channel.Add(plotVarDataOS.Hist("QCDdata").weighted)
		channel.Add(plotVarDataOS.Hist("Data").weighted)

    if plotVarDataOS.varName == massType+"*1.03" :
		plotVarDataOS.Hist(str("Higgsgg"+str(mass))).weighted.SetName(str("SM"+str(mass)+"_CMS_scale_tUp"))
		plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted.SetName(str("VBF"+str(mass)+"_CMS_scale_tUp"))
		#plotVarDataOS.Hist("DYJets").Add(plotVarDataOS.Hist("DYJets_Photon"))
		plotVarDataOS.Hist("DYJets").weighted.SetName("ZTT_CMS_scale_tUp")
		plotVarDataOS.Hist("DYJets_Electron").weighted.SetName("ZL_CMS_scale_tUp")
		plotVarDataOS.Hist("DYJets_Fakes").weighted.SetName("ZJ_CMS_scale_tUp")
		plotVarDataOS.Hist("WJets").Add(plotVarDataOS.Hist("WJets_Fakes"))
		plotVarDataOS.Hist("WJets").weighted.SetName("W_CMS_scale_tUp")
		plotVarDataOS.Hist("TTJets").weighted.SetName("TT_CMS_scale_tUp")
		plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("ZZ"))
		plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("WZ"))
		plotVarDataOS.Hist("WW").weighted.SetName("diBosons_CMS_scale_tUp")
		channel.Add(plotVarDataOS.Hist(str("Higgsgg"+str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist("DYJets").weighted)
		channel.Add(plotVarDataOS.Hist("DYJets_Electron").weighted)
		channel.Add(plotVarDataOS.Hist("DYJets_Fakes").weighted)
		channel.Add(plotVarDataOS.Hist("WJets").weighted)
		channel.Add(plotVarDataOS.Hist("TTJets").weighted)

    if plotVarDataOS.varName == massType+"*0.97" :
		plotVarDataOS.Hist(str("Higgsgg"+str(mass))).weighted.SetName(str("SM"+str(mass)+"_CMS_scale_tDown"))
		plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted.SetName(str("VBF"+str(mass)+"_CMS_scale_tDown"))
		#plotVarDataOS.Hist("DYJets").Add(plotVarDataOS.Hist("DYJets_Photon"))
		plotVarDataOS.Hist("DYJets").weighted.SetName("ZTT_CMS_scale_tDown")
		plotVarDataOS.Hist("DYJets_Electron").weighted.SetName("ZL_CMS_scale_tDown")
		plotVarDataOS.Hist("DYJets_Fakes").weighted.SetName("ZJ_CMS_scale_tDown")
		plotVarDataOS.Hist("WJets").Add(plotVarDataOS.Hist("WJets_Fakes"))
		plotVarDataOS.Hist("WJets").weighted.SetName("W_CMS_scale_tDown")
		plotVarDataOS.Hist("TTJets").weighted.SetName("TT_CMS_scale_tDown")
		plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("ZZ"))
		plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("WZ"))
		plotVarDataOS.Hist("WW").weighted.SetName("diBosons_CMS_scale_tDown")
		channel.Add(plotVarDataOS.Hist(str("Higgsgg"+str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted)
		channel.Add(plotVarDataOS.Hist("DYJets").weighted)
		channel.Add(plotVarDataOS.Hist("DYJets_Electron").weighted)
		channel.Add(plotVarDataOS.Hist("DYJets_Fakes").weighted)
		channel.Add(plotVarDataOS.Hist("WJets").weighted)
		channel.Add(plotVarDataOS.Hist("TTJets").weighted)

    channel.Write()
    rootfile.Close()

def saveForPlotting(plotVarDataOS, prefixLabel, mass):
    rootfile=TFile(os.getcwd()+"/"+prefixLabel+"/"+prefixLabel+"_tauTau_mH"+str(mass)+"_PLOT.root","UPDATE")
    channel=rootfile.mkdir(str(plotVarDataOS.varName))
    rootfile.cd(str(plotVarDataOS.varName))

########## CHANGE FOR SCALED TES ###########


    plotVarDataOS.Hist(str("Higgsgg"+str(mass))).weighted.SetName(str("SM"+str(mass)))
    plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted.SetName(str("VBF"+str(mass)))
    #plotVarDataOS.Hist("DYJets").Add(plotVarDataOS.Hist("DYJets_Photon"))
    plotVarDataOS.Hist("DYJets").weighted.SetName("ZTT")
    plotVarDataOS.Hist("DYJets_Electron").weighted.SetName("ZL")
    plotVarDataOS.Hist("DYJets_Fakes").weighted.SetName("ZJ")
    plotVarDataOS.Hist("WJets").Add(plotVarDataOS.Hist("WJets_Fakes"))
    plotVarDataOS.Hist("WJets").weighted.SetName("W")
    plotVarDataOS.Hist("TTJets").weighted.SetName("TT")
    plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("ZZ"))
    plotVarDataOS.Hist("WW").Add(plotVarDataOS.Hist("WZ"))
    plotVarDataOS.Hist("WW").weighted.SetName("diBosons")
    plotVarDataOS.Hist("QCDdata").weighted.SetName("QCD")
    plotVarDataOS.Hist("Data").weighted.SetName("data_obs")
    channel.Add(plotVarDataOS.Hist(str("Higgsgg"+str(mass))).weighted)
    channel.Add(plotVarDataOS.Hist(str("HiggsVBF"+str(mass))).weighted)
    channel.Add(plotVarDataOS.Hist("DYJets").weighted)
    channel.Add(plotVarDataOS.Hist("DYJets_Electron").weighted)
    channel.Add(plotVarDataOS.Hist("DYJets_Fakes").weighted)
    channel.Add(plotVarDataOS.Hist("WJets").weighted)
    channel.Add(plotVarDataOS.Hist("TTJets").weighted)
    channel.Add(plotVarDataOS.Hist("QCDdata").weighted)
    channel.Add(plotVarDataOS.Hist("Data").weighted)

    channel.Write()
    rootfile.Close()
