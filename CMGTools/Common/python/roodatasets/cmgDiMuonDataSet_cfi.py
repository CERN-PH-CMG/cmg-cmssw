import FWCore.ParameterSet.Config as cms

cmgDiMuonDataSet = cms.EDAnalyzer(
    'CmgDiMuonDataSet',
    inputCollection = cms.InputTag("cmgDiMuonSel"),
    argset = cms.PSet(
        pt = cms.VPSet(
            cms.PSet( 
               var = cms.untracked.string('pt()'),
               title = cms.untracked.string('p_{T} GeV'),
               setrange = cms.untracked.bool(False)
               )
            ),
        mass = cms.VPSet(
            cms.PSet( 
               var = cms.untracked.string('mass()'),
               title = cms.untracked.string('Mass GeV'),
               low = cms.untracked.double(0),
               high = cms.untracked.double(150)
               )
            ),
        ),
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
        mass = cms.VPSet(
            cms.PSet( 
               var = cms.string('mass()'),
               nbins = cms.int32(100),
               low = cms.double(0),
               high = cms.double(200)
               )
            )
        )
    )