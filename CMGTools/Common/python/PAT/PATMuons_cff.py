import FWCore.ParameterSet.Config as cms

# from PhysicsTools.PatAlgos.patSequences_cff import *
from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cff import *
from PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi import selectedPatMuons

from CMGTools.Common.PAT.patLeptModifiedIsoDeposit_cff import *

patMuons.embedCaloMETMuonCorrs = False
patMuons.embedTcMETMuonCorrs = False
patMuons.embedTrack = True

# isolation (PF and custom detector based)

from CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff import *

sourceMuons = 'muons'

# the name of the track muon deposits changed
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X
tkDeposits = cms.InputTag("muons","muIsoDepositTk")
if cmsswIs44X():
    tkDeposits = cms.InputTag("muIsoDepositTk")

muPFIsoDepositCharged.src = sourceMuons
muPFIsoDepositChargedAll.src = sourceMuons
muPFIsoDepositNeutral.src = sourceMuons
muPFIsoDepositGamma.src = sourceMuons
muPFIsoDepositPU.src = sourceMuons

patMuons.isoDeposits = cms.PSet(
    pfChargedHadrons = cms.InputTag("muPFIsoDepositCharged" ),
    pfChargedAll = cms.InputTag("muPFIsoDepositChargedAll" ),
    pfPUChargedHadrons = cms.InputTag("muPFIsoDepositPU" ),
    pfNeutralHadrons = cms.InputTag("muPFIsoDepositNeutral" ),
    pfPhotons = cms.InputTag("muPFIsoDepositGamma" ),
    tracker = tkDeposits,
    )

# needed in 44X:
muIsoFromDepsTkOptimized.deposits[0].src = tkDeposits

patMuons.isolationValues = cms.PSet(
    pfChargedHadrons = cms.InputTag("muPFIsoValueCharged04"),
    pfChargedAll = cms.InputTag("muPFIsoValueChargedAll04"),
    pfPUChargedHadrons = cms.InputTag("muPFIsoValuePU04" ),
    pfNeutralHadrons = cms.InputTag("muPFIsoValueNeutral04" ),
    pfPhotons = cms.InputTag("muPFIsoValueGamma04" ),
    user = cms.VInputTag( cms.InputTag("muIsoFromDepsTkOptimized") )
    )

selectedPatMuons.cut = 'pt()>0'

from CMGTools.Common.PAT.patMuonsWithPF_cfi import * 
from CMGTools.Common.PAT.patMuonsWithMVA_cfi import * 
from CMGTools.Common.PAT.patMuonsWithTrigger_cff import * 
from CMGTools.Common.PAT.patMuonsWithDirectionalIsolation_cfi import * 

PATMuonSequence = cms.Sequence(
    pfMuonIsolationSequence +
    detMuonIsoDepositSequence + 
    makePatMuons +
    selectedPatMuons +
    patMuonsWithPF +
    patMuonsWithMVA +
    patMuonsWithDirectionalIsolation +
    patMuonsWithTriggerSequence 
    )
