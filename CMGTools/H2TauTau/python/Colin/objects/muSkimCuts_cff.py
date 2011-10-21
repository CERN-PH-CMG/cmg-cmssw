import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.Colin.objects.muCuts_cff import *

muSkimCuts = cms.PSet(
    kinematics = cms.PSet(
      pt = cms.string('leg2().pt()>14'),
      eta = muCuts.kinematics.eta
      ),
    iso = cms.PSet(
      relIsoDBeta = cms.string('leg2().relIso(0.5)<0.5')
      )
    )
