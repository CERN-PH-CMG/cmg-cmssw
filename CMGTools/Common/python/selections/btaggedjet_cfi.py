import FWCore.ParameterSet.Config as cms

#See https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagPerformance

#these working points are for 7 TeV

trackCountingHighEffBJetTags = cms.PSet(
    mcbparton = cms.string('abs(sourcePtr().partonFlavour()) == 5'),                                    
    loose = cms.string('btag(0) >= 1.7'),                                   
    medium = cms.string('btag(0) >= 3.3'),
    tight = cms.string('btag(0) >= 10.2')                                   
    )

trackCountingHighPurBJetTags = trackCountingHighEffBJetTags.clone(
    loose = cms.string('btag(1) >= 1.19'),                                   
    medium = cms.string('btag(1) >= 1.93'),
    tight = cms.string('btag(1) >= 3.41')                                   
    )