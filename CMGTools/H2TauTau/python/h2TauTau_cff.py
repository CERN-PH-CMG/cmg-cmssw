import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.objects.tauMuObjectsMVAMET_cff import tauMuSequence
# from CMGTools.H2TauTau.objects.tauEleObjectsMVAMET_cff import tauEleSequence
# from CMGTools.H2TauTau.objects.diTauObjectsMVAMET_cff import diTauSequence
from CMGTools.H2TauTau.skims.skim_cff import tauMuFullSelSkimSequence, tauEleFullSelSkimSequence, diTauFullSelSkimSequence


from CMGTools.H2TauTau.objects.tauMuObjectsMVAMET_cff import puJetIdForPFMVAMEt, pfMVAMEt, calibratedAK4PFJetsForPFMVAMEt, cmgTauMu, cmgTauMuCor, cmgTauMuTauPtSel, cmgTauMuCorSVFitPreSel, cmgTauMuCorSVFitFullSel, tauMuMVAMetSequence

from CMGTools.H2TauTau.skims.skim_cff import tauMuFullSelCount, tauEleFullSelCount, diTauFullSelCount

# tau-mu ---

# full selection
tauMuPath = cms.Path(
    # metRegressionSequence + 
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


