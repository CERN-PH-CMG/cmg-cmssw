import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.Colin.objects.muCuts_cff import * 
from CMGTools.H2TauTau.Colin.objects.tauCuts_cff import * 
from CMGTools.H2TauTau.Colin.objects.muSkimCuts_cff import * 
from CMGTools.H2TauTau.Colin.objects.tauSkimCuts_cff import * 

tauMuCuts = cms.PSet(
    baseline = cms.PSet(
         muLeg = muCuts.clone(),
         tauLeg = tauCuts.clone()
         ),
    skimming = cms.PSet(
         mass = cms.string('mass()>10'),
         muLegSkimCuts = muSkimCuts.clone(),
         tauLegSkimCuts = tauSkimCuts.clone()
         )
    )
