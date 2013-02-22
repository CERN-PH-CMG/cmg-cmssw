
from CMGTools.HToZZTo4Leptons.samples.getFiles import getFiles
from CMGTools.HToZZTo4Leptons.samples.mc import *
import os
from copy import copy

pat='V5_10_0'
skim='SKIM_Moriond_All'
filepattern = 'cmgTuple.*root'
userName='bachtis'



################### Triggers


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


#####COMPONENT CREATOR
from CMGTools.HToZZTo4Leptons.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

#-----------MC---------------
ggH118=kreator.makeMCComponent('ggH118','/GluGluToHToZZTo4L_M-118_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH119=kreator.makeMCComponent('ggH119','/GluGluToHToZZTo4L_M-119_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH120=kreator.makeMCComponent('ggH120','/GluGluToHToZZTo4L_M-120_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH121=kreator.makeMCComponent('ggH121','/GluGluToHToZZTo4L_M-121_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH122=kreator.makeMCComponent('ggH122','/GluGluToHToZZTo4L_M-122_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH123=kreator.makeMCComponent('ggH123','/GluGluToHToZZTo4L_M-123_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH124=kreator.makeMCComponent('ggH124','/GluGluToHToZZTo4L_M-124_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH125=kreator.makeMCComponent('ggH125','/GluGluToHToZZTo4L_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH126=kreator.makeMCComponent('ggH126','/GluGluToHToZZTo4L_M-126_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH127=kreator.makeMCComponent('ggH127','/GluGluToHToZZTo4L_M-127_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH128=kreator.makeMCComponent('ggH128','/GluGluToHToZZTo4L_M-128_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH129=kreator.makeMCComponent('ggH129','/GluGluToHToZZTo4L_M-129_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH130=kreator.makeMCComponent('ggH130','/GluGluToHToZZTo4L_M-130_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH135=kreator.makeMCComponent('ggH135','/GluGluToHToZZTo4L_M-135_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH140=kreator.makeMCComponent('ggH140','/GluGluToHToZZTo4L_M-140_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH145=kreator.makeMCComponent('ggH145','/GluGluToHToZZTo4L_M-145_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH150=kreator.makeMCComponent('ggH150','/GluGluToHToZZTo4L_M-150_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH160=kreator.makeMCComponent('ggH160','/GluGluToHToZZTo4L_M-160_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH170=kreator.makeMCComponent('ggH170','/GluGluToHToZZTo4L_M-170_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH180=kreator.makeMCComponent('ggH180','/GluGluToHToZZTo4L_M-180_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH190=kreator.makeMCComponent('ggH190','/GluGluToHToZZTo4L_M-190_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH200=kreator.makeMCComponent('ggH200','/GluGluToHToZZTo4L_M-200_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH220=kreator.makeMCComponent('ggH220','/GluGluToHToZZTo4L_M-220_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH250=kreator.makeMCComponent('ggH250','/GluGluToHToZZTo4L_M-250_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH275=kreator.makeMCComponent('ggH275','/GluGluToHToZZTo4L_M-275_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH300=kreator.makeMCComponent('ggH300','/GluGluToHToZZTo4L_M-300_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH325=kreator.makeMCComponent('ggH325','/GluGluToHToZZTo4L_M-325_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH375=kreator.makeMCComponent('ggH375','/GluGluToHToZZTo4L_M-375_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH400=kreator.makeMCComponent('ggH400','/GluGluToHToZZTo4L_M-400_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH425=kreator.makeMCComponent('ggH425','/GluGluToHToZZTo4L_M-425_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH450=kreator.makeMCComponent('ggH450','/GluGluToHToZZTo4L_M-450_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH475=kreator.makeMCComponent('ggH475','/GluGluToHToZZTo4L_M-475_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH500=kreator.makeMCComponent('ggH500','/GluGluToHToZZTo4L_M-500_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH525=kreator.makeMCComponent('ggH525','/GluGluToHToZZTo4L_M-525_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH550=kreator.makeMCComponent('ggH550','/GluGluToHToZZTo4L_M-550_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH575=kreator.makeMCComponent('ggH575','/GluGluToHToZZTo4L_M-575_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH600=kreator.makeMCComponent('ggH600','/GluGluToHToZZTo4L_M-600_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH650=kreator.makeMCComponent('ggH650','/GluGluToHToZZTo4L_M-650_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH700=kreator.makeMCComponent('ggH700','/GluGluToHToZZTo4L_M-700_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH750=kreator.makeMCComponent('ggH750','/GluGluToHToZZTo4L_M-750_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH800=kreator.makeMCComponent('ggH800','/GluGluToHToZZTo4L_M-800_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH850=kreator.makeMCComponent('ggH850','/GluGluToHToZZTo4L_M-850_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH900=kreator.makeMCComponent('ggH900','/GluGluToHToZZTo4L_M-900_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH1000=kreator.makeMCComponent('ggH1000','/GluGluToHToZZTo4L_M-1000_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

qqH118=kreator.makeMCComponent('qqH118','/VBF_HToZZTo4L_M-118_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH119=kreator.makeMCComponent('qqH119','/VBF_HToZZTo4L_M-119_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH120=kreator.makeMCComponent('qqH120','/VBF_HToZZTo4L_M-120_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH121=kreator.makeMCComponent('qqH121','/VBF_HToZZTo4L_M-121_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH122=kreator.makeMCComponent('qqH122','/VBF_HToZZTo4L_M-122_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH123=kreator.makeMCComponent('qqH123','/VBF_HToZZTo4L_M-123_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH124=kreator.makeMCComponent('qqH124','/VBF_HToZZTo4L_M-124_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH125=kreator.makeMCComponent('qqH125','/VBF_HToZZTo4L_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH126=kreator.makeMCComponent('qqH126','/VBF_HToZZTo4L_M-126_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH127=kreator.makeMCComponent('qqH127','/VBF_HToZZTo4L_M-127_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH128=kreator.makeMCComponent('qqH128','/VBF_HToZZTo4L_M-128_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH129=kreator.makeMCComponent('qqH129','/VBF_HToZZTo4L_M-129_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH130=kreator.makeMCComponent('qqH130','/VBF_HToZZTo4L_M-130_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH135=kreator.makeMCComponent('qqH135','/VBF_HToZZTo4L_M-135_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH140=kreator.makeMCComponent('qqH140','/VBF_HToZZTo4L_M-140_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH145=kreator.makeMCComponent('qqH145','/VBF_HToZZTo4L_M-145_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH150=kreator.makeMCComponent('qqH150','/VBF_HToZZTo4L_M-150_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH160=kreator.makeMCComponent('qqH160','/VBF_HToZZTo4L_M-160_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH170=kreator.makeMCComponent('qqH170','/VBF_HToZZTo4L_M-170_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH180=kreator.makeMCComponent('qqH180','/VBF_HToZZTo4L_M-180_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH190=kreator.makeMCComponent('qqH190','/VBF_HToZZTo4L_M-190_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH200=kreator.makeMCComponent('qqH200','/VBF_HToZZTo4L_M-200_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH220=kreator.makeMCComponent('qqH220','/VBF_HToZZTo4L_M-220_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH250=kreator.makeMCComponent('qqH250','/VBF_HToZZTo4L_M-250_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH275=kreator.makeMCComponent('qqH275','/VBF_HToZZTo4L_M-275_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH300=kreator.makeMCComponent('qqH300','/VBF_HToZZTo4L_M-300_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH325=kreator.makeMCComponent('qqH325','/VBF_HToZZTo4L_M-325_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH350=kreator.makeMCComponent('qqH350','/VBF_HToZZTo4L_M-350_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH375=kreator.makeMCComponent('qqH375','/VBF_HToZZTo4L_M-375_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH400=kreator.makeMCComponent('qqH400','/VBF_HToZZTo4L_M-400_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH425=kreator.makeMCComponent('qqH425','/VBF_HToZZTo4L_M-425_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH450=kreator.makeMCComponent('qqH450','/VBF_HToZZTo4L_M-450_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH475=kreator.makeMCComponent('qqH475','/VBF_HToZZTo4L_M-475_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH500=kreator.makeMCComponent('qqH500','/VBF_HToZZTo4L_M-500_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH525=kreator.makeMCComponent('qqH525','/VBF_HToZZTo4L_M-525_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH550=kreator.makeMCComponent('qqH550','/VBF_HToZZTo4L_M-550_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH575=kreator.makeMCComponent('qqH575','/VBF_HToZZTo4L_M-550_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH600=kreator.makeMCComponent('qqH600','/VBF_HToZZTo4L_M-600_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH650=kreator.makeMCComponent('qqH650','/VBF_HToZZTo4L_M-650_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH700=kreator.makeMCComponent('qqH700','/VBF_HToZZTo4L_M-700_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH750=kreator.makeMCComponent('qqH750','/VBF_HToZZTo4L_M-750_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH800=kreator.makeMCComponent('qqH800','/VBF_HToZZTo4L_M-800_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH850=kreator.makeMCComponent('qqH850','/VBF_HToZZTo4L_M-850_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH900=kreator.makeMCComponent('qqH900','/VBF_HToZZTo4L_M-900_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH950=kreator.makeMCComponent('qqH950','/VBF_HToZZTo4L_M-950_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH1000=kreator.makeMCComponent('qqH1000','/VBF_HToZZTo4L_M-950_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)


VH126=kreator.makeMCComponent('VH126','/WH_ZH_TTH_HToZZ_M-126_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH127=kreator.makeMCComponent('VH127','/WH_ZH_TTH_HToZZ_M-127_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH128=kreator.makeMCComponent('VH128','/WH_ZH_TTH_HToZZ_M-128_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH129=kreator.makeMCComponent('VH129','/WH_ZH_TTH_HToZZ_M-129_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH130=kreator.makeMCComponent('VH130','/WH_ZH_TTH_HToZZ_M-130_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH135=kreator.makeMCComponent('VH135','/WH_ZH_TTH_HToZZ_M-135_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH140=kreator.makeMCComponent('VH140','/WH_ZH_TTH_HToZZ_M-140_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH145=kreator.makeMCComponent('VH145','/WH_ZH_TTH_HToZZ_M-145_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH150=kreator.makeMCComponent('VH150','/WH_ZH_TTH_HToZZ_M-150_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH160=kreator.makeMCComponent('VH160','/WH_ZH_TTH_HToZZ_M-160_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH170=kreator.makeMCComponent('VH170','/WH_ZH_TTH_HToZZ_M-170_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH180=kreator.makeMCComponent('VH180','/WH_ZH_TTH_HToZZ_M-180_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH190=kreator.makeMCComponent('VH190','/WH_ZH_TTH_HToZZ_M-190_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH200=kreator.makeMCComponent('VH200','/WH_ZH_TTH_HToZZ_M-200_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH210=kreator.makeMCComponent('VH210','/WH_ZH_TTH_HToZZ_M-210_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH220=kreator.makeMCComponent('VH220','/WH_ZH_TTH_HToZZ_M-220_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH230=kreator.makeMCComponent('VH230','/WH_ZH_TTH_HToZZ_M-230_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH250=kreator.makeMCComponent('VH250','/WH_ZH_TTH_HToZZ_M-250_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH275=kreator.makeMCComponent('VH275','/WH_ZH_TTH_HToZZ_M-275_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VH300=kreator.makeMCComponent('VH300','/WH_ZH_TTH_HToZZ_M-300_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)


#-----------Background----------#XS

ZZ2e2mu=kreator.makeMCComponent('ZZ2e2mu','/ZZTo2e2mu_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ2e2tau=kreator.makeMCComponent('ZZ2e2tau','/ZZTo2e2tau_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ2mu2tau=kreator.makeMCComponent('ZZ2mu2tau','/ZZTo2mu2tau_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ4mu=kreator.makeMCComponent('ZZ4mu','/ZZTo4mu_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ4tau=kreator.makeMCComponent('ZZ4tau','/ZZTo4tau_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ4e=kreator.makeMCComponent('ZZ4e','/ZZTo4e_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

GRAVITON=kreator.makeMCComponent('GRAVITON','/Graviton2PMToZZTo4L_M-125_TuneZ2star_8TeV-jhu/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)


GGZZ4L=kreator.makeMCComponent('GGZZ4L','/GluGluToZZTo4L_8TeV-gg2zz-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGZZ2L2L=kreator.makeMCComponent('GGZZ2L2L','/GluGluToZZTo2L2L_TuneZ2star_8TeV-gg2zz-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)


DYLOW=kreator.makeMCComponent('DYLOW','/DYJetsToLL_M-10To50filter_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

DY=kreator.makeMCComponent('DY','/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
TT=kreator.makeMCComponent('TT','/TTTo2L2Nu2B_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)







mcSamples=[
qqH118,qqH119,qqH120,qqH121,qqH122,qqH123,qqH124,qqH125,
qqH126,qqH127,qqH128,qqH129,qqH130,qqH135,qqH140,qqH145,
qqH150,qqH160,qqH170,qqH180,qqH190,qqH200,qqH220,qqH250,
qqH275,qqH300,qqH325,qqH350,qqH375,qqH400,qqH425,qqH450,
qqH475,qqH500,qqH525,qqH550,qqH575,qqH600,qqH650,qqH700,
qqH750,qqH800,qqH850,qqH900,qqH950,qqH1000,ggH118,ggH119,
ggH120,ggH121,ggH122,ggH123,ggH124,ggH125,ggH126,ggH127,
ggH128,ggH129,ggH130,ggH135,ggH140,ggH145,ggH150,ggH160,
ggH170,ggH180,ggH190,ggH200,ggH220,ggH250,ggH275,ggH300,
ggH325,ggH375,ggH400,ggH425,ggH450,ggH475,ggH500,ggH525,
ggH550,ggH575,ggH600,ggH650,ggH700,ggH750,ggH800,ggH850,
ggH900,ggH1000,VH126,VH127,VH128,VH129,VH130,VH135,VH140,
VH145,VH150,VH160,VH170,VH180,VH190,VH200,VH210,VH220,VH230,
VH250,VH275,VH300,
ZZ2e2mu,ZZ2mu2tau,ZZ2e2tau,
ZZ4mu,ZZ4tau,ZZ4e,GGZZ4L,
GGZZ2L2L,DY,DYLOW,TT
]

#-----------DATA---------------

#json='/afs/cern.ch/user/m/mangano/public/hcp2012_json_v2/hcp.json'

#json='/afs/cern.ch/user/b/botta/public/moriond.json'

json='/afs/cern.ch/user/a/anlevin/public/moriond_2012_remove_pixel_ecal_recovered.txt'

jsonBadSIP='/afs/cern.ch/user/a/anlevin/public/moriond_2012_remove_r12c_ecal_recovered.txt'
jsonRecover='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_201191-201191_8TeV_11Dec2012ReReco-recover_Collisions12_JSON.txt'


DoubleMuAB = cfg.DataComponent(
    name = 'DoubleMuAB',
    files = getFiles('/DoubleMu/Run2012A-13Jul2012-v1/AOD/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)+ \
            getFiles('/DoubleMu/Run2012A-recover-06Aug2012-v1/AOD/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)+
            getFiles('/DoubleMu/Run2012B-13Jul2012-v4/AOD/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

DoubleMuC = cfg.DataComponent(
    name = 'DoubleMuC',
    files = getFiles('/DoubleMu/Run2012C-24Aug2012-v1/AOD/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)+ \
            getFiles('/DoubleMu/Run2012C-PromptReco-v2/AOD/PAT_CMG_'+pat+'_runrange_start-203002/'+skim,userName,filepattern),

    intLumi = 1,
    triggers = [],
    json = json
    )


DoubleElectronAB = cfg.DataComponent(
    name = 'DoubleElectronAB',
    files = getFiles('/DoubleElectron/Run2012A-13Jul2012-v1/AOD/V5/PAT_CMG_'+pat+'/'+skim,userName,filepattern)+ \
            getFiles('/DoubleElectron/Run2012A-recover-06Aug2012-v1/AOD/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)+
            getFiles('/DoubleElectron/Run2012B-13Jul2012-v1/AOD/V5/PAT_CMG_'+pat+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

DoubleElectronC = cfg.DataComponent(
    name = 'DoubleElectronC',
    files = getFiles('/DoubleElectron/Run2012C-24Aug2012-v1/AOD/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)+ \
            getFiles('/DoubleElectron/Run2012C-PromptReco-v2/AOD/PAT_CMG_'+pat+'_runrange_start-203002/'+skim,userName,filepattern),


    intLumi = 1,
    triggers = [],
    json = json
    )

MuEGAB = cfg.DataComponent(
    name = 'MuEGAB',
    files = getFiles('/MuEG/Run2012A-13Jul2012-v1/AOD/V5/PAT_CMG_'+pat+'/'+skim,userName,filepattern)+ \
            getFiles('/MuEG/Run2012A-recover-06Aug2012-v1/AOD/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)+
            getFiles('/MuEG/Run2012B-13Jul2012-v1/AOD/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

MuEGC = cfg.DataComponent(
    name = 'MuEGC',
    files = getFiles('/MuEG/Run2012C-24Aug2012-v1/AOD/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)+ \
            getFiles('/MuEG/Run2012C-PromptReco-v2/AOD/PAT_CMG_'+pat+'_runrange_start-203002/'+skim,userName,filepattern),


    intLumi = 1,
    triggers = [],
    json = json
    )


#####################################################################################################################
#####################################################################################################################


DoubleMuD = cfg.DataComponent(
    name = 'DoubleMuD',
    files = getFiles('/DoubleMu/Run2012D-PromptReco-v1/AOD/PAT_CMG_'+pat+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )


DoubleElectronD = cfg.DataComponent(
    name = 'DoubleElectronD',
    files = getFiles('/DoubleElectron/Run2012D-PromptReco-v1/AOD/PAT_CMG_'+pat+'/'+skim,userName,filepattern),

    intLumi = 1,
    triggers = [],
    json = json
    )

MuEGD = cfg.DataComponent(
    name = 'MuEGD',
    files = getFiles('/MuEG/Run2012D-PromptReco-v1/AOD/PAT_CMG_'+pat+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )



DoubleMuRec = cfg.DataComponent(
    name = 'DoubleMuRec',
    files =getFiles('/DoubleMu/Run2012C-EcalRecover_11Dec2012-v1/AOD/PAT_CMG_'+pat+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = jsonRecover
    )


DoubleElectronRec = cfg.DataComponent(
    name = 'DoubleElectronRec',
    files = getFiles('/DoubleElectron/Run2012C-EcalRecover_11Dec2012-v1/AOD/PAT_CMG_'+pat+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = jsonRecover
    )

MuEGRec = cfg.DataComponent(
    name = 'MuEGRec',
    files = getFiles('/MuEG/Run2012C-EcalRecover_11Dec2012-v1/AOD/PAT_CMG_'+pat+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = jsonRecover
    )




#####################################################################################################################
#####################################################################################################################
DoubleMuBadSIP = cfg.DataComponent(
    name = 'DoubleMuBadSIP',
    files = getFiles('/DoubleMu/Run2012D-16Jan2013-v1/AOD/PAT_CMG_'+pat+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = jsonBadSIP
    )


DoubleElectronBadSIP = cfg.DataComponent(
    name = 'DoubleElectronBadSIP',
    files = getFiles('/DoubleElectron/Run2012D-16Jan2013-v1/AOD/PAT_CMG_'+pat+'/'+skim,userName,filepattern),

    intLumi = 1,
    triggers = [],
    json = jsonBadSIP
    )

MuEGBadSIP = cfg.DataComponent(
    name = 'MuEGBadSIP',
    files = getFiles('/MuEG/Run2012D-16Jan2013-v1/AOD/PAT_CMG_'+pat+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = jsonBadSIP
    )




####################################################################################################################

          
dataSamplesMu=[DoubleMuAB,DoubleMuC,DoubleMuD,DoubleMuRec,DoubleMuBadSIP]

dataSamplesE=[DoubleElectronAB,DoubleElectronC,DoubleElectronD,DoubleElectronRec,DoubleElectronBadSIP]

dataSamplesMuE=[MuEGAB,MuEGC,MuEGD,MuEGRec,MuEGBadSIP]

dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/HToZZTo4Leptons/data"




from CMGTools.HToZZTo4Leptons.setup.FakeRates import *
from CMGTools.HToZZTo4Leptons.setup.Efficiencies import *

#Define splitting
for comp in mcSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 50
    comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    comp.efficiency = eff2012


for comp in dataSamplesMu:
    comp.splitFactor = 100
    comp.fakeRates=fakeRates2012
    comp.isMC = False
    comp.isData = True

for comp in dataSamplesE:
    comp.splitFactor = 100
    comp.fakeRates=fakeRates2012
    comp.isMC = False
    comp.isData = True
    
for comp in dataSamplesMuE:
    comp.splitFactor = 100
    comp.fakeRates=fakeRates2012


                                                            

    

