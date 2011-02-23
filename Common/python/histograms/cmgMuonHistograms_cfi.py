import FWCore.ParameterSet.Config as cms

cmgMuonHistograms = cms.EDAnalyzer(
    "CmgMuonHistograms",
    inputCollection = cms.InputTag("cmgMuon"),
    histograms = cms.PSet(
       pt = cms.VPSet(
         cms.PSet( 
             var = cms.string('pt()'),
             nbins = cms.int32(100),
             low = cms.double(0),
             high = cms.double(200)
             )
         ),
       eta = cms.VPSet(
          cms.PSet( 
             var = cms.string('eta()'),
             nbins = cms.int32(50),
             low = cms.double(-3.),
             high = cms.double(3.)
             )
          ),
       phi = cms.VPSet(
          cms.PSet( 
             var = cms.string('phi()'),
             nbins = cms.int32(50),
             low = cms.double(-3.15),
             high = cms.double(3.15)
             )
          ),
       pt_eta = cms.VPSet(
          cms.PSet( 
             var = cms.string('pt()'),
             low = cms.double(0),
             high = cms.double(200),
             nbins = cms.int32(50)
             ),
          cms.PSet( 
             var = cms.string('abs(eta())'),
             low = cms.double(0),
             high = cms.double(2.5),
             nbins = cms.int32(10)
             )
          ),
       numberOfValidTrackerHits = cms.VPSet(
          cms.PSet( 
             var = cms.string('numberOfValidTrackerHits()'),
             low = cms.double(0),
             high = cms.double(50),
             nbins = cms.int32(25)
             )
          ),
       numberOfValidPixelHits = cms.VPSet(
          cms.PSet( 
             var = cms.string('numberOfValidPixelHits()'),
             low = cms.double(0),
             high = cms.double(15),
             nbins = cms.int32(15)
             )
          ),
       numberOfValidMuonHits = cms.VPSet(
          cms.PSet( 
             var = cms.string('numberOfValidMuonHits()'),
             low = cms.double(0),
             high = cms.double(50),
             nbins = cms.int32(50)
             )
          ),
        numberOfMatches = cms.VPSet(
          cms.PSet( 
             var = cms.string('numberOfMatches()'),
             low = cms.double(0),
             high = cms.double(10),
             nbins = cms.int32(10)
             )
          ),
       normalizedChi2 = cms.VPSet(
          cms.PSet( 
             var = cms.string('normalizedChi2()'),
             title = cms.untracked.string('Normalized #chi^{2}'),
             low = cms.double(0),
             high = cms.double(15),
             nbins = cms.int32(15)
             )
          ),
       dxy = cms.VPSet(
          cms.PSet( 
             var = cms.string('abs(dxy())'),
             low = cms.double(0),
             high = cms.double(1),
             nbins = cms.int32(20)
             )
          ),
       dz = cms.VPSet(
          cms.PSet( 
             var = cms.string('abs(dz())'),
             low = cms.double(0),
             high = cms.double(1),
             nbins = cms.int32(20)
             )
          ),
       absIso = cms.VPSet(
          cms.PSet( 
             var = cms.string('absIso()'),
             low = cms.double(0),
             high = cms.double(50),
             nbins = cms.int32(25)
             )
          ),
       relIso = cms.VPSet(
          cms.PSet( 
             var = cms.string('relIso()'),
             low = cms.double(0),
             high = cms.double(2),
             nbins = cms.int32(20)
             )
          ),
       absIso_pt = cms.VPSet(
          cms.PSet( 
             var = cms.string('absIso()'),
             low = cms.double(0),
             high = cms.double(2),
             nbins = cms.int32(5)
             ),
          cms.PSet( 
             var = cms.string('pt()'),
             low = cms.double(0),
             high = cms.double(200),
             nbins = cms.int32(100)
             ),
          ),           
       )
)
