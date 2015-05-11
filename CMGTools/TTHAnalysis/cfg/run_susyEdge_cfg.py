import PhysicsTools.HeppyCore.framework.config as cfg


#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import *

# Comment this line if you want the diagnostic folders produced along with the output root file
# cfg.Analyzer.nosubdir = True 




###################################################################################
## Redefinition of the objects should come here
##------------------------------------------
## Redefine what I need
##------------------------------------------
# --- LEPTON SKIMMING ---
ttHLepSkim.minLeptons = 2
ttHLepSkim.maxLeptons = 999
#ttHLepSkim.idCut  = ""
#ttHLepSkim.ptCuts = []

#Vertex
vertexAna.keepFailingEvents = True # keep events with no good vertices


############################################
## LEPTONS
############################################
## lepAna.loose_muon_id     = ""
lepAna.loose_muon_dxy    = 0.5
lepAna.loose_muon_dz     = 1.0
lepAna.loose_muon_relIso = 0.5

#lepAna.loose_electron_id           = "POG_PHYS14_25ns_v1_Loose"
lepAna.loose_electron_id           = "POG_Cuts_ID_PHYS14_25ns_v1_Loose"
lepAna.loose_electron_pt           = 5
lepAna.loose_electron_eta          = 2.4
lepAna.loose_electron_dxy          = 0.04
lepAna.loose_electron_dz           = 0.2
lepAna.loose_electron_relIso       = 0.15
lepAna.loose_electron_lostHits     = 999 # no cut
lepAna.inclusive_electron_lostHits = 999 # no cut

lepAna.mu_isoCorr = "rhoArea"
lepAna.ele_isoCorr = "rhoArea"
#lepAna.ele_tightId = "Cuts_2012"
lepAna.notCleaningElectrons = True

# run miniIso
lepAna.doMiniIsolation = True
lepAna.packedCandidates = 'packedPFCandidates'
lepAna.miniIsolationPUCorr = 'rhoArea'
lepAna.miniIsolationVetoLeptons = None # use 'inclusive' to veto inclusive leptons and their footprint in all isolation cones

# Event Analyzer for susy multi-lepton (at the moment, it's the TTH one)
from CMGTools.TTHAnalysis.analyzers.ttHLepEventAnalyzer import ttHLepEventAnalyzer
ttHEventAna = cfg.Analyzer(
    ttHLepEventAnalyzer, name="ttHLepEventAnalyzer",
    minJets25 = 0,
    )

############################################
## JETS    
############################################
# JET (for event variables do apply the jetID and not PUID yet)
jetAna.relaxJetId = False
jetAna.doPuId = False
jetAna.jetEta = 5.2
jetAna.jetEtaCentral = 2.5
jetAna.jetPt = 10.
jetAna.recalibrateJets = True
jetAna.jetLepDR = 0.4
jetAna.smearJets = False
jetAna.jetGammaDR = 0.4
jetAna.minGammaPt = 20
jetAna.gammaEtaCentral = 2.4
jetAna.cleanJetsFromFirstPhoton = True
jetAna.cleanJetsFromIsoTracks = True ## added for Dominick

############################################
# TAUS
############################################
tauAna.etaMax = 2.3
tauAna.dxyMax = 99999.
tauAna.dzMax = 99999.
tauAna.vetoLeptons = False
tauAna.vetoLeptonsPOG = True
tauAna.decayModeID = "" # ignored if not set or ""
tauAna.tauAntiMuonID = "againstMuonTight"
tauAna.tauAntiElectronID = "againstElectronLoose"

##==== tau jet analyzer, to be called (for the moment) once bjetsMedium are produced
from CMGTools.TTHAnalysis.analyzers.ttHJetTauAnalyzer import ttHJetTauAnalyzer
ttHJetTauAna = cfg.Analyzer(
    ttHJetTauAnalyzer, name="ttHJetTauAnalyzer",
    )


# Photon
photonAna.etaCentral = 2.5
photonAna.gammaID = "PhotonCutBasedIDLoose_CSA14"

# MET analyzer
metAna.recalibrate = False

# store all taus by default
genAna.allGenTaus = True

##------------------------------------------ 
##  JZB specific VARIABLES: jzb, pt1, pt2, phi1, phi2, eta1, eta2, mll, index1, index2
##------------------------------------------ 
from CMGTools.TTHAnalysis.analyzers.ttHJZBTopologicalVars import ttHJZBTopologicalVars

ttHJZBTopologicalVars = cfg.Analyzer(
            ttHJZBTopologicalVars, name = 'ttHJZBTopologicalVars'
            )

## Insert the SV analyzer in the sequence
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



##------------------------------------------
##  PRODUCER
##------------------------------------------
###Notice that at this point we are only using the double lepton triggers. We might need the HT for trigger efficiency calculation
from CMGTools.TTHAnalysis.samples.triggers_13TeV_PHYS14 import triggers_mumu_iso, triggers_ee, triggers_mue 


triggerFlagsAna.triggerBits = {
            'DoubleMu' : triggers_mumu_iso,
            'DoubleEl' : triggers_ee,
            'MuEG'     : triggers_mue,
}

#-------- SEQUENCE

from CMGTools.TTHAnalysis.analyzers.treeProducerSusyEdge import * 

treeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='treeProducerSusyEdge',
##     AutoFillTreeProducer, name='treeProducerSusyCore',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     PDFWeights = PDFWeights,
     globalVariables = susyJZBEdge_globalVariables,
     globalObjects = susyJZBEdge_globalObjects,
     collections = susyJZBEdge_collections,
     defaultFloatType = 'F',
)

#Do we need this?
# inserts the counter histogram i think
susyCoreSequence.insert(susyCoreSequence.index(skimAnalyzer), susyCounter)

sequence = cfg.Sequence(
    susyCoreSequence+[
    ttHJetTauAna,
    ttHEventAna,
    ttHJZBTopologicalVars,
    treeProducer,
    ])

###---- to switch off the compression
#treeProducer.isCompressed = 0


#-------- SEQUENCE
from CMGTools.TTHAnalysis.samples.samples_13TeV_PHYS14 import *
from CMGTools.TTHAnalysis.samples.samples_13TeV_CSA14v2 import SingleMu

selectedComponents = TTJets


## options

from PhysicsTools.HeppyCore.framework.heppy import getHeppyOption
test = getHeppyOption('test')
if test == '1':
    comp = TTJets
    if getHeppyOption('TTJets'):
        # marccomp = SMS_T1tttt_2J_mGl1500_mLSP100
        comp = TTJets
    comp.files = comp.files[:1]
    comp.splitFactor = 1
    if not getHeppyOption('single'):
        comp.fineSplitFactor = 4
    selectedComponents = [ comp ]

from PhysicsTools.HeppyCore.framework.services.tfile import TFileService 
output_service = cfg.Service(
      TFileService,
      'outputfile',
      name="outputfile",
      fname='jzb.root',
      option='recreate'
    )




# the following is declared in case this cfg is used in input to the heppy.py script                                                                                                                   
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [output_service],
                     events_class = Events)

#printComps(config.components, True)
        
