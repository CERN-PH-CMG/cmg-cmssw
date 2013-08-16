import FWCore.ParameterSet.Config as cms

from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
from CMGTools.External.puJetIDAlgo_cff import full_53x,full_53x_chs

jetsubstructureanalyzer = cms.EDAnalyzer('JetSubstructureAnalyzer',
                             
                                         ## Loic's PATtuples 
                                         MCPileupTag = cms.InputTag("addPileupInfo",""),  
                                         JetTag      = cms.InputTag("ak5PFJets",""),
                                         JetFlavTag  = cms.InputTag("AK5byValPhys",""),
                                         GenJetTag   = cms.InputTag("ak5GenJets",""),
                                         MuonTag     = cms.InputTag("muons",""),
                                         PVTag       = cms.InputTag("offlinePrimaryVertices",""),
                                         dataFlag    = cms.untracked.bool(False),
                                         computeTMVA = cms.untracked.bool(True),
                                         
                                         RhoTag = cms.InputTag("kt6PFJets","rho"),
                                         jecTag = cms.string("AK5PF"),
                                         applyJec = cms.bool(False),
                                         residualsFromTxt = cms.bool(False),
                                         residualsTxt     = cms.FileInPath("CMGTools/External/data/dummy.txt"),
                                         
                                         puJetIDAlgo = full_53x,
                                         
                                         # loose jets
                                         pfjetIdLoose = pfJetIDSelector.clone(),                           
                                         
                                         # jet pt threshold
                                         jetPtThreshold = cms.untracked.double(20.),                          
                                         #jetPtThreshold = cms.untracked.double(10.),                          
                                         
                                         #PFtoPAT ntuple (Colin)
                                         MvaTags = cms.untracked.VInputTag(#cms.InputTag("puJetMva","simpleDiscriminant"),
                                                                           cms.InputTag("puJetMva","full53xDiscriminant"),
                                                                           cms.InputTag("puJetMva","cutbasedDiscriminant"),
                                                                           ),
                                         IdTags = cms.untracked.VInputTag( #cms.InputTag("puJetMva","simpleId"),
                                                                           cms.InputTag("puJetMva","full53xId"),
                                                                           cms.InputTag("puJetMva","cutbasedId"),
                                                                           ),
                             
                                         )
