######################################################################

import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.kinematics_cfi import kinematics

photonFactory = cms.PSet(
    inputCollection = cms.InputTag("pfSelectedPhotons"),
    isoDepWithCharged = cms.InputTag("phPFIsoDepositCharged"),
    isoDepWithPhotons = cms.InputTag("phPFIsoDepositGamma"),
    isoDepWithNeutral = cms.InputTag("phPFIsoDepositNeutral"),
    isoDepWithPU = cms.InputTag("phPFIsoDepositPU"),
    #maxAbsoluteIsolation = cms.double(10.),
    #maxRelativeIsolation = cms.double(1.0),
    maxAbsoluteIsolation = cms.double(1E9),
    maxRelativeIsolation = cms.double(1E9),
    ## each IsoPar has a coneSize parameter (radius of the cone inside which
    ## the isolation is computed) plus a VPSet called vetoes, made of any of the following
    ## vetoes: ThresholdVeto, ConeVeto, RectangularEtaPhiVeto (more vetoes can be implemented if needed)
    ## look at DataFormats/RecoCandidate/src/IsoDepositVetos.cc for their meaning.
    chargedHadronIsoPar=cms.PSet(
      coneSize=cms.double(0.3),
      vetoes=cms.VPSet(
         cms.PSet(
            #see task #15859 for discussion of this veto
            type=cms.string("RectangularEtaPhiVeto"),
            dEta=cms.double(0.01),# eta width of the strip to be vetoed
            dPhi=cms.double(0.20) # phi width of the strip to be vetoed
         ),
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
            #see task #15859 for discussion of this veto
            type=cms.string("RectangularEtaPhiVeto"),
            dEta=cms.double(0.01),# eta width of the strip to be vetoed
            dPhi=cms.double(0.20) # phi width of the strip to be vetoed
         ),
         cms.PSet(
            type=cms.string("ThresholdVeto"),
            threshold=cms.double(0.2)# pt of the particles to be vetoed
         ),
      ),
    ),

    photonsIsoPar=cms.PSet(
      coneSize=cms.double(0.3),
      vetoes=cms.VPSet(
         cms.PSet(
            #see task #15859 for discussion of this veto
            type=cms.string("RectangularEtaPhiVeto"),
            dEta=cms.double(0.01),# eta width of the strip to be vetoed
            dPhi=cms.double(0.20) # phi width of the strip to be vetoed
         ),
         cms.PSet(
            type=cms.string("ThresholdVeto"),
            threshold=cms.double(0.2)# pt of the particles to be vetoed
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

    allIsoPar=cms.PSet(
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
