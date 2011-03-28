import FWCore.ParameterSet.Config as cms

cmgGenericPOHistograms = cms.EDAnalyzer(
    "GenericPOHistograms",
    inputCollection = cms.InputTag("cmgPFJetSel"),
    histograms = cms.untracked.PSet(
        charge = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('charge()'),
               nbins = cms.untracked.int32(4),
               low = cms.untracked.double(-2),
               high = cms.untracked.double(2)
               )
            ),
        energy = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('energy()'),
               nbins = cms.untracked.int32(100),
               low = cms.untracked.double(0),
               high = cms.untracked.double(1000)
               )
            ),
        mass = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('mass()'),
               nbins = cms.untracked.int32(150),
               low = cms.untracked.double(0),
               high = cms.untracked.double(150)
               )
            ),
        pt = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('pt()'),
               nbins = cms.untracked.int32(100),
               low = cms.untracked.double(0),
               high = cms.untracked.double(500)
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
        eta = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('eta()'),
               nbins = cms.untracked.int32(50),
               low = cms.untracked.double(-5),
               high = cms.untracked.double(5)
               )
            )         
    )
    )
