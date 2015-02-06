import FWCore.ParameterSet.Config as cms
from RecoMET.METProducers.METSigParams_cfi import *

metSignificance = cms.EDProducer(
    "METSignificanceProducer",
    inputPATMuons = cms.InputTag('patMuonsAK5'),
    inputPATElectrons = cms.InputTag('patElectronsAK5'),
    inputPFJets = cms.InputTag('pfJetsAK5'),
    inputPFCandidates = cms.InputTag('pfNoJetAK5'),
    resolution = METSignificance_params
)
