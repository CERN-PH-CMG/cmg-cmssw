import FWCore.ParameterSet.Config as cms

cmgMuonHistograms = cms.EDAnalyzer(
    "CmgMuonHistograms",
    inputCollection = cms.InputTag("cmgMuonSel"),
    histograms = cms.untracked.PSet(
       numberOfValidTrackerHits = cms.untracked.VPSet(
          cms.untracked.PSet( 
             var = cms.untracked.string('numberOfValidTrackerHits()'),
             low = cms.untracked.double(0),
             high = cms.untracked.double(50),
             nbins = cms.untracked.int32(25)
             )
          ),
       numberOfValidPixelHits = cms.untracked.VPSet(
          cms.untracked.PSet( 
             var = cms.untracked.string('numberOfValidPixelHits()'),
             low = cms.untracked.double(0),
             high = cms.untracked.double(15),
             nbins = cms.untracked.int32(15)
             )
          ),
       numberOfValidMuonHits = cms.untracked.VPSet(
          cms.untracked.PSet( 
             var = cms.untracked.string('numberOfValidMuonHits()'),
             low = cms.untracked.double(0),
             high = cms.untracked.double(50),
             nbins = cms.untracked.int32(50)
             )
          ),
        numberOfMatches = cms.untracked.VPSet(
          cms.untracked.PSet( 
             var = cms.untracked.string('numberOfMatches()'),
             low = cms.untracked.double(0),
             high = cms.untracked.double(10),
             nbins = cms.untracked.int32(10)
             )
          ),
       normalizedChi2 = cms.untracked.VPSet(
          cms.untracked.PSet( 
             var = cms.untracked.string('normalizedChi2()'),
             title = cms.untracked.string('Normalized #chi^{2}'),
             low = cms.untracked.double(0),
             high = cms.untracked.double(15),
             nbins = cms.untracked.int32(15)
             )
          ),
       dxy = cms.untracked.VPSet(
          cms.untracked.PSet( 
             var = cms.untracked.string('abs(dxy())'),
             low = cms.untracked.double(0),
             high = cms.untracked.double(1),
             nbins = cms.untracked.int32(100)
             )
          ),
       dz = cms.untracked.VPSet(
          cms.untracked.PSet( 
             var = cms.untracked.string('abs(dz())'),
             low = cms.untracked.double(0),
             high = cms.untracked.double(1),
             nbins = cms.untracked.int32(100)
             )
          ),
       absIso = cms.untracked.VPSet(
          cms.untracked.PSet( 
             var = cms.untracked.string('absIso()'),
             low = cms.untracked.double(0),
             high = cms.untracked.double(50),
             nbins = cms.untracked.int32(100)
             )
          ),
       relIso = cms.untracked.VPSet(
          cms.untracked.PSet( 
             var = cms.untracked.string('relIso()'),
             low = cms.untracked.double(0),
             high = cms.untracked.double(2),
             nbins = cms.untracked.int32(100)
             )
          ),
       )
    )
