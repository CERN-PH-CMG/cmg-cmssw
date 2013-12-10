from CMGTools.TTHAnalysis.samples.getFiles import getFiles
import CMGTools.RootTools.fwlite.Config as cfg
import os
from copy import copy

pat='PAT_CMG_V5_17_0'
patOld='PAT_CMG_V5_16_0'
patPF='CMGPF_V5_16_0'
skim=''
filepattern = 'cmgTuple.*root'
userName='cmgtools'



################## Triggers


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

triggers_1mu = [ 'HLT_IsoMu24_eta2p1_v*' ]
triggersMC_1mu  = triggers_1mu;
triggersFR_1mu  = [ 'HLT_Mu5_v*', 'HLT_RelIso1p0Mu5_v*', 'HLT_Mu12_v*', 'HLT_Mu24_eta2p1_v*', 'HLT_Mu40_eta2p1_v*' ]
triggersFR_mumu = [ 'HLT_Mu17_Mu8_v*', 'HLT_Mu17_TkMu8_v*', 'HLT_Mu8_v*', 'HLT_Mu17_v*' ]
triggersFR_1e   = [ 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*', 'HLT_Ele17_CaloIdL_CaloIsoVL_v*', 'HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*', 'HLT_Ele8__CaloIdL_CaloIsoVL_v*']
triggersFR_mue  = triggers_mue[:]
triggersFR_MC = triggersFR_1mu + triggersFR_mumu + triggersFR_1e + triggersFR_mue


#####COMPONENT CREATOR

from CMGTools.TTHAnalysis.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

#-----------MC---------------
## --- TTH ---
TTH      =kreator.makeMCComponent('TTH','/TTH_Inclusive_M-125_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5/'+pat+'/'+skim,userName,filepattern)
#TTH122   =kreator.makeMCComponent('TTH122','/TTH_Inclusive_M-122_5_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
#TTH127   =kreator.makeMCComponent('TTH127','/TTH_Inclusive_M-127_5_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)

## --- TTH + V ---
TTWJets  =kreator.makeMCComponent('TTWJets','/TTWJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5/'+pat+'/'+skim,userName,filepattern)
TTZJets  =kreator.makeMCComponent('TTZJets','/TTZJets_8TeV-madgraph_v2/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5/'+pat+'/'+skim,userName,filepattern)
TTWWJets =kreator.makeMCComponent('TTWWJets','/TTWWJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patPF+'/'+skim,userName,filepattern)
## --- same as above but aMC@NLO ---
TTWnlo   =kreator.makeMCComponent('TTWnlo','/TTbarW_8TeV-aMCatNLO-herwig/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/'+patPF+'/'+skim,userName,filepattern)
TTZnlo   =kreator.makeMCComponent('TTZnlo','/ttbarZ_8TeV-Madspin_aMCatNLO-herwig/Summer12_DR53X-PU_S10_START53_V19-v1/AODSIM/V5_B/'+patPF+'/'+skim,userName,filepattern)

## --- TTH + gamma ---
TTG =kreator.makeMCComponent('TTG','/TTGJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V19-v1/AODSIM/V5_B/'+patPF+'/'+skim,userName,filepattern)

## ---- DIBOSON, PYTHIA INCLUSIVE ----
WW = kreator.makeMCComponent('WW', '/WW_TuneZ2star_8TeV_pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patPF+'/'+skim,userName,filepattern)
WZ = kreator.makeMCComponent('WZ', '/WZ_TuneZ2star_8TeV_pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patPF+'/'+skim,userName,filepattern)
ZZ = kreator.makeMCComponent('ZZ', '/ZZ_TuneZ2star_8TeV_pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patPF+'/'+skim,userName,filepattern)
## ---- DIBOSON, MADGRAPH EXCLUSIVE ----
WWJets   =kreator.makeMCComponent('WWJets','/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patOld+'/'+skim,userName,filepattern)
WZJets   =kreator.makeMCComponent('WZJets','/WZJetsTo3LNu_TuneZ2_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patOld+'/'+skim,userName,filepattern)     
ZZJets4L =kreator.makeMCComponent('ZZJets4L','/ZZJetsTo4L_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patOld+'/'+skim,userName,filepattern) 
## ---- DIBOSONS, POWHEG ----
#ZZ2e2mu  =kreator.makeMCComponent('ZZ2e2mu','/ZZTo2e2mu_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/'+pat+'/'+skim,userName,filepattern)
#ZZ2e2tau =kreator.makeMCComponent('ZZ2e2tau','/ZZTo2e2tau_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/'+pat+'/'+skim,userName,filepattern)
#ZZ2mu2tau=kreator.makeMCComponent('ZZ2mu2tau','/ZZTo2mu2tau_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/'+pat+'/'+skim,userName,filepattern)
#ZZTo4mu  =kreator.makeMCComponent('ZZTo4mu','/ZZTo4mu_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/'+pat+'/'+skim,userName,filepattern)
#ZZTo4tau =kreator.makeMCComponent('ZZTo4tau','/ZZTo4tau_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/'+pat+'/'+skim,userName,filepattern)
#ZZTo4e   =kreator.makeMCComponent('ZZTo4e','/ZZTo4e_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/'+pat+'/'+skim,userName,filepattern)
#ZZJets4L =kreator.makeMCComponent('ZZJets4L','/ZZJetsTo4L_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)

## --- MULTIBOSON -----
WWWJets  =kreator.makeMCComponent('WWWJets','/WWWJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patPF+'/'+skim,userName,filepattern)
WWZJets  =kreator.makeMCComponent('WWZJets','/WWZNoGstarJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patPF+'/'+skim,userName,filepattern)
WZZJets  =kreator.makeMCComponent('WZZJets','/WZZNoGstarJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patPF+'/'+skim,userName,filepattern)
ZZZJets  =kreator.makeMCComponent('ZZZJets','/ZZZNoGstarJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patPF+'/'+skim,userName,filepattern)

## --- BOSON + PHOTON
#ZG =kreator.makeMCComponent('ZG','/ZGToLLG_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
#WGToLNuG =kreator.makeMCComponent('WGToLNuG','/WGToLNuG_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
## --- MULTIBOSON + PHOTON
WWGJets  =kreator.makeMCComponent('WWGJets','/WWGJets_8TeV-madgraph_v2/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patPF+'/'+skim,userName,filepattern)


## ---- TTBAR -----
#TTLep    =kreator.makeMCComponent('TTLep','/TTTo2L2Nu2B_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
TTJets   =kreator.makeMCComponent('TTJets','/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patPF+'/'+skim,userName,filepattern)
#TTJetsLep=kreator.makeMCComponent('TTJetsLep','/TTJets_FullLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
TTJetsLep=kreator.makeMCComponent('TTJetsLep','/TTJets_FullLeptMGDecays_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v2/AODSIM/V5_B/'+patOld+'/'+skim,userName,filepattern)
TTJetsSem=kreator.makeMCComponent('TTJetsSem','/TTJets_SemiLeptMGDecays_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/'+patOld+'/'+skim,userName,filepattern)
TTJetsHad=kreator.makeMCComponent('TTJetsHad','/TTJets_HadronicMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A_ext-v1/AODSIM/V5/'+patOld+'/'+skim,userName,filepattern)

# ---- SINGLE TOP ----
TtW       =kreator.makeMCComponent('TtW','/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
TbartW    =kreator.makeMCComponent('TbartW','/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
Ttch      =kreator.makeMCComponent('Ttch','/T_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
Tbartch   =kreator.makeMCComponent('Tbartch','/Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
Tsch      =kreator.makeMCComponent('Tsch','/T_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
Tbarsch   =kreator.makeMCComponent('Tbarsch','/Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)

# ---- Z + JETS
#DYJetsM10=kreator.makeMCComponent('DYJetsM10','/DYJetsToLL_M-10To50filter_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
DYJetsM50=kreator.makeMCComponent('DYJetsM50','/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
#DY1JetsM50=kreator.makeMCComponent('DY1JetsM50','/DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
#DY2JetsM50=kreator.makeMCComponent('DY2JetsM50','/DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
#DY3JetsM50=kreator.makeMCComponent('DY3JetsM50','/DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
#DY4JetsM50=kreator.makeMCComponent('DY4JetsM50','/DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)


# ---- W + JETS
WJets    =kreator.makeMCComponent('WJets','/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/'+patPF+'/'+skim,userName,filepattern)
W1Jets   =kreator.makeMCComponent('W1Jets','/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patOld+'/'+skim,userName,filepattern)
W2Jets   =kreator.makeMCComponent('W2Jets','/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patOld+'/'+skim,userName,filepattern)
W3Jets   =kreator.makeMCComponent('W3Jets','/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patOld+'/'+skim,userName,filepattern)
W4Jets   =kreator.makeMCComponent('W4Jets','/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+patOld+'/'+skim,userName,filepattern)
#WJets_HT250To300=kreator.makeMCComponent('WJets_HT250To300','/WJetsToLNu_HT-250To300_8TeV-madgraph_v2/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
#WJets_HT300To400=kreator.makeMCComponent('WJets_HT300To400','/WJetsToLNu_HT-300To400_8TeV-madgraph_v2/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
#WJets_HT400ToInf=kreator.makeMCComponent('WJets_HT400ToInf','/WJetsToLNu_HT-400ToInf_8TeV-madgraph_v2/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)

# ---- QCD
#QCDMuPt15=kreator.makeMCComponent('QCDMuPt15','/QCD_Pt_20_MuEnrichedPt_15_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v3/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
#QCDElPt30To80=kreator.makeMCComponent('QCDElPt30To80','//QCD_Pt_30_80_BCtoE_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)
#QCDElPt80To170=kreator.makeMCComponent('QCDElPt80To170','//QCD_Pt_80_170_BCtoE_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/'+pat+'/'+skim,userName,filepattern)

# ------ MORE SAMPLES FROM SEZEN (patPF) ----
#/Jet/Run2012A-22Jan2013-v1/AOD/CMGPF_V5_16_0
#/HT/Run2012A-22Jan2013-v1/AOD/CMGPF_V5_16_0
#/JetHT/Run2012B-22Jan2013-v1/AOD/CMGPF_V5_16_0
#/JetHT/Run2012C-22Jan2013-v1/AOD/CMGPF_V5_16_0
#/JetHT/Run2012D-22Jan2013-v1/AOD/CMGPF_V5_16_0
#/QCD_HT-100To250_TuneZ2star_8TeV-madgraph-pythia/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/CMGPF_V5_16_0
#/QCD_HT-250To500_TuneZ2star_8TeV-madgraph-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/CMGPF_V5_16_0
#/QCD_HT-500To1000_TuneZ2star_8TeV-madgraph-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/CMGPF_V5_16_0
#/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/CMGPF_V5_16_0
#/ZJetsToNuNu_50_HT_100_TuneZ2Star_8TeV_madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/CMGPF_V5_16_0
#/ZJetsToNuNu_100_HT_200_TuneZ2Star_8TeV_madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/CMGPF_V5_16_0
#/ZJetsToNuNu_200_HT_400_TuneZ2Star_8TeV_madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/CMGPF_V5_16_0
#/ZJetsToNuNu_400_HT_inf_TuneZ2Star_8TeV_madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/CMGPF_V5_16_0
# ----- MORE FRMO MARKUS -------------
# /DYJetsToLL_PtZ-50To70_TuneZ2star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_17_0
# /DYJetsToLL_PtZ-70To100_TuneZ2star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_17_0
# /DYJetsToLL_PtZ-100_TuneZ2star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_17_0
# /WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_17_0
# /WJetsToLNu_PtW-70To100_TuneZ2star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_17_0
# /WJetsToLNu_PtW-50To70_TuneZ2star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_17_0
# /DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_17_0_2nd
# /DYToTauTau_M-20_CT10_TuneZ2star_v2_8TeV-powheg-tauola-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_17_0_2nd




#   ## Critical samples (major signals and backgrounds, and a few small samples)
mcSamples_1 = [ TTH,TTWJets,TTZJets,TTWWJets,WWWJets,WWZJets,TTG,DYJetsM50,TTJetsLep,TTJetsSem,TTJets,TtW,TbartW,WZJets,ZZJets4L,WWJets ]
#   mcSamples_1 = [ TTH,TTWJets,TTZJets,TTWWJets,WWWJets,WWZJets,TTG,DYJetsM50,DY2JetsM50,DY3JetsM50,DY4JetsM50,WZJets,ZZ2e2mu,ZZ2e2tau,ZZ2mu2tau,ZZTo4mu,ZZTo4e,ZZTo4tau,TtW,TbartW,TTJetsLep,TTJetsSem ]
#   ## Minor samples and backgrounds 
mcSamples_2 = [ Tsch,Tbarsch,Ttch,Tbartch,W1Jets,W2Jets,W3Jets,W4Jets,WZZJets,ZZZJets,WWGJets,TTJetsHad ]
#   mcSamples_2 = [ TTH122,TTH127,DYJetsM10,TTLep,WWJets,TTJets,Tsch,Tbarsch,Ttch,Tbartch,W1Jets,W2Jets,W3Jets,W4Jets,TTJetsHad,DY1JetsM50, ]
#   ## Cross-check samples, ... 
mcSamples_3 = [ TTWnlo,TTZnlo,WJets ]
#   ## Samples we don't use
#   mcSamples_4 = [ QCDMuPt15,WGs2MU,WGs2E,WGs2Tau,WGToLNuG,ZG,QCDElPt30To80,QCDElPt80To170 ]
mcSamples_4 = [ WW,WZ,ZZ ]

mcSamples=mcSamples_1+mcSamples_2+mcSamples_3
extraMcSamples = mcSamples_4

#-----------DATA---------------

dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/TTHAnalysis/data"
#lumi: 12.21+7.27+0.134 = 19.62 /fb @ 8TeV

json=dataDir+'/json/Cert_Run2012ABCD_22Jan2013ReReco.json'

DoubleMuAB = cfg.DataComponent(
    name = 'DoubleMuAB',
    files = getFiles('/DoubleMu/Run2012A-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern)+ \
            getFiles('/DoubleMuParked/Run2012B-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

DoubleMuC = cfg.DataComponent(
    name = 'DoubleMuC',
    files = getFiles('/DoubleMuParked/Run2012C-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

DoubleMuD = cfg.DataComponent(
    name = 'DoubleMuD',
    files = getFiles('/DoubleMuParked/Run2012D-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

DoubleElectronAB = cfg.DataComponent(
    name = 'DoubleElectronAB',
    files = getFiles('/DoubleElectron/Run2012A-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern)+ \
            getFiles('/DoubleElectron/Run2012B-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

DoubleElectronC = cfg.DataComponent(
    name = 'DoubleElectronC',
    files = getFiles('/DoubleElectron/Run2012C-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

DoubleElectronD = cfg.DataComponent(
    name = 'DoubleElectronD',
    files = getFiles('/DoubleElectron/Run2012D-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

#   MuEGAB = cfg.DataComponent(
#       name = 'MuEGAB',
#       files = getFiles('/MuEG/Run2012A-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern)+ \
#               getFiles('/MuEG/Run2012B-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern),
#       intLumi = 1,
#       triggers = [],
#       json = json
#       )
#   
#   MuEGC = cfg.DataComponent(
#       name = 'MuEGC',
#       files = getFiles('/MuEG/Run2012C-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern),
#       intLumi = 1,
#       triggers = [],
#       json = json
#       )
#   
#   MuEGD = cfg.DataComponent(
#       name = 'MuEGD',
#       files = getFiles('/MuEG/Run2012D-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern),
#       intLumi = 1,
#       triggers = [],
#       json = json
#       )
SingleMuAB = cfg.DataComponent(
    name = 'SingleMuAB',
    files = getFiles('/SingleMu/Run2012A-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern)+ \
            getFiles('/SingleMu/Run2012B-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

SingleMuC = cfg.DataComponent(
    name = 'SingleMuC',
    files = getFiles('/SingleMu/Run2012C-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )

SingleMuD = cfg.DataComponent(
    name = 'SingleMuD',
    files = getFiles('/SingleMu/Run2012D-22Jan2013-v1/AOD/'+patPF+'/'+skim,userName,filepattern),
    intLumi = 1,
    triggers = [],
    json = json
    )


   
#   ####################################################################################################################
#   
#             
dataSamplesMu=[DoubleMuAB,DoubleMuC,DoubleMuD]
dataSamplesE=[DoubleElectronAB,DoubleElectronC,DoubleElectronD]
dataSamplesMuE=[] #MuEGAB,MuEGC,MuEGD,MuEGRec,MuEGBadSIP]
dataSamples1Mu=[SingleMuAB,SingleMuC,SingleMuD]
dataSamplesAll = dataSamplesMu+dataSamplesE+dataSamplesMuE+dataSamples1Mu
#   
#   ####################################################################################################################
#   #-----------PRIVATE FAST SIM---------------
#   
#   madFiles = [ f.strip() for f in open("%s/src/CMGTools/TTHAnalysis/python/samples/fastSim-madgraph.txt" % os.environ['CMSSW_BASE'], "r") ]
#   #ttgstarFiles = [ f.strip() for f in open("%s/src/CMGTools/TTHAnalysis/python/samples/fastSim-ttgstar.txt" % os.environ['CMSSW_BASE'], "r") ]
#   lowmllFiles = [ f.strip() for f in open("%s/src/CMGTools/TTHAnalysis/python/samples/fastSim-lowmll.txt" % os.environ['CMSSW_BASE'], "r") ]
#   pythiaFiles = [ f.strip() for f in open("%s/src/CMGTools/TTHAnalysis/python/samples/fastSim-pythia-2013-05-02.txt" % os.environ['CMSSW_BASE'], "r") ]
#   powhelFiles = [ f.strip() for f in open("%s/src/CMGTools/TTHAnalysis/python/samples/fastSim-powhel.txt" % os.environ['CMSSW_BASE'], "r") ]
#   rareFiles = [ f.strip() for f in open("%s/src/CMGTools/TTHAnalysis/python/samples/rareBg-caf.txt" % os.environ['CMSSW_BASE'], "r") ]
#   def _grep(x,l): return [ i for i in l if x in i ]
#   
#   madgraphDate = "2013-05-23"
#   FastSim_TTZJets    = kreator.makePrivateMCComponent('FastSim_TTZJets',    '/store/caf/user/gpetrucc/ttH/gen/'+madgraphDate+'/ttZ_01jets_nominal',   _grep('ttZ_01jets_nominal',   madFiles) )
#   FastSim_TTZJets_Up = kreator.makePrivateMCComponent('FastSim_TTZJets_scaleUp',    '/store/caf/user/gpetrucc/ttH/gen/'+madgraphDate+'/ttZ_01jets_scaleUp',   _grep('ttZ_01jets_scaleUp',   madFiles) )
#   FastSim_TTZJets_Dn = kreator.makePrivateMCComponent('FastSim_TTZJets_scaleDown',  '/store/caf/user/gpetrucc/ttH/gen/'+madgraphDate+'/ttZ_01jets_scaleDown', _grep('ttZ_01jets_scaleDown', madFiles) )
#   FastSim_TTZJets_MUp = kreator.makePrivateMCComponent('FastSim_TTZJets_xqtUp',    '/store/caf/user/gpetrucc/ttH/gen/'+madgraphDate+'/ttZ_01jets_xqtUp',   _grep('ttZ_01jets_xqtUp',   madFiles) )
#   FastSim_TTZJets_MDn = kreator.makePrivateMCComponent('FastSim_TTZJets_xqtDown',  '/store/caf/user/gpetrucc/ttH/gen/'+madgraphDate+'/ttZ_01jets_xqtDown', _grep('ttZ_01jets_xqtDown', madFiles) )
#   FastSim_TTWJets = kreator.makePrivateMCComponent('FastSim_TTWJets',    '/store/caf/user/gpetrucc/ttH/gen/'+madgraphDate+'/ttW_012jets_nominal',   _grep('ttW_012jets_nominal',   madFiles) )
#   FastSim_TTWJets_Up = kreator.makePrivateMCComponent('FastSim_TTWJets_scaleUp',    '/store/caf/user/gpetrucc/ttH/gen/'+madgraphDate+'/ttW_012jets_scaleUp',   _grep('ttW_012jets_scaleUp',   madFiles) )
#   FastSim_TTWJets_Dn = kreator.makePrivateMCComponent('FastSim_TTWJets_scaleDown',  '/store/caf/user/gpetrucc/ttH/gen/'+madgraphDate+'/ttW_012jets_scaleDown', _grep('ttW_012jets_scaleDown', madFiles) )
#   FastSim_TTWJets_MUp = kreator.makePrivateMCComponent('FastSim_TTWJets_xqtUp',    '/store/caf/user/gpetrucc/ttH/gen/'+madgraphDate+'/ttW_012jets_xqtUp',   _grep('ttW_012jets_xqtUp',   madFiles) )
#   FastSim_TTWJets_MDn = kreator.makePrivateMCComponent('FastSim_TTWJets_xqtDown',  '/store/caf/user/gpetrucc/ttH/gen/'+madgraphDate+'/ttW_012jets_xqtDown', _grep('ttW_012jets_xqtDown', madFiles) )
#   FastSim_TTHJets    = kreator.makePrivateMCComponent('FastSim_TTHJets',    '/store/caf/user/gpetrucc/ttH/gen/'+madgraphDate+'/ttH_012jets_nominal',   _grep('ttH_012jets_nominal',   madFiles) )
#   FastSim_TTHJets_Up = kreator.makePrivateMCComponent('FastSim_TTHJets_scaleUp',    '/store/caf/user/gpetrucc/ttH/gen/'+madgraphDate+'/ttH_012jets_scaleUp',   _grep('ttH_012jets_scaleUp',   madFiles) )
#   FastSim_TTHJets_Dn = kreator.makePrivateMCComponent('FastSim_TTHJets_scaleDown',  '/store/caf/user/gpetrucc/ttH/gen/'+madgraphDate+'/ttH_012jets_scaleDown', _grep('ttH_012jets_scaleDown', madFiles) )
#   
#   FastSim_TTZ_Powhel  = kreator.makePrivateMCComponent('FastSim_TTZ_Powhel',  '/store/caf/user/gpetrucc/ttH/gen/powhel/arxiv-1208.2665/ttz',   _grep('ttz-events',   powhelFiles) )
#   FastSim_TTWp_Powhel = kreator.makePrivateMCComponent('FastSim_TTWp_Powhel', '/store/caf/user/gpetrucc/ttH/gen/powhel/arxiv-1208.2665/ttwp',  _grep('ttwp-events',  powhelFiles) )
#   FastSim_TTWm_Powhel = kreator.makePrivateMCComponent('FastSim_TTWm_Powhel', '/store/caf/user/gpetrucc/ttH/gen/powhel/arxiv-1208.2665/ttwm',  _grep('ttwm-events',  powhelFiles) )
#   
#   FastSim_TTHJets_tuneZ2 = kreator.makePrivateMCComponent('FastSim_TTHJets_tuneZ2',  '/store/caf/user/gpetrucc/ttH/gen/2013-05-07/ttH_012jets_scaleDown', _grep('ttH_pythia_Z2-', pythiaFiles) )
#   FastSim_TTHJets_tuneZ2Star = kreator.makePrivateMCComponent('FastSim_TTHJets_tuneZ2Star',  '/store/caf/user/gpetrucc/ttH/gen/2013-05-07/ttH_012jets_scaleDown', _grep('ttH_pythia_Z2Star-', pythiaFiles) )
#   FastSim_TTHJets_tuneD6T = kreator.makePrivateMCComponent('FastSim_TTHJets_tuneD6T',  '/store/caf/user/gpetrucc/ttH/gen/2013-05-07/ttH_012jets_scaleDown', _grep('ttH_pythia_D6T-', pythiaFiles) )
#   FastSim_TTHJets_tuneProQ20 = kreator.makePrivateMCComponent('FastSim_TTHJets_tuneProQ20',  '/store/caf/user/gpetrucc/ttH/gen/2013-05-07/ttH_012jets_scaleDown', _grep('ttH_pythia_ProQ20-', pythiaFiles) )
#   FastSim_TTHJets_tuneP11 = kreator.makePrivateMCComponent('FastSim_TTHJets_tuneP11',  '/store/caf/user/gpetrucc/ttH/gen/2013-05-07/ttH_012jets_scaleDown', _grep('ttH_pythia_P11-', pythiaFiles) )
#   
#   #FastSim_TTGStar = kreator.makePrivateMCComponent('FastSim_TTGStar',  '/store/caf/user/gpetrucc/ttH/gen/2013-05-23/ttgstar_lo', _grep('ttgstar_lo', ttgstarFiles) )
#   FastSim_TTGStarMM = kreator.makePrivateMCComponent('FastSim_TTGStarMM',  '/store/caf/user/gpetrucc/ttH/gen/2013-05-23/ttgstar_lowmll_mumu_v2', _grep('ttgstar_lowmll_mumu_v2', lowmllFiles) )
#   FastSim_TTGStarEE = kreator.makePrivateMCComponent('FastSim_TTGStarEE',  '/store/caf/user/gpetrucc/ttH/gen/2013-05-23/ttgstar_lowmll_ee_v2', _grep('ttgstar_lowmll_ee_v2', lowmllFiles) )
#   FastSim_TTGStarTT = kreator.makePrivateMCComponent('FastSim_TTGStarTT',  '/store/caf/user/gpetrucc/ttH/gen/2013-05-23/ttgstar_lowmll_tautau', _grep('ttgstar_lowmll_tautau', lowmllFiles) )
#   FastSim_ZGStar4L = kreator.makePrivateMCComponent('FastSim_ZGStar4L',  '/store/caf/user/gpetrucc/ttH/gen/2013-05-23/zz_lowmll_v2', _grep('zz_lowmll_v2', lowmllFiles) )
#   
#   fastSimSamples = [ FastSim_TTZJets, FastSim_TTZJets_Up, FastSim_TTZJets_Dn, FastSim_TTZJets_MUp, FastSim_TTZJets_MDn,
#                      FastSim_TTWJets, FastSim_TTWJets_Up, FastSim_TTWJets_Dn, FastSim_TTWJets_MUp, FastSim_TTWJets_MDn,
#                      FastSim_TTHJets, FastSim_TTHJets_Up, FastSim_TTHJets_Dn,
#                      FastSim_TTHJets_tuneZ2,  FastSim_TTHJets_tuneZ2Star,  FastSim_TTHJets_tuneD6T,  FastSim_TTHJets_tuneProQ20,  FastSim_TTHJets_tuneP11, 
#                      FastSim_TTGStarMM, FastSim_TTGStarEE, FastSim_TTGStarTT, FastSim_ZGStar4L,
#                      FastSim_TTZ_Powhel, FastSim_TTWp_Powhel, FastSim_TTWm_Powhel   ]

fastSimSamples = []

#   
#   TBZToLL = kreator.makePrivateMCComponent('TBZToLL', '/store/caf/user/gpetrucc/ttH/grid/CMG/TBZToLL_4F_TuneZ2star_8TeV-madgraph-tauola', _grep('TBZToLL_4F_TuneZ2star_8TeV-madgraph-tauola', rareFiles) )
#   WpWpqq  = kreator.makePrivateMCComponent('WpWpqq',  '/store/caf/user/gpetrucc/ttH/grid/CMG/WpWpqq_8TeV-madgraph', _grep('WpWpqq_8TeV-madgraph', rareFiles) )
#   WmWmqq  = kreator.makePrivateMCComponent('WmWmqq',  '/store/caf/user/gpetrucc/ttH/grid/CMG/WmWmqq_8TeV-madgraph', _grep('WmWmqq_8TeV-madgraph', rareFiles) )
#   WWDPI   = kreator.makePrivateMCComponent('WWDPI',   '/store/caf/user/gpetrucc/ttH/grid/CMG/WW_DoubleScattering_8TeV-pythia8', _grep('WW_DoubleScattering_8TeV-pythia8', rareFiles) )
#   
#   rareSamples = [ TBZToLL, WpWpqq, WmWmqq, WWDPI ]

from CMGTools.TTHAnalysis.setup.Efficiencies import *


#Define splitting
for comp in mcSamples + fastSimSamples + extraMcSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 250 if comp.name in [ "WJets", "DY3JetsM50", "DY4JetsM50","W1Jets","W2Jets","W3Jets","W4Jets","TTJetsHad" ] else 100
    comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    comp.efficiency = eff2012
#for C in [DY1JetsM50,DY2JetsM50,DYJetsM50,WJets,TTLep,TTJetsLep,TTJetsSem]:
#    C.splitFactor = 500

for comp in fastSimSamples:
    comp.splitFactor = 10
#FastSim_ZGStar4L.splitFactor = 40

for comp in dataSamplesMu:
    comp.splitFactor = 800
    comp.isMC = False
    comp.isData = True

for comp in dataSamplesE:
    comp.splitFactor = 800
    comp.isMC = False
    comp.isData = True
    
for comp in dataSamplesMuE:
    comp.splitFactor = 400
    comp.isMC = False
    comp.isData = True

for comp in dataSamples1Mu:
    comp.splitFactor = 500
    comp.isMC = False
    comp.isData = True



