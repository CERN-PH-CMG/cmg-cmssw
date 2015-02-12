import PhysicsTools.HeppyCore.framework.config as cfg
import os




################## Triggers (FIXME: update to the PHYS14 Trigger Menu)


triggers_mumu_run1   = ["HLT_Mu17_Mu8_v*","HLT_Mu17_TkMu8_v*"]
triggers_mumu_iso    = [ "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v*", "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v*" ]
triggers_mumu_noniso = [ "HLT_Mu30_TkMu11_v*" ]
triggers_mumu = triggers_mumu_iso + triggers_mumu_noniso

triggers_ee_run1   = ["HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*",
                      "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
                      "HLT_Ele15_Ele8_Ele5_CaloIdL_TrkIdVL_v*"]
triggers_ee = [ "HLT_Ele23_Ele12_CaloId_TrackId_Iso_v*" ]
triggers_3e = [ "HLT_Ele17_Ele12_Ele10_CaloId_TrackId_v*" ]
triggers_mue   = [
    "HLT_Mu23_TrkIsoVVL_Ele12_Gsf_CaloId_TrackId_Iso_MediumWP_v*",
    "HLT_Mu8_TrkIsoVVL_Ele23_Gsf_CaloId_TrackId_Iso_MediumWP_v*"
    ]

triggers_multilep  = triggers_mumu + triggers_ee + triggers_3e + triggers_mue

triggers_1mu_iso    = [ 'HLT_IsoMu24_eta2p1_IterTrk02_v*', 'HLT_IsoTkMu24_eta2p1_IterTrk02_v*'  ]
triggers_1mu_isowid = [ 'HLT_IsoMu24_IterTrk02_v*', 'HLT_IsoTkMu24_IterTrk02_v*'  ]
triggers_1mu_isolow = [ 'HLT_IsoMu20_eta2p1_IterTrk02_v*', 'HLT_IsoTkMu20_eta2p1_IterTrk02_v*'  ]
triggers_1mu_noniso = [ 'HLT_Mu40_v*' ]
triggers_1mu = triggers_1mu_iso  + triggers_1mu_isowid + triggers_1mu_isolow + triggers_1mu_noniso

triggers_1e = [ "HLT_Ele27_eta2p1_WP85_Gsf_v*", "HLT_Ele32_eta2p1_WP85_Gsf_v*" ]

triggersFR_1mu  = [ 'HLT_Mu5_v*', 'HLT_RelIso1p0Mu5_v*', 'HLT_Mu12_v*', 'HLT_Mu24_eta2p1_v*', 'HLT_Mu40_eta2p1_v*' ]
triggersFR_mumu = [ 'HLT_Mu17_Mu8_v*', 'HLT_Mu17_TkMu8_v*', 'HLT_Mu8_v*', 'HLT_Mu17_v*' ]
triggersFR_1e   = [ 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*', 'HLT_Ele17_CaloIdL_CaloIsoVL_v*', 'HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*', 'HLT_Ele8__CaloIdL_CaloIsoVL_v*']
triggersFR_mue  = triggers_mue[:]
triggersFR_MC = triggersFR_1mu + triggersFR_mumu + triggersFR_1e + triggersFR_mue



### ----> for the MonoJet analysis
triggers_MonoJet = ["HLT_MonoCentralPFJet140_PFMETNoMu100_PFMHTNoMu140_NoiseCleaned_v*","HLT_MonoCentralPFJet140_PFMETNoMu140_PFMHTNoMu140_NoiseCleaned_v*","HLT_MonoCentralPFJet150_PFMETNoMu150_PFMHTNoMu150_NoiseCleaned_v*"]

#####COMPONENT CREATOR

from CMGTools.TTHAnalysis.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

# DY inclusive (cross section from FEWZ, StandardModelCrossSectionsat13TeV)
DYJetsToLL_M50 = kreator.makeMCComponent("DYJetsToLL_M50", "/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root", 2008.*3)

# MONOJET SAMPLES (dummy xsec)
Monojet_M_10 = kreator.makeMCComponent("Monojet_M_10", '/DarkMatter_Monojet_M-10_AV_Tune4C_13TeV-madgraph/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM', "CMS", ".*root", 0.0189612)

MonoJetSignalSamples = [
Monojet_M_10
]


mcSamplesPHYS14_PU20bx25 = [DYJetsToLL_M50] + MonoJetSignalSamples

mcSamples = mcSamplesPHYS14_PU20bx25

#-----------DATA---------------
dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/TTHAnalysis/data"
json=dataDir+'/json/Cert_Run2012ABCD_22Jan2013ReReco.json'
dataSamplesAll = []


from CMGTools.TTHAnalysis.setup.Efficiencies import *


#Define splitting
for comp in mcSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 250 #  if comp.name in [ "WJets", "DY3JetsM50", "DY4JetsM50","W1Jets","W2Jets","W3Jets","W4Jets","TTJetsHad" ] else 100
    comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    comp.efficiency = eff2012

for comp in dataSamplesAll:
    comp.splitFactor = 1000
    comp.isMC = False
    comp.isData = True

if __name__ == "__main__":
   import sys
   if "test" in sys.argv:
       from CMGTools.TTHAnalysis.samples.ComponentCreator import testSamples
       testSamples(mcSamples)

