import FWCore.ParameterSet.Config as cms

#from CMGTools.Common.generator.genParticlesStatus3_cfi import *
#from CMGTools.Common.generator.genLeptons_cff import *
from CMGTools.Common.generator.genParticlesPruned_cfi import *
from CMGTools.Common.generator.genJets_cff import *
from CMGTools.Common.generator.listParticles_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeight_cff import *

from SimGeneral.HepPDTESSource.pythiapdt_cfi import *
#from GeneratorInterface.GenFilters.TotalKinematicsFilter_cfi import *
from CMGTools.Common.eventCleaning.totalKinematicsFilterCMG_cfi import *

genSequence = cms.Sequence(
    #totalKinematicsFilter     
    totalKinematicsFilterCMG     
    #+genParticlesStatus3
    + genParticlesPruned
    #+genLeptonsSequence
    + genJetsSequence
    + listParticles
    + vertexWeightSequence
    )
