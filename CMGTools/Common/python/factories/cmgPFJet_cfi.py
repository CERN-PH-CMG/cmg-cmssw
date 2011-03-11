import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgBaseJet_cfi import baseJetFactory
pfJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJetsPFlow"),
       baseJetFactory = baseJetFactory.clone()
       )
from CMGTools.Common.selections.kinematics_cfi import kinematics
from CMGTools.Common.selections.jetid_cfi import *

cmgPFJet = cms.EDFilter(
    "PFJetPOProducer",
    cfg = pfJetFactory.clone(),
    cuts = cms.PSet(
       looseJetId = looseJetId.clone()
       ),
    verbose = cms.untracked.bool( False )
)

# to test another jet ID, do something like this in your cfg:
# process.load('CMGTools.Common.selections.jetid_cfi')
# process.cmgPFJet.cuts.tightJetId = tightJetId.clone()
