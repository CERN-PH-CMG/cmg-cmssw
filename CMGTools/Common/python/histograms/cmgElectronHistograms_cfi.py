import FWCore.ParameterSet.Config as cms

cmgElectronHistograms = cms.EDAnalyzer(
    "CmgElectronHistograms",
    inputCollection = cms.InputTag("cmgElectronSel"),
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
        mva = cms.VPSet(
            cms.PSet( 
               var = cms.string('mva()'),
               nbins = cms.int32(50),
               low = cms.double(-1.),
               high = cms.double(2.)
               )
            )
        )
    )
