import FWCore.ParameterSet.Config as cms

cmgHemiHistograms = cms.EDAnalyzer(
    "CmgHemiHistograms",
    inputCollection = cms.InputTag("cmgHemi"),
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
        pt_eta = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('pt()'),
                low = cms.untracked.double(0),
                high = cms.untracked.double(200),
                nbins = cms.untracked.int32(50),
                title = cms.untracked.string("p_{T}")
                ),
            cms.untracked.PSet(
                var = cms.untracked.string('abs(eta())'),
                low = cms.untracked.double(0),
                high = cms.untracked.double(2.5),
                nbins = cms.untracked.int32(10),
                title = cms.untracked.string("|#eta|")
                )
            ),
        )
    )
