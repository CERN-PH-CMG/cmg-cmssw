import FWCore.ParameterSet.Config as cms

from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector

jetanalyzer = cms.EDAnalyzer('JetAnalyzer',

 ## Loic's PATtuples 
 MCPileupTag = cms.InputTag("addPileupInfo",""),  
 JetTag      = cms.InputTag("selectedPatJetsPFlow",""),                        
 GenJetTag   = cms.InputTag("ak5GenJets",""),
 MuonTag     = cms.InputTag("selectedPatMuonsPFlow",""),
 PVTag       = cms.InputTag("offlinePrimaryVertices",""),
 dataFlag    = cms.untracked.bool(False),
 computeTMVA = cms.untracked.bool(True),
 impactParTkThreshold = cms.untracked.double(0.) ,
 tmvaWeights = cms.untracked.string("CMGTools/External/data/mva_JetID.weights.xml"),
 tmvaMethod  = cms.untracked.string("JetID"),                 

 # loose jets
 pfjetIdLoose = pfJetIDSelector.clone(),                           

 # jet pt threshold
 jetPtThreshold = cms.untracked.double(20.)                          
                              
 #PFtoPAT ntuple (Colin)
 #JetTag     = cms.InputTag("selectedPatJetsAK5",""),                        
 #GenJetTag  = cms.InputTag("selectedPatJetsAK5","genJets"),


)
