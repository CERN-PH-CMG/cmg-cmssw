import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import applyPostfix

HiggsMass = cms.vstring(['110','115','120','125','130','135','140','145'])

import os 
recoilrootfile_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/Common/data/metRecoilCorrection/'


#def loadFlatNtpSamples(process):
def configureFlatNtpSampleTauMu(module,sampleAlias):
    if sampleAlias == 'TauPlusXMay' : 
        module.path = "/TauPlusX/Run2011A-May10ReReco-v1/AOD/V5"
        module.dataType = 1
        module.firstRun =  cms.int32( 163262 ) 
        module.lastRun =  cms.int32( 170000 )
        module.trigPath1 = cms.InputTag("HLT_IsoMu12_LooseIsoPFTau10_v4","hltFilterIsoMu12IsoPFTau10LooseIsolation","hltSingleMuIsoL3IsoFiltered12")

    if sampleAlias == 'TauPlusXv4' :
        module.path = "/TauPlusX/Run2011A-PromptReco-v4/AOD/V5"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v2","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.trigPath2 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v4","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.trigPath3 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v5","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.trigPath4 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v6","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'TauPlusXAug' : 
        module.path = "/TauPlusX/Run2011A-05Aug2011-v1/AOD/V5"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v8","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'TauPlusXOct3' : 
        module.path = "/TauPlusX/Run2011A-03Oct2011-v1/AOD/V5"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v8","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.trigPath2 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'TauPlusX2011B' : 
        module.path = "/TauPlusX/Run2011B-PromptReco-v1/AOD/V5"
        module.dataType = 1
        #module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")#to recover ~100pb-1 #removed in TriggerEfficiency lumi calculation
        module.trigPath2 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")#used when HLT_IsoMu15_LooseIsoPFTau15_v9 got prescaled 
        module.trigPath3 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")
        module.trigPath4 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1")


    if sampleAlias == 'EmbeddedMay' : 
        #module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_May10thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_10May2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.dataType = 2

    if sampleAlias == 'EmbeddedV4' : 
        #module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.dataType = 2

    if sampleAlias == 'EmbeddedAug' : 
        #module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_Aug05thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_05Aug2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.dataType = 2

    if sampleAlias == 'EmbeddedOct' : 
        #module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.dataType = 2

    if sampleAlias == 'Embedded2011B' : 
        #module.path = "/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.path = "/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5"
        module.dataType = 2


    if sampleAlias == 'WJetsToLNu' : 
        module.path = "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/HTTSKIM5"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.randsigma = 0.10
        module.recoilCorrection = 0 #recoil corrector has memory leak and makes this sample fail
        module.fileCorrectTo =  recoilrootfile_dir + 'recoilfit_wjets_njet.root' 

    if sampleAlias == 'TTJets' : 
        module.path = "/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5/HTTSKIM1"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'ZToTauTau' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.sampleGenEventType = 5
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.recoilCorrection = 1
        module.fileCorrectTo =  recoilrootfile_dir + 'recoilfit_zjets_ltau_njet.root' 

    if sampleAlias == 'ZToMuMu' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.sampleTruthEventType = 3
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'ZToLJet' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.sampleTruthEventType = 6
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'WW' : 
        module.path = "/WW_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'WZ' : 
        module.path = "/WZ_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'ZZ' : 
        module.path = "/ZZ_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")


    for i in range(0,8):

        if sampleAlias == "HiggsGG"+HiggsMass[i] :
            module.path ="/GluGluToHToTauTau_M-%s_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.recoilCorrection = 0 #memory leak
            module.fileCorrectTo =  recoilrootfile_dir + 'recoilfit_zjets_ltau_njet.root'
            
        if sampleAlias == "HiggsVBF"+HiggsMass[i] :
            module.path = "/VBF_HToTauTau_M-%s_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.recoilCorrection = 0 #memory leak
            module.fileCorrectTo =  recoilrootfile_dir + 'recoilfit_zjets_ltau_njet.root'
            
        if sampleAlias == "HiggsVH"+HiggsMass[i] : 
            module.path = "/WH_ZH_TTH_HToTauTau_M-%s_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")


    if sampleAlias == 'GluGluSync' : 
        module.path = "/H2TAUTAU/Sync/GluGlu/AOD"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1","hltPFTau20TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'VBFSync' : 
        module.path = "/H2TAUTAU/Sync/VBF/AOD"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1","hltPFTau20TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
       


###################Tau-Ele Channel###################################################################################################
##need to
#-fix trigger paths
#-add rest of MC samples
#-add embedded samples
#-check valid ranges for each trigger path inside the c++ code

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



def configureFlatNtpSampleTauEle(module,sampleAlias):
    if sampleAlias == 'TauPlusXMay' : 
        module.path = "/TauPlusX/Run2011A-May10ReReco-v1/AOD/V5"       #160329-163869
        module.dataType = 1
        #module.firstRun =  cms.int32( 163262 ) ##used in mu-tau 
        module.firstRun =  cms.int32( 163269 ) 
        module.lastRun =  cms.int32( 163869 )
        module.trigPath1 = cms.InputTag("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4","","")


    if sampleAlias == 'TauPlusXv4' :
        module.path = "/TauPlusX/Run2011A-PromptReco-v4/AOD/V5"         #165071-168437
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6","","") #165088 - 165633
        module.trigPath2 = cms.InputTag("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v8","","") #165970 - 166967
        module.trigPath3 = cms.InputTag("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v9","","") #167039 - 167913    

      
    if sampleAlias == 'TauPlusXAug' : 
        module.path = "/TauPlusX/Run2011A-05Aug2011-v1/AOD/V5"         #170053-172619
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2","","") # 170249 - 173198 

        
    if sampleAlias == 'TauPlusXOct3' : 
        module.path = "/TauPlusX/Run2011A-03Oct2011-v1/AOD/V5"         #172635-175580
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2","","") # 170249 - 173198 
        module.trigPath2 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","","") # 173236 - 178380
                

    if sampleAlias == 'TauPlusX2011B' : 
        module.path = "/TauPlusX/Run2011B-PromptReco-v1/AOD/V5"       #175832-180296
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","","") #
        module.trigPath2 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v5","","") #178420 - 179889
        module.trigPath3 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v6","","") #179959 - 180252

    if sampleAlias == 'WJetsToLNu' : 
        module.path = "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/HTTSKIM5"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        #module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.randsigma = 0.10
        module.recoilCorrection = 0 #recoil corrector has memory leak and makes this sample fail
        module.fileCorrectTo =  recoilrootfile_dir + 'recoilfit_wjets_njet.root' 

    if sampleAlias == 'TTJets' : 
        module.path = "/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5/HTTSKIM1"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        #module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'ZToTauTau' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.sampleGenEventType = 5
        #module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.recoilCorrection = 1
        module.fileCorrectTo =  recoilrootfile_dir + 'recoilfit_zjets_ltau_njet.root' 

    if sampleAlias == 'ZToMuMu' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.sampleTruthEventType = 3
        #module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'ZToEE' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.sampleTruthEventType = 1
        #module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'ZToLJet' : 
        module.path = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        module.sampleTruthEventType = 6
        #module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")


    if sampleAlias == 'GluGluSync' : 
        module.path = "/H2TAUTAU/Sync/GluGlu/AOD"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        #module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1","hltPFTau20TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")

    if sampleAlias == 'VBFSync' : 
        module.path = "/H2TAUTAU/Sync/VBF/AOD"
        module.dataType = 0
        module.pupWeightName = cms.InputTag("vertexWeightFall112011AB")
        #module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1","hltPFTau20TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
       

#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1", kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15, "HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter", kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_EleObj, 0, "hltPFTau15TrackLooseIso", kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_TauObj, 0);
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15,"HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_EleObj,0,"hltPFTau15TrackLooseIso",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_TauObj,0);//auto
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15,"HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_EleObj,0,"hltPFTau15TrackLooseIso",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_TauObj,0);//auto
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v6",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15,"HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_EleObj,0,"hltPFTau15TrackLooseIso",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_TauObj,0);//auto
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v1",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20,"HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_EleObj,0,"hltPFTau20TrackLooseIso",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_TauObj,0);
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v2",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20,"HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_EleObj,0,"hltPFTau20TrackLooseIso",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_TauObj,0);//auto
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20,"HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_EleObj,0,"hltPFTau20TrackLooseIso",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_TauObj,0);//auto
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20,"HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_EleObj,0,"hltPFTau20TrackLooseIso",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_TauObj,0);//auto
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v8",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20,"HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_EleObj,0,"hltPFTau20TrackLooseIso",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_TauObj,0);//auto
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v9",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20,"HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_EleObj,0,"hltPFTau20TrackLooseIso",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_TauObj,0);//auto
#
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v1",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20,"HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_EleObj,0,"hltPFTauTighIso20TrackTightIso",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_TauObj,0);
#HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20,"HLTEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_EleObj,0,"hltPFTauTighIso20TrackTightIso",kHLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_TauObj,0);//auto
#HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v1",kHLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20,"HLTEle18CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_EleObj,0,"hltPFTau20TrackLooseIso",kHLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_TauObj,0);
#HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v2",kHLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20,"HLTEle18CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_EleObj,0,"hltPFTau20TrackLooseIso",kHLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_TauObj,0);//auto
#HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v3",kHLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20,"HLTEle18CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_EleObj,0,"hltPFTau20TrackLooseIso",kHLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_TauObj,0);//auto
#HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1",kHLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20,"HLTEle18CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_EleObj,0,"hltPFTauMediumIso20TrackMediumIso",kHLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_TauObj,0);
#HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1",kHLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20,"HLTEle20CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_EleObj,0,"hltPFTauMediumIso20TrackMediumIso",kHLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_TauObj,0);
#HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v5",kHLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20,"HLTEle20CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_EleObj,0,"hltPFTauMediumIso20TrackMediumIso",kHLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_TauObj,0);//auto
#HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v6",kHLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20,"HLTEle20CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter",kHLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_EleObj,0,"hltPFTauMediumIso20TrackMediumIso",kHLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_TauObj,0);//auto

