import PhysicsTools.HeppyCore.framework.config as cfg
import os

#####COMPONENT CREATOR

from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

### ----------------------------- Zero Tesla run  ----------------------------------------

dataDir = "$CMSSW_BASE/src/CMGTools/TTHAnalysis/data"  # use environmental variable, useful for instance to run on CRAB
#json=dataDir+'/json/Cert_246908-248005_13TeV_PromptReco_Collisions15_ZeroTesla_JSON.txt'
#lumi: delivered= 4.430 (/nb) recorded= 4.013 (/nb)
#json=dataDir+'/json/json_DCSONLY_Run2015B.txt'
#json=dataDir+'/json/Cert_246908-251252_13TeV_PromptReco_Collisions15_JSON.txt' # golden json 5.6/pb
json=dataDir+'/json/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt' # golden json v2 40/pb

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

### ----------------------------- summary ----------------------------------------


#dataSamples = [jetHT_0T]
 
#dataSamples = [JetHT_Run2015B, HTMHT_Run2015B, MET_Run2015B, SingleElectron_Run2015B, SingleMuon_Run2015B, SinglePhoton_Run2015B, DoubleEG_Run2015B, MuonEG_Run2015B, DoubleMuon_Run2015B] #, Jet_Run2015B, SingleMu_Run2015B, EGamma_Run2015B, minBias_Run2015B, zeroBias_Run2015B]
dataSamples = [JetHT_Run2015B_17Jul2015, HTMHT_Run2015B_17Jul2015, MET_Run2015B_17Jul2015, SingleElectron_Run2015B_17Jul2015, SingleMuon_Run2015B_17Jul2015, SinglePhoton_Run2015B_17Jul2015, DoubleEG_Run2015B_17Jul2015, MuonEG_Run2015B_17Jul2015, DoubleMuon_Run2015B_17Jul2015, JetHT_Run2015B_PromptReco, HTMHT_Run2015B_PromptReco, MET_Run2015B_PromptReco, SingleElectron_Run2015B_PromptReco, SingleMuon_Run2015B_PromptReco, SinglePhoton_Run2015B_PromptReco, DoubleEG_Run2015B_PromptReco, MuonEG_Run2015B_PromptReco, DoubleMuon_Run2015B_PromptReco]


samples = dataSamples

### ---------------------------------------------------------------------

from CMGTools.TTHAnalysis.setup.Efficiencies import *
dataDir = "$CMSSW_BASE/src/CMGTools/TTHAnalysis/data"

for comp in dataSamples:
    comp.splitFactor = 1000
    comp.isMC = False
    comp.isData = True

if __name__ == "__main__":
   import sys
   if "test" in sys.argv:
       from CMGTools.RootTools.samples.ComponentCreator import testSamples
       testSamples(samples)
