import PhysicsTools.HeppyCore.framework.config as cfg
import os

#####COMPONENT CREATOR

from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

### ----------------------------- 25 ns ----------------------------------------
# TTbar cross section: NNLO, https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO (172.5)
#TTJets = kreator.makeMCComponent("TTJets", "/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 831.76)
TTJets_LO_25ns = kreator.makeMCComponentFromDESY("TTJets_LO_25ns", "/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM", "CMS", ".*root", 831.76)
TTs = [ TTJets_LO_25ns ]

### V+jets inclusive (from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat13TeV)
WJetsToLNu = kreator.makeMCComponentFromDESY("WJetsToLNu","/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 3* 20508.9)
DYJetsToLL_M50 = kreator.makeMCComponentFromDESY("DYJetsToLL_M50", "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/MINIAODSIM", "CMS", ".*root", 2008.*3)

VJets = [ WJetsToLNu, DYJetsToLL_M50 ]

### W+jets
WJetsToLNu_HT100to200 = kreator.makeMCComponentFromDESY("WJetsToLNu_HT100to200", "/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root",1292*1.23)
WJetsToLNu_HT200to400 = kreator.makeMCComponentFromDESY("WJetsToLNu_HT200to400", "/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root",385.9*1.23)
WJetsToLNu_HT400to600 = kreator.makeMCComponentFromDESY("WJetsToLNu_HT400to600", "/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/MINIAODSIM", "CMS", ".*root",47.9*1.23)
WJetsToLNu_HT600toInf = kreator.makeMCComponentFromDESY("WJetsToLNu_HT600toInf", "/WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root",19.9*1.23)
WJetsToLNuHT = [
WJetsToLNu_HT100to200,
WJetsToLNu_HT200to400,
WJetsToLNu_HT400to600,
WJetsToLNu_HT600toInf,
]


### ----------------------------- 50 ns ----------------------------------------
#TTJets_50ns = kreator.makeMCComponentFromDESY("TTJets_50ns", "/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM", "CMS", ".*root", 831.76,)
TTJets_LO_50ns = kreator.makeMCComponentFromDESY("TTJets_LO_50ns", "/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM", "CMS", ".*root", 831.76)

### V+jets inclusive
DYJetsToLL_M50_50ns = kreator.makeMCComponentFromDESY("DYJetsToLL_M50_50ns","/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM", "CMS", ".*root", 2008.*3)

WJetsToLNu_50ns = kreator.makeMCComponentFromDESY("WJetsToLNu_50ns","/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM", "CMS", ".*root", 20508.9*3)


### ----------  Reprocessed PHYS14 SIGNAL samples ------------
# location: /nfs/dust/cms/group/susy-desy/Run2/MC/MiniAOD/RunIISpring15DR74/FromGiovanni
T1tttt_mGo1500_mChi100 = kreator.makePrivateMCComponentFromDir('T1tttt_mGo1500_mChi100', '/T1tttt_mGo1500_mChi100/', '/nfs/dust/cms/group/susy-desy/Run2/MC/MiniAOD/RunIISpring15DR74/FromGiovanni/', '*.root', 0.0141903)
T1tttt_mGo1200_mChi800 = kreator.makePrivateMCComponentFromDir('T1tttt_mGo1200_mChi800', '/T1tttt_mGo1200_mChi800/', '/nfs/dust/cms/group/susy-desy/Run2/MC/MiniAOD/RunIISpring15DR74/FromGiovanni/', '*.root', 0.0856418)
T1tttt_priv = [ T1tttt_mGo1500_mChi100, T1tttt_mGo1200_mChi800 ]

### ----------------------------- summary ----------------------------------------

mcSamples_Asymptotic25ns = TTs + VJets + WJetsToLNuHT

mcSamples_Asymptotic50ns = [ TTJets_LO_50ns, WJetsToLNu_50ns, DYJetsToLL_M50_50ns ]

mcSamples = mcSamples_Asymptotic25ns + mcSamples_Asymptotic50ns

mcSamplesPriv = T1tttt_priv

samples = mcSamples + mcSamplesPriv

dataSamples = []

### ---------------------------------------------------------------------

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
       testSamples(samples)
