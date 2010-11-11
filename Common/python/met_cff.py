import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgBaseMET_cfi import cmgBaseMET
from CMGTools.Common.histograms.baseJetLorentzVector_cfi import baseJetLorentzVector


# MHT from PFJets
cmgMHTPFJets = cmgBaseMET.clone()
cmgMHTPFJets.cfg.inputCollection = "selectedPatJetsPFlow" 
mhtPFJetsLorentzVector = baseJetLorentzVector.clone()
mhtPFJetsLorentzVector.inputCollection = 'cmgMHTPFJets'

# MHT from CaloJets
cmgMHTCaloJets = cmgBaseMET.clone()
cmgMHTCaloJets.cfg.inputCollection = "selectedPatJets" 
mhtCaloJetsLorentzVector = baseJetLorentzVector.clone()
mhtCaloJetsLorentzVector.inputCollection = 'cmgMHTCaloJets'

# MET from PFCandidates
cmgMETPFCandidates = cmgBaseMET.clone()
cmgMETPFCandidates.cfg.inputCollection = "particleFlow" 
pfMETLorentzVector = baseJetLorentzVector.clone()
pfMETLorentzVector.inputCollection = 'cmgMETPFCandidates'

metHistograms = cms.Sequence(
    mhtPFJetsLorentzVector +
    mhtCaloJetsLorentzVector +
    pfMETLorentzVector 
    )

metSequence = cms.Sequence(
    cmgMHTPFJets +
    cmgMHTCaloJets +    
    cmgMETPFCandidates +
    metHistograms
    )
