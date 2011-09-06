import FWCore.ParameterSet.Config as cms

from CMGTools.Common.generator.genParticlesStatus3_cfi import *
from CMGTools.Common.generator.genLeptons_cff import *
from CMGTools.Common.generator.listParticles_cfi import *
from SimGeneral.HepPDTESSource.pythiapdt_cfi import *

genSequence = cms.Sequence(
    genParticlesStatus3
    + genLeptonsSequence
    + listParticles
    )
