import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import applyPostfix

HiggsMass = cms.vstring(['110','115','120','125','130','135','140','145'])

#def loadFlatNtpSamples(process):
def configureFlatNtpSample(module,sampleAlias):
    if sampleAlias == 'TauPlusXMay' : 
        module.path = "/TauPlusX/Run2011A-May10ReReco-v1/AOD/V5"
        module.dataType = "Data"
        module.firstRun =  cms.int32( 163262 ) 
        module.lastRun =  cms.int32( 170000 )
        module.trigPath1 = cms.InputTag("HLT_IsoMu12_LooseIsoPFTau10_v4","hltFilterIsoMu12IsoPFTau10LooseIsolation","hltSingleMuIsoL3IsoFiltered12")

    if sampleAlias == 'TauPlusXv4' :
        module.path = "/TauPlusX/Run2011A-PromptReco-v4/AOD/V5"
        module.dataType = "Data"
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v2","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.trigPath2 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v4","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.trigPath3 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v5","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.trigPath4 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v6","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'TauPlusXAug' : 
        module.path = "/TauPlusX/Run2011A-05Aug2011-v1/AOD/V5"
        module.dataType = "Data"
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v8","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'TauPlusXOct3' : 
        module.path = "/TauPlusX/Run2011A-03Oct2011-v1/AOD/V5"
        module.dataType = "Data"
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v8","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.trigPath2 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'TauPlusX2011B' : 
        module.path = "/TauPlusX/Run2011B-PromptReco-v1/AOD/V5"
        module.dataType = "Data"
        #module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")#to recover ~100pb-1 #removed in TriggerEfficiency lumi calculation
        module.trigPath2 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")#used when HLT_IsoMu15_LooseIsoPFTau15_v9 got prescaled 
        module.trigPath3 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")
        module.trigPath4 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")
        module.diTauTag =  diTauColl

    if sampleAlias == 'EmbeddedMay' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_May10thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.dataType = "Embedded"

    if sampleAlias == 'EmbeddedV4' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.dataType = "Embedded"

    if sampleAlias == 'EmbeddedAug' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_Aug05thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.dataType = "Embedded"

    if sampleAlias == 'EmbeddedOct' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.dataType = "Embedded"

    if sampleAlias == 'Embedded2011B' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.dataType = "Embedded"


    if sampleAlias == 'WJetsToLNu' : 
        module.path = "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/HTTSKIM"
        module.dataType = "MC"
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.randsigma = 0.10

    if sampleAlias == 'TTJets' : 
        module.path = "/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5/HTTSKIM10"
        module.dataType = "MC"
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'ZToTauTau' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = "MC"
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.sampleGenEventType = 5
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'ZToMuMu' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = "MC"
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.sampleTruthEventType = 3
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'ZToLJet' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = "MC"
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.sampleTruthEventType = 6
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'WW' : 
        module.path = "/WW_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = "MC"
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'WZ' : 
        module.path = "/WZ_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = "MC"
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'ZZ' : 
        module.path = "/ZZ_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = "MC"
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")


    for i in range(0,8):

        if sampleAlias == "HiggsGG"+HiggsMass[i] :
            module.path ="/GluGluToHToTauTau_M-%s_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5" % HiggsMass[i]
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

        if sampleAlias == "HiggsVBF"+HiggsMass[i] :
            module.path = "/VBF_HToTauTau_M-%s_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5" % HiggsMass[i]
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

        if sampleAlias == "HiggsVH"+HiggsMass[i] : 
            module.path = "/WH_ZH_TTH_HToTauTau_M-%s_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5" % HiggsMass[i]
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")


