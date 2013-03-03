from CMGTools.TTHAnalysis.samples.getFiles import getFiles
import CMGTools.RootTools.fwlite.Config as cfg
import os
from copy import copy

pat='V5_10_0'
skim=''
filepattern = 'cmgTuple.*root'
userName='cmgtools'



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

triggersFR_1mu  = [ 'HLT_Mu17_v*', 'HLT_Mu12_v*', 'HLT_Mu24_eta2p1_v*', 'HLT_Mu24_v*', 'HLT_Mu30_eta2p1_v*', 'HLT_Mu30_v*', 'HLT_Mu40_eta2p1_v*', 'HLT_Mu40_v*', 'HLT_IsoMu24_eta2p1_v*' ]
triggersFR_mumu = [ 'HLT_Mu17_Mu8_v*', 'HLT_Mu17_TkMu8_v*' ]
triggersFR_1e   = [ 'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*', 'HLT_Ele17_CaloIdL_CaloIsoVL_v*', 'HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*', 'HLT_Ele8__CaloIdL_CaloIsoVL_v*']
triggersFR_mue  = triggers_mue[:]
triggersFR_MC = triggersFR_1mu + triggersFR_mumu + triggersFR_1e + triggersFR_mue


#####COMPONENT CREATOR

from CMGTools.TTHAnalysis.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

#-----------MC---------------

TTH      =kreator.makeMCComponent('TTH','/TTH_Inclusive_M-125_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
TTWJets  =kreator.makeMCComponent('TTWJets','/TTWJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
TTZJets  =kreator.makeMCComponent('TTZJets','/TTZJets_8TeV-madgraph_v2/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
WWWJets  =kreator.makeMCComponent('WWWJets','/WWWJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
WWZJets  =kreator.makeMCComponent('WWZJets','/WWZNoGstarJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
WGs2MU   =kreator.makeMCComponent('WGs2MU','/WGstarToLNu2Mu_TuneZ2star_7TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
WGs2E    =kreator.makeMCComponent('WGs2E','/WGstarToLNu2E_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
WGs2Tau  =kreator.makeMCComponent('WGs2Tau','/WGstarToLNu2Tau_TuneZ2star_7TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZG =kreator.makeMCComponent('ZG','/ZGToLLG_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
TTG =kreator.makeMCComponent('TTG','/TTGJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
TTWWJets =kreator.makeMCComponent('TTWWJets','/TTWWJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
DYJetsM10=kreator.makeMCComponent('DYJetsM10','/DYJetsToLL_M-10To50filter_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
DYJetsM50=kreator.makeMCComponent('DYJetsM50','/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
DY1JetsM50=kreator.makeMCComponent('DY1JetsM50','/DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
DY2JetsM50=kreator.makeMCComponent('DY2JetsM50','/DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
DY3JetsM50=kreator.makeMCComponent('DY3JetsM50','/DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
DY4JetsM50=kreator.makeMCComponent('DY4JetsM50','/DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
TTLep    =kreator.makeMCComponent('TTLep','/TTTo2L2Nu2B_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
WWJets   =kreator.makeMCComponent('WWJets','/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
WZJets   =kreator.makeMCComponent('WZJets','/WZJetsTo3LNu_TuneZ2_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ2e2mu  =kreator.makeMCComponent('ZZ2e2mu','/ZZTo2e2mu_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ2e2tau =kreator.makeMCComponent('ZZ2e2tau','/ZZTo2e2tau_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZ2mu2tau=kreator.makeMCComponent('ZZ2mu2tau','/ZZTo2mu2tau_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZTo4mu  =kreator.makeMCComponent('ZZTo4mu','/ZZTo4mu_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZTo4tau =kreator.makeMCComponent('ZZTo4tau','/ZZTo4tau_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
ZZTo4e   =kreator.makeMCComponent('ZZTo4e','/ZZTo4e_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
TTJets   =kreator.makeMCComponent('TTJets','/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
TtW      =kreator.makeMCComponent('TtW','/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
TbartW   =kreator.makeMCComponent('TbartW','/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
W1Jets   =kreator.makeMCComponent('W1Jets','/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
W2Jets   =kreator.makeMCComponent('W2Jets','/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
W3Jets   =kreator.makeMCComponent('W3Jets','/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
W4Jets   =kreator.makeMCComponent('W4Jets','/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
WGToLNuG =kreator.makeMCComponent('WGToLNuG','/WGToLNuG_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
WJets    =kreator.makeMCComponent('WJets','/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
#WJets_HT250To300=kreator.makeMCComponent('WJets_HT250To300','/WJetsToLNu_HT-250To300_8TeV-madgraph_v2/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
#WJets_HT300To400=kreator.makeMCComponent('WJets_HT300To400','/WJetsToLNu_HT-300To400_8TeV-madgraph_v2/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
#WJets_HT400ToInf=kreator.makeMCComponent('WJets_HT400ToInf','/WJetsToLNu_HT-400ToInf_8TeV-madgraph_v2/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)
QCDMuPt15=kreator.makeMCComponent('QCDMuPt15','/QCD_Pt_20_MuEnrichedPt_15_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v3/AODSIM/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)

mcSamples=[
TTH,TTWJets,TTZJets,WWWJets,WWZJets,WGs2MU,WGs2E,WGs2Tau,ZG,TTG,TTWWJets,DYJetsM10,DYJetsM50,DY1JetsM50,DY2JetsM50,DY3JetsM50,DY4JetsM50,TTLep,WWJets,WZJets,ZZ2e2mu,ZZ2e2tau,ZZ2mu2tau,ZZTo4mu,ZZTo4tau,ZZTo4e,TTJets,TtW,TbartW,W1Jets,W2Jets,W3Jets,W4Jets,WGToLNuG,WJets]#,WJets_HT250To300,WJets_HT300To400,WJets_HT400ToInf]


#-----------DATA---------------

dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/TTHAnalysis/data"
#lumi: 12.21+7.27+0.134 = 19.62 /fb @ 8TeV

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

SingleMuAB = cfg.DataComponent(
    name = 'SingleMuAB',
    files = (#getFiles('/SingleMu/Run2012A-13Jul2012-v1/AOD/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)+ \
             #getFiles('/SingleMu/Run2012A-recover-06Aug2012-v1/AOD/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)+
            getFiles('/SingleMu/Run2012B-13Jul2012-v1/AOD/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)),
    intLumi = 1,
    triggers = [],
    json = json
    )

SingleMuC = cfg.DataComponent(
    name = 'SingleMuC',
    files = getFiles('/SingleMu/Run2012C-24Aug2012-v1/AOD/V5_B/PAT_CMG_'+pat+'/'+skim,userName,filepattern)+ \
            getFiles('/SingleMu/Run2012C-PromptReco-v2/AOD/PAT_CMG_'+pat+'/'+skim,userName,filepattern),

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


SingleMuD = cfg.DataComponent(
    name = 'SingleMuD',
    files = getFiles('/SingleMu/Run2012D-PromptReco-v1/AOD/PAT_CMG_'+pat+'/'+skim,userName,filepattern),
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

SingleMuRec = cfg.DataComponent(
    name = 'DoubleMuRec',
    files =getFiles('/SingleMu/Run2012C-EcalRecover_11Dec2012-v1/AOD/PAT_CMG_'+pat+'/'+skim,userName,filepattern),
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

dataSamples1Mu=[SingleMuAB,SingleMuC,SingleMuD,SingleMuRec]


from CMGTools.TTHAnalysis.setup.Efficiencies import *


#Define splitting
for comp in mcSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 200 #is needed for WJets 
    comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    comp.efficiency = eff2012

for comp in dataSamplesMu:
    comp.splitFactor = 500
    comp.isMC = False
    comp.isData = True

for comp in dataSamplesE:
    comp.splitFactor = 500
    comp.isMC = False
    comp.isData = True
    
for comp in dataSamplesMuE:
    comp.splitFactor = 500
    comp.isMC = False
    comp.isData = True

for comp in dataSamples1Mu:
    comp.splitFactor = 500
    comp.isMC = False
    comp.isData = True



