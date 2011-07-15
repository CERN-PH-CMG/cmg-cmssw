import FWCore.ParameterSet.Config as cms


cmgDiMuSelHistograms = cms.EDAnalyzer(
    "CmgDiMuonHistograms",
    inputCollection = cms.InputTag("cmgDiMuonSel"),
    histograms = cms.untracked.PSet(

      pt = cms.untracked.VPSet(
        cms.untracked.PSet( 
         var = cms.untracked.string('pt()'),
         low = cms.untracked.double(0),
         high = cms.untracked.double(200),
         nbins = cms.untracked.int32(50)
        )
      ),
      
      eta = cms.untracked.VPSet(
        cms.untracked.PSet( 
         var = cms.untracked.string('eta()'),
         low = cms.untracked.double(-5),
         high = cms.untracked.double(5),
         nbins = cms.untracked.int32(40)
        )
      ),

      phi = cms.untracked.VPSet(
        cms.untracked.PSet( 
         var = cms.untracked.string('phi()'),
         low = cms.untracked.double(-3.2),
         high = cms.untracked.double(3.2),
         nbins = cms.untracked.int32(64)
        )
      ),

      mass = cms.untracked.VPSet(
        cms.untracked.PSet( 
         var = cms.untracked.string('mass()'),
         low = cms.untracked.double(1),
         high = cms.untracked.double(200),
         nbins = cms.untracked.int32(100)
        )
      ),

      
    )
)


cmgDiMuSelChargeHistograms = cms.EDAnalyzer("CmgDiMuonHistograms",
                                       inputCollection = cms.InputTag("cmgDiMuSelCharge"),
                                       histograms = cmgDiMuSelHistograms.histograms)

cmgDiMuSelMassHistograms = cms.EDAnalyzer("CmgDiMuonHistograms",
                                       inputCollection = cms.InputTag("cmgDiMuSelMass"),
                                       histograms = cmgDiMuSelHistograms.histograms)


cmgDiMuSelCleanHistograms = cms.EDAnalyzer("CmgDiMuonHistograms",
                                          inputCollection = cms.InputTag("cmgDiMuSelCharge"),
                                          histograms = cmgDiMuSelHistograms.histograms)


