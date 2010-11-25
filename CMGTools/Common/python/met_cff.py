import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgBaseMET_cfi import cmgBaseMET
from CMGTools.Common.histograms.baseJetLorentzVector_cfi import baseJetLorentzVector
from CMGTools.Common.histograms.baseMETHistograms_cfi import baseMETHistograms


# MHT from PFJets
cmgMHTPFJets = cmgBaseMET.clone()
cmgMHTPFJets.cfg.inputCollection = "cmgPFBaseJet" 
mhtPFJetsHistograms = baseMETHistograms.clone()
mhtPFJetsHistograms.inputCollection = 'cmgMHTPFJets'

# MHT from PFJets, pt threshold 30
cmgMHTPFJets30 = cmgMHTPFJets.clone()
cmgMHTPFJets30.cfg.ptThreshold = 30.0
mhtPFJets30Histograms = baseMETHistograms.clone()
mhtPFJets30Histograms.inputCollection = 'cmgMHTPFJets30'

# MHT from PFJets, pt threshold 50
cmgMHTPFJets50 = cmgMHTPFJets.clone()
cmgMHTPFJets50.cfg.ptThreshold = 50.0
mhtPFJets50Histograms = baseMETHistograms.clone()
mhtPFJets50Histograms.inputCollection = 'cmgMHTPFJets50'



# MHT from CaloJets
cmgMHTCaloJets = cmgBaseMET.clone()
cmgMHTCaloJets.cfg.inputCollection = "cmgCaloBaseJet" 
mhtCaloJetsHistograms = baseMETHistograms.clone()
mhtCaloJetsHistograms.inputCollection = 'cmgMHTCaloJets'

# MHT from CaloJets, pt threshold 30
cmgMHTCaloJets30 = cmgMHTCaloJets.clone()
cmgMHTCaloJets30.cfg.ptThreshold = 30.0
mhtCaloJets30Histograms = baseMETHistograms.clone()
mhtCaloJets30Histograms.inputCollection = 'cmgMHTCaloJets30'

# MHT from CaloJets, pt threshold 50
cmgMHTCaloJets50 = cmgMHTCaloJets.clone()
cmgMHTCaloJets50.cfg.ptThreshold = 50.0
mhtCaloJets50Histograms = baseMETHistograms.clone()
mhtCaloJets50Histograms.inputCollection = 'cmgMHTCaloJets50'



# MET from PFCandidates
cmgMETPFCandidates = cmgBaseMET.clone()
cmgMETPFCandidates.cfg.inputCollection = "particleFlow" 
pfMETHistograms = baseMETHistograms.clone()
pfMETHistograms.inputCollection = 'cmgMETPFCandidates'

# MET from PFCandidates, pt threshold 2 (to remove pile-up)
cmgMETPFCandidates2 = cmgMETPFCandidates.clone()
cmgMETPFCandidates2.cfg.ptThreshold = 2.0
pfMET2Histograms = baseMETHistograms.clone()
pfMET2Histograms.inputCollection = 'cmgMETPFCandidates2'



metHistograms = cms.Sequence(
    mhtPFJetsHistograms +
    mhtPFJets30Histograms +
    mhtPFJets50Histograms +
    mhtCaloJetsHistograms +
    mhtCaloJets30Histograms +
    mhtCaloJets50Histograms +
    pfMETHistograms +
    pfMET2Histograms
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
