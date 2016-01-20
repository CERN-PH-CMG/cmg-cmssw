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

SMS_T1bbbb_mGluino1000_1025_mLSP1to975_1000 = kreator.makeMCComponent("SMS_T1bbbb_mGluino1000_1025_mLSP1to975_1000", "/SMS-T1bbbb_mGluino-1000-1025_mLSP-1to975-1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15FSPremix-MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
SMS_T1bbbb_mGluino1450_1500_mLSP250to1400_1350to1450 = kreator.makeMCComponent("SMS_T1bbbb_mGluino1450_1500_mLSP250to1400_1350to1450", "/SMS-T1bbbb_mGluino-1450-1500_mLSP-250to1400-1350to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15FSPremix-MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
SMS_T1bbbb_mGluino1500_1550_mLSP1to1300_1000to1300 = kreator.makeMCComponent("SMS_T1bbbb_mGluino1500_1550_mLSP1to1300_1000to1300", "/SMS-T1bbbb_mGluino-1500-1550_mLSP-1to1300-1000to1300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15FSPremix-MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)

SignalSUSYFullScan = [ SMS_T1bbbb_mGluino1000_1025_mLSP1to975_1000, SMS_T1bbbb_mGluino1500_1550_mLSP1to1300_1000to1300 ]


#SMS_T2bb_2J_mStop600_mLSP580 = kreator.makeMyPrivateMCComponent("SMS_T2bb_2J_mStop600_mLSP580", "/SMS-T2bb_2J_mStop-600_mLSP-580_Tune4C_13TeV-madgraph-tauola/namin-step3-fb89f44b0d6970d718ed21d513cd1c9d/USER", "PRIVATE", ".*root", "phys03", 0.174599, useAAA=True)
#SMS_T2bb_2J_mStop900_mLSP100 = kreator.makeMyPrivateMCComponent("SMS_T2bb_2J_mStop900_mLSP100", "/SMS-T2bb_2J_mStop-900_mLSP-100_Tune4C_13TeV-madgraph-tauola/namin-step3-fb89f44b0d6970d718ed21d513cd1c9d/USER", "PRIVATE", ".*root", "phys03", 0.0128895, useAAA=True)
#SMS_T2qq_2J_mStop1200_mLSP100 = kreator.makeMyPrivateMCComponent("SMS_T2qq_2J_mStop1200_mLSP100", "/SMS-T2qq_2J_mStop-1200_mLSP-100_Tune4C_13TeV-madgraph-tauola/namin-step3-fb89f44b0d6970d718ed21d513cd1c9d/USER", "PRIVATE", ".*root", "phys03", 0.0162846, useAAA=True)
#SMS_T2qq_2J_mStop600_mLSP550 = kreator.makeMyPrivateMCComponent("SMS_T2qq_2J_mStop600_mLSP550", "/SMS-T2qq_2J_mStop-600_mLSP-550_Tune4C_13TeV-madgraph-tauola/namin-step3-fb89f44b0d6970d718ed21d513cd1c9d/USER", "PRIVATE", ".*root", "phys03", 1.76645, useAAA=True)
#SMS_T2tt_2J_mStop425_mLSP325 = kreator.makeMyPrivateMCComponent("SMS_T2tt_2J_mStop425_mLSP325", "/SMS-T2tt_2J_mStop-425_mLSP-325_Tune4C_13TeV-madgraph-tauola/namin-step3-fb89f44b0d6970d718ed21d513cd1c9d/USER", "PRIVATE", ".*root", "phys03", 1.31169, useAAA=True)
#SMS_T2tt_2J_mStop500_mLSP325 = kreator.makeMyPrivateMCComponent("SMS_T2tt_2J_mStop500_mLSP325", "/SMS-T2tt_2J_mStop-500_mLSP-325_Tune4C_13TeV-madgraph-tauola/namin-step3-fb89f44b0d6970d718ed21d513cd1c9d/USER", "PRIVATE", ".*root", "phys03", 0.51848, useAAA=True)
#SMS_T2tt_2J_mStop650_mLSP325 = kreator.makeMyPrivateMCComponent("SMS_T2tt_2J_mStop650_mLSP325", "/SMS-T2tt_2J_mStop-650_mLSP-325_Tune4C_13TeV-madgraph-tauola/namin-step3-fb89f44b0d6970d718ed21d513cd1c9d/USER", "PRIVATE", ".*root", "phys03", 0.107045, useAAA=True)
#SMS_T2tt_2J_mStop850_mLSP100 = kreator.makeMyPrivateMCComponent("SMS_T2tt_2J_mStop850_mLSP100", "/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola/namin-step3-fb89f44b0d6970d718ed21d513cd1c9d/USER", "PRIVATE", ".*root", "phys03", 0.0189612, useAAA=True)
#
#SignalSUSYPrivate = [ SMS_T2bb_2J_mStop600_mLSP580, SMS_T2bb_2J_mStop900_mLSP100, SMS_T2qq_2J_mStop1200_mLSP100, SMS_T2qq_2J_mStop600_mLSP550, SMS_T2tt_2J_mStop425_mLSP325, SMS_T2tt_2J_mStop500_mLSP325, SMS_T2tt_2J_mStop650_mLSP325, SMS_T2tt_2J_mStop850_mLSP100 ] 

SMS_miniAODv2_T1tttt = [ # warning: not all points are there yet
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1475to1500_mLSP_1to1250","/SMS-T1tttt_mGluino-1475to1500_mLSP-1to1250_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_800to825_mLSP_1to575","/SMS-T1tttt_mGluino-800to825_mLSP-1to575_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_600_mLSP_250to325","/SMS-T1tttt_mGluino-600_mLSP-250to325_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1400_mLSP_1to1175","/SMS-T1tttt_mGluino-1400_mLSP-1to1175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1100to1125_mLSP_700to900","/SMS-T1tttt_mGluino-1100to1125_mLSP-700to900_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_675_mLSP_325to450","/SMS-T1tttt_mGluino-675_mLSP-325to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1175_mLSP_950","/SMS-T1tttt_mGluino-1175_mLSP-950_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_950to975_mLSP_350to725","/SMS-T1tttt_mGluino-950to975_mLSP-350to725_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1250to1275_mLSP_700to1050","/SMS-T1tttt_mGluino-1250to1275_mLSP-700to1050_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_650to675_mLSP_250to425","/SMS-T1tttt_mGluino-650to675_mLSP-250to425_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_625to650_mLSP_200to400","/SMS-T1tttt_mGluino-625to650_mLSP-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1000_mLSP_1to700","/SMS-T1tttt_mGluino-1000_mLSP-1to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1325to1350_mLSP_1to1125","/SMS-T1tttt_mGluino-1325to1350_mLSP-1to1125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1350to1375_mLSP_50to1025","/SMS-T1tttt_mGluino-1350to1375_mLSP-50to1025_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_975_mLSP_600to750","/SMS-T1tttt_mGluino-975_mLSP-600to750_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_750to775_mLSP_350to525","/SMS-T1tttt_mGluino-750to775_mLSP-350to525_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1525to1550_mLSP_1to1300","/SMS-T1tttt_mGluino-1525to1550_mLSP-1to1300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1375_mLSP_950to1150","/SMS-T1tttt_mGluino-1375_mLSP-950to1150_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_825to850_mLSP_200to600","/SMS-T1tttt_mGluino-825to850_mLSP-200to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1450to1475_mLSP_50to1075","/SMS-T1tttt_mGluino-1450to1475_mLSP-50to1075_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1200_mLSP_1to825","/SMS-T1tttt_mGluino-1200_mLSP-1to825_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1100_mLSP_1to775","/SMS-T1tttt_mGluino-1100_mLSP-1to775_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_850to875_mLSP_450to625","/SMS-T1tttt_mGluino-850to875_mLSP-450to625_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1700to1750_mLSP_1to1450","/SMS-T1tttt_mGluino-1700to1750_mLSP-1to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1000to1050_mLSP_1to800","/SMS-T1tttt_mGluino-1000to1050_mLSP-1to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1300to1325_mLSP_700to1100","/SMS-T1tttt_mGluino-1300to1325_mLSP-700to1100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1300_mLSP_1to1075","/SMS-T1tttt_mGluino-1300_mLSP-1to1075_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1800to1850_mLSP_1to1450","/SMS-T1tttt_mGluino-1800to1850_mLSP-1to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1150to1175_mLSP_750to925","/SMS-T1tttt_mGluino-1150to1175_mLSP-750to925_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_625_mLSP_275to375","/SMS-T1tttt_mGluino-625_mLSP-275to375_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_875to900_mLSP_1to650","/SMS-T1tttt_mGluino-875to900_mLSP-1to650_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1900to1950_mLSP_0to1450","/SMS-T1tttt_mGluino-1900to1950_mLSP-0to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1500to1525_mLSP_50to1125","/SMS-T1tttt_mGluino-1500to1525_mLSP-50to1125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1650to1700_mLSP_1to1400","/SMS-T1tttt_mGluino-1650to1700_mLSP-1to1400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_775_mLSP_475to550","/SMS-T1tttt_mGluino-775_mLSP-475to550_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1850to1900_mLSP_1to1450","/SMS-T1tttt_mGluino-1850to1900_mLSP-1to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_700_mLSP_1to450","/SMS-T1tttt_mGluino-700_mLSP-1to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1950_mLSP_700to950","/SMS-T1tttt_mGluino-1950_mLSP-700to950_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1750_mLSP_50to1450","/SMS-T1tttt_mGluino-1750_mLSP-50to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1425to1450_mLSP_1to1200","/SMS-T1tttt_mGluino-1425to1450_mLSP-1to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1275_mLSP_900to975","/SMS-T1tttt_mGluino-1275_mLSP-900to975_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1600to1650_mLSP_1to1350","/SMS-T1tttt_mGluino-1600to1650_mLSP-1to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1150_mLSP_1to800","/SMS-T1tttt_mGluino-1150_mLSP-1to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1400to1425_mLSP_50to1100","/SMS-T1tttt_mGluino-1400to1425_mLSP-50to1100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_1200to1225_mLSP_800to1000","/SMS-T1tttt_mGluino-1200to1225_mLSP-800to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
kreator.makeMCComponent("SMS_miniAODv2_T1tttt_mGluino_700to750_mLSP_200to500","/SMS-T1tttt_mGluino-700to750_mLSP-200to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/MINIAODSIM","CMS",".*root",1),
]

### ----------------------------- summary ----------------------------------------


signalSamples = SignalEXO + SignalSUSY + SignalSUSYFullScan + SMS_miniAODv2_T1tttt #+ SignalSUSYPrivate

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
