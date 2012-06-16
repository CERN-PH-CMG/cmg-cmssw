import FWCore.ParameterSet.Config as cms


# Energy scale corrections and MC smearing
from EgammaCalibratedGsfElectrons.CalibratedElectronProducers.calibratedGsfElectrons_cfi import calibratedGsfElectrons as gsfElectrons
gsfElectrons.updateEnergyError = cms.bool(True)
gsfElectrons.isAOD = cms.bool(True)
RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    gsfElectrons = cms.PSet(
        initialSeed = cms.untracked.uint32(1),
        engineName = cms.untracked.string('TRandom3')
    ),
)

# prepare reco information
# from PhysicsTools.PatAlgos.recoLayer0.electronId_cff import *
from PhysicsTools.PatAlgos.recoLayer0.electronIsolation_cff import *

# add PAT specifics
from PhysicsTools.PatAlgos.mcMatchLayer0.electronMatch_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.electronProducer_cfi import *

makePatElectrons = cms.Sequence(
    # reco pre-production
    # patElectronId *
    # patElectronIsolation *
    # pat specifics
    electronMatch *
    # object production
    patElectrons
    )


from PhysicsTools.PatAlgos.selectionLayer1.electronSelector_cfi import selectedPatElectrons

# from PhysicsTools.PatAlgos.patSequences_cff import *

from CMGTools.Common.PAT.patLeptModifiedIsoDeposit_cff import *

# NOTE dunno why this is needed, but necessary to run on V5 PFAOD
# - Taejeong?
patElectrons.pfElectronSource = 'particleFlow'

# PF isolation

from CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff import *

# 44X need to remove spurious sequences from the path
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X
if cmsswIs44X():
    pfElectronIsolationSequence.remove( pfElectronIsoDepositsSequence )
    pfElectronIsolationSequence.remove( pfElectronIsolationFromDepositsSequence )

sourceElectrons = 'gsfElectrons'

elPFIsoDepositCharged.src = sourceElectrons
elPFIsoDepositChargedAll.src = sourceElectrons
elPFIsoDepositNeutral.src = sourceElectrons
elPFIsoDepositGamma.src = sourceElectrons
elPFIsoDepositPU.src = sourceElectrons

patElectrons.isoDeposits = cms.PSet(
    pfChargedHadrons = cms.InputTag("elPFIsoDepositCharged" ),
    pfChargedAll = cms.InputTag("elPFIsoDepositChargedAll" ),
    pfPUChargedHadrons = cms.InputTag("elPFIsoDepositPU" ),
    pfNeutralHadrons = cms.InputTag("elPFIsoDepositNeutral" ),
    pfPhotons = cms.InputTag("elPFIsoDepositGamma" ),
    tracker = cms.InputTag("eleIsoDepositTk")
    )

electronUserIsolation  = cms.PSet(
    user = cms.VPSet(
    cms.PSet( src = cms.InputTag("eleIsoFromDepsTkOptimized5") ),
    cms.PSet( src = cms.InputTag("eleIsoFromDepsTkOptimized7") ),
    )         
)

patElectrons.isolationValues = cms.PSet(
    pfChargedHadrons = cms.InputTag("elPFIsoValueCharged04PFId"),
    pfChargedAll = cms.InputTag("elPFIsoValueChargedAll04PFId"),
    pfPUChargedHadrons = cms.InputTag("elPFIsoValuePU04PFId" ),
    pfNeutralHadrons = cms.InputTag("elPFIsoValueNeutral04PFId" ),
    pfPhotons = cms.InputTag("elPFIsoValueGamma04PFId" ),
    )

patElectrons.userIsolation.user = electronUserIsolation.user

#NOTE the following should not be used for now, but we keep them just in case.
patElectrons.isolationValuesNoPFId = cms.PSet(
    pfChargedHadrons = cms.InputTag("elPFIsoValueCharged04NoPFId"),
    pfChargedAll = cms.InputTag("elPFIsoValueChargedAll04NoPFId"),
    pfPUChargedHadrons = cms.InputTag("elPFIsoValuePU04NoPFId" ),
    pfNeutralHadrons = cms.InputTag("elPFIsoValueNeutral04NoPFId" ),
    pfPhotons = cms.InputTag("elPFIsoValueGamma04NoPFId" )
    )

# identification

from CMGTools.Common.PAT.patElectronID_cff import *
patElectrons.addElectronID = True
from CMGTools.Common.PAT.electronIDs_cfi import electronIDs
patElectrons.electronIDSources  = electronIDs.clone()

patElectrons.embedTrack = True

selectedPatElectrons.cut = 'pt()>5'

from CMGTools.Common.PAT.patElectronsWithTrigger_cff import * 
from CMGTools.Common.PAT.patElectronsWithMVA_cfi import * 

# conversions
from CMGTools.Common.PAT.patConversions_cfi import patConversions

PATElectronSequence = cms.Sequence(
    pfElectronIsolationSequence +
    detElectronIsoDepositSequence + 
    patElectronIDSequence + 
    makePatElectrons +
    selectedPatElectrons + 
    patElectronsWithMVA +
    patElectronsWithTriggerSequence +
    patConversions 
    )


PATElectronSequence.insert( 0, gsfElectrons )
