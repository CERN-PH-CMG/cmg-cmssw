from CMGTools.HToZZTo4Leptons.samples.getFiles import getFiles
from CMGTools.HToZZTo4Leptons.samples.mc import *
import os
from copy import copy

pat='V5_4_0/calib'
filepattern = 'cmgTuple.*root'
userName='bachtis'



# Triggers


triggers_mumu = ["HLT_Mu17_Mu8_v*","HLT_Mu17_TkMu8_v*"]
triggers_ee   = ["HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*"]


triggers_mue   = [
    "HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
    "HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*"
    ]



triggersMC_mumu = ["HLT_Mu17_Mu8_v*","HLT_Mu17_TkMu8_v*"]
triggersMC_ee   = ["HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*"]


triggersMC_mue   = [
    "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
    "HLT_Mu17_Mu8_v*",
    "HLT_Mu17_TkMu8_v*",
    "HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
    "HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*"
    ]


#-----------MC---------------

ZZ2mu2tau.files=getFiles('/ZZTo2mu2tau_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
ZZ4e.files=getFiles('/ZZTo4e_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
ZZ4mu.files=getFiles('/ZZTo4mu_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
ZZ2e2mu.files=getFiles('/ZZTo2e2mu_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
ZZ2e2tau.files=getFiles('/ZZTo2e2tau_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
TTJets.files=getFiles('/TTJets_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S7_START50_V15-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)





GGH115.files=getFiles('/GluGluToHToZZTo4L_M-115_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH117.files=getFiles('/GluGluToHToZZTo4L_M-117_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH119.files=getFiles('/GluGluToHToZZTo4L_M-119_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH120.files=getFiles('/GluGluToHToZZTo4L_M-120_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH121.files=getFiles('/GluGluToHToZZTo4L_M-121_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH123.files=getFiles('/GluGluToHToZZTo4L_M-123_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH124.files=getFiles('/GluGluToHToZZTo4L_M-124_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH125.files=getFiles('/GluGluToHToZZTo4L_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH126.files=getFiles('/GluGluToHToZZTo4L_M-126_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH127.files=getFiles('/GluGluToHToZZTo4L_M-127_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)


GGH145.files=getFiles('/GluGluToHToZZTo4L_M-145_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH150.files=getFiles('/GluGluToHToZZTo4L_M-150_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH180.files=getFiles('/GluGluToHToZZTo4L_M-180_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH200.files=getFiles('/GluGluToHToZZTo4L_M-200_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH220.files=getFiles('/GluGluToHToZZTo4L_M-220_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH250.files=getFiles('/GluGluToHToZZTo4L_M-250_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH300.files=getFiles('/GluGluToHToZZTo4L_M-300_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)
GGH325.files=getFiles('/GluGluToHToZZTo4L_M-325_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)

GGH350.files=getFiles('/GluGluToHToZZTo4L_M-350_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)



GGH400.files=getFiles('/GluGluToHToZZTo4L_M-400_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)

GGH450.files=getFiles('/GluGluToHToZZTo4L_M-450_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)

GGH500.files=getFiles('/GluGluToHToZZTo4L_M-500_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)

GGH550.files=getFiles('/GluGluToHToZZTo4L_M-550_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)

GGH600.files=getFiles('/GluGluToHToZZTo4L_M-600_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)

GGH650.files=getFiles('/GluGluToHToZZTo4L_M-650_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)

GGH700.files=getFiles('/GluGluToHToZZTo4L_M-700_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)

GGH750.files=getFiles('/GluGluToHToZZTo4L_M-750_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)

GGH800.files=getFiles('/GluGluToHToZZTo4L_M-800_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)

GGH850.files=getFiles('/GluGluToHToZZTo4L_M-850_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)


GGH900.files=getFiles('/GluGluToHToZZTo4L_M-900_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)

GGH950.files=getFiles('/GluGluToHToZZTo4L_M-950_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)

GGH1000.files=getFiles('/GluGluToHToZZTo4L_M-1000_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,userName,filepattern)

mcSamples=[ZZ2mu2tau,
           ZZ4e,
           ZZ4mu,
           ZZ2e2mu,
           ZZ2e2tau,
           TTJets,
#           ZZ4tau,
#           ZZGG4L,
#           ZZGG2L2L,
           GGH115,
           GGH117,
           GGH119,
           GGH121,
           GGH123,
           GGH124,
           GGH125,
           GGH126,
           GGH127,
           GGH145,
           GGH150,
           GGH180,
           GGH200,
           GGH220,
           GGH250,
           GGH300,
           GGH325,
           GGH350,
           GGH400,
           GGH450,
           GGH500,
           GGH550,
           GGH600,
           GGH650,
           GGH700,
           GGH750,
           GGH800,
           GGH850,
           GGH900,
           GGH950,
           GGH1000
           ]




#-----------DATA---------------
json ='/afs/cern.ch/user/m/mangano/public/ichep2012_json/Jun22_190456-196509_noLowPU.json'


doubleMuFiles=getFiles('/DoubleMu/Run2012A-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_190605-194076/calib', userName, filepattern)+ \
               getFiles('/DoubleMu/Run2012B-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_start-194479/calib', userName, filepattern)+ \
               getFiles('/DoubleMu/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_194480-195016/calib', userName, filepattern)+ \
               getFiles('/DoubleMu/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195017-195396/calib', userName, filepattern)+ \
               getFiles('/DoubleMu/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195397-195775/calib', userName, filepattern)+ \
               getFiles('/DoubleMu/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195776-195947/calib', userName, filepattern)+ \
               getFiles('/DoubleMu/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195948-196509/calib', userName, filepattern)



doubleEleFiles=getFiles('/DoubleElectron/Run2012A-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_190605-194076/calib', userName, filepattern)+ \
               getFiles('/DoubleElectron/Run2012B-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_start-194479/calib', userName, filepattern)+ \
               getFiles('/DoubleElectron/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_194480-195016/calib', userName, filepattern)+ \
               getFiles('/DoubleElectron/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195017-195396/calib', userName, filepattern)+ \
               getFiles('/DoubleElectron/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195397-195775/calib', userName, filepattern)+ \
               getFiles('/DoubleElectron/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195776-195947/calib', userName, filepattern)+ \
               getFiles('/DoubleElectron/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195948-196509/calib', userName, filepattern)

               

muEGFiles=getFiles('/MuEG/Run2012A-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_190605-194076/calib', userName, filepattern)+ \
           getFiles('/MuEG/Run2012B-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_start-194479/calib', userName, filepattern)+ \
           getFiles('/MuEG/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_194480-195016/calib', userName, filepattern)+ \
           getFiles('/MuEG/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195017-195396/calib', userName, filepattern)+ \
           getFiles('/MuEG/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195397-195775/calib', userName, filepattern)+ \
           getFiles('/MuEG/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195776-195947/calib', userName, filepattern)+ \
           getFiles('/MuEG/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195948-196509/calib', userName, filepattern)


data_DoubleMu = cfg.DataComponent(
    name = 'data_DoubleMu',
    files = doubleMuFiles,
    intLumi = 1,
    triggers = [],
    json = json
    )


data_DoubleElectron = cfg.DataComponent(
    name = 'data_DoubleElectron',
    files =doubleEleFiles,
    intLumi = 1,
    triggers = [],
    json = json
    )


data_DoubleElectronRecovered = cfg.DataComponent(
    name = 'data_DoubleElectronRecovered',
    files =getFiles('/DoubleElectron/Run2012A-23May2012-v2/AOD/PAT_CMG_V5_4_0/calib', userName, filepattern),
    intLumi = 1,
    triggers = [],
    json = '/afs/cern.ch/user/m/mangano/public/ichep2012_json/May23ReReco_190782-190949_noLowPU.json'
    )


data_DoubleMuRecovered = cfg.DataComponent(
    name = 'data_DoubleMuRecovered',
    files =getFiles('/DoubleMu/Run2012A-23May2012-v2/AOD/PAT_CMG_V5_4_0/calib', userName, filepattern),
    intLumi = 1,
    triggers = [],
    json = '/afs/cern.ch/user/m/mangano/public/ichep2012_json/May23ReReco_190782-190949_noLowPU.json'

    )

data_MuEGRecovered = cfg.DataComponent(
    name = 'data_MuEGRecovered',
    files =getFiles('/MuEG/Run2012A-23May2012-v1/AOD/PAT_CMG_V5_4_0/calib', userName, filepattern),
    intLumi = 1,
    triggers = [],
    json = '/afs/cern.ch/user/m/mangano/public/ichep2012_json/May23ReReco_190782-190949_noLowPU.json'
    )


data_DoubleElectronRecovered2 = cfg.DataComponent(
    name = 'data_DoubleElectronRecovered2',
    files =getFiles('/DoubleElectron/Run2012A-08Jun2012-v2/AOD/PAT_CMG_V5_4_0/calib', userName, filepattern),
    intLumi = 1,
    triggers = [],
    json = '/afs/cern.ch/user/m/mangano/public/ichep2012_json/Jun08ReReco_noLowPileUp_noGoldetJsonJun22_noMay23.json'
    )


data_DoubleMuRecovered2 = cfg.DataComponent(
    name = 'data_DoubleMuRecovered2',
    files =getFiles('/DoubleMu/Run2012A-08Jun2012-v2/AOD/PAT_CMG_V5_4_0/calib', userName, filepattern),
    intLumi = 1,
    triggers = [],
    json = '/afs/cern.ch/user/m/mangano/public/ichep2012_json/Jun08ReReco_noLowPileUp_noGoldetJsonJun22_noMay23.json'
    )

data_MuEGRecovered2 = cfg.DataComponent(
    name = 'data_MuEGRecovered2',
    files =getFiles('/MuEG/Run2012A-08Jun2012-v3/AOD/PAT_CMG_V5_4_0/calib', userName, filepattern),
    intLumi = 1,
    triggers = [],
    json = '/afs/cern.ch/user/m/mangano/public/ichep2012_json/Jun08ReReco_noLowPileUp_noGoldetJsonJun22_noMay23.json'
    )


data_MuEG = cfg.DataComponent(
    name = 'data_MuEG',
    files = muEGFiles,
    intLumi = 1,
    triggers = [],
    json = json
    )


          
dataSamplesMu=[data_DoubleMu,data_DoubleMuRecovered,data_DoubleMuRecovered2]

dataSamplesE=[data_DoubleElectron,data_DoubleElectronRecovered,data_DoubleElectronRecovered2]

dataSamplesMuE=[data_MuEG,data_MuEGRecovered,data_MuEGRecovered2]

dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/HToZZTo4Leptons/data"


from CMGTools.HToZZTo4Leptons.setup.FakeRates import *
from CMGTools.HToZZTo4Leptons.setup.Efficiencies import *

#Define splitting
for comp in mcSamples:
    comp.isMC = True
    comp.splitFactor = 50
    comp.puFileMC=dataDir+"/puProfile_Summer12.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    comp.efficiency = eff2012
    
for comp in dataSamplesMu:
    comp.splitFactor = 900
    comp.fakeRates=fakeRates2012
for comp in dataSamplesE:
    comp.splitFactor = 900
    comp.fakeRates=fakeRates2012

for comp in dataSamplesMuE:
    comp.splitFactor = 900
    comp.fakeRates=fakeRates2012

data_DoubleMuRecovered.splitFactor=50
data_DoubleElectronRecovered.splitFactor=50
data_MuEGRecovered.splitFactor=50
data_DoubleMuRecovered2.splitFactor=50
data_DoubleElectronRecovered2.splitFactor=50
data_MuEGRecovered2.splitFactor=50


             

    

