import FWCore.ParameterSet.Config as cms

razorDiHemiHistograms = cms.EDAnalyzer(
    "CmgDiHemiHistograms",
    inputCollection = cms.InputTag("RazorDiHemiHadBox"),
    histograms = cms.untracked.PSet(
        pt = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('pt()'),
               nbins = cms.untracked.int32(100),
               low = cms.untracked.double(0),
               high = cms.untracked.double(1000)
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
               high = cms.untracked.double(1000)
               )
            ),
        alphaT = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('alphaT()'),
               nbins = cms.untracked.int32(10),
               low = cms.untracked.double(0),
               high = cms.untracked.double(1)
               )
            ),
        betaR = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('betaR()'),
               nbins = cms.untracked.int32(10),
               low = cms.untracked.double(-1),
               high = cms.untracked.double(1)
               )
            ),
        mR = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('mR()'),
               nbins = cms.untracked.int32(100),
               low = cms.untracked.double(0),
               high = cms.untracked.double(1000)
               )
            ),
        mRT = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('mRT()'),
               nbins = cms.untracked.int32(100),
               low = cms.untracked.double(0),
               high = cms.untracked.double(1000)
               )
            ),
        r = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('R()'),
               nbins = cms.untracked.int32(10),
               low = cms.untracked.double(0),
               high = cms.untracked.double(1)
               )
            ),
        alphaT_r = cms.untracked.VPSet(
             cms.untracked.PSet( 
               var = cms.untracked.string('alphaT()'),
               nbins = cms.untracked.int32(10),
               low = cms.untracked.double(0),
               high = cms.untracked.double(1)
               ),
             cms.untracked.PSet( 
               var = cms.untracked.string('R()'),
               nbins = cms.untracked.int32(10),
               low = cms.untracked.double(0),
               high = cms.untracked.double(1)
               )
            ),
        deltaPhi = cms.untracked.VPSet(
            cms.untracked.PSet( 
               var = cms.untracked.string('deltaPhi(leg1().phi(),leg2().phi())'),
               nbins = cms.untracked.int32(10),
               low = cms.untracked.double(0),
               high = cms.untracked.double(3.13)
               )
            ),
        )
    )
