import PhysicsTools.HeppyCore.framework.config as cfg
from CMGTools.TTHAnalysis.samples.samples_13TeV_PHYS14 import *
import os

### ----> for the MonoJet analysis
triggers_MonoJet = ["HLT_MonoCentralPFJet140_PFMETNoMu100_PFMHTNoMu140_NoiseCleaned_v*","HLT_MonoCentralPFJet140_PFMETNoMu140_PFMHTNoMu140_NoiseCleaned_v*","HLT_MonoCentralPFJet150_PFMETNoMu150_PFMHTNoMu150_NoiseCleaned_v*"]

#####COMPONENT CREATOR

from CMGTools.TTHAnalysis.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

# MONOJET SAMPLES (dummy xsec)
Monojet_M_10 = kreator.makeMCComponent("Monojet_M_10", '/DarkMatter_Monojet_M-10_AV_Tune4C_13TeV-madgraph/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM', "CMS", ".*root", 0.0189612)

MonoJetSignalSamples = [
Monojet_M_10
]

mcSamplesPHYS14_PU20bx25_monojet = [DYJetsToLL_M50] + MonoJetSignalSamples

mcSamples_monojet = mcSamplesPHYS14_PU20bx25_monojet

from CMGTools.TTHAnalysis.setup.Efficiencies import *


#Define splitting
for comp in mcSamples_monojet:
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

