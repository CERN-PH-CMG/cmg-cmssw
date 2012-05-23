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

                             RhoTag = cms.InputTag("kt6PFJets","rho"),
                             jecTag = cms.string("AK5PF"),
                             applyJec = cms.bool(False),
                             residualsFromTxt = cms.bool(False),
                             residualsTxt     = cms.FileInPath("CMGTools/External/data/dummy.txt"),
                             
                             puJetIDAlgo = full_5x,
                             
                             # loose jets
                             pfjetIdLoose = pfJetIDSelector.clone(),                           
                             
                             # jet pt threshold
                             ## jetPtThreshold = cms.untracked.double(0.)                          
                             jetPtThreshold = cms.untracked.double(10.),                          
                             
                             #PFtoPAT ntuple (Colin)
                             MvaTags = cms.untracked.VInputTag(cms.InputTag("puJetMva","simpleDiscriminant"),
                                                               cms.InputTag("puJetMva","fullDiscriminant"),
                                                               cms.InputTag("puJetMva","cutbasedDiscriminant"),
                                                               ),
                             IdTags = cms.untracked.VInputTag( cms.InputTag("puJetMva","simpleId"),
                                                               cms.InputTag("puJetMva","fullId"),
                                                               cms.InputTag("puJetMva","cutbasedId"),
                                                               ),
                             
                             )
