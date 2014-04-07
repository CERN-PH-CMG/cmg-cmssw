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

### for 2012
# From https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP#B_tagging_Operating_Points_for_5
# TCHPT is also recommended for 2012
jetProbabilityBJetTags = cms.PSet(
    mcbparton = cms.string('abs(sourcePtr().partonFlavour()) == 5'),                                    
    loose = cms.string('btag(2) >= 0.275'),                                   
    medium = cms.string('btag(2) >= 0.545'),
    tight = cms.string('btag(2) >= 0.790')                                   
    )

combinedSecondaryVertexBJetTags = cms.PSet(
    mcbparton = cms.string('abs(sourcePtr().partonFlavour()) == 5'),                                    
    loose = cms.string('btag(6) >= 0.244'),                                   
    medium = cms.string('btag(6) >= 0.679'),
    tight = cms.string('btag(6) >= 0.898')                                   
    )
