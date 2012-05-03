import FWCore.ParameterSet.Config as cms

from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
from CMGTools.External.puJetIDAlgo_cff import full_5x

jetanalyzer = cms.EDAnalyzer('JetAnalyzer',
                             
                             ## Loic's PATtuples 
                             MCPileupTag = cms.InputTag("addPileupInfo",""),  
                             JetTag      = cms.InputTag("selectedPatJetsPFlow",""),                        
                             GenJetTag   = cms.InputTag("ak5GenJets",""),
                             MuonTag     = cms.InputTag("selectedPatMuonsPFlow",""),
                             PVTag       = cms.InputTag("offlinePrimaryVertices",""),
                             dataFlag    = cms.untracked.bool(False),
                             computeTMVA = cms.untracked.bool(True),
                             
                             puJetIDAlgo = full_5x,
                             
                             # loose jets
                             pfjetIdLoose = pfJetIDSelector.clone(),                           
                             
                             # jet pt threshold
                             ## jetPtThreshold = cms.untracked.double(0.)                          
                             jetPtThreshold = cms.untracked.double(10.),                          
                             
                             #PFtoPAT ntuple (Colin)
                             #JetTag     = cms.InputTag("selectedPatJetsAK5",""),                        
                             #GenJetTag  = cms.InputTag("selectedPatJetsAK5","genJets"),
                             
                             MvaTags = cms.untracked.VInputTag(cms.InputTag("puJetMva","simpleDiscriminant"),
                                                               cms.InputTag("puJetMva","fullDiscriminant"),
                                                               cms.InputTag("puJetMva","cutbasedDiscriminant"),
                                                               ),
                             IdTags = cms.untracked.VInputTag( cms.InputTag("puJetMva","simpleId"),
                                                               cms.InputTag("puJetMva","fullId"),
                                                               cms.InputTag("puJetMva","cutbasedId"),
                                                               ),
                             
                             )
