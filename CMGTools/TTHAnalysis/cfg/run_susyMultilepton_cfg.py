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
runSMS = getHeppyOption("runSMS",False)
scaleProdToLumi = float(getHeppyOption("scaleProdToLumi",-1)) # produce rough equivalent of X /pb for MC datasets
SOS = getHeppyOption("SOS",False) ## switch True to overwrite settings for SOS skim (N.B. default settings are those from multilepton preselection)
saveSuperClusterVariables = getHeppyOption("saveSuperClusterVariables",False)
removeJetReCalibration = getHeppyOption("removeJetReCalibration",False)
doMETpreprocessor = getHeppyOption("doMETpreprocessor",False)
doT1METCorr = getHeppyOption("doT1METCorr",False)
old74XMiniAODs  = (getHeppyOption("old74XMiniAODs", not runData) not in (False,"False"))
noMETNoHF = getHeppyOption("noMETNoHF",False)
#doAK4PFCHSchargedJets = getHeppyOption("doAK4PFCHSchargedJets",False)
forcedSplitFactor = getHeppyOption("splitFactor",-1)
forcedFineSplitFactor = getHeppyOption("fineSplitFactor",-1)
isTest = getHeppyOption("test",None) != None and not re.match("^\d+$",getHeppyOption("test"))

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
lepAna.loose_electron_id = "POG_MVA_ID_Spring15_NonTrig_VLooseIdEmu"
isolation = "miniIso"

jetAna.copyJetsByValue = True # do not remove this
metAna.copyMETsByValue = True # do not remove this
jetAna.addJECShifts = True
susyCoreSequence.insert(susyCoreSequence.index(jetAna)+1, jetAnaScaleDown)
susyCoreSequence.insert(susyCoreSequence.index(jetAna)+1, jetAnaScaleUp)
susyCoreSequence.insert(susyCoreSequence.index(metAna)+1, metAnaScaleDown)
susyCoreSequence.insert(susyCoreSequence.index(metAna)+1, metAnaScaleUp)


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
    jetAnaScaleUp.minLepPt = 20 
    jetAnaScaleDown.minLepPt = 20 
    # otherwise with only absIso cut at 10 GeV and no relIso we risk cleaning away good jets

#isolation = "ptRel"
if isolation == "ptRel": 
    # delay isolation cut for leptons of pt > 10, for which we do pTrel recovery
    lepAna.loose_muon_isoCut     = lambda muon : muon.relIso03 < 0.5 or muon.pt() > 10
    lepAna.loose_electron_isoCut = lambda elec : elec.relIso03 < 0.5 or elec.pt() > 10
    # in the cleaning, keep the jet if the lepton fails relIso or ptRel
    raise RuntimeError, 'jetAnalyzer is being run multiple times, the following is not supported'
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
jetAnaScaleUp.doQG = True
jetAnaScaleDown.doQG = True

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
    jetAnaScaleUp.cleanJetsFromTaus = True
    jetAnaScaleDown.cleanJetsFromTaus = True


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
            NTupleVariable("miniIsoR", lambda x: getattr(x,'miniIsoR',-999), help="miniIso cone size"),
            NTupleVariable("effArea", lambda x: getattr(x,'EffectiveArea03',-999), help="effective area used for PU subtraction"),
            NTupleVariable("rhoForEA", lambda x: getattr(x,'rho',-999), help="rho used for EA PU subtraction")
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

susyMultilepton_globalObjects.update({
        "met_jecUp" : NTupleObject("met_jecUp", metType, help="PF E_{T}^{miss}, after type 1 corrections (JEC plus 1sigma)"),
        "met_jecDown" : NTupleObject("met_jecDown", metType, help="PF E_{T}^{miss}, after type 1 corrections (JEC minus 1sigma)"),
        })
susyMultilepton_collections.update({
            "cleanJets_jecUp"       : NTupleCollection("Jet_jecUp",     jetTypeSusyExtra, 15, help="Cental jets after full selection and cleaning, sorted by pt (JEC plus 1sigma)"),
            "cleanJets_jecDown"     : NTupleCollection("Jet_jecDown",     jetTypeSusyExtra, 15, help="Cental jets after full selection and cleaning, sorted by pt (JEC minus 1sigma)"),
            "discardedJets_jecUp"   : NTupleCollection("DiscJet_jecUp", jetTypeSusy, 15, help="Jets discarted in the jet-lepton cleaning (JEC +1sigma)"),
            "discardedJets_jecDown" : NTupleCollection("DiscJet_jecDown", jetTypeSusy, 15, help="Jets discarted in the jet-lepton cleaning (JEC -1sigma)"),
            })

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
if not runSMS:
    susyCoreSequence.insert(susyCoreSequence.index(skimAnalyzer),
                            susyCounter)
else:
    susyCounter.bypass_trackMass_check = False
    susyCoreSequence.insert(susyCoreSequence.index(susyScanAna)+1,susyCounter)

# HBHE new filter
from CMGTools.TTHAnalysis.analyzers.hbheAnalyzer import hbheAnalyzer
hbheAna = cfg.Analyzer(
    hbheAnalyzer, name="hbheAnalyzer", IgnoreTS4TS5ifJetInLowBVRegion=False
    )
if not runSMS:
    susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna),hbheAna)
    treeProducer.globalVariables.append(NTupleVariable("hbheFilterNew50ns", lambda ev: ev.hbheFilterNew50ns, int, help="new HBHE filter for 50 ns"))
    treeProducer.globalVariables.append(NTupleVariable("hbheFilterNew25ns", lambda ev: ev.hbheFilterNew25ns, int, help="new HBHE filter for 25 ns"))
    treeProducer.globalVariables.append(NTupleVariable("hbheFilterIso", lambda ev: ev.hbheFilterIso, int, help="HBHE iso-based noise filter"))

#additional MET quantities
metAna.doTkMet = True
metAnaScaleUp.doTkMet = True
metAnaScaleDown.doTkMet = True
treeProducer.globalVariables.append(NTupleVariable("met_trkPt", lambda ev : ev.tkMet.pt() if  hasattr(ev,'tkMet') else  0, help="tkmet p_{T}"))
treeProducer.globalVariables.append(NTupleVariable("met_trkPhi", lambda ev : ev.tkMet.phi() if  hasattr(ev,'tkMet') else  0, help="tkmet phi"))

# MET preprocessor and ak4PFchs charged-only jets
if doMETpreprocessor or (not noMETNoHF):
    susyCoreSequence.insert(susyCoreSequence.index(metAna)+1,metNoHFAna)
    treeProducer.globalObjects.update({"metNoHF"  : NTupleObject("metNoHF", metType, help="PF E_{T}^{miss}, after type 1 corrections (NoHF)")})
if doT1METCorr:
    if doMETpreprocessor: 
        print "WARNING: you're running the MET preprocessor and also Type1 MET corrections. This is probably not intended."
    jetAna.calculateType1METCorrection = True
    metAna.recalibrate = "type1"
    metAna.old74XMiniAODs = old74XMiniAODs
    jetAnaScaleUp.calculateType1METCorrection = True
    metAnaScaleUp.recalibrate = "type1"
    metAnaScaleUp.old74XMiniAODs = old74XMiniAODs
    jetAnaScaleDown.calculateType1METCorrection = True
    metAnaScaleDown.recalibrate = "type1"
    metAnaScaleDown.old74XMiniAODs = old74XMiniAODs

#if doAK4PFCHSchargedJets:
#    if not doMETpreprocessor: raise RuntimeError, "ak4PFchs charged-only jets are reclustered in the MET preprocessor, but this configuration is not going to run it"
#    treeProducer.collections["cleanJetsPFChargedCHS"] = NTupleCollection("JetPFChargedCHS", jetTypeSusyExtra, 15, help="Central PFChargedCHS jets after full selection and cleaning, sorted by pt") # ak4PFchs charged-only jets
#    susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna), pfChargedCHSjetAna)
#treeProducer.collections["jetsAllNoID"] = NTupleCollection("AllJet", jetTypeSusyExtra, 15, help="Central jets, sorted by pt") # warning, increases tree size considerably

#-------- SAMPLES AND TRIGGERS -----------


from CMGTools.RootTools.samples.triggers_13TeV_Spring15 import *
from CMGTools.RootTools.samples.triggers_8TeV import triggers_1mu_8TeV, triggers_mumu_8TeV, triggers_mue_8TeV, triggers_ee_8TeV;
#triggerFlagsAna.triggerBits = {
#    'DoubleMu' : triggers_mumu_iso,
#    'DoubleMuSS' : triggers_mumu_ss,
#    'DoubleMuNoIso' : triggers_mumu_noniso,
#    'DoubleEl' : triggers_ee,
#    'MuEG'     : triggers_mue,
#    'DoubleMuHT' : triggers_mumu_ht,
#    'DoubleElHT' : triggers_ee_ht,
#    'MuEGHT' : triggers_mue_ht,
#    'TripleEl' : triggers_3e,
#    'TripleMu' : triggers_3mu,
#    'TripleMuA' : triggers_3mu_alt,
#    'DoubleMuEl' : triggers_2mu1e,
#    'DoubleElMu' : triggers_2e1mu,
#    'SingleMu' : triggers_1mu_iso,
#    'SingleMu50ns' : triggers_1mu_iso_50ns,
#    'SingleEl'     : triggers_1e,
#    'SingleEl50ns' : triggers_1e_50ns,
#    'SingleMu_8TeV' : triggers_1mu_8TeV + triggers_1mu_iso_r,
#    'DoubleMu_8TeV' : triggers_mumu_8TeV + triggers_mumu_run1,
#    'MuEG_8TeV'     : triggers_mue_8TeV + triggers_mue_run1,
#    'DoubleEl_8TeV' : triggers_ee_8TeV + triggers_ee_run1,
#    'MonoJet80MET90' : triggers_Jet80MET90,
#    'MonoJet80MET120' : triggers_Jet80MET120,
#    'METMu5' : triggers_MET120Mu5,
#}
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
    'SingleEl'     : triggers_1e,
    'MonoJet80MET90' : triggers_Jet80MET90,
    'MonoJet80MET120' : triggers_Jet80MET120,
    'METMu5' : triggers_MET120Mu5,
}
triggerFlagsAna.unrollbits = True
triggerFlagsAna.saveIsUnprescaled = True
triggerFlagsAna.checkL1Prescale = True

if runSMS:
    susyCoreSequence.remove(triggerFlagsAna)
    susyCoreSequence.remove(triggerAna)
    susyCoreSequence.remove(eventFlagsAna)
    ttHLepSkim.requireSameSignPair = True

from CMGTools.RootTools.samples.samples_13TeV_RunIISpring15MiniAODv2 import *
from CMGTools.RootTools.samples.samples_13TeV_74X_susySignalsPriv import *
from CMGTools.RootTools.samples.samples_13TeV_signals import *
#from CMGTools.RootTools.samples.samples_8TeVReReco_74X import *
#from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import *

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

### ra5 prod
#selectedComponents = [TTJets, WJetsToLNu, DYJetsToLL_M10to50,  DYJetsToLL_M50, WZTo3LNu, ZZTo4L, WWTo2L2Nu, GGHZZ4L, TTHnobb, TTWToLNu, TTZToLLNuNu, TTLLJets_m1to10, TTGJets, T5ttttDeg_mGo1000_mStop300_mCh285_mChi280, T5qqqqWWDeg_mGo1000_mCh315_mChi300_dilep, WGToLNuG, ZGTo2LG, WWDouble, VHToNonbb, WpWpJJ] + SingleTop + TopPlusX + T1tttt + T6ttWW + T5qqqqWW + Rares
#selectedComponents = [WZTo3LNu, ZZTo4L, GGHZZ4L, TTHnobb, TTWToLNu, TTZToLLNuNu, TTLLJets_m1to10, TTGJets, T5ttttDeg_mGo1000_mStop300_mCh285_mChi280, T5qqqqWWDeg_mGo1000_mCh315_mChi300_dilep, WGToLNuG, ZGTo2LG, WWDouble, VHToNonbb, WpWpJJ] + T1tttt + T6ttWW + T5qqqqWW + Rares + TriBosons + TopPlusX

#selectedComponents1 = [TTJets_SingleLeptonFromTbar,TTJets_SingleLeptonFromTbar_ext,TTJets_SingleLeptonFromT,TTJets_SingleLeptonFromT_ext,TTJets_DiLepton,TTJets_DiLepton_ext] + WJetsToLNuHT + [DYJetsToLL_M10to50,DYJetsToLL_M50] # do not reproduce with v6
#selectedComponents2 = [T1tttt_mGo1200_mChi800,T1tttt_mGo1500_mChi100,T5qqqqWWDeg_mGo1000_mCh315_mChi300_dilep,T5qqqqWW_mGo1200_mCh1000_mChi800_dilep,T5ttttDeg_mGo1000_mStop300_mCh285_mChi280,T6ttWW_mSbot600_mCh425_mChi50,T6ttWW_mSbot650_mCh150_mChi50]
#selectedComponents3 = [TTWToLNu,TTZToLLNuNu,TTLLJets_m1to10,TTHnobb,WZTo3LNu,WpWpJJ,TTGJets,TGJets,WGToLNuG,WWDouble,VHToNonbb,WWZ,ZZZ,WZZ,TTTT]
#selectedComponents4 = [TToLeptons_tch_amcatnlo,TToLeptons_tch_amcatnlo_ext,TToLeptons_sch_amcatnlo,TBar_tWch,T_tWch,WWTo2L2Nu] # do not reproduce with v6
#selectedComponents5 = [TbarToLeptons_tch] # where is this??? # do not reproduce with v6
#selectedComponents6 = [tZq_ll,ZZTo4L,ZGTo2LG,GGHZZ4L] # split a lot

#for c in selectedComponents2: c.splitFactor = len(c.files)/3
#for c in selectedComponents3: c.splitFactor = len(c.files)/3
#for c in selectedComponents6: c.splitFactor = len(c.files)

#selectedComponents = selectedComponents2 + selectedComponents3 + selectedComponents6

#selectedComponents = SMS_miniAODv2_T1tttt
#susyCounter.SMS_varying_masses = ['genSusyMGluino','genSusyMNeutralino']

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


if runData and not isTest: # For running on data

#    # low-PU 50ns run (251721)
#    json = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-252126_13TeV_PromptReco_Collisions15_LOWPU_50ns_JSON.txt";
#    processing = "Run2015B-PromptReco-v1"; short = "Run2015B_v1"; run_ranges = [ (251721,251721) ]; useAAA=False; is50ns=True; triggerFlagsAna.checkL1Prescale = False;

#    # Run2015B, 50 ns, 3.8T + one run at 50 ns in Run2015C
#    json = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-255031_13TeV_PromptReco_Collisions15_50ns_JSON.txt";
#    processing = "Run2015B-17Jul2015-v1"; short = "Run2015B_17Jul2015"; run_ranges = [ (251244, 251562) ]; useAAA=False; is50ns=True; triggerFlagsAna.checkL1Prescale = False;
#    processing = "Run2015B-PromptReco-v1"; short = "Run2015B_v1"; run_ranges = [ (251643,251883) ]; useAAA=False; is50ns=True; triggerFlagsAna.checkL1Prescale = False;
#    json = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_254833_13TeV_PromptReco_Collisions15_JSON.txt"; # taken at 50 ns with 25 ns reconstruction
#    processing = "Run2015C-PromptReco-v1"; short = "Run2015C_v1"; run_ranges = [ (254833,254833) ]; useAAA=False; is50ns=True; triggerFlagsAna.checkL1Prescale = False;

#    normalize with: brilcalc lumi --normtag /afs/cern.ch/user/c/cmsbril/public/normtag_json/OfflineNormtagV1.json -i jsonfile.txt

    is50ns = False
    dataChunks = []

    # Oct05 rereco of Run2015C
    json = os.environ['CMSSW_BASE']+'/src/CMGTools/TTHAnalysis/data/json/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
    processing = "Run2015C_25ns-05Oct2015-v1"; short = "Run2015C_Oct05"; run_ranges = [ (254231,254914) ]; useAAA=False;
    if old74XMiniAODs: raise RuntimeError, 'Incorrect old74XMiniAODs configuration'
    dataChunks.append((json,processing,short,run_ranges,useAAA))

    # Oct05 rereco of Run2015D-PromptReco-v3 (up to run 258158)
    json = os.environ['CMSSW_BASE']+'/src/CMGTools/TTHAnalysis/data/json/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
    processing = "Run2015D-05Oct2015-v1"; short = "Run2015D_Oct05"; run_ranges = [ (256630,258158) ]; useAAA=False;
    if old74XMiniAODs: raise RuntimeError, 'Incorrect old74XMiniAODs configuration'
    dataChunks.append((json,processing,short,run_ranges,useAAA))

    # Run2015D PromptReco-v4 (258159-260627) - WARNING: beware of CACHING in .cmgdataset
    json = os.environ['CMSSW_BASE']+'/src/CMGTools/TTHAnalysis/data/json/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
    processing = "Run2015D-PromptReco-v4"; short = "Run2015D_PromptV4"; run_ranges = [ (258159,260627) ]; useAAA=False;
    if old74XMiniAODs: raise RuntimeError, 'Incorrect old74XMiniAODs configuration'
    dataChunks.append((json,processing,short,run_ranges,useAAA))

    compSelection = ""; compVeto = ""
    DatasetsAndTriggers = []
    selectedComponents = [];
 
    if SOS == True:
        DatasetsAndTriggers.append( ("MET", triggers_Jet80MET90 + triggers_Jet80MET120 + triggers_MET120Mu5 ) )
        #DatasetsAndTriggers.append( ("SingleMuon", triggers_1mu_iso + triggers_1mu_iso_50ns + triggers_1mu_noniso) )
    else:
        DatasetsAndTriggers.append( ("DoubleMuon", triggers_mumu_iso + triggers_mumu_ss + triggers_mumu_ht + triggers_3mu + triggers_3mu_alt) )
        DatasetsAndTriggers.append( ("DoubleEG",   triggers_ee + triggers_ee_ht + triggers_3e) )
        DatasetsAndTriggers.append( ("MuonEG",     triggers_mue + triggers_mue_ht + triggers_2mu1e + triggers_2e1mu) )
#        DatasetsAndTriggers.append( ("SingleMuon", triggers_1mu_iso + triggers_1mu_iso_50ns + triggers_1mu_noniso) )
#        DatasetsAndTriggers.append( ("SingleElectron", triggers_1e + triggers_1e_50ns) )

        if runDataQCD: # for fake rate measurements in data
            lepAna.loose_muon_dxy = 999
            lepAna.loose_electron_dxy = 999
            ttHLepSkim.minLeptons = 1
            FRTrigs = triggers_FR_1mu_iso + triggers_FR_1mu_noiso + triggers_FR_1e_noiso + triggers_FR_1e_iso
            for t in FRTrigs:
                tShort = t.replace("HLT_","FR_").replace("_v*","")
                triggerFlagsAna.triggerBits[tShort] = [ t ]
                FRTrigs_mu = triggers_FR_1mu_iso + triggers_FR_1mu_noiso
                FRTrigs_el = triggers_FR_1e_noiso + triggers_FR_1e_iso + triggers_FR_1e_b2g
                DatasetsAndTriggers = [ (pd,trig) for pd,trig in DatasetsAndTriggers if pd in ['DoubleMuon','DoubleEG'] ]
                for pd,trig in DatasetsAndTriggers:
                    if pd in ['DoubleMuon','SingleMuon']:
                        trig.extend(FRTrigs_mu)
                    elif pd in ['DoubleEG','SingleElectron']:
                        trig.extend(FRTrigs_el)
                    else:
                        print 'the strategy for trigger selection on MuonEG for FR studies should yet be implemented'
                        assert(False)

    for json,processing,short,run_ranges,useAAA in dataChunks:
        vetos = []
        for pd,triggers in DatasetsAndTriggers:
            for run_range in run_ranges:
                label = "runs_%d_%d" % run_range if run_range[0] != run_range[1] else "run_%d" % (run_range[0],)
                compname = pd+"_"+short+"_"+label
                if ((compSelection and not re.search(compSelection, compname)) or
                    (compVeto      and     re.search(compVeto,      compname))):
                        print "Will skip %s" % (compname)
                        continue
                myprocessing = processing
                if pd=="MuonEG" and ("Run2015D-05Oct2015" in processing): myprocessing = myprocessing.replace("05Oct2015-v1","05Oct2015-v2")
                comp = kreator.makeDataComponent(compname, 
                                                 "/"+pd+"/"+myprocessing+"/MINIAOD", 
                                                 "CMS", ".*root", 
                                                 json=json, 
                                                 run_range=run_range, 
                                                 triggers=triggers[:], vetoTriggers = vetos[:],
                                                 useAAA=useAAA)
                print "Will process %s (%d files)" % (comp.name, len(comp.files))
    #            print "\ttrigger sel %s, veto %s" % (triggers, vetos)
                comp.splitFactor = len(comp.files)/4
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
    jetAnaScaleUp.mcGT     = "Summer15_50nsV5_MC"
    jetAnaScaleUp.dataGT   = "Summer15_50nsV5_DATA"
    jetAnaScaleDown.mcGT     = "Summer15_50nsV5_MC"
    jetAnaScaleDown.dataGT   = "Summer15_50nsV5_DATA"
#    if doAK4PFCHSchargedJets:
#        pfChargedCHSjetAna.mcGT     = "Summer15_50nsV5_MC"
#        pfChargedCHSjetAna.dataGT   = "Summer15_50nsV5_DATA"

if runSMS:
    jetAna.mcGT = "MCRUN2_74_V9_FASTSIM_291115"
    jetAnaScaleUp.mcGT = "MCRUN2_74_V9_FASTSIM_291115"
    jetAnaScaleDown.mcGT = "MCRUN2_74_V9_FASTSIM_291115"
    jetAna.applyL2L3Residual = False
    jetAnaScaleUp.applyL2L3Residual = False
    jetAnaScaleDown.applyL2L3Residual = False

if removeJetReCalibration:
    ## NOTE: jets will still be recalibrated, since calculateSeparateCorrections is True,
    ##       however the code will check that the output 4-vector is unchanged.
    jetAna.recalibrateJets = False
    jetAnaScaleUp.recalibrateJets = False
    jetAnaScaleDown.recalibrateJets = False

if forcedSplitFactor>0 or forcedFineSplitFactor>0:
    if forcedFineSplitFactor>0 and forcedSplitFactor!=1: raise RuntimeError, 'splitFactor must be 1 if setting fineSplitFactor'
    for c in selectedComponents:
        if forcedSplitFactor>0: c.splitFactor = forcedSplitFactor
        if forcedFineSplitFactor>0: c.fineSplitFactor = forcedFineSplitFactor

#trigMatchExample = cfg.Analyzer(
#    TriggerMatchAnalyzer, name="TriggerMatchEle27",
#    processName = 'PAT',
#    label = 'Ele27_WP85_Gsf',
#    unpackPathNames = True,
#    trgObjSelectors = [lambda ob: ob.pt()>20, lambda ob: abs(ob.eta())<2.5, lambda ob: len( [t for t in ob.pathNames(True) if re.match("HLT_Ele27_WP85_Gsf_v",t)] )>0 ],
#    collToMatch = "selectedLeptons",
#    collMatchSelectors = [lambda lep,ob: abs(lep.pt()/ob.pt()-1)<0.5, lambda lep,ob: abs(lep.pdgId())==11],
#    collMatchDRCut = 0.3,
#    univoqueMatching = True,
#    verbose = False
#)
#susyCoreSequence.append(trigMatchExample)
#leptonTypeSusyExtra.addVariables([
#        NTupleVariable("matchedTrgObj_Ele27_WP85_Gsf_pt", lambda x: getattr(x,'matchedTrgObjEle27_WP85_Gsf').pt() if getattr(x,'matchedTrgObjEle27_WP85_Gsf',None) else -999, help="Electron trigger pt")
#])


#-------- SEQUENCE -----------

sequence = cfg.Sequence(susyCoreSequence+[
        ttHJetTauAna,
        ttHEventAna,
        treeProducer,
    ])
preprocessor = None

if doMETpreprocessor:
    raise RuntimeError, 'Probably outdated; check especially the uncertainty file (Uncertainty or UncertaintySources? ; is the right version used throughout the produced py file for CmsRun?)'
    removeResiduals = False # residuals are set to 1 in MC, no need to remove them
    import tempfile
    # -------------------- Running pre-processor
    import subprocess
    if is50ns: jectag = '50nsV5'
    else: jectag = '25nsV6' if runData else '25nsV6'
    jecDBFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_%s_%s.db'%(jectag,'DATA' if runData else 'MC')
    jecEra    = 'Summer15_%s_%s'%(jectag, 'DATA'if runData else 'MC')
    tempfile.tempdir=os.environ['CMSSW_BASE']+'/tmp'
    tfile, tpath = tempfile.mkstemp(suffix='.py',prefix='MET_preproc_')
    os.close(tfile)
    extraArgs=[]
    if runData:
      extraArgs.append('--isData')
      if is50ns: GT= '74X_dataRun2_v2'
      else: raise RuntimeError,'GT for 25ns data with 25nsV5 JEC is to be announced'
    else:
      GT= '74X_mcRun2_startup_v2' if is50ns else '74X_mcRun2_asymptotic_v2'
    if removeResiduals: extraArgs.append('--removeResiduals')
#    if doAK4PFCHSchargedJets: extraArgs.append('--addReclusterTrackJetsAK4')
    args = ['python',
      os.path.expandvars('$CMSSW_BASE/python/CMGTools/ObjectStudies/corMETMiniAOD_cfgCreator.py'),\
      '--GT='+GT,
      '--outputFile='+tpath,
      '--jecDBFile='+jecDBFile,
      '--jecEra='+jecEra
      ] + extraArgs
    #print "Making pre-processorfile:"
    #print " ".join(args)
    subprocess.call(args)
    staticname=tempfile.tempdir+"/MET_preproc_%s_%s_%s_%s.py"%(jecEra,GT,"nores" if removeResiduals else "","") #"AK4PFCHSchargedJets" if doAK4PFCHSchargedJets else "")
    import filecmp
    if os.path.isfile(staticname) and filecmp.cmp(tpath,staticname):
        os.system("rm %s"%tpath)
    else:
        os.system("mv %s %s"%(tpath,staticname))
    preprocessorFile=staticname
    print 'Using preprocessor file: %s'%preprocessorFile
    from PhysicsTools.Heppy.utils.cmsswPreprocessor import CmsswPreprocessor
    preprocessor = CmsswPreprocessor(preprocessorFile,prefetch=True) # prefetching input file for preprocessor


#-------- HOW TO RUN -----------

test = getHeppyOption('test')
if test == '1':
    comp = selectedComponents[0]
    comp.files = comp.files[:1]
    comp.splitFactor = 1
    comp.fineSplitFactor = 1
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
        if is50ns or runData or not old74XMiniAODs: raise RuntimeError, 'Incorrect is50ns or runData or old74XMiniAODs configuration'
        tmpfil = os.path.expandvars("/tmp/$USER/0C1B984D-F408-E511-872E-0002C90B7F2E.root")
        if not os.path.exists(tmpfil):
            os.system("xrdcp root://eoscms//eos/cms%s %s" % (comp.files[0],tmpfil))
        comp.files = [ tmpfil ]
    elif what == "TTLep_v2":
        selectedComponents = [ TTLep_pow ]
        comp = selectedComponents[0]
        comp.files = [ '/store/mc/RunIISpring15MiniAODv2/TTTo2L2Nu_13TeV-powheg/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/10000/725B5B34-C66D-E511-86AC-001E67248688.root' ]
        if is50ns or runData or old74XMiniAODs: raise RuntimeError, 'Incorrect is50ns or runData or old74XMiniAODs configuration'
        tmpfil = os.path.expandvars("/tmp/$USER/725B5B34-C66D-E511-86AC-001E67248688.root")
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
elif test == '74Xv2-MC':
    what = getHeppyOption("sample", "TTLep")
    #susyCounter.doLHE = False
    if what == "TTLep":
        selectedComponents = [ TTLep_pow ]
        comp = selectedComponents[0]
        comp.files = [ '/store/mc/RunIISpring15MiniAODv2/TTTo2L2Nu_13TeV-powheg/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/10000/004613BA-C46D-E511-9EB6-001E67248732.root' ]
        if is50ns: raise RuntimeError, 'Incorrect is50ns configuration'
        if old74XMiniAODs: raise RuntimeError, 'Incorrect old74XMiniAODs configuration'
        if doT1METCorr or doMETpreprocessor or noMETNoHF or not removeJetReCalibration: 
            print "WARNING: running some option which is not needed on miniAOD v2 out of the box" 
        tmpfil = os.path.expandvars("/tmp/$USER/004613BA-C46D-E511-9EB6-001E67248732.root")
        if not os.path.exists(tmpfil):
            os.system("xrdcp root://eoscms//eos/cms%s %s" % (comp.files[0],tmpfil))
        comp.files = [ tmpfil ]
    elif what == "TTJets":
        selectedComponents = [ TTJets_LO ]
        comp = selectedComponents[0]
        comp.files = [ '/store/mc/RunIISpring15MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/30000/001F4F14-786E-E511-804F-0025905A60FE.root' ]
        if is50ns: raise RuntimeError, 'Incorrect is50ns configuration'
        if old74XMiniAODs: raise RuntimeError, 'Incorrect old74XMiniAODs configuration'
        if doT1METCorr or doMETpreprocessor or noMETNoHF or not removeJetReCalibration: 
            print "WARNING: running some option which is not needed on miniAOD v2 out of the box" 
        tmpfil = os.path.expandvars("/tmp/$USER/001F4F14-786E-E511-804F-0025905A60FE.root")
        if not os.path.exists(tmpfil):
            os.system("xrdcp root://eoscms//eos/cms%s %s" % (comp.files[0],tmpfil))
        comp.files = [ tmpfil ]
    else: raise RuntimeError, "Unknown sample: "+what
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
                        triggers = [ "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*" ])
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
elif test == 'PromptRecoD':
    DoubleMuon = kreator.makeDataComponent("DoubleMuon_Run2015D_run260577",
                        "/DoubleMuon/Run2015D-PromptReco-v4/MINIAOD", 
                        "CMS", ".*root",
                        run_range = (260577,260577),
                        triggers = triggers_mumu_iso)
    DoubleEG = kreator.makeDataComponent("DoubleEG_Run2015D_run260577",
                        "/DoubleEG/Run2015D-PromptReco-v4/MINIAOD", 
                        "CMS", ".*root",
                        run_range = (260577,260577),
                        triggers = triggers_ee)
    selectedComponents = [ DoubleMuon, DoubleEG ]
    if old74XMiniAODs or doMETpreprocessor or is50ns or not runData: raise RuntimeError, 'Wrong settings'
    for comp in selectedComponents:
        comp.json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
        comp.splitFactor = 1
        if not getHeppyOption("full"):
            comp.files = comp.files[:1]
            comp.fineSplitFactor = 2
        else:
            comp.splitFactor = len(comp.files)
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
elif test == "prompt2015D":
    comp = cfg.DataComponent( files = ["root://eoscms.cern.ch//store/data/Run2015D/DoubleEG/MINIAOD/PromptReco-v3/000/256/676/00000/E6FC61D6-C55F-E511-88D2-02163E011BE0.root"], name="Prompt_2015D", intLumi=1 )
    comp.triggers = []
    comp.json     = None
    selectedComponents = [ comp ]
    ttHLepSkim.minLeptons = 0
    sequence.remove(jsonAna)
    if is50ns: raise RuntimeError, 'Incorrect is50ns configuration'
elif test == 'miniAODv2':
    comp = cfg.MCComponent( files = ["root://eoscms.cern.ch//store/mc/RunIISpring15MiniAODv2/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/00000/0014DC94-DC5C-E511-82FB-7845C4FC39F5.root"], name="TTJets_miniAODv2" )
    comp.triggers = []
    selectedComponents = [ comp ]
    ttHLepSkim.minLeptons = 0
    sequence.remove(jsonAna)
    if is50ns: raise RuntimeError, 'Incorrect is50ns configuration'
elif test == "ttH-sync":
#    TTHnobb_mWCutfix_ch0.files = ["/data/p/peruzzi/088378DB-3D24-E511-8B0E-20CF3027A589.root"]
    TTHnobb_mWCutfix_ch0.files = ["root://eoscms.cern.ch//store/mc/RunIISpring15DR74/ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_mWCutfix/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/30000/088378DB-3D24-E511-8B0E-20CF3027A589.root"]
    selectedComponents = [TTHnobb_mWCutfix_ch0]
    for comp in selectedComponents:
        comp.splitFactor=1
        comp.fineSplitFactor=30
    ttHLepSkim.minLeptons = 0
elif test == "ra5-sync-mc":
    comp = cfg.MCComponent( files = ["root://eoscms.cern.ch//store/mc/RunIISpring15DR74/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/30000/60087A61-9134-E511-B0C6-0025905B855E.root"], name="TTW_RA5_sync" )
    comp.triggers = []
    selectedComponents = [ comp ]
    sequence.remove(jsonAna)
    if is50ns or runData: raise RuntimeError, 'Wrong configuration'
elif test == "ra5-sync-mc-v2":
#    comp = cfg.MCComponent( files = ["/data1/p/peruzzi/3A2285C7-CA6D-E511-B68F-0CC47A13D09C.root"], name="TTWv2_RA5_sync" )
    comp = cfg.MCComponent( files = ["root://eoscms.cern.ch//store/mc/RunIISpring15MiniAODv2/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/10000/3A2285C7-CA6D-E511-B68F-0CC47A13D09C.root"], name="TTWv2_RA5_sync" )
    comp.triggers = []
    comp.splitFactor = 1
    comp.fineSplitFactor = 8
    selectedComponents = [ comp ]
    sequence.remove(jsonAna)
    if is50ns or runData: raise RuntimeError, 'Wrong configuration'
elif test == "ra5-sync-data":
    comp = cfg.DataComponent( files = [
    "root://eoscms.cern.ch//store/data/Run2015D/DoubleMuon/MINIAOD/PromptReco-v3/000/257/531/00000/B26DC190-8166-E511-AE66-02163E0133A7.root",
    "root://eoscms.cern.ch//store/data/Run2015D/DoubleMuon/MINIAOD/PromptReco-v3/000/257/599/00000/908F5705-8666-E511-8362-02163E01432A.root",
    "root://eoscms.cern.ch//store/data/Run2015D/DoubleMuon/MINIAOD/PromptReco-v3/000/256/941/00000/10AA8DF0-0062-E511-B01C-02163E0146ED.root",
    "root://eoscms.cern.ch//store/data/Run2015D/DoubleEG/MINIAOD/PromptReco-v3/000/257/531/00000/2CFA9A0A-0F66-E511-88FF-02163E013484.root",
    "root://eoscms.cern.ch//store/data/Run2015D/DoubleEG/MINIAOD/PromptReco-v3/000/257/599/00000/4EF0C7A4-8E66-E511-AF95-02163E011C53.root",
    "root://eoscms.cern.ch//store/data/Run2015D/DoubleEG/MINIAOD/PromptReco-v3/000/256/941/00000/5A9CB350-3462-E511-A04D-02163E014393.root",
    "root://eoscms.cern.ch//store/data/Run2015D/MuonEG/MINIAOD/PromptReco-v3/000/257/531/00000/D62608D5-FB65-E511-B031-02163E0145C1.root",
    "root://eoscms.cern.ch//store/data/Run2015D/MuonEG/MINIAOD/PromptReco-v3/000/257/599/00000/FCE8F3F5-6066-E511-8E56-02163E014781.root",
    "root://eoscms.cern.ch//store/data/Run2015D/MuonEG/MINIAOD/PromptReco-v3/000/256/941/00000/C4AEA503-0462-E511-B2AE-02163E012B0C.root",
            ], name="data_RA5_sync", intLumi=1 )
    comp.triggers = []
    comp.splitFactor = len(comp.files)
    comp.json = os.environ['CMSSW_BASE']+'/src/CMGTools/TTHAnalysis/data/json/Cert_246908-258750_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
    selectedComponents = [ comp ]
    if is50ns or (not runData): raise RuntimeError, 'Wrong configuration'
elif test != None:
    raise RuntimeError, "Unknown test %r" % test

## FAST mode: pre-skim using reco leptons, don't do accounting of LHE weights (slow)"
## Useful for large background samples with low skim efficiency
if getHeppyOption("fast"):
    susyCounter.doLHE = False
    from CMGTools.TTHAnalysis.analyzers.ttHFastLepSkimmer import ttHFastLepSkimmer
    fastSkim = cfg.Analyzer(
        ttHFastLepSkimmer, name="ttHFastLepSkimmer",
        muons = 'slimmedMuons', muCut = lambda mu : mu.pt() > 5 and mu.isLooseMuon(),
        electrons = 'slimmedElectrons', eleCut = lambda ele : ele.pt() > 7,
        minLeptons = 2, 
    )
    if jsonAna in sequence:
        sequence.insert(sequence.index(jsonAna)+1, fastSkim)
    else:
        sequence.insert(sequence.index(skimAnalyzer)+1, fastSkim)

## Auto-AAA
if not getHeppyOption("isCrab"):
    from CMGTools.Production import changeComponentAccessMode
    from CMGTools.Production.localityChecker import LocalityChecker
    tier2Checker = LocalityChecker("T2_CH_CERN", datasets="/*/*/MINIAOD*")
    for comp in selectedComponents:
        if not hasattr(comp,'dataset'): continue
        if not re.match("/[^/]+/[^/]+/MINIAOD(SIM)?", comp.dataset): continue
        if "/store/" not in comp.files[0]: continue
        if re.search("/store/(group|user|cmst3)/", comp.files[0]): continue
        if not tier2Checker.available(comp.dataset):
            print "Dataset %s is not available, will use AAA" % comp.dataset
            changeComponentAccessMode.convertComponent(comp, "root://cms-xrd-global.cern.ch/%s")

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
