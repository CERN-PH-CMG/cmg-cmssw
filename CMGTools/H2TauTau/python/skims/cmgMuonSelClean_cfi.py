import FWCore.ParameterSet.Config as cms


##
cmgMuonSelPt = cms.EDFilter("CmgMuonSelector",src = cms.InputTag( "cmgMuonSel" ),
                           cut = cms.string( " pt() > 15.0" ))
cmgMuonSelEta = cms.EDFilter("CmgMuonSelector",src = cms.InputTag( "cmgMuonSelPt" ),
                            cut = cms.string( " -2.1 < eta() && eta() < 2.1 " ))
cmgMuonSelIsGlobalTracker = cms.EDFilter("CmgMuonSelector",src = cms.InputTag( "cmgMuonSelEta" ),
                            cut = cms.string( "isGlobal()==1 && isTracker()==1" ))
cmgMuonSelPixelHits = cms.EDFilter("CmgMuonSelector",src = cms.InputTag( "cmgMuonSelIsGlobalTracker" ),
                            cut = cms.string( " numberOfValidPixelHits() > 0" ))
cmgMuonSelTrackerHits = cms.EDFilter("CmgMuonSelector",src = cms.InputTag( "cmgMuonSelPixelHits" ),
                            cut = cms.string( " numberOfValidTrackerHits() > 10 " ))
cmgMuonSelChi2 = cms.EDFilter("CmgMuonSelector",src = cms.InputTag( "cmgMuonSelTrackerHits" ),
                            cut = cms.string( " normalizedChi2() < 10 " ))
cmgMuonSelVertex = cms.EDFilter("CmgMuonSelector",src = cms.InputTag( "cmgMuonSelChi2" ),
                            cut = cms.string( " -0.2 < dz() && dz() < 0.2 && -0.02 < dxy() && dxy() < 0.02" ))
#cmgMuonSel = cms.EDFilter("CmgMuonSelector",src = cms.InputTag( "cmgMuonSel" ),
#                            cut = cms.string( " " ))


##last selector makes no cuts, just to create a final list with always the same name.
cmgMuonSelClean = cms.EDFilter("CmgMuonSelector",src = cms.InputTag( "cmgMuonSelVertex" ),
                              cut = cms.string( "" ))




