import FWCore.ParameterSet.Config as cms

leptonFactory = cms.PSet(
                         
    # set this to use the isoDeposits rather than the userIsolation
    useIsoDeposits = cms.bool(False),
    
    # set this to false to use pat::TrackIso, pat::HcalIso, and pat::EcalIso
    useParticleFlowIso = cms.bool(True),

    ## each IsoPar has a coneSize parameter (radius of the cone inside which
    ## the isolation is computed) plus a VPSet called vetoes, made of any of the following
    ## vetoes: ThresholdVeto, ConeVeto, RectangularEtaPhiVeto (more vetoes can be implemented if needed)
    ## look at DataFormats/RecoCandidate/src/IsoDepositVetos.cc for their meaning.
    chargedHadronIsoPar=cms.PSet(
      coneSize=cms.double(0.4),
      vetoes=cms.VPSet()#no veto
      ),

    chargedAllIsoPar=cms.PSet(
      coneSize=cms.double(0.4),
      vetoes=cms.VPSet()#no veto
      ),

    puChargedHadronIsoPar=cms.PSet(
      coneSize=cms.double(0.4),
      vetoes=cms.VPSet()#no veto
      ),

    neutralHadronIsoPar=cms.PSet(
      coneSize=cms.double(0.4),
      vetoes=cms.VPSet()#no veto
      ),

    photonsIsoPar=cms.PSet(
      coneSize=cms.double(0.4),
      vetoes=cms.VPSet(
         cms.PSet(
            #see task #15859 for discussion of this veto
            type=cms.string("RectangularEtaPhiVeto"),
            dEta=cms.double(0.1),# eta width of the strip to be vetoed
            dPhi=cms.double(0.2) # phi width of the strip to be vetoed
         )
     )
   ),
    
    vertexCollection = cms.InputTag("offlinePrimaryVertices"),
    vertexType = cms.int32(0)#use the primary vertex by default
                              
)
