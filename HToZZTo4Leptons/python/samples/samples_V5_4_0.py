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
GGH125.files =getFiles('/GluGluToHToZZTo4L_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START50_V15-v1/AODSIM/V5/PAT_CMG_V5_3_0', 'bachtis', filePattern)

mcSamples=[GGH125]


             

    

