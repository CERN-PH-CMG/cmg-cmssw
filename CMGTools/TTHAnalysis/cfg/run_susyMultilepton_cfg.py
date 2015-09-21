##########################################################
##       CONFIGURATION FOR SUSY MULTILEPTON TREES       ##
## skim condition: >= 2 loose leptons, no pt cuts or id ##
##########################################################
import PhysicsTools.HeppyCore.framework.config as cfg
import re


#-------- LOAD ALL ANALYZERS -----------

from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import *
from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption

#-------- SET OPTIONS AND REDEFINE CONFIGURATIONS -----------

is50ns = getHeppyOption("is50ns",False)
runData = getHeppyOption("runData",False)
runDataQCD = getHeppyOption("runDataQCD",False)
runFRMC = getHeppyOption("runFRMC",False)
scaleProdToLumi = getHeppyOption("scaleProdToLumi",-1) # produce rough equivalent of X /pb for MC datasets
SOS = getHeppyOption("SOS",False) ## switch True to overwrite settings for SOS skim (N.B. default settings are those from multilepton preselection)
saveSuperClusterVariables = getHeppyOption("saveSuperClusterVariables",False)
removeSeparateJetCorrections = getHeppyOption("removeSeparateJetCorrections",False)
doMETpreprocessor = getHeppyOption("doMETpreprocessor",False)
doAK4PFCHSchargedJets = getHeppyOption("doAK4PFCHSchargedJets",False)
forcedSplitFactor = getHeppyOption("splitFactor",-1)
forcedFineSplitFactor = getHeppyOption("fineSplitFactor",-1)

# Lepton Skimming
ttHLepSkim.minLeptons = 2
ttHLepSkim.maxLeptons = 999
#ttHLepSkim.idCut  = ""
#ttHLepSkim.ptCuts = []

# Run miniIso
lepAna.doMiniIsolation = True
lepAna.packedCandidates = 'packedPFCandidates'
lepAna.miniIsolationPUCorr = 'rhoArea'
lepAna.miniIsolationVetoLeptons = None # use 'inclusive' to veto inclusive leptons and their footprint in all isolation cones
lepAna.doIsolationScan = False

# Lepton Preselection
lepAna.loose_electron_id = "POG_MVA_ID_Run2_NonTrig_VLoose"
isolation = "miniIso"

if SOS == True:
## -- SOS preselection settings ---

    # Lepton Skimming
    ttHLepSkim.minLeptons = 2
    ttHLepSkim.maxLeptons = 999
    ttHLepSkim.ptCuts = [5,3]
    
    # Jet-Met Skimming
    ttHJetMETSkim.jetPtCuts = [100,]
    ttHJetMETSkim.metCut    = 100

    # Lepton Preselection
    lepAna.inclusive_muon_pt  = 3
    lepAna.loose_muon_pt  = 3
    lepAna.inclusive_electron_pt  = 5
    lepAna.loose_electron_pt  = 5
    isolation = "absIso03"

    # Lepton-Jet Cleaning
    jetAna.minLepPt = 20 
    # otherwise with only absIso cut at 10 GeV and no relIso we risk cleaning away good jets

#isolation = "ptRel"
if isolation == "ptRel": 
    # delay isolation cut for leptons of pt > 10, for which we do pTrel recovery
    lepAna.loose_muon_isoCut     = lambda muon : muon.relIso03 < 0.5 or muon.pt() > 10
    lepAna.loose_electron_isoCut = lambda elec : elec.relIso03 < 0.5 or elec.pt() > 10
    # in the cleaning, keep the jet if the lepton fails relIso or ptRel
    jetAna.jetLepArbitration = lambda jet,lepton : (
        lepton if (lepton.relIso03 < 0.4 or ptRelv1(lepton.p4(),jet.p4()) > 5) else jet
    )
    ttHCoreEventAna.leptonMVAKindTTH = "SusyWithBoost"
    ttHCoreEventAna.leptonMVAKindSusy = "SusyWithBoost" 
    ttHCoreEventAna.leptonMVAPathTTH = "CMGTools/TTHAnalysis/macros/leptons/trainingPHYS14leptonMVA_PHYS14eleMVA_MiniIso_ttH/weights/%s_BDTG.weights.xml"
    ttHCoreEventAna.leptonMVAPathSusy = "CMGTools/TTHAnalysis/macros/leptons/trainingPHYS14leptonMVA_PHYS14eleMVA_MiniIso_SusyT1/weights/%s_BDTG.weights.xml"
    # insert a second skimmer after the jet cleaning 
    ttHLepSkim2 = cfg.Analyzer(
        ttHLepSkimmer, name='ttHLepSkimmer2',
        minLeptons = 2,
        maxLeptons = 999,
        )
    susyCoreSequence.insert(susyCoreSequence.index(jetAna)+1, ttHLepSkim2)
elif isolation == "miniIso": 
    lepAna.loose_muon_isoCut     = lambda muon : muon.miniRelIso < 0.4
    lepAna.loose_electron_isoCut = lambda elec : elec.miniRelIso < 0.4
elif isolation == None:
    lepAna.loose_muon_isoCut     = lambda muon : True
    lepAna.loose_electron_isoCut = lambda elec : True
elif isolation == "absIso03":
    lepAna.loose_muon_absIso = 10.0
    lepAna.loose_electron_relIso = 99.0
    lepAna.loose_muon_relIso = 99.0
    lepAna.loose_electron_absIso = 10.0
else:
    # nothing to do, will use normal relIso03
    pass

# Switch on slow QGL
jetAna.doQG = True

# Switch off slow photon MC matching
photonAna.do_mc_match = False

# Loose Tau configuration
tauAna.loose_decayModeID = "decayModeFinding"
tauAna.loose_ptMin = 20
tauAna.loose_etaMax = 2.3
# Current ra7 config (but not ttH)
#tauAna.loose_vetoLeptonsPOG = True
#tauAna.loose_tauAntiMuonID = "againstMuonTight"
#tauAna.loose_tauAntiElectronID = "againstElectronLoose"
if False: #if cleaning jet-loose tau cleaning
    jetAna.cleanJetsFromTaus = True


#-------- ADDITIONAL ANALYZERS -----------

## Event Analyzer for susy multi-lepton (at the moment, it's the TTH one)
from CMGTools.TTHAnalysis.analyzers.ttHLepEventAnalyzer import ttHLepEventAnalyzer
ttHEventAna = cfg.Analyzer(
    ttHLepEventAnalyzer, name="ttHLepEventAnalyzer",
    minJets25 = 0,
    )

## JetTau analyzer, to be called (for the moment) once bjetsMedium are produced
from CMGTools.TTHAnalysis.analyzers.ttHJetTauAnalyzer import ttHJetTauAnalyzer
ttHJetTauAna = cfg.Analyzer(
    ttHJetTauAnalyzer, name="ttHJetTauAnalyzer",
    )

## Insert the FatJet, SV, HeavyFlavour analyzers in the sequence
susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna), 
                        ttHFatJetAna)
susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna), 
                        ttHSVAna)
susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna), 
                        ttHHeavyFlavourHadronAna)

## Insert declustering analyzer
from CMGTools.TTHAnalysis.analyzers.ttHDeclusterJetsAnalyzer import ttHDeclusterJetsAnalyzer
ttHDecluster = cfg.Analyzer(
    ttHDeclusterJetsAnalyzer, name='ttHDecluster',
    lepCut     = lambda lep,ptrel : lep.pt() > 10,
    maxSubjets = 6, # for exclusive reclustering
    ptMinSubjets = 5, # for inclusive reclustering
    drMin      = 0.2, # minimal deltaR(l,subjet) required for a successful subjet match
    ptRatioMax = 1.5, # maximum pt(l)/pt(subjet) required for a successful match
    ptRatioDiff = 0.1,  # cut on abs( pt(l)/pt(subjet) - 1 ) sufficient to call a match successful
    drMatch     = 0.02, # deltaR(l,subjet) sufficient to call a match successful
    ptRelMin    = 5,    # maximum ptRelV1(l,subjet) sufficient to call a match successful
    prune       = True, # also do pruning of the jets 
    pruneZCut       = 0.1, # pruning parameters (usual value in CMS: 0.1)
    pruneRCutFactor = 0.5, # pruning parameters (usual value in CMS: 0.5)
    verbose     = 0,   # print out the first N leptons
    jetCut = lambda jet : jet.pt() > 20,
    mcPartonPtCut = 20,
    mcLeptonPtCut =  5,
    mcTauPtCut    = 15,
    )
susyCoreSequence.insert(susyCoreSequence.index(ttHFatJetAna)+1, ttHDecluster)


from CMGTools.TTHAnalysis.analyzers.treeProducerSusyMultilepton import * 

if lepAna.doIsolationScan:
    leptonTypeSusyExtra.addVariables([
            NTupleVariable("scanAbsIsoCharged005", lambda x : x.ScanAbsIsoCharged005 if hasattr(x,'ScanAbsIsoCharged005') else -999, help="PF abs charged isolation dR=0.05, no pile-up correction"),
            NTupleVariable("scanAbsIsoCharged01", lambda x : x.ScanAbsIsoCharged01 if hasattr(x,'ScanAbsIsoCharged01') else -999, help="PF abs charged isolation dR=0.1, no pile-up correction"),
            NTupleVariable("scanAbsIsoCharged02", lambda x : x.ScanAbsIsoCharged02 if hasattr(x,'ScanAbsIsoCharged02') else -999, help="PF abs charged isolation dR=0.2, no pile-up correction"),
            NTupleVariable("scanAbsIsoCharged03", lambda x : x.ScanAbsIsoCharged03 if hasattr(x,'ScanAbsIsoCharged03') else -999, help="PF abs charged isolation dR=0.3, no pile-up correction"),
            NTupleVariable("scanAbsIsoCharged04", lambda x : x.ScanAbsIsoCharged04 if hasattr(x,'ScanAbsIsoCharged04') else -999, help="PF abs charged isolation dR=0.4, no pile-up correction"),
            NTupleVariable("scanAbsIsoNeutral005", lambda x : x.ScanAbsIsoNeutral005 if hasattr(x,'ScanAbsIsoNeutral005') else -999, help="PF abs neutral+photon isolation dR=0.05, no pile-up correction"),
            NTupleVariable("scanAbsIsoNeutral01", lambda x : x.ScanAbsIsoNeutral01 if hasattr(x,'ScanAbsIsoNeutral01') else -999, help="PF abs neutral+photon isolation dR=0.1, no pile-up correction"),
            NTupleVariable("scanAbsIsoNeutral02", lambda x : x.ScanAbsIsoNeutral02 if hasattr(x,'ScanAbsIsoNeutral02') else -999, help="PF abs neutral+photon isolation dR=0.2, no pile-up correction"),
            NTupleVariable("scanAbsIsoNeutral03", lambda x : x.ScanAbsIsoNeutral03 if hasattr(x,'ScanAbsIsoNeutral03') else -999, help="PF abs neutral+photon isolation dR=0.3, no pile-up correction"),
            NTupleVariable("scanAbsIsoNeutral04", lambda x : x.ScanAbsIsoNeutral04 if hasattr(x,'ScanAbsIsoNeutral04') else -999, help="PF abs neutral+photon isolation dR=0.4, no pile-up correction"),
            ])

# for electron scale and resolution checks
if saveSuperClusterVariables:
    leptonTypeSusyExtra.addVariables([
            NTupleVariable("e5x5", lambda x: x.e5x5() if (abs(x.pdgId())==11 and hasattr(x,"e5x5")) else -999, help="Electron e5x5"),
            NTupleVariable("r9", lambda x: x.r9() if (abs(x.pdgId())==11 and hasattr(x,"r9")) else -999, help="Electron r9"),
            NTupleVariable("sigmaIetaIeta", lambda x: x.sigmaIetaIeta() if (abs(x.pdgId())==11 and hasattr(x,"sigmaIetaIeta")) else -999, help="Electron sigmaIetaIeta"),
            NTupleVariable("sigmaIphiIphi", lambda x: x.sigmaIphiIphi() if (abs(x.pdgId())==11 and hasattr(x,"sigmaIphiIphi")) else -999, help="Electron sigmaIphiIphi"),
            NTupleVariable("hcalOverEcal", lambda x: x.hcalOverEcal() if (abs(x.pdgId())==11 and hasattr(x,"hcalOverEcal")) else -999, help="Electron hcalOverEcal"),
            NTupleVariable("full5x5_e5x5", lambda x: x.full5x5_e5x5() if (abs(x.pdgId())==11 and hasattr(x,"full5x5_e5x5")) else -999, help="Electron full5x5_e5x5"),
            NTupleVariable("full5x5_r9", lambda x: x.full5x5_r9() if (abs(x.pdgId())==11 and hasattr(x,"full5x5_r9")) else -999, help="Electron full5x5_r9"),
            NTupleVariable("full5x5_sigmaIetaIeta", lambda x: x.full5x5_sigmaIetaIeta() if (abs(x.pdgId())==11 and hasattr(x,"full5x5_sigmaIetaIeta")) else -999, help="Electron full5x5_sigmaIetaIeta"),
            NTupleVariable("full5x5_sigmaIphiIphi", lambda x: x.full5x5_sigmaIphiIphi() if (abs(x.pdgId())==11 and hasattr(x,"full5x5_sigmaIphiIphi")) else -999, help="Electron full5x5_sigmaIphiIphi"),
            NTupleVariable("full5x5_hcalOverEcal", lambda x: x.full5x5_hcalOverEcal() if (abs(x.pdgId())==11 and hasattr(x,"full5x5_hcalOverEcal")) else -999, help="Electron full5x5_hcalOverEcal"),
            NTupleVariable("correctedEcalEnergy", lambda x: x.correctedEcalEnergy() if (abs(x.pdgId())==11 and hasattr(x,"correctedEcalEnergy")) else -999, help="Electron correctedEcalEnergy"),
            NTupleVariable("eSuperClusterOverP", lambda x: x.eSuperClusterOverP() if (abs(x.pdgId())==11 and hasattr(x,"eSuperClusterOverP")) else -999, help="Electron eSuperClusterOverP"),
            NTupleVariable("ecalEnergy", lambda x: x.ecalEnergy() if (abs(x.pdgId())==11 and hasattr(x,"ecalEnergy")) else -999, help="Electron ecalEnergy"),
            NTupleVariable("superCluster_rawEnergy", lambda x: x.superCluster().rawEnergy() if (abs(x.pdgId())==11 and hasattr(x,"superCluster")) else -999, help="Electron superCluster.rawEnergy"),
            NTupleVariable("superCluster_preshowerEnergy", lambda x: x.superCluster().preshowerEnergy() if (abs(x.pdgId())==11 and hasattr(x,"superCluster")) else -999, help="Electron superCluster.preshowerEnergy"),
            NTupleVariable("superCluster_correctedEnergy", lambda x: x.superCluster().correctedEnergy() if (abs(x.pdgId())==11 and hasattr(x,"superCluster")) else -999, help="Electron superCluster.correctedEnergy"),
            NTupleVariable("superCluster_energy", lambda x: x.superCluster().energy() if (abs(x.pdgId())==11 and hasattr(x,"superCluster")) else -999, help="Electron superCluster.energy"),
            NTupleVariable("superCluster_clustersSize", lambda x: x.superCluster().clustersSize() if (abs(x.pdgId())==11 and hasattr(x,"superCluster")) else -999, help="Electron superCluster.clustersSize"),
            NTupleVariable("superCluster_seed.energy", lambda x: x.superCluster().seed().energy() if (abs(x.pdgId())==11 and hasattr(x,"superCluster")) else -999, help="Electron superCluster.seed.energy"),
])

jetAna.calculateSeparateCorrections = (not removeSeparateJetCorrections)
if jetAna.calculateSeparateCorrections:
    jetTypeSusyExtra.addVariables([
            NTupleVariable("CorrFactor_L1", lambda x: x.CorrFactor_L1 if hasattr(x,'CorrFactor_L1') else 0, help="L1 correction factor"),
            NTupleVariable("CorrFactor_L1L2", lambda x: x.CorrFactor_L1L2 if hasattr(x,'CorrFactor_L1L2') else 0, help="L1L2 correction factor"),
            NTupleVariable("CorrFactor_L1L2L3", lambda x: x.CorrFactor_L1L2L3 if hasattr(x,'CorrFactor_L1L2L3') else 0, help="L1L2L3 correction factor"),
            NTupleVariable("CorrFactor_L1L2L3Res", lambda x: x.CorrFactor_L1L2L3Res if hasattr(x,'CorrFactor_L1L2L3Res') else 0, help="L1L2L3Res correction factor")
            ])
    leptonTypeSusyExtra.addVariables([
            NTupleVariable("jetPt", lambda x: x.jet.pt() if x.jet!=x else x.pt(), help="matched jet corrected pt"),
            NTupleVariable("jetRawPt", lambda x: x.jet.pt() * x.jet.rawFactor() if x.jet!=x else x.pt(), help="matched jet raw pt"),
            NTupleVariable("jetEta", lambda x: x.jet.eta() if x.jet!=x else x.eta(), help="matched jet eta"),
            NTupleVariable("jetPhi", lambda x: x.jet.phi() if x.jet!=x else x.phi(), help="matched jet phi"),
            NTupleVariable("jetE", lambda x: x.jet.energy() if x.jet!=x else x.energy(), help="matched jet energy"),
            NTupleVariable("jetCorrFactor_L1", lambda x: x.jet.CorrFactor_L1 if hasattr(x.jet,'CorrFactor_L1') else 1, help="matched jet L1 correction factor"),
            NTupleVariable("jetCorrFactor_L1L2", lambda x: x.jet.CorrFactor_L1L2 if hasattr(x.jet,'CorrFactor_L1L2') else 1, help="matched jet L1L2 correction factor"),
            NTupleVariable("jetCorrFactor_L1L2L3", lambda x: x.jet.CorrFactor_L1L2L3 if hasattr(x.jet,'CorrFactor_L1L2L3') else 1, help="matched jet L1L2L3 correction factor"),
            NTupleVariable("jetCorrFactor_L1L2L3Res", lambda x: x.jet.CorrFactor_L1L2L3Res if hasattr(x.jet,'CorrFactor_L1L2L3Res') else 1, help="matched jet L1L2L3Res correction factor"),        
            NTupleVariable("jetPtRatio_LepAwareJECv2", lambda lepton: lepton.pt()/jetLepAwareJEC(lepton).Pt() if hasattr(lepton,'jet') else -1, help="pt(lepton)/[rawpt(jet-PU-lep)*L2L3Res+pt(lepton)]"),
            NTupleVariable("jetPtRelv2", lambda lepton : ptRelv2(lepton) if hasattr(lepton,'jet') else -1, help="pt of the lepton transverse to the jet axis (subtracting the lepton) - v2"),
            NTupleVariable("jetPtRelHv2", lambda lepton : ptRelHv2(lepton) if hasattr(lepton,'jet') else -1, help="pt of the jet (subtracting the lepton) transverse to the lepton axis - v2"),
            ])
    leptonTypeSusyExtra.addSubObjects([
            NTupleSubObject("jetLepAwareJEC",lambda x: jetLepAwareJEC(x), tlorentzFourVectorType)
            ])

## Tree Producer
treeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='treeProducerSusyMultilepton',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     defaultFloatType = 'F', # use Float_t for floating point
     PDFWeights = PDFWeights,
     globalVariables = susyMultilepton_globalVariables,
     globalObjects = susyMultilepton_globalObjects,
     collections = susyMultilepton_collections,
)

## histo counter
susyCoreSequence.insert(susyCoreSequence.index(skimAnalyzer),
                        susyCounter)

# HBHE new filter
from CMGTools.TTHAnalysis.analyzers.hbheAnalyzer import hbheAnalyzer
hbheAna = cfg.Analyzer(
    hbheAnalyzer, name="hbheAnalyzer", IgnoreTS4TS5ifJetInLowBVRegion=False
    )
susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna),hbheAna)
treeProducer.globalVariables.append(NTupleVariable("hbheFilterNew50ns", lambda ev: ev.hbheFilterNew50ns, int, help="new HBHE filter for 50 ns"))
treeProducer.globalVariables.append(NTupleVariable("hbheFilterNew25ns", lambda ev: ev.hbheFilterNew25ns, int, help="new HBHE filter for 25 ns"))
treeProducer.globalVariables.append(NTupleVariable("hbheFilterIso", lambda ev: ev.hbheFilterIso, int, help="HBHE iso-based noise filter"))

#additional MET quantities
metAna.doTkMet = True
treeProducer.globalVariables.append(NTupleVariable("met_trkPt", lambda ev : ev.tkMet.pt() if  hasattr(ev,'tkMet') else  0, help="tkmet p_{T}"))
treeProducer.globalVariables.append(NTupleVariable("met_trkPhi", lambda ev : ev.tkMet.phi() if  hasattr(ev,'tkMet') else  0, help="tkmet phi"))

# MET preprocessor and ak4PFchs charged-only jets
if doMETpreprocessor:
    susyCoreSequence.insert(susyCoreSequence.index(metAna)+1,metNoHFAna)
    metNoHFAna.doTkMet = True
    treeProducer.globalObjects.update({"metNoHF"  : NTupleObject("metNoHF", metType, help="PF E_{T}^{miss}, after type 1 corrections (NoHF)")})
    treeProducer.globalVariables.append(NTupleVariable("metNoHF_rawPt", lambda ev : ev.metNoHF.uncorPt() if  hasattr(ev,'metNoHF') else  0, help="raw NoHF met p_{T}"))
    treeProducer.globalVariables.append(NTupleVariable("metNoHF_rawPhi", lambda ev : ev.metNoHF.uncorPhi() if  hasattr(ev,'metNoHF') else  0, help="raw NoHF met phi"))
    treeProducer.globalVariables.append(NTupleVariable("metNoHF_trkPt", lambda ev : ev.tkMetNoHF.pt() if  hasattr(ev,'tkMetNoHF') else  0, help="tkmetNoHF p_{T}"))
    treeProducer.globalVariables.append(NTupleVariable("metNoHF_trkPhi", lambda ev : ev.tkMetNoHF.phi() if  hasattr(ev,'tkMetNoHF') else  0, help="tkmetNoHF phi"))
if doAK4PFCHSchargedJets:
    if not doMETpreprocessor: raise RuntimeError, "ak4PFchs charged-only jets are reclustered in the MET preprocessor, but this configuration is not going to run it"
    treeProducer.collections["cleanJetsPFChargedCHS"] = NTupleCollection("JetPFChargedCHS", jetTypeSusyExtra, 15, help="Central PFChargedCHS jets after full selection and cleaning, sorted by pt") # ak4PFchs charged-only jets
    susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna), pfChargedCHSjetAna)
#treeProducer.collections["jetsAllNoID"] = NTupleCollection("AllJet", jetTypeSusyExtra, 15, help="Central jets, sorted by pt") # warning, increases tree size considerably

#-------- SAMPLES AND TRIGGERS -----------


from CMGTools.RootTools.samples.triggers_13TeV_Spring15 import *
from CMGTools.RootTools.samples.triggers_8TeV import triggers_1mu_8TeV, triggers_mumu_8TeV, triggers_mue_8TeV, triggers_ee_8TeV;
triggerFlagsAna.triggerBits = {
    'DoubleMu' : triggers_mumu_iso,
    'DoubleMuSS' : triggers_mumu_ss,
    'DoubleMuNoIso' : triggers_mumu_noniso,
    'DoubleEl' : triggers_ee,
    'MuEG'     : triggers_mue,
    'DoubleMuHT' : triggers_mumu_ht,
    'DoubleElHT' : triggers_ee_ht,
    'MuEGHT' : triggers_mue_ht,
    'TripleEl' : triggers_3e,
    'TripleMu' : triggers_3mu,
    'TripleMuA' : triggers_3mu_alt,
    'DoubleMuEl' : triggers_2mu1e,
    'DoubleElMu' : triggers_2e1mu,
    'SingleMu' : triggers_1mu_iso,
    'SingleMu50ns' : triggers_1mu_iso_50ns,
    'SingleEl'     : triggers_1e,
    'SingleEl50ns' : triggers_1e_50ns,
    'SingleMu_8TeV' : triggers_1mu_8TeV + triggers_1mu_iso_r,
    'DoubleMu_8TeV' : triggers_mumu_8TeV + triggers_mumu_run1,
    'MuEG_8TeV'     : triggers_mue_8TeV + triggers_mue_run1,
    'DoubleEl_8TeV' : triggers_ee_8TeV + triggers_ee_run1,
    'MonoJet80MET90' : triggers_Jet80MET90,
    'MonoJet80MET120' : triggers_Jet80MET120,
    'METMu5' : triggers_MET120Mu5,
}

from CMGTools.RootTools.samples.samples_13TeV_74X import *
from CMGTools.RootTools.samples.samples_13TeV_74X_susySignalsPriv import *
from CMGTools.RootTools.samples.samples_8TeVReReco_74X import *
from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import *

selectedComponents = [];

### 8TeV data 74X ReReco
#selectedComponents = [ SingleMu_742, MuEG_742, DoubleMu_742 ] ; is50ns = True
### 25 ns 74X MC samples
#selectedComponents = [ TTJets, TTJets_LO, WJetsToLNu, DYJetsToLL_M10to50,  DYJetsToLL_M50,  ] + SingleTop + DiBosons + TTV + Higgs ; is50ns = False
#selectedComponents = mcSamplesPriv ; is50ns = False
### 50 ns 74X MC samples
#selectedComponents = [ DYJetsToLL_M10to50_50ns, DYJetsToLL_M50_50ns, TBar_tWch_50ns, TTJets_LO_50ns, TToLeptons_tch_50ns, T_tWch_50ns, WJetsToLNu_50ns, WWTo2L2Nu_50ns, WZp8_50ns, ZZp8_50ns, TTJets_50ns ] ; is50ns = True
#selectedComponents = [ TT_pow_50ns ] ; is50ns = True
#selectedComponents = [ DYJetsToLL_LO_M50_50ns ] ; is50ns = True

isData = False

if scaleProdToLumi>0: # select only a subset of a sample, corresponding to a given luminosity (assuming ~30k events per MiniAOD file, which is ok for central production)
    target_lumi = scaleProdToLumi # in inverse picobarns
    for c in selectedComponents:
        if not c.isMC: continue
        nfiles = int(min(ceil(target_lumi * c.xSection / 30e3), len(c.files)))
        #if nfiles < 50: nfiles = min(4*nfiles, len(c.files))
        print "For component %s, will want %d/%d files; AAA %s" % (c.name, nfiles, len(c.files), "eoscms" not in c.files[0])
        c.files = c.files[:nfiles]
        c.splitFactor = len(c.files)
        c.fineSplitFactor = 1


if runData: # For running on data
    isData = True

#    # low-PU 50ns run (251721)
#    json = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-252126_13TeV_PromptReco_Collisions15_LOWPU_50ns_JSON.txt";
#    processing = "Run2015B-PromptReco-v1"; short = "Run2015B_v1"; run_ranges = [ (251721,251721) ]; useAAA=False; is50ns=True

#    # Run2015B, 50 ns, 3.8T + one run at 50 ns in Run2015C
#    json = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-255031_13TeV_PromptReco_Collisions15_50ns_JSON.txt";
#    processing = "Run2015B-17Jul2015-v1"; short = "Run2015B_17Jul2015"; run_ranges = [ (251244, 251562) ]; useAAA=False; is50ns=True
#    processing = "Run2015B-PromptReco-v1"; short = "Run2015B_v1"; run_ranges = [ (251643,251883) ]; useAAA=False; is50ns=True
#    json = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_254833_13TeV_PromptReco_Collisions15_JSON.txt"; # taken at 50 ns with 25 ns reconstruction
#    processing = "Run2015C-PromptReco-v1"; short = "Run2015C_v1"; run_ranges = [ (254833,254833) ]; useAAA=False; is50ns=True

    # Run2015C, 25 ns, 3.8T
    json = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-255031_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt"
    processing = "Run2015C-PromptReco-v1"; short = "Run2015C_v1"; run_ranges = [ (254231,254907) ]; useAAA=False; is50ns=False

    compSelection = ""; compVeto = ""
    DatasetsAndTriggers = []
    selectedComponents = []; vetos = []  
 
    if SOS == True:
        DatasetsAndTriggers.append( ("MET", triggers_Jet80MET90 + triggers_Jet80MET120 + triggers_MET120Mu5 ) )
        #DatasetsAndTriggers.append( ("SingleMuon", triggers_1mu_iso + triggers_1mu_iso_50ns + triggers_1mu_noniso) )
    else:
        DatasetsAndTriggers.append( ("DoubleMuon", triggers_mumu_iso + triggers_mumu_ss + triggers_mumu_ht + triggers_3mu + triggers_3mu_alt) )
        DatasetsAndTriggers.append( ("DoubleEG",   triggers_ee + triggers_ee_ht + triggers_3e) )
        DatasetsAndTriggers.append( ("MuonEG",     triggers_mue + triggers_mue_ht + triggers_2mu1e + triggers_2e1mu) )
        DatasetsAndTriggers.append( ("SingleMuon", triggers_1mu_iso + triggers_1mu_iso_50ns + triggers_1mu_noniso) )
        DatasetsAndTriggers.append( ("SingleElectron", triggers_1e + triggers_1e_50ns) )

        if runDataQCD: # for fake rate measurements in data
            lepAna.loose_muon_dxy = 999
            lepAna.loose_electron_dxy = 999
            ttHLepSkim.minLeptons = 1
            FRTrigs = triggers_FR_1mu_iso + triggers_FR_1mu_noiso + triggers_FR_1e_noiso + triggers_FR_1e_iso
            for t in FRTrigs:
                tShort = t.replace("HLT_","FR_").replace("_v*","")
                triggerFlagsAna.triggerBits[tShort] = [ t ]
                FRTrigs_mu = triggers_FR_1mu_iso + triggers_FR_1mu_noiso
                FRTrigs_el = triggers_FR_1e_noiso + triggers_FR_1e_iso
                DatasetsAndTriggers = [ (pd,trig) for pd,trig in DatasetsAndTriggers if pd in ['DoubleMuon','DoubleEG'] ]
                for pd,trig in DatasetsAndTriggers:
                    if pd in ['DoubleMuon','SingleMuon']:
                        trig.extend(FRTrigs_mu)
                    elif pd in ['DoubleEG','SingleElectron']:
                        trig.extend(FRTrigs_el)
                    else:
                        print 'the strategy for trigger selection on MuonEG for FR studies should yet be implemented'
                        assert(False)
                        
    for pd,triggers in DatasetsAndTriggers:
        for run_range in run_ranges:
            label = "runs_%d_%d" % run_range if run_range[0] != run_range[1] else "run_%d" % (run_range[0],)
            compname = pd+"_"+short+"_"+label
            if ((compSelection and not re.search(compSelection, compname)) or
                (compVeto      and     re.search(compVeto,      compname))):
                    print "Will skip %s" % (compname)
                    continue
            comp = kreator.makeDataComponent(compname, 
                                             "/"+pd+"/"+processing+"/MINIAOD", 
                                             "CMS", ".*root", 
                                             json=json, 
                                             run_range=run_range, 
                                             triggers=triggers[:], vetoTriggers = vetos[:],
                                             useAAA=useAAA)
            print "Will process %s (%d files)" % (comp.name, len(comp.files))
#            print "\ttrigger sel %s, veto %s" % (triggers, vetos)
            comp.splitFactor = len(comp.files)
            comp.fineSplitFactor = 1
            selectedComponents.append( comp )
        vetos += triggers
    if json is None:
        susyCoreSequence.remove(jsonAna)

if runFRMC: # QCD
    selectedComponents = QCD_MuX_50ns + QCD_ElX + [DYJetsToLL_M50_50ns, WJetsToLNu_50ns, TTJets_50ns]
    lepAna.loose_muon_dxy = 999
    lepAna.loose_electron_dxy = 999
    ttHLepSkim.minLeptons = 1
    FRTrigs = triggers_FR_1mu_iso + triggers_FR_1mu_noiso + triggers_FR_1e_noiso + triggers_FR_1e_iso
    for c in selectedComponents:
        c.triggers = [] # FRTrigs
        c.vetoTriggers = [] 
        c.splitFactor = len(c.files)/4
    for t in FRTrigs:
        tShort = t.replace("HLT_","FR_").replace("_v*","")
        triggerFlagsAna.triggerBits[tShort] = [ t ]
    treeProducer.collections = {
        "selectedLeptons" : NTupleCollection("LepGood",  leptonTypeSusyExtra, 8, help="Leptons after the preselection"),
        "cleanJets"       : NTupleCollection("Jet",     jetTypeSusy, 15, help="Cental jets after full selection and cleaning, sorted by pt"),
    }

if is50ns:
    jetAna.mcGT     = "Summer15_50nsV5_MC"
    jetAna.dataGT   = "Summer15_50nsV5_DATA"
    pfChargedCHSjetAna.mcGT     = "Summer15_50nsV5_MC"
    pfChargedCHSjetAna.dataGT   = "Summer15_50nsV5_DATA"

if forcedSplitFactor>0 or forcedFineSplitFactor>0:
    if forcedFineSplitFactor>0 and forcedSplitFactor!=1: raise RuntimeError, 'splitFactor must be 1 if setting fineSplitFactor'
    for c in selectedComponents:
        if forcedSplitFactor>0: c.splitFactor = forcedSplitFactor
        if forcedFineSplitFactor>0: c.fineSplitFactor = forcedFineSplitFactor

#trigMatchExample = cfg.Analyzer(
#    TriggerMatchAnalyzer, name="TriggerMatchAllObjects",
#    processName = 'PAT',
#    label = 'dummyTrigMatch',
#    unpackPathNames = True,
#    trgObjSelectors = [lambda ob: ob.pt()>20, lambda ob: abs(ob.eta())<2.5],
#    collToMatch = "selectedLeptons",
#    collMatchSelectors = [lambda lep,ob: abs(lep.pt()/ob.pt()-1)<0.5],
#    collMatchDRCut = 0.3,
#    univoqueMatching = True,
#    verbose = True
#)
#susyCoreSequence.append(trigMatchExample)

    
#-------- SEQUENCE -----------

sequence = cfg.Sequence(susyCoreSequence+[
        ttHJetTauAna,
        ttHEventAna,
        treeProducer,
    ])
preprocessor = None

if doMETpreprocessor:
    removeResiduals = False # residuals are set to 1 in MC, no need to remove them
    import tempfile
    # -------------------- Running pre-processor
    import subprocess
    jecDBFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_%s_%s.db'%('50nsV5' if is50ns else '25nsV2','DATA' if isData else 'MC')
    jecEra    = 'Summer15_%s_%s'%('50nsV5' if is50ns else '25nsV2', 'DATA'if isData else 'MC')
    tempfile.tempdir=os.environ['CMSSW_BASE']+'/tmp'
    tfile, tpath = tempfile.mkstemp(suffix='.py',prefix='MET_preproc_')
    os.close(tfile)
    preprocessorFile = tpath
    extraArgs=[]
    if isData:
      extraArgs.append('--isData')
      GT= '74X_dataRun2_v2'
    else:
      GT= '74X_mcRun2_startup_v2' if is50ns else '74X_mcRun2_asymptotic_v2'
    if removeResiduals: extraArgs.append('--removeResiduals')
    if doAK4PFCHSchargedJets: extraArgs.append('--addReclusterTrackJetsAK4')
    args = ['python',
      os.path.expandvars('$CMSSW_BASE/python/CMGTools/ObjectStudies/corMETMiniAOD_cfgCreator.py'),\
      '--GT='+GT,
      '--outputFile='+preprocessorFile,
      '--jecDBFile='+jecDBFile,
      '--jecEra='+jecEra
      ] + extraArgs
    #print "Making pre-processorfile:"
    #print " ".join(args)
    subprocess.call(args)
    from PhysicsTools.Heppy.utils.cmsswPreprocessor import CmsswPreprocessor
    preprocessor = CmsswPreprocessor(preprocessorFile,prefetch=True) # prefetching input file for preprocessor


#-------- HOW TO RUN -----------

test = getHeppyOption('test')
if test == '1':
    comp = DYJetsToLL_M50_50ns
    if not is50ns: raise RuntimeError, 'Incorrect is50ns configuration'
    comp.files = comp.files[:1]
    comp.splitFactor = 1
    if not getHeppyOption('single'):
        comp.fineSplitFactor = 4
    selectedComponents = [ comp ]
elif test == '125':
    comp = TTJets
    if is50ns: raise RuntimeError, 'Incorrect is50ns configuration'
    comp.files = comp.files[:1]
    comp.splitFactor = 1
    if not getHeppyOption('single'):
        comp.fineSplitFactor = 4
    selectedComponents = [ comp ]
elif test == '2':
    for comp in selectedComponents:
        comp.files = comp.files[:1]
        comp.splitFactor = 1
        comp.fineSplitFactor = 1
elif test == '3':
    for comp in selectedComponents:
        comp.files = comp.files[:1]
        comp.splitFactor = 1
        comp.fineSplitFactor = 4
elif test == '5':
    for comp in selectedComponents:
        comp.files = comp.files[:5]
        comp.splitFactor = 1
        comp.fineSplitFactor = 5
elif test == '74X-MC':
    what = getHeppyOption("sample")
    if what == "TTLep":
        selectedComponents = [ TTLep_pow ]
        comp = selectedComponents[0]
        comp.files = [ '/store/mc/RunIISpring15DR74/TTTo2L2Nu_13TeV-powheg/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/50000/0C1B984D-F408-E511-872E-0002C90B7F2E.root' ]
        if is50ns: raise RuntimeError, 'Incorrect is50ns configuration'
        tmpfil = os.path.expandvars("/tmp/$USER/0C1B984D-F408-E511-872E-0002C90B7F2E.root")
        if not os.path.exists(tmpfil):
            os.system("xrdcp root://eoscms//eos/cms%s %s" % (comp.files[0],tmpfil))
        comp.files = [ tmpfil ]
    elif what == "TT":
        ttHLepSkim.minLeptons = 0
        selectedComponents = [ TT_bx25 ]
        if is50ns: raise RuntimeError, 'Incorrect is50ns configuration'
    elif what == "Z":
        selectedComponents = [ ZEE_bx25, ZMM_bx25, ZTT_bx25 ]
        if is50ns: raise RuntimeError, 'Incorrect is50ns configuration'
    else:
        selectedComponents = RelVals740
        if is50ns: raise RuntimeError, 'Incorrect is50ns configuration'
    if not getHeppyOption("all"):
        for comp in selectedComponents:
            comp.files = comp.files[:1]
            comp.splitFactor = 1
            comp.fineSplitFactor = 1 if getHeppyOption("single") else 4
elif test == 'PromptReco':
    DoubleMuon = kreator.makeDataComponent("DoubleMuon_Run2015B_run251252",
                        "/DoubleMuon/Run2015B-PromptReco-v1/MINIAOD", 
                        "CMS", ".*root",
                        run_range = (251252,251252),
                        triggers = triggers_mumu_iso)
    DoubleEG = kreator.makeDataComponent("DoubleEG_Run2015B_run251252",
                        "/DoubleEG/Run2015B-PromptReco-v1/MINIAOD", 
                        "CMS", ".*root",
                        run_range = (251252,251252),
                        triggers = triggers_ee)
    selectedComponents = [ DoubleMuon, DoubleEG ]
    if not is50ns: raise RuntimeError, 'Incorrect is50ns configuration'
    for comp in selectedComponents:
        comp.splitFactor = 1
        if getHeppyOption("single"):
            selectedComponents = [ comp ]
            break
        else:
            comp.fineSplitFactor = 2
    if jsonAna in sequence: sequence.remove(jsonAna)
elif test == "express":

#    # beware of cmgdataset caching!!!
#    comp = kreator.makeDataComponent("ExpressRun2015D",
#                                     "/ExpressPhysics/Run2015D-Express-v3/FEVT",
#                                     "CMS", ".*root",
#                                     json=None,
#                                     run_range=(256673,256677),
#                                     triggers=triggers_ee_run1[:], vetoTriggers = [],
#                                     useAAA=False)
#    comp.splitFactor = 200
#    selectedComponents = [ comp ]

    comp = cfg.DataComponent( files = ["root://eoscms//store/express/Run2015D/ExpressPhysics/FEVT/Express-v3/000/256/675/00000/ACC501C8-E95C-E511-A432-02163E014147.root"], name="ExpressPhysics_2015D", intLumi=1 )
    comp.triggers = []
    comp.json     = None
    selectedComponents = [ comp ]
    ttHLepSkim.minLeptons = 0

    sequence.remove(jsonAna)
    if is50ns: raise RuntimeError, 'Incorrect is50ns configuration'
    # preprocessor cfg to be created with
    #    cmsDriver.py miniAOD-data -s PAT --data --runUnscheduled --eventcontent MINIAOD --conditions 74X_dataRun2_Prompt_v2 --no_exec
    #    sed -i 's/process.MINIAODoutput_step/process.endpath/' miniAOD-data_PAT.py
    from PhysicsTools.Heppy.utils.cmsswPreprocessor import CmsswPreprocessor
    preprocessor = CmsswPreprocessor("$CMSSW_BASE/src/CMGTools/TTHAnalysis/cfg/miniAOD-data_PAT.py",prefetch=True)


## output histogram
outputService=[]
from PhysicsTools.HeppyCore.framework.services.tfile import TFileService
output_service = cfg.Service(
    TFileService,
    'outputfile',
    name="outputfile",
    fname='treeProducerSusyMultilepton/tree.root',
    option='recreate'
    )    
outputService.append(output_service)

# the following is declared in case this cfg is used in input to the heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
from CMGTools.TTHAnalysis.tools.EOSEventsWithDownload import EOSEventsWithDownload
event_class = EOSEventsWithDownload if not preprocessor else Events
EOSEventsWithDownload.aggressive = 2 # always fetch if running on Wigner
if getHeppyOption("nofetch") or getHeppyOption("isCrab"):
    event_class = Events
    if preprocessor: preprocessor.prefetch = False
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = outputService, 
                     preprocessor = preprocessor, 
                     events_class = event_class)
