import FWCore.ParameterSet.Config as cms


from CMGTools.H2TauTau.objects.tauMuObjects_cff import *
from CMGTools.H2TauTau.objects.tauEleObjects_cff import *
from CMGTools.H2TauTau.objects.muEleObjects_cff import *
from CMGTools.H2TauTau.objects.diTauObjects_cff import *

#from CMGTools.H2TauTau.objects.object_cff import *
from CMGTools.H2TauTau.skims.skim_cff import *

#TODO:tau scaling could be general

# tau-mu ---

# preselection
tauMuPreSelPath = cms.Path(
    tauMuSequence + 
    tauMuPreSelSkimSequence 
    )

# full selection
tauMuFullSelPath = cms.Path(
    tauMuSequence + 
    tauMuFullSelSkimSequence
    )


# tau-ele ---

# preselection
tauElePreSelPath = cms.Path(
    #objectSequence +
    tauEleSequence + 
    tauElePreSelSkimSequence 
    )

# full selection
tauEleFullSelPath = cms.Path(
    #objectSequence +
    tauEleSequence + 
    tauEleFullSelSkimSequence     
    )


# mu-ele ---

# preselection
muElePreSelPath = cms.Path(
    #objectSequence +
    muEleSequence +
    muElePreSelSkimSequence 
    )

# full selection
muEleFullSelPath = cms.Path(
    #objectSequence +
    muEleSequence +
    muEleFullSelSkimSequence     
    )


# tau-tau ---

# preselection
diTauPreSelPath = cms.Path(
    #objectSequence +
    diTauSequence +
    diTauPreSelSkimSequence 
    )

# full selection
diTauFullSelPath = cms.Path(
    #objectSequence +
    diTauSequence +
    diTauFullSelSkimSequence     
    )


