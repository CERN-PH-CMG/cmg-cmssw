import FWCore.ParameterSet.Config as cms

cmgElectronHistograms = cms.EDAnalyzer(
    "CmgElectronHistograms",
    inputCollection = cms.InputTag("cmgElectronSel"),
    histograms = cms.untracked.PSet(
        pt = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('pt()'),
               nbins = cms.untracked.int32(100),
               low = cms.untracked.double(0),
               high = cms.untracked.double(200)
               )
            ),
        eta = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('eta()'),
               nbins = cms.untracked.int32(50),
               low = cms.untracked.double(-3.),
               high = cms.untracked.double(3.)
               )
            ),
        phi = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('phi()'),
               nbins = cms.untracked.int32(50),
               low = cms.untracked.double(-3.15),
               high = cms.untracked.double(3.15)
               )
            ),
        mva = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('mva()'),
               nbins = cms.untracked.int32(50),
               low = cms.untracked.double(-1.),
               high = cms.untracked.double(2.)
               )
            )
        )
    )
