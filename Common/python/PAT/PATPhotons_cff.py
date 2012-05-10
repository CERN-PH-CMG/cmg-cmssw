import FWCore.ParameterSet.Config as cms

# isolation (PF only, must not bourred !)

# from CommonTools.ParticleFlow.Isolation.pfPhotonIsolation_cff import *
from CMGTools.Common.PAT.photonPFIsolationDeposits_cff import *
from CommonTools.ParticleFlow.ParticleSelectors.pfSelectedPhotons_cfi import *
# from CommonTools.ParticleFlow.Isolation.pfIsolatedPhotons_cfi import *
from CMGTools.Common.photon_cff import *

sourcePhotons = 'pfSelectedPhotons'

# the name of the track muon deposits changed
# from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X
# tkDeposits = cms.InputTag("muons","muIsoDepositTk")
# if cmsswIs44X():
#    tkDeposits = cms.InputTag("muIsoDepositTk")

phPFIsoDepositCharged.src = sourcePhotons
phPFIsoDepositChargedAll.src = sourcePhotons
phPFIsoDepositNeutral.src = sourcePhotons
phPFIsoDepositGamma.src = sourcePhotons
phPFIsoDepositPU.src = sourcePhotons

pfPhotonIsoDepositsSequence = cms.Sequence(
    phPFIsoDepositCharged   +
    phPFIsoDepositChargedAll   +
    phPFIsoDepositNeutral   +
    phPFIsoDepositGamma   +
    phPFIsoDepositPU
)

PATPhotonSequence = cms.Sequence(
    pfSelectedPhotons+
    pfPhotonIsoDepositsSequence
    # +
    # pfIsolatedPhotons
    )
