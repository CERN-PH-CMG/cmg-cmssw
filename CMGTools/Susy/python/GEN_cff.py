import FWCore.ParameterSet.Config as cms

from PhysicsTools.HepMCCandAlgos.genParticles_cfi import genParticles
# from RecoJets.Configuration.GenJetParticles_cff import genJetParticles
# from RecoJets.Configuration.RecoGenJets_cff import ak5GenJets
from RecoMET.Configuration.GenMETParticles_cff import *
from RecoMET.Configuration.RecoGenMET_cff import genMetTrue

from CommonTools.ParticleFlow.genForPF2PAT_cff import *

from CMGTools.Susy.particleFlow_cfi import *

genMetTrue.src = 'particleFlow:GEN'
genMetTrueEt = genMetTrue.clone()
genMetTrueEt.usePt = False

GENSequence = cms.Sequence(
    genParticles +
    particleFlow +
    genMetTrue +
    genMetTrueEt
    )
