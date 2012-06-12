from CMGTools.HToZZTo4Leptons.samples.getFiles import getFiles
from CMGTools.HToZZTo4Leptons.samples.mc import *
import os

pat='V5_4_0'
filepattern = 'cmgTuple.*root'


# Triggers
triggers_mumu = ["HLT_DoubleMu7_v*","HLT_Mu13_Mu8_v*","HLT_Mu17_Mu8_v*"]
triggers_ee   = ["HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
                 "HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*"
                 ]
triggers_mue   = ["HLT_DoubleMu_7_v*","HLT_Mu13_Mu8_v*","HLT_Mu17_Mu8_v*","HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*","HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*"]

triggersMC_mumu = ["HLT_DoubleMu7_v*","HLT_Mu13_Mu8_v*","HLT_Mu17_Mu8_v*"]
triggersMC_ee   = ["HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*",
                 "HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*"
                 ]
triggersMC_mue   = ["HLT_DoubleMu_7_v*","HLT_Mu13_Mu8_v*","HLT_Mu17_Mu8_v*","HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*","HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*"]


#-----------MC---------------

ZZ2mu2tau.files=getFiles('/Summer11/zz2mu2tau_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat,'cmgtools',filepattern)
ZZ4e.files=getFiles('/Summer11/zz4e_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat,'cmgtools',filepattern)
ZZ4mu.files=getFiles('/Summer11/zz4mu_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat,'cmgtools',filepattern)
ZZ2e2mu.files=getFiles('/Summer11/zz2e2m_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat,'cmgtools',filepattern)
ZZ2e2tau.files=getFiles('/Summer11/zz2e2tau_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat,'cmgtools',filepattern)
ZZ4tau.files=getFiles('/Summer11/zz4tau_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat,'cmgtools',filepattern)
ZZGG4L.files=getFiles('/GluGluToZZTo4L_7TeV-gg2zz-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
ZZGG2L2L.files=getFiles('/GluGluToZZTo2L2L_7TeV-gg2zz-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)



GGH120.files=getFiles('/GluGluToHToZZTo4L_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH130.files=getFiles('/GluGluToHToZZTo4L_M-130_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH140.files=getFiles('/GluGluToHToZZTo4L_M-140_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH150.files=getFiles('/GluGluToHToZZTo4L_M-150_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH160.files=getFiles('/GluGluToHToZZTo4L_M-160_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH170.files=getFiles('/GluGluToHToZZTo4L_M-170_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH180.files=getFiles('/GluGluToHToZZTo4L_M-180_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH190.files=getFiles('/GluGluToHToZZTo4L_M-190_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH200.files=getFiles('/GluGluToHToZZTo4L_M-200_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH210.files=getFiles('/GluGluToHToZZTo4L_M-210_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH220.files=getFiles('/GluGluToHToZZTo4L_M-220_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH250.files=getFiles('/GluGluToHToZZTo4L_M-250_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH275.files=getFiles('/GluGluToHToZZTo4L_M-275_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH300.files=getFiles('/GluGluToHToZZTo4L_M-300_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH325.files=getFiles('/GluGluToHToZZTo4L_M-325_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH350.files=getFiles('/GluGluToHToZZTo4L_M-350_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH375.files=getFiles('/GluGluToHToZZTo4L_M-375_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH400.files=getFiles('/GluGluToHToZZTo4L_M-400_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH425.files=getFiles('/GluGluToHToZZTo4L_M-425_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH450.files=getFiles('/GluGluToHToZZTo4L_M-450_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH475.files=getFiles('/GluGluToHToZZTo4L_M-475_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH525.files=getFiles('/GluGluToHToZZTo4L_M-525_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH550.files=getFiles('/GluGluToHToZZTo4L_M-550_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH575.files=getFiles('/GluGluToHToZZTo4L_M-575_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)
GGH600.files=getFiles('/GluGluToHToZZTo4L_M-600_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_'+pat,'cmgtools',filepattern)

uflSamples=[
    ZZ2mu2tau,
    ZZ4e,
    ZZ4mu,
    ZZ2e2mu,
    ZZ2e2tau,
    ZZ4tau,
    ]

mcSamples=[ZZ2mu2tau,
           ZZ4e,
           ZZ4mu,
           ZZ2e2mu,
           ZZ2e2tau,
           ZZ4tau,
           ZZGG4L,
           ZZGG2L2L,
           GGH120,
           GGH130,
           GGH140,
           GGH150,
           GGH160,
           GGH180,
           GGH190,
           GGH200,
           GGH220,
           GGH250,
           GGH275,
           GGH300,
           GGH325,
           GGH350,
           GGH375,
           GGH400,
           GGH425,
           GGH450,
           GGH475,
           GGH525,
           GGH550,
           GGH575,
           GGH600
           ]


#-----------DATA---------------
json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON_v2.txt'
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
    files =getFiles('/DoubleElectron/Run2011B-16Jan2012-v1/AOD/V5/PAT_CMG_'+pat, 'cmgtools', filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

data_DoubleMuB = cfg.DataComponent(
    name = 'data_DoubleMuB',
    files = getFiles('/DoubleMu/Run2011B-16Jan2012-v1/AOD/V5/PAT_CMG_'+pat, 'cmgtools', filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

          
dataSamplesMu=[data_DoubleMuA,
             data_DoubleMuB]

dataSamplesE=[data_DoubleElectronA,
              data_DoubleElectronB]



dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/HToZZTo4Leptons/data"

#Define splitting
for comp in mcSamples:
    comp.isMC = True
    comp.splitFactor = 10
    comp.puFileMC=dataDir+"/puProfile_Fall11.root"
    comp.puFileData=dataDir+"/puProfile_Data11.root"


#Fix the UFL PU crap
for comp in uflSamples:
    comp.isMC = True
    comp.splitFactor = 10
    comp.puFileMC=dataDir+"/puProfile_Summer11.root"
    comp.puFileData=dataDir+"/puProfile_Data11.root"


#run some samples for 8TeV
ZZ2e2mu8TeV = copy(ZZ2e2mu)
ZZ2e2mu8TeV.puFileMC=dataDir+"/puProfile_Summer11.root"
ZZ2e2mu8TeV.puFileData=dataDir+"/puProfile_Data12.root"

ZZ4tau8TeV = copy(ZZ4tau)
ZZ4tau8TeV.puFileMC=dataDir+"/puProfile_Summer11.root"
ZZ4tau8TeV.puFileData=dataDir+"/puProfile_Data12.root"

ZZGG4L8TeV = copy(ZZGG4L)
ZZGG4L8TeV.puFileMC=dataDir+"/puProfile_Fall11.root"
ZZGG4L8TeV.puFileData=dataDir+"/puProfile_Data12.root"

ZZGG2L2L8TeV = copy(ZZGG2L2L)
ZZGG2L2L8TeV.puFileMC=dataDir+"/puProfile_Fall11.root"
ZZGG2L2L8TeV.puFileData=dataDir+"/puProfile_Data12.root"

mcSamples.extend([ZZ2e2mu8TeV,ZZ4tau8TeV,ZZGG4L8TeV,ZZGG2L2L8TeV])
                  

    
for comp in dataSamplesMu:
    comp.splitFactor = 100
    
for comp in dataSamplesE:
    comp.splitFactor = 100





    


#  LocalWords:  ZZGG
