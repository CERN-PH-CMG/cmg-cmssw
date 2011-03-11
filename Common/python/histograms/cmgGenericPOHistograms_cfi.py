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
        p = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('p()'),
               nbins = cms.untracked.int32(200),
               low = cms.untracked.double(0),
               high = cms.untracked.double(200)
               )
            ),
        energy = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('energy()'),
               nbins = cms.untracked.int32(200),
               low = cms.untracked.double(0),
               high = cms.untracked.double(200)
               )
            ),
        et = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('et()'),
               nbins = cms.untracked.int32(200),
               low = cms.untracked.double(0),
               high = cms.untracked.double(200)
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
        mt = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('mt()'),
               nbins = cms.untracked.int32(150),
               low = cms.untracked.double(0),
               high = cms.untracked.double(150)
               )
            ),
        px = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('px()'),
               nbins = cms.untracked.int32(200),
               low = cms.untracked.double(0),
               high = cms.untracked.double(200)
               )
            ),            
        py = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('py()'),
               nbins = cms.untracked.int32(200),
               low = cms.untracked.double(0),
               high = cms.untracked.double(200)
               )
            ),            
        pz = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('pz()'),
               nbins = cms.untracked.int32(200),
               low = cms.untracked.double(0),
               high = cms.untracked.double(200)
               )
            ),            
        pt = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('pt()'),
               nbins = cms.untracked.int32(200),
               low = cms.untracked.double(0),
               high = cms.untracked.double(200)
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
        theta = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('theta()'),
               nbins = cms.untracked.int32(50),
               low = cms.untracked.double(-3.15),
               high = cms.untracked.double(3.15)
               )
            ),
        eta = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('eta()'),
               nbins = cms.untracked.int32(50),
               low = cms.untracked.double(-3.15),
               high = cms.untracked.double(3.15)
               )
            ),              
        y = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('y()'),
               nbins = cms.untracked.int32(50),
               low = cms.untracked.double(-3.15),
               high = cms.untracked.double(3.15)
               )
            ),
        pt_eta = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('pt()'),
               nbins = cms.untracked.int32(200),
               low = cms.untracked.double(0),
               high = cms.untracked.double(200)
               ),
            cms.untracked.PSet( 
               var = cms.untracked.string('eta()'),
               nbins = cms.untracked.int32(50),
               low = cms.untracked.double(-3.15),
               high = cms.untracked.double(3.15)
               )
            ),                                
        )
    )
