import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import applyPostfix
#from CMGTools.H2TauTau.tools.joseFlatNtpSample_cfi import flatNtp

diTauColl = cms.InputTag("cmgTauMuCorSVFitFullSel")
HiggsMass = cms.vstring(['110','115','120','125','130','135','140','145'])

#def loadFlatNtpSamples(process):
def configureFlatNtpSample(module,sampleAlias,year="2011"):

    if year == "2011":
        if sampleAlias == 'TauPlusXMay' : 
            module.path = "/TauPlusX/Run2011A-May10ReReco-v1/AOD/V2"
            module.dataType = "Data"
            module.firstRun =  cms.int32( 163262 ) 
            module.lastRun =  cms.int32( 170000 )
            module.trigPath1 = cms.InputTag("HLT_IsoMu12_LooseIsoPFTau10_v4","hltFilterIsoMu12IsoPFTau10LooseIsolation","hltSingleMuIsoL3IsoFiltered12")
            module.diTauTag =  diTauColl

        if sampleAlias == 'TauPlusXv4' :
            module.path = "/TauPlusX/Run2011A-PromptReco-v4/AOD/V2"
            module.dataType = "Data"
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v2","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.trigPath2 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v4","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.trigPath3 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v5","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.trigPath4 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v6","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag =  diTauColl

        if sampleAlias == 'TauPlusXAug' : 
            module.path = "/TauPlusX/Run2011A-05Aug2011-v1/AOD/V2"
            module.dataType = "Data"
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v8","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag =  diTauColl

        if sampleAlias == 'TauPlusXOct3' : 
            module.path = "/TauPlusX/Run2011A-03Oct2011-v1/AOD/V2"
            module.dataType = "Data"
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v8","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.trigPath2 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag =  diTauColl

        if sampleAlias == 'TauPlusX2011B' : 
            module.path = "/TauPlusX/Run2011B-PromptReco-v1/AOD/V2"
            module.dataType = "Data"
            #module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")#to recover ~100pb-1 #removed in TriggerEfficiency lumi calculation
            module.trigPath2 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")#used when HLT_IsoMu15_LooseIsoPFTau15_v9 got prescaled 
            module.trigPath3 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")
            module.trigPath4 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")
            module.diTauTag =  diTauColl

        if sampleAlias == 'EmbeddedMay' : 
            module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_May10thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER"
            module.dataType = "Embedded"
            module.diTauTag =  diTauColl

        if sampleAlias == 'EmbeddedV4' : 
            module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER"
            module.dataType = "Embedded"
            module.diTauTag =  diTauColl

        if sampleAlias == 'EmbeddedAug' : 
            module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_Aug05thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER"
            module.dataType = "Embedded"
            module.diTauTag =  diTauColl

        if sampleAlias == 'EmbeddedOct' : 
            module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER"
            module.dataType = "Embedded"
            module.diTauTag =  diTauColl

        if sampleAlias == 'Embedded2011B' : 
            module.path = "/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER"
            module.dataType = "Embedded"
            module.diTauTag =  diTauColl

        if sampleAlias == 'WJetsToLNu' : 
            module.path = "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag =  diTauColl
            module.randsigma = 0.10

        if sampleAlias == 'W2JetsToLNu' : 
            module.path = "/W2Jets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag =  diTauColl

        if sampleAlias == 'W3JetsToLNu' : 
            module.path = "/W3Jets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag =  diTauColl

        if sampleAlias == 'TTJets' : 
            module.path = "/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag =  diTauColl

        if sampleAlias == 'ZToTauTau' : 
            module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.sampleGenEventType = 5
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag =  diTauColl

        if sampleAlias == 'ZToMuMu' : 
            module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.sampleTruthEventType = 3
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag =  diTauColl

        if sampleAlias == 'ZToLJet' : 
            module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.sampleTruthEventType = 6
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag =  diTauColl

        if sampleAlias == 'WW' : 
            module.path = "/WW_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeight2011AB")
            module.diTauTag =  diTauColl

        if sampleAlias == 'WZ' : 
            module.path = "/WZ_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeight2011AB")
            module.diTauTag = diTauColl

        if sampleAlias == 'ZZ' : 
            module.path = "/ZZ_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeight2011AB")
            module.diTauTag = diTauColl


        for i in range(0,8):

            if sampleAlias == "HiggsGG"+HiggsMass[i] :
                module.path ="/GluGluToHToTauTau_M-%s_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/V2" % HiggsMass[i]
                module.dataType = "MC"
                module.pupWeightName = cms.InputTag("vertexWeight2011AB")
                module.diTauTag = diTauColl

            if sampleAlias == "HiggsVBF"+HiggsMass[i] :
                module.path = "/VBF_HToTauTau_M-%s_7TeV-powheg-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2" % HiggsMass[i]
                module.dataType = "MC"
                module.pupWeightName = cms.InputTag("vertexWeight2011AB")
                module.diTauTag = diTauColl

            if sampleAlias == "HiggsVH"+HiggsMass[i] : 
                module.path = "/WH_ZH_TTH_HToTauTau_M-%s_7TeV-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2" % HiggsMass[i]
                module.dataType = "MC"
                module.pupWeightName = cms.InputTag("vertexWeight2011AB")
                module.diTauTag = diTauColl



    ##Samples for special studies    
        if sampleAlias == "QCD_Pt_20_MuEnrichedPt_15" :
            module.path = "/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeight2011AB")
            module.diTauTag = diTauColl

        if sampleAlias == "QCDPt15to20" :
            module.path = "/QCD_Pt-15to20_MuPt5Enriched_TuneZ2_7TeV-pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM/V3"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeight2011AB")
            module.diTauTag = diTauColl

        if sampleAlias == "QCDPt20to30" :
            module.path = "/QCD_Pt-20to30_MuPt5Enriched_TuneZ2_7TeV-pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM/V3"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeight2011AB")
            module.diTauTag = diTauColl

        if sampleAlias == "QCDPt30to50" :
            module.path = "/QCD_Pt-30to50_MuPt5Enriched_TuneZ2_7TeV-pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM/V3"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeight2011AB")
            module.diTauTag = diTauColl


    ###Fall11 QCD samples        
        if sampleAlias == "QCDPt15to20" :
            module.path = "/QCD_Pt-15to20_MuPt5Enriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V4"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag = diTauColl

        if sampleAlias == "QCDPt20to30" :
            module.path = "/QCD_Pt-20to30_MuPt5Enriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V4"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag = diTauColl

        if sampleAlias == "QCDPt30to50" :
            module.path = "/QCD_Pt-30to50_MuPt5Enriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V4"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag = diTauColl

        if sampleAlias == "QCDPt50to80" :
            module.path = "/QCD_Pt-50to80_MuPt5Enriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V4"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag = diTauColl

        if sampleAlias == "QCDPt80to120" :
            module.path = "/QCD_Pt-80to120_MuPt5Enriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V4"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag = diTauColl

        if sampleAlias == "QCDPt120to150" :
            module.path = "/QCD_Pt-120to150_MuPt5Enriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V4"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag = diTauColl

        if sampleAlias == "QCDPt150" :
            module.path = "/QCD_Pt-150_MuPt5Enriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V4"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.diTauTag = diTauColl

    ##########################################################
    ###################### 2012 Samples ####################
    ##########################################################
    if year == "2012":
        print "configuring sample in year=2012"
        if sampleAlias == 'ZToTauTau' : 
            module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V5-v2/AODSIM"
            module.dataType = "MC"
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.sampleGenEventType = 5
            module.trigPath1 = cms.InputTag("","","")
            module.diTauTag =  diTauColl

