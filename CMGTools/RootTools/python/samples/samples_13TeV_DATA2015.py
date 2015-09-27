import PhysicsTools.HeppyCore.framework.config as cfg
import os

#####COMPONENT CREATOR

from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

### ----------------------------- Zero Tesla run  ----------------------------------------

dataDir = "$CMSSW_BASE/src/CMGTools/TTHAnalysis/data"  # use environmental variable, useful for instance to run on CRAB
json=dataDir+'/json/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON.txt'
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

Jet_Run2015B            = kreator.makeDataComponent("Jet_Run2015B"           , "/Jet/Run2015B-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json)
JetHT_Run2015B          = kreator.makeDataComponent("JetHT_Run2015B"         , "/JetHT/Run2015B-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json)
HTMHT_Run2015B          = kreator.makeDataComponent("HTMHT_Run2015B"         , "/HTMHT/Run2015B-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json)
MET_Run2015B            = kreator.makeDataComponent("MET_Run2015B"           , "/MET/Run2015B-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json)
SingleElectron_Run2015B = kreator.makeDataComponent("SingleElectron_Run2015B", "/SingleElectron/Run2015B-PromptReco-v1/MINIAOD", "CMS", ".*root", json)
SingleMu_Run2015B       = kreator.makeDataComponent("SingleMu_Run2015B"      , "/SingleMu/Run2015B-PromptReco-v1/MINIAOD"      , "CMS", ".*root", json)
SingleMuon_Run2015B     = kreator.makeDataComponent("SingleMuon_Run2015B"    , "/SingleMuon/Run2015B-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json)
SinglePhoton_Run2015B   = kreator.makeDataComponent("SinglePhoton_Run2015B"  , "/SinglePhoton/Run2015B-PromptReco-v1/MINIAOD"  , "CMS", ".*root", json)
EGamma_Run2015B         = kreator.makeDataComponent("EGamma_Run2015B"        , "/EGamma/Run2015B-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json)
DoubleEG_Run2015B       = kreator.makeDataComponent("DoubleEG_Run2015B"      , "/DoubleEG/Run2015B-PromptReco-v1/MINIAOD"      , "CMS", ".*root", json)
MuonEG_Run2015B         = kreator.makeDataComponent("MuonEG_Run2015B"        , "/MuonEG/Run2015B-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json)
DoubleMuon_Run2015B     = kreator.makeDataComponent("DoubleMuon_Run2015B"    , "/DoubleMuon/Run2015B-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json)

minBias_Run2015B  = kreator.makeDataComponent("minBias_Run2015B" , "/MinimumBias/Run2015B-PromptReco-v1/MINIAOD", "CMS", ".*root", json)
zeroBias_Run2015B = kreator.makeDataComponent("zeroBias_Run2015B", "/ZeroBias/Run2015B-PromptReco-v1/MINIAOD"   , "CMS", ".*root", json)

dataSamples_Run2015B = [Jet_Run2015B, JetHT_Run2015B, HTMHT_Run2015B, MET_Run2015B, SingleElectron_Run2015B, SingleMu_Run2015B, SingleMuon_Run2015B, SinglePhoton_Run2015B, EGamma_Run2015B, DoubleEG_Run2015B, MuonEG_Run2015B, DoubleMuon_Run2015B, minBias_Run2015B, zeroBias_Run2015B]

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

dataSamples_17Jul = [Jet_Run2015B_17Jul, JetHT_Run2015B_17Jul, HTMHT_Run2015B_17Jul, MET_Run2015B_17Jul, SingleElectron_Run2015B_17Jul, SingleMu_Run2015B_17Jul, SingleMuon_Run2015B_17Jul, SinglePhoton_Run2015B_17Jul, EGamma_Run2015B_17Jul, DoubleEG_Run2015B_17Jul, MuonEG_Run2015B_17Jul, DoubleMuon_Run2015B_17Jul, minBias_Run2015B_17Jul, zeroBias_Run2015B_17Jul]

### ----------------------------- Run2015C ----------------------------------------

Jet_Run2015C            = kreator.makeDataComponent("Jet_Run2015C"           , "/Jet/Run2015C-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json)
JetHT_Run2015C          = kreator.makeDataComponent("JetHT_Run2015C"         , "/JetHT/Run2015C-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json)
HTMHT_Run2015C          = kreator.makeDataComponent("HTMHT_Run2015C"         , "/HTMHT/Run2015C-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json)
MET_Run2015C            = kreator.makeDataComponent("MET_Run2015C"           , "/MET/Run2015C-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json)
SingleElectron_Run2015C = kreator.makeDataComponent("SingleElectron_Run2015C", "/SingleElectron/Run2015C-PromptReco-v1/MINIAOD", "CMS", ".*root", json)
SingleMuon_Run2015C     = kreator.makeDataComponent("SingleMuon_Run2015C"    , "/SingleMuon/Run2015C-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json)
SinglePhoton_Run2015C   = kreator.makeDataComponent("SinglePhoton_Run2015C"  , "/SinglePhoton/Run2015C-PromptReco-v1/MINIAOD"  , "CMS", ".*root", json)
EGamma_Run2015C         = kreator.makeDataComponent("EGamma_Run2015C"        , "/EGamma/Run2015C-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json)
DoubleEG_Run2015C       = kreator.makeDataComponent("DoubleEG_Run2015C"      , "/DoubleEG/Run2015C-PromptReco-v1/MINIAOD"      , "CMS", ".*root", json)
MuonEG_Run2015C         = kreator.makeDataComponent("MuonEG_Run2015C"        , "/MuonEG/Run2015C-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json)
DoubleMuon_Run2015C     = kreator.makeDataComponent("DoubleMuon_Run2015C"    , "/DoubleMuon/Run2015C-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json)

minBias_Run2015C  = kreator.makeDataComponent("minBias_Run2015C" , "/MinimumBias/Run2015C-PromptReco-v1/MINIAOD", "CMS", ".*root", json)
zeroBias_Run2015C = kreator.makeDataComponent("zeroBias_Run2015C", "/ZeroBias/Run2015C-PromptReco-v1/MINIAOD"   , "CMS", ".*root", json)

dataSamples_Run2015C = [Jet_Run2015C, JetHT_Run2015C, HTMHT_Run2015C, MET_Run2015C, SingleElectron_Run2015C, SingleMuon_Run2015C, SinglePhoton_Run2015C, EGamma_Run2015C, DoubleEG_Run2015C, MuonEG_Run2015C, DoubleMuon_Run2015C, minBias_Run2015C, zeroBias_Run2015C]

### ----------------------------- Run2015D ----------------------------------------

Jet_Run2015D            = kreator.makeDataComponent("Jet_Run2015D"           , "/Jet/Run2015D-PromptReco-v3/MINIAOD"           , "CMS", ".*root", json)
JetHT_Run2015D          = kreator.makeDataComponent("JetHT_Run2015D"         , "/JetHT/Run2015D-PromptReco-v3/MINIAOD"         , "CMS", ".*root", json)
HTMHT_Run2015D          = kreator.makeDataComponent("HTMHT_Run2015D"         , "/HTMHT/Run2015D-PromptReco-v3/MINIAOD"         , "CMS", ".*root", json)
MET_Run2015D            = kreator.makeDataComponent("MET_Run2015D"           , "/MET/Run2015D-PromptReco-v3/MINIAOD"           , "CMS", ".*root", json)
SingleElectron_Run2015D = kreator.makeDataComponent("SingleElectron_Run2015D", "/SingleElectron/Run2015D-PromptReco-v3/MINIAOD", "CMS", ".*root", json)
SingleMuon_Run2015D     = kreator.makeDataComponent("SingleMuon_Run2015D"    , "/SingleMuon/Run2015D-PromptReco-v3/MINIAOD"    , "CMS", ".*root", json)
SinglePhoton_Run2015D   = kreator.makeDataComponent("SinglePhoton_Run2015D"  , "/SinglePhoton/Run2015D-PromptReco-v3/MINIAOD"  , "CMS", ".*root", json)
EGamma_Run2015D         = kreator.makeDataComponent("EGamma_Run2015D"        , "/EGamma/Run2015D-PromptReco-v3/MINIAOD"        , "CMS", ".*root", json)
DoubleEG_Run2015D       = kreator.makeDataComponent("DoubleEG_Run2015D"      , "/DoubleEG/Run2015D-PromptReco-v3/MINIAOD"      , "CMS", ".*root", json)
MuonEG_Run2015D         = kreator.makeDataComponent("MuonEG_Run2015D"        , "/MuonEG/Run2015D-PromptReco-v3/MINIAOD"        , "CMS", ".*root", json)
DoubleMuon_Run2015D     = kreator.makeDataComponent("DoubleMuon_Run2015D"    , "/DoubleMuon/Run2015D-PromptReco-v3/MINIAOD"    , "CMS", ".*root", json)

minBias_Run2015D  = kreator.makeDataComponent("minBias_Run2015D" , "/MinimumBias/Run2015D-PromptReco-v3/MINIAOD", "CMS", ".*root", json)
zeroBias_Run2015D = kreator.makeDataComponent("zeroBias_Run2015D", "/ZeroBias/Run2015D-PromptReco-v3/MINIAOD"   , "CMS", ".*root", json)

dataSamples_Run2015D = [Jet_Run2015D, JetHT_Run2015D, HTMHT_Run2015D, MET_Run2015D, SingleElectron_Run2015D, SingleMuon_Run2015D, SinglePhoton_Run2015D, EGamma_Run2015D, DoubleEG_Run2015D, MuonEG_Run2015D, DoubleMuon_Run2015D, minBias_Run2015D, zeroBias_Run2015D]


### ----------------------------- summary ----------------------------------------

dataSamples = dataSamples_Run2015B + dataSamples_17Jul + dataSamples_Run2015C + dataSamples_Run2015D
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
