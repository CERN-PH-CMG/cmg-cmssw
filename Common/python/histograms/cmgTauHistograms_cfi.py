import FWCore.ParameterSet.Config as cms

cmgTauHistograms = cms.EDAnalyzer(
    "CmgTauHistograms",
    inputCollection = cms.InputTag("cmgTauSel"),
    histograms = cms.untracked.PSet(
       pt_eta = cms.untracked.VPSet(
          cms.untracked.PSet( 
             var = cms.untracked.string('pt()'),
             low = cms.untracked.double(0),
             high = cms.untracked.double(200),
             nbins = cms.untracked.int32(50)
             ),
          cms.untracked.PSet( 
             var = cms.untracked.string('abs(eta())'),
             low = cms.untracked.double(0),
             high = cms.untracked.double(2.5),
             nbins = cms.untracked.int32(10)
             )
          )

       )
)


