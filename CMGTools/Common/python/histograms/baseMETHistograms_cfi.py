import FWCore.ParameterSet.Config as cms

baseMETHistograms = cms.EDAnalyzer(
    "GenericBaseMETHistograms",
    inputCollection = cms.InputTag("cmgMETPFCandidates"),
    histograms = cms.PSet(
        met = cms.VPSet(
            cms.PSet( 
               var = cms.string('pt()'),
               nbins = cms.int32(100),
               low = cms.double(0),
               high = cms.double(1000)
               )
            ),
    
        sumEt = cms.VPSet(
            cms.PSet( 
               var = cms.string('sumEt()'),
               nbins = cms.int32(100),
               low = cms.double(0),
               high = cms.double(2000)
               )
            ),
        )
    )
