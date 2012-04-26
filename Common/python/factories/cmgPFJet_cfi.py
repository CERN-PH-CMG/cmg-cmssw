import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgBaseJet_cfi import baseJetFactory
pfJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJetsAK5"),
       baseJetFactory = baseJetFactory.clone(),
       useConstituents = cms.bool( True ),
       puVariables = cms.InputTag("puJetId"),
       puMvas = cms.VInputTag(cms.InputTag("puJetMva","fullDiscriminant"),
                              cms.InputTag("puJetMva","simpleDiscriminant"),
                              cms.InputTag("puJetMva","philv1Discriminant"),
                              ),
       puIds = cms.VInputTag(cms.InputTag("puJetMva","fullId"),
                             cms.InputTag("puJetMva","simpleId"),
                             cms.InputTag("puJetMva","philv1Id"),
                             )
       )
from CMGTools.Common.selections.btaggedjet_cfi import trackCountingHighEffBJetTags
from CMGTools.Common.selections.jetId_cfi import *

cmgPFJet = cms.EDFilter(
    "PFJetPOProducer",
    cfg = pfJetFactory.clone(),
    cuts = cms.PSet(
       btag = trackCountingHighEffBJetTags.clone(),
       veryLooseJetId99 = veryLooseJetId99.clone(),
       veryLooseJetId95 = veryLooseJetId95.clone(),       
       veryLooseJetId95h0 = veryLooseJetId95h0.clone(),       
       veryLooseJetId95gamma = veryLooseJetId95gamma.clone(),       
       looseJetId = looseJetId.clone(),
       mediumJetId = mediumJetId.clone(),
       tightJetId = tightJetId.clone(),
       ),
    verbose = cms.untracked.bool( False )
)

# to test another jet ID, do something like this in your cfg:
# process.load('CMGTools.Common.selections.jetId_cfi')
# process.cmgPFJet.cuts.tightJetId = tightJetId.clone()
