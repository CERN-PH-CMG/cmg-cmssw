import FWCore.ParameterSet.Config as cms

cmgDiMuonHistograms = cms.EDAnalyzer(
    "CmgDiMuonHistograms",
    inputCollection = cms.InputTag("cmgDiMuonSel"),
    histograms = cms.PSet(
        pt = cms.VPSet(
            cms.PSet( 
               var = cms.string('pt()'),
               nbins = cms.int32(100),
               low = cms.double(0),
               high = cms.double(200)
               )
            ),
        eta = cms.VPSet(
            cms.PSet( 
               var = cms.string('eta()'),
               nbins = cms.int32(50),
               low = cms.double(-3.),
               high = cms.double(3.)
               )
            ),
        phi = cms.VPSet(
            cms.PSet( 
               var = cms.string('phi()'),
               nbins = cms.int32(50),
               low = cms.double(-3.15),
               high = cms.double(3.15)
               )
            ),
        mass = cms.VPSet(
            cms.PSet( 
               var = cms.string('mass()'),
               nbins = cms.int32(100),
               low = cms.double(0),
               high = cms.double(200)
               )
            )
        )
    )
