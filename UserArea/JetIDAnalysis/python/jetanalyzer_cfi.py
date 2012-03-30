import FWCore.ParameterSet.Config as cms

from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
from CMGTools.External.puJetIDAlgo_cff import PhilV0

jetanalyzer = cms.EDAnalyzer('JetAnalyzer',

 ## Loic's PATtuples 
 MCPileupTag = cms.InputTag("addPileupInfo",""),  
 JetTag      = cms.InputTag("selectedPatJetsPFlow",""),                        
 GenJetTag   = cms.InputTag("ak5GenJets",""),
 MuonTag     = cms.InputTag("selectedPatMuonsPFlow",""),
 PVTag       = cms.InputTag("offlinePrimaryVertices",""),
 dataFlag    = cms.untracked.bool(False),
 computeTMVA = cms.untracked.bool(True),
                             
 puJetIDAlgo = PhilV0,

 # loose jets
 pfjetIdLoose = pfJetIDSelector.clone(),                           

 # jet pt threshold
 jetPtThreshold = cms.untracked.double(10.)                          
                              
 #PFtoPAT ntuple (Colin)
 #JetTag     = cms.InputTag("selectedPatJetsAK5",""),                        
 #GenJetTag  = cms.InputTag("selectedPatJetsAK5","genJets"),


)
