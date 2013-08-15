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


# Add the one-to-one correspondence between PFCandidates and Primary Vertices

from CMGTools.Common.PFVertexProducer_cfi import particleFlow
PATCMGVertexSequence = cms.Sequence ( particleFlow )


# PU SUB AND PARTICLES FOR ISO ---------------

from CommonTools.ParticleFlow.pfNoPileUp_cff import * 
from CommonTools.ParticleFlow.pfParticleSelection_cff import *

# note pfPileUp modified according to JetMET's recommendations
pfPileUp.checkClosestZVertex = False
pfPileUp.Vertices = 'goodOfflinePrimaryVertices'
pfPileUp.PFCandidates = 'particleFlow'
pfNoPileUp.bottomCollection = 'particleFlow'

from CommonTools.ParticleFlow.goodOfflinePrimaryVertices_cfi import goodOfflinePrimaryVertices
pfNoPileUpSequence.insert(0, goodOfflinePrimaryVertices)

PATCMGPileUpSubtractionSequence = cms.Sequence(
    pfNoPileUpSequence +
    pfParticleSelectionSequence
    )

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



# PHOTONS        ----------------------------

from CMGTools.Common.PAT.PATPhotons_cff import *

cmgPhoton.cfg.inputCollection = 'pfSelectedPhotons'

PATCMGPhotonSequence = cms.Sequence(
    PATPhotonSequence 
    +photonSequence
    )



# JETS NO PU CHS ----------------------------

from CMGTools.Common.PAT.PATJets_cff import *

# cmgPFJet.cfg.inputCollection = 'selectedPatJets'
# cmgPFBaseJet.cfg.inputCollection = 'selectedPatJets'

# Pile-up jet ID
# cmgPUJetMva.jets = 'selectedPatJets'

#patJetSource = 'selectedPatJets'
patJetSource = cms.InputTag('patJetsWithVar')
cmgPFJet.cfg.inputCollection = patJetSource

# leading jets for MET regression
from CMGTools.Common.factories.cmgBaseJet_cfi import cmgBaseJet
cmgPFBaseJetAll = cmgBaseJet.clone()
cmgPFBaseJetAll.cfg.inputCollection = patJetSource

from CMGTools.Common.skims.leadingCMGBaseJetSelector_cfi import leadingCMGBaseJetSelector
cmgPFBaseJetLead = leadingCMGBaseJetSelector.clone()
cmgPFBaseJetLead.inputCollection = 'cmgPFBaseJetAll'
cmgPFBaseJetLead.index = 4

PATCMGJetSequence = cms.Sequence(
    PATJetSequence + 
    jetSequence +
    cmgPFBaseJetAll + 
    cmgPFBaseJetLead
    )

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X, isNewerThan
from RecoJets.JetProducers.ak5PFJets_cfi import ak5PFJets

if cmsswIs44X():
    # ak5PFJets in 42X Fall11 MC samples are missing the jet area
    # add them to rho sequence, so they don't get duplicated for the CHS jets
    PATCMGRhoSequence += ak5PFJets


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

# 44X Does not work - in 44X case, importing nothing
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

# MET REGRESSION       ----------------------------

from CMGTools.Common.PAT.MetRegression_cff import *

PATCMGMetRegressionSequence = cms.Sequence(
    MetRegressionSequence
    )

####  FULL SEQUENCE  ####

# NOTE: object sequences are defined so that they can be easily removed from the path
# contrary to PAT layer-wise sequences

PATCMGSequence = cms.Sequence(
    PATCMGGenSequence +
    PATCMGTriggerSequence +
    PATCMGVertexSequence +
    PATCMGPileUpSubtractionSequence +
    PATCMGRhoSequence +
    PATCMGMuonSequence +
    PATCMGElectronSequence +
    PATCMGPhotonSequence +
    PATCMGJetSequence +
    PATCMGTauSequence +
    PATCMGMetSequence +
    MetSignificanceSequence +
    PATCMGMetRegressionSequence
    )

#if isNewerThan('CMSSW_5_2_0'):
#    PATCMGSequence += PATCMGJetSequenceCHSpruned
