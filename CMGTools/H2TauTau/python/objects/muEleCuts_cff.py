import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.objects.muCuts_cff import * 
from CMGTools.H2TauTau.objects.eleCuts_cff import * 
from CMGTools.H2TauTau.objects.muSkimCuts_cff import * 
from CMGTools.H2TauTau.objects.eleSkimCuts_cff import * 

muEleCuts = cms.PSet(
    baseline = cms.PSet(
         mass = cms.string('mass()>10'),
         muLeg = getMuCuts('leg1').clone(),
         eleLeg = eleCuts.clone(),
         ),
    skimming = cms.PSet(
         mass = cms.string('mass()>10'),
         muLegSkimCuts = muSkimCuts.clone(),
         eleLegSkimCuts = eleSkimCuts.clone(),
         )
    )
