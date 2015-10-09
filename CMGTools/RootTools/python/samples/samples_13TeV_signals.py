import PhysicsTools.HeppyCore.framework.config as cfg
import os

#####COMPONENT CREATOR

from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

### Signals, 25 ns

ZprimeToTT_M2000_W200 = kreator.makeMCComponent("ZprimeToTT_M2000_W200", "/ZprimeToTT_M-2000_W-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
ZprimeToTT_M2000_W20 = kreator.makeMCComponent("ZprimeToTT_M2000_W20", "/ZprimeToTT_M-2000_W-20_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
ZprimeToTT_M2000_W600 = kreator.makeMCComponent("ZprimeToTT_M2000_W600", "/ZprimeToTT_M-2000_W-600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)

ZprimeToTT_M3000_W300 = kreator.makeMCComponent("ZprimeToTT_M3000_W300", "/ZprimeToTT_M-3000_W-300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
ZprimeToTT_M3000_W30 = kreator.makeMCComponent("ZprimeToTT_M3000_W30", "/ZprimeToTT_M-3000_W-30_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
ZprimeToTT_M3000_W900 = kreator.makeMCComponent("ZprimeToTT_M3000_W900", "/ZprimeToTT_M-3000_W-900_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)

ZprimeToTauTau_M2000 = kreator.makeMCComponent("ZprimeToTauTau_M2000", "/ZprimeToTauTau_M_2000_TuneCUETP8M1_tauola_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
ZprimeToTauTau_M3000= kreator.makeMCComponent("ZprimeToTauTau_M3000", "/ZprimeToTauTau_M_3000_TuneCUETP8M1_tauola_13TeV_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)

ZprimeToTprimeT_TprimeToHT_MZp2000Nar_MTp1200Nar_LH = kreator.makeMCComponent("ZprimeToTprimeT_TprimeToHT_MZp2000Nar_MTp1200Nar_LH", "/ZprimeToTprimeT_TprimeToHT_MZp-2000Nar_MTp-1200Nar_LH_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)

ZprimeToWW_narrow_M2000 = kreator.makeMCComponent("ZprimeToWW_narrow_M2000", "/ZprimeToWW_narrow_M-2000_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
ZprimeToWW_narrow_M3000 = kreator.makeMCComponent("ZprimeToWW_narrow_M3000", "/ZprimeToWW_narrow_M-3000_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
ZprimeToWW_narrow_M4500 = kreator.makeMCComponent("ZprimeToWW_narrow_M4500", "/ZprimeToWW_narrow_M-4500_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)

ZprimeToZhToZinvhbb_narrow_M2000 = kreator.makeMCComponent("ZprimeToZhToZinvhbb_narrow_M2000", "/ZprimeToZhToZinvhbb_narrow_M-2000_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
ZprimeToZhToZinvhbb_narrow_M3000 = kreator.makeMCComponent("ZprimeToZhToZinvhbb_narrow_M3000", "/ZprimeToZhToZinvhbb_narrow_M-3000_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
ZprimeToZhToZinvhbb_narrow_M4500 = kreator.makeMCComponent("ZprimeToZhToZinvhbb_narrow_M4500", "/ZprimeToZhToZinvhbb_narrow_M-4500_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)

WprimeToMuNu_M2000 = kreator.makeMCComponent("WprimeToMuNu_M2000", "/WprimeToMuNu_M-2000_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
WprimeToENu_M2000 = kreator.makeMCComponent("WprimeToENu_M2000", "/WprimeToENu_M-2000_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
WprimeToTauNu_M2000 = kreator.makeMCComponent("WprimeToTauNu_M2000", "/WprimeToTauNu_M-2000_TuneCUETP8M1_13TeV-pythia8-tauola/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)

WprimeToWZ_M2000 = kreator.makeMCComponent("WprimeToWZ_M2000", "/WprimeToWZ_M-2000_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
WprimeToWhToWlephbb_narrow_M2000 = kreator.makeMCComponent("WprimeToWhToWlephbb_narrow_M2000", "/WprimeToWhToWlephbb_narrow_M-2000_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
WprimeToTB_plusSM_TToLep_M2000  = kreator.makeMCComponent("WprimeToTB_plusSM_TToLep_M2000", "/WprimeToTB_plusSM_TToLep_M-2000_LH_TuneCUETP8M1_13TeV-comphep-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
WprimeToTB_TToLep_M2000 = kreator.makeMCComponent("WprimeToTB_TToLep_M2000", "/WprimeToTB_TToLep_M-2000_RH_TuneCUETP8M1_13TeV-comphep-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)

SignalEXO = [ ZprimeToTT_M2000_W200, ZprimeToTT_M2000_W20, ZprimeToTT_M2000_W600, ZprimeToTT_M3000_W300, ZprimeToTT_M3000_W30, ZprimeToTT_M3000_W900, ZprimeToTauTau_M2000, ZprimeToTauTau_M3000, ZprimeToTprimeT_TprimeToHT_MZp2000Nar_MTp1200Nar_LH, ZprimeToWW_narrow_M2000, ZprimeToWW_narrow_M3000, ZprimeToWW_narrow_M4500, ZprimeToZhToZinvhbb_narrow_M2000, ZprimeToZhToZinvhbb_narrow_M3000, ZprimeToZhToZinvhbb_narrow_M4500, WprimeToMuNu_M2000, WprimeToENu_M2000, WprimeToTauNu_M2000, WprimeToWZ_M2000, WprimeToWhToWlephbb_narrow_M2000, WprimeToTB_plusSM_TToLep_M2000, WprimeToTB_TToLep_M2000 ]

#BulkGravToZZToZlepZhad_narrow_M2000 = kreator.makeMCComponent("BulkGravToZZToZlepZhad_narrow_M2000", "/BulkGravToZZToZlepZhad_narrow_M-2000_13TeV-madgraph/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
#RSGravToZZ_kMpl01_M2000 = kreator.makeMCComponent("RSGravToZZ_kMpl01_M2000", "/RSGravToZZ_kMpl01_M-2000_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
#
#SignalEXO_50ns = SignalEX0_50ns + [ BulkGravToZZToZlepZhad_narrow_M2000, RSGravToZZ_kMpl01_M2000 ]

SMS_T1bbbb_mGluino1000_mLSP900 = kreator.makeMCComponent("SMS_T1bbbb_mGluino1000_mLSP900", "/SMS-T1bbbb_mGluino-1000_mLSP-900_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 0.325388, useAAA=True)
SMS_T1bbbb_mGluino1500_mLSP100 = kreator.makeMCComponent("SMS_T1bbbb_mGluino1500_mLSP100", "/SMS-T1bbbb_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 0.0141903, useAAA=True)
SMS_T1qqqq_mGluino1000_mLSP800 = kreator.makeMCComponent("SMS_T1qqqq_mGluino1000_mLSP800", "/SMS-T1qqqq_mGluino-1000_mLSP-800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 0.325388, useAAA=True)
SMS_T1qqqq_mGluino1400_mLSP100 = kreator.makeMCComponent("SMS_T1qqqq_mGluino1400_mLSP100", "/SMS-T1qqqq_mGluino-1400_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 0.0252977, useAAA=True)
SMS_T1tttt_mGluino1200_mLSP800 = kreator.makeMCComponent("SMS_T1tttt_mGluino1200_mLSP800", "/SMS-T1tttt_mGluino-1200_mLSP-800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 0.0856418, useAAA=True) 
SMS_T1tttt_mGluino1500_mLSP100 = kreator.makeMCComponent("SMS_T1tttt_mGluino1500_mLSP100", "/SMS-T1tttt_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 0.0141903, useAAA=True)

SignalSUSY = [ SMS_T1bbbb_mGluino1000_mLSP900, SMS_T1bbbb_mGluino1500_mLSP100, SMS_T1qqqq_mGluino1000_mLSP800, SMS_T1qqqq_mGluino1400_mLSP100, SMS_T1tttt_mGluino1200_mLSP800, SMS_T1tttt_mGluino1500_mLSP100 ]


### ----------------------------- summary ----------------------------------------


signalSamples = SignalEXO + SignalSUSY

samples = signalSamples

### ---------------------------------------------------------------------

from CMGTools.TTHAnalysis.setup.Efficiencies import *
dataDir = "$CMSSW_BASE/src/CMGTools/TTHAnalysis/data"

#Define splitting
for comp in signalSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 250 #  if comp.name in [ "WJets", "DY3JetsM50", "DY4JetsM50","W1Jets","W2Jets","W3Jets","W4Jets","TTJetsHad" ] else 100
    comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    comp.efficiency = eff2012

if __name__ == "__main__":
   import sys
   if "test" in sys.argv:
       from CMGTools.RootTools.samples.ComponentCreator import testSamples
       testSamples(samples)
