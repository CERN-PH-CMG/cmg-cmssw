import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import applyPostfix
from CMGTools.H2TauTau.tools.joseFlatNtpSample_cfi import tauMuFlatNtp

def loadFlatNtpSamples(process):

    process.tauMuFlatNtpTauPlusXMay = tauMuFlatNtp.clone()
    process.tauMuFlatNtpTauPlusXMay.path = "/TauPlusX/Run2011A-May10ReReco-v1/AOD/V2"
    process.tauMuFlatNtpTauPlusXMay.dataType = "Data"
    process.tauMuFlatNtpTauPlusXMay.firstRun =  cms.int32( 163262 ) 
    process.tauMuFlatNtpTauPlusXMay.lastRun =  cms.int32( 170000 )
    process.tauMuFlatNtpTauPlusXMay.trigPath1 = cms.InputTag("HLT_IsoMu12_LooseIsoPFTau10_v4","hltFilterIsoMu12IsoPFTau10LooseIsolation","hltSingleMuIsoL3IsoFiltered12")
    process.tauMuFlatNtpTauPlusXMay.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpTauPlusXv4 = tauMuFlatNtp.clone()
    process.tauMuFlatNtpTauPlusXv4.path = "/TauPlusX/Run2011A-PromptReco-v4/AOD/V2"
    process.tauMuFlatNtpTauPlusXv4.dataType = "Data"
    process.tauMuFlatNtpTauPlusXv4.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v2","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
    process.tauMuFlatNtpTauPlusXv4.trigPath2 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v4","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
    process.tauMuFlatNtpTauPlusXv4.trigPath3 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v5","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
    process.tauMuFlatNtpTauPlusXv4.trigPath4 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v6","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
    process.tauMuFlatNtpTauPlusXv4.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpTauPlusXAug  = tauMuFlatNtp.clone()
    process.tauMuFlatNtpTauPlusXAug.path = "/TauPlusX/Run2011A-05Aug2011-v1/AOD/V2"
    process.tauMuFlatNtpTauPlusXAug.dataType = "Data"
    process.tauMuFlatNtpTauPlusXAug.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v8","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
    process.tauMuFlatNtpTauPlusXAug.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpTauPlusXOct3 = tauMuFlatNtp.clone()
    process.tauMuFlatNtpTauPlusXOct3.path = "/TauPlusX/Run2011A-03Oct2011-v1/AOD/V2"
    process.tauMuFlatNtpTauPlusXOct3.dataType = "Data"
    process.tauMuFlatNtpTauPlusXOct3.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v8","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
    process.tauMuFlatNtpTauPlusXOct3.trigPath2 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
    process.tauMuFlatNtpTauPlusXOct3.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpTauPlusX2011B = tauMuFlatNtp.clone()
    process.tauMuFlatNtpTauPlusX2011B.path = "/TauPlusX/Run2011B-PromptReco-v1/AOD/V2"
    process.tauMuFlatNtpTauPlusX2011B.dataType = "Data"
    process.tauMuFlatNtpTauPlusX2011B.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")#to recover ~100pb-1 
    process.tauMuFlatNtpTauPlusX2011B.trigPath2 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")#used when HLT_IsoMu15_LooseIsoPFTau15_v9 got prescaled 
    process.tauMuFlatNtpTauPlusX2011B.trigPath3 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")
    process.tauMuFlatNtpTauPlusX2011B.trigPath4 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")
    process.tauMuFlatNtpTauPlusX2011B.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpEmbeddedMay = tauMuFlatNtp.clone()
    process.tauMuFlatNtpEmbeddedMay.path = "/DoubleMu/StoreResults-DoubleMu_2011A_May10thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER"
    process.tauMuFlatNtpEmbeddedMay.dataType = "Embedded"
    process.tauMuFlatNtpEmbeddedMay.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpEmbeddedV4 = tauMuFlatNtp.clone()
    process.tauMuFlatNtpEmbeddedV4.path = "/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER"
    process.tauMuFlatNtpEmbeddedV4.dataType = "Embedded"
    process.tauMuFlatNtpEmbeddedV4.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpEmbeddedAug = tauMuFlatNtp.clone()
    process.tauMuFlatNtpEmbeddedAug.path = "/DoubleMu/StoreResults-DoubleMu_2011A_Aug05thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER"
    process.tauMuFlatNtpEmbeddedAug.dataType = "Embedded"
    process.tauMuFlatNtpEmbeddedAug.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpEmbeddedOct = tauMuFlatNtp.clone()
    process.tauMuFlatNtpEmbeddedOct.path = "/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER"
    process.tauMuFlatNtpEmbeddedOct.dataType = "Embedded"
    process.tauMuFlatNtpEmbeddedOct.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpEmbedded2011B = tauMuFlatNtp.clone()
    process.tauMuFlatNtpEmbedded2011B.path = "/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER"
    process.tauMuFlatNtpEmbedded2011B.dataType = "Embedded"
    process.tauMuFlatNtpEmbedded2011B.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")


    process.tauMuFlatNtpWJetsToLNu = tauMuFlatNtp.clone()
    process.tauMuFlatNtpWJetsToLNu.path = "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"
    process.tauMuFlatNtpWJetsToLNu.dataType = "MC"
    process.tauMuFlatNtpWJetsToLNu.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
    process.tauMuFlatNtpWJetsToLNu.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
    process.tauMuFlatNtpWJetsToLNu.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpW2JetsToLNu = tauMuFlatNtp.clone()
    process.tauMuFlatNtpW2JetsToLNu.path = "/W2Jets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3"
    process.tauMuFlatNtpW2JetsToLNu.dataType = "MC"
    process.tauMuFlatNtpW2JetsToLNu.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
    process.tauMuFlatNtpW2JetsToLNu.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
    process.tauMuFlatNtpW2JetsToLNu.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpW3JetsToLNu = tauMuFlatNtp.clone()
    process.tauMuFlatNtpW3JetsToLNu.path = "/W3Jets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3"
    process.tauMuFlatNtpW3JetsToLNu.dataType = "MC"
    process.tauMuFlatNtpW3JetsToLNu.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
    process.tauMuFlatNtpW3JetsToLNu.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
    process.tauMuFlatNtpW3JetsToLNu.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpTTJets = tauMuFlatNtp.clone()
    process.tauMuFlatNtpTTJets.path = "/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"
    process.tauMuFlatNtpTTJets.dataType = "MC"
    process.tauMuFlatNtpTTJets.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
    process.tauMuFlatNtpTTJets.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
    process.tauMuFlatNtpTTJets.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpZToTauTau = tauMuFlatNtp.clone()
    process.tauMuFlatNtpZToTauTau.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"
    process.tauMuFlatNtpZToTauTau.dataType = "MC"
    process.tauMuFlatNtpZToTauTau.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
    process.tauMuFlatNtpZToTauTau.sampleGenEventType = 5
    process.tauMuFlatNtpZToTauTau.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
    process.tauMuFlatNtpZToTauTau.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpZToMuMu = tauMuFlatNtp.clone()
    process.tauMuFlatNtpZToMuMu.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"
    process.tauMuFlatNtpZToMuMu.dataType = "MC"
    process.tauMuFlatNtpZToMuMu.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
    process.tauMuFlatNtpZToMuMu.sampleTruthEventType = 3
    process.tauMuFlatNtpZToMuMu.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
    process.tauMuFlatNtpZToMuMu.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpZToLJet = tauMuFlatNtp.clone()
    process.tauMuFlatNtpZToLJet.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"
    process.tauMuFlatNtpZToLJet.dataType = "MC"
    process.tauMuFlatNtpZToLJet.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
    process.tauMuFlatNtpZToLJet.sampleTruthEventType = 6
    process.tauMuFlatNtpZToLJet.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
    process.tauMuFlatNtpZToLJet.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpWW  = tauMuFlatNtp.clone()
    process.tauMuFlatNtpWW.path = "/WW_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"
    process.tauMuFlatNtpWW.dataType = "MC"
    process.tauMuFlatNtpWW.pupWeightName = cms.InputTag("vertexWeight2011AB")
    process.tauMuFlatNtpWW.diTauTag =  cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpWZ = tauMuFlatNtp.clone()
    process.tauMuFlatNtpWZ.path = "/WZ_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"
    process.tauMuFlatNtpWZ.dataType = "MC"
    process.tauMuFlatNtpWZ.pupWeightName = cms.InputTag("vertexWeight2011AB")
    process.tauMuFlatNtpWZ.diTauTag = cms.InputTag("cmgTauMuCorSVFitFullSel")

    process.tauMuFlatNtpZZ = tauMuFlatNtp.clone()
    process.tauMuFlatNtpZZ.path = "/ZZ_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"
    process.tauMuFlatNtpZZ.dataType = "MC"
    process.tauMuFlatNtpZZ.pupWeightName = cms.InputTag("vertexWeight2011AB")
    process.tauMuFlatNtpZZ.diTauTag = cms.InputTag("cmgTauMuCorSVFitFullSel")


    HiggsMass = cms.vstring(['110','115','120','125','130','135','140','145'])
    for i in range(0,8):

        setattr(process,"tauMuFlatNtpHiggsGG"+HiggsMass[i],tauMuFlatNtp.clone())
        getattr(process,"tauMuFlatNtpHiggsGG"+HiggsMass[i]).path ="/GluGluToHToTauTau_M-%s_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/V2" % HiggsMass[i]
        getattr(process,"tauMuFlatNtpHiggsGG"+HiggsMass[i]).dataType = "MC"
        getattr(process,"tauMuFlatNtpHiggsGG"+HiggsMass[i]).pupWeightName = cms.InputTag("vertexWeight2011AB")
        getattr(process,"tauMuFlatNtpHiggsGG"+HiggsMass[i]).diTauTag = cms.InputTag("cmgTauMuCorSVFitFullSel")

        setattr(process,"tauMuFlatNtpHiggsVBF"+HiggsMass[i],tauMuFlatNtp.clone())
        getattr(process,"tauMuFlatNtpHiggsVBF"+HiggsMass[i]).path = "/VBF_HToTauTau_M-%s_7TeV-powheg-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2" % HiggsMass[i]
        getattr(process,"tauMuFlatNtpHiggsVBF"+HiggsMass[i]).dataType = "MC"
        getattr(process,"tauMuFlatNtpHiggsVBF"+HiggsMass[i]).pupWeightName = cms.InputTag("vertexWeight2011AB")
        getattr(process,"tauMuFlatNtpHiggsVBF"+HiggsMass[i]).diTauTag = cms.InputTag("cmgTauMuCorSVFitFullSel")

        setattr(process,"tauMuFlatNtpHiggsVH"+HiggsMass[i],tauMuFlatNtp.clone())
        getattr(process,"tauMuFlatNtpHiggsVH"+HiggsMass[i]).path = "/WH_ZH_TTH_HToTauTau_M-%s_7TeV-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2" % HiggsMass[i]
        getattr(process,"tauMuFlatNtpHiggsVH"+HiggsMass[i]).dataType = "MC"
        getattr(process,"tauMuFlatNtpHiggsVH"+HiggsMass[i]).pupWeightName = cms.InputTag("vertexWeight2011AB")
        getattr(process,"tauMuFlatNtpHiggsVH"+HiggsMass[i]).diTauTag = cms.InputTag("cmgTauMuCorSVFitFullSel")
        
