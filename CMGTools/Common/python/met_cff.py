import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgBaseMET_cfi import cmgBaseMET
from CMGTools.Common.histograms.baseJetLorentzVector_cfi import baseJetLorentzVector


# MHT from PFJets
cmgMHTPFJets = cmgBaseMET.clone()
cmgMHTPFJets.cfg.inputCollection = "cmgPFBaseJet" 
mhtPFJetsLorentzVector = baseJetLorentzVector.clone()
mhtPFJetsLorentzVector.inputCollection = 'cmgMHTPFJets'

# MHT from PFJets, pt threshold 30
cmgMHTPFJets30 = cmgMHTPFJets.clone()
cmgMHTPFJets30.cfg.ptThreshold = 30.0
mhtPFJets30LorentzVector = baseJetLorentzVector.clone()
mhtPFJets30LorentzVector.inputCollection = 'cmgMHTPFJets30'

# MHT from PFJets, pt threshold 50
cmgMHTPFJets50 = cmgMHTPFJets.clone()
cmgMHTPFJets50.cfg.ptThreshold = 50.0
mhtPFJets50LorentzVector = baseJetLorentzVector.clone()
mhtPFJets50LorentzVector.inputCollection = 'cmgMHTPFJets50'



# MHT from CaloJets
cmgMHTCaloJets = cmgBaseMET.clone()
cmgMHTCaloJets.cfg.inputCollection = "cmgCaloBaseJet" 
mhtCaloJetsLorentzVector = baseJetLorentzVector.clone()
mhtCaloJetsLorentzVector.inputCollection = 'cmgMHTCaloJets'

# MHT from CaloJets, pt threshold 30
cmgMHTCaloJets30 = cmgMHTCaloJets.clone()
cmgMHTCaloJets30.cfg.ptThreshold = 30.0
mhtCaloJets30LorentzVector = baseJetLorentzVector.clone()
mhtCaloJets30LorentzVector.inputCollection = 'cmgMHTCaloJets30'

# MHT from CaloJets, pt threshold 50
cmgMHTCaloJets50 = cmgMHTCaloJets.clone()
cmgMHTCaloJets50.cfg.ptThreshold = 50.0
mhtCaloJets50LorentzVector = baseJetLorentzVector.clone()
mhtCaloJets50LorentzVector.inputCollection = 'cmgMHTCaloJets50'



# MET from PFCandidates
cmgMETPFCandidates = cmgBaseMET.clone()
cmgMETPFCandidates.cfg.inputCollection = "particleFlow" 
pfMETLorentzVector = baseJetLorentzVector.clone()
pfMETLorentzVector.inputCollection = 'cmgMETPFCandidates'

# MET from PFCandidates, pt threshold 2 (to remove pile-up)
cmgMETPFCandidates2 = cmgMETPFCandidates.clone()
cmgMETPFCandidates2.cfg.ptThreshold = 2.0
pfMET2LorentzVector = baseJetLorentzVector.clone()
pfMET2LorentzVector.inputCollection = 'cmgMETPFCandidates2'



metHistograms = cms.Sequence(
    mhtPFJetsLorentzVector +
    mhtPFJets30LorentzVector +
    mhtPFJets50LorentzVector +
    mhtCaloJetsLorentzVector +
    mhtCaloJets30LorentzVector +
    mhtCaloJets50LorentzVector +
    pfMETLorentzVector +
    pfMET2LorentzVector
    )

metSequence = cms.Sequence(
    cmgMHTPFJets +
    cmgMHTPFJets30 +
    cmgMHTPFJets50 +
    cmgMHTCaloJets +    
    cmgMHTCaloJets30 +    
    cmgMHTCaloJets50 +    
    cmgMETPFCandidates +
    cmgMETPFCandidates2 +
    metHistograms
    )
