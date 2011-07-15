import FWCore.ParameterSet.Config as cms


cmgMuonSelHistograms = cms.EDAnalyzer(
    "CmgMuonHistograms",
    inputCollection = cms.InputTag("cmgMuonSel"),
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

      
    )
)


#cmgMuonSelPtHistograms = cms.EDAnalyzer("CmgMuonHistograms",
#                                       inputCollection = cms.InputTag("cmgMuonSelPt"),
#                                       histograms = cmgMuonSelHistograms.histograms)
#
#cmgMuonSelEtaHistograms = cms.EDAnalyzer("CmgMuonHistograms",
#                                        inputCollection = cms.InputTag("cmgMuonSelEta"),
#                                        histograms = cmgMuonSelHistograms.histograms)
#
#cmgMuonSelCleanHistograms = cms.EDAnalyzer("CmgMuonHistograms",
#                                          inputCollection = cms.InputTag("cmgMuonSelEta"),
#                                          histograms = cmgMuonSelHistograms.histograms)

cmgMuonSelPtHistograms = cmgMuonSelHistograms.clone()
cmgMuonSelPtHistograms.inputCollection = cms.InputTag("cmgMuonSelPt")

cmgMuonSelEtaHistograms = cmgMuonSelHistograms.clone()
cmgMuonSelEtaHistograms.inputCollection = cms.InputTag("cmgMuonSelEta")

cmgMuonSelIsGlobalTrackerHistograms = cmgMuonSelHistograms.clone()
cmgMuonSelIsGlobalTrackerHistograms.inputCollection = cms.InputTag("cmgMuonSelIsGlobalTracker")

cmgMuonSelPixelHitsHistograms = cmgMuonSelHistograms.clone()
cmgMuonSelPixelHitsHistograms.inputCollection = cms.InputTag("cmgMuonSelPixelHits")

cmgMuonSelTrackerHitsHistograms = cmgMuonSelHistograms.clone()
cmgMuonSelTrackerHitsHistograms.inputCollection = cms.InputTag("cmgMuonSelTrackerHits")

cmgMuonSelChi2Histograms = cmgMuonSelHistograms.clone()
cmgMuonSelChi2Histograms.inputCollection = cms.InputTag("cmgMuonSelChi2")

cmgMuonSelVertexHistograms = cmgMuonSelHistograms.clone()
cmgMuonSelVertexHistograms.inputCollection = cms.InputTag("cmgMuonSelVertex")


#cmgMuonSelHistograms = cmgMuonSelHistograms.clone()
#cmgMuonSelHistograms.inputCollection = cms.InputTag("cmgMuonSel")
#
#cmgMuonSelHistograms = cmgMuonSelHistograms.clone()
#cmgMuonSelHistograms.inputCollection = cms.InputTag("cmgMuonSel")
#
#cmgMuonSelHistograms = cmgMuonSelHistograms.clone()
#cmgMuonSelHistograms.inputCollection = cms.InputTag("cmgMuonSel")

cmgMuonSelCleanHistograms = cmgMuonSelHistograms.clone()
cmgMuonSelCleanHistograms.inputCollection = cms.InputTag("cmgMuonSelClean")
