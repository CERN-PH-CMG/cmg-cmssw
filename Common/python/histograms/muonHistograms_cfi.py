import FWCore.ParameterSet.Config as cms

muonHistograms = cms.EDAnalyzer(
                "GenericMuonHistograms",
                name = cms.string('MuonHistograms'),
                inputCollection = cms.InputTag("cmgMuon"),
                histograms = cms.PSet(
                             pt = cms.VPSet(
                                      cms.PSet( 
                                                var = cms.string('pt()'),
                                                low = cms.double(0),
                                                high = cms.double(100),
                                                nbins = cms.int32(100)
                                               )
                             ),
                             eta = cms.VPSet(
                                      cms.PSet( 
                                                var = cms.string('abs(eta())'),
                                                low = cms.double(0),
                                                high = cms.double(2.5),
                                                nbins = cms.int32(10)
                                               )
                             ),
                             pt_eta = cms.VPSet(
                                      cms.PSet( 
                                                var = cms.string('pt()'),
                                                low = cms.double(0),
                                                high = cms.double(100),
                                                nbins = cms.int32(100)
                                               ),
                                     cms.PSet( 
                                                var = cms.string('abs(eta())'),
                                                low = cms.double(0),
                                                high = cms.double(2.5),
                                                nbins = cms.int32(10)
                                               )
                             )          
                                       
                )
)