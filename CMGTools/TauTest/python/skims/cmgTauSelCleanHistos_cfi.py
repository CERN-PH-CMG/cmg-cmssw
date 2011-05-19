import FWCore.ParameterSet.Config as cms


cmgTauSelHistograms = cms.EDAnalyzer(
    "CmgTauHistograms",
    inputCollection = cms.InputTag("cmgTauSel"),
    histograms = cms.untracked.PSet(

      pt = cms.untracked.VPSet(
        cms.untracked.PSet( 
         var = cms.untracked.string('pt()'),
         low = cms.untracked.double(0),
         high = cms.untracked.double(200),
         nbins = cms.untracked.int32(40)
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

      
    )
)


cmgTauSelPtHistograms = cms.EDAnalyzer(
    "CmgTauHistograms",
    inputCollection = cms.InputTag("cmgTauSelPt"),
    histograms = cmgTauSelHistograms.histograms
 
)

cmgTauSelEtaHistograms = cms.EDAnalyzer(
    "CmgTauHistograms",
    inputCollection = cms.InputTag("cmgTauSelEta"),
    histograms = cmgTauSelHistograms.histograms
 
)

cmgTauSelCleanHistograms = cms.EDAnalyzer(
    "CmgTauHistograms",
    inputCollection = cms.InputTag("cmgTauSelClean"),
    histograms = cmgTauSelHistograms.histograms
 
)


