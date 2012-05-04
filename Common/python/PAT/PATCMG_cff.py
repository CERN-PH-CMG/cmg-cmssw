import FWCore.ParameterSet.Config as cms


from CMGTools.Common.analysis_cff import *

# FIXME : corrrect vetoes for PU iso electrons
# FIXME : include type1 met corrections
# FIXME : check the taus in 44X and 52X
# FIXME : adapt pile-up jet id to 44X


# GEN              ---------------------------

from CMGTools.Common.PAT.PATGenJet_cff import * 

from CMGTools.Common.gen_cff import *

PATCMGGenSequence = cms.Sequence(
    PATGenJetSequence + 
    genSequence
    )
    

# TRIGGER          ---------------------------

from CMGTools.Common.PAT.PATTrigger_cff import * 

PATCMGTriggerSequence = cms.Sequence(
    PATTriggerSequence +
    triggerSequence
    ) 

# PU SUB AND PARTICLES FOR ISO ---------------

from CommonTools.ParticleFlow.pfNoPileUp_cff import * 
from CommonTools.ParticleFlow.pfParticleSelection_cff import * 

# note pfPileUp modified according to JetMET's recommendations
pfPileUp.checkClosestZVertex = False
pfPileUp.Vertices = 'goodOfflinePrimaryVertices'
from CommonTools.ParticleFlow.goodOfflinePrimaryVertices_cfi import goodOfflinePrimaryVertices
pfNoPileUpSequence.insert(0, goodOfflinePrimaryVertices)

PATCMGPileUpSubtractionSequence = cms.Sequence(
    pfNoPileUpSequence +
    pfParticleSelectionSequence 
    )

# EVENT CLEANING   ----------------------------

from CMGTools.Common.eventCleaning.eventCleaning_cff import *
trackingFailureFilterCMG.JetSource = 'ak5PFJets'

# RHO's            ----------------------------

from CMGTools.Common.PAT.rho_cff import *
PATCMGRhoSequence = rhoSequence


# MUONS           ----------------------------

from CMGTools.Common.PAT.PATMuons_cff import *

cmgMuon.cfg.inputCollection = 'patMuonsWithTrigger'

PATCMGMuonSequence = cms.Sequence(
    PATMuonSequence + 
    muonSequence
    )


# ELECTRONS      ----------------------------

from CMGTools.Common.PAT.PATElectrons_cff import *

cmgElectron.cfg.inputCollection = 'patElectronsWithTrigger'

PATCMGElectronSequence = cms.Sequence(
    PATElectronSequence + 
    electronSequence
    )



# JETS NO PU CHS ----------------------------

from CMGTools.Common.PAT.PATJets_cff import *

cmgPFJet.cfg.inputCollection = 'selectedPatJets'
cmgPFBaseJet.cfg.inputCollection = 'selectedPatJets'

# Pile-up jet ID
cmgPUJetMva.jets = 'selectedPatJets'

PATCMGJetSequence = cms.Sequence(
    PATJetSequence + 
    jetSequence
    )


# TAUS NO PU CHS  ----------------------------

from CMGTools.Common.PAT.PATTaus_cff import *

cmgTau.cfg.inputCollection = 'selectedPatTaus'

PATCMGTauSequence = cms.Sequence(
    PATTauSequence + 
    tauSequence
    )


# JETS CHS ----------------------------

from CMGTools.Common.PAT.PATJetsCHS_cff import *


# JETS CHS PRUNED ----------------------------

# 44X Does not work 
from CMGTools.Common.PAT.jetSubstructure_cff import *

# MET      ----------------------------

from CMGTools.Common.PAT.PATMet_cff import *

cmgPFMET.cfg.inputCollection = 'patMETs'
cmgPFMETRaw = cmgPFMET.clone()
cmgPFMETRaw.cfg.inputCollection = 'patMETsRaw'

PATCMGMetSequence = cms.Sequence(
    PATMetSequence + 
    cmgPFMET +
    cmgPFMETRaw 
    )

# MET  SIGNIFICANCE    ----------------------------

from CMGTools.Common.PAT.MetSignificance_cff import *

# COUNTERS  ----------------------------
prePathCounter = cms.EDProducer("EventCountProducer")
postPathCounter = cms.EDProducer("EventCountProducer")


####  FULL SEQUENCE  ####

# NOTE: object sequences are defined so that they can be easily removed from the path
# contrary to PAT layer-wise sequences

PATCMGSequence = cms.Sequence(
    PATCMGGenSequence +
    PATCMGTriggerSequence +
    eventCleaningSequence + 
    PATCMGPileUpSubtractionSequence +
    PATCMGRhoSequence +
    PATCMGMuonSequence +
    PATCMGElectronSequence +
    PATCMGJetSequence +
    PATCMGTauSequence +
    PATCMGMetSequence +
    MetSignificanceSequence
    )

