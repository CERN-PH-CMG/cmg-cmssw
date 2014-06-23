from CMGTools.TTHAnalysis.samples.getFiles import getFiles
import CMGTools.RootTools.fwlite.Config as cfg
import os

pat='PAT_CMG_V5_17_0'
patOld='PAT_CMG_V5_16_0'
pat15='PAT_CMG_V5_15_0'
patPF='CMGPF_V5_16_0'
skim=''
filepattern = 'cmgTuple.*root'
userName='cmgtools'



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
DYJetsM50_Flat20to50 = kreator.makePrivateMCComponent("DYJetsM50_Flat20to50","/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Spring14dr-Flat20to50_POSTLS170_V5-v1/AODSIM", [ "/store/cmst3/user/gpetrucc/miniAOD/v1/DYJetsToLL_M-50_13TeV-madgraph-pythia8_Flat20to50_PAT_big.root" ])
DYJetsM50_PU20bx25   = kreator.makePrivateMCComponent("DYJetsM50_PU20bx25",  "/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Spring14dr-PU20bx25_POSTLS170_V5-v1/AODSIM",   [ "/store/cmst3/user/gpetrucc/miniAOD/v1/DYJetsToLL_M-50_13TeV-madgraph-pythia8_PU20bx25_PAT_big.root" ])
GluGluToHToZZTo4L_PUS14  = kreator.makePrivateMCComponent("GluGluToHToZZTo4L_PUS14",  "/GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6/Spring14dr-PU_S14_POSTLS170_V6-v1/AODSIM", ["/store/cmst3/user/gpetrucc/miniAOD/v1/GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6_PU_S14_PAT_big.root" ])
GluGluToHToZZTo4L_PU20bx25  = kreator.makePrivateMCComponent("GluGluToHToZZTo4L_PU20bx25",  "/GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6/Spring14dr-Flat20to50_POSTLS170_V5-v1/AODSIM", ["/store/cmst3/user/gpetrucc/miniAOD/v1/GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6_PU20bx25_PAT.root" ])
GluGluToHToGG_Flat20to50 = kreator.makePrivateMCComponent("GluGluToHToGG_Flat20to50", "/GluGluToHToGG_M-125_13TeV-powheg-pythia6/Spring14dr-Flat20to50_POSTLS170_V5-v1/AODSIM", [ "/store/cmst3/user/gpetrucc/miniAOD/v1/GluGluToHToGG_M-125_13TeV-powheg-pythia6_Flat20to50_PAT_big.root" ])
TTJets_PUS14 = kreator.makePrivateMCComponent("TTJets_PUS14", "/TT_Tune4C_13TeV-pythia8-tauola/Spring14dr-PU_S14_POSTLS170_V6-v1/AODSIM", ["/store/cmst3/user/gpetrucc/miniAOD/v1/TT_Tune4C_13TeV-pythia8-tauola_PU_S14_PAT_big.root"])
TTJets_PU20bx25 = kreator.makePrivateMCComponent("TTJets_PU20bx25", "/TT_Tune4C_13TeV-pythia8-tauola/Spring14dr-PU20bx25_POSTLS170_V5-v1/AODSIM",   [ "/store/cmst3/user/gpetrucc/miniAOD/v1/TT_Tune4C_13TeV-pythia8-tauola_PU20bx25_PAT_big.root" ])
VBFHTauTau_PU20bx25 = kreator.makePrivateMCComponent("VBFHTauTau_PU20bx25", "/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/Spring14dr-PU20bx25_POSTLS170_V5-v1/AODSIM",   [ "/store/cmst3/user/gpetrucc/miniAOD/v1/VBF_HToTauTau_M-125_13TeV-powheg-pythia6_PU20bx25_PAT.root" ])
VBFHTauTau_PUS14    = kreator.makePrivateMCComponent("VBFHTauTau_PUS14",    "/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/Spring14dr-PU_S14_POSTLS170_V6-v1/AODSIM", ["/store/cmst3/user/gpetrucc/miniAOD/v1/VBF_HToTauTau_M-125_13TeV-powheg-pythia6_PU_S14_PAT.root" ])
WHZHToZZ4LFilter_PU20bx25 = kreator.makePrivateMCComponent("WHZHToZZ4LFilter_PU20bx25", "/WH_ZH_HToZZ_4LFilter_M-125_13TeV_pythia6/Spring14dr-PU20bx25_POSTLS170_V5-v1/AODSIM",   [ "/store/cmst3/user/gpetrucc/miniAOD/v1/WH_ZH_HToZZ_4LFilter_M-125_13TeV_pythia6_PAT.root" ])
TTHToWW_PUS14 = kreator.makePrivateMCComponent("TTHToWW_PUS14", "/TTbarH_HToWWTo2LAndTauNu_M-125_13TeV_pythia6/Spring14dr-PU_S14_POSTLS170_V6-v1/AODSIM",   [ "/store/cmst3/user/gpetrucc/miniAOD/v1/TTbarH_HToWWTo2LAndTauNu_M-125_13TeV_pythia6_PU_S14_PAT.root" ])


mcSamples = [ DYJetsM50_Flat20to50, DYJetsM50_PU20bx25, GluGluToHToZZTo4L_PUS14, GluGluToHToZZTo4L_PU20bx25, GluGluToHToGG_Flat20to50, TTJets_PUS14, TTJets_PU20bx25, VBFHTauTau_PU20bx25, VBFHTauTau_PUS14, WHZHToZZ4LFilter_PU20bx25, TTHToWW_PUS14 ]

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
