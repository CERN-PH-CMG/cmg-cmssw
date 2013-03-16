import FWCore.ParameterSet.Config as cms


cmgTauESCorrector = cms.EDFilter(
    "TauESCorrectorProducer",
    cfg = cms.PSet(
    inputCollection = cms.InputTag("cmgTauSel"),
    ##some parameters of the pT dependence correction are hardcode inside
    OneProngNoPi0Correction = cms.double(1.000),
    OneProng1Pi0Correction = cms.double(1.000),
    OneProng1Pi0CorrectionPtSlope = cms.double(0.0),
    ThreeProngCorrection = cms.double(1.000),
    ThreeProngCorrectionPtSlope = cms.double(0.0),
    ),    
    cuts = cms.PSet()
)

