import FWCore.ParameterSet.Config as cms

from CMGTools.Utilities.mvaMET.weights_gbr import weights_gbrmet,weights_gbrmetphi, weights_gbrmetu1cov, weights_gbrmetu2cov
from CMGTools.Common.miscProducers.mvaMET.metRegression_cff import pfMetForRegression

mvaMETTauMu = cms.EDProducer(
    "MVAMETProducerTauMu",
    pfmetSrc = cms.InputTag('pfMetForRegression'),
    tkmetSrc = cms.InputTag('tkMet'),
    nopumetSrc = cms.InputTag('nopuMet'),
    pucmetSrc = cms.InputTag('pcMet'),
    pumetSrc = cms.InputTag('puMet'),
    recBosonSrc = cms.InputTag('cmgTauMu'),
    jetSrc = cms.InputTag('patJetsAK4PF'),
    minJetPt = cms.double(30.),
    maxJetEta = cms.double(5.),
    puJetIdLabel = pfMetForRegression.puJetIDName,
    vertexSrc = cms.InputTag('goodPVFilter'),
    nJetsPtGt1Src = cms.InputTag('nJetsPtGt1'),
    rhoSrc = cms.InputTag('fixedGridRhoFastjetAll'),
    enable = cms.bool(True),
    verbose = cms.untracked.bool( False ),
    weights_gbrmet = cms.string(weights_gbrmet),
    weights_gbrmetphi = cms.string(weights_gbrmetphi),
    weights_gbrmetu1cov = cms.string(weights_gbrmetu1cov),
    weights_gbrmetu2cov = cms.string(weights_gbrmetu2cov),
    
    #COLIN: make delta R a parameter
    )

