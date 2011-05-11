import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgBaseJet_cfi import baseJetFactory
pfJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJetsPFlow"),
       baseJetFactory = baseJetFactory.clone()
       )
from CMGTools.Common.selections.btaggedjet_cfi import trackCountingHighEffBJetTags
from CMGTools.Common.selections.kinematics_cfi import kinematics
from CMGTools.Common.selections.jetid_cfi import *

cmgPFJet = cms.EDFilter(
    "PFJetPOProducer",
    cfg = pfJetFactory.clone(),
    cuts = cms.PSet(
       jetKinematics = kinematics.clone(),
       btag = trackCountingHighEffBJetTags.clone(), 
       looseJetId = looseJetId.clone(),
       mediumJetId = mediumJetId.clone(),
       tightJetId = tightJetId.clone(),
       ),
    verbose = cms.untracked.bool( False )
)

# to test another jet ID, do something like this in your cfg:
# process.load('CMGTools.Common.selections.jetid_cfi')
# process.cmgPFJet.cuts.tightJetId = tightJetId.clone()
