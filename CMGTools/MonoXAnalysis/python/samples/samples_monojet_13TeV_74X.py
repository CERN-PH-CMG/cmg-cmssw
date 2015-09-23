import PhysicsTools.HeppyCore.framework.config as cfg
import os

#####COMPONENT CREATOR
from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

### common MC samples
from CMGTools.RootTools.samples.samples_13TeV_74X import *
from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import *

### additional MC samples

### 25 ns

### 50 ns

###

## === 741 RelVals ===
ADD_MJ = kreator.makeMCComponent("ADD_MJ", "/RelValADDMonoJet_d3MD3_13/CMSSW_7_4_1-MCRUN2_74_V9_gensim_740pre7-v1/MINIAODSIM", "CMS", ".*root")
TTLep = kreator.makeMCComponent("TTLep", "/RelValTTbarLepton_13/CMSSW_7_4_1-MCRUN2_74_V9_gensim_740pre7-v1/MINIAODSIM", "CMS", ".*root")
TTbar = kreator.makeMCComponent("TTbar", "/RelValTTbar_13/CMSSW_7_4_1-MCRUN2_74_V9_gensim71X-v1/MINIAODSIM", "CMS", ".*root")
RSGravGaGa = kreator.makeMCComponent("RSGravGaGa", "/RelValRSGravitonToGaGa_13TeV/CMSSW_7_4_1-MCRUN2_74_V9_gensim71X-v1/MINIAODSIM", "CMS", ".*root")

RelVals741 = [ ADD_MJ, TTLep, TTbar, RSGravGaGa ] 



#-----------DATA--------------- 
json = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-255031_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt"

### ---- private samples ----
privEGamma2015Afiles = [ f.strip() for f in open("%s/src/CMGTools/MonoXAnalysis/python/samples/privEGamma_2015A_MINIAOD.txt"  % os.environ['CMSSW_BASE'], "r") ]
privDoubleEG2015Afiles = [ f.strip() for f in open("%s/src/CMGTools/MonoXAnalysis/python/samples/privDoubleEG_2015A_MINIAOD.txt"  % os.environ['CMSSW_BASE'], "r") ]
privHLTPhysics2015Afiles = [ f.strip() for f in open("%s/src/CMGTools/MonoXAnalysis/python/samples/privHLTPhysics_2015A_MINIAOD.txt"  % os.environ['CMSSW_BASE'], "r") ]
privReRecoDoubleEG2015Cfiles = [ f.strip() for f in open("%s/src/CMGTools/MonoXAnalysis/python/samples/privReReco_DoubleEG2015C_MINIAOD.txt" % os.environ['CMSSW_BASE'], "r") ]
privExpress2015Dfiles = [ f.strip() for f in open("%s/src/CMGTools/MonoXAnalysis/python/samples/privExpress_Run2015D_MINIAOD.txt" % os.environ['CMSSW_BASE'], "r") ]
def _grep(x,l): return [ i for i in l if x in i ]
privEGamma2015A_0T = kreator.makePrivateDataComponent('EGamma2015A', '/store/group/dpg_ecal/comm_ecal/data13TeV/EGamma/MINIAOD', _grep('2015A', privEGamma2015Afiles), json )
privDoubleEG2015A_0T = kreator.makePrivateDataComponent('DoubleEG2015A', '/store/group/dpg_ecal/comm_ecal/data13TeV/DoubleEG/MINIAOD', _grep('PAT', privDoubleEG2015Afiles), json )
privHLTPhysics2015A_0T = kreator.makePrivateDataComponent('HLTPhysics2015A', '/store/group/dpg_ecal/comm_ecal/data13TeV/HLTPhysics/MINIAOD', _grep('HLTPhysics', privHLTPhysics2015Afiles), json )
privReRecoDoubleEG2015C = kreator.makePrivateDataComponent('DoubleEG2015C', '/store/group/dpg_ecal/comm_ecal/localreco/data2015_zskim_multifits/miniaod//run2015C_dataPS', _grep('2015C', privReRecoDoubleEG2015Cfiles), json )
privExpress2015D = kreator.makePrivateDataComponent('Express2015D', '/store/group/dpg_ecal/comm_ecal/localreco/data2015D_zskim/miniaod/ExpressPhysics/256/843', _grep('Express', privExpress2015Dfiles), json )
privDataSamples_0T = [ privEGamma2015A_0T, privDoubleEG2015A_0T, privHLTPhysics2015A_0T ]

### ---------------------------- 3.8T data ----------------------------------------

### ----------------------------- Run2015D ----------------------------------------

Jet_Run2015D            = kreator.makeDataComponent("Jet_Run2015D"           , "/Jet/Run2015D-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json)
JetHT_Run2015D          = kreator.makeDataComponent("JetHT_Run2015D"         , "/JetHT/Run2015D-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json)
HTMHT_Run2015D          = kreator.makeDataComponent("HTMHT_Run2015D"         , "/HTMHT/Run2015D-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json)
MET_Run2015D            = kreator.makeDataComponent("MET_Run2015D"           , "/MET/Run2015D-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json)
SingleElectron_Run2015D = kreator.makeDataComponent("SingleElectron_Run2015D", "/SingleElectron/Run2015D-PromptReco-v1/MINIAOD", "CMS", ".*root", json)
SingleMu_Run2015D       = kreator.makeDataComponent("SingleMu_Run2015D"      , "/SingleMu/Run2015D-PromptReco-v1/MINIAOD"      , "CMS", ".*root", json)
SingleMuon_Run2015D     = kreator.makeDataComponent("SingleMuon_Run2015D"    , "/SingleMuon/Run2015D-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json)
SinglePhoton_Run2015D   = kreator.makeDataComponent("SinglePhoton_Run2015D"  , "/SinglePhoton/Run2015D-PromptReco-v1/MINIAOD"  , "CMS", ".*root", json)
EGamma_Run2015D         = kreator.makeDataComponent("EGamma_Run2015D"        , "/EGamma/Run2015D-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json)
DoubleEG_Run2015D       = kreator.makeDataComponent("DoubleEG_Run2015D"      , "/DoubleEG/Run2015D-PromptReco-v1/MINIAOD"      , "CMS", ".*root", json)
MuonEG_Run2015D         = kreator.makeDataComponent("MuonEG_Run2015D"        , "/MuonEG/Run2015D-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json)
DoubleMuon_Run2015D     = kreator.makeDataComponent("DoubleMuon_Run2015D"    , "/DoubleMuon/Run2015D-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json)

minBias_Run2015D  = kreator.makeDataComponent("minBias_Run2015D" , "/MinimumBias/Run2015D-PromptReco-v1/MINIAOD", "CMS", ".*root", json)
zeroBias_Run2015D = kreator.makeDataComponent("zeroBias_Run2015D", "/ZeroBias/Run2015D-PromptReco-v1/MINIAOD"   , "CMS", ".*root", json)

dataSamples_Run2015D = [Jet_Run2015D, JetHT_Run2015D, HTMHT_Run2015D, MET_Run2015D, SingleElectron_Run2015D, SingleMu_Run2015D, SingleMuon_Run2015D, SinglePhoton_Run2015D, EGamma_Run2015D, DoubleEG_Run2015D, MuonEG_Run2015D, DoubleMuon_Run2015D, minBias_Run2015D, zeroBias_Run2015D]


### ----------------------------- summary ----------------------------------------     

### --- data ---
dataSamples = dataSamples_Run2015B + dataSamples_17Jul + dataSamples_Run2015C + dataSamples_Run2015D

### --- mc ---
mcSamples_monojet_Asymptotic25ns = TTs + SingleTop + [WJetsToLNu, DYJetsToLL_M50, DYJetsToLL_M10to50 ] + ZJetsToNuNuHT + DYJetsM50HT + WJetsToLNuHT + GJetsHT + QCDHT + DiBosons
mcSamples_monojet_Asymptotic50ns = [ TTJets_50ns, TTJets_LO_50ns, DYJetsToLL_M50_50ns, WJetsToLNu_50ns ] + QCDPt_50ns
mcSamples_monojet = RelVals740 + RelVals741 + mcSamples_monojet_Asymptotic25ns + mcSamples_monojet_Asymptotic50ns

from CMGTools.TTHAnalysis.setup.Efficiencies import *
dataDir = "$CMSSW_BASE/src/CMGTools/TTHAnalysis/data"

#Define splitting
for comp in mcSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 250 #  if comp.name in [ "WJets", "DY3JetsM50", "DY4JetsM50","W1Jets","W2Jets","W3Jets","W4Jets","TTJetsHad" ] else 100
    comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    comp.efficiency = eff2012
for comp in dataSamples:
    comp.splitFactor = 1000
    comp.isMC = False
    comp.isData = True


if __name__ == "__main__":
   import sys
   if "test" in sys.argv:
       from CMGTools.RootTools.samples.ComponentCreator import testSamples
       testSamples(mcSamples)
