from CMGTools.HToZZTo4Leptons.samples.getFiles import getFiles
from CMGTools.HToZZTo4Leptons.samples.mc import *
import os
from copy import copy

pat='V5_4_0'
filepattern = 'cmgTuple.*root'


# Triggers
triggers_mumu = ["HLT_DoubleMu7_v*","HLT_Mu13_Mu8_v*","HLT_Mu17_Mu8_v*"]
triggers_ee   = ["HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
                 "HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*"]

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

ZZ2mu2tau.files=getFiles('/ZZTo2mu2tau_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
ZZ4e.files=getFiles('/ZZTo4e_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
ZZ4mu.files=getFiles('/ZZTo4mu_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
ZZ2e2mu.files=getFiles('/Summer11/zz2e2m_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat,'cmgtools',filepattern)
ZZ2e2tau.files=getFiles('/ZZTo4mu_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
ZZ4tau.files=getFiles('/Summer11/zz4tau_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat,'cmgtools',filepattern)






GGH115.files=getFiles('/GluGluToHToZZTo4L_M-115_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH117.files=getFiles('/GluGluToHToZZTo4L_M-117_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH119.files=getFiles('/GluGluToHToZZTo4L_M-119_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH120.files=getFiles('/GluGluToHToZZTo4L_M-120_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH121.files=getFiles('/GluGluToHToZZTo4L_M-121_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH123.files=getFiles('/GluGluToHToZZTo4L_M-123_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH124.files=getFiles('/GluGluToHToZZTo4L_M-124_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH125.files=getFiles('/GluGluToHToZZTo4L_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH126.files=getFiles('/GluGluToHToZZTo4L_M-126_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH127.files=getFiles('/GluGluToHToZZTo4L_M-127_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)


GGH145.files=getFiles('/GluGluToHToZZTo4L_M-145_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH150.files=getFiles('/GluGluToHToZZTo4L_M-150_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH180.files=getFiles('/GluGluToHToZZTo4L_M-180_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH200.files=getFiles('/GluGluToHToZZTo4L_M-200_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH220.files=getFiles('/GluGluToHToZZTo4L_M-220_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH250.files=getFiles('/GluGluToHToZZTo4L_M-250_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH300.files=getFiles('/GluGluToHToZZTo4L_M-300_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH325.files=getFiles('/GluGluToHToZZTo4L_M-325_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)

GGH350.files=getFiles('/GluGluToHToZZTo4L_M-350_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)



GGH400.files=getFiles('/GluGluToHToZZTo4L_M-400_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)

GGH450.files=getFiles('/GluGluToHToZZTo4L_M-450_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)

GGH500.files=getFiles('/GluGluToHToZZTo4L_M-500_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)

GGH550.files=getFiles('/GluGluToHToZZTo4L_M-550_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)

GGH600.files=getFiles('/GluGluToHToZZTo4L_M-600_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)

GGH650.files=getFiles('/GluGluToHToZZTo4L_M-650_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)

GGH700.files=getFiles('/GluGluToHToZZTo4L_M-700_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)

GGH750.files=getFiles('/GluGluToHToZZTo4L_M-750_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)

GGH800.files=getFiles('/GluGluToHToZZTo4L_M-800_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)

GGH850.files=getFiles('/GluGluToHToZZTo4L_M-850_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)


GGH900.files=getFiles('/GluGluToHToZZTo4L_M-900_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)

GGH950.files=getFiles('/GluGluToHToZZTo4L_M-950_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)

GGH1000.files=getFiles('/GluGluToHToZZTo4L_M-1000_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)

mcSamples=[ZZ2mu2tau,
           ZZ4e,
           ZZ4mu,
#           ZZ2e2mu,
           ZZ2e2tau,
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
json ='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-195396_8TeV_PromptReco_Collisions12_JSON_v2.txt' 



data_DoubleElectronA = cfg.DataComponent(
    name = 'data_DoubleElectronA',
    files = getFiles('/DoubleElectron/Run2011A-16Jan2012-v1/AOD/V5/PAT_CMG_'+pat, 'cmgtools', filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

data_DoubleMuA = cfg.DataComponent(
    name = 'data_DoubleMuA',
    files = getFiles('/DoubleMu/Run2011A-16Jan2012-v1/AOD/V5/PAT_CMG_'+pat, 'cmgtools', filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )


data_DoubleElectronB = cfg.DataComponent(
    name = 'data_DoubleElectronB',
    files =getFiles('/DoubleElectron/Run2012B-PromptReco-v1/AOD/PAT_CMG_'+pat+'_runrange_194480-195016', 'cmgtools', filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

data_DoubleMuB = cfg.DataComponent(
    name = 'data_DoubleMuB',
    files = getFiles('/DoubleMu/Run2012B-PromptReco-v1/AOD/PAT_CMG_'+pat+'_runrange_194480-195016', 'cmgtools', filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )


data_MuEGB = cfg.DataComponent(
    name = 'data_MuEGB',
    files = getFiles('/MuEG/Run2012B-PromptReco-v1/AOD/PAT_CMG_'+pat+'_runrange_194480-195016','cmgtools',filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )




          
dataSamplesMu=[data_DoubleMuB]

dataSamplesE=[data_DoubleElectronB]

dataSamplesMuE=[data_MuEGB]

dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/HToZZTo4Leptons/data"


from CMGTools.HToZZTo4Leptons.setup.FakeRates import *

#Define splitting
for comp in mcSamples:
    comp.isMC = True
    comp.splitFactor = 10
    comp.puFileMC=dataDir+"/puProfile_Summer12.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    
for comp in dataSamplesMu:
    comp.splitFactor = 100
    comp.fakeRates=fakeRates2012
for comp in dataSamplesE:
    comp.splitFactor = 100
    comp.fakeRates=fakeRates2012

for comp in dataSamplesMuE:
    comp.splitFactor = 100
    comp.fakeRates=fakeRates2012


             

    

