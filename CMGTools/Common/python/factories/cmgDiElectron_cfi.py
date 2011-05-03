import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

diElectronFactory = diObjectFactory.clone(
       leg1Collection = cms.InputTag("cmgElectronSel"),
       leg2Collection = cms.InputTag("cmgElectronSel"),
       metCollection = cms.InputTag("")
)
from CMGTools.Common.selections.razorbeta_cfi import razorbeta
from CMGTools.Common.selections.zee_cfi import zee
cmgDiElectron = cms.EDFilter(
    "DiElectronPOProducer",
    cfg = diElectronFactory.clone(),
    cuts = cms.PSet(
       razorbeta = razorbeta.clone(),             
       zee = zee.clone()
      ),
    )


