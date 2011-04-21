import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgMuon_cfi import cmgMuon
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

dimuonFactory = diObjectFactory.clone(
       leg1Collection = cms.InputTag("cmgMuonSel"),
       leg2Collection = cms.InputTag("cmgMuonSel"),
       metCollection = cms.InputTag("")
)
from CMGTools.Common.selections.razorbeta_cfi import razorbeta
from CMGTools.Common.selections.zmumu_cfi import zmumu
cmgDiMuon = cms.EDFilter(
    "DiMuonPOProducer",
    cfg = dimuonFactory.clone(),
    cuts = cms.PSet(
       razorbeta = razorbeta.clone(),           
       zmumu = zmumu.clone()
      ),
    )


