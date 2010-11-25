import FWCore.ParameterSet.Config as cms

wmunu = cms.PSet(
    muon = cms.string( 'leg1().getSelection(\"cuts_vbtfmuon\") && '),
    isolation = cms.string('leg1().absIso() < 100')
)