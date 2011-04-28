import FWCore.ParameterSet.Config as cms

cmgDiTauDataSet = cms.EDAnalyzer(
    'CmgDiTauDataSet',
    inputCollection = cms.InputTag("cmgDiTauSel"),
    argset = cms.untracked.PSet(
        pt = cms.untracked.PSet( 
               var = cms.untracked.string('pt()'),
               title = cms.untracked.string('p_{T} GeV'),
               setrange = cms.untracked.bool(False)
               ),
        leg1pt = cms.untracked.PSet( 
               var = cms.untracked.string('leg1().pt()'),
               title = cms.untracked.string('Leg 1 p_{T} GeV'),
               setrange = cms.untracked.bool(False)
               ),
        mass = cms.untracked.PSet( 
               var = cms.untracked.string('mass()'),
               title = cms.untracked.string('Mass GeV'),
               low = cms.untracked.double(0),
               high = cms.untracked.double(150)
               )
        ),
    categories = cms.untracked.PSet(
        masscut = cms.untracked.VPSet(
            cms.PSet(
               name = cms.untracked.string('signal'),      
               cut = cms.untracked.string('mass() >= 60 && mass() <= 120')
               ),
            cms.PSet(
               name = cms.untracked.string('sideband'),      
               cut = cms.untracked.string('mass() < 60 || mass() > 120')
               ),
            )
        ),
    histograms = cms.untracked.PSet(
        pt = cms.untracked.VPSet(
            cms.PSet( 
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
        mass = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('mass()'),
               nbins = cms.untracked.int32(100),
               low = cms.untracked.double(0),
               high = cms.untracked.double(200)
               )
            )
        )
    )
