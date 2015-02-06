import FWCore.ParameterSet.Config as cms

# isolation (PF only, must not bourred !)

from CMGTools.Common.PAT.photonPFIsolationDeposits_cff import *
from CommonTools.ParticleFlow.ParticleSelectors.pfSelectedPhotons_cfi import *
from CMGTools.Common.photon_cff import *

sourcePhotons = 'pfSelectedPhotons'
pfSelectedPhotons.cut = cms.string("pt>2&&abs(eta)<2.5")

phPFIsoDepositCharged.src = sourcePhotons
phPFIsoDepositChargedAll.src = sourcePhotons
phPFIsoDepositNeutral.src = sourcePhotons
phPFIsoDepositGamma.src = sourcePhotons
phPFIsoDepositPU.src = sourcePhotons

PATPhotonSequence = cms.Sequence(
    pfSelectedPhotons+
    photonPFIsolationDepositsSequence
    )
