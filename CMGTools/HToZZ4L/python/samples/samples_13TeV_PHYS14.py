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


triggers_multilep  = triggers_mumu_iso + triggers_ee + triggers_3e + triggers_mue


#####COMPONENT CREATOR
from CMGTools.TTHAnalysis.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()


##################  PU40 bx25ns (not default, so samples have a _PU40bx25 postfix) ################## 
GGHZZ4L_PU40bx25 = kreator.makeMCComponent("GGHZZ4L_PU40bx25", "/GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6/Phys14DR-PU40bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root", 43.92*2.76E-04)
mcSamplesPHYS14_PU40bx25 = [GGHZZ4L_PU40bx25]


################## PU20 bx25ns (default of phys14, so no postfix) ##############

# DY inclusive (cross section from FEWZ, StandardModelCrossSectionsat13TeV)
DYJetsToLL_M50 = kreator.makeMCComponent("DYJetsToLL_M50", "/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root", 2008.*3)
# DY HT bins: cross sections for DYJets taken from McM LO times inclusive k-factor from FEWZ(2008 pb x3)/McM(4746)
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

# TTbar cross section: MCFM with dynamic scale, StandardModelCrossSectionsat13TeV
TTJets = kreator.makeMCComponent("TTJets", "/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",809.1)
# TTV cross sections are from 13 TeV MG5_aMC@NLO v2.2.1, NNPDF 2.3nlo, fixed scale = mtop + 0.5*mv
TTWJets = kreator.makeMCComponent("TTWJets", "/TTWJets_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.6647)
TTZJets = kreator.makeMCComponent("TTZJets", "/TTZJets_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",0.8565)
# TTH cross section from LHC Higgs XS WG: https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt1314TeV?rev=15
TTH = kreator.makeMCComponent("TTH", "/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2/MINIAODSIM", "CMS", ".*root",0.5085)

# cross section from StandardModelCrossSectionsat13TeV (NLO MCFM, mll > 12) times BR=(3*0.108)*(3*0.0337)
WZJetsTo3LNu = kreator.makeMCComponent("WZJetsTo3LNu", "/WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",2.165)
# cross section from StandardModelCrossSectionsat13TeV (NLO MCFM, mll > 12) times BR=(3*0.0337)**2
ZZTo4L = kreator.makeMCComponent("ZZTo4L","/ZZTo4L_Tune4C_13TeV-powheg-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root", 	31.8*(3*0.03366**2))

# GGH cross section from LHC Higgs XS WG: https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt1314TeV?rev=15
GGHZZ4L = kreator.makeMCComponent("GGHZZ4L", "/GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root", 43.92*2.76E-04)


mcSamplesPHYS14_PU20bx25 =  [DYJetsToLL_M50] + DYJetsM50HT  + [ TTJets, TTWJets, TTZJets, TTH, WZJetsTo3LNu, ZZTo4L, GGHZZ4L]


mcSamples = mcSamplesPHYS14_PU20bx25 + mcSamplesPHYS14_PU40bx25 

#-----------DATA---------------
dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/TTHAnalysis/data"
#lumi: 12.21+7.27+0.134 = 19.62 /fb @ 8TeV
json=dataDir+'/json/Cert_Run2012ABCD_22Jan2013ReReco.json'

#SingleMu = cfg.DataComponent(
#    name = 'SingleMu',
#    files = kreator.getFilesFromEOS('SingleMu', 
#                                    '/SingleMu/Run2012D-15Apr2014-v1/AOD/02e0a1be-c9c7-11e3-bfe2-0024e83ef644/MINIAOD/CMSSW_7_0_9_patch2_GR_70_V2_AN1',
#                                    '/eos/cms/store/cmst3/user/cmgtools/CMG/%s'),
#    intLumi = 1,
#    triggers = [],
#    json = json
#    )

           
dataSamplesMu=[]
dataSamplesE=[]
dataSamplesMuE=[]
dataSamplesAll = dataSamplesMu+dataSamplesE+dataSamplesMuE

from CMGTools.TTHAnalysis.setup.Efficiencies import *


#Define splitting
for comp in mcSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 250 #  if comp.name in [ "WJets", "DY3JetsM50", "DY4JetsM50","W1Jets","W2Jets","W3Jets","W4Jets","TTJetsHad" ] else 100
    comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    comp.efficiency = eff2012

#for comp in dataSamplesAll:
#    comp.splitFactor = 1000
#    comp.isMC = False
#    comp.isData = True


