import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.objects.muCuts_cff import * 
from CMGTools.H2TauTau.objects.tauCuts_cff import * 
from CMGTools.H2TauTau.objects.muSkimCuts_cff import * 
from CMGTools.H2TauTau.objects.tauSkimCuts_cff import * 

tauMuCuts = cms.PSet(
    baseline = cms.PSet(
         mass = cms.string('mass()>10'),
         muLeg = muCuts.clone(),
         tauLeg = tauCuts.clone()
         ),
    # this cut is kept out of the baseline for now, until it is studied.
    caloMuVeto = cms.string('leg1().eOverP()>0.2'),
    skimming = cms.PSet(
         mass = cms.string('mass()>10'),
         muLegSkimCuts = muSkimCuts.clone(),
         tauLegSkimCuts = tauSkimCuts.clone()
         )
    )
