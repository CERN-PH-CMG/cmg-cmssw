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
         nbins = cms.untracked.int32(200)
        )
      ),
      
      eta = cms.untracked.VPSet(
        cms.untracked.PSet( 
         var = cms.untracked.string('eta()'),
         low = cms.untracked.double(-5),
         high = cms.untracked.double(5),
         nbins = cms.untracked.int32(100)
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


cmgTauSelPtHistograms = cms.EDAnalyzer("CmgTauHistograms",
                                       inputCollection = cms.InputTag("cmgTauSelPt"),
                                       histograms = cmgTauSelHistograms.histograms)

cmgTauSelEtaHistograms = cms.EDAnalyzer("CmgTauHistograms",
                                        inputCollection = cms.InputTag("cmgTauSelEta"),
                                        histograms = cmgTauSelHistograms.histograms)

cmgTauSelDecayModeFindingHistograms = cms.EDAnalyzer("CmgTauHistograms",
                                                     inputCollection = cms.InputTag("cmgTauSelDecayModeFinding"),
                                                     histograms = cmgTauSelHistograms.histograms)

cmgTauSelAgainstMuonTightHistograms = cms.EDAnalyzer("CmgTauHistograms",
                                                     inputCollection = cms.InputTag("cmgTauSelAgainstMuonTight"),
                                                     histograms = cmgTauSelHistograms.histograms)

cmgTauSelAgainstElectronLooseHistograms = cms.EDAnalyzer("CmgTauHistograms",
                                                     inputCollection = cms.InputTag("cmgTauSelAgainstElectronLoose"),
                                                     histograms = cmgTauSelHistograms.histograms)

cmgTauSelVertexHistograms = cms.EDAnalyzer("CmgTauHistograms",
                                           inputCollection = cms.InputTag("cmgTauSelVertex"),
                                           histograms = cmgTauSelHistograms.histograms)

cmgTauSelSumPtIsolationHistograms = cms.EDAnalyzer("CmgTauHistograms",
                                           inputCollection = cms.InputTag("cmgTauSelSumPtIsolation"),
                                           histograms = cmgTauSelHistograms.histograms)


cmgTauSelCleanHistograms = cms.EDAnalyzer("CmgTauHistograms",
                                          inputCollection = cms.InputTag("cmgTauSelClean"),
                                          histograms = cmgTauSelHistograms.histograms)





###############plots of the generator variables
cmgTauSelHistogramsGen = cms.EDAnalyzer(
    "CmgTauHistograms",
    inputCollection = cmgTauSelHistograms.inputCollection,
    histograms = cms.untracked.PSet(

      pt = cms.untracked.VPSet(
        cms.untracked.PSet( 
         var = cms.untracked.string('genTaup4().pt()'),
         low = cms.untracked.double(0),
         high = cms.untracked.double(200),
         nbins = cms.untracked.int32(200)
        )
      ),
      
      eta = cms.untracked.VPSet(
        cms.untracked.PSet( 
         var = cms.untracked.string('genTaup4().eta()'),
         low = cms.untracked.double(-5),
         high = cms.untracked.double(5),
         nbins = cms.untracked.int32(100)
        )
      ),

      phi = cms.untracked.VPSet(
        cms.untracked.PSet( 
         var = cms.untracked.string('genTaup4().phi()'),
         low = cms.untracked.double(-3.2),
         high = cms.untracked.double(3.2),
         nbins = cms.untracked.int32(64)
        )
      ),

      
    )
)

cmgTauSelPtHistogramsGen = cmgTauSelPtHistograms.clone()
cmgTauSelPtHistogramsGen.histograms = cmgTauSelHistogramsGen.histograms 

cmgTauSelEtaHistogramsGen = cmgTauSelEtaHistograms.clone()
cmgTauSelEtaHistogramsGen.histograms = cmgTauSelHistogramsGen.histograms 

cmgTauSelDecayModeFindingHistogramsGen = cmgTauSelDecayModeFindingHistograms.clone()
cmgTauSelDecayModeFindingHistogramsGen.histograms = cmgTauSelHistogramsGen.histograms

cmgTauSelAgainstMuonTightHistogramsGen = cmgTauSelAgainstMuonTightHistograms.clone()
cmgTauSelAgainstMuonTightHistogramsGen.histograms = cmgTauSelHistogramsGen.histograms

cmgTauSelAgainstElectronLooseHistogramsGen = cmgTauSelAgainstElectronLooseHistograms.clone()
cmgTauSelAgainstElectronLooseHistogramsGen.histograms = cmgTauSelHistogramsGen.histograms

cmgTauSelVertexHistogramsGen = cmgTauSelVertexHistograms.clone()
cmgTauSelVertexHistogramsGen.histograms = cmgTauSelHistogramsGen.histograms

cmgTauSelSumPtIsolationHistogramsGen = cmgTauSelSumPtIsolationHistograms.clone()
cmgTauSelSumPtIsolationHistogramsGen.histograms = cmgTauSelHistogramsGen.histograms

cmgTauSelCleanHistogramsGen = cmgTauSelCleanHistograms.clone()
cmgTauSelCleanHistogramsGen.histograms = cmgTauSelHistogramsGen.histograms 


## cmgTauSelPtHistogramsGen = cms.EDAnalyzer("CmgTauHistograms",
##                                        inputCollection = cms.InputTag("cmgTauSelPt"),
##                                        histograms = cmgTauSelHistogramsGen.histograms)

## cmgTauSelEtaHistogramsGen = cms.EDAnalyzer("CmgTauHistograms",
##                                         inputCollection = cms.InputTag("cmgTauSelEta"),
##                                         histograms = cmgTauSelHistogramsGen.histograms)

## cmgTauSelDecayModeFindingHistogramsGen = cms.EDAnalyzer("CmgTauHistograms",
##                                                      inputCollection = cms.InputTag("cmgTauSelDecayModeFinding"),
##                                                      histograms = cmgTauSelHistogramsGen.histograms)

## cmgTauSelAgainstMuonTightHistogramsGen = cms.EDAnalyzer("CmgTauHistograms",
##                                                      inputCollection = cms.InputTag("cmgTauSelAgainstMuonTight"),
##                                                      histograms = cmgTauSelHistogramsGen.histograms)

## cmgTauSelAgainstElectronLooseHistogramsGen = cms.EDAnalyzer("CmgTauHistograms",
##                                                      inputCollection = cms.InputTag("cmgTauSelAgainstElectronLoose"),
##                                                      histograms = cmgTauSelHistogramsGen.histograms)

## cmgTauSelVertexHistogramsGen = cms.EDAnalyzer("CmgTauHistograms",
##                                            inputCollection = cms.InputTag("cmgTauSelVertex"),
##                                            histograms = cmgTauSelHistogramsGen.histograms)

## cmgTauSelSumPtIsolationHistogramsGen = cms.EDAnalyzer("CmgTauHistograms",
##                                            inputCollection = cms.InputTag("cmgTauSelSumPtIsolation"),
##                                            histograms = cmgTauSelHistogramsGen.histograms)


## cmgTauSelCleanHistogramsGen = cms.EDAnalyzer("CmgTauHistograms",
##                                           inputCollection = cms.InputTag("cmgTauSelClean"),
##                                           histograms = cmgTauSelHistogramsGen.histograms)


