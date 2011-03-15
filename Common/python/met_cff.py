import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgBaseCandMET_cfi import cmgBaseCandMET
from CMGTools.Common.factories.cmgBaseMET_cfi import cmgBaseMET
from CMGTools.Common.skims.cmgPFMETSel_cfi import cmgPFMETSel

#PFMET from pat::MET
cmgPFMET = cmgBaseMET.clone()
cmgPFMET.cfg.inputCollection = "patMETsPFlow"

# MHT from PFJets
cmgMHTPFJets = cmgBaseCandMET.clone()
cmgMHTPFJets.cfg.inputCollection = "cmgPFBaseJet" 

# MHT from PFJets, pt threshold 30
cmgMHTPFJets30 = cmgMHTPFJets.clone()
cmgMHTPFJets30.cfg.ptThreshold = 30.0

# MHT from PFJets, pt threshold 50
# cmgMHTPFJets50 = cmgMHTPFJets.clone()
# cmgMHTPFJets50.cfg.ptThreshold = 50.0

# MHT from CaloJets
cmgMHTCaloJets = cmgBaseCandMET.clone()
cmgMHTCaloJets.cfg.inputCollection = "cmgCaloBaseJet" 

# MHT from CaloJets, pt threshold 30
cmgMHTCaloJets30 = cmgMHTCaloJets.clone()
cmgMHTCaloJets30.cfg.ptThreshold = 30.0

# MHT from CaloJets, pt threshold 50
# cmgMHTCaloJets50 = cmgMHTCaloJets.clone()
# cmgMHTCaloJets50.cfg.ptThreshold = 50.0

# MET from PFCandidates
cmgMETPFCandidates = cmgBaseCandMET.clone()
cmgMETPFCandidates.cfg.inputCollection = "particleFlow" 

# MET from PFCandidates, pt threshold 2 (to remove pile-up)
cmgMETPFCandidates2 = cmgMETPFCandidates.clone()
cmgMETPFCandidates2.cfg.ptThreshold = 2.0

# FOR EVENT SELECTION:

pfMetSequence = cms.Sequence(
    cmgPFMET +                             
    cmgMETPFCandidates +                          
    cmgMHTPFJets30 +
    # cmgMHTPFJets50 +
    cmgPFMETSel
    )

caloMetSequence = cms.Sequence(
    cmgMHTCaloJets30 
    # cmgMHTCaloJets50 
    )

metSequence = cms.Sequence(
    pfMetSequence +
    caloMetSequence
    ) 

