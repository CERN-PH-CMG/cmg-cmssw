######################################################################

import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.kinematics_cfi import kinematics

photonFactory = cms.PSet(
    inputCollection = cms.InputTag("pfSelectedPhotons"),
    pfCollection = cms.InputTag("particleFlow"),
    muonCollection = cms.InputTag("patMuonsWithTrigger"),
    isoDepWithCharged = cms.InputTag("phPFIsoDepositCharged"),
    isoDepWithPhotons = cms.InputTag("phPFIsoDepositGamma"),
    isoDepWithNeutral = cms.InputTag("phPFIsoDepositNeutral"),
    isoDepWithPU = cms.InputTag("phPFIsoDepositPU"),
    ## each IsoPar has a coneSize parameter (radius of the cone inside which
    ## the isolation is computed) plus a VPSet called vetoes, made of any of the following
    ## vetoes: ThresholdVeto, ConeVeto, RectangularEtaPhiVeto (more vetoes can be implemented if needed)
    ## look at DataFormats/RecoCandidate/src/IsoDepositVetos.cc for their meaning.
    chargedHadronIsoPar=cms.PSet(
      coneSize=cms.double(0.3),
      vetoes=cms.VPSet(
         cms.PSet(
            type=cms.string("ThresholdVeto"),
            threshold=cms.double(0.2)# pt of the particles to be vetoed
         ),
      ),
    ),

    neutralHadronIsoPar=cms.PSet(
      coneSize=cms.double(0.3),
      vetoes=cms.VPSet(
         cms.PSet(
            type=cms.string("ThresholdVeto"),
            threshold=cms.double(0.5)# pt of the particles to be vetoed
         ),
      ),
    ),

    photonsIsoPar=cms.PSet(
      coneSize=cms.double(0.3),
      vetoes=cms.VPSet(
         cms.PSet(
            type=cms.string("ThresholdVeto"),
            threshold=cms.double(0.5)# pt of the particles to be vetoed
         ),
      ),
    ),                          

    puIsoPar=cms.PSet(
      coneSize=cms.double(0.3),
      vetoes=cms.VPSet(
         cms.PSet(
            type=cms.string("ThresholdVeto"),
            threshold=cms.double(0.2)# pt of the particles to be vetoed
         ),
      ),
    ),                          

)

cmgPhoton = cms.EDFilter("PhotonPOProducer",
                         cfg = photonFactory.clone(
                         ),
                         cuts = cms.PSet(kinematics = kinematics.clone(
                              pt = cms.string('pt() > 2'),
                              eta = cms.string('abs(eta()) < 2.5'),
                              phi = cms.string('abs(phi()) < 3.2')
                         ))
                         )

######################################################################
