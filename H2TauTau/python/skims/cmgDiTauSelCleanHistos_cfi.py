import FWCore.ParameterSet.Config as cms


cmgDiTauSelHistograms = cms.EDAnalyzer(
    "CmgDiTauHistograms",
    inputCollection = cms.InputTag("cmgDiTauSel"),
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


cmgDiTauSelMassHistograms =cmgDiTauSelHistograms.clone()
cmgDiTauSelMassHistograms.inputCollection = cms.InputTag("cmgDiTauSelMass")
                                       

cmgDiTauSelChargeHistograms =cmgDiTauSelHistograms.clone()
cmgDiTauSelChargeHistograms.inputCollection = cms.InputTag("cmgDiTauSelCharge")
                                       

cmgDiTauSelPtHistograms =cmgDiTauSelHistograms.clone()
cmgDiTauSelPtHistograms.inputCollection = cms.InputTag("cmgDiTauSelPt")
                                       

cmgDiTauSelEtaHistograms =cmgDiTauSelHistograms.clone()
cmgDiTauSelEtaHistograms.inputCollection = cms.InputTag("cmgDiTauSelEta")
                                       

cmgDiTauSelAgainstMuonHistograms =cmgDiTauSelHistograms.clone()
cmgDiTauSelAgainstMuonHistograms.inputCollection = cms.InputTag("cmgDiTauSelAgainstMuon")
                                       

cmgDiTauSelAgainstElectronHistograms =cmgDiTauSelHistograms.clone()
cmgDiTauSelAgainstElectronHistograms.inputCollection = cms.InputTag("cmgDiTauSelAgainstElectron")
                                       

cmgDiTauSelSumPtIsolationHistograms =cmgDiTauSelHistograms.clone()
cmgDiTauSelSumPtIsolationHistograms.inputCollection = cms.InputTag("cmgDiTauSelSumPtIsolation")
                                       

#cmgDiTauSelHistograms =cmgDiTauSelHistograms.clone()
#cmgDiTauSelHistograms.inputCollection = cms.InputTag("cmgDiTauSel")
#                                       

cmgDiTauSelCleanHistograms =cmgDiTauSelHistograms.clone()
cmgDiTauSelCleanHistograms.inputCollection = cms.InputTag("cmgDiTauSelClean")
                                          


