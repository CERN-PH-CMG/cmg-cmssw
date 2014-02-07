import FWCore.ParameterSet.Config as cms

from CMGTools.Common.miscProducers.metSignificance_cfi import metSignificance
pfMetSignificance = metSignificance.clone()
pfMetSignificance.inputPATElectrons = cms.InputTag('')   
pfMetSignificance.inputPATMuons = cms.InputTag('')
# pfMetSignificance.inputPFJets = cms.InputTag('') #Jose: previously used pfJets here and pfNoJet below but now pfNoJet is no loger available 
# pfMetSignificance.inputPFCandidates = cms.InputTag('particleFlow')
pfMetSignificance.inputPFJets = cms.InputTag('ak5PFJets') #Jose: previously used pfJets here and pfNoJet below but now pfNoJet is no loger available 

from CommonTools.ParticleFlow.TopProjectors.pfNoJet_cfi import *
pfNoJet.topCollection = 'ak5PFJetsPtrs'
pfNoJet.bottomCollection = 'particleFlow'
pfMetSignificance.inputPFCandidates = cms.InputTag('pfNoJet')
pfMetSignificance.inputPFJets = cms.InputTag('ak5PFJetsPtrs')

MetSignificanceSequence = cms.Sequence(
    pfNoJet + 
    pfMetSignificance
    )

