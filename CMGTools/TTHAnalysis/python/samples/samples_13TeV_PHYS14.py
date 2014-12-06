from CMGTools.TTHAnalysis.samples.getFiles import getFiles
from CMGTools.TTHAnalysis.samples.getMyFiles import getMyFiles
import CMGTools.RootTools.fwlite.Config as cfg
import os




################## Triggers (FIXME: update to the PHYS14 Trigger Menu)


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

##################  PU40 bx25ns ################## 
GGHZZ4L_PU40bx25 = kreator.makeMCComponent("GGHZZ4L_PU40bx25", "/GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6/Phys14DR-PU40bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root")

DYJetsMuMuM50_PtZ180_PU40bx25 = kreator.makeMCComponent("DYJetsMuMuM50_PtZ180_PU40bx25", "/DYJetsToMuMu_PtZ-180_M-50_13TeV-madgraph/Phys14DR-PU40bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root")

TT_PU40bx25 = kreator.makeMCComponent("TT", "/TT_Tune4C_13TeV-pythia8-tauola/Phys14DR-PU40bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",809.1)
TTH_PU40bx25 = kreator.makeMCComponent("TTH", "/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/Phys14DR-PU40bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.5085)

mcSamplesPHYS14_PU40bx25 = [TT_PU40bx25,TTH_PU40bx25,DYJetsMuMuM50_PtZ180_PU40bx25,GGHZZ4L_PU40bx25]

################## PU20 bx25ns ##############

GGHZZ4L_PU20bx25 = kreator.makeMCComponent("GGHZZ4L_PU20bx25", "/GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root")


#### Background samples
QCD_HT250To500 = kreator.makeMCComponent("QCD_HT250To500", "/QCD_HT_250To500_13TeV-madgraph/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root")
QCD_HT500To1000 = kreator.makeMCComponent("QCD_HT500To1000", "/QCD_HT-500To1000_13TeV-madgraph/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root")
QCDHT = [
QCD_HT250To500,
QCD_HT500To1000,
]
WJetsToLNu_HT200to400 = kreator.makeMCComponent("WJetsToLNu_HT200to400", "/WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",1817.0*1.23)
WJetsToLNu_HT400to600 = kreator.makeMCComponent("WJetsToLNu_HT400to600", "/WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",55.61*1.23)
WJetsToLNuHT = [
WJetsToLNu_HT200to400,
WJetsToLNu_HT400to600,
]
DYJetsToLL_M50 = kreator.makeMCComponent("DYJetsToLL_M50", "/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root")

DYJetsToLL_M50_HT100to200 = kreator.makeMCComponent("DYJetsToLL_M50_HT100to200", "/DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",194.3*1.27)
DYJetsToLL_M50_HT200to400 = kreator.makeMCComponent("DYJetsToLL_M50_HT200to400", "/DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",52.24*1.27)
DYJetsToLL_M50_HT400to600 = kreator.makeMCComponent("DYJetsToLL_M50_HT400to600", "/DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",6.546*1.27)
DYJetsToLL_M50_HT600toInf = kreator.makeMCComponent("DYJetsToLL_M50_HT600toInf", "/DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",2.179*1.27)
DYJetsM50HT = [
DYJetsToLL_M50_HT100to200,
DYJetsToLL_M50_HT200to400,
DYJetsToLL_M50_HT400to600,
DYJetsToLL_M50_HT600toInf,
]
GJets_HT100to200 = kreator.makeMCComponent("GJets_HT100to200", "/GJets_HT-100to200_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",1534)
GJets_HT200to400 = kreator.makeMCComponent("GJets_HT200to400", "/GJets_HT-200to400_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",489.9)
GJets_HT600toInf = kreator.makeMCComponent("GJets_HT600toInf", "/GJets_HT-600toInf_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",20.87)
GJetsHT = [
GJets_HT100to200,
GJets_HT200to400,
GJets_HT600toInf,
]
ZJetsToNuNu_HT100to200 = kreator.makeMCComponent("ZJetsToNuNu_HT100to200", "/ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",372.6*1.27)
ZJetsToNuNu_HT200to400 = kreator.makeMCComponent("ZJetsToNuNu_HT200to400", "/ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",100.8*1.27)
ZJetsToNuNu_HT600toInf = kreator.makeMCComponent("ZJetsToNuNu_HT600toInf", "/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",4.113*1.27)
ZJetsToNuNuHT = [
ZJetsToNuNu_HT100to200,
ZJetsToNuNu_HT200to400,
ZJetsToNuNu_HT600toInf,
]
TToLeptons_tch = kreator.makeMCComponent("TToLeptons_tch", "/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root") 
TToLeptons_sch = kreator.makeMCComponent("TToLeptons_sch", "/TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root")
TBarToLeptons_tch = kreator.makeMCComponent("TBarToLeptons_tch", "/TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root")
TBarToLeptons_sch = kreator.makeMCComponent("TBarToLeptons_sch", "/TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root")
TBar_tWch = kreator.makeMCComponent("TBar_tWch", "/Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root")
T_tWch = kreator.makeMCComponent("T_tWch", "/T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root")

SingleTop = [
    TToLeptons_tch, TToLeptons_sch, TBarToLeptons_tch, TBarToLeptons_sch, TBar_tWch, T_tWch
]

TTJets = kreator.makeMCComponent("TTJets", "/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",809.1)
TTWJets = kreator.makeMCComponent("TTWJets", "/TTWJets_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.6647)
TTZJets = kreator.makeMCComponent("TTZJets", "/TTZJets_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.8565)


WZJetsTo3LNu = kreator.makeMCComponent("WZJetsTo3LNu", "/WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",2.29)
ZZTo4L = kreator.makeMCComponent("ZZTo4L","/ZZTo4L_Tune4C_13TeV-powheg-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root")

#### Signal samples
SMS_T2tt_2J_mStop850_mLSP100 = kreator.makeMCComponent("SMS_T2tt_2J_mStop850_mLSP100", "/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.0189612)
SMS_T2tt_2J_mStop650_mLSP325 = kreator.makeMCComponent("SMS_T2tt_2J_mStop650_mLSP325", "/SMS-T2tt_2J_mStop-650_mLSP-325_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.107045)
SMS_T2tt_2J_mStop500_mLSP325 = kreator.makeMCComponent("SMS_T2tt_2J_mStop500_mLSP325", "/SMS-T2tt_2J_mStop-500_mLSP-325_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.51848)
SMS_T2tt_2J_mStop425_mLSP325 = kreator.makeMCComponent("SMS_T2tt_2J_mStop425_mLSP325", "/SMS-T2tt_2J_mStop-425_mLSP-325_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",1.31169)
SMS_T2qq_2J_mStop600_mLSP550 = kreator.makeMCComponent("SMS_T2qq_2J_mStop600_mLSP550", "/SMS-T2qq_2J_mStop-600_mLSP-550_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",1.76645)
SMS_T2qq_2J_mStop1200_mLSP100 = kreator.makeMCComponent("SMS_T2qq_2J_mStop1200_mLSP100", "/SMS-T2qq_2J_mStop-1200_mLSP-100_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.0162846)
SMS_T2bb_2J_mStop900_mLSP100 = kreator.makeMCComponent("SMS_T2bb_2J_mStop900_mLSP100", "/SMS-T2bb_2J_mStop-900_mLSP-100_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.0128895)
SMS_T2bb_2J_mStop600_mLSP580 = kreator.makeMCComponent("SMS_T2bb_2J_mStop600_mLSP580", "/SMS-T2bb_2J_mStop-600_mLSP-580_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.174599)
SMS_T1tttt_2J_mGl1500_mLSP100 = kreator.makeMCComponent("SMS_T1tttt_2J_mGl1500_mLSP100", "/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.0141903)
SMS_T1tttt_2J_mGl1200_mLSP800 = kreator.makeMCComponent("SMS_T1tttt_2J_mGl1200_mLSP800", "/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.0856418)
SMS_T1qqqq_2J_mGl1400_mLSP100 = kreator.makeMCComponent("SMS_T1qqqq_2J_mGl1400_mLSP100", "/SMS-T1qqqq_2J_mGl-1400_mLSP-100_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.0252977)
SMS_T1qqqq_2J_mGl1000_mLSP800 = kreator.makeMCComponent("SMS_T1qqqq_2J_mGl1000_mLSP800", "/SMS-T1qqqq_2J_mGl-1000_mLSP-800_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.325388)
SMS_T1bbbb_2J_mGl1500_mLSP100 = kreator.makeMCComponent("SMS_T1bbbb_2J_mGl1500_mLSP100", "/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.0141903)
SMS_T1bbbb_2J_mGl1000_mLSP900 = kreator.makeMCComponent("SMS_T1bbbb_2J_mGl1000_mLSP900", "/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.325388)
SusySignalSamples = [
SMS_T2tt_2J_mStop850_mLSP100, 
SMS_T2tt_2J_mStop650_mLSP325, 
SMS_T2tt_2J_mStop500_mLSP325, 
SMS_T2tt_2J_mStop425_mLSP325, 
SMS_T2qq_2J_mStop600_mLSP550, 
SMS_T2qq_2J_mStop1200_mLSP100, 
SMS_T2bb_2J_mStop900_mLSP100, 
SMS_T2bb_2J_mStop600_mLSP580, 
SMS_T1tttt_2J_mGl1500_mLSP100, 
SMS_T1tttt_2J_mGl1200_mLSP800, 
SMS_T1qqqq_2J_mGl1400_mLSP100, 
SMS_T1qqqq_2J_mGl1000_mLSP800, 
SMS_T1bbbb_2J_mGl1500_mLSP100, 
SMS_T1bbbb_2J_mGl1000_mLSP900,
]

mcSamplesPHYS14_PU20bx25 = QCDHT + WJetsToLNuHT +  [DYJetsToLL_M50, DYJetsMuMuM50_PtZ180_PU40bx25] + DYJetsM50HT + GJetsHT + ZJetsToNuNuHT + SingleTop + [ TTJets, TTWJets, TTZJets, WZJetsTo3LNu, ZZTo4L] + SusySignalSamples

mcSamples = mcSamplesPHYS14_PU20bx25 + mcSamplesPHYS14_PU40bx25

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
