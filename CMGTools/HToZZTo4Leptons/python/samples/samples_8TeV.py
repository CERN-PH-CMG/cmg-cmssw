from CMGTools.HToZZTo4Leptons.samples.getFiles import getFiles
from CMGTools.HToZZTo4Leptons.samples.mc import *
import os
from copy import copy

pat='V5_10_0'
skim='SKIM_Oct13_All'
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
GGH118=kreator.makeMCComponent('GGH118','/GluGluToHToZZTo4L_M-118_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH119=kreator.makeMCComponent('GGH119','/GluGluToHToZZTo4L_M-119_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH120=kreator.makeMCComponent('GGH120','/GluGluToHToZZTo4L_M-120_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH121=kreator.makeMCComponent('GGH121','/GluGluToHToZZTo4L_M-121_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH122=kreator.makeMCComponent('GGH122','/GluGluToHToZZTo4L_M-122_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH123=kreator.makeMCComponent('GGH123','/GluGluToHToZZTo4L_M-123_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH124=kreator.makeMCComponent('GGH124','/GluGluToHToZZTo4L_M-124_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH125=kreator.makeMCComponent('GGH125','/GluGluToHToZZTo4L_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH126=kreator.makeMCComponent('GGH126','/GluGluToHToZZTo4L_M-126_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH127=kreator.makeMCComponent('GGH127','/GluGluToHToZZTo4L_M-127_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH128=kreator.makeMCComponent('GGH128','/GluGluToHToZZTo4L_M-128_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH129=kreator.makeMCComponent('GGH129','/GluGluToHToZZTo4L_M-129_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH130=kreator.makeMCComponent('GGH130','/GluGluToHToZZTo4L_M-130_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH135=kreator.makeMCComponent('GGH135','/GluGluToHToZZTo4L_M-135_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH140=kreator.makeMCComponent('GGH140','/GluGluToHToZZTo4L_M-140_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH145=kreator.makeMCComponent('GGH145','/GluGluToHToZZTo4L_M-145_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH150=kreator.makeMCComponent('GGH150','/GluGluToHToZZTo4L_M-150_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH160=kreator.makeMCComponent('GGH160','/GluGluToHToZZTo4L_M-160_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH170=kreator.makeMCComponent('GGH170','/GluGluToHToZZTo4L_M-170_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH180=kreator.makeMCComponent('GGH180','/GluGluToHToZZTo4L_M-180_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH190=kreator.makeMCComponent('GGH190','/GluGluToHToZZTo4L_M-190_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH200=kreator.makeMCComponent('GGH200','/GluGluToHToZZTo4L_M-200_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH220=kreator.makeMCComponent('GGH220','/GluGluToHToZZTo4L_M-220_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH250=kreator.makeMCComponent('GGH250','/GluGluToHToZZTo4L_M-250_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH275=kreator.makeMCComponent('GGH275','/GluGluToHToZZTo4L_M-275_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH300=kreator.makeMCComponent('GGH300','/GluGluToHToZZTo4L_M-300_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH325=kreator.makeMCComponent('GGH325','/GluGluToHToZZTo4L_M-325_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH375=kreator.makeMCComponent('GGH375','/GluGluToHToZZTo4L_M-375_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH400=kreator.makeMCComponent('GGH400','/GluGluToHToZZTo4L_M-400_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH425=kreator.makeMCComponent('GGH425','/GluGluToHToZZTo4L_M-425_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH450=kreator.makeMCComponent('GGH450','/GluGluToHToZZTo4L_M-450_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH475=kreator.makeMCComponent('GGH475','/GluGluToHToZZTo4L_M-475_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH500=kreator.makeMCComponent('GGH500','/GluGluToHToZZTo4L_M-500_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH525=kreator.makeMCComponent('GGH525','/GluGluToHToZZTo4L_M-525_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH550=kreator.makeMCComponent('GGH550','/GluGluToHToZZTo4L_M-550_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH575=kreator.makeMCComponent('GGH575','/GluGluToHToZZTo4L_M-575_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH600=kreator.makeMCComponent('GGH600','/GluGluToHToZZTo4L_M-600_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH650=kreator.makeMCComponent('GGH650','/GluGluToHToZZTo4L_M-650_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH700=kreator.makeMCComponent('GGH700','/GluGluToHToZZTo4L_M-700_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH750=kreator.makeMCComponent('GGH750','/GluGluToHToZZTo4L_M-750_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH800=kreator.makeMCComponent('GGH800','/GluGluToHToZZTo4L_M-800_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH850=kreator.makeMCComponent('GGH850','/GluGluToHToZZTo4L_M-850_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH900=kreator.makeMCComponent('GGH900','/GluGluToHToZZTo4L_M-900_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH1000=kreator.makeMCComponent('GGH1000','/GluGluToHToZZTo4L_M-1000_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

VBF118=kreator.makeMCComponent('VBF118','/VBF_HToZZTo4L_M-118_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF119=kreator.makeMCComponent('VBF119','/VBF_HToZZTo4L_M-119_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF120=kreator.makeMCComponent('VBF120','/VBF_HToZZTo4L_M-120_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF121=kreator.makeMCComponent('VBF121','/VBF_HToZZTo4L_M-121_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF122=kreator.makeMCComponent('VBF122','/VBF_HToZZTo4L_M-122_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF123=kreator.makeMCComponent('VBF123','/VBF_HToZZTo4L_M-123_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF124=kreator.makeMCComponent('VBF124','/VBF_HToZZTo4L_M-124_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF125=kreator.makeMCComponent('VBF125','/VBF_HToZZTo4L_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF126=kreator.makeMCComponent('VBF126','/VBF_HToZZTo4L_M-126_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF127=kreator.makeMCComponent('VBF127','/VBF_HToZZTo4L_M-127_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF128=kreator.makeMCComponent('VBF128','/VBF_HToZZTo4L_M-128_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF129=kreator.makeMCComponent('VBF129','/VBF_HToZZTo4L_M-129_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF130=kreator.makeMCComponent('VBF130','/VBF_HToZZTo4L_M-130_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF135=kreator.makeMCComponent('VBF135','/VBF_HToZZTo4L_M-135_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF140=kreator.makeMCComponent('VBF140','/VBF_HToZZTo4L_M-140_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF145=kreator.makeMCComponent('VBF145','/VBF_HToZZTo4L_M-145_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF150=kreator.makeMCComponent('VBF150','/VBF_HToZZTo4L_M-150_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF160=kreator.makeMCComponent('VBF160','/VBF_HToZZTo4L_M-160_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF170=kreator.makeMCComponent('VBF170','/VBF_HToZZTo4L_M-170_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF180=kreator.makeMCComponent('VBF180','/VBF_HToZZTo4L_M-180_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF190=kreator.makeMCComponent('VBF190','/VBF_HToZZTo4L_M-190_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF200=kreator.makeMCComponent('VBF200','/VBF_HToZZTo4L_M-200_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF220=kreator.makeMCComponent('VBF220','/VBF_HToZZTo4L_M-220_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF250=kreator.makeMCComponent('VBF250','/VBF_HToZZTo4L_M-250_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF275=kreator.makeMCComponent('VBF275','/VBF_HToZZTo4L_M-275_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF300=kreator.makeMCComponent('VBF300','/VBF_HToZZTo4L_M-300_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF325=kreator.makeMCComponent('VBF325','/VBF_HToZZTo4L_M-325_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF350=kreator.makeMCComponent('VBF350','/VBF_HToZZTo4L_M-350_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF375=kreator.makeMCComponent('VBF375','/VBF_HToZZTo4L_M-375_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF400=kreator.makeMCComponent('VBF400','/VBF_HToZZTo4L_M-400_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF425=kreator.makeMCComponent('VBF425','/VBF_HToZZTo4L_M-425_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF450=kreator.makeMCComponent('VBF450','/VBF_HToZZTo4L_M-450_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF475=kreator.makeMCComponent('VBF475','/VBF_HToZZTo4L_M-475_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF500=kreator.makeMCComponent('VBF500','/VBF_HToZZTo4L_M-500_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF525=kreator.makeMCComponent('VBF525','/VBF_HToZZTo4L_M-525_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF550=kreator.makeMCComponent('VBF550','/VBF_HToZZTo4L_M-550_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF575=kreator.makeMCComponent('VBF575','/VBF_HToZZTo4L_M-550_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF600=kreator.makeMCComponent('VBF600','/VBF_HToZZTo4L_M-600_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF650=kreator.makeMCComponent('VBF650','/VBF_HToZZTo4L_M-650_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF700=kreator.makeMCComponent('VBF700','/VBF_HToZZTo4L_M-700_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF750=kreator.makeMCComponent('VBF750','/VBF_HToZZTo4L_M-750_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF800=kreator.makeMCComponent('VBF800','/VBF_HToZZTo4L_M-800_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF850=kreator.makeMCComponent('VBF850','/VBF_HToZZTo4L_M-850_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF900=kreator.makeMCComponent('VBF900','/VBF_HToZZTo4L_M-900_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF950=kreator.makeMCComponent('VBF950','/VBF_HToZZTo4L_M-950_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF1000=kreator.makeMCComponent('VBF1000','/VBF_HToZZTo4L_M-950_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)



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
VBF118,VBF119,VBF120,VBF121,VBF122,VBF123,VBF124,VBF125,
VBF126,VBF127,VBF128,VBF129,VBF130,VBF135,VBF140,VBF145,
VBF150,VBF160,VBF170,VBF180,VBF190,VBF200,VBF220,VBF250,
VBF275,VBF300,VBF325,VBF350,VBF375,VBF400,VBF425,VBF450,
VBF475,VBF500,VBF525,VBF550,VBF575,VBF600,VBF650,VBF700,
VBF750,VBF800,VBF850,VBF900,VBF950,VBF1000,GGH118,GGH119,
GGH120,GGH121,GGH122,GGH123,GGH124,GGH125,GGH126,GGH127,
GGH128,GGH129,GGH130,GGH135,GGH140,GGH145,GGH150,GGH160,
GGH170,GGH180,GGH190,GGH200,GGH220,GGH250,GGH275,GGH300,
GGH325,GGH375,GGH400,GGH425,GGH450,GGH475,GGH500,GGH525,
GGH550,GGH575,GGH600,GGH650,GGH700,GGH750,GGH800,GGH850,
GGH900,GGH1000,
ZZ2e2mu,ZZ2mu2tau,ZZ2e2tau,
ZZ4mu,ZZ4tau,ZZ4e,GGZZ4L,
GGZZ2L2L,DY,DYLOW,TT
]

#-----------DATA---------------
json ='/afs/cern.ch/user/m/mangano/public/hcp2012_json/hcp.reRecoJul13PlusReRecoAug06PlusPromptReco.json'  

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





          
dataSamplesMu=[DoubleMuAB,DoubleMuC]

dataSamplesE=[DoubleElectronAB,DoubleElectronC]

dataSamplesMuE=[MuEGAB,MuEGC]

dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/HToZZTo4Leptons/data"




from CMGTools.HToZZTo4Leptons.setup.FakeRates import *
from CMGTools.HToZZTo4Leptons.setup.Efficiencies import *

#Define splitting
for comp in mcSamples:
    comp.isMC = True
    comp.splitFactor = 50
    comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    comp.efficiency = eff2012


for comp in dataSamplesMu:
    comp.splitFactor = 500
    comp.fakeRates=fakeRates2012
for comp in dataSamplesE:
    comp.splitFactor = 500
    comp.fakeRates=fakeRates2012
    
for comp in dataSamplesMuE:
    comp.splitFactor = 500
    comp.fakeRates=fakeRates2012

                                                            

    

