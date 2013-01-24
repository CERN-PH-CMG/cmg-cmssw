from CMGTools.HToZZTo4Leptons.samples.getFiles import getFiles
from CMGTools.HToZZTo4Leptons.samples.mc import *
import os
from copy import copy

pat='V5_9_0'
#skim='SKIM_Oct15_All'
skim='SKIM_Nov12_All'
filepattern = 'cmgTuple.*root'
userName='bachtis'



################### Triggers
triggers_ee   = ['HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*',
                 'HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*',
                 'HLT_TripleEle10_CaloIdL_TrkIdVL_v*']


triggers_mumu = ["HLT_Mu17_Mu8_v*","HLT_DoubleMu7_v*","HLT_Mu13_Mu8_v*"]

triggers_mue   = [
              'HLT_Mu17_Ele8_CaloIdL_v*',
              'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v*',
              'HLT_Mu8_Ele17_CaloIdL_v*',
              'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v*'
              ]



triggersMC_mumu = ["HLT_Mu17_Mu8_v*"]

triggersMC_ee   = [
    'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*',
    'HLT_TripleEle10_CaloIdL_TrkIdVL_v*'
    ]



triggersMC_mue   = [
    'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*',
    'HLT_TripleEle10_CaloIdL_TrkIdVL_v*',
    "HLT_Mu17_Mu8_v*",
    "HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v*",
    "HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v*"
    ]


#####COMPONENT CREATOR
from CMGTools.HToZZTo4Leptons.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

#-----------MC---------------
ggH115=kreator.makeMCComponent('ggH115','/GluGluToHToZZTo4L_M-115_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH120=kreator.makeMCComponent('ggH120','/GluGluToHToZZTo4L_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH124=kreator.makeMCComponent('ggH124','/GluGluToHToZZTo4L_M-124_mll1_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH125=kreator.makeMCComponent('ggH125','/GluGluToHToZZTo4L_M-125_mll1_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH126=kreator.makeMCComponent('ggH126','/GluGluToHToZZTo4L_M-126_mll1_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH130=kreator.makeMCComponent('ggH130','/GluGluToHToZZTo4L_M-130_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH140=kreator.makeMCComponent('ggH140','/GluGluToHToZZTo4L_M-140_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH150=kreator.makeMCComponent('ggH150','/GluGluToHToZZTo4L_M-150_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH160=kreator.makeMCComponent('ggH160','/GluGluToHToZZTo4L_M-160_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH170=kreator.makeMCComponent('ggH170','/GluGluToHToZZTo4L_M-170_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH180=kreator.makeMCComponent('ggH180','/GluGluToHToZZTo4L_M-180_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH190=kreator.makeMCComponent('ggH190','/GluGluToHToZZTo4L_M-190_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH200=kreator.makeMCComponent('ggH200','/GluGluToHToZZTo4L_M-200_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH210=kreator.makeMCComponent('ggH210','/GluGluToHToZZTo4L_M-210_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH220=kreator.makeMCComponent('ggH220','/GluGluToHToZZTo4L_M-220_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH230=kreator.makeMCComponent('ggH230','/GluGluToHToZZTo4L_M-230_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH250=kreator.makeMCComponent('ggH250','/GluGluToHToZZTo4L_M-250_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH275=kreator.makeMCComponent('ggH275','/GluGluToHToZZTo4L_M-275_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH300=kreator.makeMCComponent('ggH300','/GluGluToHToZZTo4L_M-300_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH325=kreator.makeMCComponent('ggH325','/GluGluToHToZZTo4L_M-325_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH350=kreator.makeMCComponent('ggH350','/GluGluToHToZZTo4L_M-350_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH400=kreator.makeMCComponent('ggH400','/GluGluToHToZZTo4L_M-400_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH425=kreator.makeMCComponent('ggH425','/GluGluToHToZZTo4L_M-425_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH450=kreator.makeMCComponent('ggH450','/GluGluToHToZZTo4L_M-450_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH475=kreator.makeMCComponent('ggH475','/GluGluToHToZZTo4L_M-475_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH525=kreator.makeMCComponent('ggH525','/GluGluToHToZZTo4L_M-525_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH550=kreator.makeMCComponent('ggH550','/GluGluToHToZZTo4L_M-550_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH575=kreator.makeMCComponent('ggH575','/GluGluToHToZZTo4L_M-575_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH600=kreator.makeMCComponent('ggH600','/GluGluToHToZZTo4L_M-600_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH650=kreator.makeMCComponent('ggH650','/GluGluToHToZZTo4L_M-650_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH700=kreator.makeMCComponent('ggH700','/GluGluToHToZZTo4L_M-700_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH750=kreator.makeMCComponent('ggH750','/GluGluToHToZZTo4L_M-750_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH800=kreator.makeMCComponent('ggH800','/GluGluToHToZZTo4L_M-800_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH900=kreator.makeMCComponent('ggH900','/GluGluToHToZZTo4L_M-900_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH950=kreator.makeMCComponent('ggH950','/GluGluToHToZZTo4L_M-950_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ggH1000=kreator.makeMCComponent('ggH1000','/GluGluToHToZZTo4L_M-1000_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

GGZZ2L2L=kreator.makeMCComponent('GGZZ2L2L','/GluGluToZZTo2L2L_7TeV-gg2zz-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGZZ4L  =kreator.makeMCComponent('GGZZ4L','/GluGluToZZTo4L_7TeV-gg2zz-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

GRAVITON  =kreator.makeMCComponent('GRAVITON','/Graviton2PMToZZTo4L_M-125_7TeV-jhu-PYTHIA6_Tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
PSH  =kreator.makeMCComponent('PSH','/Higgs0MToZZTo4L_M-125_7TeV-jhu-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

ZZ2e2mu  =kreator.makeMCComponent('ZZ2e2mu','/Summer11/zz2e2m_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ2e2tau  =kreator.makeMCComponent('ZZ2e2tau','/Summer11/zz2e2tau_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ2mu2tau  =kreator.makeMCComponent('ZZ2mu2tau','/Summer11/zz2mu2tau_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

ZZ4e     =kreator.makeMCComponent('ZZ4e','/Summer11/zz4e_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ4mu    =kreator.makeMCComponent('ZZ4mu','/Summer11/zz4mu_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ4tau   =kreator.makeMCComponent('ZZ4tau','/Summer11/zz4tau_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

WW=kreator.makeMCComponent('WW','/WWJetsTo2L2Nu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
WZ=kreator.makeMCComponent('WZ','/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
TT=kreator.makeMCComponent('TT','/TTTo2L2Nu2B_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
DYLOW=kreator.makeMCComponent('DYLOW','/DYJetsToLL_M-10To50_TuneZ2_7TeV-madgraph/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
DY=kreator.makeMCComponent('DY','/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)



qqH115=kreator.makeMCComponent('qqH115','/VBF_ToHToZZTo4L_M-115_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH120=kreator.makeMCComponent('qqH120','/VBF_ToHToZZTo4L_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH125=kreator.makeMCComponent('qqH125','/VBF_ToHToZZTo4L_M-125_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH130=kreator.makeMCComponent('qqH130','/VBF_ToHToZZTo4L_M-130_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH140=kreator.makeMCComponent('qqH140','/VBF_ToHToZZTo4L_M-140_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH150=kreator.makeMCComponent('qqH150','/VBF_ToHToZZTo4L_M-150_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH160=kreator.makeMCComponent('qqH160','/VBF_ToHToZZTo4L_M-160_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH170=kreator.makeMCComponent('qqH170','/VBF_ToHToZZTo4L_M-170_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH180=kreator.makeMCComponent('qqH180','/VBF_ToHToZZTo4L_M-180_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH190=kreator.makeMCComponent('qqH190','/VBF_ToHToZZTo4L_M-190_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH200=kreator.makeMCComponent('qqH200','/VBF_ToHToZZTo4L_M-200_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH210=kreator.makeMCComponent('qqH210','/VBF_ToHToZZTo4L_M-210_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH220=kreator.makeMCComponent('qqH220','/VBF_ToHToZZTo4L_M-220_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH230=kreator.makeMCComponent('qqH230','/VBF_ToHToZZTo4L_M-230_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH250=kreator.makeMCComponent('qqH250','/VBF_ToHToZZTo4L_M-250_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH275=kreator.makeMCComponent('qqH275','/VBF_ToHToZZTo4L_M-275_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH300=kreator.makeMCComponent('qqH300','/VBF_ToHToZZTo4L_M-300_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH325=kreator.makeMCComponent('qqH325','/VBF_ToHToZZTo4L_M-325_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH350=kreator.makeMCComponent('qqH350','/VBF_ToHToZZTo4L_M-350_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH375=kreator.makeMCComponent('qqH375','/VBF_ToHToZZTo4L_M-375_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH400=kreator.makeMCComponent('qqH400','/VBF_ToHToZZTo4L_M-400_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH425=kreator.makeMCComponent('qqH425','/VBF_ToHToZZTo4L_M-425_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH450=kreator.makeMCComponent('qqH450','/VBF_ToHToZZTo4L_M-450_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH475=kreator.makeMCComponent('qqH475','/VBF_ToHToZZTo4L_M-475_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH500=kreator.makeMCComponent('qqH500','/VBF_ToHToZZTo4L_M-500_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH575=kreator.makeMCComponent('qqH575','/VBF_ToHToZZTo4L_M-575_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH600=kreator.makeMCComponent('qqH600','/VBF_ToHToZZTo4L_M-600_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH650=kreator.makeMCComponent('qqH650','/VBF_ToHToZZTo4L_M-650_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH700=kreator.makeMCComponent('qqH700','/VBF_ToHToZZTo4L_M-700_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH800=kreator.makeMCComponent('qqH800','/VBF_ToHToZZTo4L_M-800_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH900=kreator.makeMCComponent('qqH900','/VBF_ToHToZZTo4L_M-900_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH950=kreator.makeMCComponent('qqH950','/VBF_ToHToZZTo4L_M-950_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
qqH1000=kreator.makeMCComponent('qqH1000','/VBF_ToHToZZTo4L_M-1000_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)







mcSamples=[
    ggH115,ggH120,ggH124,ggH125,ggH126,ggH130,ggH140,ggH150,ggH160,ggH170,
    ggH180,ggH190,ggH200,ggH210,ggH220,ggH230,ggH250,
    ggH275,ggH300,ggH325,ggH350,ggH400,ggH425,
    ggH450,ggH475,ggH525,ggH550,ggH575,ggH600,
    ggH650,ggH700,ggH750,ggH800,ggH900,ggH1000,
    GGZZ2L2L,GGZZ4L,GRAVITON,PSH,ZZ2e2mu,ZZ2e2tau,
    ZZ2mu2tau,ZZ4e,ZZ4mu,ZZ4tau,WW,WZ,
    TT,DYLOW,DY,qqH115,qqH120,qqH125,qqH130,qqH140,
    qqH150,qqH160,qqH170,qqH180,qqH190,qqH200,qqH210,
    qqH220,qqH230,qqH250,qqH275,qqH300,qqH325,qqH350,qqH375,
    qqH400,qqH425,qqH450,qqH475,qqH500,qqH575,qqH600,qqH650,
    qqH700,qqH800,qqH900,qqH950,qqH1000]

uflSamples=[
    ZZ2e2mu,ZZ2e2tau,
    ZZ2mu2tau,ZZ4e,ZZ4mu,ZZ4tau
    ]



json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11//7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON_v2.txt'

#skim='SKIM_Oct20_GTAG'


DoubleElectron = cfg.DataComponent(
    name = 'DoubleElectron',
    files =getFiles('/DoubleElectron/Run2011A-16Jan2012-v1/AOD/V5/PAT_CMG_'+pat+'/'+skim, userName, filepattern)+ \
           getFiles('/DoubleElectron/Run2011B-16Jan2012-v1/AOD/V5/PAT_CMG_'+pat+'/'+skim, userName, filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

DoubleMu = cfg.DataComponent(
    name = 'DoubleMu',
    files =getFiles('/DoubleMu/Run2011A-16Jan2012-v1/AOD/V5/PAT_CMG_'+pat+'/'+skim, userName, filepattern)+ \
           getFiles('/DoubleMu/Run2011B-16Jan2012-v1/AOD/V5/PAT_CMG_'+pat+'/'+skim, userName, filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )


skim='SKIM_Jan22_All'
print 'using custom skim for MuEG'

MuEG = cfg.DataComponent(
    name = 'MuEG',
    files =getFiles('/MuEG/Run2011A-13Dec2012-v1/RECO/PAT_CMG_'+pat+'/'+skim, userName, filepattern)+ \
           getFiles('/MuEG/Run2011B-13Dec2012-v1/RECO/PAT_CMG_'+pat+'/'+skim, userName, filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )


          
dataSamplesMu=[DoubleMu]
dataSamplesE=[DoubleElectron]
dataSamplesMuE=[MuEG]

dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/HToZZTo4Leptons/data"

from CMGTools.HToZZTo4Leptons.setup.FakeRates import *
from CMGTools.HToZZTo4Leptons.setup.Efficiencies import *


#Define splitting
for comp in mcSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 10
    comp.puFileMC=dataDir+"/puProfile_Fall11.root"
    comp.puFileData=dataDir+"/puProfile_Data11.root"
    comp.efficiency = eff2011


#Fix the UFL PU crap
for comp in uflSamples:
    comp.isMC = True
    comp.splitFactor = 10
    comp.puFileMC=dataDir+"/puProfile_Summer11.root"
    comp.puFileData=dataDir+"/puProfile_Data11.root"
    comp.efficiency = eff2011

                                                            

    

for comp in dataSamplesMu:
    comp.splitFactor = 200
    comp.fakeRates=fakeRates2011
    comp.isMC = False
    comp.isData = True

for comp in dataSamplesE:
    comp.splitFactor = 200
    comp.fakeRates=fakeRates2011
    comp.isMC = False
    comp.isData = True

for comp in dataSamplesMuE:
    comp.splitFactor = 200
    comp.fakeRates=fakeRates2011
    comp.isMC = False
    comp.isData = True
