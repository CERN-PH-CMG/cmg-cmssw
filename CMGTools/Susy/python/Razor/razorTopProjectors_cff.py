import FWCore.ParameterSet.Config as cms

def addTopProjector(moduleType, topCollection, bottomCollection):

    return cms.EDProducer(
                   moduleType,
                   topCollection=cms.InputTag(topCollection),
                   bottomCollection=cms.InputTag(bottomCollection),
                   enable = cms.bool(True)
                   )

RazorParticleFlowNoMu = addTopProjector("TPCmgMuonOnPFCandidate","RazorCmgMuon","particleFlow")
RazorParticleFlowNoEle = addTopProjector("TPCmgElectronOnPFCandidate","RazorCmgElectron","particleFlow")    

RazorParticleFlow = cms.Sequence(RazorParticleFlowNoMu+RazorParticleFlowNoEle)
