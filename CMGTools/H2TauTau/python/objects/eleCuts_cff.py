import FWCore.ParameterSet.Config as cms

#TODO implement real electron cuts

eleCuts = cms.PSet(
    kinematics = cms.PSet(
      pt = cms.string('leg2().pt()>20'),
      eta = cms.string('abs(leg2().eta())<2.5')
    ),
    iso = cms.PSet(
      #COLIN the iso elest be done on charged particles, not charged hadrons
      relIsoDBeta = cms.string('leg2().relIso(0.5, 1)<100.0')
    )
    )
