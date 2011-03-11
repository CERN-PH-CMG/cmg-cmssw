import FWCore.ParameterSet.Config as cms

wmunu = cms.PSet(
    mt = cms.string( 'mt() >= 20'),
    leg1_kinematics = cms.PSet(
                               pt = cms.string('leg1().pt() > 15'),
                               eta = cms.string('abs(leg1().eta()) < 2.1'),
                               phi = cms.string('abs(leg1().phi()) < 10')
    ),
    muon = cms.string( 'leg1().getSelection(\"cuts_vbtfmuon\")'),
    isolation = cms.string('leg1().getSelection(\"cuts_isomuon\")')
)