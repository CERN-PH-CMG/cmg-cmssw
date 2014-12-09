import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.objects.tauMuObjectsMVAMET_cff import *
from CMGTools.H2TauTau.objects.tauEleObjectsMVAMET_cff import *
from CMGTools.H2TauTau.objects.diTauObjectsMVAMET_cff import *
from CMGTools.H2TauTau.skims.skim_cff import *
from CMGTools.Common.miscProducers.mvaMET.metRegression_cff import *


# tau-mu ---

# full selection
tauMuPath = cms.Path(
    metRegressionSequence + 
    tauMuSequence + 
    tauMuFullSelSkimSequence
    )

# # tau-ele ---

# # full selection
# tauElePath = cms.Path(
#     metRegressionSequence + 
#     tauEleSequence + 
#     tauEleFullSelSkimSequence     
#     )

# # tau-tau ---

# # full selection
# diTauPath = cms.Path(
#     metRegressionSequence + 
#     diTauSequence +
#     diTauFullSelSkimSequence     
#     )


