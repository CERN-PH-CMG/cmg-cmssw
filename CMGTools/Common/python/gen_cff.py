import FWCore.ParameterSet.Config as cms

from CMGTools.Common.generator.genParticlesPruned_cfi import *
from CMGTools.Common.generator.genJets_cff import *
from CMGTools.Common.generator.listParticles_cfi import *
from SimGeneral.HepPDTESSource.pythiapdt_cfi import *

genSequence = cms.Sequence(
    genParticlesPruned
    + genJetsSequence
    + listParticles
    )
