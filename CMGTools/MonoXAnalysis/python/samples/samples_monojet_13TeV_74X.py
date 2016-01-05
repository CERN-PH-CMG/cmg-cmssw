import PhysicsTools.HeppyCore.framework.config as cfg
import os

#####COMPONENT CREATOR
from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

### common MC samples
from CMGTools.RootTools.samples.samples_13TeV_RunIISpring15MiniAODv2 import *
### DM MC samples
from CMGTools.RootTools.samples.samples_monojet_13TeV_74X_signals import *

### --- mc ---

# --- 25 ns background samples ---
DYJetsToNuNu_MJ = [ DYJetsToNuNu_M50 ] + ZJetsToNuNuHT
VJets_MJ        = [ WJetsToLNu ] + WJetsToLNuHT + DYJetsM50HT + [ DYJetsToLL_M50, DYJetsToLL_M10to50 ]
Top_MJ          = [ TTJets_LO, TToLeptons_tch_amcatnlo, TToLeptons_tch_amcatnlo_ext, TBar_tWch, T_tWch ]
DiBosons_MJ     = [ WW, WZ, ZZ ]

mcSamples_monojet_Asymptotic25ns = DYJetsToNuNu_MJ + VJets_MJ + Top_MJ + DiBosons_MJ + QCDHT
mcSamples_monojet_Asymptotic25ns_signals = DM_Scalars + DM_Pseudoscalars + DM_Vectors + DM_Axials


### ----------------------------- summary ----------------------------------------     
mcSamples_monojet = mcSamples_monojet_Asymptotic25ns + mcSamples_monojet_Asymptotic25ns_signals


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
# for comp in dataSamples:
#     comp.splitFactor = 1000
#     comp.isMC = False
#     comp.isData = True


if __name__ == "__main__":
   import sys
   if "test" in sys.argv:
       from CMGTools.RootTools.samples.ComponentCreator import testSamples
       testSamples(mcSamples)
