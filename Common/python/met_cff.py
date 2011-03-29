import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgBaseCandMET_cfi import cmgBaseCandMET
from CMGTools.Common.factories.cmgBaseMET_cfi import cmgBaseMET
from CMGTools.Common.skims.cmgCandSel_cfi import cmgCandSel

#PFMET from pat::MET
cmgPFMET = cmgBaseMET.clone()
cmgPFMET.cfg.inputCollection = "patMETsPFlow"

cmgPFMETSel = cmgCandSel.clone( src = 'cmgPFMET' )



# MHT from PFJets, pt threshold 30
cmgMHTPFJets30 = cmgBaseCandMET.clone()
cmgMHTPFJets30.cfg.inputCollection = "cmgPFBaseJet"
cmgMHTPFJets30.cfg.ptThreshold = 30.0
cmgMHTPFJets30Sel = cmgCandSel.clone( src = 'cmgMHTPFJets30' )


# Calo MET from pat::MET
cmgCaloMET = cmgBaseMET.clone()
cmgCaloMET.cfg.inputCollection = "patMETs"

# MHT from CaloJets, pt threshold 30
cmgMHTCaloJets30 = cmgBaseCandMET.clone()
cmgMHTCaloJets30.cfg.inputCollection = "cmgCaloBaseJet"
cmgMHTCaloJets30.cfg.ptThreshold = 30.0


# MET from PFCandidates
cmgMETPFCandidates = cmgBaseCandMET.clone()
cmgMETPFCandidates.cfg.inputCollection = "particleFlow" 

# MET from PFCandidates, pt threshold 2 (to remove pile-up)
cmgMETPFCandidates2 = cmgMETPFCandidates.clone()
cmgMETPFCandidates2.cfg.ptThreshold = 2.0

# FOR EVENT SELECTION:
pfSimpleMetSequence = cms.Sequence(
    cmgPFMET +                             
    cmgPFMETSel
    )

pfMetSequence = cms.Sequence(
    pfSimpleMetSequence +                              
    cmgMETPFCandidates +                          
    cmgMHTPFJets30 +
    cmgMHTPFJets30Sel
    )

caloMetSequence = cms.Sequence(
    cmgCaloMET + 
    cmgMHTCaloJets30 
    )

metSequence = cms.Sequence(
    pfMetSequence +
    caloMetSequence
    ) 

