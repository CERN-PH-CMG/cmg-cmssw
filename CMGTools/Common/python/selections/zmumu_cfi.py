import FWCore.ParameterSet.Config as cms

zmumu = cms.PSet(
    muons = cms.string( 'leg1().getSelection(\"vbtfmuon\") || leg2().getSelection(\"vbtfmuon\")'),
    mass = cms.string('mass() >= 40 && mass() < 120')
)
