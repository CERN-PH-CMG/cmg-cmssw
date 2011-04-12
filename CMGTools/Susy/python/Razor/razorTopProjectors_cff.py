import FWCore.ParameterSet.Config as cms

def addTopProjector(moduleType, topCollection, bottomCollection):

    return cms.EDProducer(
                   moduleType,
                   topCollection=cms.InputTag(topCollection),
                   bottomCollection=cms.InputTag(bottomCollection),
                   enable = cms.bool(True)
                   )

RazorParticleFlowNoMu = addTopProjector("TPCmgMuonOnPFCandidate","RazorMuon","particleFlow")
RazorParticleFlowNoEle = addTopProjector("TPCmgElectronOnPFCandidate","RazorElectron","particleFlow")    

RazorParticleFlow = cms.Sequence(RazorParticleFlowNoMu+RazorParticleFlowNoEle)
