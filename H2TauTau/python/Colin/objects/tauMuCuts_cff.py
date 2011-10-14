import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.Colin.objects.muCuts_cff import * 
from CMGTools.H2TauTau.Colin.objects.tauCuts_cff import * 

tauMuCuts = cms.PSet(
    baseline = cms.PSet(
         muLeg = muCuts.clone(),
         tauLeg = tauCuts.clone()
         )
    )
