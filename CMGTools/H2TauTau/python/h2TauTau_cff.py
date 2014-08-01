import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.objects.gen_cff import *
from CMGTools.H2TauTau.objects.tauMuObjectsMVAMET_cff import *
from CMGTools.H2TauTau.objects.tauEleObjectsMVAMET_cff import *
from CMGTools.H2TauTau.objects.diTauObjectsMVAMET_cff import *
from CMGTools.H2TauTau.skims.skim_cff import *
from CMGTools.Common.PAT.MetRegression_cff import *


# tau-mu ---

# full selection
tauMuPath = cms.Path(
    genSequence + 
    MetRegressionSequence + 
    tauMuSequence + 
    tauMuFullSelSkimSequence
    )


# tau-ele ---

# full selection
tauElePath = cms.Path(
    genSequence + 
    MetRegressionSequence + 
    tauEleSequence + 
    tauEleFullSelSkimSequence     
    )


# tau-tau ---

# full selection
diTauPath = cms.Path(
    genSequence +
    MetRegressionSequence + 
    diTauSequence +
    diTauFullSelSkimSequence     
    )


