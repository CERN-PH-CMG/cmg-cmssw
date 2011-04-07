import FWCore.ParameterSet.Config as cms

# The four-vector sum of all jets does not tell us too much new, but
# it's nice to just make these histograms by default anyway.

cmgMultiJetHistograms = cms.EDAnalyzer(
    "CmgMultiObjectHistograms",
    inputCollection = cms.InputTag("cmgMultiJet"),
    histograms = cms.untracked.PSet(
        pt = cms.untracked.VPSet(
            cms.untracked.PSet(
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
                nbins = cms.untracked.int32(63),
                low = cms.untracked.double(-3.15),
                high = cms.untracked.double(3.15)
                )
            ),
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
            ),
        deltaPhiStar = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('deltaPhiStar()'),
                nbins = cms.untracked.int32(63),
                low = cms.untracked.double(0.),
                high = cms.untracked.double(3.15)
                )
            ),
        )
    )
