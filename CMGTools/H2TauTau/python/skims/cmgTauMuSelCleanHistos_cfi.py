import FWCore.ParameterSet.Config as cms


cmgTauMuSelHistograms = cms.EDAnalyzer(
    "CmgTauMuHistograms",
    inputCollection = cms.InputTag("cmgTauMuSel"),
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


cmgTauMuSelMassHistograms =cmgTauMuSelHistograms.clone()
cmgTauMuSelMassHistograms.inputCollection = cms.InputTag("cmgTauMuSelMass")
                                       

cmgTauMuSelChargeHistograms =cmgTauMuSelHistograms.clone()
cmgTauMuSelChargeHistograms.inputCollection = cms.InputTag("cmgTauMuSelCharge")
                                       

cmgTauMuSelPtHistograms =cmgTauMuSelHistograms.clone()
cmgTauMuSelPtHistograms.inputCollection = cms.InputTag("cmgTauMuSelPt")
                                       

cmgTauMuSelEtaHistograms =cmgTauMuSelHistograms.clone()
cmgTauMuSelEtaHistograms.inputCollection = cms.InputTag("cmgTauMuSelEta")
                                       

cmgTauMuSelAgainstMuonHistograms =cmgTauMuSelHistograms.clone()
cmgTauMuSelAgainstMuonHistograms.inputCollection = cms.InputTag("cmgTauMuSelAgainstMuon")
                                       

cmgTauMuSelAgainstElectronHistograms =cmgTauMuSelHistograms.clone()
cmgTauMuSelAgainstElectronHistograms.inputCollection = cms.InputTag("cmgTauMuSelAgainstElectron")
                                       

cmgTauMuSelIsolationHistograms =cmgTauMuSelHistograms.clone()
cmgTauMuSelIsolationHistograms.inputCollection = cms.InputTag("cmgTauMuSelIsolation")
                                       

#cmgTauMuSelHistograms =cmgTauMuSelHistograms.clone()
#cmgTauMuSelHistograms.inputCollection = cms.InputTag("cmgTauMuSel")


cmgTauMuSelCleanHistograms =cmgTauMuSelHistograms.clone()
cmgTauMuSelCleanHistograms.inputCollection = cms.InputTag("cmgTauMuSelClean")
                                          


