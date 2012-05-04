import FWCore.ParameterSet.Config as cms

from RecoJets.Configuration.GenJetParticles_cff import *
from RecoJets.Configuration.RecoGenJets_cff import *

# we want GenJets without neutrinos!
# will be added to the gen sequence later (see bottom of the file)
PATGenJetSequence = cms.Sequence(
    genParticlesForJetsNoNu +
    ak5GenJetsNoNu 
)
