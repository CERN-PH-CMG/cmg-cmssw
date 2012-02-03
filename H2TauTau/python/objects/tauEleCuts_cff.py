import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.objects.eleCuts_cff import * 
from CMGTools.H2TauTau.objects.tauCuts_cff import * 
from CMGTools.H2TauTau.objects.eleSkimCuts_cff import * 
from CMGTools.H2TauTau.objects.tauSkimCuts_cff import * 

tauEleCuts = cms.PSet(
    baseline = cms.PSet(
         mass = cms.string('mass()>10'),
         eleLeg = eleCuts.clone(),
         tauLeg = tauCuts.clone()
         ),
    skimming = cms.PSet(
         mass = cms.string('mass()>10'),
         eleLegSkimCuts = eleSkimCuts.clone(),
         tauLegSkimCuts = tauSkimCuts.clone()
         )
    )
