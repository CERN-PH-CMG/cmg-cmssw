import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgBaseJet_cfi import baseJetFactory

from CMGTools.External.pujetidsequence_cff import stdalgos
algolables = [ a.label.value() for a in stdalgos ]

pfJetFactory = cms.PSet(
       inputCollection = cms.InputTag('patJetsWithVar'),
       baseJetFactory = baseJetFactory.clone(),
       useConstituents = cms.bool( True ),
       puVariables = cms.InputTag("puJetId"),
       puMvas = cms.VInputTag( cms.InputTag("cmgPUJetMva","%sDiscriminant" % a ) for a in algolables ),
       puIds = cms.VInputTag( cms.InputTag("cmgPUJetMva","%sId" % a ) for a in algolables ),
       )


from CMGTools.Common.selections.btaggedjet_cfi import *
from CMGTools.Common.selections.jetId_cfi import *

cmgPFJet = cms.EDFilter(
    "PFJetPOProducer",
    cfg = pfJetFactory.clone(),
    cuts = cms.PSet(
       tchp = trackCountingHighPurBJetTags.clone(),
       tche = trackCountingHighEffBJetTags.clone(),
       jp = jetProbabilityBJetTags.clone(),
       csv = combinedSecondaryVertexBJetTags.clone(),
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
