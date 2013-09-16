import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgBaseCandMET_cfi import cmgBaseCandMET
from CMGTools.Common.factories.cmgBaseMET_cfi import cmgBaseMET
from CMGTools.Common.factories.cmgPFMETScaler_cfi import cmgPFMETType1Corrected
from CMGTools.Common.skims.cmgCandSel_cfi import cmgCandSel

#PFMET from pat::MET
cmgPFMET = cmgBaseMET.clone()
cmgPFMET.cfg.inputCollection = "patMETsAK5"

cmgPFMETSel = cmgCandSel.clone( src = 'cmgPFMET' )



# MHT from PFJets, pt threshold 30
cmgMHTPFJet30 = cmgBaseCandMET.clone()
cmgMHTPFJet30.cfg.inputCollection = "cmgPFBaseJet"
cmgMHTPFJet30.cfg.ptThreshold = 30.0
cmgMHTPFJet30Sel = cmgCandSel.clone( src = 'cmgMHTPFJet30' )

# Calo MET from pat::MET
cmgCaloMET = cmgBaseMET.clone()
cmgCaloMET.cfg.inputCollection = "patMETs"

# MHT from CaloJets, pt threshold 30
cmgMHTCaloJet30 = cmgBaseCandMET.clone()
cmgMHTCaloJet30.cfg.inputCollection = "cmgCaloBaseJet"
cmgMHTCaloJet30.cfg.ptThreshold = 30.0


# MET from PFCandidates
# cmgMETPFCandidates = cmgBaseCandMET.clone()
# cmgMETPFCandidates.cfg.inputCollection = cms.InputTag("particleFlow")

# MET from PFCandidates, pt threshold 2 (to remove pile-up)
# cmgMETPFCandidates2 = cmgMETPFCandidates.clone()
# cmgMETPFCandidates2.cfg.ptThreshold = 2.0


pfSimpleMetSequence = cms.Sequence(
    cmgPFMET *                             
    cmgPFMETSel*
    cmgPFMETType1Corrected
    )

pfMetSequence = cms.Sequence(
    pfSimpleMetSequence +                              
    # cmgMETPFCandidates +                          
    cmgMHTPFJet30 +
    cmgMHTPFJet30Sel
    )

caloMetSequence = cms.Sequence(
    cmgCaloMET + 
    cmgMHTCaloJet30 
    )

metSequence = cms.Sequence(
    pfMetSequence
    # + caloMetSequence
    ) 

