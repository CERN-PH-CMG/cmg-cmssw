import FWCore.ParameterSet.Config as cms


pfMetForRegression   = cms.EDProducer(
    "MetFlavorProducer",
    CorrJetName     = cms.InputTag("patJetsAK4PF"),
    PFCandidateName = cms.InputTag("packedPFCandidates"),
    VertexName      = cms.InputTag("offlineSlimmedPrimaryVertices"),
    RhoName         = cms.InputTag('fixedGridRhoFastjetAll'),
    JetPtMin        = cms.double(0.), # should be 0 for 5X and 1 for 4X
    dZMin           = cms.double(0.1),
    MetFlavor       = cms.int32(0),  # 0 PF  1 TK  2 No PU 3 PU  4 PUC
    WorkingPointId  = cms.uint32(3), # 3 for 5X and higher
    puJetIDName     = cms.string('pileupJetIdMET:met53xDiscriminant'),
    puJetIDNameLowPt = cms.string('pileupJetIdMET:met53xDiscriminant')
    )
   
tkMet     =  pfMetForRegression.clone(MetFlavor = cms.int32(1))
nopuMet   =  pfMetForRegression.clone(MetFlavor = cms.int32(2))
puMet     =  pfMetForRegression.clone(MetFlavor = cms.int32(3),
                                      dZMin = 0.2)
pcMet     =  pfMetForRegression.clone(MetFlavor = cms.int32(4))

                          
metRegressionSequence  = cms.Sequence (
    pfMetForRegression + 
    nopuMet +
    puMet + 
    pcMet +
    tkMet
    )
