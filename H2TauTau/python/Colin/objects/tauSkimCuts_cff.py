import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.Colin.objects.tauCuts_cff import *

tauSkimCuts = cms.PSet(
    kinematics = cms.PSet(
      pt = cms.string('leg1().pt()>18'),
      eta = tauCuts.kinematics.eta
    ),
    id = tauCuts.id.clone()
    )
