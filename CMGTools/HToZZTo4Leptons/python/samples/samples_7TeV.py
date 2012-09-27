from CMGTools.HToZZTo4Leptons.samples.getFiles import getFiles
from CMGTools.HToZZTo4Leptons.samples.mc import *
import os
from copy import copy

pat='V5_6_0_B'
skim='SKIM_Sep26_NoCal_MuClean'
filepattern = 'cmgTuple.*root'
userName='bachtis'



################### Triggers
triggers_ee   = ['HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*',
                 'HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*',
                 'HLT_TripleEle10_CaloIdL_TrkIdVL_v*']


triggers_mumu = ["HLT_Mu17_Mu8_v*","HLT_DoubleMu7_v*","HLT_Mu13_Mu8_v*"]


triggersMC_mumu = ["HLT_Mu17_Mu8_v*"]

triggersMC_ee   = [
    'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*',
    'HLT_TripleEle10_CaloIdL_TrkIdVL_v*'
    ]



triggersMC_mue   = [
    'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*',
    'HLT_TripleEle10_CaloIdL_TrkIdVL_v*',
    "HLT_Mu17_Mu8_v*"
    ]


#####COMPONENT CREATOR
from CMGTools.HToZZTo4Leptons.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

#-----------MC---------------

GGH120=kreator.makeMCComponent('GGH120','/GluGluToHToZZTo4L_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH130=kreator.makeMCComponent('GGH130','/GluGluToHToZZTo4L_M-130_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH140=kreator.makeMCComponent('GGH140','/GluGluToHToZZTo4L_M-140_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH150=kreator.makeMCComponent('GGH150','/GluGluToHToZZTo4L_M-150_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH160=kreator.makeMCComponent('GGH160','/GluGluToHToZZTo4L_M-160_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH170=kreator.makeMCComponent('GGH170','/GluGluToHToZZTo4L_M-170_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH180=kreator.makeMCComponent('GGH180','/GluGluToHToZZTo4L_M-180_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH190=kreator.makeMCComponent('GGH190','/GluGluToHToZZTo4L_M-190_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH200=kreator.makeMCComponent('GGH200','/GluGluToHToZZTo4L_M-200_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH210=kreator.makeMCComponent('GGH210','/GluGluToHToZZTo4L_M-210_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH220=kreator.makeMCComponent('GGH220','/GluGluToHToZZTo4L_M-220_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH250=kreator.makeMCComponent('GGH250','/GluGluToHToZZTo4L_M-250_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH275=kreator.makeMCComponent('GGH275','/GluGluToHToZZTo4L_M-275_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH300=kreator.makeMCComponent('GGH300','/GluGluToHToZZTo4L_M-300_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH325=kreator.makeMCComponent('GGH325','/GluGluToHToZZTo4L_M-325_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH350=kreator.makeMCComponent('GGH350','/GluGluToHToZZTo4L_M-350_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH400=kreator.makeMCComponent('GGH400','/GluGluToHToZZTo4L_M-400_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH425=kreator.makeMCComponent('GGH425','/GluGluToHToZZTo4L_M-425_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH450=kreator.makeMCComponent('GGH450','/GluGluToHToZZTo4L_M-450_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH475=kreator.makeMCComponent('GGH475','/GluGluToHToZZTo4L_M-475_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH525=kreator.makeMCComponent('GGH525','/GluGluToHToZZTo4L_M-525_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH550=kreator.makeMCComponent('GGH550','/GluGluToHToZZTo4L_M-550_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH575=kreator.makeMCComponent('GGH575','/GluGluToHToZZTo4L_M-575_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH600=kreator.makeMCComponent('GGH600','/GluGluToHToZZTo4L_M-600_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH650=kreator.makeMCComponent('GGH650','/GluGluToHToZZTo4L_M-650_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH700=kreator.makeMCComponent('GGH700','/GluGluToHToZZTo4L_M-700_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH750=kreator.makeMCComponent('GGH750','/GluGluToHToZZTo4L_M-750_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH800=kreator.makeMCComponent('GGH800','/GluGluToHToZZTo4L_M-800_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH900=kreator.makeMCComponent('GGH900','/GluGluToHToZZTo4L_M-900_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH950=kreator.makeMCComponent('GGH950','/GluGluToHToZZTo4L_M-950_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGH1000=kreator.makeMCComponent('GGH1000','/GluGluToHToZZTo4L_M-1000_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

GGZZ2L2L=kreator.makeMCComponent('GGZZ2L2L','/GluGluToZZTo2L2L_7TeV-gg2zz-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
GGZZ4L  =kreator.makeMCComponent('GGZZ4L','/GluGluToZZTo4L_7TeV-gg2zz-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

GRAV125  =kreator.makeMCComponent('GRAV125','/Graviton2PMToZZTo4L_M-125_7TeV-jhu-PYTHIA6_Tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
PSH125  =kreator.makeMCComponent('PSH125','/Higgs0MToZZTo4L_M-125_7TeV-jhu-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

ZZ2e2mu  =kreator.makeMCComponent('ZZ2e2mu','/Summer11/zz2e2m_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ2e2tau  =kreator.makeMCComponent('ZZ2e2tau','/Summer11/zz2e2tau_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ2mu2tau  =kreator.makeMCComponent('ZZ2mu2tau','/Summer11/zz2mu2tau_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

ZZ4e     =kreator.makeMCComponent('ZZ4e','/Summer11/zz4e_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ4mu    =kreator.makeMCComponent('ZZ4mu','/Summer11/zz4mu_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ4tau   =kreator.makeMCComponent('ZZ4tau','/Summer11/zz4tau_powheg_GENSIMRECO_v2/USER/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

WW=kreator.makeMCComponent('WW','/WWJetsTo2L2Nu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
WZ=kreator.makeMCComponent('WZ','/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZGEE=kreator.makeMCComponent('ZGEE','/ZGToEEG_TuneZ2_7TeV-madgraph/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZGMM=kreator.makeMCComponent('ZGMM','/ZGToMuMuG_TuneZ2_7TeV-madgraph/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
TT=kreator.makeMCComponent('ZGTT','/TTTo2L2Nu2B_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZJETSL=kreator.makeMCComponent('ZJETSL','/DYJetsToLL_M-10To50_TuneZ2_7TeV-madgraph/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZJETS=kreator.makeMCComponent('ZJETS','/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)



VBF115=kreator.makeMCComponent('VBF115','/VBF_ToHToZZTo4L_M-115_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF120=kreator.makeMCComponent('VBF120','/VBF_ToHToZZTo4L_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF125=kreator.makeMCComponent('VBF125','/VBF_ToHToZZTo4L_M-125_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF130=kreator.makeMCComponent('VBF130','/VBF_ToHToZZTo4L_M-130_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF140=kreator.makeMCComponent('VBF140','/VBF_ToHToZZTo4L_M-140_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF150=kreator.makeMCComponent('VBF150','/VBF_ToHToZZTo4L_M-150_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF160=kreator.makeMCComponent('VBF160','/VBF_ToHToZZTo4L_M-160_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF170=kreator.makeMCComponent('VBF170','/VBF_ToHToZZTo4L_M-170_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF180=kreator.makeMCComponent('VBF180','/VBF_ToHToZZTo4L_M-180_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF190=kreator.makeMCComponent('VBF190','/VBF_ToHToZZTo4L_M-190_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF200=kreator.makeMCComponent('VBF200','/VBF_ToHToZZTo4L_M-200_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF210=kreator.makeMCComponent('VBF210','/VBF_ToHToZZTo4L_M-210_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF220=kreator.makeMCComponent('VBF220','/VBF_ToHToZZTo4L_M-220_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF230=kreator.makeMCComponent('VBF230','/VBF_ToHToZZTo4L_M-230_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF250=kreator.makeMCComponent('VBF250','/VBF_ToHToZZTo4L_M-250_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF275=kreator.makeMCComponent('VBF275','/VBF_ToHToZZTo4L_M-275_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF300=kreator.makeMCComponent('VBF300','/VBF_ToHToZZTo4L_M-300_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF325=kreator.makeMCComponent('VBF325','/VBF_ToHToZZTo4L_M-325_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF350=kreator.makeMCComponent('VBF350','/VBF_ToHToZZTo4L_M-350_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF375=kreator.makeMCComponent('VBF375','/VBF_ToHToZZTo4L_M-375_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF400=kreator.makeMCComponent('VBF400','/VBF_ToHToZZTo4L_M-400_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF425=kreator.makeMCComponent('VBF425','/VBF_ToHToZZTo4L_M-425_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF450=kreator.makeMCComponent('VBF450','/VBF_ToHToZZTo4L_M-450_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF475=kreator.makeMCComponent('VBF475','/VBF_ToHToZZTo4L_M-475_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF500=kreator.makeMCComponent('VBF500','/VBF_ToHToZZTo4L_M-500_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF575=kreator.makeMCComponent('VBF575','/VBF_ToHToZZTo4L_M-575_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF600=kreator.makeMCComponent('VBF600','/VBF_ToHToZZTo4L_M-600_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF650=kreator.makeMCComponent('VBF650','/VBF_ToHToZZTo4L_M-650_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF700=kreator.makeMCComponent('VBF700','/VBF_ToHToZZTo4L_M-700_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF800=kreator.makeMCComponent('VBF800','/VBF_ToHToZZTo4L_M-800_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF900=kreator.makeMCComponent('VBF900','/VBF_ToHToZZTo4L_M-900_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF950=kreator.makeMCComponent('VBF950','/VBF_ToHToZZTo4L_M-950_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
VBF1000=kreator.makeMCComponent('VBF1000','/VBF_ToHToZZTo4L_M-1000_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)







mcSamples=[
    GGH120,GGH130,GGH140,GGH150,GGH160,GGH170,
    GGH180,GGH190,GGH200,GGH210,GGH220,GGH250,
    GGH275,GGH300,GGH325,GGH350,GGH400,GGH425,
    GGH450,GGH475,GGH525,GGH550,GGH575,GGH600,
    GGH650,GGH700,GGH750,GGH800,GGH900,GGH1000,
    GGZZ2L2L,GGZZ4L,GRAV125,PSH125,ZZ2e2mu,ZZ2e2tau,
    ZZ2mu2tau,ZZ4e,ZZ4mu,ZZ4tau,WW,WZ,ZGEE,ZGMM,
    TT,ZJETSL,ZJETS,VBF115,VBF120,VBF125,VBF130,VBF140,
    VBF150,VBF160,VBF170,VBF180,VBF190,VBF200,VBF210,
    VBF220,VBF230,VBF250,VBF275,VBF300,VBF325,VBF350,VBF375,
    VBF400,VBF425,VBF450,VBF475,VBF500,VBF575,VBF600,VBF650,
    VBF700,VBF800,VBF900,VBF950,VBF1000]

uflSamples=[
    ZZ2e2mu,ZZ2e2tau,
    ZZ2mu2tau,ZZ4e,ZZ4mu,ZZ4tau
    ]




json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11//7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON_v2.txt'



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



          
dataSamplesMu=[DoubleMu]
dataSamplesE=[DoubleElectron]

dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/HToZZTo4Leptons/data"
from CMGTools.HToZZTo4Leptons.setup.Efficiencies import *


#Define splitting
for comp in mcSamples:
    comp.isMC = True
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

                                                            

    

