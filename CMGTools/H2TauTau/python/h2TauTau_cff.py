import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.objects.object_cff import *
from CMGTools.H2TauTau.skims.skim_cff import *

#TODO:tau scaling could be general

# tau-mu ---

# preselection
tauMuPreSelPath = cms.Path(
    objectSequence + 
    tauMuPreSelSkimSequence 
    )

# full selection
tauMuFullSelPath = cms.Path(
    objectSequence + 
    tauMuFullSelSkimSequence
    )


# tau-ele ---

# preselection
tauElePreSelPath = cms.Path(
    objectSequence + 
    tauElePreSelSkimSequence 
    )

# full selection
tauEleFullSelPath = cms.Path(
    objectSequence + 
    tauEleFullSelSkimSequence     
    )


# mu-ele ---

# preselection
muElePreSelPath = cms.Path(
    objectSequence + 
    muElePreSelSkimSequence 
    )

# full selection
muEleFullSelPath = cms.Path(
    objectSequence + 
    muEleFullSelSkimSequence     
    )


# tau-tau ---

# preselection
diTauPreSelPath = cms.Path(
    objectSequence + 
    diTauPreSelSkimSequence 
    )

# full selection
diTauFullSelPath = cms.Path(
    objectSequence + 
    diTauFullSelSkimSequence     
    )


