import FWCore.ParameterSet.Config as cms

#See https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagPerformance

#these working points are for 7 TeV

trackCountingHighEffBJetTags = cms.PSet(
    loose = cms.string('btag() >= 1.7'),                                   
    medium = cms.string('btag() >= 3.3'),
    tight = cms.string('btag() >= 10.2')                                   
    )

trackCountingHighPurBJetTags = cms.PSet(
    loose = cms.string('btag() >= 1.19'),                                   
    medium = cms.string('btag() >= 1.93'),
    tight = cms.string('btag() >= 3.41')                                   
    )