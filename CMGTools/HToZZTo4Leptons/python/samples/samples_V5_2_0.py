from CMGTools.HToZZTo4Leptons.samples.getFiles import getFiles
from CMGTools.HToZZTo4Leptons.samples.mc import *
from CMGTools.HToZZTo4Leptons.samples.data_2011 import *

pat = 'PAT_CMG_V5_2_0'
filePattern = 'cmgTuple.*root'


# Triggers
triggers_mumu = ["HLT_Mu13_Mu8_v*","HLT_Mu17_Mu8_v*"]
triggers_ee   = ["HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*"]
triggers_mue   = ["HLT_Mu13_Mu8_v*","HLT_Mu17_Mu8_v*","HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*"]



#Samples
DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
DYJetsLowMass.files = getFiles('/DYJetsToLL_M-10To50_TuneZ2_7TeV-madgraph/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
TTJets.files = getFiles('/TTTo2L2Nu2B_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
WZ.files = getFiles('/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
ZZ4mu.files = getFiles('/ZZTo4mu_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
ZZ4e.files = getFiles('/ZZTo4e_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
ZZ4tau.files = getFiles('/ZZTo4tau_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
ZZ2e2mu.files = getFiles('/ZZTo2e2mu_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
ZZ2e2tau.files = getFiles('/ZZTo2e2tau_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
ZZ2mu2tau.files = getFiles('/ZZTo2mu2tau_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
ZZGG4L.files = getFiles('/GluGluToZZTo4L_7TeV-gg2zz-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
ZZGG2L2L.files = getFiles('/GluGluToZZTo2L2L_7TeV-gg2zz-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
#GGH115.files = getFiles('/GluGluToHToZZTo4L_M-115_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
GGH120.files = getFiles('/GluGluToHToZZTo4L_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
GGH130.files = getFiles('/GluGluToHToZZTo4L_M-130_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
GGH150.files = getFiles('/GluGluToHToZZTo4L_M-150_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
GGH200.files = getFiles('/GluGluToHToZZTo4L_M-200_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
GGH350.files = getFiles('/GluGluToHToZZTo4L_M-350_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
GGH400.files = getFiles('/GluGluToHToZZTo4L_M-400_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)



mcSamples=[DYJets,
          DYJetsLowMass,
          TTJets,
          WZ,
          ZZ4mu,
          ZZ4e,
          ZZ4tau,
          ZZ2mu2tau,
          ZZ2e2mu,
          ZZ2e2tau,
          ZZGG4L,
          ZZGG2L2L,
#          GGH115,
          GGH120,
          GGH130,
          GGH150,
          GGH200,
          GGH350,
          GGH400]


data_DoubleMuA.files = getFiles('/DoubleMu/Run2011A-16Jan2012-v1/AOD/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
data_DoubleMuB.files = getFiles('/DoubleMu/Run2011B-16Jan2012-v1/AOD/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
data_DoubleElectronA.files = getFiles('/DoubleElectron/Run2011A-16Jan2012-v1/AOD/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)
data_DoubleElectronB.files = getFiles('/DoubleElectron/Run2011B-16Jan2012-v1/AOD/V5/PAT_CMG_V5_2_0', 'cmgtools', filePattern)


          
dataSamplesMu=[data_DoubleMuA,
             data_DoubleMuB]

dataSamplesE=[data_DoubleElectronA,
              data_DoubleElectronB]

             

    

