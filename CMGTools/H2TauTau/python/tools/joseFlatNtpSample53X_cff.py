import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import applyPostfix

HiggsMass = cms.vstring(['110','115','120','125','130','135','140','145'])
#SUSYMass = cms.vstring(['90','100','110','120','130','140','160','180','200','250','300','350','400','450','500','600','700','800','900','1000'])
SUSYMass = cms.vstring(['80','90','100','110','120','130','140','160','180','200','250','300','350','400','450','500','600','700','800','900','1000'])

#import os 
#recoilrootfile_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/Common/data/metRecoilCorrection/'
import os 
rootfile_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/Common/data/metRecoilCorrection/'
recoilCorr_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/metRecoilCorrection/'
httjson_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/json/'
httdata_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/'
 
#def loadFlatNtpSamples(process):
def configureFlatNtpSampleTauMu2011(module,sampleAlias):
    if sampleAlias == 'TauPlusXMay' : 
        module.path = "/TauPlusX/Run2011A-May10ReReco-v1/AOD/V5/PAT_CMG_V5_4_1"
        module.dataType = 1
        module.firstRun =  cms.int32( 163262 ) 
        module.lastRun =  cms.int32( 170000 )
        module.trigPath1 = cms.InputTag("HLT_IsoMu12_LooseIsoPFTau10_v4","hltFilterIsoMu12IsoPFTau10LooseIsolation","hltSingleMuIsoL3IsoFiltered12")

    if sampleAlias == 'TauPlusXv4' :
        module.path = "/TauPlusX/Run2011A-PromptReco-v4/AOD/V5/PAT_CMG_V5_4_1"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v2","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.trigPath2 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v4","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.trigPath3 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v5","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.trigPath4 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v6","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'TauPlusXAug' : 
        module.path = "/TauPlusX/Run2011A-05Aug2011-v1/AOD/V5/PAT_CMG_V5_4_1"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v8","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'TauPlusXOct3' : 
        module.path = "/TauPlusX/Run2011A-03Oct2011-v1/AOD/V5/PAT_CMG_V5_4_1"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v8","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.trigPath2 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'TauPlusX2011B' : 
        module.path = "/TauPlusX/Run2011B-PromptReco-v1/AOD/V5/PAT_CMG_V5_4_1"
        module.dataType = 1
        #module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")#to recover ~100pb-1 #removed in TriggerEfficiency lumi calculation
        module.trigPath2 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")#used when HLT_IsoMu15_LooseIsoPFTau15_v9 got prescaled 
        module.trigPath3 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")
        module.trigPath4 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")


    if sampleAlias == 'EmbeddedMay' : 
        #module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_May10thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_10May2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.dataType = 2

    if sampleAlias == 'EmbeddedV4' : 
        #module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.dataType = 2

    if sampleAlias == 'EmbeddedAug' : 
        #module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_Aug05thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_05Aug2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.dataType = 2

    if sampleAlias == 'EmbeddedOct' : 
        #module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.dataType = 2

    if sampleAlias == 'Embedded2011B' : 
        #module.path = "/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.dataType = 2


    if sampleAlias == 'WJetsToLNu' : 
        module.path = "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/HTTSKIM5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.randsigma = 0.10
        module.recoilCorrection = 2 
        module.fileCorrectTo =  rootfile_dir + 'recoilfit_wjets_njet.root'

    if sampleAlias == 'W3JetsToLNu' : 
        module.path = "/W3Jets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/PAT_CMG_V5_4_1" # V5 PFAOD was not made
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.recoilCorrection = 2 
        module.fileCorrectTo =  rootfile_dir + 'recoilfit_wjets_njet.root'

    if sampleAlias == 'W2JetsToLNu' : 
        module.path = "/W2Jets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1" # V5 PFAOD was not made
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.recoilCorrection = 2 
        module.fileCorrectTo =  rootfile_dir + 'recoilfit_wjets_njet.root' 

    if sampleAlias == 'TTJets' : 
        module.path = "/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5/HTTSKIM1/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'ZToTauTau' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.sampleGenEventType = 5
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.recoilCorrection = 1
        module.fileCorrectTo =  rootfile_dir + 'recoilfit_zjets_ltau_njet.root' 

    if sampleAlias == 'ZToTauTauM10To50' : 
        module.path = "/DYJetsToLL_M-10To50_TuneZ2_7TeV-madgraph/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.sampleGenEventType = 5
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.recoilCorrection = 1
        module.fileCorrectTo =  rootfile_dir + 'recoilfit_zjets_ltau_njet.root'
        

    if sampleAlias == 'ZToMuMu' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.sampleGenEventType = 3
        module.sampleTruthEventType = 3
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'ZToLJet' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.sampleGenEventType = 3
        module.sampleTruthEventType = 6
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'WW' : 
        module.path = "/WW_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'WZ' : 
        module.path = "/WZ_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'ZZ' : 
        module.path = "/ZZ_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")


    for i in range(0,8):

        if sampleAlias == "HiggsGG"+HiggsMass[i] :
            module.path ="/GluGluToHToTauTau_M-%s_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightFall112011AB'
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.recoilCorrection = 1
            module.fileCorrectTo =  rootfile_dir + 'recoilfit_zjets_ltau_njet.root'
            module.signalWeightMass = HiggsMass[i]
            
        if sampleAlias == "HiggsVBF"+HiggsMass[i] :
            module.path = "/VBF_HToTauTau_M-%s_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightFall112011AB'
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.recoilCorrection = 1
            module.fileCorrectTo =  rootfile_dir + 'recoilfit_zjets_ltau_njet.root'
            
        if sampleAlias == "HiggsVH"+HiggsMass[i] : 
            module.path = "/WH_ZH_TTH_HToTauTau_M-%s_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightFall112011AB'
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")


    for i in range(0,19):

        if sampleAlias == "SUSYBB"+SUSYMass[i] :
            module.path ="/SUSYBBHToTauTau_M-%s_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"% SUSYMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightFall112011AB'
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.recoilCorrection = 1 
            module.fileCorrectTo =  rootfile_dir + 'recoilfit_zjets_ltau_njet.root'

        if sampleAlias == "SUSYGG"+SUSYMass[i] :
            module.path ="/SUSYGluGluToHToTauTau_M-%s_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"% SUSYMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightFall112011AB'
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.recoilCorrection = 1 
            module.fileCorrectTo =  rootfile_dir + 'recoilfit_zjets_ltau_njet.root'


    if sampleAlias == 'GluGluSync' : 
        module.path = "/H2TAUTAU/Sync/GluGlu/AOD/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.signalWeightMass = '125'
        
    if sampleAlias == 'VBFSync' : 
        #module.path = "/H2TAUTAU/Sync/VBF/AOD/PAT_CMG_V5_4_1"
        module.path = "/H2TAUTAU/Sync/VBF/AOD/PAT_CMG_V5_5_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
       



###################Tau-Ele Channel###################################################################################################

def configureFlatNtpSampleTauEle2011(module,sampleAlias):
    if sampleAlias == 'TauPlusXMay' : 
        module.path = "/TauPlusX/Run2011A-May10ReReco-v1/AOD/V5/PAT_CMG_V5_4_1"       #160329-163869
        module.dataType = 1
        module.firstRun =  cms.int32( 163269 ) 
        module.lastRun =  cms.int32( 163869 )
        module.trigPath1 = cms.InputTag("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4","hltOverlapFilterIsoEle15IsoPFTau15","hltEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter")


    if sampleAlias == 'TauPlusXv4' :
        module.path = "/TauPlusX/Run2011A-PromptReco-v4/AOD/V5/PAT_CMG_V5_4_1"         #165071-168437
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle15IsoPFTau20","hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") #165088 - 165633
        module.trigPath2 = cms.InputTag("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v8","hltOverlapFilterIsoEle15IsoPFTau20","hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") #165970 - 166967
        module.trigPath3 = cms.InputTag("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v9","hltOverlapFilterIsoEle15IsoPFTau20","hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") #167039 - 167913
      
    if sampleAlias == 'TauPlusXAug' : 
        module.path = "/TauPlusX/Run2011A-05Aug2011-v1/AOD/V5/PAT_CMG_V5_4_1"         #170053-172619
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2","hltOverlapFilterIsoEle15TightIsoPFTau20","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") # 170249 - 173198
        
    if sampleAlias == 'TauPlusXOct3' : 
        module.path = "/TauPlusX/Run2011A-03Oct2011-v1/AOD/V5/PAT_CMG_V5_4_1"         #172635-175580
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2","hltOverlapFilterIsoEle15TightIsoPFTau20","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") # 170249 - 173198
        module.trigPath2 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltOverlapFilterIsoEle18MediumIsoPFTau20","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") # 173236 - 178380 -->possible overlap
                
    if sampleAlias == 'TauPlusX2011B' : 
        module.path = "/TauPlusX/Run2011B-PromptReco-v1/AOD/V5/PAT_CMG_V5_4_1"       #175832-180296
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltOverlapFilterIsoEle20MediumIsoPFTau20","hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20") #
        module.trigPath2 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v5","hltOverlapFilterIsoEle20MediumIsoPFTau20","hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20") #178420 - 179889
        module.trigPath3 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v6","hltOverlapFilterIsoEle20MediumIsoPFTau20","hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20") #179959 - 180252
        

    if sampleAlias == 'EmbeddedMay' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_10May2011_v1_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.dataType = 2

    if sampleAlias == 'EmbeddedV4' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.dataType = 2

    if sampleAlias == 'EmbeddedAug' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_05Aug2011_v1_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.dataType = 2

    if sampleAlias == 'EmbeddedOct' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.dataType = 2

    if sampleAlias == 'Embedded2011B' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_4_1"
        module.dataType = 2


    if sampleAlias == 'WJetsToLNu' :
        module.path = "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/HTTSKIM5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter")        
        module.randsigma = 0.
        module.recoilCorrection = 2 
        module.fileCorrectTo =  rootfile_dir + 'recoilfit_wjets_njet.root'

    if sampleAlias == 'TTJets' :
        module.path = "/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5/HTTSKIM1/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter")  

    if sampleAlias == 'ZToTauTau' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.sampleGenEventType = 5
        module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 
        module.recoilCorrection = 1
        module.fileCorrectTo =  rootfile_dir + 'recoilfit_zjets_ltau_njet.root' 

    if sampleAlias == 'ZToEE' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.sampleGenEventType = 1
        module.sampleTruthEventType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 

    if sampleAlias == 'ZToLJet' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.sampleGenEventType = 1
        module.sampleTruthEventType = 6
        module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 


    if sampleAlias == 'ZToMuMu' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.sampleGenEventType = 3
        module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 

    if sampleAlias == 'W3JetsToLNu' : 
        module.path = "/W3Jets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/PAT_CMG_V5_4_1" # V5 PFAOD was not made
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 
        module.recoilCorrection = 2 
        module.fileCorrectTo =  rootfile_dir + 'recoilfit_wjets_njet.root'

    if sampleAlias == 'W2JetsToLNu' : 
        module.path = "/W2Jets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1" # V5 PFAOD was not made
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 
        module.recoilCorrection = 2 
        module.fileCorrectTo =  rootfile_dir + 'recoilfit_wjets_njet.root' 

    if sampleAlias == 'WW' : 
        module.path = "/WW_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 

    if sampleAlias == 'WZ' : 
        module.path = "/WZ_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 

    if sampleAlias == 'ZZ' : 
        module.path = "/ZZ_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 


    for i in range(0,8):

        if sampleAlias == "HiggsGG"+HiggsMass[i] :
            module.path ="/GluGluToHToTauTau_M-%s_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightFall112011AB'
            module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 
            module.recoilCorrection = 1
            module.fileCorrectTo =  rootfile_dir + 'recoilfit_zjets_ltau_njet.root'
            module.signalWeightMass = HiggsMass[i]
            
        if sampleAlias == "HiggsVBF"+HiggsMass[i] :
            module.path = "/VBF_HToTauTau_M-%s_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightFall112011AB'
            module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 
            module.recoilCorrection = 1
            module.fileCorrectTo =  rootfile_dir + 'recoilfit_zjets_ltau_njet.root'
            
        if sampleAlias == "HiggsVH"+HiggsMass[i] : 
            module.path = "/WH_ZH_TTH_HToTauTau_M-%s_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightFall112011AB'
            module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 


    for i in range(0,19):

        if sampleAlias == "SUSYBB"+SUSYMass[i] :
            module.path ="/SUSYBBHToTauTau_M-%s_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"% SUSYMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightFall112011AB'
            module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 
            module.recoilCorrection = 1 
            module.fileCorrectTo =  rootfile_dir + 'recoilfit_zjets_ltau_njet.root'

        if sampleAlias == "SUSYGG"+SUSYMass[i] :
            module.path ="/SUSYGluGluToHToTauTau_M-%s_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1"% SUSYMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightFall112011AB'
            module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 
            module.recoilCorrection = 1 
            module.fileCorrectTo =  rootfile_dir + 'recoilfit_zjets_ltau_njet.root'

    if sampleAlias == 'GluGluSync' : 
        module.path = "/H2TAUTAU/Sync/GluGlu/AOD/PAT_CMG_V5_4_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter")
        module.signalWeightMass = '125'

    if sampleAlias == 'VBFSync' : 
        #module.path = "/H2TAUTAU/Sync/VBF/AOD/PAT_CMG_V5_4_1"
        module.path = "/H2TAUTAU/Sync/VBF/AOD/PAT_CMG_V5_5_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightFall112011AB'
        module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter")
        #module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_TrkIdT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTTrkIdTDphiFilter")
        ##this last one gives a better sync with Lorenzo but still distributions are not same
        #hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter
        #hltEle18CaloIdVTTrkIdTDphiFilter      


##From Lorenzo
#      if(run>=160404 && run<=161176)
#//HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1
#      else if(run>=161216 && run<=163261)
#//HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2
#      else if(run>=163269 && run<=163869)
#//HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4
#      else if(run>=165088 && run<=165633)
#//HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6
#      else if(run>=165970 && run<=166967)
#//HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v8
#      else if(run>=167039 && run<=167913)
#//HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v9
#      else if(run>=170249 && run<=173198)
#//HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2
#      else if(run>=173236 && run<=178380)
#//HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1
#|| //HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1
#      else if(run>=178420 && run<=179889)
#//HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v5
#      else if(run>=179959 && run<=180252)
#//HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v6


###from twiki
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4 	163269 - 163869 	168.6 	L1_SingleEG12 	 
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6 	165088 - 165633 	139.0 	L1_SingleEG12 	 
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v8 	165970 - 166967 	526.7 	L1_SingleEG12 	 
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v9 	167039 - 167913 	268.3 	L1_SingleEG12 	 
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2 	170249 - 173198 	785.7 	L1_SingleEG12 	tight iso
#HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1 	173236 - 178380 	1945 	L1_SingleEG15 	medium iso, ET(e)>18 GeV
#HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v5 	178420 - 179889 	706.7 	L1_SingleEG18 OR EG20 	medium iso, ET(e)>20 GeV
#HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v6 	179959 - 180252 	120.7 	L1_SingleEG18 OR EG20 	end of 2011 run



##filter names from Pietro
##        http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/H2TauTau/python/triggerMap.py?revision=1.11&view=markup
#    # 2011 TauEle Ele15_*_LooseIsoPFTau15
#    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1": ("hltOverlapFilterIsoEle15IsoPFTau15",      "hltEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter"),
#    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2": ("hltOverlapFilterIsoEle15IsoPFTau15",      "hltEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter"),    
#    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4": ("hltOverlapFilterIsoEle15IsoPFTau15",      "hltEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter"),
#    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v6": ("hltOverlapFilterIsoEle15IsoPFTau15",      "hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter"),
#    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v8": ("hltOverlapFilterIsoEle15IsoPFTau15",      "hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter"),
#    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v9": ("hltOverlapFilterIsoEle15IsoPFTau15",      "hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter"),                                                                                                                  
#    # 2011 TauEle Ele15_*_TightIsoPFTau20
#    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v1": ("hltOverlapFilterIsoEle15TightIsoPFTau20", "hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter"),
#    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2": ("hltOverlapFilterIsoEle15TightIsoPFTau20", "hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter"),
#    "HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2": ("hltOverlapFilterIsoEle18TightIsoPFTau20", "hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter"),
#    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4": ("hltOverlapFilterIsoEle15IsoPFTau20",      "hltEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter"),
#    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6": ("hltOverlapFilterIsoEle15IsoPFTau20",      "hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter"),
#    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v8": ("hltOverlapFilterIsoEle15IsoPFTau20",      "hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter"),
#    "HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v9": ("hltOverlapFilterIsoEle15IsoPFTau20",      "hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter"),
#    #PG old name for the lepton filter: hltEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter 
#    #PG I found the new one in the trigger menu for 10^33
#    # 2011 TauEle Ele18_*_MediumIsoPFTau20 
#    "HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1": ("hltOverlapFilterIsoEle18MediumIsoPFTau20", "hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter"),
#    # 2011 TauEle Ele20_*_MediumIsoPFTau20 
#    "HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1": ("hltOverlapFilterIsoEle20MediumIsoPFTau20", "hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20"),
#    "HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v5": ("hltOverlapFilterIsoEle20MediumIsoPFTau20", "hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20"),
#    "HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v6": ("hltOverlapFilterIsoEle20MediumIsoPFTau20", "hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20"),
#    "HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v7": ("hltOverlapFilterIsoEle20MediumIsoPFTau20", "hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20"),
#    # the following is used for synchronization
#    "HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1": ("hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter"),

    
####--------------------------------------muTau 2012--------------------------------------------
    
def configureFlatNtpSampleTauMu2012(module,sampleAlias):
    module.dataPeriodFlag = 2012
    module.muPtCut = 20.
    module.tauPtCut = 20.
    module.fileZmmData = recoilCorr_dir + 'recoilfit_datamm53X_20pv_njet.root'
    module.fileZmmMC = recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'
    module.mvaWeights2012 = cms.string(httdata_dir + 'VBFMVA_BDTG_HCP_52X.weights.xml')


    if sampleAlias == 'TauPlusX2012A' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-13Jul2012-v1/AOD/V5/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.trigPath2 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.trigPath3 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012A2' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-recover-06Aug2012-v1/AOD/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath2 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.jsonfile = httjson_dir + 'Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt'
        
    if sampleAlias == 'TauPlusX2012B' : #DAS range: 193752 - 197044
        module.path = "/TauPlusX/Run2012B-13Jul2012-v1/AOD/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012Cv1' : #DAS range: 197770 - 198913
        module.path = "/TauPlusX/Run2012C-24Aug2012-v1/AOD/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.jsonfile = httjson_dir + 'Cert_198022-198523_8TeV_24Aug2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012Cv2' : #DAS range: 198934 - 203755
        module.path = "/TauPlusX/Run2012C-PromptReco-v2/AOD/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.trigPath2 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.jsonfile = httjson_dir + 'Cert_190456-203002_8TeV_PromptReco_Collisions12_JSON.txt'



    #/afs/cern.ch/user/c/cmgtools/scratch0/ProductionSoftware/V5_4_0/5X/Type1METColin/Embedded/CMSSW_5_2_5/src/CMGTools/Common/prod/
    if sampleAlias == 'Embedded2012A' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2012A_PromptReco_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_6_0_B"
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_PromptReco_Collisions12_JSON-LowPileupRunsRemoved.txt'
        
    if sampleAlias == 'Embedded2012B1' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run193752to195135_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_6_0_B"
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_PromptReco_Collisions12_JSON-LowPileupRunsRemoved.txt'
        
    if sampleAlias == 'Embedded2012B2' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run195147to196070_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_6_0_B"
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_PromptReco_Collisions12_JSON-LowPileupRunsRemoved.txt'

    if sampleAlias == 'Embedded2012B3' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run196090to196531_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_6_0_B"
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_PromptReco_Collisions12_JSON-LowPileupRunsRemoved.txt'        



    if sampleAlias == 'ZToTauTau' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.sampleGenEventType = 5
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 31
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'

    if sampleAlias == 'ZToMuMu' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.sampleGenEventType = 3
        module.sampleTruthEventType = 3
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        
    if sampleAlias == 'ZToLJet' :
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.sampleGenEventType = 3
        module.sampleTruthEventType = 6
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
 
    if sampleAlias == 'WJetsToLNu' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 32
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'WJetsToLNu2' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 32
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W1JetsToLNu' :
        module.path = "/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 32
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W2JetsToLNu' :
        module.path = "/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 32
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W3JetsToLNu' :
        module.path = "/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 32
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W4JetsToLNu' :
        module.path = "/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 32
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'TTJets' :
        module.path = "/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 


    if sampleAlias == 'WW2L2Nu' :
        module.path = "/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'WZ3LNu' :
        module.path = "/WZJetsTo3LNu_TuneZ2_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'WZ2L2Q' :
        module.path = "/WZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'ZZ4L' :
        module.path = "/ZZJetsTo4L_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'ZZ2L2Nu' :
        module.path = "/ZZJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v3/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'ZZ2L2Q' :
        module.path = "/ZZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 



    if sampleAlias == 'TopTW' :        
        module.path = "/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'TopBTW' :        
        module.path = "/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 



    for i in range(0,8):

        if sampleAlias == "HiggsGG"+HiggsMass[i] :
            module.path ="/GluGluToHToTauTau_M-%s_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
            module.recoilCorrection = 31
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root' 

        if sampleAlias == "HiggsVBF"+HiggsMass[i] :
            module.dataType = 0
            module.path = "/VBF_HToTauTau_M-%s_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
            module.recoilCorrection = 31
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root' 

            
        if sampleAlias == "HiggsVH"+HiggsMass[i] :
            if i == 5 :
                module.path = "/WH_ZH_TTH_HToTauTau_M-%s_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            else :
                module.path = "/WH_ZH_TTH_HToTauTau_M-%s_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 


    for i in range(0,21):
        if sampleAlias == "SUSYBB"+SUSYMass[i] :
            module.path ="/SUSYBBHToTauTau_M-%s_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % SUSYMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

        if sampleAlias == "SUSYGG"+SUSYMass[i] :
            module.path ="/SUSYGluGluToHToTauTau_M-%s_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % SUSYMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 


    if sampleAlias == 'VBFSync' : 
        module.path = "/H2TAUTAU/Sync/2012/VBF/AOD/PAT_CMG_V5_6_0_B"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MCICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 


    
  

####--------------------------------------e-Tau 2012--------------------------------------------

def configureFlatNtpSampleTauEle2012(module,sampleAlias):
    module.dataPeriodFlag = 2012
    module.muPtCut = 24.
    module.tauPtCut = 20.
    module.fileZmmData = recoilCorr_dir + 'recoilfit_datamm53X_20pv_njet.root'
    module.fileZmmMC = recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'
    module.mvaWeights2012 = cms.string(httdata_dir + 'VBFMVA_BDTG_HCP_52X.weights.xml')
        
    if sampleAlias == 'TauPlusX2012A' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-13Jul2012-v1/AOD/V5/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPath2 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012A2' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-recover-06Aug2012-v1/AOD/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPath2 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.jsonfile = httjson_dir + 'Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt'
        
    if sampleAlias == 'TauPlusX2012B' : #DAS range: 193752 - 197044
        module.path = "/TauPlusX/Run2012B-13Jul2012-v1/AOD/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012Cv1' : #DAS range: 197770 - 198913
        module.path = "/TauPlusX/Run2012C-24Aug2012-v1/AOD/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath2 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v3","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath4 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath5 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.jsonfile = httjson_dir + 'Cert_198022-198523_8TeV_24Aug2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012Cv2' : #DAS range: 198934 - 203755
        module.path = "/TauPlusX/Run2012C-PromptReco-v2/AOD/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath2 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v3","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath4 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath5 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath6 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v7","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath7 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v8","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.jsonfile = httjson_dir + 'Cert_190456-203002_8TeV_PromptReco_Collisions12_JSON.txt'

        
    if sampleAlias == 'Embedded2012A' :
        module.path = "/DoubleMu/StoreResults-DoubleMu_2012A_PromptReco_v1_embedded_trans1_tau115_ptelec1_17had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_5_1"
        module.dataType = 2

    if sampleAlias == 'Embedded2012B1' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run193752to195135_embedded_trans1_tau115_ptelec1_17had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_5_1"
        module.dataType = 2

    if sampleAlias == 'Embedded2012B2' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run195147to196070_embedded_trans1_tau115_ptelec1_17had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_5_1"
        module.dataType = 2

    if sampleAlias == 'ZToTauTau' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.sampleGenEventType = 5
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 31
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'


    if sampleAlias == 'ZToEE' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.sampleGenEventType = 1
        module.sampleTruthEventType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'ZToLJet' :
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.sampleGenEventType = 3
        module.sampleTruthEventType = 6
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        
 
    if sampleAlias == 'WJetsToLNu' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 32
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'WJetsToLNu2' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 32
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W1JetsToLNu' :
        module.path = "/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 32
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W2JetsToLNu' :
        module.path = "/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 32
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W3JetsToLNu' :
        module.path = "/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 32
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W4JetsToLNu' :
        module.path = "/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 32
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 


    if sampleAlias == 'TTJets' :
        module.path = "/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'WW2L2Nu' :
        module.path = "/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'WZ3LNu' :
        module.path = "/WZJetsTo3LNu_TuneZ2_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'WZ2L2Q' :
        module.path = "/WZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'ZZ4L' :
        module.path = "/ZZJetsTo4L_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'ZZ2L2Nu' :
        module.path = "/ZZJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v3/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'ZZ2L2Q' :
        module.path = "/ZZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'TopTW' :        
        module.path = "/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'TopBTW' :        
        module.path = "/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")



    for i in range(0,8):

        if sampleAlias == "HiggsGG"+HiggsMass[i] :
            module.path ="/GluGluToHToTauTau_M-%s_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
            module.recoilCorrection = 31
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root' 

        if sampleAlias == "HiggsVBF"+HiggsMass[i] :
            module.path = "/VBF_HToTauTau_M-%s_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'                                             
           #hltOverlapFilterEle20LooseIsoPFTau20, hltIsoElePFTau20TrackLooseIso, hltOverlapFilterIsoEle20WP90LooseIsoPFTau20, hltOverlapFilterEle20LooseIsoPFTau20
            module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
            module.recoilCorrection = 31
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root' 

            
        if sampleAlias == "HiggsVH"+HiggsMass[i] :
            if i == 5 :
                module.path = "/WH_ZH_TTH_HToTauTau_M-%s_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            else :
                module.path = "/WH_ZH_TTH_HToTauTau_M-%s_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")



    for i in range(0,21):
        if sampleAlias == "SUSYBB"+SUSYMass[i] :
            module.path ="/SUSYBBHToTauTau_M-%s_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % SUSYMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")


        if sampleAlias == "SUSYGG"+SUSYMass[i] :
            module.path ="/SUSYGluGluToHToTauTau_M-%s_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % SUSYMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")


    if sampleAlias == 'VBFSync' : 
        module.path = "/H2TAUTAU/Sync/2012/VBF/AOD/PAT_CMG_V5_5_1"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MCICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")

