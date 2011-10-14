import FWCore.ParameterSet.Config as cms

cmgGenericPOHistograms = cms.EDAnalyzer(
    "GenericPOHistograms",
    inputCollection = cms.InputTag("cmgPFJetSel"),
    histograms = cms.untracked.PSet(
        charge = cms.untracked.VPSet(
            cms.untracked.PSet(
               var = cms.untracked.string('charge()'),
               nbins = cms.untracked.int32(5),
               low = cms.untracked.double(-2.5),
               high = cms.untracked.double(2.5),
               title = cms.untracked.string("charge")
               )
            ),
        energy = cms.untracked.VPSet(
            cms.untracked.PSet(
               var = cms.untracked.string('energy()'),
               nbins = cms.untracked.int32(100),
               low = cms.untracked.double(0),
               high = cms.untracked.double(1000),
               title = cms.untracked.string("E")
               )
            ),
        mass = cms.untracked.VPSet(
            cms.untracked.PSet(
               var = cms.untracked.string('mass()'),
               nbins = cms.untracked.int32(150),
               low = cms.untracked.double(0),
               high = cms.untracked.double(150),
               title = cms.untracked.string("M")
               )
            ),
        pt = cms.untracked.VPSet(
            cms.untracked.PSet(
               var = cms.untracked.string('pt()'),
               nbins = cms.untracked.int32(100),
               low = cms.untracked.double(0),
               high = cms.untracked.double(500),
               title = cms.untracked.string("p_{T}")
               )
            ),
        phi = cms.untracked.VPSet(
            cms.untracked.PSet(
               var = cms.untracked.string('phi()'),
               nbins = cms.untracked.int32(50),
               low = cms.untracked.double(-3.15),
               high = cms.untracked.double(3.15),
               title = cms.untracked.string("#phi")
               )
            ),
        eta = cms.untracked.VPSet(
            cms.untracked.PSet(
               var = cms.untracked.string('eta()'),
               nbins = cms.untracked.int32(50),
               low = cms.untracked.double(-5),
               high = cms.untracked.double(5),
               title = cms.untracked.string("#eta")
               )
            ),
# added bi-dim hist (T.Camporesi) May 2 -2011
        pt_eta = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('pt()'),
               low = cms.untracked.double(0),
               high = cms.untracked.double(200),
               nbins = cms.untracked.int32(50)
               ),
            cms.untracked.PSet( 
               var = cms.untracked.string('abs(eta())'),
               low = cms.untracked.double(0),
               high = cms.untracked.double(2.5),
               nbins = cms.untracked.int32(10)
               )
            )
    )
    )
