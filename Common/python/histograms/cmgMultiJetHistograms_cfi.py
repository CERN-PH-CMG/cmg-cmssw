import FWCore.ParameterSet.Config as cms

cmgMultiJetHistograms = cms.EDAnalyzer(
    "CmgMultiObjectHistograms",
    inputCollection = cms.InputTag("cmgMultiJet"),
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
                nbins = cms.untracked.int32(63),
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
        pt_lead = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('leadPtr().pt()'),
                nbins = cms.untracked.int32(100),
                low = cms.untracked.double(0),
                high = cms.untracked.double(200),
                title = cms.untracked.string("p_{T, lead}")
                )
            ),
        eta_lead = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('leadPtr().eta()'),
                nbins = cms.untracked.int32(50),
                low = cms.untracked.double(-3.),
                high = cms.untracked.double(3.),
                title = cms.untracked.string("#eta_{lead}")
                )
            ),
        phi_lead = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('leadPtr().phi()'),
                nbins = cms.untracked.int32(63),
                low = cms.untracked.double(-3.15),
                high = cms.untracked.double(3.15),
                title = cms.untracked.string("#phi_{lead}")
                )
            ),
        pt_eta_lead = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('leadPtr().pt()'),
                low = cms.untracked.double(0),
                high = cms.untracked.double(200),
                nbins = cms.untracked.int32(50),
                title = cms.untracked.string("p_{T, lead}")
                ),
            cms.untracked.PSet(
                var = cms.untracked.string('abs(leadPtr().eta())'),
                low = cms.untracked.double(0),
                high = cms.untracked.double(2.5),
                nbins = cms.untracked.int32(10),
                title = cms.untracked.string("|#eta_{lead}|")
                )
            ),
        numConstituents = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('numConstituents()'),
                nbins = cms.untracked.int32(100),
                low = cms.untracked.double(0),
                high = cms.untracked.double(100),
                title = cms.untracked.string("N_{constituents}")
                )
            ),
        deltaPhiStar = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('deltaPhiStar()'),
                nbins = cms.untracked.int32(63),
                low = cms.untracked.double(0.),
                high = cms.untracked.double(3.15),
                title = cms.untracked.string("#Delta#phi^{*}")
                )
            ),
        pt_pt_lead = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('leadPtr().pt()'),
                low = cms.untracked.double(0),
                high = cms.untracked.double(200),
                nbins = cms.untracked.int32(50),
                title = cms.untracked.string("p_{T, lead}")
                ),
            cms.untracked.PSet(
                var = cms.untracked.string('pt()'),
                low = cms.untracked.double(0),
                high = cms.untracked.double(200),
                nbins = cms.untracked.int32(50),
                title = cms.untracked.string("p_{T}")
                )
            ),
        )
    )
