import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.objects.gen_cff import *
from CMGTools.H2TauTau.objects.tauMuObjects_cff import *
from CMGTools.H2TauTau.objects.tauEleObjects_cff import *
from CMGTools.H2TauTau.objects.muEleObjects_cff import *
from CMGTools.H2TauTau.objects.diTauObjects_cff import *
from CMGTools.H2TauTau.skims.skim_cff import *

#TODO:tau scaling could be general


# tau-mu ---

# full selection
tauMuPath = cms.Path(
    genSequence + 
    tauMuSequence + 
    tauMuFullSelSkimSequence
    )


# tau-ele ---

# full selection
tauElePath = cms.Path(
    genSequence + 
    tauEleSequence + 
    tauEleFullSelSkimSequence     
    )


# tau-tau ---

# full selection
diTauPath = cms.Path(
    genSequence +
    diTauSequence +
    diTauFullSelSkimSequence     
    )


