import FWCore.ParameterSet.Config as cms


cmgElectronSelHistograms = cms.EDAnalyzer(
    "CmgElectronHistograms",
    inputCollection = cms.InputTag("cmgElectronSel"),
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


cmgElectronSelPtHistograms = cms.EDAnalyzer("CmgElectronHistograms",
                                       inputCollection = cms.InputTag("cmgElectronSelPt"),
                                       histograms = cmgElectronSelHistograms.histograms)

cmgElectronSelEtaHistograms = cms.EDAnalyzer("CmgElectronHistograms",
                                        inputCollection = cms.InputTag("cmgElectronSelEta"),
                                        histograms = cmgElectronSelHistograms.histograms)


cmgElectronSelCleanHistograms = cms.EDAnalyzer("CmgElectronHistograms",
                                          inputCollection = cms.InputTag("cmgElectronSelEta"),
                                          histograms = cmgElectronSelHistograms.histograms)


