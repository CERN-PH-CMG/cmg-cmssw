import PhysicsTools.HeppyCore.framework.config as cfg
import os

#####COMPONENT CREATOR

from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

### ----------------------------- Zero Tesla run  ----------------------------------------

dataDir = "$CMSSW_BASE/src/CMGTools/TTHAnalysis/data"  # use environmental variable, useful for instance to run on CRAB
#json=dataDir+'/json/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON.txt'
#json=dataDir+'/json/Cert_246908-255031_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
json=dataDir+'/json/json_DCSONLY.txt'
# https://hypernews.cern.ch/HyperNews/CMS/get/physics-validation/2446.html


#jetHT_0T = cfg.DataComponent(
#    name = 'jetHT_0T',
#    files = kreator.getFilesFromEOS('jetHT_0T',
#                                    'firstData_JetHT_v2',
#                                    '/store/user/pandolf/MINIAOD/%s'),
#    intLumi = 4.0,
#    triggers = [],
#    json = None #json
#    )


### ----------------------------- Magnetic Field On ----------------------------------------

#Jet_Run2015B            = kreator.makeDataComponent("Jet_Run2015B"           , "/Jet/Run2015B-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json)
#SingleMu_Run2015B       = kreator.makeDataComponent("SingleMu_Run2015B"      , "/SingleMu/Run2015B-PromptReco-v1/MINIAOD"      , "CMS", ".*root", json)
#EGamma_Run2015B         = kreator.makeDataComponent("EGamma_Run2015B"        , "/EGamma/Run2015B-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json)

#JetHT_Run2015B          = kreator.makeDataComponent("JetHT_Run2015B"         , "/JetHT/Run2015B-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json)
#HTMHT_Run2015B          = kreator.makeDataComponent("HTMHT_Run2015B"         , "/HTMHT/Run2015B-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json)
#MET_Run2015B            = kreator.makeDataComponent("MET_Run2015B"           , "/MET/Run2015B-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json)
#SingleElectron_Run2015B = kreator.makeDataComponent("SingleElectron_Run2015B", "/SingleElectron/Run2015B-PromptReco-v1/MINIAOD", "CMS", ".*root", json)
#SingleMuon_Run2015B     = kreator.makeDataComponent("SingleMuon_Run2015B"    , "/SingleMuon/Run2015B-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json)
#SinglePhoton_Run2015B   = kreator.makeDataComponent("SinglePhoton_Run2015B"  , "/SinglePhoton/Run2015B-PromptReco-v1/MINIAOD"  , "CMS", ".*root", json)
#DoubleEG_Run2015B       = kreator.makeDataComponent("DoubleEG_Run2015B"      , "/DoubleEG/Run2015B-PromptReco-v1/MINIAOD"      , "CMS", ".*root", json)
#MuonEG_Run2015B         = kreator.makeDataComponent("MuonEG_Run2015B"        , "/MuonEG/Run2015B-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json)
#DoubleMuon_Run2015B     = kreator.makeDataComponent("DoubleMuon_Run2015B"    , "/DoubleMuon/Run2015B-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json)

# July17 for run<=251561 with MINIAOD reprocessed with correct MET filters
run_range = (251244, 251562)
label = "_runs%s_%s"%(run_range[0], run_range[1])

JetHT_Run2015B_17Jul2015          = kreator.makeDataComponent("JetHT_Run2015B_17Jul2015"         , "/JetHT/Run2015B-17Jul2015-v1/MINIAOD"         , "CMS", ".*root", json, run_range)
HTMHT_Run2015B_17Jul2015          = kreator.makeDataComponent("HTMHT_Run2015B_17Jul2015"         , "/HTMHT/Run2015B-17Jul2015-v1/MINIAOD"         , "CMS", ".*root", json, run_range)
MET_Run2015B_17Jul2015            = kreator.makeDataComponent("MET_Run2015B_17Jul2015"           , "/MET/Run2015B-17Jul2015-v1/MINIAOD"           , "CMS", ".*root", json, run_range)
SingleElectron_Run2015B_17Jul2015 = kreator.makeDataComponent("SingleElectron_Run2015B_17Jul2015", "/SingleElectron/Run2015B-17Jul2015-v1/MINIAOD", "CMS", ".*root", json, run_range)
SingleMuon_Run2015B_17Jul2015     = kreator.makeDataComponent("SingleMuon_Run2015B_17Jul2015"    , "/SingleMuon/Run2015B-17Jul2015-v1/MINIAOD"    , "CMS", ".*root", json, run_range)
SinglePhoton_Run2015B_17Jul2015   = kreator.makeDataComponent("SinglePhoton_Run2015B_17Jul2015"  , "/SinglePhoton/Run2015B-17Jul2015-v1/MINIAOD"  , "CMS", ".*root", json, run_range)
DoubleEG_Run2015B_17Jul2015       = kreator.makeDataComponent("DoubleEG_Run2015B_17Jul2015"      , "/DoubleEG/Run2015B-17Jul2015-v1/MINIAOD"      , "CMS", ".*root", json, run_range)
MuonEG_Run2015B_17Jul2015         = kreator.makeDataComponent("MuonEG_Run2015B_17Jul2015"        , "/MuonEG/Run2015B-17Jul2015-v1/MINIAOD"        , "CMS", ".*root", json, run_range)
DoubleMuon_Run2015B_17Jul2015     = kreator.makeDataComponent("DoubleMuon_Run2015B_17Jul2015"    , "/DoubleMuon/Run2015B-17Jul2015-v1/MINIAOD"    , "CMS", ".*root", json, run_range)


# PromptReco-v1 for run > 251561
run_range = (251643, 251883)
label = "_runs%s_%s"%(run_range[0], run_range[1])

JetHT_Run2015B_PromptReco          = kreator.makeDataComponent("JetHT_Run2015B_PromptReco"         , "/JetHT/Run2015B-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json, run_range)
HTMHT_Run2015B_PromptReco          = kreator.makeDataComponent("HTMHT_Run2015B_PromptReco"         , "/HTMHT/Run2015B-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json, run_range)
MET_Run2015B_PromptReco            = kreator.makeDataComponent("MET_Run2015B_PromptReco"           , "/MET/Run2015B-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json, run_range)
SingleElectron_Run2015B_PromptReco = kreator.makeDataComponent("SingleElectron_Run2015B_PromptReco", "/SingleElectron/Run2015B-PromptReco-v1/MINIAOD", "CMS", ".*root", json, run_range)
SingleMuon_Run2015B_PromptReco     = kreator.makeDataComponent("SingleMuon_Run2015B_PromptReco"    , "/SingleMuon/Run2015B-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json, run_range)
SinglePhoton_Run2015B_PromptReco   = kreator.makeDataComponent("SinglePhoton_Run2015B_PromptReco"  , "/SinglePhoton/Run2015B-PromptReco-v1/MINIAOD"  , "CMS", ".*root", json, run_range)
DoubleEG_Run2015B_PromptReco       = kreator.makeDataComponent("DoubleEG_Run2015B_PromptReco"      , "/DoubleEG/Run2015B-PromptReco-v1/MINIAOD"      , "CMS", ".*root", json, run_range)
MuonEG_Run2015B_PromptReco         = kreator.makeDataComponent("MuonEG_Run2015B_PromptReco"        , "/MuonEG/Run2015B-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json, run_range)
DoubleMuon_Run2015B_PromptReco     = kreator.makeDataComponent("DoubleMuon_Run2015B_PromptReco"    , "/DoubleMuon/Run2015B-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json, run_range)


#minBias_Run2015B  = kreator.makeDataComponent("minBias_Run2015B" , "/MinimumBias/Run2015B-PromptReco-v1/MINIAOD", "CMS", ".*root", json)
#zeroBias_Run2015B = kreator.makeDataComponent("zeroBias_Run2015B", "/ZeroBias/Run2015B-PromptReco-v1/MINIAOD"   , "CMS", ".*root", json)

dataSamples_Run2015B = [JetHT_Run2015B_PromptReco, HTMHT_Run2015B_PromptReco, MET_Run2015B_PromptReco, SingleElectron_Run2015B_PromptReco, SingleMuon_Run2015B_PromptReco, SinglePhoton_Run2015B_PromptReco, DoubleEG_Run2015B_PromptReco, MuonEG_Run2015B_PromptReco, DoubleMuon_Run2015B_PromptReco]

### ----------------------------- 17July re-reco ----------------------------------------
# https://twiki.cern.ch/twiki/bin/view/CMS/PdmVDataReprocessing747reMiniAod2015B

Jet_Run2015B_17Jul            = kreator.makeDataComponent("Jet_Run2015B_17Jul"           , "/Jet/Run2015B-17Jul2015-v1/MINIAOD"           , "CMS", ".*root", json)
JetHT_Run2015B_17Jul          = kreator.makeDataComponent("JetHT_Run2015B_17Jul"         , "/JetHT/Run2015B-17Jul2015-v1/MINIAOD"         , "CMS", ".*root", json)
HTMHT_Run2015B_17Jul          = kreator.makeDataComponent("HTMHT_Run2015B_17Jul"         , "/HTMHT/Run2015B-17Jul2015-v1/MINIAOD"         , "CMS", ".*root", json)
MET_Run2015B_17Jul            = kreator.makeDataComponent("MET_Run2015B_17Jul"           , "/MET/Run2015B-17Jul2015-v1/MINIAOD"           , "CMS", ".*root", json)
SingleElectron_Run2015B_17Jul = kreator.makeDataComponent("SingleElectron_Run2015B_17Jul", "/SingleElectron/Run2015B-17Jul2015-v1/MINIAOD", "CMS", ".*root", json)
SingleMu_Run2015B_17Jul       = kreator.makeDataComponent("SingleMu_Run2015B_17Jul"      , "/SingleMu/Run2015B-17Jul2015-v1/MINIAOD"      , "CMS", ".*root", json)
SingleMuon_Run2015B_17Jul     = kreator.makeDataComponent("SingleMuon_Run2015B_17Jul"    , "/SingleMuon/Run2015B-17Jul2015-v1/MINIAOD"    , "CMS", ".*root", json)
SinglePhoton_Run2015B_17Jul   = kreator.makeDataComponent("SinglePhoton_Run2015B_17Jul"  , "/SinglePhoton/Run2015B-17Jul2015-v1/MINIAOD"  , "CMS", ".*root", json)
EGamma_Run2015B_17Jul         = kreator.makeDataComponent("EGamma_Run2015B_17Jul"        , "/EGamma/Run2015B-17Jul2015-v1/MINIAOD"        , "CMS", ".*root", json)
DoubleEG_Run2015B_17Jul       = kreator.makeDataComponent("DoubleEG_Run2015B_17Jul"      , "/DoubleEG/Run2015B-17Jul2015-v1/MINIAOD"      , "CMS", ".*root", json)
MuonEG_Run2015B_17Jul         = kreator.makeDataComponent("MuonEG_Run2015B_17Jul"        , "/MuonEG/Run2015B-17Jul2015-v1/MINIAOD"        , "CMS", ".*root", json)
DoubleMuon_Run2015B_17Jul     = kreator.makeDataComponent("DoubleMuon_Run2015B_17Jul"    , "/DoubleMuon/Run2015B-17Jul2015-v1/MINIAOD"    , "CMS", ".*root", json)

minBias_Run2015B_17Jul  = kreator.makeDataComponent("minBias_Run2015B_17Jul" , "/MinimumBias/Run2015B-17Jul2015-v1/MINIAOD", "CMS", ".*root", json)
zeroBias_Run2015B_17Jul = kreator.makeDataComponent("zeroBias_Run2015B_17Jul", "/ZeroBias/Run2015B-17Jul2015-v1/MINIAOD"   , "CMS", ".*root", json)


#dataSamples = [JetHT_Run2015B_17Jul2015, HTMHT_Run2015B_17Jul2015, MET_Run2015B_17Jul2015, SingleElectron_Run2015B_17Jul2015, SingleMuon_Run2015B_17Jul2015, SinglePhoton_Run2015B_17Jul2015, DoubleEG_Run2015B_17Jul2015, MuonEG_Run2015B_17Jul2015, DoubleMuon_Run2015B_17Jul2015, JetHT_Run2015B_PromptReco, HTMHT_Run2015B_PromptReco, MET_Run2015B_PromptReco, SingleElectron_Run2015B_PromptReco, SingleMuon_Run2015B_PromptReco, SinglePhoton_Run2015B_PromptReco, DoubleEG_Run2015B_PromptReco, MuonEG_Run2015B_PromptReco, DoubleMuon_Run2015B_PromptReco]
dataSamples_17Jul = [Jet_Run2015B_17Jul, JetHT_Run2015B_17Jul, HTMHT_Run2015B_17Jul, MET_Run2015B_17Jul, SingleElectron_Run2015B_17Jul, SingleMu_Run2015B_17Jul, SingleMuon_Run2015B_17Jul, SinglePhoton_Run2015B_17Jul, EGamma_Run2015B_17Jul, DoubleEG_Run2015B_17Jul, MuonEG_Run2015B_17Jul, DoubleMuon_Run2015B_17Jul, minBias_Run2015B_17Jul, zeroBias_Run2015B_17Jul]

### ----------------------------- Run2015C ----------------------------------------

Jet_Run2015C            = kreator.makeDataComponent("Jet_Run2015C"           , "/Jet/Run2015C-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json)
JetHT_Run2015C          = kreator.makeDataComponent("JetHT_Run2015C"         , "/JetHT/Run2015C-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json)
HTMHT_Run2015C          = kreator.makeDataComponent("HTMHT_Run2015C"         , "/HTMHT/Run2015C-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json)
MET_Run2015C            = kreator.makeDataComponent("MET_Run2015C"           , "/MET/Run2015C-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json)
SingleElectron_Run2015C = kreator.makeDataComponent("SingleElectron_Run2015C", "/SingleElectron/Run2015C-PromptReco-v1/MINIAOD", "CMS", ".*root", json)
SingleMu_Run2015C       = kreator.makeDataComponent("SingleMu_Run2015C"      , "/SingleMu/Run2015C-PromptReco-v1/MINIAOD"      , "CMS", ".*root", json)
SingleMuon_Run2015C     = kreator.makeDataComponent("SingleMuon_Run2015C"    , "/SingleMuon/Run2015C-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json)
SinglePhoton_Run2015C   = kreator.makeDataComponent("SinglePhoton_Run2015C"  , "/SinglePhoton/Run2015C-PromptReco-v1/MINIAOD"  , "CMS", ".*root", json)
EGamma_Run2015C         = kreator.makeDataComponent("EGamma_Run2015C"        , "/EGamma/Run2015C-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json)
DoubleEG_Run2015C       = kreator.makeDataComponent("DoubleEG_Run2015C"      , "/DoubleEG/Run2015C-PromptReco-v1/MINIAOD"      , "CMS", ".*root", json)
MuonEG_Run2015C         = kreator.makeDataComponent("MuonEG_Run2015C"        , "/MuonEG/Run2015C-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json)
DoubleMuon_Run2015C     = kreator.makeDataComponent("DoubleMuon_Run2015C"    , "/DoubleMuon/Run2015C-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json)

minBias_Run2015C  = kreator.makeDataComponent("minBias_Run2015C" , "/MinimumBias/Run2015C-PromptReco-v1/MINIAOD", "CMS", ".*root", json)
zeroBias_Run2015C = kreator.makeDataComponent("zeroBias_Run2015C", "/ZeroBias/Run2015C-PromptReco-v1/MINIAOD"   , "CMS", ".*root", json)

dataSamples_Run2015C = [Jet_Run2015C, JetHT_Run2015C, HTMHT_Run2015C, MET_Run2015C, SingleElectron_Run2015C, SingleMu_Run2015C, SingleMuon_Run2015C, SinglePhoton_Run2015C, EGamma_Run2015C, DoubleEG_Run2015C, MuonEG_Run2015C, DoubleMuon_Run2015C, minBias_Run2015C, zeroBias_Run2015C]


### ----------------------------- Run2015D ----------------------------------------

run_range = (256630, 256941)

JetHT_Run2015D          = kreator.makeDataComponent("JetHT_Run2015D"         , "/JetHT/Run2015D-PromptReco-v3/MINIAOD"         , "CMS", ".*root", json, run_range)
HTMHT_Run2015D          = kreator.makeDataComponent("HTMHT_Run2015D"         , "/HTMHT/Run2015D-PromptReco-v3/MINIAOD"         , "CMS", ".*root", json, run_range)
MET_Run2015D            = kreator.makeDataComponent("MET_Run2015D"           , "/MET/Run2015D-PromptReco-v3/MINIAOD"           , "CMS", ".*root", json, run_range)
SingleElectron_Run2015D = kreator.makeDataComponent("SingleElectron_Run2015D", "/SingleElectron/Run2015D-PromptReco-v3/MINIAOD", "CMS", ".*root", json, run_range)
SingleMuon_Run2015D     = kreator.makeDataComponent("SingleMuon_Run2015D"    , "/SingleMuon/Run2015D-PromptReco-v3/MINIAOD"    , "CMS", ".*root", json, run_range)
SinglePhoton_Run2015D   = kreator.makeDataComponent("SinglePhoton_Run2015D"  , "/SinglePhoton/Run2015D-PromptReco-v3/MINIAOD"  , "CMS", ".*root", json, run_range)
DoubleEG_Run2015D       = kreator.makeDataComponent("DoubleEG_Run2015D"      , "/DoubleEG/Run2015D-PromptReco-v3/MINIAOD"      , "CMS", ".*root", json, run_range)
MuonEG_Run2015D         = kreator.makeDataComponent("MuonEG_Run2015D"        , "/MuonEG/Run2015D-PromptReco-v3/MINIAOD"        , "CMS", ".*root", json, run_range)
DoubleMuon_Run2015D     = kreator.makeDataComponent("DoubleMuon_Run2015D"    , "/DoubleMuon/Run2015D-PromptReco-v3/MINIAOD"    , "CMS", ".*root", json, run_range)


dataSamples_Run2015D = [JetHT_Run2015D, HTMHT_Run2015D, MET_Run2015D, SingleElectron_Run2015D, SingleMuon_Run2015D, SinglePhoton_Run2015D, DoubleEG_Run2015D, MuonEG_Run2015D, DoubleMuon_Run2015D]

### ----------------------------- summary ----------------------------------------

#dataSamples = dataSamples_Run2015B + dataSamples_17Jul + dataSamples_Run2015C
dataSamples =  dataSamples_Run2015D
samples = dataSamples

### ---------------------------------------------------------------------

from CMGTools.TTHAnalysis.setup.Efficiencies import *
dataDir = "$CMSSW_BASE/src/CMGTools/TTHAnalysis/data"

for comp in samples:
    comp.splitFactor = 1000
    comp.isMC = False
    comp.isData = True

if __name__ == "__main__":
   import sys
   if "test" in sys.argv:
       from CMGTools.RootTools.samples.ComponentCreator import testSamples
       testSamples(samples)
