from CMGTools.TTHAnalysis.samples.getFiles import getFiles
from CMGTools.TTHAnalysis.samples.getMyFiles import getMyFiles
import CMGTools.RootTools.fwlite.Config as cfg
import os




################## Triggers 


triggers_mumu = ["HLT_Mu17_Mu8_v*","HLT_Mu17_TkMu8_v*"]
triggers_ee   = ["HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*",
                 "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
                 "HLT_Ele15_Ele8_Ele5_CaloIdL_TrkIdVL_v*"]

triggers_mue   = [
    "HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
    "HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*"
    ]

triggersMC_mumu = ["HLT_Mu17_Mu8_v*","HLT_Mu17_TkMu8_v*"]

triggersMC_ee   = ["HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*",
                   "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
                   "HLT_Ele15_Ele8_Ele5_CaloIdL_TrkIdVL_v*"]

triggersMC_mue   = ["HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*",
                    "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
                    "HLT_Ele15_Ele8_Ele5_CaloIdL_TrkIdVL_v*",
                    "HLT_Mu17_Mu8_v*",
                    "HLT_Mu17_TkMu8_v*",
                    "HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
                    "HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*"
                   ]

triggers_1mu = [ 'HLT_IsoMu24_eta2p1_v*' ]
triggersMC_1mu  = triggers_1mu;
triggersFR_1mu  = [ 'HLT_Mu5_v*', 'HLT_RelIso1p0Mu5_v*', 'HLT_Mu12_v*', 'HLT_Mu24_eta2p1_v*', 'HLT_Mu40_eta2p1_v*' ]
triggersFR_mumu = [ 'HLT_Mu17_Mu8_v*', 'HLT_Mu17_TkMu8_v*', 'HLT_Mu8_v*', 'HLT_Mu17_v*' ]
triggersFR_1e   = [ 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*', 'HLT_Ele17_CaloIdL_CaloIsoVL_v*', 'HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*', 'HLT_Ele8__CaloIdL_CaloIsoVL_v*']
triggersFR_mue  = triggers_mue[:]
triggersFR_MC = triggersFR_1mu + triggersFR_mumu + triggersFR_1e + triggersFR_mue


#####COMPONENT CREATOR

from CMGTools.TTHAnalysis.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

## CENTRALLY PRODUCED MINIAODs V2 (from global DBS, in T2_CH_CAF)
### PU40 bx50ns

#### Background samples
DYJetsToLL_M50_HT100to200 = kreator.makeMCComponent("DYJetsToLL_M50_HT100to200", "/DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",194.3*1.27)
DYJetsToLL_M50_HT200to400 = kreator.makeMCComponent("DYJetsToLL_M50_HT200to400", "/DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",52.24*1.27)
DYJetsToLL_M50_HT600toInf = kreator.makeMCComponent("DYJetsToLL_M50_HT600toInf", "/DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",2.179*1.27)
DYJetsM50HT = [
DYJetsToLL_M50_HT100to200,
DYJetsToLL_M50_HT200to400,
DYJetsToLL_M50_HT600toInf,
]
ZJetsToNuNu_HT100to200 = kreator.makeMCComponent("ZJetsToNuNu_HT100to200", "/ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",372.6*1.27)
ZJetsToNuNu_HT200to400 = kreator.makeMCComponent("ZJetsToNuNu_HT200to400", "/ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",100.8*1.27)
ZJetsToNuNu_HT400to600 = kreator.makeMCComponent("ZJetsToNuNu_HT400to600", "/ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",11.99*1.27)
ZJetsToNuNuHT = [
ZJetsToNuNu_HT100to200,
ZJetsToNuNu_HT200to400,
ZJetsToNuNu_HT400to600,
]
WJetsToLNu_HT100to200 = kreator.makeMCComponent("WJetsToLNu_HT100to200", "/WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",1817*1.23)
WJetsToLNu_HT200to400 = kreator.makeMCComponent("WJetsToLNu_HT200to400", "/WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",471.6*1.23)
WJetsToLNu_HT600toInf = kreator.makeMCComponent("WJetsToLNu_HT600toInf", "/WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",18.81*1.23)
WJetsToLNuHT = [
WJetsToLNu_HT100to200,
WJetsToLNu_HT200to400,
WJetsToLNu_HT600toInf,
]
GJets_HT100to200 = kreator.makeMCComponent("GJets_HT100to200", "/GJets_HT-100to200_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",1534)
GJets_HT400to600 = kreator.makeMCComponent("GJets_HT400to600", "/GJets_HT-400to600_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",62.05)
GJetsHT = [
GJets_HT100to200,
GJets_HT400to600,
]
QCD_HT500To1000 = kreator.makeMCComponent("QCD_HT500To1000", "/QCD_HT-500To1000_13TeV-madgraph/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root")
QCDHT = [
QCD_HT500To1000,
]
QCD_Pt1800to2400 = kreator.makeMCComponent("QCD_Pt1800to2400", "/QCD_Pt-1800to2400_Tune4C_13TeV_pythia8/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root")
QCD_Pt2400to3200 = kreator.makeMCComponent("QCD_Pt2400to3200", "/QCD_Pt-2400to3200_Tune4C_13TeV_pythia8/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root")
QCD_Pt3200 = kreator.makeMCComponent("QCD_Pt3200", "/QCD_Pt-3200_Tune4C_13TeV_pythia8/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root")
QCDPt = [
QCD_Pt1800to2400,
QCD_Pt2400to3200,
QCD_Pt3200,
]
TTJets = kreator.makeMCComponent("TTJets", "/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",809.1)
TToLeptons_tch = kreator.makeMCComponent("TToLeptons_tch", "/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root")
WJetsToLNu = kreator.makeMCComponent("WJetsToLNu", "/WJetsToLNu_13TeV-madgraph-pythia8-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root")
WZJetsTo3LNu = kreator.makeMCComponent("WZJetsTo3LNu", "/WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",2.29)


#### Signal samples
SMS_T2qq_2J_mStop600_mLSP550 = kreator.makeMCComponent("SMS_T2qq_2J_mStop600_mLSP550", "/SMS-T2qq_2J_mStop-600_mLSP-550_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",1.76645)
SMS_T2bb_2J_mStop600_mLSP580 = kreator.makeMCComponent("SMS_T2bb_2J_mStop600_mLSP580", "/SMS-T2bb_2J_mStop-600_mLSP-580_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",0.174599)
SMS_T2bb_2J_mStop900_mLSP100 = kreator.makeMCComponent("SMS_T2bb_2J_mStop900_mLSP100", "/SMS-T2bb_2J_mStop-900_mLSP-100_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",0.0128895)
SMS_T2tt_2J_mStop500_mLSP325 = kreator.makeMCComponent("SMS_T2tt_2J_mStop500_mLSP325", "/SMS-T2tt_2J_mStop-500_mLSP-325_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",0.51848)
SMS_T1bbbb_2J_mGl1000_mLSP900 = kreator.makeMCComponent("SMS_T1bbbb_2J_mGl1000_mLSP900", "/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM", "CMS", ".*root",0.325388)
SusySignalSamples = [
SMS_T2qq_2J_mStop600_mLSP550,
SMS_T2bb_2J_mStop600_mLSP580,
SMS_T2bb_2J_mStop900_mLSP100,
SMS_T2tt_2J_mStop500_mLSP325,
SMS_T1bbbb_2J_mGl1000_mLSP900,
]

mcSamplesCSA14v2 = DYJetsM50HT + ZJetsToNuNuHT + WJetsToLNuHT + GJetsHT + QCDHT +  QCDPt + [TTJets, TToLeptons_tch, WJetsToLNu, WZJetsTo3LNu]  + SusySignalSamples

## MORE private samples on EOS
### PU40 bx50ns
#### Signal samples
T5WW_2J_mGo1200_mCh1000_mChi800 = kreator.makeMCComponentFromEOS('T5WW_2J_mGo1200_mCh1000_mChi800', '/T5Full-1200-1000-800-Decay-MGMMatch50/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s',".*root",0.0856418)
T5WW_2J_mGo1500_mCh800_mChi100  = kreator.makeMCComponentFromEOS('T5WW_2J_mGo1500_mCh800_mChi100', '/T5Full-1500-800-100-Decay-MGMMatch50/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s',".*root",0.0141903)
T5WW_2J_mGo1400_mCh315_mChi300 = kreator.makePrivateMCComponent('T5WW_2J_mGo1400_mCh315_mChi300','', ["/store/cmst3/group/susy/alobanov/MC/MiniAOD_v2/13TeV_T5qqqqWW_Gl_1400_LSP_300_Chi_315/13TeV_T5qqqqWW_Gl_1400_LSP_300_Chi_315_MiniAOD-v2.root"],0.0252977)

T1tttt_2J_mGo1000_mStop300_mCh285_mChi280 = kreator.makeMCComponentFromEOS('T1tttt_2J_mGo1000_mStop300_mCh285_mChi280', '/T1tttt_2J_mGo1000_mStop300_mCh285_mChi280_pythia8-23bodydec/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s',".*root",0.325388)
T1tttt_2J_mGo1300_mStop300_mCh285_mChi280 = kreator.makeMCComponentFromEOS('T1tttt_2J_mGo1300_mStop300_mCh285_mChi280', '/T1tttt_2J_mGo1300_mStop300_mCh285_mChi280_pythia8-23bodydec/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s',".*root",0.0460525)
T1tttt_2J_mGo1300_mStop300_mChi280 = kreator.makeMCComponentFromEOS('T1tttt_2J_mGo1300_mStop300_mChi280', '/T1tttt_2J_mGo1300_mStop300_mChi280_pythia8-4bodydec/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s/',".*root",0.0460525)
T1tttt_2J_mGo800_mStop300_mCh285_mChi280 = kreator.makeMCComponentFromEOS('T1tttt_2J_mGo800_mStop300_mCh285_mChi280', '/T1tttt_2J_mGo800_mStop300_mCh285_mChi280_pythia8-23bodydec/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s',".*root",1.4891)
T1tttt_2J_mGo800_mStop300_mChi280 = kreator.makeMCComponentFromEOS('T1tttt_2J_mGo800_mStop300_mChi280', '/T1tttt_2J_mGo800_mStop300_mChi280_pythia8-4bodydec/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s',".*root",1.4891)
T1tttt_2J_mGo1000_mStop300_mCh285_mChi280_dilep = kreator.makeMCComponentFromEOS('T1tttt_2J_mGo1000_mStop300_mCh285_mChi280_dilep', '/T1tttt_2J_mGo1000_mStop300_mCh285_mChi280_23bodydec_dilepfilter/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s',".*root",0.325388*(40.3244/20.9656)*(3596.7/28378))
T1tttt_2J_mGo1300_mStop300_mCh285_mChi280_dilep = kreator.makeMCComponentFromEOS('T1tttt_2J_mGo1300_mStop300_mCh285_mChi280_dilep', '/T1tttt_2J_mGo1300_mStop300_mCh285_mChi280_23bodydec_dilepfilter/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s',".*root",0.0460525*(48.03625/22.0665)*(6699.05/89779))
T6ttWW_2J_mSbot600_mCh425_mChi50 = kreator.makeMCComponentFromEOS('T6ttWW_2J_mSbot600_mCh425_mChi50', '/T6ttWW_600_425_50/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s',".*root",0.174599)
T6ttWW_2J_mSbot650_mCh150_mChi50 = kreator.makeMCComponentFromEOS('T6ttWW_2J_mSbot650_mCh150_mChi50', '/T6ttWW_650_150_50/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s',".*root",0.107045)

T1ttbb_2J_mGo1500_mChi100 = kreator.makeMCComponentFromEOS('T1ttbb_2J_mGo1500_mChi100','T1ttbb_2J_mGo1500_mChi100_3bodydec_asymmDecOnly','/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s',".*root",0.0141903)

mcPrivateSamplesCSA14v2 = [T5WW_2J_mGo1200_mCh1000_mChi800, T5WW_2J_mGo1500_mCh800_mChi100, T5WW_2J_mGo1400_mCh315_mChi300, T1tttt_2J_mGo1000_mStop300_mCh285_mChi280, T1tttt_2J_mGo1300_mStop300_mCh285_mChi280, T1tttt_2J_mGo1300_mStop300_mChi280, T1tttt_2J_mGo800_mStop300_mCh285_mChi280, T1tttt_2J_mGo800_mStop300_mChi280, T1tttt_2J_mGo1000_mStop300_mCh285_mChi280_dilep, T1tttt_2J_mGo1300_mStop300_mCh285_mChi280_dilep, T6ttWW_2J_mSbot600_mCh425_mChi50, T6ttWW_2J_mSbot650_mCh150_mChi50,T1ttbb_2J_mGo1500_mChi100]

mcSamples = mcSamplesCSA14v2 + mcPrivateSamplesCSA14v2

### Samples from ACDV group
T1ttbbWW_2J_mGo1000_mCh725_mChi715_3bodydec = kreator.makeMCComponentFromEOS('T1ttbbWW_2J_mGo1000_mCh725_mChi715_3bodydec', '/T1ttbbWW_2J_mGo1000_mCh725_mChi715_3bodydec/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s',".*root", 0.325388)
T1ttbbWW_2J_mGo1000_mCh725_mChi720_3bodydec = kreator.makeMCComponentFromEOS('T1ttbbWW_2J_mGo1000_mCh725_mChi720_3bodydec', '/T1ttbbWW_2J_mGo1000_mCh725_mChi720_3bodydec/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s', ".*root", 0.325388)
T1ttbbWW_2J_mGo1300_mCh300_mChi290_3bodydec = kreator.makeMCComponentFromEOS('T1ttbbWW_2J_mGo1300_mCh300_mChi290_3bodydec', '/T1ttbbWW_2J_mGo1300_mCh300_mChi290_3bodydec/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s', ".*root", 0.0460525)
T1ttbbWW_2J_mGo1300_mCh300_mChi295_3bodydec = kreator.makeMCComponentFromEOS('T1ttbbWW_2J_mGo1300_mCh300_mChi295_3bodydec', '/T1ttbbWW_2J_mGo1300_mCh300_mChi295_3bodydec/', '/store/cmst3/group/susy/gpetrucc/13TeV/MINIAODSIM/%s', ".*root", 0.0460525)

T1tttt_gluino_1300_LSP_100 = kreator.makeMCComponentFromEOS('T1tttt_gluino_1300_LSP_100', '/13TeV_T1tttt_gluino_1300_LSP_100/', '/store/cmst3/group/susy/alobanov/MC/MiniAOD_v2/%s', ".*root",0.0460525)
T1tttt_gluino_800_LSP_450 = kreator.makeMCComponentFromEOS('T1tttt_gluino_800_LSP_450', '/13TeV_T1tttt_gluino_800_LSP_450/', '/store/cmst3/group/susy/alobanov/MC/MiniAOD_v2/%s', ".*root", 1.4891)
T5qqqqWW_Gl_1400_LSP_100_Chi_325 = kreator.makeMCComponentFromEOS('T5qqqqWW_Gl_1400_LSP_100_Chi_325', '/13TeV_T5qqqqWW_Gl_1400_LSP_100_Chi_325/', '/store/cmst3/group/susy/alobanov/MC/MiniAOD_v2/%s', ".*root", 0.0252977)
T5qqqqWW_Gl_1400_LSP_300_Chi_315 = kreator.makeMCComponentFromEOS('T5qqqqWW_Gl_1400_LSP_300_Chi_315', '/13TeV_T5qqqqWW_Gl_1400_LSP_300_Chi_315/', '/store/cmst3/group/susy/alobanov/MC/MiniAOD_v2/%s', ".*root", 0.0252977)

#SqGltttt_Gl_1300_Sq_1300_LSP_100 = kreator.makeMCComponentFromEOS('SqGltttt_Gl_1300_Sq_1300_LSP_100', '/13TeV_SqGltttt_Gl_1300_Sq_1300_LSP_100/', '/store/cmst3/group/susy/alobanov/MC/MiniAOD_v2/%s')
T6qqWW_Sq_950_LSP_300_Chi_325 =kreator.makeMCComponentFromEOS('T6qqWW_Sq_950_LSP_300_Chi_325', '/13TeV_T6qqWW_Sq_950_LSP_300_Chi_325/', '/store/cmst3/group/susy/alobanov/MC/MiniAOD_v2/%s', ".*root", 0.0898112)
    
acdvSignals= [T1ttbbWW_2J_mGo1000_mCh725_mChi715_3bodydec, T1ttbbWW_2J_mGo1000_mCh725_mChi720_3bodydec, T1ttbbWW_2J_mGo1300_mCh300_mChi290_3bodydec, T1ttbbWW_2J_mGo1300_mCh300_mChi295_3bodydec, T1tttt_gluino_1300_LSP_100, T1tttt_gluino_800_LSP_450, T5qqqqWW_Gl_1400_LSP_100_Chi_325, T5qqqqWW_Gl_1400_LSP_300_Chi_315, T6qqWW_Sq_950_LSP_300_Chi_325]
mcSamples+=acdvSignals


#-----------DATA---------------

dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/TTHAnalysis/data"
#lumi: 12.21+7.27+0.134 = 19.62 /fb @ 8TeV

json=dataDir+'/json/Cert_Run2012ABCD_22Jan2013ReReco.json'

SingleMu = cfg.DataComponent(
    name = 'SingleMu',
    files = kreator.getFilesFromEOS('SingleMu', 
                                    '/SingleMu/Run2012D-15Apr2014-v1/AOD/02e0a1be-c9c7-11e3-bfe2-0024e83ef644/MINIAOD/CMSSW_7_0_9_patch2_GR_70_V2_AN1',
                                    '/eos/cms/store/cmst3/user/cmgtools/CMG/%s'),
    intLumi = 1,
    triggers = [],
    json = json
    )

           
dataSamplesMu=[]
dataSamplesE=[]
dataSamplesMuE=[]
dataSamples1Mu=[SingleMu]
dataSamplesAll = dataSamplesMu+dataSamplesE+dataSamplesMuE+dataSamples1Mu


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
