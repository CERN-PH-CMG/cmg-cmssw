import FWCore.ParameterSet.Config as cms

from PhysicsTools.HepMCCandAlgos.genParticles_cfi import genParticles
# from RecoJets.Configuration.GenJetParticles_cff import genJetParticles
# from RecoJets.Configuration.RecoGenJets_cff import ak5GenJets
from RecoMET.Configuration.GenMETParticles_cff import *
from RecoMET.Configuration.RecoGenMET_cff import genMetTrue

from PhysicsTools.PFCandProducer.genForPF2PAT_cff import *

gen = cms.Sequence(
    genParticles +
    genForPF2PATSequence + 
    # genJetParticles +
    # ak5GenJets +
    genMETParticles +
    genMetTrue 
    )
