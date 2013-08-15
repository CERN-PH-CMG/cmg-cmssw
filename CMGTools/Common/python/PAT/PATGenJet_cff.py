import FWCore.ParameterSet.Config as cms

from RecoJets.Configuration.GenJetParticles_cff import *
from RecoJets.Configuration.RecoGenJets_cff import *

# we want GenJets without neutrinos!
# will be added to the gen sequence later (see bottom of the file)
# PATGenJetSequence = cms.Sequence()

# from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X
# if cmsswIs44X():
# in 44X ak5GenJetsNoNu is not in the AOD, need to rebuild.
PATGenJetSequence = cms.Sequence(
    genParticlesForJetsNoNu +
    ak5GenJetsNoNu 
    )
