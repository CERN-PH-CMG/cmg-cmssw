import FWCore.ParameterSet.Config as cms

cmgDiHemiHistograms = cms.EDAnalyzer(
    "CmgDiHemiHistograms",
    inputCollection = cms.InputTag("cmgDiHemi"),
    histograms = cms.untracked.PSet(
        pt = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('pt()'),
                nbins = cms.untracked.int32(100),
                low = cms.untracked.double(0),
                high = cms.untracked.double(200),
                title = cms.untracked.string("p_{T}")
                )
            ),
        eta = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('eta()'),
                nbins = cms.untracked.int32(50),
                low = cms.untracked.double(-3.),
                high = cms.untracked.double(3.),
                title = cms.untracked.string("#eta")
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
        mass = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('mass()'),
                nbins = cms.untracked.int32(100),
                low = cms.untracked.double(0),
                high = cms.untracked.double(200),
                title = cms.untracked.string("M")
                )
            ),
        alphaT = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('alphaT()'),
                nbins = cms.untracked.int32(10),
                low = cms.untracked.double(0),
                high = cms.untracked.double(1),
                title = cms.untracked.string("#alpha_{T}")
                )
            ),
        betaR = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('betaR()'),
                nbins = cms.untracked.int32(10),
                low = cms.untracked.double(-1),
                high = cms.untracked.double(1),
                title = cms.untracked.string("#beta_{R}")
                )
            ),
        mR = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('mR()'),
                nbins = cms.untracked.int32(100),
                low = cms.untracked.double(0),
                high = cms.untracked.double(1000),
                title = cms.untracked.string("M_{R}")
                )
            ),
        mRT = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('mRT()'),
                nbins = cms.untracked.int32(100),
                low = cms.untracked.double(0),
                high = cms.untracked.double(1000),
                title = cms.untracked.string("M_{R_{T}}")
                )
            ),
        r = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('R()'),
                nbins = cms.untracked.int32(10),
                low = cms.untracked.double(0),
                high = cms.untracked.double(1),
                title = cms.untracked.string("R")
                )
            ),
        alphaT_r = cms.untracked.VPSet(
             cms.untracked.PSet(
                var = cms.untracked.string('alphaT()'),
                nbins = cms.untracked.int32(10),
                low = cms.untracked.double(0),
                high = cms.untracked.double(1),
                title = cms.untracked.string("#alpha_{T}")
                ),
             cms.untracked.PSet(
                var = cms.untracked.string('R()'),
                nbins = cms.untracked.int32(10),
                low = cms.untracked.double(0),
                high = cms.untracked.double(1),
                title = cms.untracked.string("R")
                )
            ),
        deltaPhi = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('deltaPhi(leg1().phi(), leg2().phi())'),
                nbins = cms.untracked.int32(10),
                low = cms.untracked.double(0),
                high = cms.untracked.double(3.13),
                title = cms.untracked.string("#Delta #phi(leg_{1}, leg_{2})")
                )
            )
        )
    )
