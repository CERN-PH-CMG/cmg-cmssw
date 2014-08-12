from CMGTools.TTHAnalysis.samples.getFiles import getFiles
import CMGTools.RootTools.fwlite.Config as cfg
import os




################## Triggers (FIXME: update!)


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

## CENTRALLY PRODUCED MINIAODs (from global DBS, in T2_CH_CAF)
DYJetsM50_PU20bx25 = kreator.makeMCComponent("DYJetsM50_PU20bx25", "/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
DYJetsM50pythia6_PU20bx25 = kreator.makeMCComponent("DYJetsM50pythia6_PU20bx25", "/DYJetsToLL_M-50_13TeV-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
DYJetsM50_HT200to400_PU20bx25 = kreator.makeMCComponent("DYJetsM50_HT200to400_PU20bx25", "/DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
DYJetsM50_HT400to600_PU20bx25 = kreator.makeMCComponent("DYJetsM50_HT400to600_PU20bx25", "/DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
DYJetsM50_HT600toInf_PU20bx25 = kreator.makeMCComponent("DYJetsM50_HT600toInf_PU20bx25", "/DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
DYJetsMuMuM50_PtZ180_PU20bx25 = kreator.makeMCComponent("DYJetsMuMuM50_PtZ180_PU20bx25", "/DYJetsToMuMu_PtZ-180_M-50_13TeV-madgraph/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
DYJetsMuMuM6pythia8_PU20bx25 = kreator.makeMCComponent("DYJetsMuMuM6pythia8_PU20bx25", "/DYToMuMu_M-6To15_Tune4C_13TeV-pythia8/Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
DYJetsMuMuM15pythia8_PU20bx25 = kreator.makeMCComponent("DYJetsMuMuM15pythia8_PU20bx25", "/DYToMuMu_M-15To50_Tune4C_13TeV-pythia8/Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
DYJetsMuMuM50pythia8_PU20bx25 = kreator.makeMCComponent("DYJetsMuMuM50pythia8_PU20bx25", "/DYToMuMu_M-50_Tune4C_13TeV-pythia8/Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
DYJetsEEpythia8_PU20bx25 = kreator.makeMCComponent("DYJetsEEpythia8_PU20bx25", "/DYToEE_Tune4C_13TeV-pythia8/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
DYJetsMuMupythia8_PU20bx25 = kreator.makeMCComponent("DYJetsMuMupythia8_PU20bx25", "/DYToMuMu_Tune4C_13TeV-pythia8/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
EWKWmin_PU20bx25 = kreator.makeMCComponent("EWKWmin_PU20bx25", "/EWKWmin_mqq120_mnl50_13TeV_madgraph-pythia8/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
EWKWplus_PU20bx25 = kreator.makeMCComponent("EWKWplus_PU20bx25", "/EWKWplus_mqq120_mnl50_13TeV_madgraph-pythia8/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
EWKZjj_PU20bx25 = kreator.makeMCComponent("EWKZjj_PU20bx25", "/EWKZjj_mqq120_mll50_13TeV_madgraph-pythia8/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
EleGun_PU20bx25 = kreator.makeMCComponent("EleGun_PU20bx25", "/Electron_Pt-2to250_gun/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
GGHTauTau_PU20bx25 = kreator.makeMCComponent("GGHTauTau_PU20bx25", "/GluGluToHToTauTau_M-125_13TeV-powheg-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
GGHZZ4L_PU20bx25 = kreator.makeMCComponent("GGHZZ4L_PU20bx25", "/GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
GJet_PU20bx25 = kreator.makeMCComponent("GJet_PU20bx25", "/GJet_Pt-15to3000_Tune4C_13TeV_pythia8/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
JPsiPt20_PU20bx25 = kreator.makeMCComponent("JPsiPt20_PU20bx25", "/JpsiToMuMu_JPsiPt20WithFSR_13TeV-pythia6-evtgen/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
JPsiPt7_PU20bx25 = kreator.makeMCComponent("JPsiPt7_PU20bx25", "/JpsiToMuMu_JPsiPt7WithFSR_13TeV-pythia6-evtgen/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
MinBias_PU20bx25 = kreator.makeMCComponent("MinBias_PU20bx25", "/MinBias_TuneZ2star_13TeV_pythia6/Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
MuMinGunPt100_PU20bx25 = kreator.makeMCComponent("MuMinGunPt100_PU20bx25", "/Muminus_Pt100-gun/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
MuMinGunPt10_PU20bx25 = kreator.makeMCComponent("MuMinGunPt10_PU20bx25", "/Muminus_Pt10-gun/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
MuPlusGunPt100_PU20bx25 = kreator.makeMCComponent("MuPlusGunPt100_PU20bx25", "/Muplus_Pt100-gun/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
MuPlusGunPt10_PU20bx25 = kreator.makeMCComponent("MuPlusGunPt10_PU20bx25", "/Muplus_Pt10-gun/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
NeutrinoGun_PU20bx25 = kreator.makeMCComponent("NeutrinoGun_PU20bx25", "/Neutrino_Pt-2to20_gun/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
QCDEM_20to30_PU20bx25 = kreator.makeMCComponent("QCDEM_20to30_PU20bx25", "/QCD_Pt-20to30_EMEnriched_Tune4C_13TeV_pythia8/Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
QCDEM_30to80_PU20bx25 = kreator.makeMCComponent("QCDEM_30to80_PU20bx25", "/QCD_Pt-30to80_EMEnriched_Tune4C_13TeV_pythia8/Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
QCDEM_80to170_PU20bx25 = kreator.makeMCComponent("QCDEM_80to170_PU20bx25", "/QCD_Pt-80to170_EMEnriched_Tune4C_13TeV_pythia8/Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
QCDMu_20to30_PU20bx25 = kreator.makeMCComponent("QCDMu_20to30_PU20bx25", "/QCD_Pt-20to30_MuEnrichedPt5_Tune4C_13TeV_pythia8/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
QCDMu_30to50_PU20bx25 = kreator.makeMCComponent("QCDMu_30to50_PU20bx25", "/QCD_Pt-30to50_MuEnrichedPt5_Tune4C_13TeV_pythia8/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
QCDMu_50to80_PU20bx25 = kreator.makeMCComponent("QCDMu_50to80_PU20bx25", "/QCD_Pt-50to80_MuEnrichedPt5_Tune4C_13TeV_pythia8/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
QCDMu_80to120_PU20bx25 = kreator.makeMCComponent("QCDMu_80to120_PU20bx25", "/QCD_Pt-80to120_MuEnrichedPt5_Tune4C_13TeV_pythia8/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
QCDMu_pythia6_120to170_PU20bx25 = kreator.makeMCComponent("QCDMu_pythia6_120to170_PU20bx25", "/QCD_Pt-120to170_MuEnrichedPt5_TuneZ2star_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
QCDMu_pythia6_20to30_PU20bx25 = kreator.makeMCComponent("QCDMu_pythia6_20to30_PU20bx25", "/QCD_Pt-20to30_MuEnrichedPt5_TuneZ2star_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
QCDMu_pythia6_30to50_PU20bx25 = kreator.makeMCComponent("QCDMu_pythia6_30to50_PU20bx25", "/QCD_Pt-30to50_MuEnrichedPt5_TuneZ2star_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
QCDMu_pythia6_50to80_PU20bx25 = kreator.makeMCComponent("QCDMu_pythia6_50to80_PU20bx25", "/QCD_Pt-50to80_MuEnrichedPt5_TuneZ2star_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
QCDMu_pythia6_80to120_PU20bx25 = kreator.makeMCComponent("QCDMu_pythia6_80to120_PU20bx25", "/QCD_Pt-80to120_MuEnrichedPt5_TuneZ2star_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
T1tttt_PU20bx25 = kreator.makeMCComponent("T1tttt_PU20bx25", "/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
TTHBB_PU20bx25 = kreator.makeMCComponent("TTHBB_PU20bx25", "/TTbarH_HToBB_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
TTHGG_PU20bx25 = kreator.makeMCComponent("TTHGG_PU20bx25", "/TTbarH_HToGG_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
TTHTauTau_PU20bx25 = kreator.makeMCComponent("TTHTauTau_PU20bx25", "/TTbarH_HToTauTau_M-125_13TeV_amcatnlo-pythia8-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
TTHWW_PU20bx25 = kreator.makeMCComponent("TTHWW_PU20bx25", "/TTbarH_HToWWTo2LAndTauNu_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
TTHZZ4L_PU20bx25 = kreator.makeMCComponent("TTHZZ4L_PU20bx25", "/TTbarH_HToZZ_4LFilter_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
TTJets_PU20bx25 = kreator.makeMCComponent("TTJets_PU20bx25", "/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v2/MINIAODSIM", "CMS", ".*root")
TTJets_PUS14 = kreator.makeMCComponent("TTJets_PUS14", "/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-PU_S14_POSTLS170_V6-v1/MINIAODSIM", "CMS", ".*root")
TTpythia8_PU20bx25 = kreator.makeMCComponent("TTpythia8_PU20bx25", "/TT_Tune4C_13TeV-pythia8-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
VBFHBB_PU20bx25 = kreator.makeMCComponent("VBFHBB_PU20bx25", "/VBF_HToBB_M-125_13TeV-powheg-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
VBFHGG_PU20bx25 = kreator.makeMCComponent("VBFHGG_PU20bx25", "/VBF_HToGG_M-125_13TeV-powheg-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
VBFHWWSemi_PU20bx25 = kreator.makeMCComponent("VBFHWWSemi_PU20bx25", "/VBF_HToWWToLAndTauNuQQ_M-125_13TeV-powheg-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
VBFHWW_PU20bx25 = kreator.makeMCComponent("VBFHWW_PU20bx25", "/VBF_HToWWTo2LAndTau2Nu_M-125_13TeV-powheg-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
VBFHZG_PU20bx25 = kreator.makeMCComponent("VBFHZG_PU20bx25", "/VBF_HToZgamma_ZToLL_M-125_13TeV-powheg-pythia8/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
VBFHZZ4L_PU20bx25 = kreator.makeMCComponent("VBFHZZ4L_PU20bx25", "/VBF_HToZZTo4L_M-125_13TeV-powheg-pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
VHMuMu_PU20bx25 = kreator.makeMCComponent("VHMuMu_PU20bx25", "/WH_ZH_HToMuMu_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
VHTauTau_PU20bx25 = kreator.makeMCComponent("VHTauTau_PU20bx25", "/WH_ZH_HToTauTau_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
VHWWInc_PU20bx25 = kreator.makeMCComponent("VHWWInc_PU20bx25", "/WH_ZH_HToWW_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
VHWWLep_PU20bx25 = kreator.makeMCComponent("VHWWLep_PU20bx25", "/WH_ZH_HToWW_2Or3WToLNuAndTau_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
VHZZ4L_PU20bx25 = kreator.makeMCComponent("VHZZ4L_PU20bx25", "/WH_ZH_HToZZ_4LFilter_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
WENupyhia8_PU20bx25 = kreator.makeMCComponent("WENupyhia8_PU20bx25", "/WToENu_Tune4C_13TeV-pythia8/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
WJets_PU20bx25 = kreator.makeMCComponent("WJets_PU20bx25", "/W1234JetsToLNu_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
WminTau_PU20bx25 = kreator.makeMCComponent("WminTau_PU20bx25", "/WminusToTauNu_CT10_13TeV-powheg-pythia8-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
WplusMu_PU20bx25 = kreator.makeMCComponent("WplusMu_PU20bx25", "/WplusToMuNu_CT10_13TeV-powheg-pythia8/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
WplusTau_PU20bx25 = kreator.makeMCComponent("WplusTau_PU20bx25", "/WplusToTauNu_CT10_13TeV-powheg-pythia8-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
ZHBBInv_PU20bx25 = kreator.makeMCComponent("ZHBBInv_PU20bx25", "/ZH_HToBB_ZToNuNu_M-125_13TeV_powheg-herwigpp/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
ZHBBLL_PU20bx25 = kreator.makeMCComponent("ZHBBLL_PU20bx25", "/ZH_HToBB_ZToLL_M-125_13TeV_powheg-herwigpp/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")
ZHLLInv_PU20bx25 = kreator.makeMCComponent("ZHLLInv_PU20bx25", "/ZH_ZToLL_HToInv_M-125_13TeV_pythia6/Spring14miniaod-PU20bx25_POSTLS170_V5-v1/MINIAODSIM", "CMS", ".*root")

mcSamplesCSA14_PU20bx25 = [ DYJetsM50_PU20bx25, DYJetsM50pythia6_PU20bx25, DYJetsM50_HT200to400_PU20bx25, DYJetsM50_HT400to600_PU20bx25, DYJetsM50_HT600toInf_PU20bx25, DYJetsMuMuM50_PtZ180_PU20bx25, DYJetsMuMuM6pythia8_PU20bx25, DYJetsMuMuM15pythia8_PU20bx25, DYJetsMuMuM50pythia8_PU20bx25, DYJetsEEpythia8_PU20bx25, DYJetsMuMupythia8_PU20bx25, EWKWmin_PU20bx25, EWKWplus_PU20bx25, EWKZjj_PU20bx25, EleGun_PU20bx25, GGHTauTau_PU20bx25, GGHZZ4L_PU20bx25, GJet_PU20bx25, JPsiPt20_PU20bx25, JPsiPt7_PU20bx25, MinBias_PU20bx25, MuMinGunPt100_PU20bx25, MuMinGunPt10_PU20bx25, MuPlusGunPt100_PU20bx25, MuPlusGunPt10_PU20bx25, NeutrinoGun_PU20bx25, QCDEM_20to30_PU20bx25, QCDEM_30to80_PU20bx25, QCDEM_80to170_PU20bx25, QCDMu_20to30_PU20bx25, QCDMu_30to50_PU20bx25, QCDMu_50to80_PU20bx25, QCDMu_80to120_PU20bx25, QCDMu_pythia6_120to170_PU20bx25, QCDMu_pythia6_20to30_PU20bx25, QCDMu_pythia6_30to50_PU20bx25, QCDMu_pythia6_50to80_PU20bx25, QCDMu_pythia6_80to120_PU20bx25, T1tttt_PU20bx25, TTHBB_PU20bx25, TTHGG_PU20bx25, TTHTauTau_PU20bx25, TTHWW_PU20bx25, TTHZZ4L_PU20bx25, TTJets_PU20bx25, TTJets_PUS14, TTpythia8_PU20bx25, VBFHBB_PU20bx25, VBFHGG_PU20bx25, VBFHWWSemi_PU20bx25, VBFHWW_PU20bx25, VBFHZG_PU20bx25, VBFHZZ4L_PU20bx25, VHMuMu_PU20bx25, VHTauTau_PU20bx25, VHWWInc_PU20bx25, VHWWLep_PU20bx25, VHZZ4L_PU20bx25, WENupyhia8_PU20bx25, WJets_PU20bx25, WminTau_PU20bx25, WplusMu_PU20bx25, WplusTau_PU20bx25, ZHBBInv_PU20bx25, ZHBBLL_PU20bx25, ZHLLInv_PU20bx25 ]


##  SOME  PRIVATELY PRODUCED TEST SAMPLES 
DYJetsM50_Flat20to50 = kreator.makePrivateMCComponent("DYJetsM50_Flat20to50","/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Spring14dr-Flat20to50_POSTLS170_V5-v1/AODSIM", [ "/store/cmst3/user/gpetrucc/miniAOD/v1/DYJetsToLL_M-50_13TeV-madgraph-pythia8_Flat20to50_PAT_big.root" ])
GluGluToHToZZTo4L_PUS14  = kreator.makePrivateMCComponent("GluGluToHToZZTo4L_PUS14",  "/GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6/Spring14dr-PU_S14_POSTLS170_V6-v1/AODSIM", ["/store/cmst3/user/gpetrucc/miniAOD/v1/GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6_PU_S14_PAT_big.root" ])
GluGluToHToGG_Flat20to50 = kreator.makePrivateMCComponent("GluGluToHToGG_Flat20to50", "/GluGluToHToGG_M-125_13TeV-powheg-pythia6/Spring14dr-Flat20to50_POSTLS170_V5-v1/AODSIM", [ "/store/cmst3/user/gpetrucc/miniAOD/v1/GluGluToHToGG_M-125_13TeV-powheg-pythia6_Flat20to50_PAT_big.root" ])
VBFHTauTau_PUS14    = kreator.makePrivateMCComponent("VBFHTauTau_PUS14",    "/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/Spring14dr-PU_S14_POSTLS170_V6-v1/AODSIM", ["/store/cmst3/user/gpetrucc/miniAOD/v1/VBF_HToTauTau_M-125_13TeV-powheg-pythia6_PU_S14_PAT.root" ])
TTHToWW_PUS14 = kreator.makePrivateMCComponent("TTHToWW_PUS14", "/TTbarH_HToWWTo2LAndTauNu_M-125_13TeV_pythia6/Spring14dr-PU_S14_POSTLS170_V6-v1/AODSIM",   [ "/store/cmst3/user/gpetrucc/miniAOD/v1/TTbarH_HToWWTo2LAndTauNu_M-125_13TeV_pythia6_PU_S14_PAT.root" ])


mcSamplesTest = [ DYJetsM50_Flat20to50, GluGluToHToZZTo4L_PUS14, GluGluToHToGG_Flat20to50, VBFHTauTau_PUS14, TTHToWW_PUS14 ]


## SOME PRIVATELY PRODUCED 50ns SAMPLES
DYJetsM50_HT100to200_PU_S14_POSTLS170 = kreator.makeMyPrivateMCComponent("DYJetsM50_HT100to200_PU_S14_POSTLS170", "/DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola/sisagir-Spring14dr-PU_S14_POSTLS170_V6AN1-miniAOD706p1-814812ec83fce2f620905d2bb30e9100/USER", "phys03")
DYJetsM50_HT200to400_PU_S14_POSTLS170 = kreator.makeMyPrivateMCComponent("DYJetsM50_HT200to400_PU_S14_POSTLS170", "/DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola/duanders-Spring14dr-PU_S14_POSTLS170_V6AN1-miniAOD706p1-814812ec83fce2f620905d2bb30e9100/USER", "phys03")
DYJetsM50_HT400to600_PU_S14_POSTLS170 = kreator.makeMyPrivateMCComponent("DYJetsM50_HT400to600_PU_S14_POSTLS170", "/DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola/phys_susy-miniAODcopy_DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola_lhx-Spring14dr-PU_S14_POSTLS170-af38aa319b7b7c91a6797b31c3be19b7/USER", "phys03")
DYJetsM50_HT600toInf_PU_S14_POSTLS170 = kreator.makeMyPrivateMCComponent("DYJetsM50_HT600toInf_PU_S14_POSTLS170", "/DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola/lhx-Spring14dr-PU_S14_POSTLS170_V6AN1-miniAOD706p1-814812ec83fce2f620905d2bb30e9100/USER", "phys03")

GJets_HT-100to200_PU_S14_POSTLS170 = kreator.makeMyPrivateMCComponent("GJets_HT-100to200_PU_S14_POSTLS170", "/GJets_HT-100to200_Tune4C_13TeV-madgraph-tauola/StoreResults-Spring14dr_PU_S14_POSTLS170_V6AN1_miniAOD706p1_814812ec83fce2f620905d2bb30e9100-v1/USER", "phys03")
GJets_HT-200to400_PU_S14_POSTLS170 = kreator.makeMyPrivateMCComponent("GJets_HT-200to400_PU_S14_POSTLS170", "/GJets_HT-200to400_Tune4C_13TeV-madgraph-tauola/duanders-Spring14dr-PU_S14_POSTLS170_V6AN1-miniAOD706p1-814812ec83fce2f620905d2bb30e9100/USER", "phys03")
GJets_HT-400to600_PU_S14_POSTLS170 = kreator.makeMyPrivateMCComponent("GJets_HT-400to600_PU_S14_POSTLS170", "/GJets_HT-400to600_Tune4C_13TeV-madgraph-tauola/duanders-Spring14dr-PU_S14_POSTLS170_V6AN1-miniAOD706p1-814812ec83fce2f620905d2bb30e9100/USER", "phys03")

# = kreator.makeMyPrivateMCComponent("", "", "phys03")


mcSamplesCSA14_PU40bx50 = [ DYJetsM50_HT400to600_PU_S14_POSTLS170 ]


mcSamples = mcSamplesCSA14_PU20bx25 + mcSamplesTest + mcSamplesCSA14_PU40bx50


#-----------DATA---------------

dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/TTHAnalysis/data"
#lumi: 12.21+7.27+0.134 = 19.62 /fb @ 8TeV

json=dataDir+'/json/Cert_Run2012ABCD_22Jan2013ReReco.json'

SingleMu = cfg.DataComponent(
    name = 'SingleMu',
    files = [ 'root://eoscms//eos/cms/store/cmst3/user/gpetrucc/miniAOD/v1/SingleMu-Run2012D-15Apr2014-v1_PAT_big.root' ],
    intLumi = 1,
    triggers = [],
    json = json
    )

DoubleElectron = cfg.DataComponent(
    name = 'DoubleElectron',
    files = [ 'root://eoscms//eos/cms/store/cmst3/user/gpetrucc/miniAOD/v1/DoubleElectron-Run2012D-15Apr2014-v1_PAT_big.root' ],
    intLumi = 1,
    triggers = [],
    json = json
    )
           
dataSamplesMu=[]
dataSamplesE=[DoubleElectron]
dataSamplesMuE=[]
dataSamples1Mu=[SingleMu,]
dataSamplesAll = dataSamplesMu+dataSamplesE+dataSamplesMuE+dataSamples1Mu


from CMGTools.TTHAnalysis.setup.Efficiencies import *


#Define splitting
for comp in mcSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 1 #  if comp.name in [ "WJets", "DY3JetsM50", "DY4JetsM50","W1Jets","W2Jets","W3Jets","W4Jets","TTJetsHad" ] else 100
    comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    comp.efficiency = eff2012

for comp in dataSamplesAll:
    comp.splitFactor = 1
    comp.isMC = False
    comp.isData = True
